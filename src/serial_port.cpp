#include <serial_port.h>

#include <Arduino.h>
#include <base64.h>
#include <sstream>
#include <lwip/def.h>

union UInteger32 {
    unsigned int integer;
    unsigned char bytes[4];
};

void sendBytes(const void *buffer, unsigned long length) {
    // String str = base64::encode((const uint8_t *)buffer, length, false);
    // std::stringstream ss;
    UInteger32 len = { .integer = sizeof(char) * 6 + length };

    // if (len.integer > 256 * 256 * 256) {
    //     Serial.println("!!!! Was tried to send message, longer, than maximum length !!!!");
    //     return;
    // }

    len.integer = htonl(len.integer);
    // String str2 = base64::encode((const uint8_t *)len.bytes + 1, 3, false);
    // ss << str2.c_str() << "RACv1w==" << str.c_str();

    // unsigned char *bytes = new unsigned char[sizeof(len.integer) + sizeof(char) * 6 + length];
    // memcpy(bytes, len.bytes, sizeof(len.integer));
    // memcpy(bytes + sizeof(len.integer), "RACv1w", sizeof(char) * 6);
    // memcpy(bytes + sizeof(len.integer) + sizeof(char) * 6, buffer, length);
    // Serial.write(bytes, sizeof(len.integer) + sizeof(char) * 6 + length);
    // delete bytes;
    Serial.write(len.bytes, sizeof(len.integer));
    Serial.write("RACv1w", 6 * sizeof(char));
    Serial.write((const uint8_t *)buffer, length);

    // Serial.println(ss.str().c_str());
    Serial.flush();
}

void receiveBytes(std::vector<unsigned char> &buffer) {
    unsigned short lengthRead = 0;
    unsigned int length;

    delay(1000);

    while (lengthRead < sizeof(length)) {
        lengthRead += Serial.readBytes(((unsigned char *)&length) + lengthRead, sizeof(length) - lengthRead);
    }

    length = __ntohl(length);
    buffer.clear();
    buffer.reserve(length);
    unsigned char dataByte;

    for (unsigned int i = 0; i < length; i++) {
        while (Serial.readBytes(&dataByte, 1) < 1);
        buffer.push_back(dataByte);
    }
}
