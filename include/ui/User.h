#ifndef USER_H
#define USER_H
#include <cstdint>

void UI_RenderUserInput(const char* input);
void UI_RenderUserRemove(const char* input);

void handle_UserInput(const char* typed, char preview, const char* output);
void UI_UserNextPage();
void UI_ResetUserPage();
void UI_RenderUserShow();

void UI_UserNextPage();
void UI_UserPrevSelection();
void UI_UserNextSelection();
void UI_UserDeleteSelected();

void UI_RenderUserShow();
void UI_ResetUserPage();


static uint16_t UI_UserCount();

#endif