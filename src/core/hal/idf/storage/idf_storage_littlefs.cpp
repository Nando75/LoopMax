#include "idf_storage_littlefs.h"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

namespace LoopMax::Core::Hal {

    void idf_storage_littlefs::begin() {
        // LittleFS init nativo ESP-IDF
        _ready = true; // assume filesystem montato
    }

    bool idf_storage_littlefs::isReady() const {
        return _ready;
    }

    void idf_storage_littlefs::end() {
        _ready = false;
    }

    bool idf_storage_littlefs::getFile(const std::string &path, std::string &outData) {
        if (!_ready) return false;

        struct stat st;
        if (stat(path.c_str(), &st) != 0) return false;

        std::ifstream inFile(path, std::ios::binary);
        if (!inFile.is_open()) return false;

        outData.resize(st.st_size);
        if (st.st_size > 0)
            inFile.read(&outData[0], st.st_size);
        inFile.close();

        return true;
    }



   bool idf_storage_littlefs::saveFile(const std::string &path, const std::string &data) {
        if (!_ready) return false;

        std::string tmpPath = path + ".tmp";

        std::ofstream outFile(tmpPath, std::ios::binary);
        if (!outFile.is_open()) return false;

        outFile.write(data.data(), data.size());
        outFile.close();

        // Atomico
        if (rename(tmpPath.c_str(), path.c_str()) != 0) {
            unlink(tmpPath.c_str());
            return false;
        }

        return true;
    }


}
