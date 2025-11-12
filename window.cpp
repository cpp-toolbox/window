#include "window.hpp"
#include <iostream>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

static void error_callback(int error, const char *description);

Window::Window(unsigned int width_px, unsigned int height_px, const std::string &window_name, bool start_in_fullscreen,
               bool start_with_mouse_captured, bool vsync, bool print_out_opengl_data)
    : width_px(width_px), height_px(height_px) {

    cursor_is_disabled = start_with_mouse_captured;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("glfw couldn't be initialized");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (start_in_fullscreen) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        width_px = mode->width;
        height_px = mode->height;
        glfw_window = glfwCreateWindow(width_px, height_px, window_name.c_str(), monitor, NULL);
    } else {
        glfw_window = glfwCreateWindow(width_px, height_px, window_name.c_str(), NULL, NULL);
    }

    if (glfw_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }

    glfwMakeContextCurrent(glfw_window);

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

    if (print_out_opengl_data) {
        print_opengl_info();
    }

    // disable this for debugging so you can move the mouse outside the window
    if (start_with_mouse_captured) {
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool rendering_3d_graphics = true;

    if (rendering_3d_graphics) {
        glEnable(GL_DEPTH_TEST); // configure global opengl state
    }

    int vsync_int = vsync;

    glfwSwapInterval(vsync_int);

    if (glfwRawMouseMotionSupported()) {
        // logger.info("raw mouse motion supported, using it");
        glfwSetInputMode(glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    logger.info("window has been successfully initialized");
}

// the window manges the glfw lifetime, also since we initialize window first before operating with opengl it is
// destructed last so that all other operations will not fail during program close
Window::~Window() {
    if (glfw_window)
        glfwDestroyWindow(glfw_window);

    glfwTerminate();
}

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
 */

void Window::print_opengl_info() {
    // Get OpenGL version and renderer info
    const char *version = (const char *)glGetString(GL_VERSION);
    const char *vendor = (const char *)glGetString(GL_VENDOR);
    const char *renderer = (const char *)glGetString(GL_RENDERER);
    const char *glslVersion = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

    // Print OpenGL version and hardware details
    std::cout << "==== OpenGL Information ====" << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "GLSL Version: " << glslVersion << std::endl;
    std::cout << std::endl;

    // GPU Resource Limits
    std::cout << "==== GPU Resource Limits ====" << std::endl;

    int max_vertex_uniforms, max_fragment_uniforms;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_vertex_uniforms);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_fragment_uniforms);
    std::cout << "Max Vertex Uniforms: " << max_vertex_uniforms << std::endl;
    std::cout << "Max Fragment Uniforms: " << max_fragment_uniforms << std::endl;

    GLint maxVertexUniformBlocks = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
    std::cout << "Maximum vertex uniform blocks: " << maxVertexUniformBlocks << std::endl;

    GLint maxGeometryUniformBlocks = 0;
    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &maxGeometryUniformBlocks);
    std::cout << "Maximum geometry uniform blocks: " << maxGeometryUniformBlocks << std::endl;

    GLint maxFragmentUniformBlocks = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
    std::cout << "Maximum fragment uniform blocks: " << maxFragmentUniformBlocks << std::endl;

    GLint maxCombinedUniformBlocks = 0;
    glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &maxCombinedUniformBlocks);
    std::cout << "Maximum combined uniform blocks: " << maxCombinedUniformBlocks << std::endl;

    GLint maxUniformBlockSize = 0;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
    std::cout << "Maximum uniform block size: " << maxUniformBlockSize << " bytes" << std::endl;

    GLint maxUniformBufferBindings = 0;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBindings);
    std::cout << "Maximum uniform buffer bindings: " << maxUniformBufferBindings << std::endl;

    GLint maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    std::cout << "Maximum texture size: " << maxTextureSize << "x" << maxTextureSize << " pixels" << std::endl;

    GLint maxVertexAttributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
    std::cout << "Maximum number of vertex attributes: " << maxVertexAttributes << std::endl;

    GLint maxVaryingFloats = 0;
    glGetIntegerv(GL_MAX_VARYING_FLOATS, &maxVaryingFloats);
    std::cout << "Maximum number of varying floats: " << maxVaryingFloats << std::endl;

    GLint maxVertexUniformComponents = 0;
    glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
    std::cout << "Maximum combined vertex uniform components: " << maxVertexUniformComponents << std::endl;

    GLint maxGeometryUniformComponents = 0;
    glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, &maxGeometryUniformComponents);
    std::cout << "Maximum combined geometry uniform components: " << maxGeometryUniformComponents << std::endl;

    GLint maxFragmentUniformComponents = 0;
    glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
    std::cout << "Maximum combined fragment uniform components: " << maxFragmentUniformComponents << std::endl;
    std::cout << std::endl;

    // Additional OpenGL capabilities
    std::cout << "==== OpenGL Additional Capabilities ====" << std::endl;

    GLint maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    std::cout << "Maximum texture units: " << maxTextureUnits << std::endl;

    GLint maxTextureLodBias = 0;
    glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, &maxTextureLodBias);
    std::cout << "Maximum texture LOD bias: " << maxTextureLodBias << std::endl;

    GLint maxRenderbufferSize = 0;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
    std::cout << "Maximum renderbuffer size: " << maxRenderbufferSize << "x" << maxRenderbufferSize << " pixels"
              << std::endl;

    GLint maxDrawBuffers = 0;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
    std::cout << "Maximum number of draw buffers: " << maxDrawBuffers << std::endl;

    GLint maxColorAttachments = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
    std::cout << "Maximum number of color attachments: " << maxColorAttachments << std::endl;

    GLint maxSampleMaskWords = 0;
    glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &maxSampleMaskWords);
    std::cout << "Maximum sample mask words: " << maxSampleMaskWords << std::endl;

    GLint maxTransformFeedbackInterleavedComponents = 0;
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, &maxTransformFeedbackInterleavedComponents);
    std::cout << "Maximum transform feedback interleaved components: " << maxTransformFeedbackInterleavedComponents
              << std::endl;

    // OpenGL Extensions
    std::cout << "==== OpenGL Extensions ====" << std::endl;
    GLint numExtensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    for (GLint i = 0; i < numExtensions; ++i) {
        const char *extension = (const char *)glGetStringi(GL_EXTENSIONS, i);
        std::cout << extension << std::endl;
    }
    std::cout << std::endl;
}

static void error_callback(int error, const char *description) { fprintf(stderr, "Error: %s\n", description); }

void Window::toggle_mouse_mode() {
    if (cursor_is_disabled) {
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    cursor_is_disabled = !cursor_is_disabled;
}

void Window::disable_cursor() {
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cursor_is_disabled = true;
}
void Window::enable_cursor() {
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cursor_is_disabled = false;
}

void Window::set_cursor_pos(double xpos, double ypos) { glfwSetCursorPos(glfw_window, 400.0, 300.0); }

std::tuple<double, double> Window::convert_point_from_2d_screen_space_to_2d_normalized_screen_space(double x,
                                                                                                    double y) {
    int width, height;
    glfwGetWindowSize(glfw_window, &width, &height);
    return {(2.0f * x) / width - 1.0f, 1.0f - (2.0f * y) / height};
}

std::tuple<double, double> Window::get_corrective_aspect_ratio_scale() {
    auto [aspect_ratio_x, aspect_ratio_y] = get_aspect_ratio_in_simplest_terms();
    float aspect = static_cast<float>(aspect_ratio_x) / static_cast<float>(aspect_ratio_y);
    double x_scale = 1, y_scale = 1;
    if (aspect > 1.0f) {
        // wider: shader shrinks x => visible area extends further in x
        x_scale = aspect;
    } else {
        // taller: shader shrinks y => visible area extends further in y
        y_scale = 1 / aspect;
    }
    return {x_scale, y_scale};
}

std::tuple<double, double>
Window::convert_point_from_2d_screen_space_to_2d_aspect_corrected_normalized_screen_space(double x, double y) {
    auto [nssx, nssy] = convert_point_from_2d_screen_space_to_2d_normalized_screen_space(x, y);
    auto [carsx, carsy] = get_corrective_aspect_ratio_scale();
    return {nssx * carsx, nssy * carsy};
}

void Window::enable_wireframe_mode() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
void Window::disable_wireframe_mode() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

void Window::toggle_fullscreen() {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    // Get the current window's position and size
    int window_x, window_y, window_width, window_height;
    glfwGetWindowPos(glfw_window, &window_x, &window_y);
    glfwGetWindowSize(glfw_window, &window_width, &window_height);

    if (window_in_fullscreen) {
        // If the window is currently fullscreen, switch to windowed mode
        glfwSetWindowMonitor(glfw_window, nullptr, window_x, window_y, window_width, window_height, 0);
    } else {
        // If the window is not fullscreen, switch to fullscreen mode
        // Use the monitor's native resolution (as given by the GLFWvidmode)
        glfwSetWindowMonitor(glfw_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    window_in_fullscreen = !window_in_fullscreen; // Toggle fullscreen state
}

void Window::enable_fullscreen() {
    if (window_in_fullscreen)
        return;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwGetWindowPos(glfw_window, &top_left_corner_of_window_x, &top_left_corner_of_window_y);

    glfwSetWindowMonitor(glfw_window, monitor, 0, 0, width_px, height_px, mode->refreshRate);
    window_in_fullscreen = true;
}

void Window::disable_fullscreen() {
    bool window_is_windowed = not window_in_fullscreen;
    if (window_is_windowed)
        return;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(glfw_window, nullptr, top_left_corner_of_window_x, top_left_corner_of_window_y, width_px,
                         height_px, 0);

    window_in_fullscreen = false; // Toggle fullscreen state
}

#include <sstream>

std::optional<std::pair<int, int>> parse_aspect_ratio(const std::string &aspect_ratio) {
    std::istringstream ss(aspect_ratio);
    int w, h;
    char sep;
    if (ss >> w >> sep >> h && sep == ':' && h != 0) {
        return std::make_pair(w, h);
    }
    return std::nullopt;
}

std::vector<VideoMode> get_available_video_modes(GLFWmonitor *monitor,
                                                 const std::optional<std::string> &aspect_ratio = std::nullopt) {
    std::vector<VideoMode> modes_out;

    if (!monitor) {
        std::cerr << "Invalid monitor pointer.\n";
        return modes_out;
    }

    std::optional<std::pair<int, int>> parsed_ratio;
    if (aspect_ratio) {
        parsed_ratio = parse_aspect_ratio(*aspect_ratio);
    }

    int count;
    const GLFWvidmode *modes = glfwGetVideoModes(monitor, &count);

    for (int i = 0; i < count; ++i) {
        int width = modes[i].width;
        int height = modes[i].height;

        // If filtering by aspect ratio
        if (parsed_ratio) {
            const auto &[target_w, target_h] = *parsed_ratio;
            if (width * target_h != height * target_w) {
                continue;
            }
        }

        modes_out.push_back({width, height, modes[i].refreshRate});
    }

    return modes_out;
}

std::vector<std::string> video_modes_to_resolutions(const std::vector<VideoMode> &video_modes) {
    std::vector<std::string> resolutions;
    std::unordered_set<std::string> seen;

    for (const auto &mode : video_modes) {
        std::string res = std::to_string(mode.width) + "x" + std::to_string(mode.height);
        if (seen.insert(res).second) {
            resolutions.push_back(res);
        }
    }

    return resolutions;
}

std::vector<std::string> get_available_resolutions(const std::optional<std::string> &aspect_ratio) {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cerr << "Failed to get primary monitor.\n";
        return {};
    }

    auto filtered_modes = get_available_video_modes(monitor, aspect_ratio);
    return video_modes_to_resolutions(filtered_modes);
}

void Window::set_resolution(const std::string &resolution) {
    size_t x_pos = resolution.find('x');
    unsigned int width, height;
    if (x_pos != std::string::npos) {
        width = std::stoi(resolution.substr(0, x_pos));
        height = std::stoi(resolution.substr(x_pos + 1));
        width_px = width;
        height_px = height;
        glfwSetWindowSize(glfw_window, width, height);
    } else {
        throw std::invalid_argument("Input string is not in the correct format (e.g. 1280x960)");
    }
}

void Window::set_fullscreen_by_on_off(const std::string &on_off_string) {
    // TODO: if value is on / off we call window.enable/disable_fullscreen accordingly.
    if (on_off_string == "on") {
        enable_fullscreen();
    } else if (on_off_string == "off") {
        disable_fullscreen();
    } else {
        std::cout << "Invalid value for fullscreen: {}" << on_off_string << std::endl;
    }
}
