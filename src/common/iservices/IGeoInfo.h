#pragma once 

namespace LoopMax::Services {

    class IGeoInfo {
    public:
             // Inizializza il GPS
            virtual bool begin() =0;  

            // Aggiorna i dati GPS (da chiamare periodicamente)
            virtual void update() =0;

            // Ottieni latitudine e longitudine
            virtual double getLatitude() =0;
            virtual void setLatitude(double lat) =0;

            virtual double getLongitude() =0;
            virtual void setLongitude(double lon) =0;
            
            // Ottieni altitudine
            virtual double getAltitude() =0;
            virtual void setAltitude(double alt) =0;
            
            // Ottieni precisione della lettura
            virtual float getAccuracy() =0;
            virtual void setAccuracy(float acc) =0;
            // Controlla se il GPS ha un fix valido
            virtual bool hasFix() =0;


            virtual std::string Continent() =0;
            virtual void setContinent(std::string continent) =0;

            virtual std::string ContinentCode() =0;
            virtual void setContinentCode(std::string code) =0;

            virtual std::string Country() =0;
            virtual void setCountry(std::string country) =0;

            virtual std::string CountryCode() =0;
            virtual void setCountryCode(std::string code) =0;

            virtual std::string Region() =0;
            virtual void setRegion(std::string region) =0;

            virtual std::string City() =0;
            virtual void setCity(std::string city) =0;

            
        

        virtual ~IGeoInfo() {}
    };

}
