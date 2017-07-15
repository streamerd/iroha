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

#ifndef IROHA_CONSENSUS_SERVICE_IMPL_HPP
#define IROHA_CONSENSUS_SERVICE_IMPL_HPP

#include <consensus.grpc.pb.h>
#include <consensus/messages.hpp>

namespace iroha {

  using ServerContext = grpc::ServerContext;
  using Status = grpc::Status;

  class ConsensusService final : public consensus::Sumeragi::Service {
   public:
    ConsensusService();

    Status OnProposal(ServerContext *context, const Proposal *request,
                      Null *reply) override;
    Status OnVote(ServerContext *context, const Vote *request,
                  Null *reply) override;
    Status OnCommit(ServerContext *context, const Commit *request,
                    Null *reply) override;
    Status OnAbort(ServerContext *context, const Abort *request,
                   Null *reply) override;
  };
}

#endif  // IROHA_CONSENSUS_SERVICE_IMPL_HPP
