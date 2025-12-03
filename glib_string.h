#ifndef __GLIB_STRING_H_
#define __GLIB_STRING_H_

#include "glib.h"
#include "glib_color.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

EMSTATUS GLIB_drawChar(GLIB_Context_t *pContext,
                       char myChar,
                       int32_t x,
                       int32_t y,
                       bool opaque);

EMSTATUS GLIB_drawString(GLIB_Context_t *pContext,
                         const char *pString,
                         uint32_t sLength,
                         int32_t x0,
                         int32_t y0,
                         bool opaque);

EMSTATUS GLIB_drawStringOnLine(GLIB_Context_t *pContext,
                               const char *pString,
                               uint8_t line,
                               GLIB_Align_t align,
                               int32_t xOffset,
                               int32_t yOffset,
                               bool opaque);

EMSTATUS GLIB_setFont(GLIB_Context_t *pContext,
                      GLIB_Font_t *pFont);

#ifdef __cplusplus
}
#endif

#endif // __GLIB_STRING_H_
