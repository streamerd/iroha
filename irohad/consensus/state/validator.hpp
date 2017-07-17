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

#ifndef IROHA_VALIDATOR_HPP
#define IROHA_VALIDATOR_HPP

#include "member.hpp"

namespace iroha {




  class Validator : public Member{
   public:

    Validator();

    virtual Role self() override;

    virtual void on_proposal(const Proposal* proposal) override;

   private:
    enum class State : uint8_t {
      UNDEFINED = 0,
      IDLE = 1, // waiting for a proposal
      VOTED = 2 // waiting for a commit
    };

    State state;

  };
}

#endif  // IROHA_VALIDATOR_HPP
