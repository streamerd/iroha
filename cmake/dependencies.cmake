include(ExternalProject)
set(EP_PREFIX "../external")
set_directory_properties(PROPERTIES
    EP_PREFIX ${EP_PREFIX}
    )

# not required yet
##############################
#         rapidjson          #
##############################
#find_package(RapidJSON REQUIRED)
#if (RapidJSON_FOUND)
#    include_directories(${RapidJSON_INCLUDE_DIRS})
#    link_directories(${RapidJSON_LIBRARIES})
#else()
#    message(FATAL_ERROR "No rapidjson found")
#endif()

#########################
#         gRPC          #
#########################
find_package(GRPC)
if (NOT GRPC_FOUND)
  ExternalProject_Add(grpc_grpc
      GIT_REPOSITORY "https://github.com/grpc/grpc.git"
      GIT_TAG "v1.3.0"
      BUILD_IN_SOURCE 1
      BUILD_COMMAND $(MAKE)
      CONFIGURE_COMMAND "" # remove configure step
      INSTALL_COMMAND "" # remove install step
      TEST_COMMAND "" # remove test step
      UPDATE_COMMAND "" # remove update step
      )
  ExternalProject_Get_Property(grpc_grpc source_dir)
  set(grpc_INCLUDE_DIR ${source_dir}/include)
  set(grpc_LIBRARY ${source_dir}/libs/opt/libgrpc.so)
  set(grpc_grpc++_LIBRARY ${source_dir}/libs/opt/libgrpc++.so)
  set(grpc_grpc++_reflection_LIBRARY ${source_dir}/libs/opt/libgrpc++_reflection.so)
  set(grpc_CPP_PLUGIN ${source_dir})
  file(MAKE_DIRECTORY ${grpc_INCLUDE_DIR})
  add_custom_target(grpc_cpp_plugin DEPENDS grpc_grpc protoc)
else ()
  set(grpc_INCLUDE_DIR ${GRPC_INCLUDE_DIR})
  set(grpc_LIBRARY ${GRPC_LIBRARY})
  set(grpc_grpc++_LIBRARY ${GRPC_GRPC++_LIBRARY})
  set(grpc_grpc++_reflection_LIBRARY ${GRPC_GRPC++_REFLECTION_LIBRARY})
  set(grpc_CPP_PLUGIN ${GRPC_CPP_PLUGIN})
  add_custom_target(grpc_cpp_plugin DEPENDS protoc)
endif ()

# libgrpc
add_library(grpc SHARED IMPORTED)
set_target_properties(grpc PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${grpc_INCLUDE_DIR}
    IMPORTED_LOCATION ${grpc_LIBRARY}
    )

# libgrpc++
add_library(grpc++ SHARED IMPORTED)
set_target_properties(grpc++ PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${grpc_INCLUDE_DIR}
    IMPORTED_LOCATION ${grpc_grpc++_LIBRARY}
    )

# libgrpc++_reflection
add_library(grpc++_reflection SHARED IMPORTED)
set_target_properties(grpc++_reflection PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${grpc_INCLUDE_DIR}
    IMPORTED_LOCATION ${grpc_grpc++_reflection_LIBRARY}
    )

if (NOT GRPC_FOUND)
  add_dependencies(grpc grpc_grpc)
  add_dependencies(grpc++ grpc_grpc)
  add_dependencies(grpc++_reflection grpc_grpc)
endif ()

# right now there is no need in libuv, but might be useful in future
########################
##         uv          #
########################
#find_package(uv REQUIRED)
#if (uv_FOUND)
#    include_directories(${uv_INCLUDE_DIRS})
#    link_directories(${uv_LIBRARIES})
#else()
#    message(FATAL_ERROR "No libuv found")
#endif()

###########################
#         keccak          #
###########################
find_package(LibXslt QUIET)
if (NOT LIBXSLT_XSLTPROC_EXECUTABLE)
  message(FATAL_ERROR "xsltproc not found")
endif ()

ExternalProject_Add(gvanas_keccak
    GIT_REPOSITORY "https://github.com/gvanas/KeccakCodePackage.git"
    BUILD_IN_SOURCE 1
    BUILD_COMMAND bash -c "CFLAGS='-fPIC -DKeccakP200_excluded -DKeccakP400_excluded -DKeccakP800_excluded'\
    $(MAKE) CC='${CMAKE_C_COMPILER}' generic64/libkeccak.a"
    CONFIGURE_COMMAND "" # remove configure step
    INSTALL_COMMAND "" # remove install step
    TEST_COMMAND "" # remove test step
    UPDATE_COMMAND "" # remove update step
    )
ExternalProject_Get_Property(gvanas_keccak source_dir)
set(keccak_SOURCE_DIR "${source_dir}")

# Hash internals for keccak
add_library(keccak STATIC IMPORTED)
file(MAKE_DIRECTORY ${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a.headers)
set_target_properties(keccak PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a.headers"
    IMPORTED_LOCATION "${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a"
    )

add_dependencies(keccak gvanas_keccak)

############################
#         ed25519          #
############################
ExternalProject_Add(mizukisonoko_ed25519
    GIT_REPOSITORY "https://github.com/MizukiSonoko/ed25519.git"
    BUILD_IN_SOURCE 1
    BUILD_COMMAND $(MAKE)
    CONFIGURE_COMMAND "" # remove configure step
    INSTALL_COMMAND "" # remove install step
    TEST_COMMAND "" # remove test step
    UPDATE_COMMAND "" # remove update step
    )
ExternalProject_Get_Property(mizukisonoko_ed25519 source_dir)
set(ed25519_SOURCE_DIR "${source_dir}")

add_library(ed25519 STATIC IMPORTED)
file(MAKE_DIRECTORY ${ed25519_SOURCE_DIR}/src)
set_target_properties(ed25519 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${ed25519_SOURCE_DIR}/src
    IMPORTED_LOCATION ${ed25519_SOURCE_DIR}/lib/libed25519.a
    )
add_dependencies(ed25519 mizukisonoko_ed25519)

##########################
#         gtest          #
##########################
# util is an option. Look at the main CMakeLists.txt for details.
if (TESTING)
  ExternalProject_Add(google_test
      GIT_REPOSITORY "https://github.com/google/googletest.git"
      CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -Dgtest_force_shared_crt=ON
      -Dgtest_disable_pthreads=OFF
      -DBUILD_GTEST=ON
      -DBUILD_GMOCK=OFF
      INSTALL_COMMAND "" # remove install step
      UPDATE_COMMAND "" # remove update step
      TEST_COMMAND "" # remove test step
      )
  ExternalProject_Get_Property(google_test source_dir binary_dir)
  set(gtest_SOURCE_DIR ${source_dir})
  set(gtest_BINARY_DIR ${binary_dir})

  add_library(gtest STATIC IMPORTED)
  file(MAKE_DIRECTORY ${gtest_SOURCE_DIR}/googletest/include)

  set_target_properties(gtest
      PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/googletest/include
      INTERFACE_LINK_LIBRARIES "pthread;${gtest_BINARY_DIR}/googletest/libgtest_main.a"
      IMPORTED_LOCATION ${gtest_BINARY_DIR}/googletest/libgtest.a
      )
  add_dependencies(gtest google_test)
endif ()

#############################
#         speedlog          #
#############################
ExternalProject_Add(gabime_spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v0.13.0"
    CONFIGURE_COMMAND "" # remove configure step
    BUILD_COMMAND "" # remove build step
    INSTALL_COMMAND "" # remove install step
    TEST_COMMAND "" # remove test step
    UPDATE_COMMAND "" # remove update step
    )
ExternalProject_Get_Property(gabime_spdlog source_dir)
set(spdlog_INCLUDE_DIRS ${source_dir}/include)
file(MAKE_DIRECTORY ${spdlog_INCLUDE_DIRS})

add_library(spdlog INTERFACE IMPORTED)
set_target_properties(spdlog PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${spdlog_INCLUDE_DIRS}
    )

add_dependencies(spdlog gabime_spdlog)

#############################
#         optional          #
#############################
ExternalProject_Add(martinmoene_optional
    GIT_REPOSITORY "https://github.com/martinmoene/optional-lite"
    GIT_TAG "v2.0.0"
    CONFIGURE_COMMAND "" # remove configure step
    BUILD_COMMAND "" # remove build step
    INSTALL_COMMAND "" # remove install step
    TEST_COMMAND "" # remove test step
    UPDATE_COMMAND "" # remove update step
    )
ExternalProject_Get_Property(martinmoene_optional source_dir)
set(optional_INCLUDE_DIRS ${source_dir}/include)
file(MAKE_DIRECTORY ${optional_INCLUDE_DIRS})

add_library(optional INTERFACE IMPORTED)
set_target_properties(optional PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${optional_INCLUDE_DIRS}
    )

add_dependencies(optional martinmoene_optional)

########################
#         any          #
########################
ExternalProject_Add(martinmoene_any
    GIT_REPOSITORY "https://github.com/martinmoene/any-lite"
    GIT_TAG "v0.0.0"
    CONFIGURE_COMMAND "" # remove configure step
    BUILD_COMMAND "" # remove build step
    INSTALL_COMMAND "" # remove install step
    TEST_COMMAND "" # remove test step
    UPDATE_COMMAND "" # remove update step
    )
ExternalProject_Get_Property(martinmoene_any source_dir)
set(any_INCLUDE_DIRS ${source_dir}/include)
file(MAKE_DIRECTORY ${any_INCLUDE_DIRS})

add_library(any INTERFACE IMPORTED)
set_target_properties(any PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${any_INCLUDE_DIRS}
    )

add_dependencies(any martinmoene_any)

################################
#           protobuf           #
################################
find_package(Protobuf 3.3.0)
if (NOT PROTOBUF_FOUND OR NOT PROTOBUF_PROTOC_EXECUTABLE)
  ExternalProject_Add(google_protobuf
      URL https://github.com/google/protobuf/releases/download/v3.3.0/protobuf-cpp-3.3.0.tar.gz
      CONFIGURE_COMMAND ./configure
      BUILD_IN_SOURCE 1
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND "" # remove install step
      TEST_COMMAND "" # remove test step
      UPDATE_COMMAND "" # remove update step
      )
  ExternalProject_Get_Property(google_protobuf source_dir)
  set(protobuf_INCLUDE_DIRS ${source_dir}/src)
  set(protobuf_LIBRARIES ${source_dir}/src/.libs/libprotobuf.a)
  set(protoc_EXECUTABLE ${source_dir}/src/protoc)
  file(MAKE_DIRECTORY ${protobuf_INCLUDE_DIRS})
  add_custom_target(protoc DEPENDS google_protobuf)
else ()
  set(protobuf_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS})
  set(protobuf_LIBRARIES ${PROTOBUF_LIBRARY})
  set(protoc_EXECUTABLE ${PROTOBUF_PROTOC_EXECUTABLE})
  add_custom_target(protoc)
endif ()

add_library(protobuf STATIC IMPORTED)
set_target_properties(protobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${protobuf_INCLUDE_DIRS}
    IMPORTED_LOCATION ${protobuf_LIBRARIES}
    )

if (NOT PROTOBUF_FOUND OR NOT PROTOBUF_PROTOC_EXECUTABLE)
  add_dependencies(protobuf google_protobuf protoc)
endif ()

#########
#  天地  #
#########
ExternalProject_Add(hyperledger_iroha-ametsuchi
    GIT_REPOSITORY "https://github.com/hyperledger/iroha-ametsuchi"
    GIT_TAG "feature/without-grpc"
    CMAKE_ARGS -DTESTING=OFF
    INSTALL_COMMAND "" # remove install step
    TEST_COMMAND "" # remove test step
    UPDATE_COMMAND "" # remove update step
)
ExternalProject_Get_Property(hyperledger_iroha-ametsuchi source_dir binary_dir)
set(ametsuchi_INCLUDE_DIR ${source_dir}/include)
set(ametsuchi_LIBRARY ${binary_dir}/lib/libametsuchi.a)

add_library(ametsuchi STATIC IMPORTED)
file(MAKE_DIRECTORY ${ametsuchi_INCLUDE_DIR})

set_target_properties(ametsuchi PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${ametsuchi_INCLUDE_DIR}
    IMPORTED_LOCATION ${ametsuchi_LIBRARY}
    )
add_dependencies(ametsuchi hyperledger_iroha-ametsuchi)