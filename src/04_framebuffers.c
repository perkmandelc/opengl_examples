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

typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, const GLint value);

typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC) (GLenum texture);

typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);

typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

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

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation_f;
PFNGLUNIFORM1IPROC glUniform1i_f;

PFNGLACTIVETEXTUREPROC glActiveTexture_f;

PFNGLGENBUFFERSPROC glGenBuffers_f;
PFNGLBINDBUFFERPROC glBindBuffer_f;
PFNGLBUFFERDATAPROC glBufferData_f;

PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers_f;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer_f;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample_f;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D_f;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus_f;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers_f;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer_f;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage_f;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer_f;

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
    "in vec2 a_tex_coords;\n"
    "out vec3 out_color;\n"
    "out vec2 out_tex_coords;\n"
    "void main() {\n"
    "out_color = a_color;\n"
    "out_tex_coords = a_tex_coords;\n"
    "gl_Position = vec4(a_position, 0.0f, 1.0f);\n"
    "}";

static char *fragment_shader_source =
    "#version 130\n"
    "uniform sampler2D tex;"
    "in vec3 out_color;\n"
    "in vec2 out_tex_coords;\n"
    "void main() {\n"
    "gl_FragColor = vec4(mix(out_color, texture(tex, out_tex_coords).rgb, 0.5f), 1.0f);\n"
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
    int tex_coords_attrib_id = -1;
    
    { // load opengl functions
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

        LOAD_GL_FUNCTION(glGetUniformLocation);
        LOAD_GL_FUNCTION(glUniform1i);

        LOAD_GL_FUNCTION(glActiveTexture);

        LOAD_GL_FUNCTION(glGenBuffers);
        LOAD_GL_FUNCTION(glBindBuffer);
        LOAD_GL_FUNCTION(glBufferData);

        LOAD_GL_FUNCTION(glGenFramebuffers);
        LOAD_GL_FUNCTION(glBindFramebuffer);
        LOAD_GL_FUNCTION(glFramebufferTexture2D);
        LOAD_GL_FUNCTION(glCheckFramebufferStatus);
        LOAD_GL_FUNCTION(glGenRenderbuffers);
        LOAD_GL_FUNCTION(glBindRenderbuffer);
        LOAD_GL_FUNCTION(glRenderbufferStorage);
        LOAD_GL_FUNCTION(glFramebufferRenderbuffer);
    }

    { // compile shaders
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
            GL_CHECK(tex_coords_attrib_id = glGetAttribLocation_f(program_id, "a_tex_coords"));
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

    int texture_handle = -1;
    { // setup texture
        SDL_Surface *bmp_surface = SDL_LoadBMP("../src/image.bmp");
        assert(bmp_surface);

        printf("loaded image: w: %d, h: %d\n", bmp_surface->w, bmp_surface->h);
        
        
        GL_CHECK(glGenTextures(1, (GLuint *)&texture_handle));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_handle));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                              bmp_surface->w, bmp_surface->h, 0,
                              GL_BGRA_EXT, GL_UNSIGNED_BYTE, bmp_surface->pixels));

        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GL_CHECK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));
    }

    int fbo_handle = -1;
    int fbo_texture_handle = -1;
    { // setup the fbo
        int rbo_handle = -1;
        
        GL_CHECK(glGenFramebuffers_f(1, &fbo_handle));
        GL_CHECK(glBindFramebuffer_f(GL_FRAMEBUFFER, fbo_handle));

        GL_CHECK(glGenRenderbuffers_f(1, &rbo_handle));
        GL_CHECK(glBindRenderbuffer_f(GL_RENDERBUFFER, rbo_handle));

        GL_CHECK(glGenTextures(1, (GLuint *)&fbo_texture_handle));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, fbo_texture_handle));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                              window_width, window_height, 0,
                              GL_BGRA_EXT, GL_UNSIGNED_BYTE, NULL));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        GL_CHECK(glFramebufferTexture2D_f(
                     GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                     GL_TEXTURE_2D, fbo_texture_handle, 0));

        GL_CHECK(glRenderbufferStorage_f(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F,
                                         window_width, window_height));

        assert(glCheckFramebufferStatus_f(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        GL_CHECK(glBindFramebuffer_f(GL_FRAMEBUFFER, 0));
        GL_CHECK(glBindRenderbuffer_f(GL_RENDERBUFFER, 0));
    }

    int vbo_id = -1;
    { // setup vertex buffers
        //                              position    , color           , tex_coords 
        float triangle_vertex_data[] = {-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                                         0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                                         0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                                         1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                                        // screen triangles (for framebuffer rendering
        //                              position    , color           , tex_coords 
                                        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                                         1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                                        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};

        GL_CHECK(glGenBuffers_f(1, &vbo_id));
        GL_CHECK(glBindBuffer_f(GL_ARRAY_BUFFER, vbo_id));
        GL_CHECK(glBufferData_f(GL_ARRAY_BUFFER, sizeof(triangle_vertex_data), triangle_vertex_data, GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer_f(GL_ARRAY_BUFFER, 0));
    }

    while (1) {
        {
            SDL_Event e;
            SDL_PollEvent(&e);
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        GL_CHECK(glBindBuffer_f(GL_ARRAY_BUFFER, vbo_id));
        GL_CHECK(glUseProgram_f(shader_program_id));
        
        int num_position_elements = 2;
        int num_color_elements = 3;
        int num_tex_coords_elements = 2;
        int vertex_stride = (num_position_elements + num_color_elements + num_tex_coords_elements)*sizeof(float);

        GL_CHECK(glEnableVertexAttribArray_f(position_attrib_id));
        GL_CHECK(glEnableVertexAttribArray_f(color_attrib_id));
        GL_CHECK(glEnableVertexAttribArray_f(tex_coords_attrib_id));

        GL_CHECK(glVertexAttribPointer_f(position_attrib_id,
                                         num_position_elements,
                                         GL_FLOAT,
                                         GL_FALSE,
                                         vertex_stride,
                                         (void *)(0*sizeof(GL_FLOAT))));
        GL_CHECK(glVertexAttribPointer_f(color_attrib_id,
                                         num_color_elements,
                                         GL_FLOAT,
                                         GL_FALSE,
                                         vertex_stride,
                                         (void *)(2*sizeof(GL_FLOAT))));
        GL_CHECK(glVertexAttribPointer_f(tex_coords_attrib_id,
                                         num_tex_coords_elements,
                                         GL_FLOAT,
                                         GL_FALSE,
                                         vertex_stride,
                                         (void *)(5*sizeof(GL_FLOAT))));

        int tex_unit_offset = 0;
        int texture_uniform_id = -1;
        GL_CHECK(texture_uniform_id = glGetUniformLocation_f(shader_program_id, "tex"));
        GL_CHECK(glUniform1i_f(texture_uniform_id, tex_unit_offset));
        GL_CHECK(glActiveTexture_f(GL_TEXTURE0 + tex_unit_offset));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_handle));

        {
            GL_CHECK(glBindFramebuffer_f(GL_FRAMEBUFFER, fbo_handle));
            GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
            GL_CHECK(glBindFramebuffer_f(GL_FRAMEBUFFER, 0));
        }

        {
            GL_CHECK(glActiveTexture_f(GL_TEXTURE0 + tex_unit_offset));
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, fbo_texture_handle));
            GL_CHECK(glDrawArrays(GL_TRIANGLES, 6, 6));
        }

        GL_CHECK(glDisableVertexAttribArray_f(position_attrib_id));
        GL_CHECK(glDisableVertexAttribArray_f(color_attrib_id));
        GL_CHECK(glDisableVertexAttribArray_f(tex_coords_attrib_id));

        SDL_GL_SwapWindow(window);
        SDL_Delay(30);
    }

    return 0;
}
