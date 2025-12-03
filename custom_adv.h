#ifndef _CUSTOM_ADV_H_
#define _CUSTOM_ADV_H_

#include "sl_bt_api.h"

#define NAME_MAX_LENGTH 14

typedef struct
{
  uint8_t flags_len;
  uint8_t flags_type;
  uint8_t flags_val;

  uint8_t name_len;
  uint8_t name_type;
  char    name[NAME_MAX_LENGTH];

  uint8_t svc_len;
  uint8_t svc_type;
  uint8_t svc_uuid_lo;
  uint8_t svc_uuid_hi;

  uint16_t temp_x10;
  uint16_t hum_x10;

  uint8_t total_size;

} AdvPacket_t;

void adv_build(AdvPacket_t *p, const char *name,
               uint16_t temp_x10, uint16_t hum_x10);

void adv_start(uint8_t adv_handle, AdvPacket_t *p);
void adv_update(uint8_t adv_handle, AdvPacket_t *p,
                uint16_t temp_x10, uint16_t hum_x10);

#endif
