#include "Zappy2.h"
//AVALAIBLE SERVICES src/common ...
#include "common/tools/Helper.h"
#include "common/iservices/ISerial.h"
#include "common/iservices/ILogs.h"
#include "common/iservices/IConfig.h"
#include "common/iservices/IHttps.h"
#include "common/iservices/IMqtts.h"

using namespace LoopMax::Modules;
using namespace LoopMax::Types;
using namespace LoopMax::Utils;

// ─────────────────────────────
// Constructor
// ─────────────────────────────
Zappy2::Zappy2() = default;

// ─────────────────────────────
// INIT:(LOAD MODULE DATA TO CORE, LOGS REGISTRY, WEB COMMANDS REGISTRY, ECC)
// (ALL CORE SERVICES ARE READY)
// ─────────────────────────────
void Zappy2::init(Services::ModuleContext* context) 
{
    ctx = context;
    initModuleData();
    initPins();
    scheduler = new PinScheduleManager(moduleData, ctx);
    scheduler->loadWebCommands();
   
    if(ctx->config.ExistSavedConfig(moduleData))
    {
      //ctx->serial.printLn("DATA EXIST");
      //ctx->serial.printLn("PRIMA");
      //ctx->serial.printLn(moduleData.DeviceName);
      //printPins();
      ctx->config.loadConfig(moduleData);
      this->loadJsonPayload();
      //ctx->serial.printLn("DOPO");
      //ctx->serial.printLn(moduleData.DeviceName);
      //printPins();
    }
    else
    {
        //ctx->serial.print("DATA NOT EXIST -- SAVE:");
        //ctx->serial.printLn("PRIMA");
        //ctx->serial.printLn(moduleData.DeviceName);
        //ctx->serial.printLn(ctx->config.saveModuleConfig(moduleData));
        ctx->config.saveModuleConfig(moduleData);
        //printPins();
        //ctx->serial.printLn("DOPO");
        //ctx->serial.printLn(moduleData.DeviceName);
    }


    //ctx->logs.write(this->name() + " Ready",LogType::INFO,this->name(), this->icon());
}

// ─────────────────────────────
// BEGIN: INIZIALIZE MODULE WITH ALL CORE SERVICES READY
// ─────────────────────────────
void Zappy2::begin() {
    //EXTERNAL API  .....
    //checkServer();
    scheduler->load(); // <-- carica i programmi dal JsonConfig
    ctx->logs.write("Module Zappy 2ch Ready",LogType::INFO,name().c_str(),icon().c_str());
}


void Zappy2::stopMqttService()
{
    ctx->mqtts.disconnect();
    unsigned long t0 = millis();
    while (ctx->mqtts.isConnected() && millis() - t0 < 500) {
        delay(1);
    }
}

// ─────────────────────────────
// LOOP: 
// ─────────────────────────────
void Zappy2::loop() {
    /*
    if (_timer && _serial)
        _serial->printLn("TEST DAL MODULO: " + moduleData.name);
    */
   
    if (scheduler && ctx->wifi.internet()) 
    {
        if(scheduler->IsTriggered)
        {
            //Heare can notifi change status by scheduler ... 
            //this->stopMqttService();
            //initMqtts();
            scheduler->IsTriggered=false;
        }
        scheduler->loop();
    }
    
    //Debounce saving to flash ...
    if (pendingSave) {
        if (millis() - lastChangeMillis >= SAVE_DEBOUNCE_MS) {
            //ctx->serial.printLn("DEBOUNCED SAVE");
            ctx->config.saveModuleConfig(moduleData);
            pendingSave = false;
        }
    }

        
}

// ─────────────────────────────
// Module Data (LOADED FROM LoopMax Core)
// ─────────────────────────────
void Zappy2::initModuleData() {
    moduleData.JsUIClass   = MODULE_UI_JS;
    //Externa used js (data\web\modules\):
    moduleData.JsFiles = {"PinScheduler.js"};
    //moduleData.JsFiles = {"module1/file1.js","module1/file2.js"};

    moduleData.Icon = "🔌";
    buildJsonPayload(moduleData.JsonConfig);
    moduleData.FwVersion    = MODULE_VERSION;
    moduleData.Code   = MODULE_CODE;
    moduleData.Name   = MODULE_NAME;
    moduleData.DeviceName   = MODULE_DEVICE_NAME;
    moduleData.Company  = MODULE_COMPANY;
    moduleData.Url   = MODULE_COMPANY_URL;

    
    moduleData.httpsData.Https_ApiUrl       = MODULE_HTTPS_API_URL;
    moduleData.httpsData.Https_Port       = MODULE_HTTPS_PORT;
    moduleData.httpsData.Https_ShowData    = MODULE_HTTPS_SHOW;
    moduleData.httpsData.Https_UseCA   = MODULE_HTTPS_USE_CA;
    moduleData.httpsData.Https_TimeOut = MODULE_HTTPS_TIME_OUT;
    //moduleData.httpsData.Https_CA      = MODULE_HTTPS_CA;
    moduleData.httpsData.Https_User      = MODULE_HTTPS_USR;
    moduleData.httpsData.Https_Psw      = MODULE_HTTPS_PSW;
    //moduleData.httpsData.Https_Method = WMethod::GET;
    
    
    moduleData.mqttsData.Mqtts_UseCA    = MODULE_MQTT_USE_CA;
    moduleData.mqttsData.Mqtts_Server   = MODULE_MQTT_SERVER;
    moduleData.mqttsData.Mqtts_Port     = MODULE_MQTT_PORT;
    moduleData.mqttsData.Mqtts_User     = MODULE_MQTT_USER;
    moduleData.mqttsData.Mqtts_Psw      = MODULE_MQTT_PSW;
    moduleData.mqttsData.Mqtts_ShowData = MODULE_SHOW_MQTT;
    //moduleData.mqttsData.Mqtts_CA       = MODULE_MQTT_CA;
    moduleData.mqttsData.Mqtts_TimeOut  = MODULE_MQTT_TIMEOUT;
         
    // ── Popola il vettore pins dai define ──
    moduleData.SaveConfig = true; //Enable flash dta conf saving
    moduleData.pins.clear();
    moduleData.pins.reserve(MODULE_PIN_COUNT);
    Helper::addPin(moduleData.pins, MODULE_PIN_0_NAME, MODULE_PIN_0_NUMBER, MODULE_PIN_0_MODE, PinLevel::Low);
    Helper::addPin(moduleData.pins, MODULE_PIN_1_NAME, MODULE_PIN_1_NUMBER, MODULE_PIN_1_MODE, PinLevel::Low);
    
    //Heare can register to logs events:
    //ctx->logs.registerSink(this);

    //Heare can register to reset events:
    ctx->config.registerSink(this);
    
}




/// @brief REGISTER TO LOGS EVENT FROM LoopMax Core
/// @param logs from LoopMax core
void Zappy2::publishLogs(const char* logs) {
   // _serial->printLn("MODULO: " + moduleData.name + " LOGS!");
    //_serial->printLn(logs);
}


/// @brief REGISTER TO SYSTEM RESET EVENT FROM LoopMax Core
void Zappy2::onSystemReset() {
    //this->stopMqttService();
}



void Zappy2::initPins()
{
    for (auto& pin : moduleData.pins) {
            ctx->pins.setMode(pin.number, pin.mode);
    }
}


bool Zappy2::loadJsonPayload() {
    /*
    JsonDocument doc;
    if (deserializeJson(doc, moduleData.JsonConfig)) return false;
    if (!doc["DeviceName"]) return false;
    moduleData.DeviceName = doc["DeviceName"].as<std::string>();
    */
    return true;
}

//Extra json Data.
void Zappy2::buildJsonPayload(std::string& out) {
    /*
    JsonDocument doc;
    doc["DeviceName"] = moduleData.DeviceName;
    out.clear();
    serializeJson(doc, out);
    */

}


/// @brief All Module Informtaions
/// @return  IModuleData Object
IModuleData& Zappy2::Data() {
    return moduleData;
}


/// @brief Print Module Pins Status ...
void Zappy2::printPins()
{
    for (auto& pin : moduleData.pins) {
            std::string msg = "Modulo pin " + std::to_string(pin.number)
                        + " nome: " + pin.name
                        + " val: " + std::to_string(PinLevelToInt(pin.level));

            ctx->logs.write(msg,LoopMax::Types::LogType::DEBUG,this->name(), this->icon());
    }
    ctx->logs.write("PAYLOAD",LoopMax::Types::LogType::DEBUG,this->name(), this->icon(), moduleData.JsonConfig);
}





    /// @brief Resiter Web command From LoopMax Web UI
    void Zappy2::registerEndpoints() {
           
                moduleData.webCommands.push_back({
                    moduleData.Name + "/invert",
                    0x01, // GET
                    [this](IHttpContext& httpCtx){
                        for (auto& pin : moduleData.pins) {
                            pin.level = (pin.level == PinLevel::High) ? PinLevel::Low : PinLevel::High;
                            ctx->pins.write(pin.number, pin.level);
                        }
                        std::string jsonPins;
                        this->getJsonPins(jsonPins);
                        //bool res = ctx->config.saveModuleConfig(moduleData);
                        pendingSave = true;
                        lastChangeMillis = millis();


                        httpCtx.send(200, "application/json", jsonPins);
                    }
                });

                moduleData.webCommands.push_back({
                    moduleData.Name + "/set",
                    0x02, // post
                    [this](IHttpContext& httpCtx){
                        bool res = this->setChannelFromClient(httpCtx.getParam("channel"), httpCtx.getParam("value"));
                        if(res) httpCtx.send(200, "text/plain", "ok");
                        if(!res) httpCtx.send(400, "text/plain", "ko");
                    }
                });

                moduleData.webCommands.push_back({
                    moduleData.Name + "/edit/pinName",
                    0x02, // post
                    [this](IHttpContext& httpCtx){
                        bool res = this->setLineName(httpCtx.getParam("number"), httpCtx.getParam("name"));
                        if(res) httpCtx.send(200, "text/plain", "ok");
                        if(!res) httpCtx.send(400, "text/plain", "ko");
                    }
                });


                moduleData.webCommands.push_back({
                    moduleData.Name + "/edit/devName",
                    0x02, // post
                    [this](IHttpContext& httpCtx){
                        moduleData.DeviceName = httpCtx.getParam("name");
                        moduleData.JsonConfig.clear();
                        this->buildJsonPayload(moduleData.JsonConfig);
                        //SAVE JSON DATA:
                        //bool res = ctx->config.saveModuleConfig(moduleData);
                        bool res = true;
                        pendingSave = true;
                        lastChangeMillis = millis();

                        //ctx->serial.printLn(moduleData.JsonConfig);
                        if(res) httpCtx.send(200, "text/plain", "ok");
                        if(!res) httpCtx.send(400, "text/plain", "ko");
                    }
                });


                 moduleData.webCommands.push_back({
                    moduleData.Name + "/fwUpdate",
                    0x01, // GET
                    [this](IHttpContext& httpCtx){
                        httpCtx.send(200, "application/json", "{\"status\":\"ok\"}");
                        if(ctx->wifi.internet())
                        {
                            //this->stopMqttService();
                            //API CALLS ....
                        }
                        
                    }
                });

          
    }


    bool Zappy2::setLineName(std::string channel, std::string name)
        {
            int ch = 0;
            if(!Helper::toInt(channel,ch)) return false;
             bool found = false;
             for (auto& pin : moduleData.pins) {
                    if(pin.number == ch) { pin.name = name; found = true; }
             }
            if(found)
             { 
                //SAVE DATA
                pendingSave = true;
                lastChangeMillis = millis();
                //return ctx->config.saveModuleConfig(moduleData); 
                return true; 
             }
            return false;
        }


      bool Zappy2::setChannelFromClient(std::string channel, std::string value)
            {
                int ch = 0;
                int vl = 0;
                if(!Helper::toInt(channel,ch)) return false;
                if(!Helper::toInt(value,vl)) return false;
                for (auto& pin : moduleData.pins) {
                    if(pin.number == ch)
                    {
                        pin.level = PinLevelFromStr(value);
                        ctx->pins.write(pin.number, pin.level);
                        //SAVE DATA
                        //this->printPins();
                        pendingSave = true;
                        lastChangeMillis = millis();
                        //return ctx->config.saveModuleConfig(moduleData); 
                        return true; 
                    }

                }

                return false;
            }



    void Zappy2::getJsonPins(std::string& out) {
        JsonDocument doc;
        JsonArray pinsArr = doc["pins"].to<JsonArray>();
        for (const auto& p : moduleData.pins) {
            JsonObject pObj = pinsArr.add<JsonObject>();
            pObj["number"] = p.number;
            pObj["name"]   = p.name;
            pObj["level"]  = PinLevelToInt(p.level);
        }
        out.clear();
        serializeJson(doc, out);
    }

        
    void Zappy2::checkServer()
            {
                //haveMqttsData = true;
                //if (haveMqttsData) { initMqtts(); }
            }
            

            /// @brief MQTTS:
            void Zappy2::initMqtts() {
                if(!haveMqttsData) return;
                ctx->mqtts.begin(moduleData.mqttsData);
                if(ctx->mqtts.connect())
                {
                    ctx->mqtts.subscribe(moduleData.mqttsData.Mqtts_BaseTopic + "#");
                    ctx->mqtts.onMessage([this](const std::string& topic, const std::string& payload) {
                            this->processMqtts(topic, payload);
                    });
                }
                else
                {
                    ctx->logs.write("Mqtts connection failed",LogType::ERROR,name().c_str(),icon().c_str());
                }
            }
            
            void Zappy2::processMqtts(const std::string& topic, const std::string& payload) {
                if(payload.empty() || payload.length()<=0) return;
                //Serial.print("RX topic: ");
                //Serial.println(topic.c_str());
                //Serial.print("RX payload: ");
                //Serial.println(payload.c_str());

                //Check device from server:
                if(topic == moduleData.mqttsData.Mqtts_BaseTopic + "isonline")
                {
                    this->stopMqttService();
                    //API CALL
                    //initMqtts();
                    return;
                }

                //Multicommands (allon, alloff, invert)
                if(topic == moduleData.mqttsData.Mqtts_BaseTopic + "multicommand")
                {
                    for (auto& p : moduleData.pins) {
                        if(payload == "allon") this->setChannelFromClient(std::to_string(p.number), "1");
                        if(payload == "alloff") this->setChannelFromClient(std::to_string(p.number), "0");
                        if(payload == "invert")
                        {
                            p.level = (p.level == PinLevel::High) ? PinLevel::Low : PinLevel::High;
                            ctx->pins.write(p.number, p.level);
                            //bool res = ctx->config.saveModuleConfig(moduleData);
                            pendingSave = true;
                            lastChangeMillis = millis();
                        }
                    }
                    //this->stopMqttService();
                    //API CALL
                    //initMqtts();
                    return;
                }
                
                //Set one line:
                int count = 1;
                for (const auto& p : moduleData.pins) {
                    if(topic == moduleData.mqttsData.Mqtts_BaseTopic + "l" + std::to_string(count))
                    {
                        bool res = this->setChannelFromClient(std::to_string(p.number), payload);
                        if(res)
                        {
                            //this->stopMqttService();
                            //API CALL
                            //initMqtts();
                        }
                        return;
                    }
                    count++;
                }
                //this->stopMqttService();
                //smartiLabApi->processMqtts(moduleData, topic, payload);
                //initMqtts();
                
            }



// ─────────────────────────────
// Static registration
// ─────────────────────────────
REGISTER_MODULE_STATIC(Zappy2)
