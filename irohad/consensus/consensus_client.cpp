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
#include <spdlog/spdlog.h>
namespace iroha {

  Ack ConsensusClient::SendProposal(const Proposal *proposal) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendProposal(&context, *proposal, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console_->error("SendProposal RPC failed. details={}, message={}",
                     status.error_details(), status.error_message());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  Ack ConsensusClient::SendVote(const Vote *vote) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendVote(&context, *vote, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console_->error("SendVote RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  Ack ConsensusClient::SendCommit(const Commit *commit) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendCommit(&context, *commit, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console_->error("SendCommit RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    }
  }

  Ack ConsensusClient::SendAbort(const Abort *abort) {
    grpc::ClientContext context;
    grpc::Status status;
    Ack ack;

    status = stub_->SendAbort(&context, *abort, &ack);

    if (status.ok()) {
      return ack;
    } else {
      console_->error("SendAbort RPC failed: {}", status.error_details());
      throw std::system_error();  // TODO: we need good exception design
    };
  }

  ConsensusClient::ConsensusClient(std::string ip, uint16_t port) {
    auto channel = grpc::CreateChannel(ip + ":" + std::to_string(port),
                                       grpc::InsecureChannelCredentials());
    stub_ = consensus::Sumeragi::NewStub(channel);
    console_ = spdlog::get("consensus_client");
    if (!console_) {
      console_ = spdlog::stdout_color_st("consensus_client");
    }
  }
}