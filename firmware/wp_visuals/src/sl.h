#ifndef SL_H
#define SL_H

#include <cmath>
#include <cstdint>

#ifdef __amd64__
#include <smmintrin.h>
#include <xmmintrin.h>
#endif /* __amd64__ */

// TODO:
// constexpr cos, sin, tan
// constexpr sqrt

namespace sl {

//////////////////////////////////////////////////////////////////////////////

static constexpr float pi = 3.141592653589793f;
static constexpr float tau = 2 * pi;

//////////////////////////////////////////////////////////////////////////////

struct ivec2 {
  int32_t x, y;
};

struct ivec3 {
  int32_t x, y, z;
};

struct ivec4 {
  int32_t x, y, z, w;

  // XXX
  operator int32_t *() {
    return reinterpret_cast<int32_t *>(this);
  }
};

////////////////////////////////////////

struct vec2 {
  float x, y;
};

struct vec3 {
  float x, y, z;
};

struct vec4 {
  float x, y, z, w;

  // XXX
  operator float *() {
    return reinterpret_cast<float *>(this);
  }
};

////////////////////////////////////////

struct mat4 {
  float e00, e01, e02, e03;
  float e10, e11, e12, e13;
  float e20, e21, e22, e23;
  float e30, e31, e32, e33;
};

//////////////////////////////////////////////////////////////////////////////

inline constexpr float max(float x, float y) {
  return x < y ? y : x;
}

inline constexpr float min(float x, float y) {
  return x < y ? x : y;
}

#ifdef __amd64__
// TODO(mastensg): constexpr
inline float floor(float x) {
  __m128 _x = *reinterpret_cast<__m128 *>(&x);
  _x = _mm_floor_ss(_x, _x);  // NOLINT
  return *reinterpret_cast<float *>(&_x);
}

// TODO(mastensg): constexpr
inline float sqrt(float x) {
  __m128 _x = *reinterpret_cast<__m128 *>(&x);
  _x = _mm_sqrt_ss(_x);
  return *reinterpret_cast<float *>(&_x);
}

// TODO(mastensg): std::abs already declared. but why does that matter for
// sl::abs?
// TODO(mastensg): constexpr
#if 1
#else
inline float my_abs(float x) {
  __m128i _x = *reinterpret_cast<__m128i *>(&x);
  _x = _mm_slli_epi32(_x, 1);
  _x = _mm_srli_epi32(_x, 1);
  return *reinterpret_cast<float *>(&_x);
}
#endif
#endif /* __amd64__ */

#ifdef __ARM_ARCH_7EM__
// TODO(mastensg): untested
inline constexpr float floor(float x) {
  uint32_t i;
  i = static_cast<uint32_t>(x);
  x = static_cast<float>(i);
  return x;
}

// TODO(mastensg): untested
inline constexpr float sqrt(float x) {
  __asm__ __volatile__("vsqrt.f32 %0, %1" : "+w"(x) : "w"(x));
  return x;
}

// TODO(mastensg): untested
inline constexpr float abs(float x) {
  __asm__ __volatile__("vabs.f32 %0, %1" : "+w"(x) : "w"(x));
  return x;
}
#endif /* __ARM_ARCH_7EM__ */

// glsl's mod, which is different from c's fmod
// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/mod.xhtml
// TODO(mastensg): constexpr
inline float mod(float x, float y) {
  return x - y * floor(x / y);
}

inline constexpr float radians(float degrees) {
  return degrees * tau / 360.0f;
}

inline constexpr float clamp(float x, float lo, float hi) {
  return min(max(lo, x), hi);
}
inline constexpr int clamp(int x, int lo, int hi) {
  return min(max(lo, x), hi);
}

inline constexpr float clamp01(float x) {
  return min(max(0.0f, x), 1.0f);
}

// TODO(mastensg): constexpr
inline float inversesqrt(float x) {
  const float threehalfs = 1.5f;

  float x2 = x * 0.5f;
  auto i = reinterpret_cast<uint32_t *>(&x);
  *i = 0x5f3759df - (*i >> 1u);
  auto y = reinterpret_cast<float *>(i);
  *y = *y * (threehalfs - (x2 * *y * *y));

  return *y;
}

inline float smootherstep(float edge0, float edge1, float x) {
  // Scale, and clamp x to 0..1 range
  x = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  // Evaluate polynomial
  return x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f);
}

// ivec2  ////////////////////////////////////////////////////////////////////

inline constexpr ivec2 to_ivec2(ivec3 v) {
  return ivec2{v.x, v.y};
}

inline constexpr ivec2 to_ivec2(ivec4 v) {
  return ivec2{v.x, v.y};
}

inline constexpr ivec2 to_ivec2(vec2 v) {
  return ivec2{static_cast<int32_t>(v.x), static_cast<int32_t>(v.y)};
}

inline constexpr ivec2 operator+(ivec2 l, ivec2 r) {
  return ivec2{l.x + r.x, l.y + r.y};
}

inline constexpr ivec2 operator-(ivec2 l, ivec2 r) {
  return ivec2{l.x - r.x, l.y - r.y};
}

// ivec3  ////////////////////////////////////////////////////////////////////

inline constexpr ivec3 to_ivec3(ivec4 v) {
  return ivec3{v.x, v.y, v.z};
}

inline constexpr ivec3 to_ivec3(vec3 v) {
  return ivec3{static_cast<int32_t>(v.x), static_cast<int32_t>(v.y),
               static_cast<int32_t>(v.z)};
}

inline constexpr ivec3 operator+(ivec3 l, ivec3 r) {
  return ivec3{l.x + r.x, l.y + r.y, l.z + r.z};
}

inline constexpr ivec3 operator-(ivec3 l, ivec3 r) {
  return ivec3{l.x - r.x, l.y - r.y, l.z - r.z};
}

inline constexpr ivec3 operator*(int32_t l, ivec3 r) {
  return ivec3{l * r.x, l * r.y, l * r.z};
}
inline constexpr ivec3 operator*(ivec3 l, int32_t r) {
  return r * l;
}

// ivec4  ////////////////////////////////////////////////////////////////////

inline constexpr ivec4 to_ivec4(vec4 v) {
  return ivec4{static_cast<int32_t>(v.x), static_cast<int32_t>(v.y),
               static_cast<int32_t>(v.z), static_cast<int32_t>(v.w)};
}

// vec2  /////////////////////////////////////////////////////////////////////

inline constexpr vec2 to_vec2(ivec2 v) {
  return vec2{static_cast<float>(v.x), static_cast<float>(v.y)};
}

inline constexpr vec2 to_vec2(vec3 v) {
  return vec2{v.x, v.y};
}

inline constexpr vec2 to_vec2(vec4 v) {
  return vec2{v.x, v.y};
}

inline constexpr vec2 operator+(vec2 l, vec2 r) {
  return vec2{l.x + r.x, l.y + r.y};
}

inline constexpr vec2 operator-(vec2 l, vec2 r) {
  return vec2{l.x - r.x, l.y - r.y};
}

inline constexpr vec2 operator*(float l, vec2 r) {
  return vec2{l * r.x, l * r.y};
}
inline constexpr vec2 operator*(vec2 l, float r) {
  return r * l;
}

// vec3  /////////////////////////////////////////////////////////////////////

inline constexpr vec3 to_vec3(vec4 v) {
  return vec3{v.x, v.y, v.z};
}

inline constexpr vec3 to_vec3(ivec3 v) {
  return vec3{static_cast<float>(v.x), static_cast<float>(v.y),
              static_cast<float>(v.z)};
}

inline constexpr vec3 operator+(vec3 l, vec3 r) {
  return vec3{l.x + r.x, l.y + r.y, l.z + r.z};
}

inline constexpr vec3 operator-(vec3 l, vec3 r) {
  return vec3{l.x - r.x, l.y - r.y, l.z - r.z};
}

inline constexpr vec3 operator*(float l, vec3 r) {
  return vec3{l * r.x, l * r.y, l * r.z};
}
inline constexpr vec3 operator*(vec3 l, float r) {
  return r * l;
}

// TODO(mastensg): constexpr
inline vec3 normalize(vec3 v) {
  float dp = v.x * v.x + v.y * v.y + v.z * v.z;
  float dp_is = inversesqrt(dp);
  return dp_is * v;
}

// vec4  /////////////////////////////////////////////////////////////////////

inline constexpr vec4 to_vec4(ivec4 v) {
  return vec4{static_cast<float>(v.x), static_cast<float>(v.y),
              static_cast<float>(v.z), static_cast<float>(v.w)};
}

inline constexpr const float *data(const vec4 *v) {
  return static_cast<const float *>(static_cast<const void *>(v));
}

inline constexpr vec4 point(vec3 v) {
  return vec4{v.x, v.y, v.z, 1.0f};
}

inline constexpr vec4 vector(vec3 v) {
  return vec4{v.x, v.y, v.z, 0.0f};
}

// mat4  /////////////////////////////////////////////////////////////////////

#if 0
inline constexpr std::array<float,16> const *array(const mat4 *m) {
  return static_cast<const float *>(static_cast<const void *>(m));
}
#endif

inline constexpr const float *data(const mat4 *m) {
  return static_cast<const float *>(static_cast<const void *>(m));
}

inline constexpr mat4 operator*(mat4 l, mat4 r) {
  return mat4{
      l.e00 * r.e00 + l.e01 * r.e10 + l.e02 * r.e20 + l.e03 * r.e30,
      l.e00 * r.e01 + l.e01 * r.e11 + l.e02 * r.e21 + l.e03 * r.e31,
      l.e00 * r.e02 + l.e01 * r.e12 + l.e02 * r.e22 + l.e03 * r.e32,
      l.e00 * r.e03 + l.e01 * r.e13 + l.e02 * r.e23 + l.e03 * r.e33,

      l.e10 * r.e00 + l.e11 * r.e10 + l.e12 * r.e20 + l.e13 * r.e30,
      l.e10 * r.e01 + l.e11 * r.e11 + l.e12 * r.e21 + l.e13 * r.e31,
      l.e10 * r.e02 + l.e11 * r.e12 + l.e12 * r.e22 + l.e13 * r.e32,
      l.e10 * r.e03 + l.e11 * r.e13 + l.e12 * r.e23 + l.e13 * r.e33,

      l.e20 * r.e00 + l.e21 * r.e10 + l.e22 * r.e20 + l.e23 * r.e30,
      l.e20 * r.e01 + l.e21 * r.e11 + l.e22 * r.e21 + l.e23 * r.e31,
      l.e20 * r.e02 + l.e21 * r.e12 + l.e22 * r.e22 + l.e23 * r.e32,
      l.e20 * r.e03 + l.e21 * r.e13 + l.e22 * r.e23 + l.e23 * r.e33,

      l.e30 * r.e00 + l.e31 * r.e10 + l.e32 * r.e20 + l.e33 * r.e30,
      l.e30 * r.e01 + l.e31 * r.e11 + l.e32 * r.e21 + l.e33 * r.e31,
      l.e30 * r.e02 + l.e31 * r.e12 + l.e32 * r.e22 + l.e33 * r.e32,
      l.e30 * r.e03 + l.e31 * r.e13 + l.e32 * r.e23 + l.e33 * r.e33,
  };
}

inline constexpr vec4 operator*(mat4 l, vec4 r) {
  return vec4{
      l.e00 * r.x + l.e01 * r.y + l.e02 * r.z + l.e03 * r.w,
      l.e10 * r.x + l.e11 * r.y + l.e12 * r.z + l.e13 * r.w,
      l.e20 * r.x + l.e21 * r.y + l.e22 * r.z + l.e23 * r.w,
      l.e30 * r.x + l.e31 * r.y + l.e32 * r.z + l.e33 * r.w,
  };
}

inline constexpr mat4 identity() {
  return mat4{
      1.0f, 0.0f, 0.0f, 0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      0.0f, 0.0f, 1.0f, 0.0f,  //
      0.0f, 0.0f, 0.0f, 1.0f,
  };
}

inline constexpr mat4 transposed(mat4 m) {
  return mat4{
      m.e00, m.e10, m.e20, m.e30,  //
      m.e01, m.e11, m.e21, m.e31,  //
      m.e02, m.e12, m.e22, m.e32,  //
      m.e03, m.e13, m.e23, m.e33,
  };
}

inline constexpr mat4 scaling(float f) {
  return mat4{
      f,    0.0f, 0.0f, 0.0f,  //
      0.0f, f,    0.0f, 0.0f,  //
      0.0f, 0.0f, f,    0.0f,  //
      0.0f, 0.0f, 0.0f, 1.0f,
  };
}
inline constexpr mat4 scaling(vec3 v) {
  return mat4{
      v.x,  0.0f, 0.0f, 0.0f,  //
      0.0f, v.y,  0.0f, 0.0f,  //
      0.0f, 0.0f, v.z,  0.0f,  //
      0.0f, 0.0f, 0.0f, 1.0f,
  };
}

inline constexpr mat4 translation(vec3 v) {
  return mat4{
      1.0f, 0.0f, 0.0f, 0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      0.0f, 0.0f, 1.0f, 0.0f,  //
      v.x,  v.y,  v.z,  1.0f,
  };
}

inline /*constexpr*/ mat4 x_rotation(float a) {
  float c = std::cos(a);
  float s = std::sin(a);

  return mat4{
      1.0f, 0.0f, 0.0f, 0.0f,  //
      0.0f, c,    -s,   0.0f,  //
      0.0f, s,    c,    0.0f,  //
      0.0f, 0.0f, 0.0f, 1.0f,
  };
}

inline /*constexpr*/ mat4 y_rotation(float a) {
  float c = std::cos(a);
  float s = std::sin(a);

  return mat4{
      c,    0.0f, s,    0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      -s,   0.0f, c,    0.0f,  //
      0.0f, 0.0f, 0.0f, 1.0f,
  };
}

inline /*constexpr*/ mat4 z_rotation(float a) {
  float c = std::cos(a);
  float s = std::sin(a);

  return mat4{
      c,    -s,   0.0f, 0.0f,  //
      s,    c,    0.0f, 0.0f,  //
      0.0f, 0.0f, 1.0f, 0.0f,  //
      0.0f, 0.0f, 0.0f, 1.0f,
  };
}

// TODO(mastensg)
#if 0
inline constexpr mat4 rotation(vec3 n, float a) {
  return mat4{
      1.0f, 0.0f, 0.0f, 0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      0.0f, 0.0f, 1.0f, 0.0f,  //
      0.0f, 0.0f, 0.0f, 1.0f,
  };
}
#endif

inline /*constexpr*/ mat4 perspective(float fov, float aspect, float near,
                                      float far) {
  float tf = std::tan(0.5f * fov);
  float atf = aspect * tf;
  float fpn_fmn = (far + near) / (far - near);
  float tfn_fmn = -2.0f * far * near / (far - near);

#if 0
  return mat4{
      1.0f / atf, 0.0f,      0.0f,     0.0f,   //
      0.0f,       1.0f / tf, 0.0f,     0.0f,   //
      0.0f,       0.0f,      -fpn_fmn, -1.0f,  //
      0.0f,       0.0f,      tfn_fmn,  0.0f,
  };
#else
  return mat4{
      1.0f / atf, 0.0f,      0.0f,     0.0f,     //
      0.0f,       1.0f / tf, 0.0f,     0.0f,     //
      0.0f,       0.0f,      -fpn_fmn, tfn_fmn,  //
      0.0f,       0.0f,      -1.0f,    0.0f,
  };
#endif
}

//////////////////////////////////////////////////////////////////////////////

inline constexpr sl::mat4 zoom_to_fit(sl::vec2 image, sl::vec2 frame) {
  float image_aspect = image.x / image.y;
  float frame_aspect = frame.x / frame.y;

  if (image_aspect < frame_aspect) {
    return sl::scaling(sl::vec3{image_aspect / frame_aspect, 1.0f, 1.0f});
  } else {
    return sl::scaling(sl::vec3{1.0f, frame_aspect / image_aspect, 1.0f});
  }
}

inline constexpr sl::mat4 gravitation(sl::mat4 t, sl::vec3 g) {
  sl::mat4 tg = sl::translation(sl::vec3{
      g.x * (1.0f / t.e00 - 1.0f),  //
      g.y * (1.0f / t.e11 - 1.0f),  //
      g.z * (1.0f / t.e22 - 1.0f),  //
  });

  return tg;
}

//////////////////////////////////////////////////////////////////////////////

inline constexpr float dot(vec2 l, vec2 r) {
  return l.x * r.x + l.y * r.y;
}

inline constexpr float dot(vec3 l, vec3 r) {
  return l.x * r.x + l.y * r.y + l.z * r.z;
}

inline constexpr float dot(vec4 l, vec4 r) {
  return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}

////////////////////////////////////////

inline /*constexpr*/ float length(vec2 v) {
  return std::sqrt(dot(v, v));
}

inline /*constexpr*/ float length(vec3 v) {
  return std::sqrt(dot(v, v));
}

inline /*constexpr*/ float length(vec4 v) {
  return std::sqrt(dot(v, v));
}

////////////////////////////////////////

}  // namespace sl

#endif  // SL_H
