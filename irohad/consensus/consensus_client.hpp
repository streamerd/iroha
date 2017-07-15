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

#ifndef IROHA_CONSENSUS_CLIENT_HPP
#define IROHA_CONSENSUS_CLIENT_HPP

#include <consensus.grpc.pb.h>
#include <consensus.pb.h>
#include <logger/logger.hpp>
#include "messages.hpp"

namespace iroha {


  using grpc::Channel;
  using consensus::Sumeragi;

  class ConsensusClient {
   public:
    ConsensusClient(std::string ip, uint16_t port);

    void SendProposal(Proposal* proposal);
    void SendVote(Vote* vote);
    void SendCommit(Commit* commit);
    void SendAbort(Abort* abort);

   private:

    logger::Logger log;
    std::unique_ptr<Sumeragi::Stub> stub_;
  };
}

#endif  // IROHA_CONSENSUS_CLIENT_HPP
