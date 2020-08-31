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

* C++20 compiler;
* Conan package manager;
* CMake;

##### Build steps

######  Clone engine repository  
```bash
git clone --recurse-submodules git@github.com:n-paukov/swengine.git  

cd swengine  

mkdir bin  
  ```

######  Install third-party dependencies
```bash
conan remote add https://api.bintray.com/conan/bincrafters/public-conan

mkdir build

cd build  

conan install ..  
  
cd ..
 ```

Check the existence of `conanbuildinfo.cmake` file in the  `build` directory.

######  Build project
Build projects `Engine`, `Game` or `Tests` with CMake. 

For `Game` or `Tests` projects set working directory to directory `bin`.

#### Project status and contributing:

StarWind project is just pet project and is do not develop very active. But it is quite alive and is being improved.

We would be happy to see new contributors. We need some help, especially help of designer or 3D modeller.
