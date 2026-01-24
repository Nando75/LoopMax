// TaskManager.cpp
#include "TaskManager.h"
#include "core/hal/hal.h"
//#include "common/IModule.h"
#include "core/ModuleRegistry.h"
#include "core/SystemContext.h"

namespace LoopMax {
        //using namespace Modules;
    

                    // --- Monitor Tasks ---
                    #ifdef HAL_HAS_RTOS
                        #include "freertos/FreeRTOS.h"
                            #include "freertos/task.h"
                    #endif
                    
                            TaskManager::TaskManager()
                            :   srvMan(),
                                timeService(),
                                logsService(),
                                serialService(),
                                systemService(),
                                pinsService(),
                                resetService(),
                                geoService(),
                                updateService(),
                                storageService(),
                                configService(),
                                wifiService(),
                                webService(),
                                httpsService(),
                                mqttsServices(),
                                ctx(srvMan,timeService, logsService, serialService, systemService,pinsService,resetService,geoService,
                                        updateService,storageService,configService,wifiService,webService,httpsService,mqttsServices),

                                task_main(Core::Hal::createTask()),
                                task_network(Core::Hal::createTask()),
                                task_monitor(Core::Hal::createTask()),
                                task_components(Core::Hal::createTask())
                            { 


                                mdx = new LoopMax::Services::ModuleContext(
                                    srvMan,
                                    timeService,
                                    logsService,
                                    serialService,
                                    systemService,
                                    pinsService,
                                    resetService,
                                    geoService,
                                    updateService,
                                    configService,
                                    wifiService,
                                    webService,
                                    httpsService,
                                    mqttsServices
                                );

                                mod = &Core::ModuleRegistry::instance();
                                ctx.initModules(*mod);
                                
                                timeService.initContext(ctx);
                                logsService.initContext(ctx);
                                serialService.initContext(ctx);
                                systemService.initContext(ctx);
                                pinsService.initContext(ctx);
                                resetService.initContext(ctx);
                                geoService.initContext(ctx);
                                updateService.initContext(ctx);
                                storageService.initContext(ctx);
                                configService.initContext(ctx);
                                wifiService.initContext(ctx);
                                webService.initContext(ctx);
                                httpsService.initContext(ctx);
                                mqttsServices.initContext(ctx);
                               


                            }


                    #ifdef HAL_HAS_RTOS
                            // Task entry functions
                            static void main_task_entry(void* arg) {
                                static_cast<TaskManager*>(arg)->taskMain();
                            }
                            static void net_task_entry(void* arg) {
                                static_cast<TaskManager*>(arg)->taskNetwork();
                            }
                            static void mon_task_entry(void* arg) {
                                static_cast<TaskManager*>(arg)->taskMonitor();
                            }       
                            static void comp_task_entry(void* arg) {
                                static_cast<TaskManager*>(arg)->taskComponents();
                            }
                                

                            void TaskManager::taskMain() {

                                
                               
                                for (;;) {

                                    
                                     //Refresh services status;
                                    //ctx.srvMan->check(serialService);
                                    //loopService(pinsService);
                                    //loopService(updateService);
                                    //MODULE LOOP:
                                    //Core::ModuleRegistry::instance().loop(); 


                                    //TEST HTTPS:
                                    /*
                                    HttpsData req;
                                    req.Https_ShowData = true;
                                    req.Https_ApiUrl = "https://google.it";
                                    req.Https_Port = 80;
                                    req.Https_User = "";
                                    req.Https_Psw = "";
                                    req.Https_UseCA = false;     
                                    req.Https_CA = "";
                                    req.Https_TimeOut = 3000;   
                                    httpsService.begin(req);
                                    HttpsResponse resp =  httpsService.request("",WMethod::GET,"trxt/html");
                                    serialService.printLn(resp.body);
                                    */


                                    
                                     
                                    timeService.loop();

                                    vTaskDelay(pdMS_TO_TICKS(30));
                                }
                            }


                            void TaskManager::taskNetwork() {

                               
                                for (;;) {

                                    auto hw = uxTaskGetStackHighWaterMark(nullptr);
                                    if (hw < 200) {
                                        printf("STACK LOW %s: %u\n", pcTaskGetName(nullptr), hw);

                                    }


                                     wifiService.loop();
                                     webService.loop();

                                    vTaskDelay(pdMS_TO_TICKS(10));
                                }
                            }

                            void TaskManager::taskMonitor() {
                               
                                for (;;) {

                                  auto hw = uxTaskGetStackHighWaterMark(nullptr);
                                    if (hw < 200) {
                                        printf("STACK LOW %s: %u\n", pcTaskGetName(nullptr), hw);
                                    }

                                    
                                    serialService.loop();

                                    vTaskDelay(pdMS_TO_TICKS(100));
                                }
                            }

                            void TaskManager::taskComponents() {
                               

                                for (;;) {

                                    auto hw = uxTaskGetStackHighWaterMark(nullptr);
                                    if (hw < 200) {
                                        printf("STACK LOW %s: %u\n", pcTaskGetName(nullptr), hw);
                                    }
                                    
                                    resetService.loop();
                                    
                                    //MODULES LOOP
                                    mod->loop();
                                    
                                    vTaskDelay(pdMS_TO_TICKS(150));
                                }
                            }



                            void TaskManager::monitorTasks() {


                                for (;;) {
                                    
                                    auto hw = uxTaskGetStackHighWaterMark(nullptr);
                                    if (hw < 200) {
                                        printf("STACK LOW %s: %u\n", pcTaskGetName(nullptr), hw);
                                    }


                                    vTaskDelay(pdMS_TO_TICKS(150));
                                }
                               
                            }




                    #endif


                    void TaskManager::start() {

                                
                                srvMan.registerService(&timeService);
                                srvMan.registerService(&logsService);
                                srvMan.registerService(&serialService);
                                srvMan.registerService(&storageService);
                                srvMan.registerService(&pinsService);
                                srvMan.registerService(&configService);
                                srvMan.registerService(&updateService);
                                srvMan.registerService(&systemService);
                                srvMan.registerService(&resetService);
                                srvMan.registerService(&geoService);
                                srvMan.registerService(&wifiService);
                                srvMan.registerService(&webService);
                                srvMan.registerService(&httpsService);
                                srvMan.registerService(&mqttsServices);
                                
                                
                                                                
                                
                                logsService.start();
                                serialService.start();
                                systemService.start();
                                pinsService.start();
                                resetService.start();
                                geoService.start();
                                updateService.start();
                                storageService.start(); 
                                //LOAD moduleContext INTO Modules
                                mod->initModules(mdx);
                                configService.start();
                                wifiService.start();
                                webService.start();
                                httpsService.start();
                                mqttsServices.start();
                                //MODULES BEGIN
                                mod->begin();

                               //serialService.printLn(ctx.config.buildModulesJson());
                                
                                

                        
                        #ifdef HAL_HAS_RTOS
                            #if HAL_CORES == 2
                                // --- CONFIGURAZIONE 2 CORE ---
                                // Core 0: Tutto il traffico dati e radio (Network Stack dell'ESP32 è già qui)
                                task_network->start(net_task_entry, this, "net", 10480, 5, 0); // 10KB (WiFi/web/HTTP/MQTT)
                                
                                // Core 1: Logica applicativa, JSON e Hardware
                                task_main->start(main_task_entry, this, "main", 8192, 5, 1);       // 8KB (Config/Json)
                                task_monitor->start(mon_task_entry, this, "mon", 8192, 4, 1);      // 3KB (Serial/Logs)
                                task_components->start(comp_task_entry, this, "cmp", 8192, 4, 1);   // 4KB (Pins/Storage/Geo)

                            #else
                                // --- CONFIGURAZIONE 1 CORE ---
                                // Qui la priorità è vitale. Alziamo gli stack ma dobbiamo stare attenti alla Heap totale.
                                // Usiamo priorità diverse per forzare lo scheduler a dare precedenza al network.
                                
                                task_network->start(net_task_entry, this, "net", 8192, 6, 0);  // Priorità alta (6)
                                task_main->start(main_task_entry, this, "main", 8192, 5, 0);     // Priorità media (5)
                                task_monitor->start(mon_task_entry, this, "mon", 3072, 4, 0);    // Priorità bassa (4)
                                task_components->start(comp_task_entry, this, "cmp", 3072, 4, 0); // Priorità bassa (4)
                            #endif
                        #else
                            #pragma message(">>> TASK MANAGER | CORES: 1 | RTOS: NO <<<")
                        #endif
                    }

                    void TaskManager::stop() {
                        #ifdef HAL_HAS_RTOS
                            task_main->stop();
                            task_network->stop();
                            task_monitor->stop();
                            task_components->stop();
                        #else
                            hal::serial().printLn("No RTOS: stop() does nothing");
                        #endif
                    }

                    

                    void TaskManager::loopService(Core::IService* service) {
                        if (!service) return;
                        if(service->state() == Types::ServiceState::READY) service->loop();
                    }
                    


 
}