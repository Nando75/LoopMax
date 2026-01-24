// hal/esp32/idf/idf_esp32wifi.cpp
#include "idf_esp32wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <cstring>
#include <vector>

namespace LoopMax::Core::Hal {


 
    bool idf_esp32wifi::setMode(WifiMode mode) {
        _mode = mode;
        wifi_mode_t m;
        switch(mode){
            case WifiMode::AP: m = WIFI_MODE_AP; break;
            case WifiMode::STA: m = WIFI_MODE_STA; break;
            case WifiMode::AP_STA: m = WIFI_MODE_APSTA; break;
        }
        return esp_wifi_set_mode(m) == ESP_OK;
    }

    WifiMode idf_esp32wifi::getMode() const {
        return _mode;
    }

   


         bool idf_esp32wifi::connect(const char* ssid, const char* psw) {
            if (_mode == WifiMode::AP) return false;

            _initNetifOnce();

            if (!_netif_sta) {
                _netif_sta = esp_netif_create_default_wifi_sta();
            }

            wifi_config_t cfg = {};
            if (ssid) {
                strncpy((char*)cfg.sta.ssid, ssid, sizeof(cfg.sta.ssid));
                strncpy((char*)cfg.sta.password, psw, sizeof(cfg.sta.password));
            }

            ESP_ERROR_CHECK(
                esp_wifi_set_mode((_mode == WifiMode::AP_STA) ? WIFI_MODE_APSTA : WIFI_MODE_STA)
            );
            ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &cfg));
            ESP_ERROR_CHECK(esp_wifi_start());
            ESP_ERROR_CHECK(esp_wifi_connect());

            return true;
        }




    

            bool idf_esp32wifi::startAP(const char* ssid, const char* psw) {
                if (_mode == WifiMode::STA) return false;

                _initNetifOnce();

                if (!_netif_ap) {
                    _netif_ap = esp_netif_create_default_wifi_ap();
                }

                wifi_config_t cfg = {};
                if (ssid) strncpy((char*)cfg.ap.ssid, ssid, sizeof(cfg.ap.ssid));
                if (psw) strncpy((char*)cfg.ap.password, psw, sizeof(cfg.ap.password));
                cfg.ap.max_connection = 4;
                cfg.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

                ESP_ERROR_CHECK(
                    esp_wifi_set_mode((_mode == WifiMode::AP_STA) ? WIFI_MODE_APSTA : WIFI_MODE_AP)
                );
                ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &cfg));
                ESP_ERROR_CHECK(esp_wifi_start());

                return true;
            }


      void idf_esp32wifi::stopAP() {
        if (_mode == WifiMode::AP) {
                // solo AP → ferma tutto
                esp_wifi_stop();
            } else if (_mode == WifiMode::AP_STA) {
                // AP_STA → forza modalità STA
                ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
            }
        }


    void idf_esp32wifi::scanWifi() {
        _scanDone = false;
        wifi_scan_config_t scanConf = {};
        esp_wifi_scan_start(&scanConf, false); // async
    }


        std::vector<WifiNetwork> idf_esp32wifi::getLocalWifi() const {
            return _wifiList;
        }





    void idf_esp32wifi::_eventHandler(void* arg,esp_event_base_t event_base,int32_t event_id,void* event_data) {
            if (event_base != WIFI_EVENT || event_id != WIFI_EVENT_SCAN_DONE)
                return;

            auto* self = static_cast<idf_esp32wifi*>(arg);

            uint16_t n = 0;
            esp_wifi_scan_get_ap_num(&n);

            self->_wifiList.clear();
            if (n == 0) {
                self->_scanDone = true;
                return;
            }

            std::vector<wifi_ap_record_t> ap_list(n);
            esp_wifi_scan_get_ap_records(&n, ap_list.data());

            for (uint16_t i = 0; i < n; i++) {
                WifiNetwork w;
                w.ssid = reinterpret_cast<const char*>(ap_list[i].ssid);

                w.rssi = ap_list[i].rssi;
                self->_wifiList.push_back(w);
            }

            self->_scanDone = true;
        }


          
           

                void idf_esp32wifi::_initNetifOnce() {
                    if (_netifInit) return;

                    esp_netif_init();
                    esp_event_loop_create_default();

                    _netifInit = true;
                }




}
