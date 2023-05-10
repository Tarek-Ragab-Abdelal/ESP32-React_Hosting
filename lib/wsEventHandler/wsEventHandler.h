#ifndef _WS_EVENT_HANDLER_H_
#define _WS_EVENT_HANDLER_H_
#define LED_PIN 2

void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#endif