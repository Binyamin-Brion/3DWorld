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


add_executable(GameProtoType main.cpp ${GLAD_DIRECTORY}/glad.c Window/RenderWindow.cpp Window/RenderWindow.h Logging/Logger.cpp
               Logging/Logger.h Logging/LoggerPIMPL.cpp Logging/LoggerPIMPL.h Render/CompileTimeChecks/ValidMappingFlags.h
               Render/CompileTimeChecks/ValidBufferTarget.h Render/CompileTimeChecks/ValidBufferFlags.h
               Render/VBO/BufferStorage.h Render/Shaders/ShaderProgramBase.cpp Render/Shaders/ShaderProgramBase.h
               Render/Shaders/InstanceShaderProgram.cpp Render/Shaders/InstanceShaderProgram.h Render/CommandCentre.cpp
               Render/CommandCentre.h ProgramSettings.h World/BoundingVolumes/AABB.cpp World/BoundingVolumes/AABB.h World/BoundingVolumes/StaticAABB.cpp
               World/RangeImplementation/Range.h World/RangeImplementation/Range.cpp World/BoundingVolumes/StaticAABB.h World/BoundingVolumes/DynamicAABB.cpp
                World/BoundingVolumes/DynamicAABB.h  World/RangeImplementation/Range.cpp World/RangeImplementation/XRange.cpp
                World/RangeImplementation/XRange.h World/RangeImplementation/Range.cpp World/RangeImplementation/Range.h
                World/RangeImplementation/YRange.cpp World/RangeImplementation/YRange.h World/RangeImplementation/ZRange.cpp
                World/RangeImplementation/ZRange.h World/WorldLogic/GridSection.h World/WorldLogic/GridSection.cpp World/WorldLogic/WorldGenerator.cpp
        World/WorldLogic/WorldGenerator.h ProgramInformation/WorldSettings.cpp ProgramInformation/WorldSettings.h Window/Camera/CameraMovement.h
        Window/Camera/CameraObject.cpp Window/Camera/CameraObject.h)

target_link_libraries(GameProtoType glfw assimp -lstdc++fs)