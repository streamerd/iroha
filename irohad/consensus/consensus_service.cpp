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

namespace consensus {
  Status ConsensusService::OnProposal(ServerContext *context,
                                      const Proposal *request, Null *reply) {
    // TODO: verify request data
    this->publish(request);
    return Status::OK;
  }

  Status ConsensusService::OnVote(ServerContext *context, const Vote *request,
                                  Null *reply) {
    // TODO: verify request data
    this->publish(request);
    return Status::OK;
  }

  Status ConsensusService::OnCommit(ServerContext *context,
                                    const Commit *request, Null *reply) {
    // TODO: verify request data
    this->publish(request);
    return Status::OK;
  }

  Status ConsensusService::OnAbort(ServerContext *context, const Abort *request,
                                   Null *reply) {
    // TODO: verify request data
    this->publish(request);
    return Status::OK;
  }

  ConsensusService::ConsensusService() {}
}