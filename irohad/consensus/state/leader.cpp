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

#include "leader.hpp"
#include <future>

static auto console = spdlog::stdout_color_mt("leader");

namespace iroha {
  void Leader::on_proposal(Proposal *proposal) {
    console->debug("Proposal handled");

    switch (state_) {
      case State::IDLE: {
        // multicast proposal to first 2f+1 peers
        for (auto &&peer : peerService->peers) {
          // no need in thread creation, std::async uses system-wide threads
          auto future = std::async(std::launch::async, [&console]() {
            auto ack = peer->SendProposal(proposal);
            if (ack.type() == ack.PROPOSAL_RECEIVED) {
              console->info("{} acknowledged", peer->pubkey.to_hexstring());
            } else {
              // TODO: view change
              console->info("{} did not respond, view change",
                            peer->pubkey.to_hexstring());
            }
          });
        }

        this->state_ = State::SENT_PROPOSAL;

        break;
      }
      case State::SENT_PROPOSAL: {
        // wait for commit
        console->info("I am waiting for a commit, but I received a proposal");
      }
      default: { console->error("Leader is at undefined state"); }
    }
  }

  Role Leader::self() { return Role::LEADER; }

  Leader::Leader() {
    // subscribe for proposals from ordering service
    orderingService->on<Proposal>([c{console}](const Proposal &p, auto &os) {
      c->debug("Leader received proposal from OS, size: {}",
                     p.transactions_size());
      this->on_proposal(&p);
    });
  }

  void Leader::on_commit(Commit *commit) {
    console->info("Commit handled");

    {
      // check if we received commit for last proposal
    }

    // and commit it
    Member::on_commit(commit);
    state_ = State::IDLE;
  }

  Leader::~Leader() {
    // unsubscribe from ordering service
    orderingService->clear();
  }
}
