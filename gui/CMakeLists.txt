if (NOT BUILD_GUI)
    return()
endif()

# ─── włącz automaty Qt (działa w Qt5 i Qt6) ───────────────────────────────────
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Client GUI

add_executable(ClientGui
    mainClientGui.cpp
    ClientGui.cpp ClientGui.hpp
)

target_link_libraries(ClientGui
    PRIVATE
        messenger
        Qt6::Widgets
        Boost::system Boost::thread
)

# Server GUI

add_executable(ServerGui
    mainServerGui.cpp
    ServerGui.cpp ServerGui.hpp
)

target_link_libraries(ServerGui
    PRIVATE
        messenger
        Qt6::Widgets
        Boost::system Boost::thread
)
