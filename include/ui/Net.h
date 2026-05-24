#ifndef NET_H
#define NET_H

//Net Input Menu
/*
  "1.Ip",
  "2.Ssid",
  "3.Ssid Usernamme",
  "4.Ssid Password",
  "5.Server Port",
  "6.Mqqt User",
  "7.Mqqt Password",
  "8.Mqqt Topic"
*/

void UI_RenderIpInput(const char* input);
void UI_RenderHostPortInput(const char* input);
void UI_RenderSsidInput(const char* input);
void UI_RenderSsidUserInput(const char* input);
void UI_RenderSsidPasswordInput(const char* input);
void UI_RenderMqttUserInput(const char* input);
void UI_RenderMqttPasswordInput(const char* input);
void UI_RenderMqttTopicInput(const char* input);


void handle_IpInput(const char* typed, char preview, const char* output);
void handle_HostPortInput(const char* typed, char preview, const char* output);
void handle_SsidInput(const char* typed, char preview, const char* output);
void handle_SsidUserInput(const char* typed, char preview, const char* output);
void handle_SsidPasswordInput(const char* typed, char preview, const char* output);
void handle_MqttUserInput(const char* typed, char preview, const char* output);
void handle_MqttPasswordInput(const char* typed, char preview, const char* output);
void handle_MqttTopicInput(const char* typed, char preview, const char* output);

void UI_RenderIpShow();
void UI_RenderHostPortShow();
void UI_RenderSsidShow();
void UI_RenderSsidUserShow();
void UI_RenderSsidPasswordShow();
void UI_RenderMqttUserShow();
void UI_RenderMqttPasswordShow();
void UI_RenderMqttTopicShow();









#endif