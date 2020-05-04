## StarWind Engine

#### 3D game engine

StarWind is 3D game engine based on ECS architecture. It supports both Windows and Linux.

![Start screen](https://user-images.githubusercontent.com/29126360/75745987-eb3e6a80-5d29-11ea-90c0-2895e6694149.png)

#### Features
* Entity-Component-System architecture for creation and processing of game objects;
* Automatic resources management (loading and unloading);
* Skeletal animation with state machine;
    * Ability to blend and override animation clips;
    * Support of smooth transition between states;
* Ability to import meshes, skeletons and animation clips from popular 3D formats like `FBX` and `Collada`;
* GUI module with screens, labels, buttons, images, input fields;
* Input processing module;
* Game screens and states management module;

#### How to build:

##### Prerequisites

* C++17/20 compiler;
* Conan package manager;
* CMake;

##### Build steps

1. Clone repository.
2. Add the following remote repositories to Conan package manager via `conan remote add`:
    *  https://api.bintray.com/conan/bincrafters/public-conan
    *  https://api.bintray.com/conan/inexorgame/inexor-conan
    * https://api.bintray.com/conan/jinncrafters/conan
3. Install dependencies via Conan package manager, generate `conanbuildinfo.cmake` file and put it in `build/conanbuildinfo.cmake`.
4. Build projects `Engine`, `Game` or `Tests` with CMake. 
5. For `Game` or `Tests` projects set working directory to directory `bin`.

The `MeshTool` building process is pretty more complicated. In this case it is required to install `Assimp` as Git module, manually build and install it with CMake and put `include/assimp` folder in `ThirdParty`, put lib file in `build/libraries/assimp.lib` and finally put shader library file in working directory.

#### Project status and contributing:

StarWind project is just pet project and is do not develop very active. But it is quite alive and is being improved.

We would be happy to see new contributors. We need some help, especially help of designer or 3D modeller.
