#ifndef LOOPMAX_SCHEDULE_H
#define LOOPMAX_SCHEDULE_H


        #pragma once

        #include <map>
        #include <vector>
        #include <string>
        #include <cstdint>
        #include <ctime>
        #include <ArduinoJson.h>
        #include "common/ModuleContext.h"
        #include "common/IModule.h"
        #include "common/tools/Helper.h"        

        
        namespace LoopMax::Types {
            enum class ScheduleType : uint8_t {
                OneTime = 0,
                Weekly  = 1
            };

            struct Schedule {
                int      id          = 0;
                ScheduleType type    = ScheduleType::OneTime;

                // OneTime
                time_t   start       = 0;
                time_t   end         = 0;

                // Weekly
                uint8_t  daysMask    = 0;     // bit 0 = Sunday, 1 = Monday, ... 6 = Saturday
                uint16_t onMinutes   = 0;     // minuti da mezzanotte
                uint16_t offMinutes  = 0;

                bool     enabled     = true;
            };

            struct ScheduleParams {
                std::string type;
                std::string pin;
                std::string start;
                std::string end;
                std::string days;
                std::string on;
                std::string off;
            };

            enum class ScheduleError {
                Ok,
                InvalidPin,
                InvalidTime,
                InvalidRange,
                PinNotFound
            };
            inline const char* scheduleErrorToString(ScheduleError err)
                {              
                switch (err)
                    {
                        case ScheduleError::Ok:           return "ok";
                        case ScheduleError::InvalidPin:   return "invalid pin";
                        case ScheduleError::InvalidTime:  return "invalid time values";
                        case ScheduleError::InvalidRange: return "invalid time range";
                        case ScheduleError::PinNotFound:  return "pin not managed";
                    }
                    return "unknown error";
            }


        }
        
        namespace LoopMax::Modules {

        

        class PinScheduleManager {
        public:
            PinScheduleManager(LoopMax::Types::IModuleData& data,
                            LoopMax::Services::ModuleContext* ctx);

            
            void loadWebCommands();
            void load();
            void save();

            // Da chiamare nel loop del modulo
            void loop();

            // API di gestione
            ScheduleError addSchedule(const LoopMax::Types::ScheduleParams& p, int* outId = nullptr);
            bool clearSchedules(int pinNumber, bool saveConf=false);
            bool hasSchedules(int pinNumber) const;
            //const std::vector<Schedule>* getSchedules(int pinNumber) const;
            bool IsTriggered = false;


        private:
            LoopMax::Types::IModuleData& moduleData;
            LoopMax::Services::ModuleContext* ctx;

            const char* Name = "PinManager";
            const char* Icon = "⏰";

            std::map<int, std::vector<Schedule>> schedules;
            int nextId = 1;

            // JSON helpers
            void loadFromJson(JsonDocument& doc);
            void saveToJson(JsonDocument& doc);
            bool applyStateIfChanged(int pinNumber, PinLevel desired, bool saveConf=true);
            std::unordered_map<int, PinLevel> lastState;
            bool newSchedule(int pinNumber, Schedule schedule, int* outId = nullptr);
            static const char* scheduleTypeToStr(ScheduleType t);
            static ScheduleType scheduleTypeFromStr(const std::string& s);
            
            


        };

        } // namespace LoopMax::Modules

#endif


/*
✅ 4. Come aggiungere un programma (esempio reale)
Aggiungere un programma Weekly sul pin 12
Lunedì, Mercoledì, Venerdì
dalle 08:00 alle 12:00

cpp
Schedule s;
s.type = ScheduleType::Weekly;
s.enabled = true;
s.daysMask = (1 << 1) | (1 << 3) | (1 << 5); // lun, mer, ven
s.onMinutes = 8 * 60;
s.offMinutes = 12 * 60;

int id;
scheduler->addSchedule(12, s, &id);





Aggiungere un OneTime sul pin 14
Dalle 18:00 alle 19:30 del 1 Febbraio 2026

cpp
Schedule s;
s.type = ScheduleType::OneTime;
s.enabled = true;
s.start = 1769911200; // UNIX
s.end   = 1769916600;

scheduler->addSchedule(14, s);






✅ 5. Come funziona il loop intelligente
Ogni ciclo:
Legge l’ora locale corretta (time(nullptr) + localtime())
Per ogni pin:
calcola lo stato desiderato
lo confronta con lo stato attuale salvato in moduleData.pins
se diverso →
scrive il pin
aggiorna moduleData.pins
salva su flash
Se uguale → non fa nulla
Quindi:
zero scritture ripetute
zero usura flash inutile
zero rimbalzi
zero logica duplicata




✅ 6. Le API funzionano?
Sì, ma devi aggiungerle tu nel modulo.
La classe PinScheduleManager non registra endpoint.
È il modulo (Zappy4) che deve esporre:
/schedule/add
/schedule/remove
/schedule/list
/schedule/clear
E poi chiamare:
cpp
scheduler->addSchedule(...)
scheduler->removeSchedule(...)
scheduler->clearSchedules(...)
scheduler->getSchedules(...)



🔵 2. COME SI SALVA IL JSON
Tu NON devi salvare nulla.
Ogni volta che:
aggiungi un programma → addSchedule()
rimuovi un programma → removeSchedule()
cambi stato del pin → applyStateIfChanged()
viene chiamato:
cpp
ctx->config.saveModuleConfig(moduleData);
E il core salva tutto su flash.

🔵 3. ESEMPIO DI JSON SALVATO
Dopo aver aggiunto due programmi:

json
{
  "schedules": {
    "12": [
      {
        "id": 1,
        "type": "weekly",
        "enabled": true,
        "days": 42,
        "on": 480,
        "off": 1320
      }
    ],
    "14": [
      {
        "id": 2,
        "type": "onetime",
        "enabled": true,
        "start": 1769911200,
        "end": 1769916600
      }
    ]
  }
}
Questo JSON vive in:
moduleData.JsonConfig


🔵 4. COME SI AGGIUNGE UN PROGRAMMA (ESEMPIO REALE)
Weekly — pin 12 — lun/mer/ven — 08:00 → 12:00
cpp
Schedule s;
s.type = ScheduleType::Weekly;
s.enabled = true;
s.daysMask = (1 << 1) | (1 << 3) | (1 << 5);
s.onMinutes = 8 * 60;
s.offMinutes = 12 * 60;
int id;
scheduler->addSchedule(12, s, &id);

OneTime — pin 14 — 1 Feb 2026 — 18:00 → 19:30
cpp
Schedule s;
s.type = ScheduleType::OneTime;
s.enabled = true;
s.start = 1769911200;
s.end   = 1769916600;
scheduler->addSchedule(14, s);





🎯 7. Esempio completo di endpoint /schedule/add
moduleData.webCommands.push_back({
    moduleData.Name + "/schedule/add",
    0x02,
    [this](IHttpContext& httpCtx){
        Schedule s;
        std::string type = httpCtx.getParam("type");

        s.type = (type == "weekly") ? ScheduleType::Weekly : ScheduleType::OneTime;
        s.enabled = true;

        int pin = std::stoi(httpCtx.getParam("pin"));

        if (s.type == ScheduleType::OneTime) {
            s.start = std::stoll(httpCtx.getParam("start"));
            s.end   = std::stoll(httpCtx.getParam("end"));
        } else {
            s.daysMask = std::stoi(httpCtx.getParam("days"));
            s.onMinutes = std::stoi(httpCtx.getParam("on"));
            s.offMinutes = std::stoi(httpCtx.getParam("off"));
        }

        int id;
        bool ok = scheduler->addSchedule(pin, s, &id);

        if (ok) httpCtx.send(200, "text/plain", std::to_string(id));
        else    httpCtx.send(400, "text/plain", "error");
    }
});


moduleData.webCommands.push_back({
    moduleData.Name + "/schedule/remove",
    0x02,
    [this](IHttpContext& httpCtx){
        int pin = std::stoi(httpCtx.getParam("pin"));
        int id  = std::stoi(httpCtx.getParam("id"));

        bool ok = scheduler->removeSchedule(pin, id);

        if (ok) httpCtx.send(200, "text/plain", "ok");
        else    httpCtx.send(400, "text/plain", "error");
    }
});



moduleData.webCommands.push_back({
    moduleData.Name + "/schedule/list",
    0x01,
    [this](IHttpContext& httpCtx){
        std::string out = moduleData.JsonConfig;
        httpCtx.send(200, "application/json", out);
    }
});



moduleData.webCommands.push_back({
    moduleData.Name + "/schedule/clear",
    0x02,
    [this](IHttpContext& httpCtx){
        int pin = std::stoi(httpCtx.getParam("pin"));

        bool ok = scheduler->clearSchedules(pin);

        if (ok) httpCtx.send(200, "text/plain", "ok");
        else    httpCtx.send(400, "text/plain", "error");
    }
});



🔵 7. COME SI USA DALLA UI
La UI può:
leggere /schedule/list
inviare /schedule/add
inviare /schedule/remove
inviare /schedule/clear
E aggiornare la tabella dei programmi.






*/