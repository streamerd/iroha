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

#include "consensus_client.hpp"
#include <grpc++/create_channel.h>

namespace iroha {

  void ConsensusClient::SendProposal(Proposal *proposal) {
    grpc::ClientContext context;
    grpc::Status status;
    Null ack;

    status = stub_->OnProposal(&context, *proposal, &ack);

    if (status.ok()) {
      // status ok;
      // TODO
    } else {
      log.error("SendProposal RPC failed. details={}, message={}",
                     status.error_details(), status.error_message());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  void ConsensusClient::SendVote(Vote *vote) {
    grpc::ClientContext context;
    grpc::Status status;
    Null ack;

    status = stub_->OnVote(&context, *vote, &ack);

    if (status.ok()) {
      // status ok
      // TODO
    } else {
      log.error("SendVote RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  void ConsensusClient::SendCommit(Commit *commit) {
    grpc::ClientContext context;
    grpc::Status status;
    Null ack;

    status = stub_->OnCommit(&context, *commit, &ack);

    if (status.ok()) {
      // status ok
      // TODO
    } else {
      log.error("SendCommit RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  void ConsensusClient::SendAbort(Abort *abort) {
    grpc::ClientContext context;
    grpc::Status status;
    Null ack;

    status = stub_->OnAbort(&context, *abort, &ack);

    if (status.ok()) {
      // status ok
      // TODO
    } else {
      log.error("SendAbort RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    };
  }

  ConsensusClient::ConsensusClient(std::string ip, uint16_t port)
      : log("consensus_client|" + ip + ":" + std::to_string(port)) {
    auto channel = grpc::CreateChannel(ip + ":" + std::to_string(port),
                                       grpc::InsecureChannelCredentials());
    stub_ = consensus::Sumeragi::NewStub(channel);
  }
}