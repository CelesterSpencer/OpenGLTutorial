project(srccmake)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE PATH "Project specific path. Set manually if it was not found.")
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)

find_package(ASSIMP REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)

include_directories(
        ${ASSIMP_INCLUDE_DIR}
        ${GLFW3_INCLUDE_PATH}
        ${GLM_INCLUDE_PATH}
        ${EXTERNAL_DIR}/gl3w/include
        ${EXTERNAL_DIR}/imgui
        ${EXTERNAL_DIR}/stb
)

# copy dlls
set(DLLS
        ${GLFW3_DLL}
        ${ASSIMP_DLL}
)
file(COPY ${DLLS} DESTINATION /)

# copy other files
file(COPY shader DESTINATION /)
file(COPY textures DESTINATION /)
file(COPY model DESTINATION /)

# add source files
file(GLOB_RECURSE SOURCE_FILES
        "src/*.cpp"
)

# add gl3w
file(GLOB_RECURSE GL3W
        "${EXTERNAL_DIR}/gl3w/src/*.c"
)

# add stb_image
file(GLOB_RECURSE STB
        "${EXTERNAL_DIR}/stb/*.h"
        "${EXTERNAL_DIR}/stb/*.c"
)

# add imgui
file(GLOB_RECURSE IMGUI
        "${EXTERNAL_DIR}/imgui/*.h"
        "${EXTERNAL_DIR}/imgui/*.cpp"
)

add_executable(OpenGLTutorial
        ${SOURCE_FILES}
        ${GL3W}
        ${STB}
        ${IMGUI}
)


target_link_libraries(OpenGLTutorial
        opengl32.lib
        ${GLFW3_LIBRARY}
        ole32.lib
        oleaut32.lib
        version.lib
        imm32.lib
        winmm.lib
        ${ASSIMP_LIBRARY}
)
