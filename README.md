# Info

A glfw window 

# Depdendencies

* [glfw](https://github.com/glfw/glfw)
* [glad3.3](https://github.com/opengl-toolbox/glad_opengl_3.3_core)
* [input snapshot](https://github.com/opengl-toolbox/input_snapshot) - stores the state of mouse and keyboard used in the key and mouse callbacks

# CMake

```
...

# GLAD: opengl function loader

include_directories(external_libraries/glad_opengl_3.3_core/include)
add_subdirectory(external_libraries/glad_opengl_3.3_core)

# GLFW

# disable unnessary steps in build
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external_libraries/glfw)

... 

target_link_libraries(your_project_name ... glfw glad)
```
