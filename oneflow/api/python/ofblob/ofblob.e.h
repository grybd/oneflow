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
#ifndef ONEFLOW_API_PYTHON_OFBLOB_OFBLOB_E_H_
#define ONEFLOW_API_PYTHON_OFBLOB_OFBLOB_E_H_

#include "oneflow/core/common/foreign_lock_helper.h"
#include "oneflow/core/common/type_traits.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "oneflow/core/register/ofblob.h"
#include "oneflow/core/common/preprocessor.h"
#include "oneflow/core/common/data_type_seq.h"
#include "oneflow/core/common/maybe.h"
#include "oneflow/extension/python/numpy.h"

namespace py = pybind11;

namespace oneflow {

struct OfBlob_CopyBuffer {
  template<typename T>
  static Maybe<void> From(uint64_t of_blob_ptr, const NumPyArrayPtr& array) {
    T* buf_ptr = (T*)array.data();
    size_t size = array.size();
    auto* of_blob = reinterpret_cast<OfBlob*>(of_blob_ptr);
    of_blob->AutoMemCopyFrom<T>(buf_ptr, size);
    return Maybe<void>::Ok();
  }

  template<typename T>
  static Maybe<void> To(uint64_t of_blob_ptr, const NumPyArrayPtr& array) {
    T* buf_ptr = (T*)array.data();
    size_t size = array.size();
    auto* of_blob = reinterpret_cast<OfBlob*>(of_blob_ptr);
    of_blob->AutoMemCopyTo<T>(buf_ptr, size);
    return Maybe<void>::Ok();
  }
};

}  // namespace oneflow

#define DEFINE_COPIER(T, type_proto)                                                               \
  inline void OfBlob_CopyToBuffer_##T(uint64_t of_blob_ptr, const oneflow::NumPyArrayPtr& array) { \
    oneflow::OfBlob_CopyBuffer::To<T>(of_blob_ptr, array).GetOrThrow();                            \
  }                                                                                                \
  inline void OfBlob_CopyFromBuffer_##T(uint64_t of_blob_ptr,                                      \
                                        const oneflow::NumPyArrayPtr& array) {                     \
    oneflow::OfBlob_CopyBuffer::From<T>(of_blob_ptr, array).GetOrThrow();                          \
  }

OF_PP_FOR_EACH_TUPLE(DEFINE_COPIER, POD_DATA_TYPE_SEQ);

#undef DEFINE_COPIER

inline std::string Dtype_GetOfBlobCopyToBufferFuncName(int64_t dtype) {
  using namespace oneflow;
  static const HashMap<int64_t, std::string> data_type2func_name{
#define DATA_TYPE_FUNC_NAME_PAIR(type_cpp, type_proto) \
  {type_proto, "OfBlob_CopyToBuffer_" #type_cpp},
      OF_PP_FOR_EACH_TUPLE(DATA_TYPE_FUNC_NAME_PAIR, POD_DATA_TYPE_SEQ)
#undef DATA_TYPE_FUNC_NAME_PAIR
  };
  return data_type2func_name.at(dtype);
}

inline std::string Dtype_GetOfBlobCopyFromBufferFuncName(int64_t dtype) {
  using namespace oneflow;
  static const HashMap<int64_t, std::string> data_type2func_name{
#define DATA_TYPE_FUNC_NAME_PAIR(type_cpp, type_proto) \
  {type_proto, "OfBlob_CopyFromBuffer_" #type_cpp},
      OF_PP_FOR_EACH_TUPLE(DATA_TYPE_FUNC_NAME_PAIR, POD_DATA_TYPE_SEQ)
#undef DATA_TYPE_FUNC_NAME_PAIR
  };
  return data_type2func_name.at(dtype);
}

#endif  // ONEFLOW_API_PYTHON_OFBLOB_OFBLOB_E_H_
