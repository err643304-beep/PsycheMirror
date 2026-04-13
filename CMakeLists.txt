cmake_minimum_required(VERSION 3.16)
project(pywgl_tabbed LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
    set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets)
else()
    set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets)
endif()

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(IMGUI_DIR "${THIRD_PARTY_DIR}/imgui")
set(GLAD_DIR "${THIRD_PARTY_DIR}/glad")

set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
    "${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp"
)

if(EXISTS "${GLAD_DIR}/src/glad.c")
    set(GLAD_SOURCES "${GLAD_DIR}/src/glad.c")
elseif(EXISTS "${GLAD_DIR}/src/gl.c")
    set(GLAD_SOURCES "${GLAD_DIR}/src/gl.c")
else()
    message(FATAL_ERROR "No Glad source file found")
endif()

add_executable(pywgl_tabbed
    src/main_simple.cpp
    src/navbar.cpp
    src/icon_renderer.cpp
    src/home_tab.cpp
    src/editor_tab.cpp
    src/viewer_tab.cpp
    src/settings_tab.cpp
    src/ics_tab.cpp
    src/csi_tab.cpp
    src/lsi_tab.cpp
    src/eikos_tab.cpp
    src/iks_tab.cpp
    src/data_bridge.cpp
    ${IMGUI_SOURCES}
    ${GLAD_SOURCES}
)

target_include_directories(pywgl_tabbed PRIVATE
    "${GLAD_DIR}/include"
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
    "${CMAKE_SOURCE_DIR}/src"
    ${Python3_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
)

target_link_libraries(pywgl_tabbed PRIVATE
    ${Python3_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${QT_LIBRARIES}
    OpenGL::GL
    dl
    pthread
)

target_compile_options(pywgl_tabbed PRIVATE
    -Wall -Wextra
)
