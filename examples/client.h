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

#ifndef IROHA_CLIENT_H
#define IROHA_CLIENT_H

#include <api.pb.h>
#include <api_service.pb.h>
#include <api_service.grpc.pb.h>
#include <grpc++/grpc++.h>
#include <grpc/support/log.h>
#include <string>


namespace iroha {

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

class RandomClient {
 public:
  explicit RandomClient(std::shared_ptr<Channel> channel)
      : stub_(iroha::api::Torii::NewStub(channel)) {}

  void SendRandomTx() {
    // Data we are sending to the server.
    api::Transaction request;


    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    // stub_->AsyncSayHello() performs the RPC call, returning an instance to
    // store in "call". Because we are using the asynchronous API, we need to
    // hold on to the "call" instance in order to get updates on the ongoing
    // RPC.
    call->response_reader =
        stub_->AsyncSendTransaction(&call->context, request, &cq_);


    // Request that, upon completion of the RPC, "reply" be updated with the
    // server's response; "status" with the indication of whether the operation
    // was successful. Tag the request with the memory address of the call
    // object.
    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
  }

  // Loop while listening for completed responses.
  // Prints out the response from the server.
  void AsyncCompleteRpc() {
    void *got_tag;
    bool ok = false;

    // Block until the next result is available in the completion queue "cq".
    while (cq_.Next(&got_tag, &ok)) {
      // The tag in this example is the memory location of the call object
      AsyncClientCall *call = static_cast<AsyncClientCall *>(got_tag);

      // Verify that the request was completed successfully. Note that "ok"
      // corresponds solely to the request for updates introduced by Finish().
      GPR_ASSERT(ok);

      if (call->status.ok()) {
        // RPC OK, we got response.
        api::TxStatus status = call->reply;
        std::cout << "Status: " << status.code() << "\n";
      } else {
        std::cout << "RPC failed" << std::endl;
      }

      // Once we're complete, deallocate the call object.
      delete call;
    }
  }

 private:
  // struct for keeping state and data information
  struct AsyncClientCall {
    // Container for the data we expect from the server.
    api::TxStatus reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // Storage for the status of the RPC upon completion.
    Status status;

    std::unique_ptr<ClientAsyncResponseReader<api::TxStatus>> response_reader;
  };

  // Out of the passed in Channel comes the stub, stored here, our view of the
  // server's exposed services.
  std::unique_ptr<api::Torii::Stub> stub_;

  // The producer-consumer queue we use to communicate asynchronously with the
  // gRPC runtime.
  CompletionQueue cq_;
};
}

#endif  // IROHA_CLIENT_H
