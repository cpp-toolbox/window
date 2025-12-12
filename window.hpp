#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <numeric>
#include <optional>
#include <ostream>
#include <vector>

#include "sbpt_generated_includes.hpp"

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
  private:
    Logger logger{"window"};

  public:
    unsigned int width_px, height_px;
    int top_left_corner_of_window_x, top_left_corner_of_window_y;

    Window(unsigned int width_px = 700, unsigned int height_px = 700, const std::string &window_name = "my program",
           bool start_in_fullscreen = false, bool start_with_mouse_captured = false, bool vsync = false,
           bool print_out_opengl_data = false);
    ~Window();
    GLFWwindow *glfw_window;
    void print_opengl_info();
    void toggle_mouse_mode();
    void disable_cursor();
    void enable_cursor();
    void set_cursor_pos(double xpos, double ypos);

    /*
     * @note 2d screen space  (2d-ss) is defined as the coordinate system where the top left of the screen is (0, 0)
     * and the bottom right is (width_px, height_px) where those values is the pixel count of your monitor. Note that
     * even though the pixel count is always an integer, positions in 2d-ss might not be, for example you could be at
     * the position (5.2, 8.9)
     *
     * @note 2d normalized screen space (2d-nss) is defined as the coordinate system where the middle of the screen is
     * (0, 0), top right edge is given by (1, 1), and the bottom left edge is (-1, 1), this means that the screen's
     * coordinates are bounded with in a 2x2 square
     *
     * @note By default 2d-nss causes stretching because the physical screen is not actually a square and is instead a
     * rectangle, but the pixels on the screen are still squares, causing the pixel count on each axis to be different.
     * In order to correct for this problem one of the axes needs to be scaled to correct for this. By default on an
     * 16:9 monitor running 1920x1080 since the width is greater than the height, then we have to scale down x axis
     * so that stretching does not occur. This new space is a space where exactly one of the axis ranges from -1 to 1,
     * and the other one ranges from -a to a where a is the scaling factor applied to fix the stretching. We call this
     * new space aspect corrected 2d-nss or 2d-acnss
     *
     * @note This function is mainly used on mouse positions as they are measured in 2d-ss
     *
     */

    std::tuple<double, double> convert_point_from_2d_screen_space_to_2d_normalized_screen_space(double x, double y);

    std::tuple<double, double> get_corrective_aspect_ratio_scale();
    std::tuple<double, double>
    convert_point_from_2d_screen_space_to_2d_aspect_corrected_normalized_screen_space(double x, double y);

    bool window_should_close() { return glfwWindowShouldClose(glfw_window); }

    // these functions only exist so I don't have to rember the opengl api for this
    void enable_wireframe_mode();
    void disable_wireframe_mode();

    void enable_backface_culling();
    void disable_backface_culling();

    void set_resolution(const std::string &resolution);

    void toggle_fullscreen();
    void enable_fullscreen();
    void disable_fullscreen();
    void set_fullscreen_by_on_off(const std::string &on_off_string);

    std::tuple<unsigned int, unsigned int> reduce_ratio(std::tuple<unsigned int, unsigned int> ratio) {
        auto [num, den] = ratio;
        if (den == 0) {
            // Handle division by zero gracefully (you can also throw an exception)
            return {0, 0};
        }

        unsigned int g = std::gcd(num, den);
        return {num / g, den / g};
    }

    std::tuple<unsigned int, unsigned int> get_aspect_ratio_in_simplest_terms() {
        return reduce_ratio({this->width_px, this->height_px});
    }

    void start_of_tick_glfw_logic() {
        {
            LogSection _(*global_logger, "gl clear", false);
            // clear buffers before tick
            // NOTE: in the future the user can specify what they want to clear.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void end_of_tick_glfw_logic() {
        {
            LogSection _(*global_logger, "gl swap buffer and poll events", false);
            // swap and poll after tick
            {
                LogSection _(*global_logger, "swap buffers");
                glfwSwapBuffers(glfw_window);
            }
            {
                LogSection _(*global_logger, "poll events");
                glfwPollEvents();
            }
        }
    }

    std::function<void(double)> wrap_tick_with_required_glfw_calls(std::function<void(double)> tick) {
        return [tick, this](double dt) {
            start_of_tick_glfw_logic();
            tick(dt);
            end_of_tick_glfw_logic();
        };
    }

    bool cursor_is_disabled = false;
    bool window_in_fullscreen = false;
};

#endif // WINDOW_HPP
