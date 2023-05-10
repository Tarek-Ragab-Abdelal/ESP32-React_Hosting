#include <Arduino.h>
#include <WiFi.h>
#include <LITTLEFS.h>
#include <ESPAsyncWebServer.h>
#include "wsEventHandler.h"

// Define Mode of operation either to be Station mode or Acess point mode
// #define AP_MODE
#define STA_MODE

#define SSID "ESP32 SoftAP" // This is the SSID that ESP32 will broadcast
#define PASSWORD "12345678" // password should be atleast 8 characters to make it work

const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

void setup()
{
  pinMode(2, OUTPUT);
  Serial.begin(115200);
#ifdef AP_MODE
  WiFi.disconnect();   // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_AP);
#ifndef PASSWORD
  WiFi.softAP(SSID);
#else
  WiFi.softAP(SSID, PASSWORD);
#endif
  WiFi.softAPConfig(apIP, apIP, gateway);
  Serial.println("");
  Serial.println("WiFi AP is now running");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
#else
  WiFi.mode(WIFI_STA);
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
#ifdef VERBOSE
              Serial.println("Serving file:  /index.html");
#endif
              AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, "/index.html", "text/html");
              response->addHeader("Content-Encoding", "gzip");
              request->send(response); });

  File root = LITTLEFS.open("/");
  while (File file = root.openNextFile())
  {
    String filename = file.name();
    server.on(filename.c_str(), HTTP_GET, [filename](AsyncWebServerRequest *request)
              {
#ifdef VERBOSE
                Serial.println("Serving file: " + filename);
#endif
                //
                String contentType = filename.substring(filename.length() - 3);
                if (contentType == "tml" || contentType == "htm")
                  contentType = "text/html";
                else if (contentType == "css")
                  contentType = "text/css";
                else if (contentType == ".js")
                  contentType = "text/javascript";
                else if (contentType == "son")
                  contentType = "application/json";
                else if (contentType == "jpg" || contentType == "peg")
                  contentType = "image/jpeg";
                else if (contentType == "png")
                  contentType = "image/png";
                else if (contentType == "svg")
                  contentType = "image/svg+xml";
                else if (contentType == "ttf")
                  contentType = "application/x-font-truetype";
                else if (contentType == "otf")
                  contentType = "application/x-font-opentype";
                else
                  contentType = "text/plain";
                AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, filename, contentType);
                response->addHeader("Content-Encoding", "gzip");
                request->send(response); });
  }
  // Captive portal to keep the client
  server.on("*", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->redirect("http://" + apIP.toString()); });
  server.begin();

  Serial.println("Server Started");
}

void loop()
{
  vTaskDelay(1);
}