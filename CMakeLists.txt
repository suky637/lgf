cmake_minimum_required(VERSION 3.10.0)
project(lgf VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(lgf
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
src/widgets/textbox.cpp)

if (LINUX)
target_sources(lgf PRIVATE src/window_linux.cpp)
endif()

if (WIN32)
target_sources(lgf PRIVATE src/window_win.cpp)
endif()
if (LINUX)
find_package(X11 REQUIRED)
find_package(PNG REQUIRED)
endif()
find_package(OpenGL REQUIRED)

if (LINUX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(XKBCOMMON REQUIRED xkbcommon xkbcommon-x11 x11 x11-xcb)

	set(BUILD_SHARED_LIBS OFF)
	find_package(PNG REQUIRED STATIC)
endif()

if (LINUX)
	target_include_directories(lgf PUBLIC ${X11_INCLUDE_DIR} ${XKBCOMMON_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
	target_link_libraries(lgf PRIVATE GL ${XKBCOMMON_LIBRARIES} ${X11_LIBRARIES} ${PNG_LIBRARIES})
endif()

target_include_directories(lgf PUBLIC include ${OPENGL_INCLUDE_DIR} ../glad/include ../glm ../freetype-2.10.0/include)
if (WIN32)
	target_include_directories(lgf PUBLIC ../zlib ../libpng-1.6.50)
	target_link_libraries(lgf PRIVATE opengl32 zlibstatic png_static )
ENDIF()
target_link_libraries(lgf PRIVATE glad glm freetype)

if (LINUX)
	target_compile_options(lgf PRIVATE ${XKBCOMMON_CFLAGS_OTHER})
endif()