cmake_minimum_required(VERSION 3.18)
project(3DParticleRenderer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# glad
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        v0.1.36
)
FetchContent_MakeAvailable(glad)

# glm
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Dear ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.89.8
)
FetchContent_MakeAvailable(imgui)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_FILES
  src/*.cpp
  src/*/*.cpp
)


add_executable(particle_renderer ${SRC_FILES} src/main.cpp)

# Project headers
target_include_directories(particle_renderer PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${glad_SOURCE_DIR}/include
)

target_link_libraries(particle_renderer PRIVATE glfw glad glm OpenGL::GL)

# imgui target
set(IMGUI_SOURCES
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends ${glad_SOURCE_DIR}/include)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL glad)

target_link_libraries(particle_renderer PRIVATE imgui)

set_target_properties(particle_renderer PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
