cmake_minimum_required(VERSION 3.5)

project(RandomCallPlus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set path for QT
# set(CMAKE_PREFIX_PATH D:/Qt/6.5.0/msvc2019_64)

set(PYTHON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Python310/include")
# set(PYTHON_LIBS_LINK_310 "${CMAKE_CURRENT_SOURCE_DIR}/Python310/libs/python310.lib")
# set(PYTHON_LIBS_LINK_3 "${CMAKE_CURRENT_SOURCE_DIR}/Python310/libs/python3.lib")
set(PYTHON_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/Python310/libs")

set(QXLSX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Qxlsx/include")
# set(QXLSX_LIBS_LINK "${CMAKE_CURRENT_SOURCE_DIR}/QXlsx/libs/libQXlsxQt6.a")
set(QXLSX_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/QXlsx/libs")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)

include_directories(${PYTHON_INCLUDE_DIR})
include_directories(${QXLSX_INCLUDE_DIR})

link_directories(${PYTHON_LIBS})
link_directories(${QXLSX_LIBS})

set(PROJECT_SOURCES
    handleword.py
    main.cpp
    cmainwindow.cpp
    cmainwindow.h
    cmainwindow.ui
    config.h
    cpythreadstatelock.h cpythreadstatelock.cpp
    ccreateclasswindow.h ccreateclasswindow.cpp ccreateclasswindow.ui
    cdeleteclasswindow.h cdeleteclasswindow.cpp cdeleteclasswindow.ui
    cchangeclasswindow.h cchangeclasswindow.cpp cchangeclasswindow.ui
    caddstudentwindow.h caddstudentwindow.cpp caddstudentwindow.ui
    cdeletestudentwindow.h cdeletestudentwindow.cpp cdeletestudentwindow.ui
    cshowotheronewindow.h cshowotheronewindow.cpp cshowotheronewindow.ui
    cstatisticswindow.h cstatisticswindow.cpp cstatisticswindow.ui
    ccreateprizewindow.h ccreateprizewindow.cpp ccreateprizewindow.ui
    cstudentdatalist.h cstudentdatalist.cpp cstudentdatalist.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RandomCallPlus
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RandomCallPlus APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RandomCallPlus SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RandomCallPlus
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(RandomCallPlus PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RandomCallPlus PRIVATE Qt6::Sql)

# target_link_libraries(RandomCallPlus PRIVATE ${PYTHON_LIBS_LINK_310})
# target_link_libraries(RandomCallPlus PRIVATE ${PYTHON_LIBS_LINK_3})
target_link_libraries(RandomCallPlus PRIVATE python3.lib)
target_link_libraries(RandomCallPlus PRIVATE python310.lib)
target_link_libraries(RandomCallPlus PRIVATE python3_d.lib)
target_link_libraries(RandomCallPlus PRIVATE python310_d.lib)
# target_link_libraries(RandomCallPlus PRIVATE ${QXLSX_LIBS_LINK})
target_link_libraries(RandomCallPlus PRIVATE libQXlsxQt6.a)

target_link_libraries(RandomCallPlus PRIVATE QXlsxQt6.lib)
target_link_libraries(RandomCallPlus PRIVATE QXlsxQt6_d.lib)
target_link_libraries(RandomCallPlus PRIVATE QXlsxQt6_rd.lib)

set_target_properties(RandomCallPlus PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS RandomCallPlus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RandomCallPlus)
endif()
