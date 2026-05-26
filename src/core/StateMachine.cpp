#include <Arduino.h>
#include <LittleFS.h>
#include "core/StateMachine.h"
#include "core/Queue.h"
#include "core/RuntimeStorage.h"
#include "core/DeviceManager.h"
//#include "core/PrinterManager.h"
#include "config/Config.h"
#include "config/ConfigHandler.h"
#include "config/UserHandler.h"
#include "tasks/TaskUI.h"
#include "io/KeypadMultiKey.h"
#include "io/LCD.h"
#include "io/GPS.h"
#include "services/TimeService.h"
#include "ui/Panel.h"
#include "ui/ConfigMenu.h"
#include "ui/Meter.h"
#include "ui/Receipt.h"
#include "ui/DeviceStatus.h"
#include "ui/Net.h"
#include "ui/User.h"
#include "ui/OperationMenu.h"

static AppState currentState;

void StateMachine_Init() {
    currentState = STATE_HOME;
}

AppState StateMachine_GetState() {
    return currentState;
}

void StateMachine_SetState(AppState newState) {
    currentState = newState;
}



bool StateMachine_IsTransactionRunning()
{
    AppState s = currentState;

    return (s == STATE_EXECUTING_TRANSACTION ||
            s == STATE_STARTING_TRANSACTION);
}

void copyToBuffer(char* dest, uint8_t &len, const char* src, uint8_t maxLen)
{
    len = 0;

    while (src[len] != '\0' && len < maxLen - 1) {
        dest[len] = src[len];
        len++;
    }

    dest[len] = '\0';
}


void StateMachine_HandleEvent(Event *evt) {
    //Serial.println("STATE: " + String(currentState) + ", EVENT: " + String(evt->type) + ", DATA: " + String((char)evt->payload[0]));  // 🔥 DEBUG

    uint8_t currentPage = 0;  
    static char inputUserId[16] = {0};
    static uint8_t inputUserIdLen = 0;
        
    static char inputPassword[16] = {0};
    static uint8_t inputPasswordLen = 0;
    
    static char inputKFactor[16] = {0};
    static uint8_t inputKFactorLen = 0;

    static char inputCFactor[16] = {0};
    static uint8_t inputCFactorLen = 0;
   
    static char inputUnitNo[16] = {0};
    static uint8_t inputUnitNoLen = 0;

    static char inputOdometer[16] = {0};
    static uint8_t inputOdometerLen = 0;

    static char inputDateTime[16] = {0};
    static uint8_t inputDateTimeLen = 0;

    static char inputAppName[21] = {0};
    static uint8_t inputAppNameLen = 0;

    static char inputPanelId[16] = {0};
    static uint8_t inputPanelIdLen = 0;

    static char inputCountryCode[21] = {0};
    static uint8_t inputCountryCodeLen = 0;

    static char inputLocation[16] = {0};
    static uint8_t inputLocationLen = 0;

    static char inputCompanyCode[16] = {0};
    static uint8_t inputCompanyCodeLen = 0;

    static char inputHeader1[21] = {0};
    static uint8_t inputHeader1Len = 0;

    static char inputHeader2[21] = {0};
    static uint8_t inputHeader2Len = 0;

    static char inputFooter1[21] = {0};
    static uint8_t inputFooter1Len = 0;

    static char inputFooter2[21] = {0};
    static uint8_t inputFooter2Len = 0;

    static char inputModemStatus[8] = {0};
    static uint8_t inputModemStatusLen = 0;

    static char inputRfidStatus[8] = {0};
    static uint8_t inputRfidStatusLen = 0;

    static char inputGpsStatus[8] = {0};
    static uint8_t inputGpsStatusLen = 0;

    static char inputIp[21] = {0};
    static uint8_t inputIpLen = 0;

    static char inputSsid[21] = {0};
    static uint8_t inputSsidLen = 0;

    static char inputSsidUser[21] = {0};
    static uint8_t inputSsidUserLen = 0;

    static char inputSsidPassword[21] = {0};
    static uint8_t inputSsidPasswordLen = 0;

    static char inputHostPort[8] = {0};
    static uint8_t inputHostPortLen = 0;

    static char inputMqttUser[21] = {0};
    static uint8_t inputMqttUserLen = 0;

    static char inputMqttPassword[21] = {0};
    static uint8_t inputMqttPasswordLen = 0;

    static char inputMqttTopic[21] = {0};
    static uint8_t inputMqttTopicLen = 0;

    static char inputSdThreshold[8] = {0};
    static uint8_t inputSdThresholdLen = 0;

    static char inputPulserChannel[8] = {0};
    static uint8_t inputPulserChannelLen = 0;

    static char inputUser[21] = {0};
    static uint8_t inputUserLen = 0;

    switch (currentState) {

        case STATE_HOME:

        {
            static uint8_t starCount = 0;
            static unsigned long lastPress = 0;

            if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];
                unsigned long now = millis();
                if (key == '*') {

                    if (now - lastPress < 1000) {
                        starCount++;
                    } else {
                        starCount = 1;
                    }

                    lastPress = now;

                    if (starCount >= 4) {
                        starCount = 0;
                        KeypadMultiKey_Clear();
                        currentState = STATE_TECHNITION_LOGIN;
                        break;
                    }

                } else {
                    starCount = 0;
                }

                if (key == 'D') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHIFT;
                    break;
                }
            }
        
            break;
        }

        case STATE_TECHNITION_LOGIN:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    inputPasswordLen = 0;
                    inputPassword[0] = '\0';
                    currentState = STATE_HOME;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputPassword, inputPasswordLen, typed, sizeof(inputPassword));

                    User* user = User_Find("tk1", inputPassword);

                    if (user != nullptr) {
                         KeypadMultiKey_Clear();
                         currentState = STATE_TECHNITION_MENU;
                         break;
                    } else {
                        showError("ACCESS DENIED");
                        KeypadMultiKey_Clear();
                        currentState = STATE_HOME;
                        break;
                    }
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
            const char* typed = KeypadMultiKey_GetTypedText();
            char preview = KeypadMultiKey_GetPreviewChar();


        handle_CalibrationPasswordInput(typed,preview);

        break;

        }

        case STATE_TECHNITION_MENU:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                
                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MENU;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_CONF;
                    break;
                }
 

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_HOME;
                    break;
                }
            }
            break;
        }

        case STATE_CALIB_MENU:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((calibPage + 1) * 4 < KALIBRASI_ITEM_COUNT) {

                        calibPage++;

                    } else {

                        // kembali ke page awal
                        calibPage = 0;
                    }

                    UI_RenderCalibMenu(calibPage);

                    break;
                }
              

                if (key == '1') {
                    KeypadMultiKey_Clear();
                     currentState = STATE_CALIB_PANEL;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_STRUCK;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_METER;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_DEVICE_STATUS;
                    break;
                }

                 if (key == '6') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_USER;
                    break;
                }


                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_TECHNITION_MENU;
                    break;
                }
            }
            break;
        }

         case STATE_SHOW_CONF:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((calibPage + 1) * 4 < KALIBRASI_ITEM_COUNT) {

                        calibPage++;

                    } else {

                        // kembali ke page awal
                        calibPage = 0;
                    }

                    UI_RenderCalibMenu(calibPage);

                    break;
                }

                if (key == '1') {
                    KeypadMultiKey_Clear();
                     currentState = STATE_SHOW_PANEL;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_STRUCK;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_NET;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_METER;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_DEVICE_STATUS;
                    break;
                }

                if (key == '6') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_USER;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_TECHNITION_MENU;
                    break;
                }
            }
            break;
        }

        //###################################################

        case STATE_CALIB_PANEL:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((panelPage + 1) * 4 < PANEL_ITEM_COUNT) {

                        panelPage++;

                    } else {

                        // kembali ke page awal
                        panelPage = 0;
                    }


                    UI_RenderPanelMenu(panelPage);

                    break;
                }

                 if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_DATE_TIME;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_APP_NAME;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_ID_PANEL;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_COUNTRY_CODE;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_COMPANY_CODE;
                    break;
                }

                if (key == '6') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_LOCATION;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_MENU;
                    break;
                }
            }
            break;
        }

        case STATE_CALIB_STRUCK:
        {
             if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((struckPage + 1) * 4 < STRUCK_ITEM_COUNT) {

                        struckPage++;

                    } else {

                        // kembali ke page awal
                        struckPage = 0;
                    }

              
                    UI_RenderStruckMenu(struckPage);

                    break;
                }

                 if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_HEADER1;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_HEADER2;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_FOOTER1;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_FOOTER2;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SLIP_NUMBER;
                    break;
                }


                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_MENU;
                    break;
                }
            }
            break;
        }

        case STATE_CALIB_NET:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((netPage + 1) * 4 < NET_ITEM_COUNT) {

                        netPage++;

                    } else {

                        // kembali ke page awal
                        netPage = 0;
                    }


                    UI_RenderNetMenu(netPage);

                    break;
                }
                    
                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_IP;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SSID;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SSID_USER;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SSID_PASSWORD;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MQTT_PORT;
                    break;
                }

                if (key == '6') {
                    KeypadMultiKey_Clear();
                    //currentState = STATE_SHOW_NET;
                    currentState = STATE_CALIB_MQTT_USER;
                    break;
                }

                if (key == '7') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_MQTT_PASSWORD;
                    break;
                }


                if (key == '8') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MQTT_TOPIC;
                    break;
                }

                
                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_MENU;
                    break;
                }
            }
            break;
        }

        case STATE_CALIB_METER:
        {
             if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((meterPage + 1) * 4 < METER_ITEM_COUNT) {

                        meterPage++;

                    } else {

                        // kembali ke page awal
                        meterPage = 0;
                    }

                    UI_RenderMeterMenu(meterPage);

                    break;
                }

                    
                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_CALIB_K_FACTOR;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_CALIB_C_FACTOR;
                    break;
                }

               
                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SD_THRESHOLD;
                    break;
                }


                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_PULSER_CHANNEL;
                    break;
                }

                 if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_RESET_TOTALISER;
                    break;
                }

              
                

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_MENU;
                    break;
                }
            }
            break;
        }

        case STATE_CALIB_DEVICE_STATUS:
        {
           if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((devicePage + 1) * 4 < DEVICE_ITEM_COUNT) {

                        devicePage++;

                    } else {

                        // kembali ke page awal
                        devicePage = 0;
                    }
              
                    UI_RenderDeviceMenu(devicePage);

                    break;
                }

                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_CALIB_RFID_STATUS;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_CALIB_MODEM_STATUS;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_GPS_STATUS;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_MENU;
                    break;
                }
            }
            break;
        }

        //#########################################

        case STATE_SHOW_PANEL:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((panelPage + 1) * 4 < PANEL_ITEM_COUNT) {

                        panelPage++;

                    } else {

                        // kembali ke page awal
                        panelPage = 0;
                    }

                    UI_RenderPanelMenu(panelPage);

                    break;
                }

                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_DATE_TIME;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_APP_NAME;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_ID_PANEL;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_COUNTRY_CODE;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_COMPANY_CODE;
                    break;
                }

                if (key == '6') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_LOCATION;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_SHOW_CONF;
                    break;
                }
            }
            break;
        }

        case STATE_SHOW_STRUCK:
        {
             if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((struckPage + 1) * 4 < STRUCK_ITEM_COUNT) {

                        struckPage++;

                    } else {

                        // kembali ke page awal
                        struckPage = 0;
                    }

                    UI_RenderStruckMenu(struckPage);

                    break;
                }

                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_HEADER1;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_HEADER2;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_FOOTER1;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_FOOTER2;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_SLIP_NUMBER;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_SHOW_CONF;
                    break;
                }
            }
            break;
        }

        case STATE_SHOW_NET:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((netPage + 1) * 4 < NET_ITEM_COUNT) {

                        netPage++;

                    } else {

                        // kembali ke page awal
                        netPage = 0;
                    }

                    UI_RenderNetMenu(netPage);

                    break;
                }

                                  
                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_IP;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_SSID;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_SSID_USER;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_SSID_PASSWORD;
                    break;
                }


                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_MQTT_PORT;
                    break;
                }

                if (key == '6') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_MQTT_USER;
                    break;
                }

                if (key == '7') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_SHOW_MQTT_PASSWORD;
                    break;
                }


                if (key == '8') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_MQTT_TOPIC;
                    break;
                }


                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_SHOW_CONF;
                    break;
                }
            }
            break;
        }

        case STATE_SHOW_METER:
        {
             if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((meterPage + 1) * 4 < METER_ITEM_COUNT) {

                        meterPage++;

                    } else {

                        // kembali ke page awal
                        meterPage = 0;
                    }

                    UI_RenderMeterMenu(meterPage);

                    break;
                }

                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_SHOW_K_FACTOR;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_SHOW_C_FACTOR;
                    break;
                }


                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_SD_THRESHOLD;
                    break;
                }


                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_PULSER_CHANNEL;
                    break;
                }

                if (key == '5') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_RESET_TOTALISER;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_SHOW_CONF;
                    break;
                }
            }
            break;
        }

        case STATE_SHOW_DEVICE_STATUS:
        {
           if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((deviceStatusPage + 1) * 4 < DEVICE_STATUS_ITEM_COUNT) {

                        deviceStatusPage++;

                    } else {

                        // kembali ke page awal
                        deviceStatusPage = 0;
                    }

              
                    UI_RenderDeviceStatusrMenu(deviceStatusPage);

                    break;
                }

                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_SHOW_RFID_STATUS;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_SHOW_MODEM_STATUS;
                    break;
                }

                if (key == '3') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHOW_GPS_STATUS;
                    break;
                }

                if (key == '4') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_WIFI_STATUS;
                    break;
                }

              

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_SHOW_CONF;
                    break;
                }
            }
            break;
        }

        //###############################
        case STATE_SHOW_DATE_TIME:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_PANEL;
                        break;
                    }
             }
            break;
        }


        case STATE_SHOW_APP_NAME:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_PANEL;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_ID_PANEL:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_PANEL;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_COUNTRY_CODE:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_PANEL;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_COMPANY_CODE:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_PANEL;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_LOCATION:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_PANEL;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_K_FACTOR:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_METER;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_C_FACTOR:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_METER;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_RESET_TOTALISER:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_METER;
                        break;
                    }
             }
            break;
        }

          case STATE_SHOW_SD_THRESHOLD:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_METER;
                        break;
                    }
             }
            break;
        }

          case STATE_SHOW_PULSER_CHANNEL:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_METER;
                        break;
                    }
             }
            break;
        }


        case STATE_SHOW_SLIP_NUMBER:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_STRUCK;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_HEADER1:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_STRUCK;
                        break;
                    }
             }
            break;
        }

         case STATE_SHOW_HEADER2:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_STRUCK;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_FOOTER1:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_STRUCK;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_FOOTER2:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_STRUCK;
                        break;
                    }
             }
            break;
        }
      
        // case STATE_SHOW_USER:    
        // {
        //     if (evt->type == EVT_KEYPAD) {
        //         char key = (char)evt->payload[0];

        //         if (key == '*') {
        //                 KeypadMultiKey_Clear();
        //                 currentState =STATE_SHOW_CONF;
        //                 break;
        //             }
        //      }
        //     break;
        // }

        case STATE_SHOW_USER:
        {
            if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];

                // ===== NEXT PAGE =====
                if (key == 'D') {

                    
                    UI_UserNextPage();
                    UI_RenderUserShow();

                    break;
                }

                 if (key == '2') {

                    UI_UserPrevSelection();
                    UI_RenderUserShow();
                    break;
                }

                if (key == '8') {

                    UI_UserNextSelection();
                    UI_RenderUserShow();
                    break;
                }

                // ===== BACK =====
                if (key == '*') {
                        KeypadMultiKey_Clear();
                        UI_ResetUserPage();
                        currentState =STATE_SHOW_CONF;
                        break;
                    }
            }

            break;
        }
        
        case STATE_SHOW_RFID_STATUS:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_DEVICE_STATUS;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_MODEM_STATUS:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_DEVICE_STATUS;
                        break;
                    }
             }
            break;
        }

        case STATE_SHOW_GPS_STATUS:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_DEVICE_STATUS;
                        break;
                    }
             }
            break;
        }

        case STATE_WIFI_STATUS:
        {   
             if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_DEVICE_STATUS;
                        break;
                    }
             }
            break;
        }

            case STATE_SHOW_IP:
            {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }
            
            case STATE_SHOW_SSID:
             {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }
            case STATE_SHOW_SSID_USER:
             {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }
            case STATE_SHOW_SSID_PASSWORD:
             {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }
            case STATE_SHOW_MQTT_PORT:
             {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }
            case STATE_SHOW_MQTT_USER:
             {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }
            case STATE_SHOW_MQTT_PASSWORD:
             {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }
            case STATE_SHOW_MQTT_TOPIC:
             {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                        KeypadMultiKey_Clear();
                        currentState =STATE_SHOW_NET;
                        break;
                    }
             }
             break;
            }

        //################################
        //Panel Kalibrasi

        case STATE_CALIB_DATE_TIME:
        {
            if (evt->type == EVT_KEYPAD) {

            char key = (char)evt->payload[0];

            bool done = handle_DateTimeInput(key);

            if (done) {

                currentState =
                    STATE_CALIB_DATE_TIME_SUCCESS;
            }

             if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_PANEL;
                    break;
                }
        }


            break;
        }

        case STATE_CALIB_APP_NAME:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_PANEL;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputAppName, inputAppNameLen, typed, sizeof(inputAppName));

                    Config::SetAppName(inputAppName);
                    ConfigHandler::ConfigHandler_UpdateValue("APP_NAME",Config::GetAppName());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_APP_NAME_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_AppNameInput(typed,preview,inputAppName);

        break;

        }

        case STATE_CALIB_ID_PANEL:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_PANEL;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputPanelId, inputPanelIdLen, typed, sizeof(inputPanelId));
              
                

                    uint8_t val = (uint8_t) strtoul(inputPanelId, nullptr, 10);
                    
                    if (val <= 0 || val > 100) {
                        inputPanelId[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_ID_PANEL_FAILED;
                        break;
                    }

                    char panelStr[8];

                    snprintf(
                        panelStr,
                        sizeof(panelStr),
                        "%u",
                        val
                    );


                    Config::SetPanelId(val);
                    ConfigHandler::ConfigHandler_UpdateValue("ID_PANEL",panelStr);
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_ID_PANEL_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_IdPanelInput(typed,preview,inputPanelId);

        break;

        }

        case STATE_CALIB_COUNTRY_CODE:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_PANEL;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputCountryCode, inputCountryCodeLen, typed, sizeof(inputCountryCode));

                    if (inputCountryCodeLen == 0  || inputCountryCodeLen > 3) {
                        inputCountryCode[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_COUNTRY_CODE_FAILED;
                        break;
                    }

                    Config::SetCountryCode(inputCountryCode);
                    ConfigHandler::ConfigHandler_UpdateValue("COUNTRY_CODE",Config::GetCountryCode());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_COUNTRY_CODE_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_CountryCodeInput(typed,preview,inputCountryCode);

        break;

        }

        case STATE_CALIB_COMPANY_CODE:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_PANEL;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputCompanyCode, inputCompanyCodeLen, typed, sizeof(inputCompanyCode));

                    if (inputCompanyCodeLen == 0 ) {
                        inputCompanyCode[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_COMPANY_CODE_FAILED;
                        break;
                    }
              
                    Config::SetCompanyCode(inputCompanyCode);
                    ConfigHandler::ConfigHandler_UpdateValue("COMPANY_CODE",Config::GetCompanyCode());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_COMPANY_CODE_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_CompanyCodeInput(typed,preview,inputCompanyCode);

        break;
        }

        case STATE_CALIB_LOCATION:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_PANEL;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputLocation, inputLocationLen, typed, sizeof(inputLocation));

                    if (inputLocationLen == 0 ) {
                        inputLocation[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_LOCATION_FAILED;
                        break;
                    }
              
                    Config::SetLocation(inputLocation);
                    ConfigHandler::ConfigHandler_UpdateValue("LOCATION",Config::GetLocation());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_LOCATION_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_LocationInput(typed,preview,inputLocation);

        break;
        }

         //Receipt Kalibrasi
        case STATE_CALIB_SLIP_NUMBER:
       
        {
               static char buffer[16] = {0};
               static uint8_t bufferLen = 0;

                if (evt->type == EVT_KEYPAD) {

                    char key = (char)evt->payload[0];

                    // CANCEL
                    if (key == '*') {
                        buffer[0] = '\0';
                        bufferLen = 0;
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_STRUCK;
                        break;
                    }

                    //HANDLE INPUT
                    if (handle_SlipNumberInput(key, buffer, bufferLen, sizeof(buffer))) {

                        // ===== VALIDASI =====
                        if (bufferLen == 0) {
                            KeypadMultiKey_Clear();
                            currentState = STATE_CALIB_SLIPNUMBER_FAILED;
                            break;
                        }

                        if (bufferLen >= 10) {
                            buffer[0] = '\0';
                            bufferLen = 0;
                            KeypadMultiKey_Clear();
                            currentState = STATE_CALIB_SLIPNUMBER_FAILED;
                            break;
                        }

                        // ===== CONVERT =====
                        uint32_t val = strtod(buffer, NULL);

                        // ===== VALIDASI NILAI =====
                        if (val <= 0) {
                            KeypadMultiKey_Clear();
                             currentState = STATE_CALIB_SLIPNUMBER_FAILED;
                            break;
                        }

                        // ===== SIMPAN =====
                        RuntimeStorage::setSlip(val);

                        // ===== RESET BUFFER =====
                        buffer[0] = '\0';
                        bufferLen = 0;


                        // ===== NEXT STATE =====
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_SLIPNUMBER_SUCCESS;
                        break;
                    }
                }

                break;

        }


        case STATE_CALIB_HEADER1:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_STRUCK;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputHeader1, inputHeader1Len, typed, sizeof(inputHeader1));

                    if (inputHeader1Len == 0 ) {
                        inputHeader1[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_HEADER1_FAILED;
                        break;
                    }
              
                    Config::SetHeader(inputHeader1);
                    ConfigHandler::ConfigHandler_UpdateValue("HEADER",Config::GetHeader());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_HEADER1_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_Header1Input(typed,preview,inputHeader1);
        break;
        }


        case STATE_CALIB_HEADER2:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_STRUCK;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputHeader2, inputHeader2Len, typed, sizeof(inputHeader2));

                    if (inputHeader2Len == 0 ) {
                        inputHeader2[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_HEADER2_FAILED;
                        break;
                    }
              
                    Config::SetHeader2(inputHeader2);
                    ConfigHandler::ConfigHandler_UpdateValue("HEADER2",Config::GetHeader2());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_HEADER2_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_Header2Input(typed,preview,inputHeader2);
        break;
        }


        case STATE_CALIB_FOOTER1:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_STRUCK;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputFooter1, inputFooter1Len, typed, sizeof(inputFooter1));

                    if (inputFooter1Len == 0 ) {
                        inputFooter1[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_FOOTER1_FAILED;
                        break;
                    }
              
                    Config::SetFooter(inputFooter1);
                    ConfigHandler::ConfigHandler_UpdateValue("FOOTER",Config::GetFooter());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_FOOTER1_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_Footer1Input(typed,preview,inputFooter1);
        break;
        }

        case STATE_CALIB_FOOTER2:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_STRUCK;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputFooter2, inputFooter2Len, typed, sizeof(inputFooter2));

                    if (inputFooter2Len == 0 ) {
                        inputFooter2[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_FOOTER2_FAILED;
                        break;
                    }
              
                    Config::SetFooter2(inputFooter2);
                    ConfigHandler::ConfigHandler_UpdateValue("FOOTER2",Config::GetFooter2());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_FOOTER2_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_Footer2Input(typed,preview,inputFooter2);
        break;
        }

        // Net Kalibrasi
        case STATE_CALIB_IP:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputIp, inputIpLen, typed, sizeof(inputIp));

                    if (inputIpLen == 0 ) {
                        inputIp[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_IP_FAILED;
                        break;
                    }
              
                    Config::SetIP(inputIp);
                    ConfigHandler::ConfigHandler_UpdateValue("IP",Config::GetIP());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_IP_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_IpInput(typed,preview,inputIp);
        break;
        }

        case STATE_CALIB_SSID:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputSsid, inputSsidLen, typed, sizeof(inputSsid));

                    if (inputSsidLen == 0 ) {
                        inputSsid[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_SSID_FAILED;
                        break;
                    }
              
                    Config::SetSSID(inputSsid);
                    ConfigHandler::ConfigHandler_UpdateValue("SSID",Config::GetSSID());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SSID_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_SsidInput(typed,preview,inputSsid);
        break;
        }

        case STATE_CALIB_SSID_USER:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputSsidUser, inputSsidUserLen, typed, sizeof(inputSsidUser));

                    if (inputSsidUserLen == 0 ) {
                        inputSsidUser[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_SSID_USER_FAILED;
                        break;
                    }
              
                    Config::SetSSIDUser(inputSsidUser);
                    ConfigHandler::ConfigHandler_UpdateValue("SSID_USER",Config::GetSSIDUser());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SSID_USER_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_SsidUserInput(typed,preview,inputSsidUser);
        break;
        }

        case STATE_CALIB_SSID_PASSWORD:
         {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputSsidPassword, inputSsidPasswordLen, typed, sizeof(inputSsidPassword));

                    if (inputSsidPasswordLen == 0 ) {
                        inputSsidPassword[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_SSID_PASSWORD_FAILED;
                        break;
                    }
              
                    Config::SetSSIDPassword(inputSsidPassword);
                    ConfigHandler::ConfigHandler_UpdateValue("SSID_PASSWORD",Config::GetSSIDPassword());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SSID_PASSWORD_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_SsidPasswordInput(typed,preview,inputSsidPassword);
        break;
        }

        case STATE_CALIB_MQTT_PORT:
         {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();
                   
                    copyToBuffer(inputHostPort, inputHostPortLen, typed, sizeof(inputHostPort));
                    
                 
                     uint16_t val = (uint16_t) strtoul(inputHostPort, nullptr,10);

                    if (val <= 0) {
                        inputHostPort[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_MQTT_PORT_FAILED;
                        break;
                    }
                    char hostPortStr[8];

                    snprintf(
                        hostPortStr,
                        sizeof(hostPortStr),
                        "%u",
                        val
                    );

                     
                    Config::SetHostPort(val);
                    ConfigHandler::ConfigHandler_UpdateValue("HOST_PORT",hostPortStr);
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MQTT_PORT_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_HostPortInput(typed,preview,inputHostPort);
        break;
        }

        case STATE_CALIB_MQTT_USER:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputMqttUser, inputMqttUserLen, typed, sizeof(inputMqttUser));

                    if (inputMqttUserLen == 0 ) {
                        inputMqttUser[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_MQTT_USER_FAILED;
                        break;
                    }
              
                    Config::SetMQTTUser(inputMqttUser);
                    ConfigHandler::ConfigHandler_UpdateValue("MQTT_USER",Config::GetMQTTUser());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MQTT_USER_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_MqttUserInput(typed,preview,inputMqttUser);
        break;
        }

        case STATE_CALIB_MQTT_PASSWORD:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputMqttPassword, inputMqttPasswordLen, typed, sizeof(inputMqttPassword));

                    if (inputMqttPasswordLen == 0 ) {
                        inputMqttPassword[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_SSID_PASSWORD_FAILED;
                        break;
                    }
              
                    Config::SetMQTTPassword(inputMqttPassword);
                    ConfigHandler::ConfigHandler_UpdateValue("MQTT_PASSWORD",Config::GetMQTTPassword());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MQTT_PASSWORD_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_MqttPasswordInput(typed,preview,inputMqttPassword);
        break;
        }

        case STATE_CALIB_MQTT_TOPIC:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_NET;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputMqttTopic, inputMqttTopicLen, typed, sizeof(inputMqttTopic));

                    if (inputMqttTopicLen == 0 ) {
                        inputMqttTopic[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_MQTT_TOPIC_FAILED;
                        break;
                    }
              
                    Config::SetMQTTTopic(inputMqttTopic);
                    ConfigHandler::ConfigHandler_UpdateValue("MQTT_TOPIC",Config::GetMQTTTopic());
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MQTT_TOPIC_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_MqttTopicInput(typed,preview,inputMqttTopic);
        break;
        }

         //Meter Kalibrasi
        case STATE_CALIB_K_FACTOR:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_METER;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputKFactor, inputKFactorLen, typed, sizeof(inputKFactor));
                    

                    double val = strtod(inputKFactor, nullptr);

                    if (val <= 0 || val > 1000) {
                        inputKFactor[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_KFACTOR_FAILED;
                        break;
                    }

                    char kFactorStr[32];

                    snprintf(
                        kFactorStr,
                        sizeof(kFactorStr),
                        "%.9f",
                        val
                    );

                    Config::SetKFactor(val);
                    ConfigHandler::ConfigHandler_UpdateValue("K_FAKTOR",kFactorStr);
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_KFACTOR_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_KFactorInput(typed,preview,inputKFactor);

        break;

        }

       
         case STATE_CALIB_C_FACTOR:
        {
              if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_METER;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();
                    copyToBuffer(inputCFactor, inputCFactorLen, typed, sizeof(inputCFactor));

                   

                    double val = strtod(inputCFactor, nullptr);

                    if (val <= 0 || val > 1000) {
                          inputCFactor[0] = '\0';
                          KeypadMultiKey_Clear();
                          currentState = STATE_CALIB_CFACTOR_FAILED;
                          break;
                    }

                     char cFactorStr[32];

                    snprintf(
                        cFactorStr,
                        sizeof(cFactorStr),
                        "%.9f",
                        val
                    );

                    Config::SetCFactor(val);
                    ConfigHandler::ConfigHandler_UpdateValue("C_FAKTOR",cFactorStr);
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_CFACTOR_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_CFactorInput(typed,preview,inputCFactor);
        break;
        }

        
        case STATE_CALIB_RESET_TOTALISER:
        {
               static char buffer[16] = {0};
               static uint8_t bufferLen = 0;

                if (evt->type == EVT_KEYPAD) {

                    char key = (char)evt->payload[0];

                    // CANCEL
                    if (key == '*') {
                        buffer[0] = '\0';
                        bufferLen = 0;
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_METER;
                        break;
                    }

                    //HANDLE INPUT
                    if (handle_TotaliserInput(key, buffer, bufferLen, sizeof(buffer))) {

                        // ===== VALIDASI =====
                        if (bufferLen == 0) {
                            KeypadMultiKey_Clear();
                            currentState = STATE_CALIB_TOTALISER_FAILED;
                            break;
                        }

                        if (bufferLen >= 10) {
                            buffer[0] = '\0';
                            bufferLen = 0;
                            KeypadMultiKey_Clear();
                            currentState = STATE_CALIB_TOTALISER_FAILED;
                            break;
                        }

                        // ===== CONVERT =====
                        double val = strtod(buffer, NULL);

                        // ===== VALIDASI NILAI =====
                        if (val <= 0) {
                            KeypadMultiKey_Clear();
                            currentState = STATE_CALIB_TOTALISER_FAILED;
                            break;
                        }

                        // ===== SIMPAN =====
                        RuntimeStorage::setTotaliser(val);

                        // ===== RESET BUFFER =====
                        buffer[0] = '\0';
                        bufferLen = 0;


                        // ===== NEXT STATE =====
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_TOTALISER_SUCCESS;
                        break;
                    }
                }

                break;

        }
        case STATE_CALIB_PULSER_CHANNEL:
        {
        if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_METER;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();
                   
                    copyToBuffer(inputPulserChannel, inputPulserChannelLen, typed, sizeof(inputPulserChannel));
                    
                 
                     uint8_t val = (uint8_t) strtoul(inputPulserChannel, nullptr,10);

                    if (val <= 0) {
                        inputPulserChannel[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_PULSER_CHANNEL_FAILED;
                        break;
                    }
                    char pulserChannelStr[8];

                    snprintf(
                        pulserChannelStr,
                        sizeof(pulserChannelStr),
                        "%u",
                        val
                    );

                     
                    Config::SetPulserChannel(val);
                    ConfigHandler::ConfigHandler_UpdateValue("PULSER_CHANNEL",pulserChannelStr);
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_PULSER_CHANNEL_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_PulserChannelInput(typed,preview,inputPulserChannel);
        break;
        }

        case STATE_CALIB_SD_THRESHOLD:
         {
        if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_METER;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();
                   
                    copyToBuffer(inputSdThreshold, inputSdThresholdLen, typed, sizeof(inputSdThreshold));
                    
                 
                     uint16_t val = (uint16_t) strtoul(inputSdThreshold, nullptr,10);

                    if (val <= 0) {
                        inputSdThreshold[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_SD_THRESHOLD_FAILED;
                        break;
                    }
                    char sdThresholdStr[8];

                    snprintf(
                        sdThresholdStr,
                        sizeof(sdThresholdStr),
                        "%u",
                        val
                    );

                     
                    Config::SetSlowDownThreshold(val);
                    ConfigHandler::ConfigHandler_UpdateValue("SLOW_DOWN_THRESHOLD",sdThresholdStr);
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_SD_THRESHOLD_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_SDThresholdInput(typed,preview,inputSdThreshold);
        break;
        }


         //Device Status Kalibrasi
        case STATE_CALIB_GPS_STATUS:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_DEVICE_STATUS;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputGpsStatus, inputGpsStatusLen, typed, sizeof(inputGpsStatus));

                    if (inputGpsStatusLen == 0 ) {
                        inputGpsStatus[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_GPS_STATUS_FAILED;
                        break;
                    }

                      // VALIDASI HANYA 0 / 1
                    if (
                        strcmp(inputGpsStatus, "0") != 0 &&
                        strcmp(inputGpsStatus, "1") != 0
                    ) {

                        inputGpsStatus[0] = '\0';

                        KeypadMultiKey_Clear();

                        currentState =
                            STATE_CALIB_GPS_STATUS_FAILED;

                        break;
                    }
                      // CONVERT KE BOOL
                        bool gpsStatus = strcmp(inputGpsStatus, "1") == 0;

              
                    Config::SetGPSStatus(gpsStatus);
                    ConfigHandler::ConfigHandler_UpdateValue("GPS_STATUS",Config::GetGPSStatus() ? "1" : "0");
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_GPS_STATUS_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_GpsStatusInput(typed,preview,inputGpsStatus);
        break;
        }

        case STATE_CALIB_MODEM_STATUS:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_DEVICE_STATUS;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputModemStatus, inputModemStatusLen, typed, sizeof(inputModemStatus));

                    if (inputModemStatusLen == 0 ) {
                        inputModemStatus[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_MODEM_STATUS_FAILED;
                        break;
                    }

                      // VALIDASI HANYA 0 / 1
                    if (
                        strcmp(inputModemStatus, "0") != 0 &&
                        strcmp(inputModemStatus, "1") != 0
                    ) {

                        inputModemStatus[0] = '\0';

                        KeypadMultiKey_Clear();

                        currentState =
                            STATE_CALIB_MODEM_STATUS_FAILED;

                        break;
                    }
                      // CONVERT KE BOOL
                        bool ModemStatus = strcmp(inputModemStatus, "1") == 0;

              
                    Config::SetModemStatus(ModemStatus);
                    ConfigHandler::ConfigHandler_UpdateValue("MODEM_STATUS",Config::GetModemStatus() ? "1" : "0");
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_MODEM_STATUS_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_ModemStatusInput(typed,preview,inputModemStatus);
        break;
        }

        case STATE_CALIB_RFID_STATUS:
        {
         if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_DEVICE_STATUS;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputRfidStatus, inputRfidStatusLen, typed, sizeof(inputRfidStatus));

                    if (inputRfidStatusLen == 0 ) {
                        inputRfidStatus[0] = '\0';
                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_RFID_STATUS_FAILED;
                        break;
                    }

                      // VALIDASI HANYA 0 / 1
                    if (
                        strcmp(inputRfidStatus, "0") != 0 &&
                        strcmp(inputRfidStatus, "1") != 0
                    ) {

                        inputRfidStatus[0] = '\0';

                        KeypadMultiKey_Clear();

                        currentState =
                            STATE_CALIB_RFID_STATUS_FAILED;

                        break;
                    }
                      // CONVERT KE BOOL
                        bool RfidStatus = strcmp(inputRfidStatus, "1") == 0;

              
                    Config::SetRFIDStatus(RfidStatus);
                    ConfigHandler::ConfigHandler_UpdateValue("RFID_STATUS",Config::GetRFIDStatus() ? "1" : "0");
                    KeypadMultiKey_Clear();
                    currentState = STATE_CALIB_RFID_STATUS_SUCCESS;
                    break;
                }
                KeypadMultiKey_ProcessKey(key);
                
            }
            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


        handle_RfidStatusInput(typed,preview,inputRfidStatus);
        break;
        }

        case STATE_CALIB_USER:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                //==NEXT Page===

                if (key == 'D') {

                    if ((userPage + 1) * 4 < USER_ITEM_COUNT) {

                        userPage++;

                    } else {

                        // kembali ke page awal
                        userPage = 0;
                    }
              
                    UI_RenderUserMenu(userPage);

                    break;
                }

                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_CALIB_USER_ADD;
                    break;
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState =  STATE_CALIB_USER_REMOVE;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState =STATE_CALIB_MENU;
                    break;
                }
            }
            break;
        }
        case STATE_CALIB_USER_ADD:
            {
                if (evt->type == EVT_KEYPAD) {

                    char key = (char)evt->payload[0];

                    if (key == '*') {

                        KeypadMultiKey_Clear();
                        currentState = STATE_CALIB_USER;
                        break;
                    }

                    // ===== SUBMIT =====
                    if (key == '#') {

                        KeypadMultiKey_Commit();

                        const char* typed =
                            KeypadMultiKey_GetTypedText();

                        copyToBuffer(
                            inputUser,
                            inputUserLen,
                            typed,
                            sizeof(inputUser)
                        );

                        // =====================================
                        // VALIDASI FORMAT:
                        // userid,username,password,level
                        // contoh:
                        // op1,operator,5678,1
                        // =====================================

                        char temp[128];

                        strncpy(temp,
                                inputUser,
                                sizeof(temp) - 1);

                        temp[sizeof(temp) - 1] = '\0';

                        // split pakai strtok
                        char* token1 = strtok(temp, ",");
                        char* token2 = strtok(NULL, ",");
                        char* token3 = strtok(NULL, ",");
                        char* token4 = strtok(NULL, ",");

                        // pastikan tepat 4 field
                        bool valid =
                            (token1 != NULL) &&
                            (token2 != NULL) &&
                            (token3 != NULL) &&
                            (token4 != NULL);

                        // tidak boleh ada field ke-5
                        if (strtok(NULL, ",") != NULL) {
                            valid = false;
                        }

                        // field kosong tidak boleh
                        if (valid) {

                            if (strlen(token1) == 0 ||
                                strlen(token2) == 0 ||
                                strlen(token3) == 0 ||
                                strlen(token4) == 0)
                            {
                                valid = false;
                            }
                        }

                        // level hanya 0-9
                        if (valid) {

                            for (size_t i = 0;
                                i < strlen(token4);
                                i++)
                            {
                                if (!isdigit(token4[i])) {
                                    valid = false;
                                    break;
                                }
                            }
                        }

                        // =====================================
                        // JIKA INVALID
                        // =====================================

                        if (!valid) {

                            inputUser[0] = '\0';

                            KeypadMultiKey_Clear();

                            currentState =
                                STATE_CALIB_USER_ADD_FAILED;

                            break;
                        }

                        // =====================================
                        // SAVE KE users.db
                        // =====================================

                        File file =
                            LittleFS.open("/users.db", "a");

                        if (!file) {

                            inputUser[0] = '\0';

                            KeypadMultiKey_Clear();

                            currentState =
                                STATE_CALIB_USER_ADD_FAILED;

                            break;
                        }

                        file.println(inputUser);

                        file.close();

                        KeypadMultiKey_Clear();

                        inputUser[0] = '\0';
                        inputUserLen = 0;

                        currentState =
                            STATE_CALIB_USER_ADD_SUCCESS;

                        break;
                    }

                    KeypadMultiKey_ProcessKey(key);
                }

                KeypadMultiKey_Update();

                const char* typed =
                    KeypadMultiKey_GetTypedText();

                char preview =
                    KeypadMultiKey_GetPreviewChar();

                handle_UserInput(
                    typed,
                    preview,
                    inputUser
                );

                break;
            }

        case STATE_CALIB_USER_REMOVE:
        {
            if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];

                if (key == '2') {

                    UI_UserPrevSelection();
                    UI_RenderUserShow();
                    break;
                }

                if (key == '8') {

                    UI_UserNextSelection();
                    UI_RenderUserShow();
                    break;
                }

                if (key == 'D') {

                    UI_UserNextPage();
                    UI_RenderUserShow();
                    break;
                }

                if (key == '#') {

                    UI_UserDeleteSelected();
                    UI_RenderUserShow();
                    break;
                }

                if (key == '*') {

                    UI_ResetUserPage();

                    currentState =
                        STATE_CALIB_USER;

                    break;
                }
            }

            break;
        }

        //###########################3


        case STATE_SHIFT:
        
        {
           if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];  // asumsi payload[0] adalah char yang ditekan

                if (key == '1') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHIFT_OPEN_USERID;
                    break; 
                }

                if (key == '2') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_SHIFT_CLOSE_USERID;
                    break;
                }

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_HOME;
                    break;
                }

            }
            break;
        }
        
        

        case STATE_SHIFT_OPEN_USERID:
        {
            if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];

                // BACK
                if (key == '*') {
                    KeypadMultiKey_Clear();
                    inputUserIdLen = 0;
                    inputUserId[0] = '\0';

                    currentState = STATE_SHIFT;
                    break;
                }

                // SUBMIT
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputUserId, inputUserIdLen, typed, sizeof(inputUserId));

                    KeypadMultiKey_Clear();
                    currentState = STATE_SHIFT_OPEN_PASSWORD;
                    break;
                }

                // proses multitap
                KeypadMultiKey_ProcessKey(key);
            }

            // update multitap (timer commit)
            KeypadMultiKey_Update();

            const char* typed = KeypadMultiKey_GetTypedText();
            char preview = KeypadMultiKey_GetPreviewChar();

            handle_UserIdInput(typed, preview);

            break;
        }
        
        

         case STATE_SHIFT_OPEN_PASSWORD:
        
        {
            if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                      inputPasswordLen = 0;
                      inputPassword[0] = '\0';

                    currentState = STATE_SHIFT;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();
            
                    copyToBuffer(inputPassword, inputPasswordLen, typed, sizeof(inputPassword));

                    User* user = User_Find(inputUserId, inputPassword);

                    if (user != nullptr) {

                        //Serial.println("LOGIN SUCCESS");

                        // ===== LOAD SHIFT DARI NVS =====
                        //ShiftData shift;
                        //RuntimeStorage::loadShift(shift);

                        // ===== CEK ADA SHIFT AKTIF =====
                        if (g_Shift.isActive) {


                            // ===== VALIDASI USER =====
                            if (strcmp(g_Shift.userId, inputUserId) == 0) {

                               // lanjutkan shift
                                KeypadMultiKey_Clear();
                                currentState = STATE_CONTINUE_SHIFT_SUCCESS;
                                break;

                            } else {
                                // tolak login
                                KeypadMultiKey_Clear();
                                currentState = STATE_OPENSHIFT_FAILED;
                                break;
                            }

                        } else {
                            // ===== BUAT SHIFT BARU =====
                            memset(&g_Shift, 0, sizeof(ShiftData));
                            uint32_t openShiftTimeUnix = Time_NowUnix();
                    
                            g_Shift.openTimestamp = openShiftTimeUnix;
                            strncpy(g_Shift.userId, inputUserId, sizeof(g_Shift.userId));
                            g_Shift.panelId = Config::GetPanelId();
                            g_Shift.startTotaliser = RuntimeStorage::getTotaliser(); 
                            g_Shift.isActive = true;

                            // ===== SIMPAN KE NVS =====
                            RuntimeStorage::saveShift(g_Shift);
                            

                            KeypadMultiKey_Clear();
                            currentState = STATE_OPENSHIFT_SUCCESS;
                            break;
                        }

                    } else {

                        Serial.println("LOGIN FAILED");

                        KeypadMultiKey_Clear();
                        currentState = STATE_OPENSHIFT_FAILED;
                        break;
                    }
                }

                KeypadMultiKey_ProcessKey(key);
            }

            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


            handle_PasswordInput(typed,preview);

            break;
        }

        case STATE_OPENSHIFT_SUCCESS:
        break;

        case STATE_OPENSHIFT_FAILED:
        break;

        case STATE_CONTINUE_SHIFT_SUCCESS:
        break;


        case STATE_TRANSACTION:
        {
            if (evt->type == EVT_KEYPAD) {
                    char key = (char)evt->payload[0];

                    if (key == '1') {
                        KeypadMultiKey_Clear();
                        currentState = STATE_INPUT_UNITNO;
                        break;
                    }

                    if (key == '2') {
                        KeypadMultiKey_Clear();
                        currentState = STATE_HOME;
                        break;
                    }

                    if (key == '3') {
                        KeypadMultiKey_Clear();
                        currentState = STATE_REPRINT;
                        break;
                    }
                }
            break;
        }

        case STATE_INPUT_UNITNO:
        {
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_TRANSACTION;
                    break;
                }

               
                if (key == '#') {
                      KeypadMultiKey_Commit();
                      const char* typed = KeypadMultiKey_GetTypedText();
                     copyToBuffer(inputUnitNo, inputUnitNoLen, typed, sizeof(inputUnitNo));

                   
                    if (inputUnitNo[0] != '\0') {
                        RuntimeStorage::Transaction_SetUnit(inputUnitNo);
                        KeypadMultiKey_Clear();
                        currentState = STATE_INPUT_UNITNO_SUCCESS;
                        break;
                       
                    } else {
                        KeypadMultiKey_Clear();
                        currentState = STATE_INPUT_UNITNO_FAILED;
                        break;
                    }
                }

                KeypadMultiKey_ProcessKey(key);
                
            }
               
            KeypadMultiKey_Update();
            const char* typed = KeypadMultiKey_GetTypedText();
            char preview = KeypadMultiKey_GetPreviewChar();

            handle_UnitNoInput(typed,preview);

        break;
        }

        case STATE_INPUT_UNITNO_SUCCESS:
        {   
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    RuntimeStorage::Transaction_SetUnit("");
                    currentState = STATE_INPUT_UNITNO;
                    break;
                }


                if (key == 'D') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_INPUT_ODOMETER;
                    break;
                }
                

            }    

            break;
        }
        
        case STATE_INPUT_ODOMETER:
        {
               static char buffer[16] = {0};
               static uint8_t bufferLen = 0;

                if (evt->type == EVT_KEYPAD) {

                    char key = (char)evt->payload[0];

                    // CANCEL
                    if (key == '*') {
                        buffer[0] = '\0';
                        bufferLen = 0;
                        RuntimeStorage::Transaction_SetUnit("");
                        currentState = STATE_INPUT_UNITNO;
                        break;
                    }

                    //HANDLE INPUT
                    if (handle_OdometerInput(key, buffer, bufferLen, sizeof(buffer))) {

                        // ===== VALIDASI =====
                        if (bufferLen == 0) {
                            currentState = STATE_INPUT_ODOMETER_FAILED;
                            break;
                        }

                        if (bufferLen >= 10) {
                            buffer[0] = '\0';
                            bufferLen = 0;
                            currentState = STATE_INPUT_ODOMETER_FAILED;
                            break;
                        }

                        // ===== CONVERT =====
                        double val = strtod(buffer, NULL);

                        // ===== VALIDASI NILAI =====
                        if (val <= 0) {
                             currentState = STATE_INPUT_ODOMETER_FAILED;
                            break;
                        }

                        // ===== SIMPAN =====
                        RuntimeStorage::Transaction_SetOdometer(val);

                        // ===== RESET BUFFER =====
                        buffer[0] = '\0';
                        bufferLen = 0;


                        // ===== NEXT STATE =====
                        currentState = STATE_INPUT_ODOMETER_SUCCESS;
                        break;
                    }
                }

          break;
        }

        case STATE_INPUT_ODOMETER_SUCCESS:
        {   
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    RuntimeStorage::Transaction_SetOdometer(0);
                    currentState = STATE_INPUT_ODOMETER;
                    break;
                }


                if (key == 'D') {
                    KeypadMultiKey_Clear();
                    currentState = STATE_INPUT_PRESET;
                    break;
                }
                

            }    

            break;
        }

        case STATE_INPUT_PRESET:
        {
               static char buffer[16] = {0};
               static uint8_t bufferLen = 0;

                if (evt->type == EVT_KEYPAD) {

                    char key = (char)evt->payload[0];

                    // CANCEL
                    if (key == '*') {
                        buffer[0] = '\0';
                        bufferLen = 0;
                        RuntimeStorage::Transaction_SetOdometer(0);
                        currentState = STATE_INPUT_ODOMETER;
                        break;
                    }

                    //HANDLE INPUT
                    if (handle_PresetInput(key, buffer, bufferLen, sizeof(buffer))) {

                        // ===== VALIDASI =====
                        if (bufferLen == 0) {
                            currentState = STATE_INPUT_PRESET_FAILED;
                            break;
                        }

                        if (bufferLen >= 10) {
                            buffer[0] = '\0';
                            bufferLen = 0;
                            currentState = STATE_INPUT_PRESET_FAILED;
                            break;
                        }

                        // ===== CONVERT =====
                        double val = strtod(buffer, NULL);

                        // ===== VALIDASI NILAI =====
                        if (val <= 0) {
                             currentState = STATE_INPUT_PRESET_FAILED;
                            break;
                        }

                        // ===== SIMPAN =====
                        RuntimeStorage::Transaction_SetPresetVolume(val);

                        // ===== RESET BUFFER =====
                        buffer[0] = '\0';
                        bufferLen = 0;


                        // ===== NEXT STATE =====
                        currentState = STATE_INPUT_PRESET_SUCCESS;
                        break;
                    }
                }

          break;
        }

        case STATE_INPUT_PRESET_SUCCESS:
        {   
            if (evt->type == EVT_KEYPAD) {
                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                    RuntimeStorage::Transaction_SetPresetVolume(0);
                    currentState = STATE_INPUT_PRESET;
                    break;
                }


                if (key == 'D') {
                    KeypadMultiKey_Clear();
                    double startTot = RuntimeStorage::getTotaliser();
                    RuntimeStorage::Transaction_SetStart(startTot);
                    
                    currentState = STATE_STARTING_TRANSACTION;
                    //pulser.startSelenoidValve();
                    break;
                }
                

            }    

            break;
        }

         case STATE_STARTING_TRANSACTION:
        {   
            break;
        }

        case STATE_EXECUTING_TRANSACTION:
        {       
               
                if (evt->type == EVT_KEYPAD) {
                        char key = (char)evt->payload[0];
    
                        if (key == '#') {
                            PrintJob job;
                            pulser.stopSelenoidValve();
                            KeypadMultiKey_Clear();
                            RuntimeStorage::Transaction_SetUserId(g_Shift.userId);
                            User* user = User_FindById(g_Shift.userId);
                            if (user) {
                                RuntimeStorage::Transaction_SetUserName(user->username);
                            } else {
                                RuntimeStorage::Transaction_SetUserName(""); // kosongkan dulu, bisa diupdate nanti
                            } 
                            RuntimeStorage::Transaction_SetPanelId(Config::GetPanelId());
                            RuntimeStorage::Transaction_SetLocation(Config::GetLocation()); // kosongkan dulu, bisa diupdate nanti
                            RuntimeStorage::Transaction_SetCompanyCode(Config::GetCompanyCode()); // kosongkan dulu, bisa diupdate nanti
                            RuntimeStorage::Transaction_SetCountryCode(Config::GetCountryCode()); // kosongkan dulu, bisa diupdate nanti

                            if (!DeviceManager::isGpsEnabled) {
                                RuntimeStorage::Transaction_SetLatitude(0);
                                RuntimeStorage::Transaction_SetLongitude(0);

                            } else {

                                if (gps.isLocationValid()) {
                                RuntimeStorage::Transaction_SetLatitude(gps.getLatitude());
                                RuntimeStorage::Transaction_SetLongitude(gps.getLongitude());
                                } else {
                                    RuntimeStorage::Transaction_SetLatitude(0);
                                    RuntimeStorage::Transaction_SetLongitude(0);
                                }

                            }
                            
                             double finish_volume =pulser.getVolume();
                             int rounded_volume;

                            double decimal = finish_volume - (int)finish_volume;

                            if (decimal > 0.4) {
                                rounded_volume = ceil(finish_volume);  // pembulatan ke atas
                            } else {
                                rounded_volume = floor(finish_volume); // pembulatan ke bawah
                            }

                            
                            //RuntimeStorage::Transaction_SetVolume(pulser.getVolume());
                            RuntimeStorage::Transaction_SetVolume(rounded_volume);
                            uint32_t currentslipNumber = RuntimeStorage::getSlip()+1;
                            RuntimeStorage::setSlip(currentslipNumber);
                            RuntimeStorage::Transaction_SetSlipNumber(RuntimeStorage::getSlip());
                            
                            
                            double currentTotaliser = (RuntimeStorage::getTotaliser()) + (RuntimeStorage::Transaction_GetVolume());
                            double endTot = currentTotaliser;
                            RuntimeStorage::setTotaliser(currentTotaliser);
                            RuntimeStorage::Transaction_SetEnd(endTot);

                            //PrinterManager::requestPrint();
                            job.type = PRINT_TRANSACTION;
                            job.trx = g_transaction;
                            PrintQueue_Send(&job);
                            //RuntimeStorage::Transaction_SaveLast();
                            RuntimeStorage::Transaction_SaveLastToFS();
                            //  jobNetwork.type = MQTT;
                            //  snprintf(
                            //     jobNetwork.mqttPayload.filename,
                            //     sizeof(jobNetwork.mqttPayload.filename),
                            //     "/trx_%06lu.json",
                            //     g_transaction.slipNumber
                            // );
                            // NetworkQueue_Send(&jobNetwork);
                            currentState = STATE_FINISHING_TRANSACTION;
                            break;
                        }
    
                    }
            break;
        }


        case STATE_FINISHING_TRANSACTION:
        {
            if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];

                if (key == 'D') currentState = STATE_TRANSACTION;
            }
           
            break;
        }
                   
        case STATE_LOG_OUT:
        { 
        break;
        }

        case STATE_REPRINT:
        {
               
            if (evt->type == EVT_KEYPAD) {
                        char key = (char)evt->payload[0];

                        if (key == '1') {
                            PrintJob job;
                            KeypadMultiKey_Clear();
                            //PrinterManager::requestRePrint();
                            TransactionData trx2;
                            RuntimeStorage::Transaction_GetLast(trx2);
                            job.type = PRINT_REPRINT;
                            job.trx = trx2;
                            PrintQueue_Send(&job);
                            currentState =  STATE_REPRINT_LAST_TRN;
                            break;
                        }

                        if (key == '2') {
                            PrintJob job;
                            KeypadMultiKey_Clear();
                            //PrinterManager::requestPrintCurrentShift();
                             ShiftData shift;
                             RuntimeStorage::Shift_GetData(shift);
                            job.type = PRINT_CURRENTSHIFT;
                            job.shift = shift;
                            PrintQueue_Send(&job);
                            currentState =  STATE_REPRINT_CURRENT_SHIFT;
                            break;
                        }

                        if (key== '*') {
                            KeypadMultiKey_Clear();
                            currentState =  STATE_TRANSACTION;
                            break;
                        }

                        
                    
                    }
                break;

        }



        case STATE_SHIFT_CLOSE_USERID:
        
        {
                if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];

                // BACK
                if (key == '*') {
                    KeypadMultiKey_Clear();
                    inputUserIdLen = 0;
                    inputUserId[0] = '\0';

                    currentState = STATE_SHIFT;
                    break;
                }

                // SUBMIT
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputUserId, inputUserIdLen, typed, sizeof(inputUserId));

                    KeypadMultiKey_Clear();
                    currentState = STATE_SHIFT_CLOSE_PASSWORD;
                    break;
                }

                // proses multitap
                KeypadMultiKey_ProcessKey(key);
            }

            // update multitap (timer commit)
            KeypadMultiKey_Update();

            const char* typed = KeypadMultiKey_GetTypedText();
            char preview = KeypadMultiKey_GetPreviewChar();

            handle_UserIdInput(typed, preview);

            break;
        } 
        
        case STATE_SHIFT_CLOSE_PASSWORD:
        
        {
             if (evt->type == EVT_KEYPAD) {

                char key = (char)evt->payload[0];

                if (key == '*') {
                    KeypadMultiKey_Clear();
                      inputPasswordLen = 0;
                      inputPassword[0] = '\0';

                    currentState = STATE_SHIFT;
                    break;
                }

                // SUBMIT PASSWORD
                if (key == '#') {
                    KeypadMultiKey_Commit();
                    const char* typed = KeypadMultiKey_GetTypedText();

                    copyToBuffer(inputPassword, inputPasswordLen, typed, sizeof(inputPassword));

                    User* user = User_Find(inputUserId, inputPassword);

                    if (user != nullptr) {


                        // ===== CEK ADA SHIFT AKTIF =====
                        if (g_Shift.isActive) {


                            // ===== VALIDASI USER =====
                            if (strcmp(g_Shift.userId, inputUserId) == 0) {

                               // close shift
                                uint32_t closeShiftTimeUnix = Time_NowUnix();
                        
                                g_Shift.closeTimestamp = closeShiftTimeUnix;
                                
                                g_Shift.endTotaliser = RuntimeStorage::getTotaliser(); 
                                g_Shift.isActive = false;

                                // ===== SIMPAN KE NVS =====
                                //PrinterManager::requestPrintlCloseShift();
                                PrintJob job;
                                ShiftData shift;
                                RuntimeStorage::Shift_GetData(shift);
                                job.type = PRINT_SHIFTREPORT;
                                job.shift = shift;
                                PrintQueue_Send(&job);
                               
                                 KeypadMultiKey_Clear();
                                currentState = STATE_CLOSESHIFT_SUCCESS;
                                break;

                            } else {
                                // tolak login
                                 KeypadMultiKey_Clear();
                                currentState = STATE_CLOSESHIFT_FAILED1;
                                break;
                            }

                        } else {
                            KeypadMultiKey_Clear();
                            currentState = STATE_CLOSESHIFT_FAILED2;
                            break;
                        }

                    } else {
                        KeypadMultiKey_Clear();
                        currentState = STATE_CLOSESHIFT_FAILED3;
                        break;
                    }
                }

                KeypadMultiKey_ProcessKey(key);
            }

            KeypadMultiKey_Update();
             const char* typed = KeypadMultiKey_GetTypedText();
             char preview = KeypadMultiKey_GetPreviewChar();


            handle_PasswordInput(typed,preview);

            break;
        }
    }
}