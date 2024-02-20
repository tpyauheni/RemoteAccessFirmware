/*
Code from: https://roboindia.com/tutorials/arduino-4x4-matrix-keypad/ (modified)
*/

#include <keypad.hpp>

unsigned char h = 0, v = 0;
const int period = 50;
int kdelay = 0;
const unsigned char rows = 4;
const unsigned char columns = 4;
const unsigned char output[rows] = { D2, D3, D4, D10 };
const unsigned char input[columns] = { D8, D11, D12, D13 };

void keypad_setup() {
  for (unsigned char i = 0; i < rows; i++) {
    pinMode(output[i], OUTPUT);
  }

  for (unsigned char s = 0; s < columns; s++) {
    pinMode(input[s], INPUT_PULLUP);
  }
}

void keypad_loop() {
  if (millis() - kdelay > period) {
    kdelay = millis();

    switch (_keypad()) {
      case 0:
        Serial.println(1);
        break;

      case 1:
        Serial.println(2);
        break;

      case 2:
        Serial.println(3);
        break;

      case 3:
        Serial.println("A");
        break;

      case 4:
        Serial.println(4);
        break;

      case 5:
        Serial.println(5);
        break;

      case 6:
        Serial.println(6);
        break;

      case 7:
        Serial.println("B");
        break;

      case 8:
        Serial.println(7);
        break;

      case 9:
        Serial.println(8);
        break;

      case 10:
        Serial.println(9);
        break;

      case 11:
        Serial.println("C");
        break;

      case 12:
        Serial.println("*");
        break;

      case 13:
        Serial.println(0);
        break;

      case 14:
        Serial.println("#");
        break;

      case 15:
        Serial.println("D");
        break;

      default:
        break;
    }
  }
}

byte _keypad() {
  static bool no_press_flag = false;

  for (byte x = 0; x < columns; x++) {
    if (digitalRead(input[x]) == LOW) {
      break;
    }

    if (x == columns - 1) {
      no_press_flag = true;
      h = 0;
      v = 0;
    }
  }

  if (no_press_flag) {
    for (byte r = 0; r < rows; r++) {
      digitalWrite(output[r], LOW);
    }

    for (h = 0; h < columns; h++) {
      if (digitalRead(input[h]) == HIGH) {
        continue;
      }

      for (v = 0; v < rows; v++) {
        digitalWrite(output[v], HIGH);

        if (digitalRead(input[h]) == HIGH) {
          no_press_flag = false;

          for (byte w = 0; w < rows; w++) {
            digitalWrite(output[w],LOW);
          }

          return v * 4 + h;
        }
      }
    }
  }

  return -1;
}
