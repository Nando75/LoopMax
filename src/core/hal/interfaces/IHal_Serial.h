//#include "BaseConfig.h"
#pragma once
#include <cstddef>  
#include <cstdint>  
namespace LoopMax::Core::Hal {

                class IHal_Serial {
                public:
                    virtual bool begin(int baud) = 0;
                    virtual void printLn(unsigned long value) = 0;
                    virtual void printLn(const char* str) = 0;
                    virtual void printLn(char c) = 0;
                    virtual void printLn(int value) = 0;
                    
                    virtual void print(unsigned long value) = 0;
                    virtual void print(const char* str) = 0;
                    virtual void print(char c) = 0;
                    virtual void print(int value) = 0;

                    virtual bool available() = 0;
                    virtual int  read() = 0;
                    virtual void write(const uint8_t* data, size_t len) = 0;
                    virtual void write(char c) = 0;
                    virtual void flush() = 0;
                   
                    //virtual void initContext(SystemContext* ctx) = 0;
                    
                    
                    virtual ~IHal_Serial() {}
                };

}

