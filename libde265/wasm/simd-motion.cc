/*
 * H.265 video codec.
 * Copyright (c) 2020, Dmitry Ilyin
 *
 * This file is part of libde265.
 *
 * libde265 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libde265 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libde265.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <wasm_simd128.h>

#include "libde265/util.h"
#include "simd-motion.h"

ALIGNED_16(const int8_t)
epel_filters[7][16] = {
    {-2, 58, 10, -2, -2, 58, 10, -2, -2, 58, 10, -2, -2, 58, 10, -2},
    {-4, 54, 16, -2, -4, 54, 16, -2, -4, 54, 16, -2, -4, 54, 16, -2},
    {-6, 46, 28, -4, -6, 46, 28, -4, -6, 46, 28, -4, -6, 46, 28, -4},
    {-4, 36, 36, -4, -4, 36, 36, -4, -4, 36, 36, -4, -4, 36, 36, -4},
    {-4, 28, 46, -6, -4, 28, 46, -6, -4, 28, 46, -6, -4, 28, 46, -6},
    {-2, 16, 54, -4, -2, 16, 54, -4, -2, 16, 54, -4, -2, 16, 54, -4},
    {-2, 10, 58, -2, -2, 10, 58, -2, -2, 10, 58, -2, -2, 10, 58, -2},
};

static const uint8_t qpel_extra_before[4] = {0, 3, 3, 2};
static const uint8_t qpel_extra_after[4] = {0, 3, 4, 4};
static const uint8_t qpel_extra[4] = {0, 6, 7, 6};

static const int epel_extra_before = 1;
static const int epel_extra_after = 2;
static const int epel_extra = 3;

void print128(const char *prefix, v128_t r) {
  unsigned char buf[16];

  wasm_v128_store(&buf, r);
  //*(__m128i *)buf = r;
  printf("%s ", prefix);

  for (int i = 0; i < 16; i++) {
    if (i > 0) {
      printf(":");
    }
    printf("%02x", buf[i]);
  }

  printf("\n");
}

void printm32(const char *prefix, unsigned char *p) {
  printf("%s ", prefix);

  for (int i = 0; i < 4; i++) {
    if (i > 0) {
      printf(":");
    }
    printf("%02x", p[i]);
  }

  printf("\n");
}

#define BIT_DEPTH 8

void put_unweighted_pred_8_fallback(uint8_t *dst, ptrdiff_t dststride,
                                    const int16_t *src, ptrdiff_t srcstride,
                                    int width, int height);

void hevc_put_unweighted_pred_8_wasm(uint8_t *_dst, ptrdiff_t dststride,
                                     const int16_t *src, ptrdiff_t srcstride,
                                     int width, int height) {

  put_unweighted_pred_8_fallback(_dst, dststride, src, srcstride, width,
                                 height);
#if 0
  int x, y;
  uint8_t *dst = (uint8_t *)_dst;
  __m128i r0, r1, f0;

  f0 = _mm_set1_epi16(32);

  if (!(width & 15)) {
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x += 16) {
        r0 = _mm_load_si128((__m128i *)(src + x));

        r1 = _mm_load_si128((__m128i *)(src + x + 8));
        r0 = _mm_adds_epi16(r0, f0);

        r1 = _mm_adds_epi16(r1, f0);
        r0 = _mm_srai_epi16(r0, 6);
        r1 = _mm_srai_epi16(r1, 6);
        r0 = _mm_packus_epi16(r0, r1);

        _mm_storeu_si128((__m128i *)(dst + x), r0);
      }
      dst += dststride;
      src += srcstride;
    }
  } else if (!(width & 7)) {
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x += 8) {
        r0 = _mm_load_si128((__m128i *)(src + x));

        r0 = _mm_adds_epi16(r0, f0);

        r0 = _mm_srai_epi16(r0, 6);
        r0 = _mm_packus_epi16(r0, r0);

        _mm_storel_epi64((__m128i *)(dst + x), r0);
      }
      dst += dststride;
      src += srcstride;
    }
  } else if (!(width & 3)) {
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x += 4) {
        r0 = _mm_loadl_epi64((__m128i *)(src + x));
        r0 = _mm_adds_epi16(r0, f0);

        r0 = _mm_srai_epi16(r0, 6);
        r0 = _mm_packus_epi16(r0, r0);
#if MASKMOVE
        _mm_maskmoveu_si128(
            r0,
            _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1),
            (char *)(dst + x));
#else
        // r0 = _mm_shuffle_epi32 (r0, 0x00);
        *((uint32_t *)(dst + x)) = _mm_cvtsi128_si32(r0);
#endif
      }
      dst += dststride;
      src += srcstride;
    }
  } else {
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x += 2) {
        r0 = _mm_loadl_epi64((__m128i *)(src + x));
        r0 = _mm_adds_epi16(r0, f0);

        r0 = _mm_srai_epi16(r0, 6);
        r0 = _mm_packus_epi16(r0, r0);
#if MASKMOVE
        _mm_maskmoveu_si128(
            r0, _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1),
            (char *)(dst + x));
#else
        *((uint16_t *)(dst + x)) = _mm_cvtsi128_si32(r0);
#endif
      }
      dst += dststride;
      src += srcstride;
    }
  }
#endif
}
