#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <ostream>
#include <vector>

struct VideoMode {
    int width;
    int height;
    int refresh_rate;

    friend std::ostream &operator<<(std::ostream &os, const VideoMode &vm) {
        os << vm.width << "x" << vm.height << " @ " << vm.refresh_rate << "Hz";
        return os;
    }
};

std::vector<std::string> get_available_resolutions(const std::optional<std::string> &aspect_ratio = std::nullopt);

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
    void disable_cursor();
    void enable_cursor();

    // these functions only exist so I don't have to rember the opengl api for this
    void enable_wireframe_mode();
    void disable_wireframe_mode();

    void set_resolution(const std::string &resolution);

    void toggle_fullscreen();
    void enable_fullscreen();
    void disable_fullscreen();
    void set_fullscreen_by_on_off(const std::string &on_off_string);

    bool cursor_is_disabled = false;
    bool window_in_fullscreen = false;
};

#endif // WINDOW_HPP
