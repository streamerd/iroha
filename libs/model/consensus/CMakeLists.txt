add_library(consensus_messages
        messages.pb.cc
        messages.pb.h
        )

target_link_libraries(consensus_messages
        protobuf
        )