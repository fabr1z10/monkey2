cmake_minimum_required(VERSION 3.4...3.18)

project(monkey2)

set(CMAKE_CXX_STANDARD 17)

set(MONKEY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PNG REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Python COMPONENTS Interpreter Development)
add_subdirectory(pybind11)

file(GLOB MONKEY_SOURCE src/*.cpp src/primitives/*.cpp src/components/*.cpp)

pybind11_add_module(monkey2 ${MONKEY_SOURCE})

target_link_libraries(monkey2 PUBLIC ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${PNG_LIBRARIES} ${YAML_CPP_LIBRARIES})

install(TARGETS monkey2 COMPONENT python LIBRARY DESTINATION "${Python_SITEARCH}")
