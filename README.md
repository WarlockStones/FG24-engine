# Futuregames 2024 Game engine
Read the docs/ directory for code convention, TODO list, and general documentation.

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


#### 4. Compile using CMake:
Build using CMakeUserPresets.json
```
cd "path\to\cloned-repository"
cmake --preset=default
cmake --build build
```

Build using terminal:
```
mkdir build
cd build
cmake ..
make
```

Build using Visual Studio 2022 version 17, CMake, and vcpkg:
```
1. Make sure you have your CMakeUserPresets.json configured with a path to vcpkg.cmake
	"CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
2. Start Visual Studio and "Open a local folder" the project's root directory.
3. In the top "standard toolbar" select not the default but your user configuration preset.
	This is probably "vcpkg" or whatever it is named in CMakeUserPresets.json.
4. Go to CMakeLists.txt and reload the CMake cache.
5. Build the project with. Build all (Ctrl+Shift+b).
```