// hal/esp32/arduino/ard_esp32wifi.cpp
#include "ard_esp32wifi.h"
#include <cstring>
#include <ESPmDNS.h>

namespace LoopMax::Core::Hal {

    
    ard_esp32wifi::ard_esp32wifi() {
               
                    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {

                         switch (event) {
                                case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
                                    ////Serial.println("[WiFi] AP client connected");
                                    //startDns();
                                    break;
                                case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
                                    //Serial.println("[WiFi] AP client disconnected");
                                    //stopDns();
                                    WiFi.reconnect();
                                    break;

                                case ARDUINO_EVENT_WIFI_STA_CONNECTED:
                                    //Serial.println("[WiFi] STA_CONNECTED");
                                    break;
                                case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                                    //Serial.print("[WiFi] GOT_IP: ");
                                    //Serial.println(WiFi.localIP());
                                    break;
                                case ARDUINO_EVENT_WIFI_STA_LOST_IP:
                                    //Serial.println("[WiFi] LOST_IP");
                                      break;
                                case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                                    //Serial.printf("[WiFi] DISCONNECTED reason=%d\n",info.wifi_sta_disconnected.reason);
                                    break;
                            }




                        // --- SCAN (già presente) ---
                        if (event == ARDUINO_EVENT_WIFI_SCAN_DONE)
                        {
                            int n = WiFi.scanComplete();
                            _wifiList.clear();
                            if (n > 0) {
                                for (int i = 0; i < n; i++) {
                                    WifiNetwork w;
                                    w.ssid = std::string(WiFi.SSID(i).c_str());
                                    w.rssi = WiFi.RSSI(i);
                                    _wifiList.push_back(w);
                                }
                            }
                            WiFi.scanDelete();
                            //WiFi.enableSTA(false);
                            //WiFi.disconnect(false);
                           _scanDone = true;
                        }

                        // --- TEST CONNECTION ---
                        if (_testRunning)
                        {
                            if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP)
                            {
                                _testIp = WiFi.localIP();
                                _testResult = true;
                                _testDone = true;
                                _testRunning = false;
                                //WiFi.disconnect(false);
                                //WiFi.enableSTA(false);

                            }

                            // ❌ NON segnare fallimento qui
                            // else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED)
                        }

                });




      }
            


            void ard_esp32wifi::testConn(const std::string& ssid,
                                          const std::string& password)
            {
                if (_testRunning || ssid.empty()) return;
                _testRunning = true;
                _testDone = false;
                _testResult = false;
                _testStart = millis();

                
                WiFi.begin(ssid.c_str(), password.c_str()); // NON tocca AP
            }


                bool ard_esp32wifi::setMode(WifiMode mode) {
                    _mode = mode;
                    switch(mode) {
                        case WifiMode::AP: 
                            WiFi.mode(WIFI_AP); 
                            break;
                        case WifiMode::STA: 
                            WiFi.mode(WIFI_STA); 
                            break;
                        case WifiMode::AP_STA: 
                            WiFi.mode(WIFI_AP_STA); 
                            break;
                    }
                    return true;
                }

                WifiMode ard_esp32wifi::getMode() const {
                    return _mode;
                }

              bool ard_esp32wifi::connect(const char* ssid, const char* psw) {
                    _mode = WifiMode::STA;
                    WiFi.persistent(false);
                    WiFi.mode(WIFI_STA);

                    if(ssid) {
                        WiFi.begin(ssid, psw); // STA connection
                        
                    }
                    /*
                    bool res = WiFi.status() == WL_CONNECTED;
                    if(res) 
                    {
                        _ip = WiFi.localIP();
                        
                    }
                    */
                   
                   initMdns();
                   return WiFi.status() == WL_CONNECTED;

                }

                void ard_esp32wifi::disconnect() {
                    if(_mode != WifiMode::AP) WiFi.disconnect();
                     stopMdns();
                }

                /*
               bool ard_esp32wifi::isConnected() const {
                    if (_mode == WifiMode::STA) return WiFi.status() == WL_CONNECTED;
                    if (_mode == WifiMode::AP) return WiFi.softAPIP() != IPAddress();       // AP pronto
                    if (_mode == WifiMode::AP_STA) return WiFi.softAPIP() != IPAddress();   // AP parte, STA opzionale
                    return false;
                }
                */

                bool ard_esp32wifi::isConnected() const {
                    if (_mode == WifiMode::STA)
                        return WiFi.status() == WL_CONNECTED;

                    if (_mode == WifiMode::AP)
                        return WiFi.softAPIP() != IPAddress();

                    if (_mode == WifiMode::AP_STA)
                        return WiFi.status() == WL_CONNECTED || WiFi.softAPIP() != IPAddress();

                    return false;
                }


                bool ard_esp32wifi::startAP(const char* ssid, const char* psw)
                {
                    _ssid = ssid;
                    _psw = psw;
                    WiFi.persistent(false);
                    WiFi.mode(WIFI_AP_STA);
                    //WiFi.enableSTA(false);
                    //WiFi.enableAP(true);

                    //WiFi.mode(WIFI_AP);
                    _mode = WifiMode::AP_STA;
                    IPAddress ip(192,168,4,1);
                    IPAddress gw(192,168,4,1);
                    IPAddress sn(255,255,255,0);
                    WiFi.softAPConfig(ip, gw, sn);
                    
                    
                    if (!WiFi.softAP(ssid, psw,1,0,1)) return false;

                    _ip = WiFi.softAPIP();
                    if (_ip == IPAddress()) return false;
                    return true;
                }


              // Stop AP
                void ard_esp32wifi::stopAP() {
                    if(_mode == WifiMode::STA) return;
                    WiFi.softAPdisconnect(true);
                    stopDns();
                    //stopMdns();
                }

              void ard_esp32wifi::scanWifi() {
                _scanDone = false;
                delay(50);   
                WiFi.scanDelete();
                WiFi.scanNetworks(true);
            }

            std::vector<WifiNetwork> ard_esp32wifi::getLocalWifi() const {
                return _wifiList;
            }

            uint32_t ard_esp32wifi::CurrentIP() {
                
                if (_mode == WifiMode::AP || _mode == WifiMode::AP_STA) {
                    _ip = WiFi.softAPIP();
                } else {
                    _ip = WiFi.localIP();
                }
                                               
                // Converti in uint32_t
                return ((uint32_t)_ip[0] << 24) |
                    ((uint32_t)_ip[1] << 16) |
                    ((uint32_t)_ip[2] << 8)  |
                    ((uint32_t)_ip[3]);
            }

            std::string ard_esp32wifi::StringIP()
            {
                if (_mode == WifiMode::AP || _mode == WifiMode::AP_STA) {
                    _ip = WiFi.softAPIP();
                } else {
                    _ip = WiFi.localIP();
                }

                return this->ipToString(_ip);
            }

             std::string ard_esp32wifi::testIP()
            {
                return this->ipToString(_testIp);
            }

            std::string ard_esp32wifi::ipToString(IPAddress ip)
            {
                // Converti in uint32_t
                uint32_t intIP = ((uint32_t)ip[0] << 24) |
                    ((uint32_t)ip[1] << 16) |
                    ((uint32_t)ip[2] << 8)  |
                    ((uint32_t)ip[3]);

                uint8_t b0 = (intIP >> 24) & 0xFF;
                uint8_t b1 = (intIP >> 16) & 0xFF;
                uint8_t b2 = (intIP >> 8) & 0xFF;
                uint8_t b3 = intIP & 0xFF;
                char buf[16];
                snprintf(buf, sizeof(buf), "%u.%u.%u.%u", b0, b1, b2, b3);
                return std::string(buf);
            }

            
            void ard_esp32wifi::tick()
            {
             
                /*
                if (_dnsActive) {
                    
                    if (millis() - _dnsStart > 30000) {
                        stopDns();
                    } else {
                        dns.processNextRequest();
                    }
                    

                    dns.processNextRequest();
                }
                */




                 if (_testRunning && millis() - _testStart > _testTimeout) 
                 {
                    _testRunning = false;
                    _testDone = true;
                    _testResult = false;
                    //WiFi.disconnect(false);
                    //WiFi.enableSTA(false);
                    WiFi.setAutoReconnect(false);
                 }
                
                  
            }


            void ard_esp32wifi::startDns()
            {
                if (_dnsActive) return;
                /*
                _ip = WiFi.softAPIP();
                if (_ip == IPAddress()) return;
                
                dns.start(53, "*", _ip);
                _dnsActive = true;
                _dnsStart = millis();
                //Serial.println("[DNS CAPTIVE] STARTED");
                */
            }

              //SOLO AP E AP_STA
            void ard_esp32wifi::stopDns()
            {
                if (!_dnsActive) return;
                /*
                dns.stop();
                _dnsActive = false;
                   //Serial.println("[DNS CAPTIVE] STOPPED");
                   */
                
            }


            //SEMPRE
            void ard_esp32wifi::initMdns()
            {
                if(_hostname.empty()) return;
                if (MDNS.begin(_hostname.c_str())) {
                    MDNS.addService("http", "tcp", 80);    
                    _mdnsActive = true;
                    //Serial.println("[MDNS] STARTED");
                }
                
            }

           

            //SEMPRE
            void ard_esp32wifi::stopMdns()
            {
              if (!_mdnsActive) return;
              MDNS.end();
              _mdnsActive = false;
              //Serial.println("[MDNS] STOPPPED");
            }


            int32_t ard_esp32wifi::CurrentRSSI() {

                // STA o AP+STA → RSSI dell'AP esterno (LAN)
                if (_mode == WifiMode::STA || _mode == WifiMode::AP_STA) {
                    return WiFi.RSSI();   // dBm (es: -45)
                }

                // AP puro → RSSI non disponibile in modo affidabile
                if (_mode == WifiMode::AP) {
                    return 0; // oppure -127 per "non disponibile"
                }

                return 0;
            }

            

}