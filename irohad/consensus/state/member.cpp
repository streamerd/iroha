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

#include "member.hpp"

namespace iroha {

  Role Member::self() { return Role::MEMBER; }

  void Member::on_commit(const Commit *commit) {
    console_->info("Commit handled");
  }

  void Member::on_vote(const Vote *vote) { console_->info("Vote handled"); }

  void Member::on_abort(const Abort *abort) { console_->info("Abort handled"); }

  void Member::on_proposal(const Proposal *proposal) {
    console_->info("Proposal handled");
  }

  std::string Member::ledgerToString() {
    std::string s = "[\n";
    auto size = ledger.size();
    for (auto i = 0u; i < size; i++) {
      s += "  ";
      s += ledger[i];
      if (i != size - 1) s += ",\n";
    }
    s += "\n]\n";
    return s;
  }

  Member::Member() {
    console_ = spdlog::get("member");
    if (!console_) {
      console_ = spdlog::stdout_color_st("member");
    }
  }
}
