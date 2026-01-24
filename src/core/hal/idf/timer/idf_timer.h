
#pragma once 
#if defined(HAL_FRAMEWORK_ESP_IDF)
    #include "core/hal/interfaces/IHal_Timer.h"
    #include "esp_timer.h"

    namespace LoopMax::Core::Hal {

        class idf_timer : public IHal_Timer {
        public:
            uint64_t millis() override {
                return esp_timer_get_time() / 1000ULL;
            }
            uint64_t micros() override {
                return esp_timer_get_time();
            }

                int64_t Unix() override { 
                    if (_unix == 0) return 0;
                    return _unix + ((::millis() - _unixBaseMillis) / 1000);
                }

               std::string Timezone() override { return _timezone; }
               int64_t Tz_Offset() override { return _tzOffset; }
               void setUnix(int64_t unix) override { 
                        _unix = unix;
                        _unixBaseMillis = ::millis();
                }

               void setTimezone(const std::string& tz) override { _timezone = tz; }
               void setTzOffset(int64_t offset) override {  _tzOffset = offset; }
               
               const std::string ntpServer() const override { return SERVER_NTP; }
               const bool isSyncronized() const override { return IsSync; };
               void checkInternetTime() override
                        {
                            if (IsSync) return;
                            configTime(0, 0, SERVER_NTP);
                            time_t now = time(nullptr);
                            while (now < 24 * 3600)
                            {
                                delay(10);
                                now = time(nullptr);
                            }
                            _unix = now;
                            IsSync = true;
                        }



            private:
                    int64_t _unix = 0;
                    int64_t _tzOffset = 0;
                    std::string _timezone;
                    uint64_t _unixBaseMillis = 0;
                     bool IsSync = false;
                        
        };

    }
#endif
