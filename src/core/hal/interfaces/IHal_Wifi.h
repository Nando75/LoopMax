#pragma once 
#include <cstdint>
#include <vector>
#include <string>
#include <common/types.h>
#include <functional>

namespace LoopMax::Core::Hal {
    
    using namespace Types;

    class IHal_Wifi {
        public:
            virtual ~IHal_Wifi() = default;



             // ---------------- Mode ----------------
            virtual bool setMode(WifiMode mode) = 0;
            virtual WifiMode getMode() const = 0;

            // ---------------- STA ----------------
            virtual bool connect(const char* ssid = nullptr, const char* psw = nullptr) = 0;
            virtual void disconnect() = 0;
            virtual bool isConnected() const = 0;

            // ---------------- AP ----------------
            virtual bool startAP(const char* ssid, const char* psw) = 0;
            virtual void stopAP() = 0;

            // ---------------- Async Scan ----------------
            virtual void scanWifi() = 0;  // async scan
            virtual volatile bool scanDone() = 0;
            virtual volatile bool testDone() = 0;
            virtual volatile bool testResult() = 0;

            // ---------------- Async Connection Test ----------------
            virtual void testConn(const std::string& ssid, const std::string& password) = 0;


            virtual uint32_t CurrentIP() = 0;
            virtual std::string StringIP() = 0;
            virtual const char* Type() = 0;
            virtual std::string testIP() = 0;

            virtual void begin(std::string hostname="LoopMax") = 0;
            virtual void tick() = 0;
            virtual bool dnsActive() = 0;
            virtual bool mdnsActive() = 0;
            virtual bool internet() = 0;
            virtual int32_t CurrentRSSI() = 0;
            
            virtual std::vector<WifiNetwork> getLocalWifi() const = 0;

            
            
        };
    

}
