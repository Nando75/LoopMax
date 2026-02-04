#pragma once
#include <stdint.h>
#include <cstddef>
#include <vector>
#include "common/types.h"

namespace LoopMax::Utils {
    using namespace Types;
/*
            class JsonManager {
            public:
                static void logToJson(char* out, size_t outSize,
                                const char* message,
                                const char* logType,
                                const char* logIcon,
                                const char* source,
                                const char* sourceIcon,
                                uint64_t millis);
            
                };
*/


            class Helper {
                    public:
                        static int getRandomNumber(int max);
                        static void getRandomString(char* out, size_t length);
                        static void addPin(std::vector<modulePin>& pins,std::string name,uint32_t number,PinMode mode,PinLevel level = PinLevel::Low);
                        static bool isNumeric(const std::string& s);
                        static bool toInt(const std::string& s, int& out);
                        static bool toInt64(const std::string& s, int64_t& out);
                        static bool toInt(const std::string& s, int& out, int min, int max);
                        static bool toInt64(const std::string& s, int64_t& out, int64_t min, int64_t max);


                    private:
                        static uint32_t getRandomU32();

            
            };






}
