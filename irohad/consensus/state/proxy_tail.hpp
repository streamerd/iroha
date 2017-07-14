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

#ifndef IROHA_PROXY_TAIL_HPP
#define IROHA_PROXY_TAIL_HPP

#include "validator.hpp"
namespace iroha {

  class ProxyTail final : public Validator {
   public:
    Role self() override;
    virtual void on_vote(Vote *vote) override;

   private:
    // maps voter's pubkey -> Vote
    // we need a component VoteCounter or Teller (literal translation - the one
    // who counts votes)
    // this component will store votes of validators for one round
    // this component should add vote for O(1), get winner for O(1)
    // simple std::map is O(n) solution
    std::map<ed25519::pubkey_t, Vote *> votes;
  };
}

#endif  // IROHA_PROXY_TAIL_HPP
