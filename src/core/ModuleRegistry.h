#pragma once
#include "common/IModule.h"
#include <vector>

#include "Arduino.h"

// Dichiarazione preventiva
namespace LoopMax::Services { struct ModuleContext; }

namespace LoopMax::Core {
    using namespace Modules;
            
           class ModuleRegistry {
                    public:
                        static ModuleRegistry& instance() {
                            static ModuleRegistry inst;
                            return inst;
                        }
                        

                        void initModules(Services::ModuleContext* LoopMaxCore){
                            for (auto* m : _modules) {
                                Serial.print("Module found: ");
                                Serial.println(m->name().c_str());
                                if (m->enabled())
                                {
                                    m->init(LoopMaxCore);
                                    
                                    m->registerEndpoints();
                                }
                                    
                            }
                        }

                        void registerModule(IModule* module) {
                            if (module)
                                _modules.push_back(module);
                        }

                        void begin() {
                            for (auto* m : _modules) {
                                if (m->enabled()) { m->begin(); }
                            }

                        }

                        void loop() {
                            for (auto* m : _modules) {
                                if (m->enabled())
                                    m->loop();
                            }
                        }

                        const std::vector<IModule*>& modules() const {
                                return _modules;
                         }

                    private:
                         std::vector<IModule*> _modules;
                         
            };



}

//MACRO REGISTRAZIONE MODULI:
#define REGISTER_MODULE_STATIC(CLASSNAME) \
    namespace { \
        struct CLASSNAME##AutoRegister { \
            CLASSNAME##AutoRegister() { \
                static CLASSNAME instance; \
                LoopMax::Core::ModuleRegistry::instance().registerModule(&instance); \
            } \
        }; \
        static CLASSNAME##AutoRegister global_##CLASSNAME##_registrar; \
    }
