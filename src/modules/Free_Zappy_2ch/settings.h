#pragma once
#include "common/types.h"  // dove c'è modulePin e PinMode
#include "core/ModuleRegistry.h"
#include "common/ModuleContext.h"


#define MODULE_VERSION "1.0.0"
#define MODULE_CODE "dev_code_here"
#define MODULE_NAME "Zappy2"
#define MODULE_DEVICE_NAME "Office Plugs"
#define MODULE_UI_JS "zappy/zappy2.js"
#define MODULE_COMPANY "SmartiLab"
#define MODULE_COMPANY_URL "https://SmartiLab.it"

#define MODULE_HTTPS_SHOW true
#define MODULE_HTTPS_API_URL "https://your-api-here"
#define MODULE_HTTPS_PORT 443
#define MODULE_HTTPS_USR ""
#define MODULE_HTTPS_PSW ""
#define MODULE_HTTPS_USE_CA true
#define MODULE_HTTPS_TIME_OUT 5000

#define MODULE_SHOW_MQTT true
#define MODULE_MQTT_USE_CA true
#define MODULE_MQTT_SERVER ""
#define MODULE_MQTT_PORT 0
#define MODULE_MQTT_USER ""
#define MODULE_MQTT_PSW ""
#define MODULE_MQTT_TIMEOUT 3000
#define MODULE_PIN_COUNT 2
// numero pin
#define MODULE_PIN_0_NAME "Line 1"
#define MODULE_PIN_0_NUMBER 22
#define MODULE_PIN_0_MODE   PinMode::Out
#define MODULE_PIN_1_NAME "Line 2"
#define MODULE_PIN_1_NUMBER 23
#define MODULE_PIN_1_MODE   PinMode::Out


//CERTIFICATES
#define MODULE_HTTPS_CA \
"-----BEGIN CERTIFICATE-----\n" \
"-----END CERTIFICATE-----\n"

#define MODULE_MQTT_CA \
"-----BEGIN CERTIFICATE-----\n" \
"-----END CERTIFICATE-----\n" \
"-----BEGIN CERTIFICATE-----\n" \
"-----END CERTIFICATE-----\n"

