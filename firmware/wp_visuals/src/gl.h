#ifndef GL_H
#define GL_H

#include <cstdio>
#include <cstring>
#include <vector>

#include <system_error>

#ifdef OPENGL_PROFILE_CORE
#include <GL/glew.h>
#endif

#ifdef OPENGL_PROFILE_ES
#include <GLES2/gl2.h>
#endif

namespace gl {

// GL <-> GLES  //////////////////////////////////////////////////////////////

#ifdef OPENGL_PROFILE_CORE
static const GLint RED = GL_RED;
#endif

#ifdef OPENGL_PROFILE_ES
static const GLint RED = GL_LUMINANCE;
#endif

// Buffer  ///////////////////////////////////////////////////////////////////

using Buffer = GLuint;

inline Buffer make_buffer() {
  Buffer b;
  glGenBuffers(1, &b);
  return b;
}

template <typename T>
inline Buffer make_buffer(std::vector<T> vs) {
  Buffer b;
  glGenBuffers(1, &b);

  glBindBuffer(GL_ARRAY_BUFFER, b);
  glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(T), vs.data(),
               GL_DYNAMIC_DRAW);

  return b;
}

inline void delete_buffer(Buffer b) {
  glDeleteBuffers(1, &b);
}

// Texture  //////////////////////////////////////////////////////////////////

using Texture = GLuint;

inline Texture make_texture_2d(GLint min_filter = GL_NEAREST,
                               GLint max_filter = GL_NEAREST,
                               GLint wrap_s = GL_REPEAT,
                               GLint wrap_t = GL_REPEAT) {
  Texture t;
  glGenTextures(1, &t);
  glBindTexture(GL_TEXTURE_2D, t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
  return t;
}

inline void delete_texture(Texture t) {
  glDeleteTextures(1, &t);
}

// Shader  ///////////////////////////////////////////////////////////////////

using Shader = GLuint;

inline Shader make_shader(GLenum type) {
  return glCreateShader(type);
}

inline void delete_shader(Shader s) {
  glDeleteShader(s);
}

inline void print_shader_infolog(Shader s) {
  GLint infolen = 0;
  glGetShaderiv(s, GL_INFO_LOG_LENGTH, &infolen);
  if (0 == infolen) {
    return;
  }

  std::vector<char> infolog(infolen);

  glGetShaderInfoLog(s, infolen, nullptr, infolog.data());

  fprintf(stderr, "shader infolog:\n%s\n", infolog.data());
}

// Program  //////////////////////////////////////////////////////////////////

using Program = GLuint;

inline Program make_program() {
  return glCreateProgram();
}

inline void delete_program(Program p) {
  glDeleteProgram(p);
}

inline void print_program_infolog(Program p) {
  GLint infolen = 0;
  glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infolen);
  if (0 == infolen) {
    return;
  }

  std::vector<char> infolog(infolen);

  glGetProgramInfoLog(p, infolen, nullptr, infolog.data());

  fprintf(stderr, "program infolog:\n%s\n", infolog.data());
}

inline Shader add_shader(Program p, GLenum type, const char *source) {
  GLint length = std::strlen(source);

  Shader s = glCreateShader(type);
  glShaderSource(s, 1, &source, &length);
  glCompileShader(s);

  GLint compile_status;
  glGetShaderiv(s, GL_COMPILE_STATUS, &compile_status);
  if (GL_FALSE == compile_status) {
    print_shader_infolog(s);
    return s;
  }

  glAttachShader(p, s);
  return s;
}

inline Shader add_fragment_shader(Program p, const char *source) {
  return add_shader(p, GL_FRAGMENT_SHADER, source);
}

inline Shader add_vertex_shader(Program p, const char *source) {
  return add_shader(p, GL_VERTEX_SHADER, source);
}

inline Program make_program(const char *vs, const char *fs) {
  Program p = make_program();

  add_vertex_shader(p, vs);
  add_fragment_shader(p, fs);

  glLinkProgram(p);
  {
    GLint link_status;
    glGetProgramiv(p, GL_LINK_STATUS, &link_status);
    if (GL_FALSE == link_status) {
      print_program_infolog(p);
    }
  }

  return p;
}

// Attribute  ////////////////////////////////////////////////////////////////

using Attribute = GLint;

inline Attribute a(Program p, const char *attribute_name) {
  return glGetAttribLocation(p, attribute_name);
}

// Uniform  //////////////////////////////////////////////////////////////////

using Uniform = GLint;

inline Uniform u(Program p, const char *uniform_name) {
  // std::fprintf(stderr, "%8d %s\n", glGetUniformLocation(p, uniform_name),
  // uniform_name);
  return glGetUniformLocation(p, uniform_name);
}

// Array  ////////////////////////////////////////////////////////////////////

#ifdef OPENGL_PROFILE_CORE
using Array = GLuint;

inline Array make_array(Buffer b) {
  Array a;

  glGenVertexArrays(1, &a);
  glBindVertexArray(a);
  glBindBuffer(GL_ARRAY_BUFFER, b);

  return a;
}

inline void delete_array(Array a) {
  glDeleteVertexArrays(1, &a);
}
#endif

// Vertex Attribute Arrays  //////////////////////////////////////////////////

// TODO: maybe offset<float>(o)
inline GLvoid *offset_float(size_t o) {
  return reinterpret_cast<GLvoid *>(o * sizeof(float));
}

struct AttributeDescriptor {
  const char *name;
  GLint size;
  GLenum type;
  GLsizei stride;
  GLvoid *offset;
};

inline void enable(Buffer b, Program p, std::vector<AttributeDescriptor> ads) {
  glBindBuffer(GL_ARRAY_BUFFER, b);

  for (auto &ad : ads) {
    Attribute attrib = a(p, ad.name);
    glEnableVertexAttribArray(attrib);

    glVertexAttribPointer(attrib, ad.size, ad.type, GL_FALSE, ad.stride,
                          ad.offset);
  }
}

// Debug  ////////////////////////////////////////////////////////////////////

inline void debug_callback(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length,
                           const GLchar *message, const void *userParam) {
  (void)type;
  (void)severity;
  (void)length;
  (void)userParam;

  std::fprintf(stderr, "\033[1;34m%d %d %s\033[0m\n", source, id, message);

#ifdef OPENGL_PROFILE_CORE
  if (GL_DEBUG_SEVERITY_HIGH == severity) {
    std::fprintf(stderr, "GL_DEBUG_SEVERITY_HIGH\n");
    // throw std::system_error(0, std::system_category());
  }
#endif
}

// Error  ////////////////////////////////////////////////////////////////////

inline const char *strerror(GLint error) {
  switch (error) {
    case GL_NO_ERROR:
      return "GL_NO_ERROR, No error has been recorded.";
      break;
    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM, An unacceptable value is specified for an "
             "enumerated argument. The offending command is ignored and has no "
             "other side effect than to set the error flag.";
      break;

    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE, A numeric argument is out of range. The "
             "offending command is ignored and has no other side effect than "
             "to set the error flag.";
      break;

    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION, The specified operation is not allowed in "
             "the current state. The offending command is ignored and has no "
             "other side effect than to set the error flag.";
      break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "GL_INVALID_FRAMEBUFFER_OPERATION, The command is trying to "
             "render to or read from the framebuffer while the currently bound "
             "framebuffer is not framebuffer complete (i.e. the return value "
             "from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE). "
             "The offending command is ignored and has no other side effect "
             "than to set the error flag.";
      break;

    case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY, There is not enough memory left to execute the "
             "command. The state of the GL is undefined, except for the state "
             "of the error flags, after this error is recorded.";
      break;
    default:
      return "Unknown OpenGL error.";
  }
}

inline void print_error() {
  std::fprintf(stderr, "glError: %s\n", strerror(glGetError()));
}
}  // namespace gl

#endif  // GL_H
