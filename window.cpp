#include "window.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void error_callback(int error, const char *description);

/**
 * \brief make a glfw window
 *
 * \details create a glfw window for opengl3.3 core and load in opengl function pointers (implementation)
 *
 * \note this doesn't register any window callbacks, you have to do that yourself.
 *
 * \param screen_width the requested screen height
 * \param screen_height the requested screen width
 * \return an optional window, based on whethe or not initialization was successful
 *
 * \author cuppajoeman
 * \date created: 2024-02-25, edited: 2024-02-26
 */
GLFWwindow *initialize_glfw_glad_and_return_window(unsigned int *window_width_px, unsigned int *window_height_px,
                                                   const char *window_name, bool start_in_fullscreen,
                                                   InputSnapshot *input_snapshot_ptr) {

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("glfw couldn't be initialized");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;

    if (start_in_fullscreen) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        *window_width_px = mode->width;
        *window_height_px = mode->height;
        window = glfwCreateWindow(*window_width_px, *window_height_px, window_name, monitor, NULL);
    } else {
        window = glfwCreateWindow(*window_width_px, *window_height_px, window_name, NULL, NULL);
    }

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }

    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers, note that opengl will not work until
    // the next line gets called.
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("failed to initialize GLAD");
    }

    glfwSetWindowUserPointer(window, input_snapshot_ptr);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

static void error_callback(int error, const char *description) { fprintf(stderr, "Error: %s\n", description); }

/**
 * \brief whenever a key is pressed update the input snapshot which is created in main() { ...
 *
 * \pre the user pointer is pointing to an InputSnapshot.
 *
 * \author cuppajoeman
 * \date created: 2024-04-10
 */
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // TODO figure out how to do mappings of key to function to simplify
    InputSnapshot *input_snapshot = static_cast<InputSnapshot *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_Q) {
        if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    } else if (key == GLFW_KEY_F) {
        // TOGGLE FULLSCREEN
        // https://stackoverflow.com/a/47462358/6660685 <- implement this
    } else if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            input_snapshot->left_pressed = true;
        } else if (action == GLFW_RELEASE) {
            input_snapshot->left_pressed = false;
        }
    } else if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            input_snapshot->right_pressed = true;
        } else if (action == GLFW_RELEASE) {
            input_snapshot->right_pressed = false;
        }
    } else if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            input_snapshot->forward_pressed = true;
        } else if (action == GLFW_RELEASE) {
            input_snapshot->forward_pressed = false;
        }
    } else if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            input_snapshot->backward_pressed = true;
        } else if (action == GLFW_RELEASE) {
            input_snapshot->backward_pressed = false;
        }
    } else if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS) {
            input_snapshot->jump_pressed = true;
        } else if (action == GLFW_RELEASE) {
            input_snapshot->jump_pressed = false;
        }
    }
}
