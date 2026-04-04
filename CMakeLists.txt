cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

project(SystemMonitor LANGUAGES CXX)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(SystemMonitor
    # WIN32 MACOSX_BUNDLE
    main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    include/json.hpp # https://github.com/nlohmann/json/releases
    src/threadpool.cpp
    include/threadpool.hpp
    include/container.h
    include/util.h
    src/util.cpp
    include/containerManager.h
    src/containerManager.cpp
    include/containerpage.h
    src/containerpage.cpp
    include/containerListPanel.h
    src/containerListPanel.cpp
)

qt_add_resources(SystemMonitor "resources"
    PREFIX "/"
    FILES
        style.qss
)
target_sources(SystemMonitor PRIVATE ${RESOURCES})

target_include_directories(SystemMonitor PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(SystemMonitor
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS SystemMonitor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SystemMonitor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
