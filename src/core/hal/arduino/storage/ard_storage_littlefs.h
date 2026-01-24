#pragma once
#include "core/hal/interfaces/IHal_Storage.h"
#include <LittleFS.h>
#include <string>

namespace LoopMax::Core::Hal {

    class ard_storage_littlefs : public IHal_Storage {
    public:
        void begin() override;
        bool isReady() const override;
        void end() override;

        
        bool getFile(const std::string &path, std::string &outData);
        bool saveFile(const std::string &path, const std::string &data);

    private:
        bool _ready = false;
    };

}
