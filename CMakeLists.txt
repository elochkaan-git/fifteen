cmake_minimum_required(VERSION 3.16)

project(fifteen VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Curses REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

add_library(libfifteen STATIC
    board_factory.h
    board.h
    cell_factory.h
    cell.h
    scoreboard.h
)

qt_add_executable(appfifteen
    main.cpp
)

qt_add_qml_module(appfifteen
    URI fifteen
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        board_controller.h
        QML_FILES MainMenu.qml
        QML_FILES Game.qml
        QML_FILES Score.qml
        SOURCES score_model.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfifteen PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfifteen
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appfifteen
    PRIVATE Qt6::Quick libfifteen
)

add_executable(cfifteen cli.cpp
    scoreboard.h)
target_link_libraries(cfifteen
    PRIVATE libfifteen ${CURSES_LIBRARIES}
)
target_include_directories(cfifteen
    PRIVATE ${CURSES_INCLUDE_DIRS}
)

include(GNUInstallDirs)
install(TARGETS appfifteen
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS cfifteen
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
