#ifndef NETWORK_SERVICE_H
#define NETWORK_SERVICE_H

// ============================================
// File : services/NetworkService.h
// ============================================

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

class NetworkService
{
private:

    // =========================
    // WIFI
    // =========================
    const char* _ssid;
    const char* _user;
    const char* _password;

    // =========================
    // MQTT
    // =========================
    WiFiClient _wifiClient;
    PubSubClient _mqtt;

    const char* _mqttHost;
    uint16_t    _mqttPort;

    const char* _mqttUser;
    const char* _mqttPassword;
    const char* _mqttTopic;

    // =========================
    // UDP
    // =========================
    WiFiUDP _udp;

    IPAddress _udpIp;
    uint16_t  _udpPort;

    // =========================
    // INTERNAL
    // =========================
    uint32_t _lastWifiReconnect;
    uint32_t _lastMqttReconnect;

    void connectWiFi();
    void connectMQTT();

public:

    NetworkService();

    void begin(
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
    );

    void update();

    // =========================
    // SEND
    // =========================
    bool publishMQTT(const char* payload);

    bool sendUDP(const char* payload);

    // =========================
    // STATUS
    // =========================
    bool isWiFiConnected();

    bool isMQTTConnected();

    const char* getIPAddress();

    const char* getMACAddress();

    void sendPendingTransaction();
};

// ============================================
// GLOBAL
// ============================================
extern NetworkService network;

#endif