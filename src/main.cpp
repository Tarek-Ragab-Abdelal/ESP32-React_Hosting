#include <Arduino.h>
#include <WiFi.h>
#include <LITTLEFS.h>
#include <ESPAsyncWebServer.h>
#include "wsEventHandler.h"

// Define Mode of operation either to be Station mode or Acess point mode
// #define AP_MODE
#define STA_MODE

#define SSID "Orange-Ragab Abdelal" // This is the SSID that ESP32 will broadcast
#define PASSWORD "Suzan712@66.com"  // password should be atleast 8 characters to make it work

#ifdef AP_MODE
const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(192, 168, 1, 1);
#else
IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional
#endif
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

void setup()
{
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  WiFi.disconnect();   // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); // added to start with the wifi off, avoid crashing
#ifdef AP_MODE
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);
  WiFi.softAPConfig(apIP, apIP, gateway);
  Serial.println("");
  Serial.println("WiFi AP is now running");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
#else
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("\nConnected\nIP:\t");
  Serial.println(WiFi.localIP());
#endif

  if (!LITTLEFS.begin())
  {
    // Serial.println("An Error has occurred while mounting LITTLEFS");
    return;
  }

  // bind websocket to async web server
  websocket.onEvent(wsEventHandler);
  server.addHandler(&websocket);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Serving file:  /index.html");
              AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, "/index.html", "text/html");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Serving file:  /main.js");
              AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, "/main.js", "text/javascript");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Serving file:  /main.css");
              AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, "/main.css", "text/css");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Serving file:  /favicon.icon");
              AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, "/favicon.ico", "text/javascript");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

#ifdef AP_MODE
  server.on("*", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->redirect("http://" + apIP.toString()); });
#else
  server.on("*", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->redirect("http://" + WiFi.localIP().toString()); });
#endif
  server.begin();

  Serial.println("Server Started");
}

void loop()
{
  vTaskDelay(1);
}