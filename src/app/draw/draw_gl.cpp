#include <app/draw.hpp>
#include <SDL3/SDL_opengl.h>

// @todo

GLenum (GLAPIENTRY *gl_get_error) (void);
void (GLAPIENTRY *gl_clear) (GLbitfield mask);
void (GLAPIENTRY *gl_viewport) (GLint x, GLint y, GLint width, GLint height);
GLuint (GLAPIENTRY *gl_create_shader) (GLenum shader_type);
void (GLAPIENTRY *gl_delete_shader) (GLuint shader);
void (GLAPIENTRY *gl_shader_source) (GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
void (GLAPIENTRY *gl_compile_shader) (GLuint shader);
GLuint (GLAPIENTRY *gl_create_program) (void);
void (GLAPIENTRY *gl_use_program) (GLuint program);
void (GLAPIENTRY *gl_link_program) (GLuint program);
void (GLAPIENTRY *gl_attach_shader) (GLuint program, GLuint shader);
GLint (GLAPIENTRY *gl_get_uniform_location) (GLuint program, const GLchar *name);
GLint (GLAPIENTRY *gl_get_attrib_location) (GLuint program, const GLchar *name);
void (GLAPIENTRY *gl_gen_buffers) (GLsizei n, GLuint *buffers);
void (GLAPIENTRY *gl_bind_buffer) (GLenum target, GLuint buffer);
void (GLAPIENTRY *gl_buffer_data) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void (GLAPIENTRY *gl_gen_vertex_arrays) (GLsizei n, GLuint *arrays);
void (GLAPIENTRY *gl_bind_vertex_array) (GLuint array);
void (GLAPIENTRY *gl_enable_vertex_attrib_array) (GLuint index);
void (GLAPIENTRY *gl_vertex_attrib_pointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
void (GLAPIENTRY *gl_uniform_matrix_4fv) (GLint location, GLsizei count, GLboolean transpose, const float* value);
void (GLAPIENTRY *gl_draw_arrays) (GLenum mode, GLint first, GLsizei count);
void (GLAPIENTRY *gl_gen_textures) (GLsizei n, GLuint *textures);
void (GLAPIENTRY *gl_bind_texture) (GLenum target, GLuint texture);
void (GLAPIENTRY *gl_active_texture) (GLenum texture);
void (GLAPIENTRY *gl_tex_image_2d) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
void (GLAPIENTRY *gl_tex_parameteri) (GLenum target, GLenum pname, GLint param);
void (GLAPIENTRY *gl_tex_parameterfv) (GLenum target, GLenum pname, const GLfloat *params);
void (GLAPIENTRY *gl_generate_mipmap) (GLenum target);
void (GLAPIENTRY *gl_generate_texture_mipmap) (GLuint texture);

bool load_gl_functions()
{
    typedef struct {
        SDL_FunctionPointer *pointer;
        const char *name;
    } FunctionMap;

    FunctionMap map[] = {
        {&(SDL_FunctionPointer)gl_get_error, "glGetError"},
        {&(SDL_FunctionPointer)gl_clear, "glClear"},
        {&(SDL_FunctionPointer)gl_viewport, "glViewport"},
        {&(SDL_FunctionPointer)gl_create_shader, "glCreateShader"},
        {&(SDL_FunctionPointer)gl_delete_shader, "glDeleteShader"},
        {&(SDL_FunctionPointer)gl_shader_source, "glShaderSource"},
        {&(SDL_FunctionPointer)gl_compile_shader, "glCompileShader"},
        {&(SDL_FunctionPointer)gl_create_program, "glCreateProgram"},
        {&(SDL_FunctionPointer)gl_link_program, "glLinkProgram"},
        {&(SDL_FunctionPointer)gl_use_program, "glUseProgram"},
        {&(SDL_FunctionPointer)gl_attach_shader, "glAttachShader"},
        {&(SDL_FunctionPointer)gl_get_uniform_location, "glGetUniformLocation"},
        {&(SDL_FunctionPointer)gl_get_attrib_location, "glGetAttribLocation"},
        {&(SDL_FunctionPointer)gl_gen_buffers, "glGenBuffers"},
        {&(SDL_FunctionPointer)gl_bind_buffer, "glBindBuffer"},
        {&(SDL_FunctionPointer)gl_buffer_data, "glBufferData"},
        {&(SDL_FunctionPointer)gl_gen_vertex_arrays, "glGenVertexArrays"},
        {&(SDL_FunctionPointer)gl_bind_vertex_array, "glBindVertexArray"},
        {&(SDL_FunctionPointer)gl_enable_vertex_attrib_array, "glEnableVertexAttribArray"},
        {&(SDL_FunctionPointer)gl_vertex_attrib_pointer, "glVertexAttribPointer"},
        {&(SDL_FunctionPointer)gl_uniform_matrix_4fv, "glUniformMatrix4fv"},
        {&(SDL_FunctionPointer)gl_draw_arrays, "glDrawArrays"},
        {&(SDL_FunctionPointer)gl_gen_textures, "glGenTextures"},
        {&(SDL_FunctionPointer)gl_bind_texture, "glBindTexture"},
        {&(SDL_FunctionPointer)gl_active_texture, "glActiveTexture"},
        {&(SDL_FunctionPointer)gl_tex_image_2d, "glTexImage2D"},
        {&(SDL_FunctionPointer)gl_tex_parameteri, "glTexParameteri"},
        {&(SDL_FunctionPointer)gl_tex_parameterfv, "glTexParameterfv"},
        {&(SDL_FunctionPointer)gl_generate_mipmap, "glGenerateMipmap"},
        {&(SDL_FunctionPointer)gl_generate_texture_mipmap, "glGenerateTextureMipmap"},
    };

    for (int i = 0; i < ARRAY_SIZE(map); i++)
    {
        *map[i].pointer = SDL_GL_GetProcAddress(map[i].name);
        if (!(*map[i].pointer))
        {
            SDL_Log("Couldn't load: %s. %s", map[i].name, SDL_GetError());
            return false;
        }
    }

    return true;
}
