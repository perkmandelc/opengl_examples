#include <SDL.h>
#include <assert.h>
#include <SDL_opengl.h>
#include <GL/gl.h>

#define SDL_CHECK(funcall) if ((funcall) < 0) { printf("sdl error: %s\n", SDL_GetError()); assert(0); }
#define SDL_CHECK_VAR(var) if (!var) { printf("sdl error: %s\n", SDL_GetError()); assert(0); }

typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);

typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);

PFNGLCREATESHADERPROC glCreateShader_f;
PFNGLSHADERSOURCEPROC glShaderSource_f;
PFNGLCOMPILESHADERPROC glCompileShader_f;
PFNGLATTACHSHADERPROC glAttachShader_f;
PFNGLDELETEPROGRAMPROC glDeleteProgram_f;
PFNGLCREATEPROGRAMPROC glCreateProgram_f;
PFNGLLINKPROGRAMPROC glLinkProgram_f;
PFNGLVALIDATEPROGRAMPROC glValidateProgram_f;
PFNGLGETPROGRAMIVPROC glGetProgramiv_f;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog_f;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog_f;
PFNGLDELETESHADERPROC glDeleteShader_f;

PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation_f;
PFNGLUSEPROGRAMPROC glUseProgram_f;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray_f;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation_f;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer_f;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray_f;


#define str(s) #s
#define LOAD_GL_FUNCTION(name) do { name ## _ ## f = SDL_GL_GetProcAddress(str(name)); assert(name ## _ ## f); } while (0)
#define GL_CHECK(stmt) do {                                             \
        stmt;                                                           \
        GLuint error = glGetError();                                  \
        if (error == GL_INVALID_ENUM) {                                 \
            printf("L%d: GL_INVALID_ENUM\n", __LINE__);                 \
        }                                                               \
        else if (error == GL_INVALID_VALUE) {                           \
            printf("L%d: GL_INVALID_VALUE\n", __LINE__);                \
        }                                                               \
        else if (error == GL_INVALID_OPERATION) {                       \
            printf("L%d: GL_INVALID_OPERATION\n", __LINE__);            \
        }                                                               \
        else if (error == GL_STACK_OVERFLOW) {                          \
            printf("L%d: GL_STACK_OVERFLOW\n", __LINE__);               \
        }                                                               \
        else if (error == GL_STACK_UNDERFLOW) {                         \
            printf("L%d: GL_STACK_UNDERFLOW\n", __LINE__);              \
        }                                                               \
        else if (error == GL_OUT_OF_MEMORY) {                           \
            printf("L%d: GL_OUT_OF_MEMORY\n", __LINE__);                \
        }                                                               \
        else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {           \
            printf("L%d: GL_INVALID_FRAMEBUFFER_OPERATION\n", __LINE__); \
        }                                                               \
        else if (error == GL_TABLE_TOO_LARGE) {                         \
            printf("L%d: GL_TABLE_TOO_LARGE\n", __LINE__);              \
        }                                                               \
        else if (error != GL_NO_ERROR) {                                \
            printf("L%d unknown error %d\n", __LINE__, error);          \
        }                                                               \
    } while (0)

static char *vertex_shader_source =
    "#version 130\n"
    "in vec2 a_position;\n"
    "in vec3 a_color;\n"
    "out vec3 out_color;\n"
    "void main() {\n"
    "out_color = a_color;\n"
    "gl_Position = vec4(a_position, 0.0f, 1.0f);\n"
    "}";

static char *fragment_shader_source =
    "#version 130\n"
    "in vec3 out_color;\n"
    "void main() {\n"
    "gl_FragColor = vec4(out_color, 1.0f);\n"
    "}";

static void print_opengl_info(void) {
    int major_version;
    int minor_version;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version);
    printf("opengl %d.%d\n", major_version, minor_version);
    printf("%s\n", glGetString(GL_VERSION));
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_GLContext gl_context = NULL;

#if 1
    int window_flags = 0;
#else
    int window_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    int window_width = 800;
    int window_height = 600;

    int window_x = SDL_WINDOWPOS_UNDEFINED;
    int window_y = SDL_WINDOWPOS_UNDEFINED;
    
    assert(SDL_Init(SDL_INIT_VIDEO) >= 0);

    {
        window = SDL_CreateWindow("window title",
                                  window_x, window_y,
                                  window_width, window_height, window_flags | SDL_WINDOW_OPENGL);
        SDL_CHECK_VAR(window);
    }

    {
        gl_context = SDL_GL_CreateContext(window);
        SDL_CHECK_VAR(gl_context);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        print_opengl_info();
        
        glViewport(0, 0, window_width, window_height);
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    int shader_program_id = -1;
    int position_attrib_id = -1;
    int color_attrib_id = -1;
    
    {
        LOAD_GL_FUNCTION(glCreateShader);
        LOAD_GL_FUNCTION(glShaderSource);
        LOAD_GL_FUNCTION(glCompileShader);
        LOAD_GL_FUNCTION(glAttachShader);
        LOAD_GL_FUNCTION(glCreateProgram);
        LOAD_GL_FUNCTION(glDeleteProgram);
        LOAD_GL_FUNCTION(glLinkProgram);
        LOAD_GL_FUNCTION(glValidateProgram);
        LOAD_GL_FUNCTION(glGetProgramiv);
        LOAD_GL_FUNCTION(glGetShaderInfoLog);
        LOAD_GL_FUNCTION(glGetProgramInfoLog);
        LOAD_GL_FUNCTION(glDeleteShader);

        LOAD_GL_FUNCTION(glGetAttribLocation);
        LOAD_GL_FUNCTION(glUseProgram);
        LOAD_GL_FUNCTION(glEnableVertexAttribArray);
        LOAD_GL_FUNCTION(glBindAttribLocation);
        LOAD_GL_FUNCTION(glVertexAttribPointer);
        LOAD_GL_FUNCTION(glDisableVertexAttribArray);

        int vs_id, fs_id, program_id;
        int num_program_sources = 1;
        int vertex_shader_source_length = strlen(vertex_shader_source);
        int fragment_shader_source_length = strlen(fragment_shader_source);

        printf("Compiling shaders...\n");
        
        GL_CHECK(vs_id = glCreateShader_f(GL_VERTEX_SHADER));
        GL_CHECK(glShaderSource_f(vs_id, num_program_sources, (const GLchar **)&vertex_shader_source, &vertex_shader_source_length));
        GL_CHECK(glCompileShader_f(vs_id));
    
        GL_CHECK(fs_id = glCreateShader_f(GL_FRAGMENT_SHADER));
        GL_CHECK(glShaderSource_f(fs_id, num_program_sources, (const GLchar **)&fragment_shader_source, &fragment_shader_source_length));
        GL_CHECK(glCompileShader_f(fs_id));
        
        GL_CHECK(program_id = glCreateProgram_f());
        GL_CHECK(glAttachShader_f(program_id, vs_id));
        GL_CHECK(glAttachShader_f(program_id, fs_id));
        GL_CHECK(glLinkProgram_f(program_id));

        GLint program_linked = 0;
        //GL_CHECK(glValidateProgram_f(program_id));
        GL_CHECK(glGetProgramiv_f(program_id, GL_LINK_STATUS, &program_linked));

        GL_CHECK(glDeleteShader_f(vs_id));
        GL_CHECK(glDeleteShader_f(fs_id));

        if (program_linked) {
            GL_CHECK(position_attrib_id = glGetAttribLocation_f(program_id, "a_position"));
            GL_CHECK(color_attrib_id = glGetAttribLocation_f(program_id, "a_color"));
        }

        {
            GLsizei errors_length;
            char errors[4096];
        
            GL_CHECK(glGetShaderInfoLog_f(vs_id, sizeof(errors), &errors_length, errors));
            if (errors_length > 0) {
                printf("%s\n", errors);
            }

            GL_CHECK(glGetShaderInfoLog_f(fs_id, sizeof(errors), &errors_length, errors));
            if (errors_length > 0) {
                printf("%s\n", errors);
            }

            GL_CHECK(glGetProgramInfoLog_f(program_id, sizeof(errors), &errors_length, errors));
            if (errors_length > 0) {
                printf("%s\n", errors);
            }
        }

        shader_program_id = program_id;
    }

    while (1) {
        {
            SDL_Event e;
            SDL_PollEvent(&e);
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        
        float triangle_vertex_data[] = {-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
        glClear(GL_COLOR_BUFFER_BIT);
        GL_CHECK(glUseProgram_f(shader_program_id));
        
        int num_position_elements = 2;
        int num_color_elements = 3;
        int vertex_stride = (num_position_elements + num_color_elements)*sizeof(float);

        GL_CHECK(glEnableVertexAttribArray_f(position_attrib_id));
        GL_CHECK(glEnableVertexAttribArray_f(color_attrib_id));

        GL_CHECK(glVertexAttribPointer_f(position_attrib_id,
                                         num_position_elements,
                                         GL_FLOAT,
                                         GL_FALSE,
                                         vertex_stride,
                                         (void *)&triangle_vertex_data[0]));
        GL_CHECK(glVertexAttribPointer_f(color_attrib_id,
                                         num_color_elements,
                                         GL_FLOAT,
                                         GL_FALSE,
                                         vertex_stride,
                                         (void *)&triangle_vertex_data[2]));

        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));

        GL_CHECK(glDisableVertexAttribArray_f(position_attrib_id));
        GL_CHECK(glDisableVertexAttribArray_f(color_attrib_id));

        SDL_GL_SwapWindow(window);
        SDL_Delay(30);
    }

    return 0;
}
