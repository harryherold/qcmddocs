cmake_minimum_required(VERSION 3.5)

project(qcmddocs LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(QMARKDOWN_EDIT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qmarkdowntextedit)

set(QMARKDOWN_EDIT_SRC
    ${QMARKDOWN_EDIT_PATH}/qmarkdowntextedit.cpp
    ${QMARKDOWN_EDIT_PATH}/qownlanguagedata.cpp
    ${QMARKDOWN_EDIT_PATH}/qplaintexteditsearchwidget.ui
    ${QMARKDOWN_EDIT_PATH}/qplaintexteditsearchwidget.cpp
    ${QMARKDOWN_EDIT_PATH}/markdownhighlighter.cpp)

set(PROJECT_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/header_iterator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/mainwindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/note_collection.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/note_document.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/note_tree.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/tree_model.hpp)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/note_document.cpp        
    ${CMAKE_CURRENT_SOURCE_DIR}/src/note_collection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tree_model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/header_iterator.cpp
    ${QMARKDOWN_EDIT_SRC})

qt5_wrap_ui(${QMARKDOWN_EDIT_PATH}/ui_qplaintexteditsearchwidget.h
            ${QMARKDOWN_EDIT_PATH}/qplaintexteditsearchwidget.ui)

add_executable(qcmddocs
    ${PROJECT_HEADER}
    ${PROJECT_SOURCES}
    ${QMARKDOWN_EDIT_SRC}
    qcmddocs.qrc)

target_link_libraries(qcmddocs PRIVATE Qt5::Widgets)
target_include_directories(qcmddocs PRIVATE ${QMARKDOWN_EDIT_PATH})
target_include_directories(qcmddocs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

include(CTest)
add_subdirectory(tests)
