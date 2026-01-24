#pragma once
#include "core/hal/interfaces/IHal_GeoInfo.h"



namespace LoopMax::Core::Hal {


    class idf_geoInfo : public IHal_GeoInfo {
        public:

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


         

        private:
            double _lat = 0.0;
            double _lon = 0.0;
            double _alt = 0.0;
            float _acc = 0.0;
            bool _fix=false;

            


    
    };

    

}
