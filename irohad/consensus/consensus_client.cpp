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
#include <spdlog/spdlog.h>
namespace iroha {

  static auto console = spdlog::stdout_color_st("consensus_client");

  Ack ConsensusClient::SendProposal(Proposal *proposal) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendProposal(&context, *proposal, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console->error("SendProposal RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  Ack ConsensusClient::SendVote(Vote *vote) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendVote(&context, *vote, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console->error("SendVote RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  Ack ConsensusClient::SendCommit(Commit *commit) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendCommit(&context, *commit, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console->error("SendCommit RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  Ack ConsensusClient::SendAbort(Abort *abort) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendAbort(&context, *abort, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console->error("SendAbort RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    };
  }
}