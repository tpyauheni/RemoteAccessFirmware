#include <board.h>
#include <serial_port.h>
#include "keypad.hpp"
#include "mqtt.h"
#include <string>
#include <belang/lexer.hpp>
#include "belang/parser.hpp"

/*
#define A D4
#define B D5
#define C D7
#define D D10
#define E D8
#define F D3
#define G D2
#define DP D6

/\*
 AAAA 
F    B
F    B
 GGGG 
E    C
E    C
 DDDD  DP

0 = ABCDEF
1 = BC
2 = ABGED
3 = ABGCD
4 = BCFG
5 = ACDFG
6 = ACDEFG
7 = ABC
8 = ABCDEFG
9 = ABCDFG
a = ABCEFG
b = CDEFG
c = DEG
d = BCDEG
e = ADEFG
f = AEFG
*\/

unsigned long delayInterval = 100;

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(D11, INPUT);
}

#define Aon digitalWrite(A, HIGH)
#define Bon digitalWrite(B, HIGH)
#define Con digitalWrite(C, HIGH)
#define Don digitalWrite(D, HIGH)
#define Eon digitalWrite(E, HIGH)
#define Fon digitalWrite(F, HIGH)
#define Gon digitalWrite(G, HIGH)

#define Aoff digitalWrite(A, LOW)
#define Boff digitalWrite(B, LOW)
#define Coff digitalWrite(C, LOW)
#define Doff digitalWrite(D, LOW)
#define Eoff digitalWrite(E, LOW)
#define Foff digitalWrite(F, LOW)
#define Goff digitalWrite(G, LOW)

#define RESET Aoff; Boff; Coff; Doff; Eoff; Foff; Goff

unsigned char num = 0;

void showNumber(unsigned char number) {
  RESET;

  switch (number) {
    case 0: {
      Aon; Bon; Con; Don; Eon; Fon;
      break;
    }
    case 1: {
      Bon; Con;
      break;
    }
    case 2: {
      Aon; Bon; Gon; Eon; Don;
      break;
    }
    case 3: {
      Aon; Bon; Gon; Con; Don;
      break;
    }
    case 4: {
      Bon; Con; Fon; Gon;
      break;
    }
    case 5: {
      Aon; Con; Don; Fon; Gon;
      break;
    }
    case 6: {
      Aon; Con; Don; Eon; Fon; Gon;
      break;
    }
    case 7: {
      Aon; Bon; Con;
      break;
    }
    case 8: {
      Aon; Bon; Con; Don; Eon; Fon; Gon;
      break;
    }
    case 9: {
      Aon; Bon; Con; Don; Fon; Gon;
      break;
    }
    case 0x0a: {
      Aon; Bon; Con; Eon; Fon; Gon;
      break;
    }
    case 0x0b: {
      Con; Don; Eon; Fon; Gon;
      break;
    }
    case 0x0c: {
      Don; Eon; Gon;
      break;
    }
    case 0x0d: {
      Bon; Con; Don; Eon; Gon;
      break;
    }
    case 0x0e: {
      Aon; Don; Eon; Fon; Gon;
      break;
    }
    case 0x0f: {
      Aon; Eon; Fon; Gon;
      break;
    }
    default: {
      Gon;
      break;
    }
  }
}

bool prev_pressed = false;

void loop() {
  bool is_pressed = digitalRead(D11) == HIGH;

  if (is_pressed) {
    if (!prev_pressed) {
      prev_pressed = true;
      num = (num + 1) % 16;
      showNumber(num);
    }
  } else {
    if (prev_pressed) {
      prev_pressed = false;
    }
  }
}
*\/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TechnoPark";
const char* password = "test@TEST";

const char* mqtt_server = "m9.wqtt.ru";
const int mqtt_port = 14780;
const char* mqtt_user = "u_7O8QAW";
const char* mqtt_password = "Et3YqjXa";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to '");
  Serial.print(ssid);
  Serial.println("'");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

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

void callback(char *topic, byte *payload, unsigned int length) {
  String data_pay;

  for (int i = 0; i < length; i++) {
    data_pay += String((char)payload[i]);
  }

  Serial.println(data_pay);
    
  if (String(topic) == "belcomp/outputs/led") {
    if (data_pay == "1") {
      digitalWrite(0, HIGH);
    } else if (data_pay == "0") {
      digitalWrite(0, LOW);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "Belcomp-" + WiFi.macAddress();

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("belcomp/outputs/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(0, OUTPUT);

  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
*/

bool isConnected = false;

void compile_code(const unsigned char *bytes, unsigned int length) {
  std::string codeText;
  codeText.reserve(length);

  for (unsigned int i = 0; i < length; i++) {
    codeText.push_back(bytes[i]);
  }

  belang::Lexer lexer(codeText);
  std::vector<belang::Token> tokens;
  std::optional<belang::Token> last_token = lexer.next();
  Serial.println("Lexical analyzer output:");

  while (last_token.has_value()) {
      last_token = lexer.findKeyword(*last_token);
      Serial.println(String(last_token->to_string().c_str()));

      if (last_token->type == belang::TokenType::LEXER_ERROR) {
          return;
      }

      tokens.push_back(*last_token);
      last_token = lexer.next();
  }

  Serial.println("\nParser output:");
  belang::Parser parser = belang::Parser(tokens);
  parser.parse()->print();
}

void setup() {
  Serial.begin(115200);

  mqtt_setup(compile_code);
  keypad_setup();
}

std::vector<unsigned char> message;

void loop() {
  if (isConnected) {
    if (Serial.available() >= (int)sizeof(unsigned int)) {
      receiveBytes(message);

      if (message[0] == 0) {
        unsigned int requestId = *((unsigned int *)(message.data() + 1));
        // Result to command with id <requestId>
        // Send via MQTT
      }
    }

    mqtt_loop();
    delay(1000);
  } else {
    char broadcast_message[] = {};
    keypad_loop();
    sendBytes(broadcast_message, sizeof(broadcast_message));
    delay(1000);

    if (Serial.available() >= (int)sizeof(unsigned int)) {
      receiveBytes(message);

      if (message.size() == 6) {
        if (memcmp(message.data(), "RACv1x", 6) == 0) {
          isConnected = true;
        } // else skip, cuz some other thing
      }
    }
  }
}
