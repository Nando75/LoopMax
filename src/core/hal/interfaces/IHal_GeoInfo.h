#pragma once 
//#include "core/SystemContext.h"
//#include "common/types.h"

//using namespace LoopMax::Types;

namespace LoopMax::Core::Hal {
    

    class IHal_GeoInfo {
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

        //virtual void initContext(SystemContext* ctx) = 0;
        virtual ~IHal_GeoInfo() {}
    };

}
