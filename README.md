# Info

A glfw window 

```cpp
#include <iostream>

#include "graphics/batcher/generated/batcher.hpp"
#include "graphics/shader_cache/shader_cache.hpp"
#include "graphics/shader_standard/shader_standard.hpp"
#include "graphics/vertex_geometry/vertex_geometry.hpp"
#include "graphics/window/window.hpp"

int main() {
    Window window(700, 700, "mwe_grid_font");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // default is GL_LESS

    std::vector<ShaderType> rs = {ShaderType::ABSOLUTE_POSITION_WITH_COLORED_VERTEX};
    ShaderCache shader_cache(rs);

    Batcher batcher(shader_cache);

    shader_cache.set_uniform(ShaderType::ABSOLUTE_POSITION_WITH_COLORED_VERTEX, ShaderUniformVariable::ASPECT_RATIO,
                             glm::vec2(1, 1));

    draw_info::IndexedVertexPositions rect = vertex_geometry::generate_rectangle(0, 0, 1, 1);
    std::vector<glm::vec3> cs(rect.xyz_positions.size(), glm::vec3(0.5, 0.5, 0.5));

    while (!glfwWindowShouldClose(window.glfw_window)) {

        if (glfwGetKey(window.glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window.glfw_window, true);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // dark blue-gray background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        batcher.absolute_position_with_colored_vertex_shader_batcher.queue_draw(0, rect.indices, rect.xyz_positions,
                                                                                cs);
        batcher.absolute_position_with_colored_vertex_shader_batcher.draw_everything();

        glfwSwapBuffers(window.glfw_window);

        glfwPollEvents();
    }

    return 0;
}
```
