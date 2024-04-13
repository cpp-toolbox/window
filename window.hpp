#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../input_snapshot/input_snapshot.hpp"

GLFWwindow *initialize_glfw_glad_and_return_window(unsigned int *window_width_px, unsigned int *window_height_px,
                                                   const char *window_name, bool start_in_fullscreen,
                                                   InputSnapshot *input_snapshot);

#endif // WINDOW_HPP
