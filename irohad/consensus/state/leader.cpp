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
#include <spdlog/spdlog.h>

static auto console = spdlog::stdout_color_mt("leader");

namespace iroha {
  void Leader::on_proposal(const Proposal *proposal) {
    console->info("Proposal handled");

    switch (state_) {
      case State::IDLE: {
        // multicast a proposal

        this->state_ = State::SENT_PROPOSAL;

        break;
      }
      case State::SENT_PROPOSAL: {
        // wait for commit
        console->info("I am waiting for a commit, but I received a proposal");
        break;
      }
      default: {
        console->error("Leader is at undefined state");
        break;
      }
    }
  }

  Role Leader::self() { return Role::LEADER; }

  void Leader::on_commit(const Commit *commit) {
    console->info("Commit handled");

    {
      // check if we received commit for last proposal
    }

    // and commit it
    Member::on_commit(commit);
    state_ = State::IDLE;
  }

  Leader::Leader() { this->state_ = State::IDLE; }
}

