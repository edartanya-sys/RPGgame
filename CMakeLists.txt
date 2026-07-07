cmake_minimum_required(VERSION 3.15)
project(RPGgame)

set(CMAKE_CXX_STANDARD 20)

set(SFML_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/SFML/include")
set(SFML_LIB "${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib")

add_executable(RPGgame
        src/main.cpp
        src/Game.cpp
        src/Entity.cpp
        src/AssetManager.cpp
        src/MathFunctions.cpp
        src/TileManager.cpp
)

target_include_directories(RPGgame PRIVATE
        ${SFML_INCLUDE}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# КРИТИЧЕСКИ ВАЖНО: отключает __declspec(dllimport) в заголовках SFML
target_compile_definitions(RPGgame PRIVATE SFML_STATIC)

target_link_libraries(RPGgame PRIVATE
        ${SFML_LIB}/libsfml-graphics-s.a
        ${SFML_LIB}/libsfml-window-s.a
        ${SFML_LIB}/libsfml-system-s.a
        ${SFML_LIB}/libfreetype.a
        ${SFML_LIB}/libharfbuzz.a
        # Системные библиотеки Windows (обязательны для статики)
        opengl32
        winmm
        gdi32
)