add_library(peer_state
        leader.cpp
        leader.hpp
        member.cpp
        member.hpp
        proxy_tail.cpp
        proxy_tail.hpp
        validator.cpp
        validator.hpp
        )

target_link_libraries(peer_state
        spdlog
        consensus_grpc
        peer_service
        uvw
        )