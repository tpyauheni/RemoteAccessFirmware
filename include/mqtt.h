#pragma once
#ifndef BELCOMP_MQTT_H
#define BELCOMP_MQTT_H 1

static const char* SSID = "TechnoPark";
static const char* PASSWORD = "test@TEST";

static const char* MQTT_SERVER = "m9.wqtt.ru";
static const int MQTT_PORT = 14780;
static const char* MQTT_USER = "u_7O8QAW";
static const char* MQTT_PASSWORD = "Et3YqjXa";

void mqtt_reconnect();
void mqtt_setup(void (*)(const unsigned char *, unsigned int));
void mqtt_loop();

#endif // BELCOMP_MQTT_H
