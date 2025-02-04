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
#ifndef ONEFLOW_CORE_EP_COMMON_PRIMITIVE_ELEMENTWISE_UNARY_H_
#define ONEFLOW_CORE_EP_COMMON_PRIMITIVE_ELEMENTWISE_UNARY_H_

#include "oneflow/core/ep/include/primitive/elementwise_unary.h"

namespace oneflow {

namespace ep {
namespace primitive {

#define UNARY_MATH_OP_SEQ OF_PP_MAKE_TUPLE_SEQ(UnaryOp::kRelu)

#define UNARY_LOGICAL_OP_SEQ OF_PP_MAKE_TUPLE_SEQ(UnaryOp::kLogicalNot)

}  // namespace primitive
}  // namespace ep
}  // namespace oneflow

#endif  // ONEFLOW_CORE_EP_COMMON_PRIMITIVE_ELEMENTWISE_UNARY_H_
