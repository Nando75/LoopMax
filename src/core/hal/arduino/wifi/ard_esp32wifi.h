// hal/esp32/arduino/HalWifi_ESP32_Arduino.h
#pragma once

#include "core/hal/interfaces/IHal_Wifi.h"
#include <vector>
#include <string>
#include <WiFi.h>
//#include <DNSServer.h>


namespace LoopMax::Core::Hal {

           

            class ard_esp32wifi : public IHal_Wifi {
            public:
                ard_esp32wifi(); // = default;
                ~ard_esp32wifi() override = default;

                // Mode
                bool setMode(WifiMode mode) override;
                WifiMode getMode() const override;

                // STA
                bool connect(const char* ssid = nullptr, const char* psw = nullptr) override;
                void disconnect() override;
                bool isConnected() const override;

                // AP
                bool startAP(const char* ssid, const char* psw) override;
                void stopAP() override;

                // Scan
                void scanWifi() override;
                volatile bool scanDone() override { return _scanDone; };

                void testConn(const std::string& ssid, const std::string& password) override;
                volatile bool testDone() override { return _testDone; };
                volatile bool testResult() override { return _testResult; };
                
                std::vector<WifiNetwork> getLocalWifi() const override;

                uint32_t CurrentIP() override;
                std::string StringIP() override;
                const char* Type() override { return "Wifi 2.4 GHz"; }

                void begin(std::string hostname="LoopMax") override {  _hostname=hostname; }
                void tick() override;
                bool dnsActive() override { return _dnsActive; }
                bool mdnsActive() override { return _mdnsActive; }
                std::string testIP() override;
                int32_t CurrentRSSI() override;
                bool internet() override { return false; }
                                

            private:
                IPAddress _ip;
                IPAddress _testIp;
                //DNSServer dns;
                std::string ipToString(IPAddress ip);
                //uint32_t _dnsLastTick = 0;
                //uint32_t _dnsInterval = 100; // ms
                

                std::string _hostname="LoopMax";
                volatile bool _scanDone = true;
                bool _dnsActive = false;
                unsigned long _dnsStart = 0;
                void startDns();
                void stopDns();

                bool _mdnsActive = false;

                WifiMode _mode = WifiMode::AP;
                std::vector<WifiNetwork> _wifiList;
                void initMdns();
                void stopMdns();


                //TEST CONNECTION:
                volatile bool _testDone = true;
                bool _testRunning = false;
                bool _testResult = false;
                uint32_t _testStart = 0;
                uint32_t _testTimeout = 5000;
                std::string _testSsid;
                std::string _testPassword;

                const char* _ssid;
                const char* _psw;

               

                
                

                //void handleScanDone(WiFiEvent_t event, WiFiEventInfo_t info);
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