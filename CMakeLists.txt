cmake_minimum_required(VERSION 3.31)
project(homework2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/form)
set(CMAKE_PREFIX_PATH "C:/Qt/6.5.3/msvc2019_64")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
FILE(GLOB_RECURSE RC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/*.rc)
FILE(GLOB_RECURSE QRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/*.qrc)
FILE(GLOB_RECURSE UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/form/*.ui)
FILE(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SOURCE_FILES)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(homework2 WIN32 main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        form/mainwindow.ui
        src/gamewindow.cpp
        include/gamewindow.h
        form/gamewindow.ui
        src/random.cpp
        include/random.h
        src/legalcheck.cpp
        include/legalcheck.h
        resource/resource.qrc
        resource/logo.rc
)
ADD_EXECUTABLE(main.cpp ${SOURCE_FILES} ${RC_FILES} ${QRC_FILES} ${UI_FILES} ${HEADER_FILES})
target_link_libraries(homework2
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
