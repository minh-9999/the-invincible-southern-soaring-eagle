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

# Source files organized by module
set(
    CHESS_SOURCES
    src/chess/ChessBoardWidget.cpp
    src/chess/ChessBoardWidget.h
    src/chess/Piece.h
    src/chess/PieceLoader.cpp
    src/chess/PieceLoader.h
    src/chess/MoveGenerator.cpp
    src/chess/MoveGenerator.h
    src/chess/BackgroundWidget.cpp
    src/chess/BackgroundWidget.h
    src/chess/clockwidget.cpp
    src/chess/clockwidget.h
    src/chess/CustomToolBar.cpp
    src/chess/CustomToolBar.h
    src/chess/CustomToolBar_2.cpp
    src/chess/CustomToolBar_2.h
)

set(
    MENUS_SOURCES
    src/menus/menu_utils.cpp
    src/menus/menu_utils.h
    src/menus/menu_file.cpp
    src/menus/menu_file.h
    src/menus/menu_position.cpp
    src/menus/menu_position.h
    src/menus/menu_move.cpp
    src/menus/menu_move.h
    src/menus/menu_engine.cpp
    src/menus/menu_engine.h
    src/menus/menu_book.cpp
    src/menus/menu_book.h
    src/menus/menu_connection.cpp
    src/menus/menu_connection.h
    src/menus/menu_settings.cpp
    src/menus/menu_settings.h
    src/menus/menu_view.cpp
    src/menus/menu_view.h
    src/menus/menu_help.cpp
    src/menus/menu_help.h
    src/menus/menu_language.cpp
    src/menus/menu_language.h
)

set(
    UI_SOURCES
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
)

set(
    GAME_SOURCES
    src/game/game_state.cpp
    src/game/game_state.h
    src/game/move_data.h
    src/game/player.h
)

set(
    DIALOGS_SOURCES
    src/dialogs/engine_settings_dialog.cpp
    src/dialogs/engine_settings_dialog.h
    src/dialogs/book_settings_dialog.cpp
    src/dialogs/book_settings_dialog.h
)

set(
    UTILS_SOURCES
    src/utils/logger.h
    src/utils/logger.cpp
)

set(
    UIS
    src/ui/mainwindow.ui
    # src/dialogs/EngineSettingsDialog.ui
)

set(
    PROJECT_SOURCES
    src/main.cpp
    ${CHESS_SOURCES}
    ${MENUS_SOURCES}
    ${UI_SOURCES}
    ${GAME_SOURCES}
    ${DIALOGS_SOURCES}
    ${UTILS_SOURCES}
    ${UIS}
    resources.qrc
    ${TS_FILES}
)

# Optional Windows icon
if(WIN32)
    list(APPEND PROJECT_SOURCES app_icon.rc)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

  qt_add_executable(
      chess
      MANUAL_FINALIZATION
      ${PROJECT_SOURCES}
  )

  qt_add_translations(chess
      TS_FILES ${TS_FILES}
      QM_FILES_OUTPUT_VARIABLE QM_FILES
  )

else()
    if(ANDROID)
        add_library(chess SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(chess
          ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(chess PRIVATE
    "${CMAKE_BINARY_DIR}/chess_autogen/include"
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/chess
    ${CMAKE_SOURCE_DIR}/src/menus
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/game
    ${CMAKE_SOURCE_DIR}/src/dialogs
    ${CMAKE_SOURCE_DIR}/src/utils
)

target_link_libraries(
  chess
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
  PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
)

add_compile_definitions(QT_MESSAGELOGCONTEXT)

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
    WIN32_EXECUTABLE FALSE
)

include(GNUInstallDirs)

install(TARGETS chess
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chess)
endif()