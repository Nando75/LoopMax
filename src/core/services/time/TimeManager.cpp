#include "TimeManager.h"
#include "core/services/serial/SerialManager.h"
#include "core/services/system/SystemManager.h"
#include "core/services/logs/LogsManager.h"


//#include "sys/core/JsonManager.h"
//#include <esp_partition.h>



namespace LoopMax::Core {

                TimeManager::TimeManager()
                    : _timer(Hal::hal_timer()), _deps{}
                {}
                //IService
                void TimeManager::start() {}
                void TimeManager::loop() {
                /*
                    if (ctx->system.mode() == SystemMode::LAN &&
                        !_timer.isSyncronized())
                    {
                        _timer.checkInternetTime();
                        ctx->logs.write("Internet Time syncronized.", LogType::INFO, name(), icon());
                    }
                */      

                }

                void TimeManager::stop() {}
                 //ITime
                uint64_t TimeManager::millis() { return _timer.millis(); }; 
                uint64_t TimeManager::micros() { return _timer.micros();}

                int64_t TimeManager::Unix()  { return _timer.Unix(); }
                std::string TimeManager::Timezone()  { return _timer.Timezone(); }
                int64_t TimeManager::Tz_Offset()  { return _timer.Tz_Offset(); }

                void TimeManager::setUnix(int64_t unix)  { 
                    _timer.setUnix(unix); 
                    ctx->logs.write("Internet Time syncronized.", LogType::INFO, name(), icon());
                }

                void TimeManager::setTimezone(const std::string& tz)  { _timer.setTimezone(tz); }
                void TimeManager::setTzOffset(int64_t offset)  { _timer.setTzOffset(offset); }



                void TimeManager::registerEndpoints() { 
                    
                    if(ctx->system.mode()==SystemMode::AP)
                    {
                           _webCommands.push_back({
                                        "time/set",
                                        0x02,
                                        [this](IHttpContext& httpCtx){
                                           
                                            if(this->setTimeFromClient(httpCtx.getParam("unix"),
                                                                       httpCtx.getParam("timezone"),
                                                                       httpCtx.getParam("offset")))
                                            {

                                                httpCtx.send(200, "text/plain", std::to_string(this->Unix()));

                                            }
                                            else
                                            {
                                               httpCtx.send(400, "text/plain", "Invalid params");
                                            }
                                            
                                        }
                                    });




                    }


                }



                bool TimeManager::setTimeFromClient(std::string unixStr, std::string timezoneStr, std::string offsetStr)
                {
                        int64_t unix = 0;
                        int64_t offset = 0;
                        try {
                            unix = std::stoll(unixStr);
                            if (unix <= 0) {
                                ctx->logs.write("Unix from web not valid",Types::LogType::WARNING,this->name(),this->icon());
                                return false;
                            }
                        } catch (...) {
                            ctx->logs.write("Unix from web not valid",Types::LogType::WARNING,this->name(),this->icon());
                            return false;
                        }

                        // ✅ Validazione offset
                        try {
                            offset = std::stoll(offsetStr);
                        } catch (...) {
                            ctx->logs.write("Offset from web not valid",Types::LogType::WARNING,this->name(),this->icon());
                            return false;
                        }

                        // ✅ Validazione timezone
                        if (timezoneStr.empty()) {
                            ctx->logs.write("Timezone from web not valid",Types::LogType::WARNING,this->name(),this->icon());
                            return false;
                        }
                        this->setUnix(unix);
                        this->setTimezone(timezoneStr);
                        this->setTzOffset(offset);
                        return true;
                }
                
                


}