cmake_minimum_required(VERSION 3.22)
project(MessengerApp LANGUAGES CXX)

# ── Global options ────────────────────────────────────────────────────────────
option(BUILD_GUI    "Build the Qt GUI front-end"           ON)
option(BUILD_TESTS  "Build unit tests"                     ON)
option(BUILD_SHARED_LIBS "Build shared libs instead of static" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -g -O1)
    # add_compile_options(-fsanitize=thread)
    # add_link_options(-fsanitize=thread)
endif()


# ── Testing ------------------------------------------------------------------
if (BUILD_TESTS)
    include(CTest)
    enable_testing()
endif()

# ── External deps ------------------------------------------------------------
find_package(OpenSSL REQUIRED)
find_package(Boost       REQUIRED COMPONENTS system thread)   # <── NEW
if (BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
endif()

# ── Sub-projects -------------------------------------------------------------
add_subdirectory(messengerInterfaces)
add_subdirectory(libmessenger)     # tworzy bibliotekę „messenger"

if (BUILD_GUI)
    add_subdirectory(gui)          # <-- folder z kodem Qt
endif()
