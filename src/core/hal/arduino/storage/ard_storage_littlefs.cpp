#include "ard_storage_littlefs.h"

namespace LoopMax::Core::Hal {

    void ard_storage_littlefs::begin() {
        _ready = LittleFS.begin(true);
    }

    bool ard_storage_littlefs::isReady() const {
        return _ready;
    }

    void ard_storage_littlefs::end() {
        LittleFS.end();
        _ready = false;
    }

    bool ard_storage_littlefs::getFile(const std::string &path, std::string &outData) {
        if (!_ready || !LittleFS.exists(path.c_str())) return false;

        File f = LittleFS.open(path.c_str(), "r");
        if (!f) return false;

        outData.clear();
        outData.reserve(f.size());

        while (f.available()) {
            outData.push_back((char)f.read());
        }

        f.close();
        return true;
    }

    bool ard_storage_littlefs::saveFile(const std::string &path, const std::string &data) {
        if (!_ready) return false;

        // File temporaneo: path + ".tmp"
        std::string tmpPath = path + ".tmp";

        File f = LittleFS.open(tmpPath.c_str(), "w");
        if (!f) return false;

        size_t written = f.write(reinterpret_cast<const uint8_t*>(data.data()), data.size());
        f.close();

        if (written != data.size()) {
            LittleFS.remove(tmpPath.c_str());
            return false;
        }

        // Rinomina atomico
        if (LittleFS.exists(path.c_str())) {
            LittleFS.remove(path.c_str());
        }

        if (!LittleFS.rename(tmpPath.c_str(), path.c_str())) return false;

        return true;
    }

}
