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

#ifndef IROHA_FOLLOWER_HPP
#define IROHA_FOLLOWER_HPP

#include <common/types.hpp>
#include <consensus/messages.hpp>

namespace iroha {

  enum class Role : uint8_t {
    UNDEFINED = 0,
    MEMBER = 1,     // the one who does not participate in validation, but may receive commits
    VALIDATOR = 2,  // the one who participates in validation, may receive proposals and commits
    PROXY_TAIL = 3, // the one who creates commits, may receive proposals and votes
    LEADER = 4      // the one who has ordering service
  };

  class Member {
   public:
    virtual Role self();
    virtual void on_proposal(const Proposal *proposal);
    virtual void on_commit(const Commit *commit);
    virtual void on_vote(const Vote *vote);
    virtual void on_abort(const Abort *abort);
  };
}
#endif  // IROHA_FOLLOWER_HPP
