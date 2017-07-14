/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <main/application.hpp>

Irohad::Irohad():
  context(new Context())
{}

void Irohad::run(){
  auto log = logger::Logger("Irohad");
  log.info("running");

  log.info("create server's keypair");
  // TODO replace with actual public private keys
  auto seed = iroha::create_seed("some passphrase");
  auto keypair = iroha::create_keypair(seed);
  log.info("publicKey: {}****, privateKey: {}****",
     keypair.pubkey.to_base64().substr(0,10),
     keypair.privkey.to_base64().substr(0,10)
  );
  iroha::model::ModelCryptoProviderImpl crypto_provider(keypair.privkey, keypair.pubkey);

  iroha::validation::StatelessValidatorImpl stateless_validator(crypto_provider);

  iroha::torii::TransactionProcessorStub tp(
    stateless_validator,
    crypto_provider
  );
}
