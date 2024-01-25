cmake_minimum_required(VERSION 3.5)

project(project_impulse_tanks LANGUAGES CXX)

add_definitions(-std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        main.cpp
        Unit.h Unit.cpp
        Robot.h Robot.cpp
        Chassis.h Chassis.cpp
        Gun.h Gun.cpp
        GlobalParams.h GlobalParams.cpp
        Init.h Init.cpp
        Screen.h Screen.cpp
        LogoScreen.h LogoScreen.cpp
        TitleScreen.h TitleScreen.cpp
        GameplayScreen.h GameplayScreen.cpp
        GameState.h GameState.cpp
        Physics.h Physics.cpp
        Map.h Map.cpp
        Gameplay.h Gameplay.cpp
##        core_2d_camera.cpp
#        test_textures_polygon.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include/nlohman_json
)

if (DEFINED EMSCRIPTEN)

  target_link_libraries(${PROJECT_NAME} raylib box2d poly2tri)

  set(BUILD_WEB_HEAP_SIZE 134217728)
  #set(BUILD_WEB_RESOURCES_PATH "../res")
  set(BUILD_WEB_RESOURCES_PATH "res")

  add_compile_definitions(PLATFORM_WEB)

  add_library(raylib STATIC IMPORTED) # or STATIC instead of SHARED
  set_target_properties(raylib PROPERTIES
          IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libraylib_web.a"
          INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
  )

  add_library(poly2tri STATIC IMPORTED) # or STATIC instead of SHARED
  set_target_properties(poly2tri PROPERTIES
          IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libpoly2tri_web.a"
          INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/poly2tri"
  )

  add_library(box2d STATIC IMPORTED) # or STATIC instead of SHARED
  set_target_properties(box2d PROPERTIES
          IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libbox2d_web.a"
          INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/box2d"
  )

  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
  #set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

  # -s USE_GLFW=3 -s TOTAL_MEMORY=${BUILD_WEB_HEAP_SIZE} -s FORCE_FILESYSTEM=1 -s GL_ENABLE_GET_PROC_ADDRESS
  # set(CMAKE_CC_FLAGS "${CMAKE_CC_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
  # set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1 -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")

  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Os -std=c++17 -Wall -Wno-missing-braces -Wunused-result -D_DEFAULT_SOURCE ")

  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s TOTAL_MEMORY=${BUILD_WEB_HEAP_SIZE} -s FORCE_FILESYSTEM=1 --preload-file ${BUILD_WEB_RESOURCES_PATH}")

else()
  # For Desktop
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)

  target_link_libraries(${PROJECT_NAME} raylib box2d poly2tri ${CMAKE_DL_LIBS} Threads::Threads)

  add_library(raylib STATIC IMPORTED) # or STATIC instead of SHARED
  set_target_properties(raylib PROPERTIES
          IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libraylib.a"
          INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
  )

  add_library(poly2tri STATIC IMPORTED) # or STATIC instead of SHARED
  set_target_properties(poly2tri PROPERTIES
          IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libpoly2tri.a"
          INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/poly2tri"
  )

  add_library(box2d STATIC IMPORTED) # or STATIC instead of SHARED
  set_target_properties(box2d PROPERTIES
          IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libbox2d_ubuntu22.a"
          INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/box2d"
  )

  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-ldl")



  set(CMAKE_BUILD_RPATH "./lib")

endif()







