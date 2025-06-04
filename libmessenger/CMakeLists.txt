# Collect sources (recurses into sub‑folders)
file(GLOB_RECURSE LIB_SRC CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(messenger ${LIB_SRC})

# Public & install‑time headers
target_include_directories(messenger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# Dependencies
find_package(Threads REQUIRED)

target_link_libraries(messenger
    PUBLIC  messengerInterfaces          # header‑only, pulls include path
    PRIVATE OpenSSL::SSL Threads::Threads)

# Build flags & visibility
target_compile_features(messenger PUBLIC cxx_std_20)
if (WIN32 AND BUILD_SHARED_LIBS)
    target_compile_definitions(messenger PRIVATE MESSENGER_BUILD_DLL)
endif()

# ── Unit tests ---------------------------------------------------------------
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

# ── Install rules ------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS messenger EXPORT messengerTargets
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/messenger
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})