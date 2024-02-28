#include "window.hpp"


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
std::optional<GLFWwindow *> initialize_glfw_and_return_window(const unsigned int screen_width, const unsigned int screen_height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "mwe font rendering", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return std::nullopt;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return std::nullopt;
    }

    return window;
}
