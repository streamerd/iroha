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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <torii/processor/transaction_processor_impl.hpp>

using namespace iroha;
using ::testing::Return;
using ::testing::_;

/**
 * Mock for stateless validation
 */
class StatelessValidationMock : public validation::StatelessValidator {
 public:
  MOCK_CONST_METHOD1(validate, bool(
      const model::Transaction &transaction));
};

/**
 * Mock for ordering service
 */
class OsMock : public ordering::OrderingService {
 public:
};

/**
 * Transaction processor test case, when handling stateless valid transaction
 */
TEST(TransactionProcessorTest,
     TransactionProcessorWhereInvokeValidTransaction) {

  OsMock os;

  StatelessValidationMock validation;
  EXPECT_CALL(validation, validate(_)).WillRepeatedly(Return(true));

  iroha::torii::TransactionProcessorImpl tp(os, validation);
  model::Transaction tx;
  model::Client client;
  //ToDo add check ordering service's test
  tp.handle(client, tx);
}

/**
 * Transaction processor test case, when handling invalid transaction
 */
TEST(TransactionProcessorTest,
     TransactionProcessorWhereInvokeInvalidTransaction) {

  OsMock os;

  StatelessValidationMock validation;
  EXPECT_CALL(validation, validate(_)).WillRepeatedly(Return(false));

  iroha::torii::TransactionProcessorImpl tp(os, validation);
  model::Transaction tx;
  model::Client client;
  //ToDo add check ordering service's test
  tp.handle(client, tx);
}
