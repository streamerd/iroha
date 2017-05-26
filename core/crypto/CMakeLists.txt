add_library(crypto STATIC
        base64.h
        hash.h
        keypair.h
        crypto.cc
        )

target_link_libraries(crypto PUBLIC
        keccak
        ed25519
        )

