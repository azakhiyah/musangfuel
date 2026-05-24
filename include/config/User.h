#pragma once
#include <stdint.h>

#define MAX_USERS 100


struct User {
    char userid[16];
    char username[16];
    char password[16];
    uint8_t level;
};

// storage global
extern User g_users[MAX_USERS];
extern uint16_t g_userCount;