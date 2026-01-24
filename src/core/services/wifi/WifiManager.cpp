#include "WifiManager.h"
#include "BaseConfig.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/config/ConfigManager.h"
#include "core/services/https/HttpsManager.h"
#include "core/services/geo/GeoManager.h"
#include "core/services/serial/SerialManager.h"


namespace LoopMax::Core {
    using namespace Hal;

                WifiManager::WifiManager()
                    : _wifi(Hal::wifi()), _deps{"logs","conf"}
                {}

                //IService
                void WifiManager::start() {
                    const Settings& settings = ctx->config.settings();
                        if(settings.mode == SystemMode::AP) {
                            _wifi.startAP(settings.apSsid.c_str(), settings.apPsw.c_str());
                            _scanCompleted = false;
                            _wifi.scanWifi();
                            ctx->logs.write("AP Mode Started", LogType::DEBUG, name(), icon());
                            // In AP mode siamo subito READY perché l'access point è attivo
                            currentState = ServiceState::READY; 
                        } 
                        else {
                            _api = settings.ApiUrl;
                            begin(settings.apSsid);
                            _wifi.connect(settings.wfSsid.c_str(), settings.wfPsw.c_str());
                            ctx->logs.write("Connecting to LAN...", LogType::DEBUG, name(), icon());
                            // Siamo in BUSY finché non otteniamo l'IP
                            currentState = ServiceState::INITIALIZING; 
                            
                        }

                        ctx->logs.write("Wifi Ready", LogType::DEBUG, name(), icon());
                }

                   void WifiManager::loop() {
                            // 1. Il solito guardiano
                            
                            if (currentState == ServiceState::STOPPED) return;

                            // 2. Frequenza di controllo (non serve esagerare con il WiFi)
                            static uint32_t lastWifiCheck = 0;
                            if (millis() - lastWifiCheck < 500) return; 
                            lastWifiCheck = millis();

                            // 3. Logica di monitoraggio connessione
                            bool connected = _wifi.isConnected();

                            if (connected && currentState != ServiceState::READY) {
                                // Siamo appena passati da BUSY a READY!
                                currentState = ServiceState::READY;
                                ctx->logs.write("Wifi Connected!", LogType::INFO, name(), icon());
                            } 
                            else if (!connected && currentState == ServiceState::READY) {
                                // Abbiamo perso la connessione!
                                currentState = ServiceState::ERROR;
                                ctx->logs.write("Wifi Link Lost", LogType::WARNING, name(), icon());
                            }
                            // Esegue i task della HAL (DNS, MDNS, Scan)
                            checkWifiScan();
                            tick();
                            checkPublicIp();
                    }

                    
                void WifiManager::stop() { 
                    _wifi.disconnect();
                    _wifi.stopAP();
                    currentState = ServiceState::STOPPED; // Coerente con il Manager
                }




                //IWifi
                    // ---------------- Mode ----------------
                     bool WifiManager::setMode(WifiMode mode) { 


                        return _wifi.setMode(mode);
                      }

                     WifiMode WifiManager::getMode() const { 

                        return _wifi.getMode();
                      }

                    // ---------------- STA ----------------
                     bool WifiManager::connect(const char* ssid, const char* psw) { 
                        return _wifi.connect(ssid,psw);
                      }

                     void WifiManager::disconnect() { 
                         _wifi.disconnect();
                      }

                     bool WifiManager::isConnected() const { 
                        return _wifi.isConnected();
                      }

                    // ---------------- AP ----------------
                     bool WifiManager::startAP(const char* ssid, const char* psw) { 
                            return _wifi.startAP(ssid, psw);
                      }

                     void WifiManager::stopAP() { 
                             _wifi.stopAP();
                      }

                    // ---------------- Scan ----------------
                     void WifiManager::scanWifi() { 

                            _wifi.scanWifi();
                      }  // async scan

                     std::vector<WifiNetwork> WifiManager::getLocalWifi() const { 
                        return _wifi.getLocalWifi();
                      }

               
                        void WifiManager::checkWifiScan() {
                            if (!_scanCompleted && _wifi.scanDone()) {
                                //if(!IS_DEBUG) return;
                                _scanCompleted = true;
                                auto list = _wifi.getLocalWifi();
                                // 1. Messaggio di riepilogo
                                std::string summary = "Wifi scan completed, found " + std::to_string(list.size()) + " networks";
                                ctx->logs.write(summary.c_str(), LogType::INFO, this->name(), this->icon());
                                // 2. Loop sulle reti trovate
                                for (size_t i = 0; i < list.size(); ++i) {
                                    const auto& net = list[i];
                                    // Gestione SSID nascosto
                                    std::string ssidName = net.ssid.empty() ? "<hidden>" : net.ssid;
                                    // Costruzione stringa report con std::string (molto più sicura di snprintf su ESP32 multi-core)
                                    std::string report = "[" + std::to_string(i) + "] SSID: " + ssidName + 
                                                        " | RSSI: " + std::to_string(net.rssi);
                                    // Passiamo il c_str(), LogsManager ne farà una copia profonda nel suo std::vector<LogEntry>
                                    ctx->logs.write(report.c_str(), LogType::INFO, this->name(), this->icon());
                                }
                            }
                        }
                    
                    
                    uint32_t WifiManager::CurrentIP() {  return _wifi.CurrentIP(); }  
                    std::string WifiManager::StringIP() {  return _wifi.StringIP(); }  

                    void WifiManager::begin(std::string hostname) { _wifi.begin(hostname); }

                    
                    bool WifiManager::dnsActive()  { return _wifi.dnsActive(); }
                    bool WifiManager::mdnsActive() { return _wifi.mdnsActive(); }
                    void WifiManager::tick() { _wifi.tick(); }


                    void WifiManager::registerEndpoints() { 
                        _webCommands.push_back({"wifi/scan",0x01,
                                [this](IHttpContext& httpCtx) {
                                    if(this->_scanCompleted && this->_wifi.scanDone())
                                    {
                                        this->_scanCompleted = false;
                                        this->_wifi.scanWifi();
                                        ctx->logs.write("Scan local wifi ...", LogType::DEBUG, this->name(), this->icon());
                                    }
                                   httpCtx.send(200, "application/json", R"({"status":"Scanning"})");
                                }
                            });


                    _webCommands.push_back({"wifi/results",0x01,
                                [this](IHttpContext& httpCtx) {
                                    if (!this->_scanCompleted && !_wifi.scanDone()) {
                                        httpCtx.send(200, "application/json", R"({"status":"Scanning"})");
                                        return;
                                    }
                                    ctx->logs.write("Scan completed.", LogType::DEBUG, this->name(), this->icon());
                                    httpCtx.send(200,"application/json",this->ctx->config.buildWifiJson());
                                }
                            });



                         _webCommands.push_back({"wifi/test",0x02,
                                [this](IHttpContext& httpCtx) {
                                    
                                    if (this->_wifi.testDone()) {
                                        std::string ssid = httpCtx.getParam("ssid");
                                        std::string psw = httpCtx.getParam("psw");
                                        this->_wifi.testConn(ssid, psw);
                                        ctx->logs.write("Test connection, ssid:" + ssid + ", psw: " + psw, LogType::DEBUG, this->name(), this->icon());
                                    }
                                   httpCtx.send(200, "application/json", R"({"status":"Testing"})");
                                }

                          });

                          _webCommands.push_back({"wifi/testResult", 0x01,
                                [this](IHttpContext& httpCtx) {
                                    if (!this->_wifi.testDone()) {
                                        httpCtx.send(200, "application/json",R"({"status":"Testing"})");
                                        return;
                                    }
                                    // Test finito → controlla risultato
                                    if (this->_wifi.testResult()) {
                                        // SUCCESSO
                                        std::string ip = this->_wifi.testIP();
                                        std::string res = std::string("{\"status\":\"Success\",\"ip\":\"") + ip + "\"}";
                                        ctx->logs.write("Test completed", LogType::DEBUG, this->name(), this->icon(),res);
                                        httpCtx.send(200, "application/json", res);
                                    } 
                                    else {
                                        ctx->logs.write("Test completed", LogType::DEBUG, this->name(), this->icon(),R"({"status":"Failed"})");
                                        httpCtx.send(200, "application/json",R"({"status":"Failed"})");
                                    }
                                }
                            });

                    }

                    /*
                    void WifiManager::checkPublicIp()
                    {

                        if(this->sincIP || !_wifi.isConnected() || ctx->system.mode() != SystemMode::LAN)
                        { return; }
                        //httpsConfig.Https_ApiUrl   = "api.ipify.org";
                        httpsConfig.Https_ApiUrl   = SERVER_INFO;
                        
                        httpsConfig.Https_Port     = 443;
                        httpsConfig.Https_UseCA    = false;
                        httpsConfig.Https_TimeOut  = 5000;

                        // 4) Richiesta (torna HttpsResponse)
                        auto res = ctx->https.request(httpsConfig, "", WMethod::GET, "");

                        // 6) Controllo codice HTTP
                        if (res.code != 200) {
                            ctx->logs.write("Https request failed", LogType::WARNING, name(), icon());
                            return;
                        }

                        // 7) Controllo body
                        if (res.body.empty()) {
                            ctx->logs.write("Wrong public ip", LogType::WARNING, name(), icon());
                            return;
                        }


                                                                       
                       _publicIp = res.body;
                       this->sincIP = true;
                       if(!_publicIp.empty() && _publicIp.length()>6) _internet = true;
                       ctx->logs.write("Internet is Active ! public ip: " + res.body, LogType::INFO, name(), icon());

                    }
                    */

                    void WifiManager::checkPublicIp()
                    {
                        if (this->sincIP || !_wifi.isConnected() || ctx->system.mode() != SystemMode::LAN)
                            return;
                        if(_api.empty() || _api.length()<=0) return;
                        // Configurazione HTTPS
                        httpsConfig.Https_ApiUrl   = _api;
                        httpsConfig.Https_Port   = 0;
                        httpsConfig.Https_UseCA    = false;
                        httpsConfig.Https_TimeOut  = 5000;

                        // Richiesta GET verso /
                        //auto res = ctx->https.request(httpsConfig, "/", WMethod::GET, "");
                        auto res = ctx->https.request(httpsConfig,R"({"command":"getPublicInfo"})",Types::WMethod::POST,"application/json");
                       
                        // Controllo codice HTTP
                        if (res.code != 200) {
                            ctx->logs.write("PublicIP: HTTP request failed", LogType::WARNING, name(), icon());
                            return;
                        }

                        // Controllo body
                        if (res.body.empty()) {
                            ctx->logs.write("PublicIP: empty body", LogType::WARNING, name(), icon());
                            return;
                        }

                        // -----------------------------
                        // PARSING JSON
                        // -----------------------------
                        JsonDocument doc;
                        auto err = deserializeJson(doc, res.body);

                        if (err) {
                            ctx->logs.write("PublicIP: JSON parse error", LogType::WARNING, name(), icon());
                            return;
                        }

                        // Controllo stato
                        bool success = doc["success"] | false;
                        if (!success) {
                            ctx->logs.write("PublicIP: API returned failure", LogType::WARNING, name(), icon());
                            return;
                        }

                        //Set Time
                        ctx->time.setUnix(doc["data"]["api"]["time"]|0);
                        ctx->time.setTimezone(doc["data"]["timezone"]["name"] | "");
                        ctx->time.setTzOffset(doc["data"]["timezone"]["gmtOffset"] | 0);
                        //Api info
                        ctx->system.setApi(doc["data"]["api"]["name"] | "");
                        ctx->system.setApiVersion(doc["data"]["api"]["version"] | "");
                        //Geo Info
                        ctx->geo.setLatitude(doc["data"]["geo"]["latitude"] | 0.0);
                        ctx->geo.setLongitude(doc["data"]["geo"]["longitude"] | 0.0   );
                        ctx->geo.setContinent(doc["data"]["geo"]["continent"] | ""    );
                        ctx->geo.setContinentCode(doc["data"]["geo"]["continent_code"] | ""    );
                        ctx->geo.setCountry(doc["data"]["geo"]["country"] | ""    );
                        ctx->geo.setCountryCode(doc["data"]["geo"]["country_code"] | ""    );
                        ctx->geo.setRegion(doc["data"]["geo"]["region"] | ""    );
                        ctx->geo.setCity(doc["data"]["geo"]["city"] | ""    );
                            
                        //Net Time
                        _publicIp = doc["data"]["client"]["ip"] | "";
                        _isp = doc["data"]["network"]["isp"] | "";

                        this->sincIP = true;

                        if (!_publicIp.empty() && _publicIp.length() > 6)
                            _internet = true;
                    }


                





}