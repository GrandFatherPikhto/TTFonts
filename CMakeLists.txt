cmake_minimum_required(VERSION 3.16)

project(TTFonts VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Проверяем компилятор
if(MSVC)
    message(STATUS "Using MSVC compiler")
    # Пути к FreeType
    set(FREETYPE_INCLUDE_DIR "./libraries/freetype/msvc/include/freetype/")
    set(FREETYPE_LIBRARY_DIR "./libraries/freetype/msvc/")
elseif(MINGW)
    message(STATUS "Using MinGW compiler")
    set(FREETYPE_INCLUDE_DIR "./libraries/freetype/mingw64/include/freetype2/")
    set(FREETYPE_LIBRARY_DIR "./libraries/freetype/mingw64/bin/")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES TTFonts_ru_RU.ts)

set(PROJECT_SOURCES
        src/main/main.cpp
        src/main/include/mainwindow.h
        src/main/mainwindow.cpp
        # UI
        src/main/mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

    qt_add_executable(TTFonts
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        # FontCharactersModel
        src/models/include/fontcharactersmodel.h
        src/models/fontcharactersmodel.cpp
        # FontMetadataModel
        src/models/include/fontmetadatamodel.h 
        src/models/fontmetadatamodel.cpp
        # Unicode Metadata Manager
        src/models/include/unicodemetadatamanager.h
        src/models/unicodemetadatamanager.cpp
        src/main/include/common.h
        src/widgets/include/glyphwidget.h
        src/widgets/glyphwidget.cpp
    )

    target_include_directories(TTFonts
        PRIVATE
        ${FREETYPE_INCLUDE_DIR}
        src/main/include
        src/models/include
        src/widgets/include
    )
    
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TTFonts APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(TTFonts SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TTFonts
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_directories(TTFonts PRIVATE ${FREETYPE_LIBRARY_DIR})

target_link_libraries(TTFonts PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TTFonts PRIVATE Qt6::Core)
target_link_libraries(TTFonts PRIVATE Qt6::Widgets)
target_link_libraries(TTFonts PRIVATE freetype)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TTFonts)
endif()
set_target_properties(TTFonts PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TTFonts
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TTFonts)
endif()
