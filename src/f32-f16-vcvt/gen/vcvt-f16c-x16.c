// Auto-generated file. Do not edit!
//   Template: src/f32-f16-vcvt/f16c.c.in
//   Generator: tools/xngen
//
// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/intrinsics-polyfill.h>
#include <xnnpack/vcvt.h>


static const int32_t mask_table[14] = {-1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0};

void xnn_f32_f16_vcvt_ukernel__f16c_x16(
    size_t n,
    const float* input,
    void* output,
    const void* params)
{
  assert(n != 0);
  assert(n % sizeof(float) == 0);
  assert(input != NULL);
  assert(output != NULL);

  uint16_t* o = (uint16_t*) output;
  for (; n >= 16 * sizeof(float); n -= 16 * sizeof(float)) {
    const __m256 vf0 = _mm256_loadu_ps(input);
    const __m256 vf1 = _mm256_loadu_ps(input + 8);
    input += 16;

    _mm_storeu_si128((__m128i*) o, _mm256_cvtps_ph(vf0, _MM_FROUND_NO_EXC));
    _mm_storeu_si128((__m128i*) (o + 8), _mm256_cvtps_ph(vf1, _MM_FROUND_NO_EXC));
    o += 16;
  }
  for (; n >= 8 * sizeof(float); n -= 8 * sizeof(float)) {
    const __m256 vf = _mm256_loadu_ps(input);
    input += 8;

    _mm_storeu_si128((__m128i*) o, _mm256_cvtps_ph(vf, _MM_FROUND_NO_EXC));
    o += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(float));
    assert(n <= 7 * sizeof(float));
    const __m256i vmask = _mm256_loadu_si256((const __m256i*) ((uintptr_t) &mask_table[7] - n));

    const __m256 vf = _mm256_maskload_ps(input, vmask);

    __m128 vf_lo = _mm256_castps256_ps128(vf);
    if (n & (4 * sizeof(float))) {
      _mm_storel_epi64((__m128i*) o, _mm_cvtps_ph(vf_lo, _MM_FROUND_NO_EXC));
      vf_lo = _mm256_extractf128_ps(vf, 1);
      o += 4;
    }
    __m128i vh = _mm_cvtps_ph(vf_lo, _MM_FROUND_NO_EXC);
    if (n & (2 * sizeof(float))) {
      _mm_storeu_si32(o, vh);
      vh = _mm_srli_epi64(vh, 32);
      o += 2;
    }
    if (n & (1 * sizeof(float))) {
      *((uint16_t*) o) = _mm_extract_epi16(vh, 0);
    }
  }
}
