#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

char *read_file(char *path)
{
    FILE *fp = fopen(path, "rb");

    if (fp == 0)
    {
        printf("Could not find file: \"%s\"\n", path);
        exit(-1);
    }

    long size;
    char *text;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    text = malloc(size * sizeof(char)+1);
    

    fread(text, 1, size, fp);
    text[size] = 0;

    fclose(fp);

    return text;
}

int main()
{
    GLFWwindow *window;

    glfwSetErrorCallback(error_callback);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Mandelbrot", 0, 0);

    // glad_set_post_callback(_post_call_callback_default);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    // OpenGL resource initialization
    const char *vertex_shader_text = read_file("src/shaders/mandelbrot.vert");
    const char *fragment_shader_text = read_file("src/shaders/mandelbrot.frag");

    printf("%s\n", vertex_shader_text);
    printf("%s\n", fragment_shader_text);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, 0);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, 0);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    typedef struct View_s{
        float view_scale_x;
        float view_scale_y;
        float view_offset_x;
        float view_offset_y;
        int resolution;
    } View;

    GLuint view_ubo;
    glGenBuffers(1, &view_ubo);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, view_ubo, 0, sizeof(View));

    View view = {1.0f, 1.0f, -0.5f, 0.0f, 10};
    float zoom = 1.0;
    const float dt = 1.0/120.0;
    float turbo = 10.0;

    while (!glfwWindowShouldClose(window))
    {
        turbo = 1.0;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            turbo = 2.0;
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            view.view_offset_y += turbo * zoom * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            view.view_offset_y -= turbo * zoom * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_A)) {
            view.view_offset_x -= turbo * zoom * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            view.view_offset_x += turbo * zoom * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_Q)) {
            zoom *= 1.05 * turbo;
        }
        if (glfwGetKey(window, GLFW_KEY_E)) {
            zoom /= 1.05 * turbo;
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        view.view_scale_x = zoom * (float)width / (float) height;
        view.view_scale_y = zoom;


        glViewport(0, 0, width, height);

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glBindBuffer(GL_UNIFORM_BUFFER, view_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(View), &view, GL_STATIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}