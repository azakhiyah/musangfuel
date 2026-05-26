#ifdef ENABLE_NETWORK
#include "services/NetworkService.h"
#include <LittleFS.h>
#include <FS.h>

#ifdef ENABLE_WPA2_ENTERPRISE
#include "esp_wpa2.h" // Untuk WPA2 Enterprise
#endif

NetworkService network;

// ============================================
// CONSTRUCTOR
// ============================================

NetworkService::NetworkService()
    : _mqtt(_wifiClient)
{
    _lastWifiReconnect = 0;
    _lastMqttReconnect = 0;
}

// ============================================
// BEGIN
// ============================================

void NetworkService::begin(
    const char* ssid,
    const char* user,
    const char* password,

    const char* mqttHost,
    uint16_t mqttPort,
    const char* mqttUser,
    const char* mqttPassword,
    const char* mqttTopic,

    IPAddress udpIp,
    uint16_t udpPort
)
{
    _ssid = ssid;
    _user = user;
    _password = password;

    _mqttHost = mqttHost;
    _mqttPort = mqttPort;

    _mqttUser = mqttUser;
    _mqttPassword = mqttPassword;
    _mqttTopic = mqttTopic;

    _udpIp = udpIp;
    _udpPort = udpPort;

    WiFi.mode(WIFI_STA);

    _mqtt.setServer(_mqttHost, _mqttPort);

    // penting untuk payload besar
    _mqtt.setBufferSize(1024);

    connectWiFi();
}

// ============================================
// WIFI CONNECT
// ============================================

void NetworkService::connectWiFi()
{
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    WiFi.disconnect(true);

    vTaskDelay(pdMS_TO_TICKS(100));

    WiFi.mode(WIFI_STA);

#ifdef ENABLE_WPA2_ENTERPRISE

    // =====================================
    // WPA2 ENTERPRISE
    // =====================================

    if (strcmp(_user, "x") != 0)
    {
        esp_wifi_sta_wpa2_ent_set_identity(
            (uint8_t*)_user,
            strlen(_user)
        );

        esp_wifi_sta_wpa2_ent_set_username(
            (uint8_t*)_user,
            strlen(_user)
        );

        esp_wifi_sta_wpa2_ent_set_password(
            (uint8_t*)_password,
            strlen(_password)
        );

        esp_wifi_sta_wpa2_ent_enable();

        WiFi.begin(_ssid);
    }
    else
    {
        WiFi.begin(
            _ssid,
            _password
        );
    }

#else

    // =====================================
    // NORMAL WPA/WPA2 PERSONAL
    // =====================================

    WiFi.begin(
        _ssid,
        _password
    );

#endif

    uint32_t start = millis();

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - start < 10000
    )
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ============================================
// MQTT CONNECT
// ============================================

void NetworkService::connectMQTT()

{
    if (!isWiFiConnected()) {
        return;
    }


    if (_mqtt.connected()) {
        return;
    }

    //Serial.println("MQTT CONNECTING...");

    char clientId[32];

    snprintf(
        clientId,
        sizeof(clientId),
        "ESP32-%lu",
        millis()
    );

    bool ok;

    // IMPORTANT
    vTaskDelay(pdMS_TO_TICKS(10));

    if (strlen(_mqttUser) > 0) {

        ok = _mqtt.connect(
            clientId,
            _mqttUser,
            _mqttPassword
        );

    } else {

        ok = _mqtt.connect(clientId);
    }
     // IMPORTANT
    vTaskDelay(pdMS_TO_TICKS(10));

    if (ok) {

        //Serial.println("MQTT CONNECTED");

    } else {

        //Serial.print("MQTT FAILED rc=");
        //Serial.println(_mqtt.state());
    }
    
}

// ============================================
// UPDATE
// ============================================

void NetworkService::update()
{
    uint32_t now = millis();

    // =========================
    // WIFI
    // =========================
    if (!isWiFiConnected()) {

        if (now - _lastWifiReconnect > 2000) {

            _lastWifiReconnect = now;

            connectWiFi();
        }

        return;
    }

    // =========================
    // MQTT
    // =========================
    if (!_mqtt.connected()) {

        if (now - _lastMqttReconnect > 5000) {

            _lastMqttReconnect = now;

            connectMQTT();
        }
    }

    _mqtt.loop();
}

void NetworkService::sendPendingTransaction()
{
    if (!isMQTTConnected()) {
        return;
    }

    File root = LittleFS.open("/");

    if (!root || !root.isDirectory()) {

        // Serial.println(
        //     "FAILED OPEN ROOT DIR"
        // );

        return;
    }

    while (true)
    {
        File file = root.openNextFile();

        if (!file) {
            break;
        }

        char filename[64];

        strncpy(
            filename,
            file.name(),
            sizeof(filename)
        );

        filename[sizeof(filename) - 1] = '\0';

        bool isTransactionFile =
            strstr(filename, "trx_") &&
            strstr(filename, ".json");

        if (!isTransactionFile) {

            file.close();
            continue;
        }

        Serial.printf(
            "SENDING FILE: %s\n",
            filename
        );

        // =========================
        // READ FILE
        // =========================
        String json =
            file.readString();

        // IMPORTANT
        file.close();

        // =========================
        // MQTT PUBLISH
        // =========================
        bool ok =
            publishMQTT(
                json.c_str()
            );

        _mqtt.loop();

        vTaskDelay(pdMS_TO_TICKS(20));

        // =========================
        // DELETE FILE
        // =========================
        if (ok)
        {
            root.close();

           char fullpath[64];

            snprintf(
                fullpath,
                sizeof(fullpath),
                "/%s",
                filename
            );

            Serial.printf(
                "DELETE PATH = [%s]\n",
                fullpath
            );

            bool removed =
                LittleFS.remove(fullpath);

            // if (removed)
            // {
            //     Serial.printf(
            //         "FILE SENT & DELETED: %s\n",
            //         filename
            //     );
            // }
            // else
            // {
            //     Serial.printf(
            //         "FAILED DELETE: %s\n",
            //         filename
            //     );
            // }

            return;
        }
        else
        {
            // Serial.printf(
            //     "MQTT SEND FAILED: %s\n",
            //     filename
            // );
        }
    }

    root.close();
}
// ============================================
// MQTT SEND
// ============================================

bool NetworkService::publishMQTT(const char* payload)
{
    if (!_mqtt.connected()) {
        return false;
    }

     bool ok =
        _mqtt.publish(
            _mqttTopic,
            payload
        );

    if (ok) {

        // Serial.println(
        //     "MQTT PUBLISH OK"
        // );

    } else {

        // Serial.println(
        //     "MQTT PUBLISH FAILED"
        // );
    }

    return ok;
}

// ============================================
// UDP SEND
// ============================================

bool NetworkService::sendUDP(const char* payload)
{
    if (!isWiFiConnected()) {
        return false;
    }

    _udp.beginPacket(_udpIp, _udpPort);

    _udp.write(
        (const uint8_t*)payload,
        strlen(payload)
    );

    return _udp.endPacket();
}

// ============================================
// STATUS
// ============================================

bool NetworkService::isWiFiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool NetworkService::isMQTTConnected()
{
    return _mqtt.connected();
}

const char* NetworkService::getIPAddress()
{
    static char ip[32];

    snprintf(
        ip,
        sizeof(ip),
        "%u.%u.%u.%u",

        WiFi.localIP()[0],
        WiFi.localIP()[1],
        WiFi.localIP()[2],
        WiFi.localIP()[3]
    );

    return ip;
}

const char* NetworkService::getMACAddress()
{
    static char mac[32];

    snprintf(
        mac,
        sizeof(mac),
        "%s",
        WiFi.macAddress().c_str()
    );

    return mac;
}

#endif