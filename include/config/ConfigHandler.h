#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

namespace ConfigHandler {

    void Init();

    void ConfigHandler_LoadFromFS();
    //void ConfigHandler_SaveToFS();

    bool ConfigHandler_UpdateValue(const char* targetKey,const char* newValue);

    
}