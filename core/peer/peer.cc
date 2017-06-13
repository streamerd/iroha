#include "peer/api/client-connection/grpcServerRunner.h"

int main(int argc, char **argv) {

  grpc_connection::GrpcServerRunner serverRunner("0.0.0.0", 50051);
  serverRunner.run();

  return 0;
}
