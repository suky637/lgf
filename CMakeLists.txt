cmake_minimum_required(VERSION 3.10.0)
project(lgf VERSION 0.1.0 LANGUAGES C CXX)

add_library(lgf src/window_linux.cpp
src/files.cpp
src/quad.cpp
src/events.cpp
src/text.cpp
src/image.cpp
src/widgets/panel.cpp
src/widgets/layout.cpp
src/widgets/label.cpp
src/widgets/image_panel.cpp
src/widgets/button.cpp
src/widgets/widget_base.cpp
src/widgets/placeholder.cpp
src/widgets/textbox.cpp
)

find_package(X11 REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon xkbcommon-x11 x11 x11-xcb)

set(BUILD_SHARED_LIBS OFF)
find_package(PNG REQUIRED STATIC)

target_include_directories(lgf PUBLIC include ${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${XKBCOMMON_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} ../glad/include ../glm ../freetype-2.10.0/include)
target_link_libraries(lgf PRIVATE ${XKBCOMMON_LIBRARIES} ${X11_LIBRARIES} ${PNG_LIBRARIES} GL glad glm freetype)

target_compile_options(lgf PRIVATE ${XKBCOMMON_CFLAGS_OTHER})