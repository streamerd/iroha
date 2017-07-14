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

#include "consensus_service.hpp"

namespace iroha {

  Status ConsensusService::SendProposal(ServerContext *context,
                                                      const Proposal *request,
                                                      Ack *reply) {
    publish(request);
    reply->set_type(Ack::PROPOSAL_RECEIVED);
    return Status::OK;
  }

  Status ConsensusService::SendVote(ServerContext *context,
                                                  const Vote *request,
                                                  Ack *reply) {
    publish(request);
    reply->set_type(Ack::VOTE_RECEIVED);
    return Status::OK;
  }

  Status ConsensusService::SendCommit(ServerContext *context,
                                                    const Commit *request,
                                                    Ack *reply) {
    publish(request);
    reply->set_type(Ack::COMMIT_RECEIVED);
    return Status::OK;
  }

  Status ConsensusService::SendAbort(ServerContext *context,
                                                   const Abort *request,
                                                   Ack *reply) {
    publish(request);
    reply->set_type(Ack::ABORT_RECEIVED);
    return Status::OK;
  }
}
