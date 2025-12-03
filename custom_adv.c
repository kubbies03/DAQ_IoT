#include "custom_adv.h"
#include <string.h>
#include <stdio.h>

void adv_build(AdvPacket_t *p, const char *name,
               uint16_t temp_x10, uint16_t hum_x10)
{
  // FLAGS (0x01 → general discoverable)
  p->flags_len  = 0x02;
  p->flags_type = 0x01;
  p->flags_val  = 0x06;

  // NAME
  int n = strlen(name);
  if (n > NAME_MAX_LENGTH) n = NAME_MAX_LENGTH;
  memcpy(p->name, name, n);
  p->name_len  = n + 1;
  p->name_type = 0x09; // Complete Local Name

  // SERVICE DATA - UUID 0x181A (Environmental Sensing)
  p->svc_len      = 1 + 2 + 4;  // type + UUID(2B) + payload(4B)
  p->svc_type     = 0x16;       // Service Data
  p->svc_uuid_lo  = 0x1A;
  p->svc_uuid_hi  = 0x18;

  p->temp_x10 = temp_x10;
  p->hum_x10  = hum_x10;

  p->total_size = 3 + (1+p->name_len) + (1+p->svc_len);
}

void adv_pack(uint8_t *buf, AdvPacket_t *p, uint16_t temp_x10, uint16_t hum_x10)
{
  uint8_t idx = 0;

  buf[idx++] = p->flags_len;
  buf[idx++] = p->flags_type;
  buf[idx++] = p->flags_val;

  buf[idx++] = p->name_len;
  buf[idx++] = p->name_type;
  memcpy(&buf[idx], p->name, p->name_len - 1);
  idx += p->name_len - 1;

  buf[idx++] = p->svc_len;
  buf[idx++] = p->svc_type;
  buf[idx++] = p->svc_uuid_lo;
  buf[idx++] = p->svc_uuid_hi;

  buf[idx++] = temp_x10 & 0xFF;
  buf[idx++] = temp_x10 >> 8;
  buf[idx++] = hum_x10 & 0xFF;
  buf[idx++] = hum_x10 >> 8;

  p->total_size = idx;
}

void adv_start(uint8_t adv_handle, AdvPacket_t *p)
{
  uint8_t buf[31];
  adv_pack(buf, p, p->temp_x10, p->hum_x10);

  sl_bt_legacy_advertiser_set_data(adv_handle, 0, p->total_size, buf);
  sl_bt_legacy_advertiser_start(adv_handle,
      sl_bt_advertiser_connectable_scannable);
}

void adv_update(uint8_t adv_handle, AdvPacket_t *p,
                uint16_t temp_x10, uint16_t hum_x10)
{
  uint8_t buf[31];
  adv_pack(buf, p, temp_x10, hum_x10);

  sl_bt_legacy_advertiser_set_data(adv_handle, 0, p->total_size, buf);
}
