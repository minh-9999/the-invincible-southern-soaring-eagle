cmake_minimum_required(VERSION 3.16)

project(chess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Multimedia)

# List of translation source files (.ts)
set(
    TS_FILES
    translations/chess_vi.ts
    translations/chess_zh_CN.ts
    translations/chess_zh_TW.ts
    translations/chess_en.ts
    translations/chess_fr.ts
    translations/chess_pt.ts
    translations/chess_es.ts
    translations/chess_ru.ts
)

# Include all subfolders
include_directories(

    ${CMAKE_SOURCE_DIR}/dialogs
)

set(
    PROJECT_SOURCES

    menu_utils.h
    menu_utils.cpp

    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ChessBoardWidget.h
    Piece.h
    ChessBoardWidget.cpp
    PieceLoader.h
    PieceLoader.cpp
    BackgroundWidget.h
    BackgroundWidget.cpp
    resources.qrc
    player.h

    menu_file.h
    menu_file.cpp

    menu_position.h
    menu_position.cpp

    menu_move.h
    menu_move.cpp

    move_data.h
    game_state.h
    game_state.cpp

    menu_engine.h
    menu_engine.cpp

    menu_book.h
    menu_book.cpp

    menu_connection.h
    menu_connection.cpp

    menu_settings.h
    menu_settings.cpp

    menu_view.h
    menu_view.cpp

    menu_help.h
    menu_help.cpp

    menu_language.h
    menu_language.cpp

    CustomToolBar.h
    CustomToolBar.cpp

    CustomToolBar_2.h
    CustomToolBar_2.cpp

    clockwidget.h
    clockwidget.cpp

    # dialogs sections
    dialogs/engine_settings_dialog.h
    dialogs/engine_settings_dialog.cpp

    # app_icon.rc

    ${TS_FILES}
)

# Optional Windows icon
if(WIN32)
    list(APPEND PROJECT_SOURCES app_icon.rc)
    set(WIN32_EXECUTABLE_FLAG WIN32)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

  # if(MINGW)
  #     set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_BINARY_DIR}/app_icon_res.o")
  #     add_custom_command(
  #         OUTPUT ${APP_ICON_RESOURCE_WINDOWS}
  #         COMMAND ${CMAKE_RC_COMPILER}
  #             -I "${CMAKE_CURRENT_SOURCE_DIR}"
  #             -O coff
  #             -o "${APP_ICON_RESOURCE_WINDOWS}"
  #             "${CMAKE_CURRENT_SOURCE_DIR}/app_icon.rc"
  #         DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/app_icon.rc"
  #     )
  #     list(APPEND PROJECT_SOURCES ${APP_ICON_RESOURCE_WINDOWS})
  # endif()

  qt_add_executable(
      chess
      MANUAL_FINALIZATION
      ${PROJECT_SOURCES}


  )


# Define target properties for Android with Qt 6 as:
#    set_property(TARGET chess APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

  # qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

  qt_add_translations(chess
      TS_FILES ${TS_FILES}
      QM_FILES_OUTPUT_VARIABLE QM_FILES
  )


else()
    if(ANDROID)
        add_library(chess SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(chess
          ${WIN32_EXECUTABLE_FLAG}
          ${PROJECT_SOURCES})
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(
  chess
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
  PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.chess)
endif()

set_target_properties(chess PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS chess
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt6_add_resources(TRANSLATION_RESOURCES ${RESOURCE_FILES})
target_sources(chess PRIVATE ${TRANSLATION_RESOURCES})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chess)
endif()
