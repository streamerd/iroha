/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "grpcServiceManager.h"
#include <mutex>
#include <google/protobuf/stubs/common.h>

namespace iroha {

grpcServiceManager::grpcServiceManager(std::string ip, int port)
    : ip_(ip), port_(port), serverReady_(false) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

void grpcServiceManager::addService(grpc::Service &service) {
  builder_.RegisterService(&service);
}

void grpcServiceManager::run() {
  auto address = ip_ + std::to_string(port_);
  {
    std::lock_guard<std::mutex> lk(waitForServer_);
    server_ = builder_.BuildAndStart().release();
    serverReady_ = true;
  }
  serverCV_.notify_one();
  server_->Wait();
}

void grpcServiceManager::waitUntilServerReady() {
  std::unique_lock<std::mutex> lk(waitForServer_);
  serverCV_.wait(lk, [this] { return serverReady_; });
}

} // namespace iroha