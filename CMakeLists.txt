cmake_minimum_required (VERSION 3.0.2)
project("Hello Window OpenGL")

set (CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

include_directories(
    # my headers
    "include/"
    )

#set( GLFW_STATIC_LIB "/media/kaan/Data7510/GitProjects/glfw-build/src/libglfw3.a")
set( GLFW_SHARED_LIB
    "${PROJECT_SOURCE_DIR}/libs/glfw/src/libglfw.so")

set (BOOST_SYSTEM_DYNAMIC_LIB
    "${PROJECT_SOURCE_DIR}/libs/boost/libboost_system.a")

set (BOOST_FILESYSTEM_DYNAMIC_LIB
    "${PROJECT_SOURCE_DIR}/libs/boost/libboost_filesystem.a")


set (FLAGS "-ldl -ggdb -Wall -Wextra")

set ( ALL_LIBS 
    ${OpenGL}
    ${GLFW_SHARED_LIB}
    ${BOOST_SYSTEM_DYNAMIC_LIB}
    ${BOOST_FILESYSTEM_DYNAMIC_LIB}
    "-ldl"
    )

add_executable(myWin 
    "src/glad.c"
    "src/window.cpp"
    )

add_executable(myTriangle
    "src/glad.c"
    "src/helloTriangle/helloTriangle.cpp"
    )


target_link_libraries(myWin ${ALL_LIBS})


target_link_libraries(myTriangle ${ALL_LIBS})

install(TARGETS myWin DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
install(TARGETS myTriangle DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
