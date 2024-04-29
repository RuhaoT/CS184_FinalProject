cmake_minimum_required(VERSION 3.11)
project(High-LevelPuzzle)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

# disable optimization
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
STRING(REPLACE "-O2" "-Od" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
add_compile_options(-fno-omit-frame-pointer -Og)


include_directories("${CMAKE_SOURCE_DIR}/json/include")
find_package(nlohmann_json 3.11.3 REQUIRED)
# document generation
if(ENABLE_DATASET)
        add_definitions(-DDATASET_ENABLE)
endif()



#########################################
#####                               #####
#####           Option              #####
#####                               #####
#########################################

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_OPENGL_GLFW_IMGUI "Use ImGui"          ON)
option(LIBIGL_WITH_CGAL              "Use CGAL"           ON)

#########################################
#####                               #####
#####      External Library         #####
#####                               #####
#########################################

#EIGEN
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

#Boost
find_package(Boost REQUIRED COMPONENTS system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#3rd party library
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/")

#########################################
#####                               #####
#####      Core Library             #####
#####                               #####
#########################################
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB 3DCoreFiles
        src/Mesh/*.h
        src/Mesh/*.cpp
        src/Utility/*.h
        src/Utility/*.cpp
        src/Puzzle/*.h
        src/Puzzle/*.cpp
        src/PuzConverter/*.h
        src/PuzConverter/*.cpp
        src/Puzzle_debug/*.h
        src/Puzzle_debug/*.cpp
        src/Puzzle/Puzzle_smooth.h
        src/Puzzle/Puzzle_smooth.cpp
        src/Dataset/*.h
        src/Dataset/*.cpp
        src/RandomForest/*.h
        src/RandomForest/*.cpp
        )

include_directories(src)

add_library(3DCorelib STATIC ${3DCoreFiles})
target_compile_options(3DCorelib PUBLIC "-Wno-deprecated-declarations")

target_include_directories(3DCorelib PUBLIC SYSTEM
        ${ext_include}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        )

target_link_libraries(3DCorelib PUBLIC
        ${ext_lib})

target_link_libraries(3DCorelib PUBLIC ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
#########################################
#####                               #####
#####      Gui Library              #####
#####                               #####
#########################################

file(GLOB 3DRenderFiles
        src/Mesh/*.cpp
        src/Mesh/*.h
        src/Puzzle/*.cpp
        src/Puzzle/*.h
        src/Utility/*.cpp
        src/Utility/*.h
        src/PuzConverter/*.h
        src/PuzConverter/*.cpp
        src/Render/*.h
        src/Render/*.cpp
        src/Dataset/*.h
        src/Dataset/*.cpp
        src/RandomForest/*.h
        src/RandomForest/*.cpp
        )

add_library(3DRenderlib STATIC ${3DRenderFiles})

target_include_directories(3DRenderlib PUBLIC SYSTEM
        ${ext_include}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Render/
        )

target_link_libraries(3DRenderlib PUBLIC
        ${ext_lib})

#########################################
#####                               #####
#####        Gui Program            #####
#####                               #####
#########################################

add_executable(${PROJECT_NAME}_bin ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME}_bin PUBLIC 3DCorelib)
target_link_libraries(${PROJECT_NAME}_bin PUBLIC 3DRenderlib)

# performance test target
add_executable(${PROJECT_NAME}_perf ${CMAKE_CURRENT_SOURCE_DIR}/src/perf.cpp)
target_link_libraries(${PROJECT_NAME}_perf PUBLIC 3DCorelib)
target_link_libraries(${PROJECT_NAME}_perf PUBLIC 3DRenderlib)

target_link_libraries(${PROJECT_NAME}_bin PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME}_perf PUBLIC nlohmann_json::nlohmann_json)

if(ENABLE_DATASET)
        # data target
        add_executable(${PROJECT_NAME}_data ${CMAKE_CURRENT_SOURCE_DIR}/src/data.cpp)
        target_link_libraries(${PROJECT_NAME}_data PUBLIC 3DCorelib)
        target_link_libraries(${PROJECT_NAME}_data PUBLIC 3DRenderlib)
        target_link_libraries(${PROJECT_NAME}_data PUBLIC nlohmann_json::nlohmann_json)
endif()

# Add the source files for the train target
file(GLOB TrainFiles
        src/Dataset/*.h
        src/Dataset/*.cpp
        src/RandomForest/*.h
        src/RandomForest/*.cpp
        )

add_executable(${PROJECT_NAME}_train ${CMAKE_CURRENT_SOURCE_DIR}/src/train.cpp)
target_link_libraries(${PROJECT_NAME}_train PUBLIC 3DCorelib)
target_link_libraries(${PROJECT_NAME}_train PUBLIC 3DRenderlib)
target_link_libraries(${PROJECT_NAME}_train PUBLIC nlohmann_json::nlohmann_json)