cmake_minimum_required(VERSION 3.20)
project(PixelEngine VERSION 0.1.0)

find_package(OpenGL REQUIRED)

if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()

if (APPLE) 
    list(APPEND glfw_LIBRARIES
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation")

    set(glfw_PKG_DEPS "")
    set(glfw_PKG_LIBS "-framework Cocoa -framework IOKit -framework CoreFoundation")

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
elseif (UNIX AND NOT APPLE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
elseif (WIN32)
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
   set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
   set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
   set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
endif()

# glfw
add_subdirectory("Libraries/GLFW/")
add_library(glfw3 SHARED IMPORTED)

# assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp build tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Assimp install" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Assimp install PDB" FORCE)

add_subdirectory("Libraries/assimp")

# freetype
add_subdirectory("Libraries/freetype")

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*")
file(GLOB_RECURSE SOURCES_GLAD CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/include/glad/*")

# OpenAl
add_subdirectory("Libraries/openal")

if (WIN32)
add_executable(PixelEngine
    WIN32
    "${SRC}" 
    "${SOURCES_GLAD}" 
)
elseif(APPLE)
add_executable(PixelEngine
    ${SRC}
    ${SOURCES_GLAD})
else() # Linux
add_executable(PixelEngine
    ${SRC}
    ${SOURCES_GLAD})
endif()

target_link_libraries(PixelEngine PRIVATE ${OPENGL_LIBRARIES} glfw)
target_link_libraries(PixelEngine PRIVATE assimp)
target_link_libraries(PixelEngine PRIVATE ${FREETYPE_LIBRARIES})
target_link_libraries(PixelEngine PRIVATE OpenAL)

target_include_directories(PixelEngine
    PRIVATE
    "Libraries/include/"
    "src/vendor/"
    "Libraries/freetype/include/"
    "Libraries/openal/include/"
)

if (WIN32)
    target_compile_definitions(PixelEngine PRIVATE "_CRT_SECURE_NO_WARNINGS")
    target_compile_options(PixelEngine PRIVATE "/W4")
    target_link_options(PixelEngine PRIVATE "/SUBSYSTEM:Windows;/ENTRY:mainCRTStartup")
endif()