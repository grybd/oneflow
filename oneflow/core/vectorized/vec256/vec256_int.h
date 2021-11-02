/*
Copyright 2020 The OneFlow Authors. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef ONEFLOW_CORE_VECTORIZED_VEC256_INT_H_
#define ONEFLOW_CORE_VECTORIZED_VEC256_INT_H_
#include "oneflow/core/vectorized/vec256/vec256_base.h"

namespace oneflow {

#ifdef WITH_AVX
#include <immintrin.h>

template<typename T>
class VectorizedAvx2<T, typename std::enable_if<!(std::is_same<T, float>::value
                                                  || std::is_same<T, double>::value)>::type> {
 public:
  // static void fmadd(size_t begin, size_t end, const T* x, const T* y, T* out, T alpha) {
  // }

  static void add(size_t begin, size_t end, const T* x, const T* y, T* out) {
    for (size_t i = begin; i <= end; i++) { out[i] = x[i] + y[i]; }
  }

  static void sub(size_t begin, size_t end, const T* x, const T* y, T* out) {
    for (size_t i = begin; i <= end; i++) { out[i] = x[i] - y[i]; }
  }

  static void mul(size_t begin, size_t end, const T* x, const T* y, T* out) {
    for (size_t i = begin; i <= end; i++) { out[i] = x[i] * y[i]; }
  }

  static void div(size_t begin, size_t end, const T* x, const T* y, T* out) {
    for (size_t i = begin; i <= end; i++) { out[i] = x[i] / y[i]; }
  }
};

#endif

}  // namespace oneflow

#endif
