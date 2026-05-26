#include "services/NetworkService.h"
#include <LittleFS.h>
#include <FS.h>
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

    Serial.println("WIFI CONNECTING...");

    WiFi.begin(_ssid, _password);

    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 10000)
    {
        delay(500);
        //Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {

        Serial.println("WIFI CONNECTED");
        Serial.println(WiFi.localIP());

    } else {

        //Serial.println("WIFI FAILED");
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

    Serial.println("MQTT CONNECTING...");

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

        Serial.println("MQTT CONNECTED");

    } else {

        Serial.print("MQTT FAILED rc=");
        Serial.println(_mqtt.state());
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

        Serial.println(
            "FAILED OPEN ROOT DIR"
        );

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

            if (removed)
            {
                Serial.printf(
                    "FILE SENT & DELETED: %s\n",
                    filename
                );
            }
            else
            {
                Serial.printf(
                    "FAILED DELETE: %s\n",
                    filename
                );
            }

            return;
        }
        else
        {
            Serial.printf(
                "MQTT SEND FAILED: %s\n",
                filename
            );
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

        Serial.println(
            "MQTT PUBLISH OK"
        );

    } else {

        Serial.println(
            "MQTT PUBLISH FAILED"
        );
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