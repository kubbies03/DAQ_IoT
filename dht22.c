#include "em_gpio.h"
#include "sl_udelay.h"
#include "dht22.h"

#define DHT_PORT   gpioPortB
#define DHT_PIN    0

static void dht_output(void)
{
  GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModePushPull, 1);
}

static void dht_input(void)
{
  GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModeInputPull, 1);
}

static uint8_t dht22_read_bit(void)
{
  while (GPIO_PinInGet(DHT_PORT, DHT_PIN) == 0);
  sl_udelay_wait(30);
  return GPIO_PinInGet(DHT_PORT, DHT_PIN);
}

bool dht22_read_raw(uint8_t data[5])
{
  uint8_t byte, bit;

  dht_output();
  GPIO_PinOutClear(DHT_PORT, DHT_PIN);
  sl_udelay_wait(1800);

  GPIO_PinOutSet(DHT_PORT, DHT_PIN);
  sl_udelay_wait(40);

  dht_input();

  if (GPIO_PinInGet(DHT_PORT, DHT_PIN))
    return false;

  while (!GPIO_PinInGet(DHT_PORT, DHT_PIN));
  while ( GPIO_PinInGet(DHT_PORT, DHT_PIN));

  for (byte = 0; byte < 5; byte++) {
    data[byte] = 0;
    for (bit = 0; bit < 8; bit++) {
      data[byte] <<= 1;
      if (dht22_read_bit())
        data[byte] |= 1;

      while (GPIO_PinInGet(DHT_PORT, DHT_PIN));
    }
  }

  uint8_t sum = data[0] + data[1] + data[2] + data[3];
  if (sum != data[4])
    return false;

  return true;
}

// ************ CONVERT DHT22 KHÔNG FLOAT (X10) ************
bool dht22_read_from_raw(uint8_t data[5], int *temp_x10, int *hum_x10)
{
  uint16_t rh = (data[0] << 8) | data[1];
  *hum_x10 = rh;   // ví dụ 595 = 59.5%

  uint16_t tc = (data[2] << 8) | data[3];

  if (tc & 0x8000) {
    tc &= 0x7FFF;
    *temp_x10 = -(tc);
  } else {
    *temp_x10 = tc;
  }

  return true;
}
