add_library(messengerInterfaces INTERFACE)

# Header‑only library exposes its own include dir
target_include_directories(messengerInterfaces INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>)        

# Install headers verbatim under <prefix>/include/messengerInterfaces/
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/messengerInterfaces
        FILES_MATCHING PATTERN "*.hpp")

# Add to the main export set so downstream `find_package(messenger)` pulls it in
install(TARGETS messengerInterfaces EXPORT messengerTargets)