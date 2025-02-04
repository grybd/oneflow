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
#include "oneflow/core/framework/framework.h"
#include "oneflow/user/kernels/dim_gather_kernel_util.h"

namespace oneflow {
namespace user_op {

REGISTER_USER_OP("dim_gather")
    .Input("input")
    .Input("index")
    .Output("output")
    .Attr<int32_t>("dim")
    .SetTensorDescInferFn([](user_op::InferContext* ctx) -> Maybe<void> {
      const TensorDesc& in = ctx->InputTensorDesc("input", 0);
      int64_t input_num_axes = in.shape().NumAxes();
      CHECK_GT_OR_RETURN(input_num_axes, 0);
      CHECK_LE_OR_RETURN(input_num_axes, kDimGatherMaxDimCount);

      const TensorDesc& index = ctx->InputTensorDesc("index", 0);
      int64_t index_num_axes = index.shape().NumAxes();

      const int32_t dim = ctx->Attr<int32_t>("dim");
      CHECK_GE_OR_RETURN(dim, 0);
      CHECK_LT_OR_RETURN(dim, input_num_axes);
      CHECK_EQ_OR_RETURN(input_num_axes, index_num_axes);

      CHECK_EQ_OR_RETURN(in.is_dynamic(), index.is_dynamic());

      user_op::TensorDesc* out = ctx->OutputTensorDesc("output", 0);
      *out->mut_shape() = index.shape();

      return Maybe<void>::Ok();
    })
    .SetDataTypeInferFn([](user_op::InferContext* ctx) -> Maybe<void> {
      const TensorDesc& index = ctx->InputTensorDesc("index", 0);
      CHECK_OR_RETURN(IsIndexDataType(index.data_type()));
      const TensorDesc& in = ctx->InputTensorDesc("input", 0);
      user_op::TensorDesc* out = ctx->OutputTensorDesc("output", 0);
      *out->mut_data_type() = in.data_type();
      return Maybe<void>::Ok();
    })
    .SetInputArgModifyFn([](user_op::GetInputArgModifier GetInputArgModifierFn,
                            const user_op::UserOpConfWrapper&) -> Maybe<void> {
      user_op::InputArgModifier* indices_modifier = GetInputArgModifierFn("index", 0);
      CHECK_OR_RETURN(indices_modifier != nullptr);
      indices_modifier->set_requires_grad(false);
      return Maybe<void>::Ok();
    })
    .SetGetSbpFn([](user_op::SbpContext* ctx) -> Maybe<void> {
      const user_op::TensorDesc& index_tensor =
          ctx->LogicalTensorDesc4InputArgNameAndIndex("index", 0);
      int64_t index_num_axes = index_tensor.shape().NumAxes();
      const int32_t dim = ctx->Attr<int32_t>("dim");

      FOR_RANGE(int64_t, i, 0, index_num_axes) {
        if (i != dim) {
          ctx->NewBuilder()
              .Split(user_op::OpArg("index", 0), i)
              .Split(user_op::OpArg("input", 0), i)
              .Split(user_op::OpArg("output", 0), i)
              .Build();
        } else if (i == dim) {
          ctx->NewBuilder()
              .Broadcast(user_op::OpArg("input", 0))
              .Split(user_op::OpArg("index", 0), i)
              .Split(user_op::OpArg("output", 0), i)
              .Build();
        }
      }
      ctx->NewBuilder()
          .PartialSum(user_op::OpArg("input", 0))
          .Broadcast(user_op::OpArg("index", 0))
          .PartialSum(user_op::OpArg("output", 0))
          .Build();
      return Maybe<void>::Ok();
    });

REGISTER_USER_OP_GRAD("dim_gather")
    .SetBackwardOpConfGenFn([](user_op::BackwardOpConfContext* ctx) -> Maybe<void> {
      const auto op_grad_name = ctx->FwOp().op_name() + "_grad";

      ctx->DefineOp(op_grad_name, [&ctx](user_op::BackwardOpBuilder& builder) {
        return builder
            .OpTypeName(
                "dim_scatter_add_like")  // dim_scatter_add_like(like, dim, index, src) -> output
            .InputBind("index", ctx->FwOp().input("index", 0))  // scatter.index <- gather.index
            .InputBind("src",
                       ctx->FwOp().output_grad("output", 0))  // scatter.src <- grad of gather.out
            .InputBind("like", ctx->FwOp().input("input", 0))
            .Output("output")
            .Attr("dim", ctx->FwOp().attr<int32_t>("dim"))
            .Build();
      });

      ctx->FwOp().InputGradBind(user_op::OpArg("input", 0),
                                [&ctx, &op_grad_name]() -> const std::string& {
                                  return ctx->GetOp(op_grad_name).output("output", 0);
                                });
      return Maybe<void>::Ok();
    });

}  // namespace user_op

}  // namespace oneflow
