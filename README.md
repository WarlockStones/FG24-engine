# Futuregames 2024 Game engine

## Compiling from source
### Windows:
#### 1. clone the git.

#### 2. Install pre-requisites:
CMake, vcpkg, Ninja, MVSC (MSBuild tools or bundled with Visual Studio IDE).

Make sure you can run all of these commands (in developer terminal):\
cmake, vcpkg, ninja, cl, link

I installed Ninja through WinGet, and MSBuild tools through Visual Studio Installer.

Dynamic libraries will be handled automatically by vcpkg.

#### 3. Configure the CMakeUserPresets.json
Check the 'CMakeUserPresets.json.example'.\
It needs path to vcpkg root and vcpkg CMake script.\
It may also need to have a compiler defined. e.g. 'g++' or 'cl'.
\

#### 4. Compile using CMake:
cd "path\to\cloned-repository"\
cmake --preset=default\
cmake --build build
