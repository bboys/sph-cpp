cmake_minimum_required (VERSION 2.6)
project(sph-cpp)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(Cg)

set(src_list)

LIST(APPEND src_list bucket.cpp)
LIST(APPEND src_list main.cpp)
LIST(APPEND src_list particle.cpp)
LIST(APPEND src_list particleiterator.cpp)
LIST(APPEND src_list particlesource.cpp)

include_directories(${PROJECT_SOURCE_DIR})

# Graphics base
LIST(APPEND src_list graphics/base/scene.cpp)
LIST(APPEND src_list graphics/base/surface.cpp)

# Backup OpenGL mode
LIST(APPEND src_list graphics/opengl/particle.cpp)
LIST(APPEND src_list graphics/opengl/plane.cpp)
LIST(APPEND src_list graphics/opengl/scene.cpp)

if (${CG_FOUND})
    LIST(APPEND src_list graphics/cg/particle.cpp)
    LIST(APPEND src_list graphics/cg/plane.cpp)
    LIST(APPEND src_list graphics/cg/scene.cpp)
    LIST(APPEND src_list graphics/cg/shader.cpp)
endif()

# Add executable called "PROJECT_NAME" that is built from the source files
add_executable(${PROJECT_NAME} ${src_list})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES})

if (${CG_FOUND})
    target_link_libraries(${PROJECT_NAME} ${CG_LIBRARIES})
endif()
