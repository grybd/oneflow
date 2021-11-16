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
#ifndef ONEFLOW_CORE_DEVICE_DEVICE_CONTEXT_H_
#define ONEFLOW_CORE_DEVICE_DEVICE_CONTEXT_H_

#include "oneflow/core/device/cuda_util.h"
#include "oneflow/core/common/auto_registration_factory.h"
#include "oneflow/core/ep/include/stream.h"

namespace oneflow {

namespace vm {

class Allocator;

}  // namespace vm

class DeviceCtx {
 public:
  OF_DISALLOW_COPY_AND_MOVE(DeviceCtx);
  virtual ~DeviceCtx() = default;

#ifdef WITH_CUDA
  virtual cudaStream_t cuda_stream() const {
    UNIMPLEMENTED();
    return nullptr;
  }
  virtual cublasHandle_t cublas_handle() const {
    UNIMPLEMENTED();
    return nullptr;
  }
  virtual cudnnHandle_t cudnn_handle() const {
    UNIMPLEMENTED();
    return nullptr;
  }
#endif

  virtual ep::Stream* stream() = 0;
  virtual void SyncDevice() { UNIMPLEMENTED(); }

  virtual vm::Allocator* mut_allocator() {
    UNIMPLEMENTED();
    return nullptr;
  }

  virtual DeviceType device_type() const = 0;

 protected:
  DeviceCtx() = default;

 private:
};

class DeviceCtxProvider {
 public:
  OF_DISALLOW_COPY_AND_MOVE(DeviceCtxProvider);
  DeviceCtxProvider() = default;
  virtual ~DeviceCtxProvider() = default;

  virtual std::shared_ptr<DeviceCtx> GetDeviceCtx() = 0;
};

#define REGISTER_DEVICE_CONTEXT(device, creator) \
  REGISTER_CLASS_CREATOR(int, device, DeviceCtx, creator, const ThreadCtx&)

}  // namespace oneflow

#endif  // ONEFLOW_CORE_DEVICE_DEVICE_CONTEXT_H_
