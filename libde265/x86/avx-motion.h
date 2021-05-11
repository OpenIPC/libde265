/*
 * H.265 video codec.
 * Copyright (c) 2020 Dmitry Ilyin
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

#ifndef AVX_MOTION_H
#define AVX_MOTION_H

#include <stddef.h>
#include <stdint.h>


void ff_hevc_put_unweighted_pred_8_avx(uint8_t *_dst, ptrdiff_t dststride,
                                       const int16_t *src, ptrdiff_t srcstride,
                                       int width, int height);

#endif
