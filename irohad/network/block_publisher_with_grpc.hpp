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
#ifndef IROHA_BLOCK_PUBLISHER_WITH_GRPC_HPP
#define IROHA_BLOCK_PUBLISHER_WITH_GRPC_HPP

#include <network/block_publisher.hpp>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

namespace network {

  /*
   * Send confirmed block to other server without Iroha,
   * This is only interface
   */
  class BlockPublisherClientWithGrpc: BlockPublisherClient{
    iroha::protocol::ApiResponse sendBlock(
      const iroha::protocol::Block& block,
      const std::string& targetIp
    );
  };

  class ApiClient {
    public:
      ApiClient(const std::string& targetIp, int port);
      iroha::protocol::ApiResponse receiveBlock(const iroha::protocol::PublishedBlock&);
    private:
      grpc::ClientContext context_;
      std::unique_ptr<iroha::protocol::SubscriberService::Stub> stub_;
  };
}  // namespace consensus

#endif //IROHA_BLOCK_PUBLISHER_WITH_GRPC_HPP
