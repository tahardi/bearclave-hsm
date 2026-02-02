cmake_minimum_required(VERSION 4.1)
project(bearclave-hsm C)

set(CMAKE_C_STANDARD 11)

# Need this for clang-tidy to work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include/)
include_directories(third_party/)

# Define STRICT_WARNINGS variable that defaults to ON and
# adds strict compile flags. If you want to turn these off,
# you can create a preset the sets "STRICT_WARNINGS": "OFF"
option(STRICT_WARNINGS "Enable strict compiler warnings" ON)
if(STRICT_WARNINGS AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Werror")
endif()

set(PKCS11_SOURCES
        src/hsm.c
        src/pkcs11.c
        src/pkcs11_decrypt.c
        src/pkcs11_digest.c
        src/pkcs11_dual.c
        src/pkcs11_encrypt.c
        src/pkcs11_general.c
        src/pkcs11_key.c
        src/pkcs11_object.c
        src/pkcs11_parallel.c
        src/pkcs11_random.c
        src/pkcs11_session.c
        src/pkcs11_sign.c
        src/pkcs11_slot_token.c
        src/pkcs11_verify.c
)

add_library(bearclave-hsm ${PKCS11_SOURCES})

target_include_directories(bearclave-hsm
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

add_executable(test_bearclave-hsm tests/test_pkcs11.c)

target_link_libraries(test_bearclave-hsm PRIVATE bearclave-hsm)

# Define a variable for turning on sanitizers. Default to off.
option(USE_SANITIZER "Enable sanitizers (Address, Undefined, AddressUndefined, Memory, Thread)" OFF)

if(USE_SANITIZER)
    if(USE_SANITIZER STREQUAL "AddressUndefined")
        add_compile_options("-fsanitize=address,undefined" "-fno-omit-frame-pointer" "-g")
        add_link_options("-fsanitize=address,undefined")
        message(STATUS "Sanitizers enabled: Address, Undefined")
    elseif(USE_SANITIZER STREQUAL "Memory")
        add_compile_options("-fsanitize=memory" "-fno-omit-frame-pointer" "-g")
        add_link_options("-fsanitize=memory")
        message(STATUS "Sanitizers enabled: Memory")
    elseif(USE_SANITIZER STREQUAL "Thread")
        add_compile_options("-fsanitize=thread" "-g")
        add_link_options("-fsanitize=thread")
        message(STATUS "Sanitizers enabled: Thread")
    endif()
endif()
