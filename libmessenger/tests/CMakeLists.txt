# ---------------------------------------------------------------------------
# libmessenger/tests/CMakeLists.txt
# ---------------------------------------------------------------------------

# 1.  Grab GoogleTest + GoogleMock (same repo) ------------------------------
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Avoid overriding parent compiler flags on MSVC
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)   # <- defines gtest, gmock, ...

enable_testing()

# 2.  Collect all *Tests.cpp files ------------------------------------------
file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/*Tests.cpp)

file(GLOB FIXTURE_SRC
     ${CMAKE_CURRENT_SOURCE_DIR}/common/commonFixture.cpp)

add_executable(libmessenger-ut ${TEST_SRC} ${FIXTURE_SRC})

# 3.  Include paths needed ONLY by the tests --------------------------------
target_include_directories(libmessenger-ut PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..        # libmessenger/include + messengerInterfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/../src    # access to internal headers (Login/…, Database/…)
)

# 4.  Link against the library under test + gmock_main ----------------------
target_link_libraries(libmessenger-ut
    PRIVATE
        messenger       # the backend
        gmock_main      # brings in gmock + gtest
)

# 5.  Register with CTest ----------------------------------------------------
add_test(NAME libmessenger_all COMMAND libmessenger-ut)

# 6.  Make the global 'ut' convenience target build these tests -------------
if (TARGET ut)
    add_dependencies(ut libmessenger-ut)
endif()