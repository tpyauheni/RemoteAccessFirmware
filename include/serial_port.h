#pragma once
#ifndef BELCOMP_SERIAL_PORT_H
#define BELCOMP_SERIAL_PORT_H 1

#include <vector>

void sendBytes(const void *buffer, unsigned long length);
void receiveBytes(std::vector<unsigned char> &buffer);

#endif // BELCOMP_SERIAL_PORT_H
