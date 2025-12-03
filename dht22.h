#ifndef _DHT22_H_
#define _DHT22_H_

#include <stdbool.h>
#include <stdint.h>

bool dht22_read_raw(uint8_t data[5]);
bool dht22_read_from_raw(uint8_t data[5], int *temp_x10, int *hum_x10);

#endif
