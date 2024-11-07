# Coding Convention
## GENERAL
### Use the tab character for indentation
### Use trailing braces everywhere (if, else, functions, structures, classes, etc.)
```
if (x) {
} else {
}
```

### Always use braces even when optional
```
if (x) {
	DoOneThing();
}
```

### Use floating point values unless there is a need for double
`float f = 1.0f`\
And not\
`float f = 1.f;`

### Function names start with an upper case
`void ThisFunctionDoesSomething();`

### Variable names start with a lower case 
`float maxExperiencePoints{};`

### Initialize your variables using braced initialization if the starting number is not important
`int distanceToCheckpoint{};`\
`float* SomePointer{};`\
`int startingGold = 0;`

### Always initialize your variables
```
constexpr int maxPlayers = 8;
int currentPlayers = 0;
Player* lastConnectedPlayer{}; 
```

### Prefer 'using' over typedef
`using FileHandle = int;`\
And not\
`typedef FileHandle int;`

### Use constexpr to define constants
`constexpr int StartingPoint = 0;`

### Use const and constexpr as much as possible

### Try to avoid early returns whenever possible
Pre condition checks are fine. 

### Don't use the continue keyword and limit the use of break
```
for (...;...;...) {
  // Statement

  if ( !expression ) {
    // Statement 2
  }
}
```
And not
```
for (...) {
	// Statement
	if ( expression ) {
    		continue;
	}
	// Statement 2
}
```

### Always use classed enums
```
enum class WebColor { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };
```
And not
```
enum WebColor { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };
```
For better type checking.

### Pointer should be placed near type
`int* p;`\
And not\
`int *p;` or `int * p;`

### Use `#pragma once` as header guard

### Avoid using macros
Macros for 

### Use the namespace FG24:: to not clutter global namespace
This also avoids the need for prefixed class names. Don't prefix class names.
### Do not indent the namespace
```
namespace FG24 {
class SomeClass {
	void SomeMethod();
}
};
```
And not
```
namespace FG24 {
	class SomeClass {
		void SomeMethod();
	}
};
```

### Forward declare classes at top of file after includes
```
#include <cstdio>

class Foo;
class Bar;

class C {
public:
	Foo f;
	Bar b;
}

```
and not
```
class C {
public:
	class Foo f;
	class Bar b;
}
```
This is to also to not declare something inside the namespace as FG24::Foo. 
Which is an issue when using external libraries.

### Use C++ // comments and not C /* */ comments
`// Some comment`
And not
`/* Some comment */`

### Keep destructors and constructors as simple as possible
They should only allocate and deallocate memory. Call other function if state needs to be changed.

### Use C++17

### Use libc instead of the C++ standard library
This is mostly for educational purposes and may be changed in the future.\
I may allow for unique_ptr because they are nice.

### Include the std namespaced c library files
```
#include <cstdio>
std::printf(str);
```
And not
```
#include <stdio.h>
printf(str);
```


## CLASSES
### Class variabls have the same naming convention as variables
```
class Vec3 {
	float x;
	float y;
	float z;
}
```

### Class methods have the same naming convention as functions
```
class Vec3 {
	float Length() const;
}
```

### Ordering of class variables and methods should be as follows
1. list of friend classes
2. public variables
3. public methods
4. protected variables
5. protected methods
6. private variables
7. private methods

This allows the public interface to be found easily at the beginning of each class.

### Always make class methods 'const' when they do not modify any variables
### Always make class methods 'constexpr' when possible


## FILE NAMES
### Each class should be in a separate source file unless it makes sense to group several smaller classes.

### The file names hould be the same as the name of the class.
class Warning {};\
src/logging/Warning.hpp\
src/logging/Warning.cpp\

### All directories and assets names are lowercase.
src/game/Game.hpp\
Longer asset names may use underscore to separate prefix and suffixes\
assets/textures/t_floor_plank_01.png\

C++ source files are named `.cpp`.\
C++ header files are named `.hpp`.\

OpenGL vertex shaders are named `.vert`\
OpenGL fragment shaders are named `.frag`\

