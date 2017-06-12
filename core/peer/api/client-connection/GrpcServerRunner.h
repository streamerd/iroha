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

#ifndef GRPC_SERVICE_MANAGER
#define GRPC_SERVICE_MANAGER

#include <condition_variable>
#include <grpc++/grpc++.h>
#include <grpc++/server_builder.h>
#include <string>

namespace grpc_connection {

class GrpcServerRunner {
public:
  GrpcServerRunner(std::string ip, int port);
  void addService(grpc::Service &service);
  void run();
  void waitUntilServerReady();

private:
  std::string ip_;
  int port_;
  grpc::Server *server_;
  std::mutex waitForServer_;
  std::condition_variable serverCV_;
  bool serverReady_;
  grpc::ServerBuilder builder_;
};

} // namespace grpc_connection

#endif // GRPC_SERVICE_MANAGER