#ifndef PANEL_H
#define PANEL_H

//Panel Input Menu
void UI_RenderDateTimeInput();
void UI_RenderAppNameInput(const char* input);
void UI_RenderIdPanelInput(const char* input);
void UI_RenderCountryCodeInput(const char* input);
void UI_RenderCompanyCodeInput(const char* input);
void UI_RenderLocationInput(const char* input);

bool handle_DateTimeInput(char key);
void handle_AppNameInput(const char* typed, char preview, const char* output);
void handle_IdPanelInput(const char* typed, char preview, const char* output);
void handle_CountryCodeInput(const char* typed, char preview, const char* output);
void handle_CompanyCodeInput(const char* typed, char preview, const char* output);
void handle_LocationInput(const char* typed, char preview, const char* output);


//Panel Show Menu
void UI_RenderDateTimeShow();
void UI_RenderAppNameShow();
void UI_RenderIdPanelShow();
void UI_RenderCountryCodeShow();
void UI_RenderCompanyCodeShow();
void UI_RenderLocationShow();




#endif