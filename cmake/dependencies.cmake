include(ExternalProject)
set(EP_PREFIX "../external")
set_directory_properties(PROPERTIES
        EP_PREFIX ${EP_PREFIX}
        )



##############################
#         rapidjson          #
##############################
find_package(RapidJSON REQUIRED)
if (RapidJSON_FOUND)
    include_directories(${RapidJSON_INCLUDE_DIRS})
    link_directories(${RapidJSON_LIBRARIES})
else()
    message(FATAL_ERROR "No rapidjson found")
endif()



#########################
#         grpc          #
#########################
find_package(grpc REQUIRED)
if (grpc_FOUND)
    message(STATUS "grpc found")
    include_directories(${grpcpp_INCLUDE_DIR})
    link_directories(${grpcpp_LIB})
    link_directories(${gpr_LIB})
else()
    message(FATAL_ERROR "No grpc found")
endif()



#######################
#         uv          #
#######################
find_package(uv REQUIRED)
if (uv_FOUND)
    include_directories(${uv_INCLUDE_DIRS})
    link_directories(${uv_LIBRARIES})
else()
    message(FATAL_ERROR "No libuv found")
endif()



###########################
#         keccak          #
###########################
find_package(LibXslt QUIET)
if (NOT LIBXSLT_XSLTPROC_EXECUTABLE)
    message(FATAL_ERROR "xsltproc not found")
endif ()

ExternalProject_Add(gvanas_keccak
        GIT_REPOSITORY    "https://github.com/gvanas/KeccakCodePackage.git"
        BUILD_IN_SOURCE   1
        BUILD_COMMAND     bash -c "CFLAGS='-fPIC -DKeccakP200_excluded -DKeccakP400_excluded -DKeccakP800_excluded'\
    $(MAKE) CC='${CMAKE_C_COMPILER}' generic64/libkeccak.a"
        CONFIGURE_COMMAND "" # remove configure step
        INSTALL_COMMAND   "" # remove install step
        TEST_COMMAND      "" # remove test step
        UPDATE_COMMAND    "" # remove update step
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
        GIT_REPOSITORY    "https://github.com/MizukiSonoko/ed25519.git"
        BUILD_IN_SOURCE   1
        BUILD_COMMAND     $(MAKE)
        CONFIGURE_COMMAND "" # remove configure step
        INSTALL_COMMAND   "" # remove install step
        TEST_COMMAND      "" # remove test step
        UPDATE_COMMAND    "" # remove update step
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
# testing is an option. Look at the main CMakeLists.txt for details.
if(TESTING)
    ExternalProject_Add(google_test
            GIT_REPOSITORY    "https://github.com/google/googletest.git"
            CMAKE_ARGS        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -Dgtest_force_shared_crt=ON
            -Dgtest_disable_pthreads=OFF
            -DBUILD_GTEST=ON
            -DBUILD_GMOCK=OFF
            INSTALL_COMMAND   "" # remove install step
            UPDATE_COMMAND    "" # remove update step
            TEST_COMMAND      "" # remove test step
            )
    ExternalProject_Get_Property(google_test source_dir binary_dir)
    set(gtest_SOURCE_DIR ${source_dir})
    set(gtest_BINARY_DIR ${binary_dir})

    add_library(gtest STATIC IMPORTED)
    file(MAKE_DIRECTORY ${gtest_SOURCE_DIR}/googletest/include)

    set_target_properties(gtest
            PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/googletest/include
            IMPORTED_LINK_INTERFACE_LIBRARIES "pthread;${gtest_BINARY_DIR}/googletest/libgtest_main.a"
            IMPORTED_LOCATION ${gtest_BINARY_DIR}/googletest/libgtest.a
            )
    add_dependencies(gtest google_test)
endif()



#############################
#         speedlog          #
#############################
ExternalProject_Add(gabime_spdlog
        GIT_REPOSITORY    "https://github.com/gabime/spdlog.git"
        GIT_TAG           "v0.13.0"
        CONFIGURE_COMMAND "" # remove configure step
        BUILD_COMMAND     "" # remove build step
        INSTALL_COMMAND   "" # remove install step
        TEST_COMMAND      "" # remove test step
        UPDATE_COMMAND    "" # remove update step
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
        GIT_REPOSITORY    "https://github.com/martinmoene/optional-lite"
        GIT_TAG           "v2.0.0"
        CONFIGURE_COMMAND "" # remove configure step
        BUILD_COMMAND     "" # remove build step
        INSTALL_COMMAND   "" # remove install step
        TEST_COMMAND      "" # remove test step
        UPDATE_COMMAND    "" # remove update step
        )
ExternalProject_Get_Property(martinmoene_optional source_dir)
set(optional_INCLUDE_DIRS ${source_dir}/include/nonstd)
file(MAKE_DIRECTORY ${optional_INCLUDE_DIRS})

add_library(optional INTERFACE IMPORTED)
set_target_properties(optional PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${optional_INCLUDE_DIRS}
        )

add_dependencies(optional martinmoene_optional)
