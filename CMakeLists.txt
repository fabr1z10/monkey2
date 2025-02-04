cmake_minimum_required(VERSION 3.4...3.18)

project(monkey2)

set(CMAKE_CXX_STANDARD 17)

#set(MONKEY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PNG REQUIRED)
find_package(yaml-cpp REQUIRED)
set(Python_EXECUTABLE "/usr/bin/python3.10")
set(Python_INCLUDE_DIR "/usr/include/python3.10")
set(Python_LIBRARY "/usr/lib/x86_64-linux-gnu/libpython3.10.so")
find_package(Python 3.10 REQUIRED COMPONENTS Interpreter Development)
execute_process(COMMAND ${Python_EXECUTABLE} -c "import site; print(site.getsitepackages()[0])"
        OUTPUT_VARIABLE PYTHON_SITEARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

message("Python 3.10 site-packages: ${Python_EXECUTABLE}")
add_subdirectory(pybind11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB MONKEY_SOURCE src/*.cpp src/primitives/*.cpp src/components/*.cpp src/skeletal/*.cpp)

pybind11_add_module(monkey2 ${MONKEY_SOURCE})

target_link_libraries(monkey2 PUBLIC ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${PNG_LIBRARIES} ${YAML_CPP_LIBRARIES})

install(TARGETS monkey2 COMPONENT python LIBRARY DESTINATION "${Python_SITEARCH}")
