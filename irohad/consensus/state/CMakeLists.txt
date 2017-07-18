add_library(peer_state
    leader.cpp
    member.cpp
    proxy_tail.cpp
    validator.cpp
    )

target_link_libraries(peer_state
    spdlog
    peer_service
    uvw
    )