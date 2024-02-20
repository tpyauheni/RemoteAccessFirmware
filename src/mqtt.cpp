#include <mqtt.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
void (*codeCallback)(const unsigned char *, unsigned int);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to '");
  Serial.print(SSID);
  Serial.println("'");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(const char *topic, const byte *payload, unsigned int length) {
  if (String(topic) == "rac/v1/runCode") {
    codeCallback(payload, length);
  }
}

void mqtt_reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "RACv1-" + WiFi.macAddress();

    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe("rac/v1/runCode");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_setup(void (*newCodeCallback)(const unsigned char *, unsigned int)) {
  codeCallback = newCodeCallback;
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void mqtt_loop() {
  if (!client.connected()) {
    mqtt_reconnect();
  }

  client.loop();
}
