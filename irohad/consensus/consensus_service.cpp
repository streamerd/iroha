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
iroha::Status iroha::ConsensusService::SendProposal(iroha::ServerContext *context,
                                                    iroha::Proposal *request,
                                                    iroha::Ack *reply) {
  publish(request);
  reply->set_type(Ack::PROPOSAL_RECEIVED);
  return iroha::Status::OK;
}
iroha::Status iroha::ConsensusService::SendVote(iroha::ServerContext *context,
                                                iroha::Vote *request,
                                                iroha::Ack *reply) {
  publish(request);
  reply->set_type(Ack::VOTE_RECEIVED);
  return iroha::Status::OK;
}
iroha::Status iroha::ConsensusService::SendCommit(iroha::ServerContext *context,
                                                  iroha::Commit *request,
                                                  iroha::Ack *reply) {
  publish(request);
  reply->set_type(Ack::COMMIT_RECEIVED);
  return iroha::Status::OK;
}
iroha::Status iroha::ConsensusService::SendAbort(iroha::ServerContext *context,
                                                 iroha::Abort *request,
                                                 iroha::Ack *reply) {
  publish(request);
  reply->set_type(Ack::ABORT_RECEIVED);
  return iroha::Status::OK;
}
