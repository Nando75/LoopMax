
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Wifi.h"
#include "common/iservices/IWifi.h"
#include "core/SystemContext.h"

namespace LoopMax {
    namespace Core {
        using namespace Types;
        using namespace Hal;
        using namespace Services;

            class WifiManager : public IService,  public IHal_Wifi, public IWifi {
                public:
                    
                    WifiManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "wifi"; }
                    const  char* icon() override { return "🛜"; }
                    const std::vector<SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 

                    void registerEndpoints() override;
                    
                    //IWifi
                    volatile bool scanDone() override { return _wifi.scanDone(); } 
                    std::string getPublicIP() override { return _publicIp; }
                    std::string Isp() override { return _isp; }
                    bool internet() override { return _internet; }
                     
                    //IHal_Wifi
                    // ---------------- Mode ----------------
                     bool setMode(WifiMode mode) override;
                     WifiMode getMode() const override;

                    // ---------------- STA ----------------
                     bool connect(const char* ssid = nullptr, const char* psw = nullptr) override;
                     void disconnect() override;
                     bool isConnected() const override;

                    // ---------------- AP ----------------
                     bool startAP(const char* ssid, const char* psw) override;
                     void stopAP() override;

                    // ---------------- Scan ----------------
                     void scanWifi() override;  // async scan
                     std::vector<WifiNetwork> getLocalWifi() const override;

                     

                     uint32_t CurrentIP() override;
                     std::string StringIP() override;
                     const char* Type() override { return _wifi.Type(); }

                     std::string testIP() override { return _wifi.testIP(); }

                     void testConn(const std::string& ssid, const std::string& password) override {  _wifi.testConn(ssid,password); }
                     volatile bool testDone() override { return _wifi.testDone(); };
                     volatile bool testResult() override { return _wifi.testResult(); };

                    void begin(std::string hostname="LoopMax") override;
                    void tick() override;
                    bool dnsActive() override;
                    bool mdnsActive() override;
                    
                    int32_t CurrentRSSI() override { return _wifi.CurrentRSSI(); }
                    
                    
                    void Test() {}
                   
                private:
                    bool _scanCompleted = true;
                    void checkWifiScan();
                    void checkPublicIp();
                    ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<SystemMode> _modes = { SystemMode::AP, SystemMode::LAN };
                    std::vector<WebCommand> _webCommands;
                    IHal_Wifi& _wifi;
                    std::vector<const char*> _deps;
                    SystemContext* ctx = nullptr;
                    bool sincIP = false;
                    bool _internet = false;
                    std::string _publicIp;
                    std::string _isp;
                    std::string _api;
                    HttpsData httpsConfig;
                                




                };


    }
}


