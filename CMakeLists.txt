cmake_minimum_required(VERSION 3.10.0)
project(lgf VERSION 0.1.0 LANGUAGES C CXX)

add_library(lgf 
src/window_linux.cpp
src/window.cpp
src/files.cpp
src/quad.cpp
src/widgets/panel.cpp
)

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

target_include_directories(lgf PUBLIC include ${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ../glad/include ../glm)
target_link_libraries(lgf PRIVATE ${X11_LIBRARIES} GL glad glm)