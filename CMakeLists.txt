cmake_minimum_required(VERSION 4.1)
project(bearsm C)

set(CMAKE_C_STANDARD 11)

# Need this for clang-tidy to work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
include_directories(third_party/pkcs11)

# Define STRICT_WARNINGS variable that defaults to ON and
# adds strict compile flags. If you want to turn these off,
# you can create a preset the sets "STRICT_WARNINGS": "OFF"
option(STRICT_WARNINGS "Enable strict compiler warnings" ON)
if(STRICT_WARNINGS AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Werror")
endif()

add_library(bearsm src/pkcs11.c)

target_include_directories(bearsm
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/pkcs11
)

add_executable(test_bearsm tests/test_pkcs11.c)

target_link_libraries(test_bearsm PRIVATE bearsm)


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
