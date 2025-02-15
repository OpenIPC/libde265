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

#include "wasm/simd.h"
#include "wasm/simd-dct.h"
#include "wasm/simd-motion.h"

void init_acceleration_functions_wasm(struct acceleration_functions *accel) {
  accel->put_unweighted_pred_8 = ff_hevc_put_unweighted_pred_8_wasm;

  accel->put_weighted_pred_avg_8 = ff_hevc_put_weighted_pred_avg_8_wasm;

  accel->put_hevc_epel_8 = ff_hevc_put_hevc_epel_pixels_8_wasm;
  accel->put_hevc_epel_h_8 = ff_hevc_put_hevc_epel_h_8_wasm;
  accel->put_hevc_epel_v_8 = ff_hevc_put_hevc_epel_v_8_wasm;
  accel->put_hevc_epel_hv_8 = ff_hevc_put_hevc_epel_hv_8_wasm;

  accel->put_hevc_qpel_8[0][0] = ff_hevc_put_hevc_qpel_pixels_8_wasm;
  accel->put_hevc_qpel_8[0][1] = ff_hevc_put_hevc_qpel_v_1_8_wasm;
  accel->put_hevc_qpel_8[0][2] = ff_hevc_put_hevc_qpel_v_2_8_wasm;
  accel->put_hevc_qpel_8[0][3] = ff_hevc_put_hevc_qpel_v_3_8_wasm;
  accel->put_hevc_qpel_8[1][0] = ff_hevc_put_hevc_qpel_h_1_8_wasm;
  accel->put_hevc_qpel_8[1][1] = ff_hevc_put_hevc_qpel_h_1_v_1_wasm;
  accel->put_hevc_qpel_8[1][2] = ff_hevc_put_hevc_qpel_h_1_v_2_wasm;
  accel->put_hevc_qpel_8[1][3] = ff_hevc_put_hevc_qpel_h_1_v_3_wasm;
  accel->put_hevc_qpel_8[2][0] = ff_hevc_put_hevc_qpel_h_2_8_wasm;
  accel->put_hevc_qpel_8[2][1] = ff_hevc_put_hevc_qpel_h_2_v_1_wasm;
  accel->put_hevc_qpel_8[2][2] = ff_hevc_put_hevc_qpel_h_2_v_2_wasm;
  accel->put_hevc_qpel_8[2][3] = ff_hevc_put_hevc_qpel_h_2_v_3_wasm;
  accel->put_hevc_qpel_8[3][0] = ff_hevc_put_hevc_qpel_h_3_8_wasm;
  accel->put_hevc_qpel_8[3][1] = ff_hevc_put_hevc_qpel_h_3_v_1_wasm;
  accel->put_hevc_qpel_8[3][2] = ff_hevc_put_hevc_qpel_h_3_v_2_wasm;
  accel->put_hevc_qpel_8[3][3] = ff_hevc_put_hevc_qpel_h_3_v_3_wasm;

  accel->transform_skip_8 = ff_hevc_transform_skip_8_wasm;

  // actually, for these two functions, the scalar fallback seems to be faster
  // than the SSE code
  //accel->transform_4x4_luma_add_8 = ff_hevc_transform_4x4_luma_add_8_wasm;
  // SSE-4 only TODO
  //accel->transform_4x4_add_8 = ff_hevc_transform_4x4_add_8_wasm;

  accel->transform_add_8[1] = ff_hevc_transform_8x8_add_8_wasm;
  accel->transform_add_8[2] = ff_hevc_transform_16x16_add_8_wasm;
  accel->transform_add_8[3] = ff_hevc_transform_32x32_add_8_wasm;
  accel->put_unweighted_pred_8 = ff_hevc_put_unweighted_pred_8_wasm;
  accel->put_weighted_pred_avg_8 = ff_hevc_put_weighted_pred_avg_8_wasm;
}
