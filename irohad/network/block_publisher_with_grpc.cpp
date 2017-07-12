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

#include <network/block_publisher_with_grpc.hpp>

namespace network {

    using iroha::protocol::Block;
    using iroha::protocol::ApiBlock;
    using iroha::protocol::ApiResponse;

    iroha::protocol::ApiResponse BlockPublisherClient::sendBlock(
      const iroha::protocol::Block &block,
      const std::string &targetIp
    ){
      ApiClient client(targetIp, 50051); // TODO: Get port from config
      ApiBlock apiBlock;
      return client.receiveBlock(apiBlock);
    }


    ApiClient::ApiClient(const std::string& ip, int port) {
      // TODO(motxx): call validation of ip format and port.
      auto channel = grpc::CreateChannel(ip + ":" + std::to_string(port), grpc::InsecureChannelCredentials());
      stub_ = iroha::protocol::ApiService::NewStub(channel);
    }

    ApiResponse ApiClient::receiveBlock(const ApiBlock& block) {
      ApiResponse response;
      auto status = stub_->receiveBlock(&context_, block, &response);

      if (status.ok()) {
        return response;
      } else {
        response.Clear();
        response.set_code(iroha::protocol::FAIL);
        return response;
      }
    }

}