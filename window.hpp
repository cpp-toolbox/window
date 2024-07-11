#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "live_input_state.hpp"

GLFWwindow *initialize_glfw_glad_and_return_window(
    unsigned int *window_width_px, unsigned int *window_height_px,
    const char *window_name, bool start_in_fullscreen,
    LiveInputState *input_snapshot);

#endif // WINDOW_HPP
