
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_GeoInfo.h"
#include "common/iservices/IGeoInfo.h"
#include "core/SystemContext.h"

namespace LoopMax {
    namespace Core {
        using namespace Services;
        using namespace Hal;

            class GeoManager : public IService,  public IHal_GeoInfo, public IGeoInfo {
                public:
                    
                    GeoManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "geo"; }
                    const  char* icon() override { return "🌍"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override {}

                     // Inizializza il GPS
                    bool begin() override {return true; }

                    // Aggiorna i dati GPS (da chiamare periodicamente)
                    void update() override {}

                    // Ottieni latitudine e longitudine
                    double getLatitude() override { return _lat; }
                    void setLatitude(double lat) override { _lat = lat; }

                    double getLongitude() override { return _lon; }
                    void setLongitude(double lon) override { _lon = lon; }
                    
                    // Ottieni altitudine
                    double getAltitude() override { return _alt; }
                    void setAltitude(double alt) override { _alt = alt; }
                    
                    // Ottieni precisione della lettura
                    float getAccuracy() override { return _acc; }
                    void setAccuracy(float acc) override { _acc = acc; }
                    
                    // Controlla se il GPS ha un fix valido
                    bool hasFix() override { return _fix; }



                    std::string Continent() override { return _continent; }
                    void setContinent(std::string continent) override { _continent = continent; }
                    
                    std::string ContinentCode() { return _continentCode; }
                    void setContinentCode(std::string code) override { _continentCode = code; }

                    std::string Country() { return _country; }
                    void setCountry(std::string country) override { _country = country; }

                    std::string CountryCode() { return _countryCode; }
                    void setCountryCode(std::string code) override { _countryCode = code; }

                    std::string Region() { return _region; }
                    void setRegion(std::string region) override { _region = region; }

                    
                    std::string City() { return _city; }
                    void setCity(std::string city) override { _city = city; }

                                        

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    Hal::IHal_GeoInfo& _geo;
                    SystemContext* ctx = nullptr;

                    std::vector<const char*> _deps;
                    std::vector<WebCommand> _webCommands;
                    
                     double _lat = 0.0;
                     double _lon = 0.0;
                     double _alt = 0.0;
                     float _acc = 0.0;
                     bool _fix=false;
                     std::string _continent;
                     std::string _continentCode;
                     std::string _country;
                     std::string _countryCode;
                     std::string _region;
                     std::string _regionCode;
                     std::string _city;
                     std::string _postal;
                     std::string _capital;


                };


    }
}


