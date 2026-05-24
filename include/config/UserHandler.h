#pragma once
#include "config/User.h"

void UserHandler_LoadFromFS();

User* User_Find(const char* username, const char* password);
User* User_FindById(const char* userid);
