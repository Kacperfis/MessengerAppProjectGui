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
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── External deps ------------------------------------------------------------
find_package(OpenSSL REQUIRED)

# ── Fetch Boost if not supplied via system packages ─────────────────────────
include(FetchContent)
set(BOOST_ENABLE_CMAKE ON)
set(BOOST_INCLUDE_LIBRARIES thread system)
FetchContent_Declare(
    boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.gz
)
FetchContent_MakeAvailable(boost)
find_package(Boost CONFIG REQUIRED COMPONENTS system thread)
if (BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
endif()

# ── Sub-projects -------------------------------------------------------------
add_subdirectory(messengerInterfaces)
add_subdirectory(libmessenger)     # tworzy bibliotekę „messenger”

if (BUILD_GUI)
    add_subdirectory(gui)          # <-- folder z kodem Qt
endif()

# ── Testing ------------------------------------------------------------------
include(CTest)
if (BUILD_TESTS)
    enable_testing()
endif()
