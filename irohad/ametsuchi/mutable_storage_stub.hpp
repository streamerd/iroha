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

#ifndef IROHA_MUTABLE_STORAGE_STUB_HPP
#define IROHA_MUTABLE_STORAGE_STUB_HPP

#include <ametsuchi/storage_stub.hpp>
#include <ametsuchi/command_executor_stub.hpp>
#include <ametsuchi/mutable_storage.hpp>

namespace iroha {
  namespace ametsuchi {
    class MutableStorageStub : public MutableStorage {
     public:
      MutableStorageStub(StorageStub& ametsuchi);
      bool apply(const dao::Block &block,
                 std::function<bool(const dao::Block &,
                                    CommandExecutor &,
                                    WsvQuery &,
                                    const dao::Block &)> function) override;

     private:
      StorageStub& ametsuchi_;
      CommandExecutorStub executor_;
    };
  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_MUTABLE_STORAGE_STUB_HPP
