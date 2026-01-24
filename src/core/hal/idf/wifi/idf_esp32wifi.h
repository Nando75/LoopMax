// hal/esp32/idf/HalWifi_ESP32_IDF.h
#pragma once

#include "core/hal/interfaces/IHal_Wifi.h"
#include <vector>
#include <string>
#include "esp_wifi.h"
#include <functional>

namespace LoopMax::Core::Hal {

                
            

            class idf_esp32wifi : public IHal_Wifi {
            public:
                idf_esp32wifi();
                

                // Mode
                bool setMode(WifiMode mode) override;
                WifiMode getMode() const override;

                // STA
                bool connect(const char* ssid = nullptr, const char* psw = nullptr) override;
                void disconnect() override { }
                bool isConnected() const override { return false; }

                // AP
                bool startAP(const char* ssid, const char* psw) override;
                void stopAP() override;

                // Scan
                void scanWifi() override;
                volatile bool scanDone() override { return _scanDone; };
                std::vector<WifiNetwork> getLocalWifi() const override;
                
                uint32_t CurrentIP() override { return 0; }
                std::string StringIP() override { return ""; }
                const char* Type() override { return "Wifi 2.4 GHz"; }
                std::string testIP() override { return ""; }


                void testConn(const std::string& ssid, const std::string& password) override { }
                volatile bool testDone() override { return true; };
                volatile bool testResult() override { return false; };
                int32_t CurrentRSSI() override { return 0; }
                bool internet() override { return false; }
                

            private:
                bool _scanDone = true;
                WifiMode _mode = WifiMode::AP;
                std::vector<WifiNetwork> _wifiList;
                esp_netif_t* _netif_sta = nullptr;
                esp_netif_t* _netif_ap  = nullptr;
                bool _netifInit = false;
                void _initNetifOnce();

                
                static void _eventHandler(
                    void* arg,
                    esp_event_base_t event_base,
                    int32_t event_id,
                    void* event_data
                );

            };


}



/*
//UTILIZZO CLASSE:
wifi.setMode(WifiMode::AP_STA);
wifi.startAP("LoopMAxAP", "87654321");
wifi.connect("SSID", "password");
wifi.scanWifi();

// nel loop / task
if (wifi.isScanDone()) {
    auto list = wifi.getLocalWifi();
}


*/