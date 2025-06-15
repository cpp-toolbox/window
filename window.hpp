#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
  public:
    unsigned int width_px, height_px;
    int top_left_corner_of_window_x, top_left_corner_of_window_y;

    Window(unsigned int width_px = 700, unsigned int height_px = 700, const char *window_name = "my program",
           bool start_in_fullscreen = false, bool start_with_mouse_captured = false, bool vsync = false,
           bool print_out_opengl_data = false);
    ~Window();
    GLFWwindow *glfw_window;
    void print_opengl_info();
    void toggle_mouse_mode();

    void toggle_fullscreen();
    void enable_fullscreen();
    void disable_fullscreen();

    bool cursor_is_grabbed = false;
    bool window_in_fullscreen = false;
};

#endif // WINDOW_HPP
