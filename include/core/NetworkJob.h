#pragma once
#include <stdint.h>

typedef enum {
    TELEMTRY,
    MQTT
} NetworkType;

typedef struct {
    uint32_t timestamp;
    uint8_t panelId;
    bool fastValve;
    bool slowValve;
    double volume;
    double flowRate;
} telemetryData;

typedef struct {
    char filename[32];
} mqttData;


typedef struct {
    NetworkType type;
    telemetryData telemetryPayload;
    mqttData mqttPayload;
} NetworkJob;