cmake_minimum_required(VERSION 3.15)
project(GameProtoType)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)
find_package(ASSIMP REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIR})

# All dependencies used as source files should be in this folder.
set(DEPENDENCIES_FOLDER /home/binybrion/ProjectSourceDependencies)

set(GLM_FOLDER ${DEPENDENCIES_FOLDER}/glm-0.9.9.3/glm)
include_directories(${GLM_FOLDER})

# Glad library cannot be included through a "include_directories" command as there are .c files; to prevent undefined
# references to functions, these sources files are added to the 'add_executable; function. These below variables are used
# to make the CMake file easier to change if the location of the glad library changes.
set(GLAD_DIRECTORY ${DEPENDENCIES_FOLDER}/glad)
include_directories(${GLAD_DIRECTORY})
set(GLAD_SOURCES ${GLAD_DIRECTORY}/glad.c Render/VBO/BufferStorage.h Render/Shaders/ShaderProgramBase.cpp Render/Shaders/ShaderProgramBase.h Render/Shaders/InstanceShaderProgram.cpp Render/Shaders/InstanceShaderProgram.h Render/CommandCentre.cpp Render/CommandCentre.h ProgramSettings.h)

add_executable(GameProtoType main.cpp ${GLAD_SOURCES} Window/RenderWindow.cpp Window/RenderWindow.h Logging/Logger.cpp
               Logging/Logger.h Logging/LoggerPIMPL.cpp Logging/LoggerPIMPL.h Render/CompileTimeChecks/ValidMappingFlags.h
               Render/CompileTimeChecks/ValidBufferTarget.h Render/CompileTimeChecks/ValidBufferFlags.h)

target_link_libraries(GameProtoType glfw assimp -lstdc++fs)