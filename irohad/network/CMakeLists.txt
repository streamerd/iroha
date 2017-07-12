add_library(networking
  impl/network_api.cpp
  peer_communication_stub.cpp
)

target_link_libraries(networking PUBLIC
  rxcpp
  model
  ametsuchi
)

add_library(block_publisher
  block_publisher_with_grpc.cpp
)

target_link_libraries(block_publisher PUBLIC
  model
)
