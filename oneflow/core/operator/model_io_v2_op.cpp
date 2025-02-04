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
#include "oneflow/core/operator/operator.h"
#include "oneflow/core/register/tensor_slice_view.h"
#include "oneflow/core/common/balanced_splitter.h"

namespace oneflow {

class ModelInitV2Op : public Operator {
 public:
  Maybe<void> InitFromOpConf() override {
    CHECK(op_conf().has_model_init_v2_conf());
    FOR_RANGE(int64_t, i, 0, op_conf().model_init_v2_conf().ref_size()) {
      EnrollInputBn(GenRepeatedBn("ref", i), false)->set_is_mutable(true);
    }
    return Maybe<void>::Ok();
  }

  Maybe<void> InferLogicalOutBlobDescs(
      const std::function<BlobDesc*(const std::string&)>& BlobDesc4BnInOp,
      const ParallelDesc& parallel_desc) const override {
    return Maybe<void>::Ok();
  }

  Maybe<void> InferOutBlobDescs(
      const std::function<BlobDesc*(const std::string&)>& GetBlobDesc4BnInOp,
      const ParallelContext* parallel_ctx) const override {
    return Maybe<void>::Ok();
  }

 private:
  Maybe<void> InferNdSbpSignature(cfg::NdSbpSignature* nd_sbp_signature,
                                  const cfg::NdSbpSignature& nd_sbp_constraints,
                                  const ParallelDesc& parallel_desc,
                                  std::function<Maybe<const NdSbpInferHint*>(const std::string&)>
                                      NdSbpInferHint4Ibn) const override {
    for (const auto& bn : input_bns()) {
      (*nd_sbp_signature->mutable_bn_in_op2nd_sbp())[bn] = JUST(NdSbpInferHint4Ibn(bn))->nd_sbp();
    }
    return Maybe<void>::Ok();
  }
};

REGISTER_OP(OperatorConf::kModelInitV2Conf, ModelInitV2Op);

class ModelLoadV2Op : public Operator {
 public:
  Maybe<void> InitFromOpConf() override {
    CHECK(op_conf().has_model_load_v2_conf());
    EnrollInputBn("path", false);
    FOR_RANGE(int64_t, i, 0, op_conf().model_load_v2_conf().ref_size()) {
      EnrollInputBn(GenRepeatedBn("ref", i), false)->set_is_mutable(true);
    }
    return Maybe<void>::Ok();
  }

  Maybe<void> InferLogicalOutBlobDescs(
      const std::function<BlobDesc*(const std::string&)>& BlobDesc4BnInOp,
      const ParallelDesc& parallel_desc) const override {
    return Maybe<void>::Ok();
  }

  Maybe<void> InferOutBlobDescs(
      const std::function<BlobDesc*(const std::string&)>& GetBlobDesc4BnInOp,
      const ParallelContext* parallel_ctx) const override {
    return Maybe<void>::Ok();
  }

 private:
  Maybe<void> InferNdSbpSignature(cfg::NdSbpSignature* nd_sbp_signature,
                                  const cfg::NdSbpSignature& nd_sbp_constraints,
                                  const ParallelDesc& parallel_desc,
                                  std::function<Maybe<const NdSbpInferHint*>(const std::string&)>
                                      NdSbpInferHint4Ibn) const override {
    FOR_RANGE(int64_t, i, 0, op_conf().model_load_v2_conf().ref_size()) {
      const std::string ref_bn = GenRepeatedBn("ref", i);
      (*nd_sbp_signature->mutable_bn_in_op2nd_sbp())[ref_bn] =
          JUST(NdSbpInferHint4Ibn(ref_bn))->nd_sbp();
    }
    const auto& hierarchy = parallel_desc.hierarchy();
    for (int64_t i = 0; i < hierarchy->NumAxes(); ++i) {
      (*nd_sbp_signature->mutable_bn_in_op2nd_sbp())["path"]
          .add_sbp_parallel()
          ->mutable_broadcast_parallel();
    }
    return Maybe<void>::Ok();
  }
};

REGISTER_OP(OperatorConf::kModelLoadV2Conf, ModelLoadV2Op);

class ModelSaveV2Op final : public Operator {
 public:
  OF_DISALLOW_COPY_AND_MOVE(ModelSaveV2Op);
  ModelSaveV2Op() = default;
  ~ModelSaveV2Op() override = default;

  Maybe<void> InitFromOpConf() override {
    CHECK(op_conf().has_model_save_v2_conf());
    EnrollInputBn("path", false);
    EnrollRepeatedInputBn("in", false);
    return Maybe<void>::Ok();
  }

  Maybe<void> InferLogicalOutBlobDescs(
      const std::function<BlobDesc*(const std::string&)>& BlobDesc4BnInOp,
      const ParallelDesc& parallel_desc) const override {
    return Maybe<void>::Ok();
  }

  Maybe<void> InferOutBlobDescs(
      const std::function<BlobDesc*(const std::string&)>& GetBlobDesc4BnInOp,
      const ParallelContext* parallel_ctx) const override {
    return Maybe<void>::Ok();
  }

 private:
  Maybe<void> InferNdSbpSignature(cfg::NdSbpSignature* nd_sbp_signature,
                                  const cfg::NdSbpSignature& nd_sbp_constraints,
                                  const ParallelDesc& parallel_desc,
                                  std::function<Maybe<const NdSbpInferHint*>(const std::string&)>
                                      NdSbpInferHint4Ibn) const override {
    FOR_RANGE(int64_t, i, 0, op_conf().model_save_v2_conf().in_size()) {
      const std::string in_bn = GenRepeatedBn("in", i);
      (*nd_sbp_signature->mutable_bn_in_op2nd_sbp())[in_bn] =
          JUST(NdSbpInferHint4Ibn(in_bn))->nd_sbp();
    }
    const auto& hierarchy = parallel_desc.hierarchy();
    for (int64_t i = 0; i < hierarchy->NumAxes(); ++i) {
      (*nd_sbp_signature->mutable_bn_in_op2nd_sbp())["path"]
          .add_sbp_parallel()
          ->mutable_broadcast_parallel();
    }
    return Maybe<void>::Ok();
  }
};

REGISTER_OP(OperatorConf::kModelSaveV2Conf, ModelSaveV2Op);

}  // namespace oneflow
