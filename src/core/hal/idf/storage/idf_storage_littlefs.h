#pragma once
#include "core/hal/interfaces/IHal_Storage.h"

extern "C" {
    #include "esp_littlefs.h"
    #include "esp_vfs.h"
}

#include <stdio.h>

namespace LoopMax::Core::Hal {

    class idf_storage_littlefs : public IHal_Storage {
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
