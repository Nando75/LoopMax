#pragma once
#include <stdint.h>
// ======================================================
// INCLUDE BASE COMUNI (sempre disponibili)
// ======================================================
#ifndef NO_FACTORY_STL
    #include <functional>
    #include <map>
    #include <string>
#endif

#include "core/hal/interfaces/IHal_Task.h"
#include "core/hal/interfaces/IHal_Timer.h"
#include "core/hal/interfaces/IHal_Logs.h"
#include "core/hal/interfaces/IHal_System.h"
#include "core/hal/interfaces/IHal_Pins.h"
#include "core/hal/interfaces/IHal_Serial.h"
#include "core/hal/interfaces/IHal_Storage.h"
#include "core/hal/interfaces/IHal_Wifi.h"
#include "core/hal/interfaces/IHal_Web.h"
#include "core/hal/interfaces/IHal_GeoInfo.h"
#include "core/hal/interfaces/IHal_Update.h"
#include "core/hal/interfaces/IHal_Https.h"
#include "core/hal/interfaces/IHal_Mqtts.h"

namespace LoopMax {
    namespace Core {
            namespace Hal {

                
                //ITask& task(); //Singleton stessa istanza.
                IHal_Task* createTask(); //Istanze diverse. locali. -- usavo questa
                IHal_Timer& hal_timer();
                //IHal_Logs& logs();
                IHal_System& sys();
                IHal_Pins& pins();
                IHal_Serial& serial();
                IHal_Storage& storage();
                IHal_Wifi& wifi();
                IHal_Web& web();
                IHal_GeoInfo& geo();
                IHal_Update& update();
                IHal_Https& https();
                IHal_Mqtts& mqtts();


            }
    }
}