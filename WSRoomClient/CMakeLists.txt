cmake_minimum_required(VERSION 3.19)
project(WSRoomClient LANGUAGES CXX)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets WebSockets)

qt_standard_project_setup()

qt_add_executable(WSRoomClient
    WIN32 MACOSX_BUNDLE
    main.cpp
    chatwindow.cpp
    chatwindow.h
)

target_link_libraries(WSRoomClient
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::WebSockets
)

install(TARGETS WSRoomClient
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WSRoomClient
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
