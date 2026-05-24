#include <Arduino.h>

#include "tasks/TaskUI.h"

#include "io/LCD.h"
#include "io/PulserEncoder.h"
#include "core/StateMachine.h"
#include "core/RuntimeStorage.h"
// /#include "config/Config.h"
#include "services/TimeService.h"
#include "ui/Panel.h"
#include "ui/ConfigMenu.h"
#include "ui/Receipt.h"
#include "ui/Meter.h"
#include "ui/DeviceStatus.h"
#include "ui/Net.h"
#include "ui/User.h"
#include "ui/OperationMenu.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


extern PulserEncoder pulser;
    bool introShown = false;
    float lastVolume = -1;
    float lastFlow = -1;
    uint32_t lastUpdateTime = 0;
    uint32_t lastFlowChangeTime = 0;
    bool firstDisplay = true;
// ==========================
// Forward
// ==========================
void UI_RenderHome();
void UI_RenderConfig();
void UI_UpdateConfigDynamic();

// ==========================

void TaskUI(void *pv) {
    static char UserId[20] = {0};
    static char UserPassword[20] = {0};
    static char TechnicianPassword[20] = {0};
    bool unameRec = false;

    LCD_Init();
    LCD_Clear();

    static int lastMinute = -1;
    static AppState lastState = (AppState)-1;
    static unsigned long lastUpdate = 0;
    static bool forceRender = false;
    
    static unsigned long stateEnterTime = 0;

    while (1) {
        unsigned long now = millis();

        AppState current = StateMachine_GetState();
       

        // // 🔥 TEST MANUAL
        // static bool testDone = false;
        // if (!testDone && current == STATE_CONFIG) {
        //     //Config::SetAppName("TEST123");
        //     testDone = true;
        //  }

        // ======================
        // RENDER SAAT STATE BERUBAH
        // ======================
        if (current != lastState || forceRender) {

            LCD_Clear();
            stateEnterTime = millis();   // 🔥 penting

            switch (current) {

                case STATE_HOME:
                    UI_RenderHome();
                    break;

                case STATE_TECHNITION_LOGIN:
                    UI_RenderCalibrationPasswordInput(TechnicianPassword); 
                    break;

                case STATE_TECHNITION_MENU:
                    UI_RenderTechnitionMenu(); 
                    break;
                
                case STATE_CALIB_MENU:
                    UI_RenderCalibMenu(calibPage);
                    break;

                case STATE_CALIB_PANEL:
                    UI_RenderPanelMenu(panelPage);
                break;

                case STATE_CALIB_DATE_TIME:
                    UI_RenderDateTimeInput();
                    break;

                case STATE_CALIB_DATE_TIME_SUCCESS:
                    showCorrect("DateTime Set Success");
                    if (now - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_MENU);
                    }
                    break;

                case STATE_CALIB_DATE_TIME_FAILED:
                    showError("DateTime Set Failed");
                    if (now - stateEnterTime > 2000) {      
                        StateMachine_SetState(STATE_CALIB_DATE_TIME);
                    }
                    break;    
                
                case STATE_CALIB_APP_NAME:
                    UI_RenderAppNameInput("");
                break;


                case STATE_CALIB_APP_NAME_SUCCESS:
                    showCorrect("APP Name Set");
                    if (now  - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_MENU);
                    }
                break;

                case STATE_CALIB_APP_NAME_FAILED:
                    showError("APP Name Fail to Set");
                    if (now  - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_PANEL);
                    }
                break;

                case STATE_CALIB_ID_PANEL:
                    UI_RenderIdPanelInput("");
                break;

                case STATE_CALIB_ID_PANEL_SUCCESS:
                    showCorrect("Id Panel Set");
                    if (now  - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_MENU);
                    }
                break;

                case STATE_CALIB_ID_PANEL_FAILED:
                    showError("Id Panel Fail to Set");
                    if (now  - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_PANEL);
                    }
                break;
                
                case STATE_CALIB_COUNTRY_CODE:
                    UI_RenderCountryCodeInput("");
                break;

                case STATE_CALIB_COUNTRY_CODE_SUCCESS:
                    showCorrect("Country Code Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_MENU);
                    }
                break;

                case STATE_CALIB_COUNTRY_CODE_FAILED:
                    showError("Country Code Failset");
                    if (now  - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_PANEL);
                    }
                break;

                case STATE_CALIB_COMPANY_CODE:
                    UI_RenderCompanyCodeInput("");
                break;
                
                case STATE_CALIB_COMPANY_CODE_SUCCESS:
                    showCorrect("Company Code Set");
                    if (now - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_MENU);
                    }
                break;

                 case STATE_CALIB_COMPANY_CODE_FAILED:
                    showError("Company Code Failset");
                    if (now  - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_PANEL);
                    }
                break;

                case STATE_CALIB_LOCATION:
                    UI_RenderLocationInput("");
                break;

                case STATE_CALIB_LOCATION_SUCCESS:
                    showCorrect("Location  Set");
                    if (now  - stateEnterTime > 2000) {
                         StateMachine_SetState(STATE_CALIB_MENU);
                    }
                break;

                case STATE_CALIB_LOCATION_FAILED:
                    showError("Location Failset");
                    if (now - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_PANEL);
                    }
                break;

                case STATE_CALIB_STRUCK:
                    UI_RenderStruckMenu(struckPage);
                break;
                
                case STATE_CALIB_HEADER1:
                    UI_RenderHeader1Input("");
                break;

                case STATE_CALIB_HEADER1_SUCCESS:
                    showCorrect("Header1 Set");
                    if (now  - stateEnterTime > 2000) {
                         StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;

                case STATE_CALIB_HEADER1_FAILED:
                    showError("Header1 Failset");
                    if (now - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;
                
                case STATE_CALIB_HEADER2:
                    UI_RenderHeader2Input("");
                break;

                case STATE_CALIB_HEADER2_SUCCESS:
                    showCorrect("Header2 Set");
                    if (now  - stateEnterTime > 2000) {
                         StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;

                case STATE_CALIB_HEADER2_FAILED:
                    showError("Header2 Failset");
                    if (now - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;
                
                case STATE_CALIB_FOOTER1:
                    UI_RenderFooter1Input("");
                break;

                case STATE_CALIB_FOOTER1_SUCCESS:
                    showCorrect("Footer1 Set");
                    if (now  - stateEnterTime > 2000) {
                         StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;

                case STATE_CALIB_FOOTER1_FAILED:
                    showError("Footer1 Failset");
                    if (now - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;
                
                case STATE_CALIB_FOOTER2:
                    UI_RenderFooter2Input("");
                break;

                case STATE_CALIB_FOOTER2_SUCCESS:
                    showCorrect("Footer2 Set");
                    if (now  - stateEnterTime > 2000) {
                         StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;

                case STATE_CALIB_FOOTER2_FAILED:
                    showError("Footer2 Failset");
                    if (now - stateEnterTime > 2000) {
                        StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;
                
                case STATE_CALIB_SLIP_NUMBER:
                    UI_RenderSlipNumberInput("");
                break;
                
                case STATE_CALIB_SLIPNUMBER_SUCCESS:
                    showCorrect("SN Update Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_STRUCK);
                    }
                break;

                case STATE_CALIB_SLIPNUMBER_FAILED:
                    showError("SN Update Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_SLIP_NUMBER);
                    }
                break;

                case STATE_CALIB_NET:
                    UI_RenderNetMenu(netPage);
                break;
                
                case STATE_CALIB_IP:
                    UI_RenderIpInput("");
                break;

                case STATE_CALIB_IP_SUCCESS:
                    showCorrect("IP Update Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_IP_FAILED:
                    showError("IP Update Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_IP);
                    }
                break;
                
                case STATE_CALIB_SSID:
                    UI_RenderSsidInput("");
                break;

                case STATE_CALIB_SSID_SUCCESS:
                    showCorrect("SSID Update Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_SSID_FAILED:
                    showError("SSID Update Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_SSID);
                    }
                break;
                
                case STATE_CALIB_SSID_USER:
                    UI_RenderSsidUserInput("");
                break;

                case STATE_CALIB_SSID_USER_SUCCESS:
                    showCorrect("SSIDUser Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_SSID_USER_FAILED:
                    showError("SSIDUser Set Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_SSID_USER);
                    }
                break;
                
                case STATE_CALIB_SSID_PASSWORD:
                    UI_RenderSsidPasswordInput("");
                break;

                    case STATE_CALIB_SSID_PASSWORD_SUCCESS:
                    showCorrect("SSIDPass Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_SSID_PASSWORD_FAILED:
                    showError("SSIDPass Set Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_SSID_PASSWORD);
                    }
                break;
                
                case STATE_CALIB_MQTT_PORT:
                    UI_RenderHostPortInput("");
                break;

                case STATE_CALIB_MQTT_PORT_SUCCESS:
                    showCorrect("Host Port Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_MQTT_PORT_FAILED:
                    showError("HostPort Set Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_MQTT_PORT);
                    }
                break;
                
                case STATE_CALIB_MQTT_USER:
                    UI_RenderMqttUserInput("");
                break;

                case STATE_CALIB_MQTT_USER_SUCCESS:
                    showCorrect("Mqtt User Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_MQTT_USER_FAILED:
                    showError("Mqtt User Set Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_MQTT_USER);
                    }
                break;
                
                case STATE_CALIB_MQTT_PASSWORD:
                    UI_RenderMqttPasswordInput("");
                break;

                case STATE_CALIB_MQTT_PASSWORD_SUCCESS:
                    showCorrect("Mqtt Password Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_MQTT_PASSWORD_FAILED:
                    showError("Mqtt Pass Set Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_MQTT_PASSWORD);
                    }
                break;
                
                case STATE_CALIB_MQTT_TOPIC:
                    UI_RenderMqttTopicInput("");
                break;

                case STATE_CALIB_MQTT_TOPIC_SUCCESS:
                    showCorrect("Mqtt Topic Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_NET);
                    }
                break;

                case STATE_CALIB_MQTT_TOPIC_FAILED:
                    showError("Mqtt Topic Set Failed");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_CALIB_MQTT_TOPIC);
                    }
                break;
                

                case STATE_CALIB_METER:
                    UI_RenderMeterMenu(meterPage);
                break;
                
                case STATE_CALIB_K_FACTOR:
                    UI_RenderKFactorInput("");
                    break;

                case STATE_CALIB_KFACTOR_SUCCESS:
                    showCorrect("KF Update Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_METER);
                    }
                    break;

                case STATE_CALIB_KFACTOR_FAILED:
                    showError("KF Update Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_K_FACTOR);
                    }
                    break;
                
                case STATE_CALIB_C_FACTOR:
                    UI_RenderCFactorInput("");
                    break;

                case STATE_CALIB_CFACTOR_SUCCESS:
                    showCorrect("CF Update Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_METER);
                    }
                    break;

                case STATE_CALIB_CFACTOR_FAILED:
                    showError("CF Update Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_C_FACTOR);
                    }
                    break;
                
                case STATE_CALIB_RESET_TOTALISER:
                    UI_RenderTotaliserInput("");
                    break;

                case STATE_CALIB_TOTALISER_SUCCESS:
                    showCorrect("TOTL Reset Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_METER);
                    }
                    break;  

                case STATE_CALIB_TOTALISER_FAILED:
                    showError("TOTL Reset Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_RESET_TOTALISER);
                    }
                    break;

                case STATE_CALIB_SD_THRESHOLD:
                   UI_RenderSDThresholdInput("");
                break;

                case STATE_CALIB_SD_THRESHOLD_SUCCESS:
                  showCorrect("SDT Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_METER);
                    }
                    break; 

                case STATE_CALIB_SD_THRESHOLD_FAILED:
                  showError("SD Reset Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_SD_THRESHOLD);
                    }
                    break;
                
                case STATE_CALIB_PULSER_CHANNEL:
                   UI_RenderPulserChannelInput("");
                break;

                case STATE_CALIB_PULSER_CHANNEL_SUCCESS:
                   showCorrect("Pulser Channel Set");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_METER);
                    }
                    break;  
                case STATE_CALIB_PULSER_CHANNEL_FAILED:
                   showError("P.Channel set Fail");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_PULSER_CHANNEL);
                    }
                    break;


                case STATE_CALIB_DEVICE_STATUS:
                    UI_RenderDeviceStatusrMenu(deviceStatusPage);
                break;
                
                case STATE_CALIB_GPS_STATUS:
                     UI_RenderGpsStatusInput("");
                break;

                case STATE_CALIB_GPS_STATUS_SUCCESS:
                    showCorrect("GPS set Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_DEVICE_STATUS);
                    }
                    break;

                case STATE_CALIB_GPS_STATUS_FAILED:
                    showError("GPS Set Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_GPS_STATUS);
                    }
                    break;
                
                case STATE_CALIB_MODEM_STATUS:
                    UI_RenderModemStatusInput("");
                break;

                case STATE_CALIB_MODEM_STATUS_SUCCESS:
                    showCorrect("Modem set Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_DEVICE_STATUS);
                    }
                    break;

                case STATE_CALIB_MODEM_STATUS_FAILED:
                    showError("Modem Set Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_MODEM_STATUS);
                    }
                    break;
                
        
                case STATE_CALIB_RFID_STATUS:
                    UI_RenderRfidStatusInput("");
                break;

                case STATE_CALIB_RFID_STATUS_SUCCESS:
                    showCorrect("RFID set Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_DEVICE_STATUS);
                    }
                    break;

                case STATE_CALIB_RFID_STATUS_FAILED:
                    showError("RFID Set Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_RFID_STATUS);
                    }
                    break;

                case STATE_CALIB_USER:
                    UI_RenderUserMenu(userPage);
                break;

                case STATE_CALIB_USER_ADD:
                    UI_RenderUserInput("");
                break;

                case STATE_CALIB_USER_ADD_SUCCESS:
                    showCorrect("User add Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_USER);
                    }
                    break;
                break;

                case STATE_CALIB_USER_ADD_FAILED:
                    showError("User add Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_CALIB_USER_ADD);
                    }
                    break;
                break;

                case STATE_CALIB_USER_REMOVE:
                    UI_RenderUserShow();
                break;

                case STATE_CALIB_USER_REMOVE_SUCCESS:
                break;

                case STATE_CALIB_USER_REMOVE_FAILED:
                break;


                
            
                case STATE_SHOW_CONF:
                    UI_RenderCalibMenu(calibPage);
                break;

                case STATE_SHOW_PANEL:
                    UI_RenderPanelMenu(panelPage);
                break;

                 case STATE_SHOW_DATE_TIME:
                    UI_RenderDateTimeShow();
                break;
                
                case STATE_SHOW_APP_NAME:
                    UI_RenderAppNameShow();
                break;
                
                case STATE_SHOW_ID_PANEL:
                    UI_RenderIdPanelShow();
                break;
                
                case STATE_SHOW_COUNTRY_CODE:
                    UI_RenderCountryCodeShow();
                break;
                
                case STATE_SHOW_COMPANY_CODE:
                    UI_RenderCompanyCodeShow();
                break;
                
                case STATE_SHOW_LOCATION:
                    UI_RenderLocationShow();
                break;

                case STATE_SHOW_STRUCK:
                    UI_RenderStruckMenu(struckPage);
                break;   
                
                case STATE_SHOW_HEADER1:
                    UI_RenderHeader1Show();
                break;
                
                case STATE_SHOW_HEADER2:
                    UI_RenderHeader2Show();
                break;
                
                case STATE_SHOW_FOOTER1:
                     UI_RenderFooter1Show();
                break;
                
                case STATE_SHOW_FOOTER2:
                     UI_RenderFooter2Show();
                break;
                
                case STATE_SHOW_SLIP_NUMBER:
                    UI_RenderSlipNumberShow();
                break;

                case STATE_SHOW_NET:
                    UI_RenderNetMenu(netPage);
                break;
                
                case STATE_SHOW_IP:
                    UI_RenderIpShow();
                break;

                case STATE_SHOW_SSID:
                    UI_RenderSsidShow();
                break;
                
                case STATE_SHOW_SSID_USER:
                    UI_RenderSsidUserShow();
                break;

                case STATE_SHOW_SSID_PASSWORD:
                    UI_RenderSsidPasswordShow();
                break;
                
                case STATE_SHOW_MQTT_PORT:
                    UI_RenderHostPortShow();
                break;
                
                case STATE_SHOW_MQTT_USER:
                    UI_RenderMqttUserShow();
                break;
                
                case STATE_SHOW_MQTT_PASSWORD:
                    UI_RenderMqttPasswordShow();
                break;
                
                case STATE_SHOW_MQTT_TOPIC:
                    UI_RenderMqttTopicShow();
                break;

                case STATE_SHOW_METER:
                    UI_RenderMeterMenu(meterPage);
                break;

                case STATE_SHOW_K_FACTOR:
                    UI_RenderKFactorShow();
                break;
              
                case STATE_SHOW_C_FACTOR:
                    UI_RenderCFactorShow();
                break;
                
                case STATE_SHOW_RESET_TOTALISER:
                    UI_RenderTotaliserShow();
                break;
                
                case STATE_SHOW_SD_THRESHOLD:
                   UI_RenderSDThresholdShow();
                break;
                
                case STATE_SHOW_PULSER_CHANNEL:
                   UI_RenderPulserChannelShow();
                break;

                case STATE_SHOW_DEVICE_STATUS:
                    UI_RenderDeviceStatusrMenu(deviceStatusPage);
                break;
                
                case STATE_SHOW_GPS_STATUS:
                    UI_RenderGpsStatusShow();
                break;
                
                case STATE_SHOW_MODEM_STATUS:
                    UI_RenderModemStatusShow();
                break;
                
                case STATE_SHOW_RFID_STATUS:
                    UI_RenderRfidStatusShow();
                break;

                case STATE_SHOW_USER:
                    UI_RenderUserShow();
                break;

                case STATE_SHIFT:
                    UI_RenderShift(0);
                break;

                case STATE_REPRINT:
                    UI_RenderReprint(0);
                break;   

                case STATE_SHIFT_OPEN_USERID: 
                    UI_RenderShiftInputUserId(UserId);
                break;

                case STATE_SHIFT_OPEN_PASSWORD: 
                    UI_RenderShiftInputPassword(UserPassword);
                break;
                
                case STATE_OPENSHIFT_SUCCESS:
                    showCorrect("Login Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_TRANSACTION);
                    }
                break;

                case STATE_CONTINUE_SHIFT_SUCCESS:
                    showCorrect("Continue Shift");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_TRANSACTION);
                    }
                break;

                case STATE_OPENSHIFT_FAILED:
                    showError("Login Failed");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_SHIFT);
                    }
                break;

                case STATE_SHIFT_CLOSE_USERID: 
                    UI_RenderShiftInputUserId(UserId);
                break;

                case STATE_SHIFT_CLOSE_PASSWORD: 
                    UI_RenderShiftInputPassword(UserPassword);
                break;

                case STATE_CLOSESHIFT_SUCCESS:
                    showCorrect("Close Shift Success");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_SHIFT);
                    }
                break;

                case STATE_CLOSESHIFT_FAILED1:
                    showError("Different User");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_SHIFT);
                    }
                break;

                case STATE_CLOSESHIFT_FAILED2:
                    showError("No Active Shift");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_SHIFT);
                    }
                break;

                case STATE_CLOSESHIFT_FAILED3:
                    showError("Bad Credential");
                    if (now  - stateEnterTime > 2000) {
                            StateMachine_SetState(STATE_SHIFT);
                    }
                break;

                case STATE_CONFIG:
                    UI_RenderConfig();
                break;

                case STATE_TRANSACTION:
                    UI_RenderTransactionMenu();
                break;

                case STATE_INPUT_UNITNO:
                    UI_RenderInputUnitNo("");
                break;

                case STATE_INPUT_ODOMETER:
                    UI_RenderInputOdometer(""); 
                break;

                case STATE_INPUT_PRESET:
                    UI_RenderInputPreset("");
                break;

                case STATE_INPUT_UNITNO_SUCCESS:
                    UI_RenderCheckInputUnitNo();
                break;

                case STATE_INPUT_UNITNO_FAILED:
                    showError("Incorrect Unit No");
                    if (now - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_INPUT_UNITNO);
                    }
                break;

                case STATE_INPUT_ODOMETER_SUCCESS:
                    UI_RenderCheckInputOdometer();
                break;

                case STATE_INPUT_ODOMETER_FAILED:
                    showError("Incorrect Odometer Value");
                break;

                case STATE_INPUT_PRESET_SUCCESS:
                    UI_RenderCheckInputPreset();
                break;

                case STATE_INPUT_PRESET_FAILED:
                    showError("Incorrect Preset Value");
                    if (now  - stateEnterTime > 2000) {      
                            StateMachine_SetState(STATE_INPUT_PRESET);
                    }
                break;

                case STATE_STARTING_TRANSACTION:
                    if (now  - stateEnterTime > 50) {      
                            StateMachine_SetState(STATE_EXECUTING_TRANSACTION);
                    }
                break;

                case STATE_EXECUTING_TRANSACTION:
                     UI_RenderPulserDisplay(0, 0); // initial render
                break;
                    
                case STATE_FINISHING_TRANSACTION:
                    UI_RenderFinish();
                break;  

                case STATE_REPRINT_CURRENT_SHIFT:
                    showCorrect("Print CurrentShift");
                    if (now  - stateEnterTime > 500) {      
                            StateMachine_SetState(STATE_REPRINT);
                    }
                break;

                case STATE_REPRINT_LAST_TRN:
                    showCorrect("Print Last TRN");
                    if (now  - stateEnterTime > 500) {      
                            StateMachine_SetState(STATE_REPRINT);
                    }
                break;
                 
                default:
                    break;
            }
            
            lastState = current;
            forceRender = false;
        }

        // ======================b
        // UPDATE PERIODIK
        // ======================
        

        // ---- HOME: update waktu ----
        if (current == STATE_HOME) {

            DateTimeSimple t = Time_GetNow();

            if (t.minute!= lastMinute) {

                char buffer[21];

                snprintf(buffer, sizeof(buffer),
                        "%02d-%02d-%04d %02d:%02d",
                        t.day, t.month, t.year,
                        t.hour, t.minute);

                LCD_SetCursor(0, 2);
                LCD_Print("                    ");
                LCD_SetCursor(2, 2);
                LCD_Print(buffer);

                lastMinute = t.minute;
            }
        }
   
        // ======================
        // EXECUTING TRANSACTION (REALTIME RENDER)
        // ======================
            if (current == STATE_EXECUTING_TRANSACTION) {

            static unsigned long lastPulserUpdate = 0;
            //pulser.startSelenoidValve();
                if (now - lastPulserUpdate > 300) {

                    double vol = pulser.getVolume();
                    double flow = pulser.getLPM();

                    UI_RenderPulserDisplay(vol, flow);

                    if (vol >= RuntimeStorage::Transaction_GetPresetVolume()) {
                        LCD_SetCursor(18, 3);
                        LCD_Print("#");
                    }

                    lastPulserUpdate = millis();
                }
            }

            vTaskDelay(pdMS_TO_TICKS(100));
        }
}



