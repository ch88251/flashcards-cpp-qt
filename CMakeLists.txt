cmake_minimum_required(VERSION 3.21)

project(flashcards
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_standard_project_setup()

qt_add_executable(flashcards
    src/main.cpp
    src/gui/MainWindow.cpp
    src/db/DatabaseManager.cpp
    src/repositories/FlashcardRepository.cpp
    src/repositories/DeckRepository.cpp
)

target_include_directories(flashcards
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(flashcards
    PRIVATE
        Qt6::Widgets
        Qt6::Sql
)
