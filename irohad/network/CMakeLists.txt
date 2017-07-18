#add_library(networking
#    impl/network_api.cpp
#    peer_communication_stub.cpp
#    )
#
#target_link_libraries(networking PUBLIC
#    rxcpp
#    model
#    ametsuchi
#    )


add_library(server
        server.cpp
        server.hpp
        )

target_link_libraries(server
        uvw
        consensus_services
        )