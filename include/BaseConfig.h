#pragma once


 #ifdef _DEBUG
    #define IS_DEBUG true 
#else
    #define IS_DEBUG false 
#endif

//#define IS_DEBUG true 
//LOGS:
#define DEBUG_MAX_LOGS 150
#define RELEASE_MAX_LOGS 50

//TASK (LOOP) SPEEDS (mS)
#define SPEED_SERVICES 20

//RAM
#define BYSIZE_MAX 2048  
#define BYSIZE_MED   512       
#define BYSIZE_MIN   128        

//PIN RESET
#define PIN_RESET 4           // Reset btn pin
#define RESET_HOLD_TIME 10000 // (mS) 10 seconds

//GENERAL SETTINGS:
#define SERVER_NTP "pool.ntp.org"


