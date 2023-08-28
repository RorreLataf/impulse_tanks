cmake_minimum_required(VERSION 3.5)

project(project_impulse_tanks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(glfw3 3.3 REQUIRED)
#find_package(OpenGL REQUIRED)



#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#MESSAGE( STATUS "CMAKE_INSTALL_RPATH: " "${CMAKE_INSTALL_PREFIX}/lib" )
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


#set(CMAKE_SKIP_RPATH TRUE)

set(CMAKE_BUILD_RPATH "./lib")

#set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

#
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#set(CMAKE_INSTALL_RPATH $ORIGIN)


add_library(raylib SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libraylib.so.4.2.0"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
)

#add_library(fade2d SHARED IMPORTED) # or STATIC instead of SHARED
#set_target_properties(fade2d PROPERTIES
#        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libfade2d.so"
#        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/fade2d"
#)

add_library(poly2tri STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(poly2tri PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libpoly2tri.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/poly2tri"
)

add_library(box2d STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(box2d PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libbox2d.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/box2d"
)

add_executable(${PROJECT_NAME}
        main.cpp
        Unit.h Unit.cpp
        Robot.h Robot.cpp
        Chassis.h Chassis.cpp
        Gun.h Gun.cpp
        GlobalParams.h GlobalParams.cpp
        Init.h Init.cpp
        #ex0_hello_triangulation.cpp
        #main_example_p2t.cpprm
        #core_2d_camera_mouse_zoom.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
        #${CMAKE_SOURCE_DIR}/include/raylib
        #${CMAKE_SOURCE_DIR}/include/poly2tri
        #${CMAKE_SOURCE_DIR}/include/fade2d
        ${CMAKE_SOURCE_DIR}/include/nlohman_json
)

#target_link_libraries(${PROJECT_NAME} raylib fade2d poly2tri glfw OpenGL::GL)
target_link_libraries(${PROJECT_NAME} raylib poly2tri box2d)
