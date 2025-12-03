#include "dht22.h"
#include "em_common.h"
#include "app_assert.h"
#include "app.h"
#include "app_log.h"
#include "sl_sleeptimer.h"
#include "em_gpio.h"

// ===== LCD LIB từ file bạn cung cấp =====
#include "glib.h"
#include "glib_color.h"
#include "glib_string.h"
#include "dmd.h"
#include "lcd_display.h"

static GLIB_Context_t glibContext;

// ============ HÀM KHỞI TẠO LCD =============
static void lcd_init(void)
{
    uint32_t status;

    status = sl_board_enable_display();
    EFM_ASSERT(status == SL_STATUS_OK);

    status = DMD_init(0);
    EFM_ASSERT(status == DMD_OK);

    status = GLIB_contextInit(&glibContext);
    EFM_ASSERT(status == GLIB_OK);

    glibContext.backgroundColor = White;
    glibContext.foregroundColor = Black;

    GLIB_clear(&glibContext);
    GLIB_setFont(&glibContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);

    GLIB_drawStringOnLine(&glibContext,
                          "DHT22 TEST",
                          0,
                          GLIB_ALIGN_CENTER,
                          0,
                          0,
                          true);

    DMD_updateDisplay();
}

// ============ In dữ liệu lên LCD =============
static void lcd_show(int temp_x10, int hum_x10)
{
    char line1[20];
    char line2[20];

    snprintf(line1, sizeof(line1), "Temp: %d.%d C", temp_x10/10, temp_x10%10);
    snprintf(line2, sizeof(line2), "Hum : %d.%d %%", hum_x10/10, hum_x10%10);

    GLIB_clear(&glibContext);

    GLIB_drawStringOnLine(&glibContext, "DHT22 TEST", 0,
                          GLIB_ALIGN_CENTER, 0, 0, true);

    GLIB_drawStringOnLine(&glibContext, line1, 2,
                          GLIB_ALIGN_CENTER, 5, 5, true);

    GLIB_drawStringOnLine(&glibContext, line2, 4,
                          GLIB_ALIGN_CENTER, 5, 5, true);

    DMD_updateDisplay();
}

// ================== DHT22 CONFIG ======================
static sl_sleeptimer_timer_handle_t sensor_timer;

static int temp_x10 = 0;
static int hum_x10  = 0;
static uint8_t raw[5];

#define DHT_PORT   gpioPortB
#define DHT_PIN    0

volatile bool dht_need_read = false;

// ================= TIMER CALLBACK ======================
static void sensor_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
    (void)handle;
    (void)data;

    dht_need_read = true;
}

// ================= INIT ===============================
SL_WEAK void app_init(void)
{
    sl_status_t sc;

    sl_sleeptimer_delay_millisecond(200);
    app_log("\r\n=== DHT22 + LCD (GLIB) TEST ===\r\n");

    lcd_init();

    GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModeInputPull, 1);

    sc = sl_sleeptimer_start_periodic_timer_ms(
            &sensor_timer,
            2000,
            sensor_timer_cb,
            NULL,
            0,
            0);

    app_assert_status(sc);

    app_log("Start reading every 2 seconds...\r\n");
}

// ================= MAIN LOOP ==========================
SL_WEAK void app_process_action(void)
{
    if (dht_need_read)
    {
        dht_need_read = false;

        GPIO_PinModeSet(DHT_PORT, DHT_PIN, gpioModeInputPull, 1);

        bool ok = dht22_read_raw(raw);

        app_log("RAW: %02X %02X %02X %02X %02X | ",
                raw[0], raw[1], raw[2], raw[3], raw[4]);

        if (ok)
        {
            dht22_read_from_raw(raw, &temp_x10, &hum_x10);

            app_log("T=%d.%d C | H=%d.%d %%\r\n",
                    temp_x10/10, temp_x10%10,
                    hum_x10/10, hum_x10%10);

            lcd_show(temp_x10, hum_x10);
        }
        else
        {
            app_log("Checksum FAIL\r\n");
            GLIB_drawStringOnLine(&glibContext, "Sensor ERROR",
                                  2, GLIB_ALIGN_CENTER, 0, 0, true);
            DMD_updateDisplay();
        }
    }
}
