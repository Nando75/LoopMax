#include <vector>
#include "core/services/IService.h"
#include "core/services/system/SystemManager.h"
#include "core/services/serial/SerialManager.h"
#include "common/IModule.h"

#include <unordered_set>



namespace LoopMax::Core {
        class ServicesManager {
            public:
                ServicesManager();
                explicit ServicesManager(SystemManager* sys) : system(sys) {}
                void registerService(IService* service);
                void check(Core::SerialManager*  serialService);
                const std::vector<IService*>& getServices();

            private:
                std::vector<IService*> services;
                //bool checkDependenciesReady(IService* service, std::unordered_set<IService*>& visited);
                SystemManager* system;
                bool isModeAllowed(IService* srv, Types::SystemMode currentMode);
                bool areDepsReady(IService* srv);
                
        };
}
    


