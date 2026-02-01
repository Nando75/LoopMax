#include "Zappy2.h"
#include "common/tools/Helper.h"
#include "common/iservices/ISerial.h"
#include "common/iservices/ILogs.h"
#include "common/iservices/IConfig.h"

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
    if(ctx->config.ExistSavedConfig(moduleData))
    {
      //ctx->serial.printLn("DATA EXIST");
      //printPins();
      ctx->config.loadConfig(moduleData);
      this->loadJsonPayload();
      //ctx->serial.printLn("DOPO");
      //printPins();
    }
    else
    {
        //ctx->serial.print("DATA NOT EXIST -- SAVE:");
        ctx->config.saveModuleConfig(moduleData);
        //printPins();
    }

    //ctx->logs.write(this->name() + " Ready",LogType::INFO,this->name(), this->icon());
}

// ─────────────────────────────
// BEGIN: INIZIALIZE MODULE WITH ALL CORE SERVICES READY
// ─────────────────────────────
void Zappy2::begin() {
    //Web Command Send From UI
    
    ctx->logs.write("Module Zappy Begin",LogType::INFO,name().c_str(),icon().c_str());
   

   if(printOnetime)
    {
        //ctx->serial.print("DOPO DI CONFIG ");
       // printPins();
        printOnetime = false;
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
   
        
        
}

// ─────────────────────────────
// Module Data (LOADED FROM LoopMax Core)
// ─────────────────────────────
void Zappy2::initModuleData() {
    moduleData.JsUIClass   = MODULE_UI_JS;
    moduleData.Icon = "⚡️";
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
    moduleData.httpsData.Https_CA      = MODULE_HTTPS_CA;
    moduleData.httpsData.Https_User      = MODULE_HTTPS_USR;
    moduleData.httpsData.Https_Psw      = MODULE_HTTPS_PSW;
    //moduleData.httpsData.Https_Method = WMethod::GET;
    
    
    moduleData.mqttsData.Mqtts_UseCA    = MODULE_MQTT_USE_CA;
    moduleData.mqttsData.Mqtts_Server   = MODULE_MQTT_SERVER;
    moduleData.mqttsData.Mqtts_Port     = MODULE_MQTT_PORT;
    moduleData.mqttsData.Mqtts_User     = MODULE_MQTT_USER;
    moduleData.mqttsData.Mqtts_Psw      = MODULE_MQTT_PSW;
    moduleData.mqttsData.Mqtts_ShowData = MODULE_SHOW_MQTT;
    moduleData.mqttsData.Mqtts_CA       = MODULE_MQTT_CA;
     
    // ── Popola il vettore pins dai define ──
    moduleData.SaveConfig = true; //ABILITA IL SALVATAGGIO DELLO STATO DEI PINS
    moduleData.pins.clear();
    moduleData.pins.reserve(MODULE_PIN_COUNT);
    
    
    Helper::addPin(moduleData.pins, MODULE_PIN_0_NAME, MODULE_PIN_0_NUMBER, MODULE_PIN_0_MODE, PinLevel::High);
    Helper::addPin(moduleData.pins, MODULE_PIN_1_NAME, MODULE_PIN_1_NUMBER, MODULE_PIN_1_MODE, PinLevel::High);
    
    //Registrazione Evento Logs
    //ctx->logs.registerSink(this);
    //ctx->config.registerSink(this);
}


/// @brief REGISTER TO LOGS EVENT FROM LoopMax Core
/// @param logs from LoopMax core
void Zappy2::publishLogs(const char* logs) {
    
   // _serial->printLn("MODULO: " + moduleData.name + " LOGS!");
    //_serial->printLn(logs);

}


/// @brief REGISTER TO SYSTEM RESET EVENT FROM LoopMax Core
void Zappy2::onSystemReset() {


}




void Zappy2::initPins()
{
    for (auto& pin : moduleData.pins) {
            ctx->pins.setMode(pin.number, pin.mode);
    }

}


bool Zappy2::loadJsonPayload() {

    /*
    //HERE CUSTOM MOUDLE JSON EXTRA:
    JsonDocument doc;
    if (deserializeJson(doc, moduleData.JsonConfig)) return false;
    if (!doc["param1"]) return false;
    moduleData.DeviceName = doc["DeviceName"].as<std::string>();

    */


    return true;
}


void Zappy2::buildJsonPayload(std::string& out) {
    /*
    //HERE CUSTOM MOUDLE JSON EXTRA:
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
                        bool res = ctx->config.saveModuleConfig(moduleData);
                        ctx->serial.printLn(moduleData.JsonConfig);
                        
                        if(res) httpCtx.send(200, "text/plain", "ok");
                        if(!res) httpCtx.send(400, "text/plain", "ko");
                    }
                });



          
    }


    bool Zappy2::setLineName(std::string channel, std::string name)
        {
            int8_t ch = 0;
            try {
                ch = std::stoll(channel);
                if (ch <= 0) return false;
            } catch (...) {
                return false;
            }

             bool found = false;
             for (auto& pin : moduleData.pins) {
                    if(pin.number == ch) { pin.name = name; found = true; }
             }
            if(found)
             { 
                //SAVE DATA
                return ctx->config.saveModuleConfig(moduleData); 
             }
            
            return false;
        }


      bool Zappy2::setChannelFromClient(std::string channel, std::string value)
            {
                int8_t ch = 0;
                int8_t vl = 0;

                try {
                    ch = std::stoll(channel);
                    if (ch <= 0) return false;
                } catch (...) {
                    return false;
                }

                try {
                    vl = std::stoll(value);
                } catch (...) {
                    return false;
                }


                for (auto& pin : moduleData.pins) {
                    if(pin.number == ch)
                    {
                        pin.level = PinLevelFromStr(value);
                        ctx->pins.write(pin.number, pin.level);
                        //SAVE DATA
                        this->printPins();
                        return ctx->config.saveModuleConfig(moduleData); 
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




// ─────────────────────────────
// Static registration
// ─────────────────────────────
REGISTER_MODULE_STATIC(Zappy2)
