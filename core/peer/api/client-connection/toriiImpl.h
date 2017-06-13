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

#ifndef TORII_IMPL_H
#define TORII_IMPL_H

#include <grpc++/server_context.h>
#include "interface.pb.h"
#include "interface.grpc.pb.h"

namespace grpc_connection {

using grpc::Status;
using grpc::ServerContext;
using api::Ping;
using api::Pong;

class ToriiImpl final : public api::Torii::Service {
  Status SendPing(ServerContext *context, Ping *ping, Pong *pong);
};

} // namespace grpc_connection

#endif // TORII_IMPL_H