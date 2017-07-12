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

#include "proxy_tail.hpp"

static auto console = spdlog::stdout_color_st("proxy tail");

namespace iroha {

  void ProxyTail::on_proposal(Proposal *proposal) {
    // TODO: add tx to votes
  }

  void ProxyTail::on_vote(Vote *vote) {
    console->debug("Vote handled");

    // weird if vote went w/o sig
    if (!vote->has_sig()) return;

    auto pubkey = vote->sig().pubkey();

    if (votes.find(pubkey) != votes.end()) {
      // peer already voted, just skip
      return;
    }

    // haven't voted yet
    votes[pubkey] = vote;

    if (!check_votes()) return;

    // start building commit msg
    auto commit = consensus::Sumeragi::Commit();
    // dunno how to grub them so todo:
    // commit.add_transactions
    for (auto &v : votes) {
      *commit.add_sigs() = v.sig();
    }

    auto &v = votes.begin()->second;
    commit.set_commit_gmroot(v.next_gmroot);
    commit.set_commit_height(v.next_height);
  }

  bool ProxyTail::check_votes() const {
    if (votes.size() < 2 * this->peerService->f() + 1) return false;

    auto next = votes.begin();
    auto prev = next++;
    for (auto prev = next++; next != votes.end(); prev++, next++) {
      if (!(prev->second.next_gmroot() == next->second.next_gmroot()))
        return false;
    }

    return true;
  }

  Role ProxyTail::self() { return Role::PROXY_TAIL; }
}
