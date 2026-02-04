#include "PinScheduleManager.h"

#include "common/iservices/IConfig.h"
#include "common/iservices/ILogs.h"
#include "common/iservices/IPins.h"
#include "common/iservices/ITimer.h"
// Se hai un servizio tempo tipo ctx->time, includi qui la sua interfaccia

using namespace LoopMax::Modules;
using namespace LoopMax::Types;
using namespace LoopMax::Services;
using LoopMax::Utils::Helper;


PinScheduleManager::PinScheduleManager(IModuleData& data,
                                       ModuleContext* ctx)
    : moduleData(data), ctx(ctx) { }

// ─────────────────────────────
// Helpers tipo
// ─────────────────────────────
const char* PinScheduleManager::scheduleTypeToStr(ScheduleType t) {
    switch (t) {
        case ScheduleType::OneTime: return "onetime";
        case ScheduleType::Weekly:  return "weekly";
        default:                    return "onetime";
    }
}

ScheduleType PinScheduleManager::scheduleTypeFromStr(const std::string& s) {
    if (s == "weekly")  return ScheduleType::Weekly;
    return ScheduleType::OneTime;
}


void PinScheduleManager::load() {
    schedules.clear();
    nextId = 1;
    // Startup sporco: niente config o "null"
    if (moduleData.JsonConfig.empty() || moduleData.JsonConfig == "null")
        return;
    JsonDocument doc;
    auto err = deserializeJson(doc, moduleData.JsonConfig);
    if (err) {
        return;
    }
    // Se non è un oggetto JSON, non lo tocchiamo
    if (!doc.is<JsonObject>()) {
        return;
    }
    loadFromJson(doc);
}

void PinScheduleManager::save() {
    JsonDocument doc;

    bool hasValidRoot = false;

    if (!moduleData.JsonConfig.empty() && moduleData.JsonConfig != "null") {
        auto err = deserializeJson(doc, moduleData.JsonConfig);
        if (!err && doc.is<JsonObject>()) {
            hasValidRoot = true;
        } else {
            doc.clear();
        }
    }

    if (!hasValidRoot) {
        // Creiamo un root {} sicuro
        doc.to<JsonObject>();
    }

    // Rimuovi eventuale vecchio nodo schedules
    doc.remove("schedules");

    // Scrivi i nuovi schedules
    saveToJson(doc);

    moduleData.JsonConfig.clear();
    serializeJson(doc, moduleData.JsonConfig);

}


void PinScheduleManager::loadFromJson(JsonDocument& doc) {
    if (!doc["schedules"].is<JsonObject>()) return;


    JsonObject root = doc["schedules"].as<JsonObject>();
    if (root.isNull()) return;

    int maxId = 0;

    for (JsonPair kv : root) {
        const char* pinKey = kv.key().c_str();
        int pinNumber = std::atoi(pinKey);
        JsonArray arr = kv.value().as<JsonArray>();
        if (arr.isNull()) continue;

        auto& vec = schedules[pinNumber];

        for (JsonObject o : arr) {
            Schedule s;
            s.id       = o["id"]       | 0;
            std::string typeStr = o["type"] | "onetime";
            s.type     = scheduleTypeFromStr(typeStr);

            s.enabled  = o["enabled"]  | true;

            // OneTime
            s.start    = o["start"]    | 0;
            s.end      = o["end"]      | 0;

            // Weekly
            s.daysMask   = o["days"]     | 0;
            s.onMinutes  = o["on"]       | 0;
            s.offMinutes = o["off"]      | 0;

            vec.push_back(s);
            if (s.id > maxId) maxId = s.id;
        }
    }

    nextId = (maxId > 0) ? (maxId + 1) : 1;
}

void PinScheduleManager::saveToJson(JsonDocument& doc) {
    if (!doc.is<JsonObject>()) {
        doc.clear();
        doc.to<JsonObject>();
    }

    JsonObject root = doc["schedules"].to<JsonObject>();

    for (const auto& kv : schedules) {
        int pinNumber = kv.first;
        const auto& vec = kv.second;

        char pinKey[8];
        std::snprintf(pinKey, sizeof(pinKey), "%d", pinNumber);

        JsonArray arr = root[pinKey].to<JsonArray>();

        for (const auto& s : vec) {
            JsonObject o = arr.add<JsonObject>();
            o["id"]      = s.id;
            o["type"]    = scheduleTypeToStr(s.type);
            o["enabled"] = s.enabled;
            o["start"]   = static_cast<long>(s.start);
            o["end"]     = static_cast<long>(s.end);
            o["days"]    = s.daysMask;
            o["on"]      = s.onMinutes;
            o["off"]     = s.offMinutes;
        }
    }

    std::string tmp;
    serializeJson(doc, tmp);
}

bool PinScheduleManager::newSchedule(int pinNumber, Schedule schedule, int* outId)
{
    auto it = std::find_if(
        moduleData.pins.begin(),
        moduleData.pins.end(),
        [pinNumber](const modulePin& mp) {
            return mp.number == pinNumber;
        }
    );

    if (it == moduleData.pins.end())
        return false;

    if (schedule.id == 0)
        schedule.id = nextId++;
    else if (schedule.id >= nextId)
        nextId = schedule.id + 1;

    schedules[pinNumber].push_back(schedule);

    if (outId) *outId = schedule.id;

    save();
    return true;
}


/*
bool PinScheduleManager::removeSchedule(int pinNumber, int scheduleId) {
    auto it = schedules.find(pinNumber);
    if (it == schedules.end()) return false;
    auto& vec = it->second;
    auto oldSize = vec.size();
    vec.erase(
        std::remove_if(vec.begin(), vec.end(),
                       [scheduleId](const Schedule& s){ return s.id == scheduleId; }),
        vec.end()
    );
    if (vec.empty()) {
        schedules.erase(it);
    }
    if (vec.size() != oldSize) {
        save();
        return true;
    }
    return false;
}
*/

bool PinScheduleManager::clearSchedules(int pinNumber, bool saveConf) {
    auto it = schedules.find(pinNumber);
    if (it == schedules.end()) return false;
    schedules.erase(it);
    save();
    if(saveConf) ctx->config.saveModuleConfig(moduleData);
    return true;
}

                void PinScheduleManager::loop() {
                    if (!ctx) return;
                    time_t now = time(nullptr);
                    if (now <= 0) return;
                    tm tinfo;
                    localtime_r(&now, &tinfo);
                    uint8_t dayMask = 1 << tinfo.tm_wday;
                    int minutes = tinfo.tm_hour * 60 + tinfo.tm_min;
                    bool saveConfig = false;
                    std::vector<int> pinsToClear;
                    for (auto& kv : schedules) {
                        int pinNumber = kv.first;
                        auto& vec = kv.second;

                        if (vec.empty()) continue;

                        bool hasDesiredState = false;
                        PinLevel desiredState = PinLevel::Low;

                        for (auto& s : vec) {
                            if (!s.enabled) continue;

                            if (s.type == ScheduleType::OneTime) {
                                if (now >= s.start && now < s.end) {
                                    desiredState = PinLevel::High;
                                    hasDesiredState = true;
                                    break;
                                }
                                if (now >= s.end && s.end > 0) {
                                    desiredState = PinLevel::Low;
                                    hasDesiredState = true;
                                    saveConfig = true;
                                    pinsToClear.push_back(pinNumber);
                                    break;
                                }
                            }

                            if (s.type == ScheduleType::Weekly) {
                                if (s.daysMask & dayMask) {
                                    // gestione normale o overnight
                                    if (s.onMinutes <= s.offMinutes) {
                                        // fascia nello stesso giorno
                                        desiredState = (minutes >= s.onMinutes && minutes < s.offMinutes)
                                                    ? PinLevel::High
                                                    : PinLevel::Low;
                                    } else {
                                        // fascia overnight (es. 20:00 → 08:00)
                                        desiredState = (minutes >= s.onMinutes || minutes < s.offMinutes)
                                                    ? PinLevel::High
                                                    : PinLevel::Low;
                                    }
                                    hasDesiredState = true;
                                    break;
                                }
                            }
                        }

                        if (!hasDesiredState) continue;

                        if (lastState[pinNumber] != desiredState) {
                            lastState[pinNumber] = desiredState;
                                ctx->serial.printLn(
                                    "SCHEDULE TRIGGER → pin " + std::to_string(pinNumber) +
                                    " = " + (desiredState == PinLevel::High ? "HIGH" : "LOW")
                                );
                                applyStateIfChanged(pinNumber, desiredState, false);
                        }
                    }

                    if(saveConfig)
                    {
                        for (int p : pinsToClear) {
                            saveConfig |= clearSchedules(p);
                        }
                        ctx->config.saveModuleConfig(moduleData);
                    }



                }


            ScheduleError PinScheduleManager::addSchedule(const ScheduleParams& p, int* outId)
            {
                Schedule s;
                s.enabled = true;

                int pin;

                // ---- PIN ----
                if (!LoopMax::Utils::Helper::toInt(p.pin, pin, 0, 39))
                    return ScheduleError::InvalidPin;

                // ---- TYPE ----
                s.type = (p.type == "weekly")
                        ? ScheduleType::Weekly
                        : ScheduleType::OneTime;

                // ---- ONE TIME ----
                if (s.type == ScheduleType::OneTime)
                {
                    int64_t startTmp, endTmp;

                    if (!LoopMax::Utils::Helper::toInt64(p.start, startTmp, 0, 4102444800LL) ||
                        !LoopMax::Utils::Helper::toInt64(p.end,   endTmp,   0, 4102444800LL))
                        return ScheduleError::InvalidTime;

                    if (endTmp <= startTmp)
                        return ScheduleError::InvalidRange;

                    s.start = static_cast<time_t>(startTmp);
                    s.end   = static_cast<time_t>(endTmp);
                }
                // ---- WEEKLY ----
                else
                {
                    int daysTmp, onTmp, offTmp;

                    if (!LoopMax::Utils::Helper::toInt(p.days, daysTmp, 0, 127) ||
                        !LoopMax::Utils::Helper::toInt(p.on,   onTmp,   0, 1440) ||
                        !LoopMax::Utils::Helper::toInt(p.off,  offTmp,  0, 1440))
                        return ScheduleError::InvalidTime;

                    if (offTmp == onTmp)
                        return ScheduleError::InvalidRange;

                    s.daysMask   = static_cast<uint8_t>(daysTmp);
                    s.onMinutes  = static_cast<uint16_t>(onTmp);
                    s.offMinutes = static_cast<uint16_t>(offTmp);
                }

                //Only one schedule on a pin
                this->clearSchedules(pin, false);

                // ---- CORE ADD ----
                bool ok = newSchedule(pin, s, outId);
                if (!ok)
                    return ScheduleError::PinNotFound;

                applyStateIfChanged(pin, PinLevel::Low, false);
                ctx->config.saveModuleConfig(moduleData);
                return ScheduleError::Ok;
            }


            bool PinScheduleManager::applyStateIfChanged(int pinNumber, PinLevel desired, bool saveConf) {
                for (auto& p : moduleData.pins) {
                    if (p.number == pinNumber) {
                        if (p.level != desired) {
                            p.level = desired;
                            ctx->pins.write(pinNumber, desired);
                            if(saveConf) ctx->config.saveModuleConfig(moduleData);
                            this->IsTriggered = true;
                            return true;
                        }
                        return false;
                    }
                }
                return false;
            }
            

            void PinScheduleManager::loadWebCommands() {

                    moduleData.webCommands.push_back({
                            moduleData.Name + "/schedule/add",
                            0x02,
                                [this](IHttpContext& httpCtx){
                                        ScheduleParams p;
                                        p.type  = httpCtx.getParam("type");
                                        p.pin   = httpCtx.getParam("pin");
                                        p.start = httpCtx.getParam("start");
                                        p.end   = httpCtx.getParam("end");
                                        p.days  = httpCtx.getParam("days");
                                        p.on    = httpCtx.getParam("on");
                                        p.off   = httpCtx.getParam("off");
                                        int id;
                                        auto res = this->addSchedule(p, &id);
                                        httpCtx.send(
                                            res == ScheduleError::Ok ? 200 : 400,
                                            res == ScheduleError::Ok ? "application/json" : "text/plain",
                                            res == ScheduleError::Ok ? moduleData.JsonConfig : scheduleErrorToString(res)
                                        );
                                        
                                    }

                            });


                            moduleData.webCommands.push_back({
                                    moduleData.Name + "/schedule/remove",
                                    0x02,
                                    [this](IHttpContext& httpCtx){
                                        int pin;
                                        bool ok = false;
                                        if (LoopMax::Utils::Helper::toInt(httpCtx.getParam("pin"), pin, 0, 39))
                                        {
                                            ok = this->clearSchedules(pin, true);
                                        }
                                        //ctx->logs.write("Schedule Remove",LoopMax::Types::LogType::DEBUG,this->name(), this->icon(), moduleData.JsonConfig);
                                        if (ok) httpCtx.send(200, "application/json", moduleData.JsonConfig);
                                        else    httpCtx.send(400, "text/plain", "error");
                                    }
                                });

                               
                                /*
                                moduleData.webCommands.push_back({
                                    moduleData.Name + "/schedule/list",
                                    0x01,
                                    [this](IHttpContext& httpCtx){
                                        std::string out = moduleData.JsonConfig;
                                        httpCtx.send(200, "application/json", out);
                                    }
                                });
                                */
    

            }
