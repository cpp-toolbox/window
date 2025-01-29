#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
  public:
    GLFWwindow *initialize_glfw_glad_and_return_window(unsigned int &window_width_px, unsigned int &window_height_px,
                                                       const char *window_name, bool start_in_fullscreen,
                                                       bool start_with_mouse_captured, bool vsync,
                                                       bool print_out_opengl_data = false);

    GLFWwindow *glfw_window;
    void print_opengl_info();
    void toggle_mouse_mode();
    bool cursor_is_grabbed = false;
    bool window_in_fullscreen = false;
};

#endif // WINDOW_HPP
