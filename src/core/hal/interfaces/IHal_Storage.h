#pragma once
#include "common/Types.h"
#include <cstddef>


namespace LoopMax::Core::Hal {

    using namespace LoopMax::Types;

    class IHal_Storage {
    public:
        virtual ~IHal_Storage() = default;

        
        virtual void begin() = 0;
        virtual bool isReady() const = 0;
        virtual void end() = 0;
        virtual bool getFile(const std::string &path, std::string &outData) = 0;
        virtual bool saveFile(const std::string &path, const std::string &data) = 0;
        virtual uint32_t FS_OFFSET() = 0;
        virtual uint32_t FS_SIZE() = 0;
        virtual bool getNVSConfig(const std::string &name, std::string &outData) = 0;
        virtual bool saveNVSConfig(const std::string &name, const std::string &data) = 0;
        //virtual void initContext(SystemContext* ctx) = 0;
    };

}
