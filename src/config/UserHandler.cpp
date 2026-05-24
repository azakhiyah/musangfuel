#include <Arduino.h>
#include <LittleFS.h>
#include "config/User.h"
/*
void UserHandler_LoadFromFS() {

    g_userCount = 0;

    if (!LittleFS.begin()) {
        Serial.println("LittleFS Mount Failed (User)");
        return;
    }

    File file = LittleFS.open("/users.db", "r");

    if (!file) {
        Serial.println("users.db not found");
        return;
    }

    //Serial.println("Loading users...");

    while (file.available()) {

        String line = file.readStringUntil('\n');
        line.trim();

        // skip kosong
        if (line.length() == 0) continue;

        // cari delimiter
        int p1 = line.indexOf(',');
        int p2 = line.indexOf(',', p1 + 1);
        int p3 = line.indexOf(',', p2 + 1);

        if (p1 == -1 || p2 == -1 || p3 == -1) {
            Serial.println("Invalid line, skip");
            continue;
        }

        if (g_userCount >= MAX_USERS) {
            Serial.println("User overflow, stop loading");
            break;
        }
        String userid = line.substring(0, p1); 
        String username = line.substring(p1 + 1, p2);
        String password = line.substring(p2 + 1, p3);
        String levelStr = line.substring(p3 + 1);

        userid.trim();
        username.trim();
        password.trim();
        levelStr.trim();

        // copy ke struct (AMAN)
        strncpy(g_users[g_userCount].userid, userid.c_str(), 15);
        g_users[g_userCount].userid[15] = '\0';

        strncpy(g_users[g_userCount].username, username.c_str(), 15);
        g_users[g_userCount].username[15] = '\0';

        strncpy(g_users[g_userCount].password, password.c_str(), 15);
        g_users[g_userCount].password[15] = '\0';

        g_users[g_userCount].level = (uint8_t) levelStr.toInt();    

        // debug
        // Serial.printf("User[%d]: %s / %s / %d\n",
        //              g_userCount,
        //              g_users[g_userCount].userid,
        //              g_users[g_userCount].username,
        //              g_users[g_userCount].password,
        //              g_users[g_userCount].level);

        g_userCount++;
    }

    file.close();

    //Serial.printf("Total users loaded: %d\n", g_userCount);
}
    */

void UserHandler_LoadFromFS()
{
    g_userCount = 0;

    if (!LittleFS.begin()) {
        Serial.println("LittleFS Mount Failed (User)");
        return;
    }

    File file = LittleFS.open("/users.db", "r");

    if (!file) {
        Serial.println("users.db not found");
        return;
    }

    char line[128];

    while (file.available()) {

        size_t len = file.readBytesUntil('\n',
                                         line,
                                         sizeof(line) - 1);

        line[len] = '\0';

        // ===== REMOVE CR/LF =====
        while (len > 0 &&
              (line[len - 1] == '\r' ||
               line[len - 1] == '\n'))
        {
            line[--len] = '\0';
        }

        // ===== SKIP EMPTY =====
        if (len == 0) {
            continue;
        }

        // ===== FIND DELIMITER =====
        char* p1 = strchr(line, ',');

        if (!p1) {
            Serial.println("Invalid line");
            continue;
        }

        char* p2 = strchr(p1 + 1, ',');

        if (!p2) {
            Serial.println("Invalid line");
            continue;
        }

        char* p3 = strchr(p2 + 1, ',');

        if (!p3) {
            Serial.println("Invalid line");
            continue;
        }

        if (g_userCount >= MAX_USERS) {
            Serial.println("User overflow");
            break;
        }

        // split string
        *p1 = '\0';
        *p2 = '\0';
        *p3 = '\0';

        char* userid   = line;
        char* username = p1 + 1;
        char* password = p2 + 1;
        char* levelStr = p3 + 1;

        // ===== COPY =====
        strncpy(g_users[g_userCount].userid,
                userid,
                sizeof(g_users[g_userCount].userid) - 1);

        g_users[g_userCount]
            .userid[sizeof(g_users[g_userCount].userid) - 1] = '\0';

        strncpy(g_users[g_userCount].username,
                username,
                sizeof(g_users[g_userCount].username) - 1);

        g_users[g_userCount]
            .username[sizeof(g_users[g_userCount].username) - 1] = '\0';

        strncpy(g_users[g_userCount].password,
                password,
                sizeof(g_users[g_userCount].password) - 1);

        g_users[g_userCount]
            .password[sizeof(g_users[g_userCount].password) - 1] = '\0';

        // ===== LEVEL =====
        g_users[g_userCount].level = (uint8_t)atoi(levelStr);

        g_userCount++;
    }

    file.close();
}


User* User_Find(const char* userid, const char* password) {

    for (uint16_t i = 0; i < g_userCount; i++) {

        if (strcmp(g_users[i].userid, userid) == 0 &&
            strcmp(g_users[i].password, password) == 0) {

            return &g_users[i];
        }
    }

    return nullptr;
}

User* User_FindById(const char* userid) {

    for (uint16_t i = 0; i < g_userCount; i++) {

        if (strcmp(g_users[i].userid, userid) == 0) {
            return &g_users[i];
        }
    }

    return nullptr;
}