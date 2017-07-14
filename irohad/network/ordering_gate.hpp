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

#ifndef IROHA_ORDERING_GATE_HPP
#define IROHA_ORDERING_GATE_HPP

#include <rxcpp/rx-observable.hpp>

#include <block.pb.h>

namespace network {

  using Transaction = iroha::protocol::Transaction;

  class OrderingGate {
    static grpc::Status gate(
        iroha::protocol::Transaction const& request,
        iroha::protocol::ToriiResponse& response
    ){

      // TODO

      response.set_code(iroha::protocol::ResponseCode::OK);
      response.set_message("Torii async response");
      return grpc::Status::OK;
    }
  };



  class OrderingGate : GRPCGate<request,response> {

    void start(){
      enqueue<Tx,TxResponse>(

      );
    }

    grpc::Status gate(
        request,
        response
    ){
      // TODO
      // Async の処理を
      return response;
    }

  };
}


#endif //IROHA_ORDERING_GATE_HPP
