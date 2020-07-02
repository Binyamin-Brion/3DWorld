# 3DWorld
A randomly generated 3D voxel world

## Overview

A randomly generated voxel 3D world to move around in. 

A voxel world is a world comprised of cubes only. This gives it a look that is distinctive to other types of rendered worlds.

## Motivation

This world is static- it contains no moving parts. It is literally just a rendered world. 

This was done to gain experience with:

* OpenGL
* Basic usage with OpenMP
* Working with a decently sized project. 

## Sample Renders

Below are two sample scenes from the voxel world. Render distance is set to 384 (game world units):

![alt-text](https://github.com/Binyamin-Brion/3DWorld/blob/master/Assets/SampleRenders/sampleRender2.png)

![alt-text](https://github.com/Binyamin-Brion/3DWorld/blob/master/Assets/SampleRenders/sampleRender.png)

## Technologies Used

* C++17
* OpenMP
* OpenGL 4.3
* Qt 5.9.9

## Libraries Used

This project was built with external libraries, which can be found in the ProjectSourceDependices Folder:

* glm (https://glm.g-truc.net/0.9.9/index.html)
* thinks/poisson-disk-sampling (https://github.com/thinks/poisson-disk-sampling)
* glad (https://glad.dav1d.de/)
* Auburns /FastNoise (https://github.com/Auburns/FastNoise)

## Notes

* In addition to needing Qt5.9.8 and a C++17 compliant compiler, the location to the Qt installation folder will need to be updated in the CMake file to match where it is installed on the machine being used for compilation.

* The textures used for the static assets were obtained from various Google Image Search Results.

* The textures used for the skybox are from babylon.js Texture Resources Page

    (https://doc.babylonjs.com/resources/playground_textures#cubetextures).

* To exit the program, press the "Escape" key.

