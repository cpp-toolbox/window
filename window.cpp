#include "window.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

static void mouse_move_callback(GLFWwindow *window, double mouse_position_x, double mouse_position_y);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void on_window_size_change(GLFWwindow *window, int width, int height);
static void error_callback(int error, const char *description);

static bool cursor_is_grabbed = false;

/**
 * \brief make a glfw window
 *
 * \details create a glfw window for opengl3.3 core and load in opengl function
 * pointers (implementation)
 *
 * \param screen_width the requested screen height
 * \param screen_height the requested screen width
 * \return an optional window, based on whether or not initialization was
 * successful
 *
 * \author cuppajoeman
 * \date created: 2024-02-25, edited: 2024-07-11
 */
GLFWwindow *initialize_glfw_glad_and_return_window(unsigned int *window_width_px, unsigned int *window_height_px,
                                                   const char *window_name, bool start_in_fullscreen,
                                                   bool start_with_mouse_captured, bool vsync,
                                                   LiveInputState *input_snapshot_ptr) {

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

    // glad: load all OpenGL function pointers, note that opengl will not work
    // until the next line gets called.

    // glad2
    // if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     throw std::runtime_error("failed to initialize GLAD");
    // }

    // glad1
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("failed to initialize GLAD");
    }

    // disable this for debugging so you can move the mouse outside the window
    if (start_with_mouse_captured) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    int vsync_int = vsync;

    glfwSwapInterval(vsync_int);

    if (glfwRawMouseMotionSupported()) {
        // logger.info("raw mouse motion supported, using it");
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetWindowUserPointer(window, input_snapshot_ptr);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetFramebufferSizeCallback(window, on_window_size_change);

    return window;
}

static void error_callback(int error, const char *description) { fprintf(stderr, "Error: %s\n", description); }

void toggle_mouse_mode(GLFWwindow *window) {
    if (cursor_is_grabbed) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    cursor_is_grabbed = !cursor_is_grabbed;
}

/**
 * \brief whenever a key is pressed update the input snapshot which is created
 * in main() { ...
 *
 * \pre the user pointer is pointing to an InputSnapshot.
 *
 * \author cuppajoeman
 * \date created: 2024-04-10
 */
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // TODO figure out how to do mappings of key to function to simplify
    LiveInputState *input_snapshot = static_cast<LiveInputState *>(glfwGetWindowUserPointer(window));
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
    } else if (key == GLFW_KEY_M) {
        bool mouse_toggle_just_pressed = (!cursor_is_grabbed and action == GLFW_PRESS) or (cursor_is_grabbed and action == GLFW_PRESS);
        if (mouse_toggle_just_pressed) {
            toggle_mouse_mode(window);
        }
    }
}

void mouse_move_callback(GLFWwindow *window, double mouse_position_x, double mouse_position_y) {
    LiveInputState *input_snapshot = static_cast<LiveInputState *>(glfwGetWindowUserPointer(window));
    input_snapshot->mouse_position_x = mouse_position_x;
    input_snapshot->mouse_position_y = mouse_position_y;
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void on_window_size_change(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
