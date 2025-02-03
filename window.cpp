#include "window.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

static void on_window_size_change(GLFWwindow *window, int width, int height);
static void error_callback(int error, const char *description);

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
GLFWwindow *Window::initialize_glfw_glad_and_return_window(unsigned int &window_width_px,
                                                           unsigned int &window_height_px, const char *window_name,
                                                           bool start_in_fullscreen, bool start_with_mouse_captured,
                                                           bool vsync, bool print_out_opengl_data) {

    cursor_is_grabbed = start_with_mouse_captured;

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
        window_width_px = mode->width;
        window_height_px = mode->height;
        glfw_window = glfwCreateWindow(window_width_px, window_height_px, window_name, monitor, NULL);
    } else {
        glfw_window = glfwCreateWindow(window_width_px, window_height_px, window_name, NULL, NULL);
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

    // NOTE for the other callbacks you have to set them yourself.
    glfwSetFramebufferSizeCallback(glfw_window, on_window_size_change);

    return glfw_window;
}

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
    if (cursor_is_grabbed) {
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    cursor_is_grabbed = !cursor_is_grabbed;
}

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

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void on_window_size_change(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
