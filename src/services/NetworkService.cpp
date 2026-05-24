// ============================================
// File : services/NetworkService.cpp
// ============================================

#include "services/NetworkService.h"

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

        //Serial.printf("SSID='%s'\n", _ssid);
        // for (int i = 0; i < 21; i++) {
        //        Serial.printf("%02X ", (uint8_t)_ssid[i]);
        //  }
        //  Serial.println();
        // Serial.printf("LEN=%d\n", strlen(_ssid));
        // Serial.printf("PASSWORD='%s'\n", _password);
        // Serial.printf("LEN=%d\n", strlen(_password));b
    // Serial.printf("SSID: %s\n",_ssid);
    // Serial.printf("PASSWORD:%s\n",_password);

    WiFi.begin(_ssid, _password);
    //WiFi.begin("ipul","asdfghjkl");

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

    if (strlen(_mqttUser) > 0) {

        ok = _mqtt.connect(
            clientId,
            _mqttUser,
            _mqttPassword
        );

    } else {

        ok = _mqtt.connect(clientId);
    }

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

        if (now - _lastWifiReconnect > 10000) {

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

// ============================================
// MQTT SEND
// ============================================

bool NetworkService::publishMQTT(const char* payload)
{
    if (!_mqtt.connected()) {
        return false;
    }

    return _mqtt.publish(
        _mqttTopic,
        payload
    );
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