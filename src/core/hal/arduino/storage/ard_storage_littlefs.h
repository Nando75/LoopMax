#pragma once
#include "core/hal/interfaces/IHal_Storage.h"
#include <LittleFS.h>
#include <string>
#include <Preferences.h>

namespace LoopMax::Core::Hal {

    class ard_storage_littlefs : public IHal_Storage {
    public:
        void begin() override;
        bool isReady() const override;
        void end() override;

        
        bool getFile(const std::string &path, std::string &outData);
        bool saveFile(const std::string &path, const std::string &data);

        bool getNVSConfig(const std::string &name, std::string &outData) override;
        bool saveNVSConfig(const std::string &name, const std::string &data) override;

    private:
        bool _ready = false;
        Preferences prefs;
    };

}
