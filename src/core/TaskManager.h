
#include "core/hal/interfaces/IHal_Task.h"
#include "core/services/IService.h"
#include "core/services/time/TimeManager.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/serial/SerialManager.h"
#include "core/services/system/SystemManager.h"
#include "core/services/pins/PinsManager.h"
#include "core/services/reset/ResetManager.h"
#include "core/services/geo/GeoManager.h"
#include "core/services/update/UpdateManager.h"
#include "core/services/storage/StorageManager.h"
#include "core/services/config/ConfigManager.h"
#include "core/services/wifi/WifiManager.h"
#include "core/services/web/WebManager.h"
#include "core/services/https/HttpsManager.h"
#include "core/services/mqtts/MqttsManager.h"

#include "core/ServicesManager.h"

#include "common/ModuleContext.h"
#include "core/ModuleRegistry.h"

#include "core/SystemContext.h"

namespace LoopMax {
    

                        class TaskManager {

                            public:
                            TaskManager();

                            void start();
                            void stop();

                        #ifdef HAL_HAS_RTOS
                            // loop dei task
                            void taskMain();
                            void taskNetwork();
                            void taskMonitor();
                            void taskComponents();
                            // Monitor dei task
                            //void monitorTasks();
                        #endif
                            
                            private:
                               Core::SystemContext ctx;
                               Core::ModuleRegistry* mod;
                               
                                Core::ServicesManager srvMan;
                                Core::TimeManager  timeService;
                                Core::LogsManager  logsService;
                                Core::SerialManager  serialService;
                                Core::SystemManager  systemService;
                                Core::PinsManager   pinsService;
                                Core::ResetManager   resetService;
                                Core::GeoManager   geoService;
                                Core::UpdateManager   updateService;
                                Core::StorageManager   storageService;
                                Core::ConfigManager   configService;
                                Core::WifiManager   wifiService;
                                Core::WebManager   webService;
                                Core::HttpsManager   httpsService;
                                Core::MqttsManager  mqttsServices;
                                

                                LoopMax::Services::ModuleContext* mdx; 
                                                
                                Core::Hal::IHal_Task* task_main;
                                Core::Hal::IHal_Task* task_network;
                                Core::Hal::IHal_Task* task_monitor;
                                Core::Hal::IHal_Task* task_components;
                                bool modStarted = false;

                        };


 
}