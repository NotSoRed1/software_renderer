#pragma once

#include <stdint.h>
#include <math.h>
#include <xmmintrin.h>

typedef uint8_t             u8;
typedef uint16_t            u16;
typedef uint32_t            u32;
typedef uint64_t            u64;

typedef int8_t              i8;
typedef int16_t             i16;
typedef int32_t             i32;
typedef int64_t             i64;

typedef u64                 usize;
typedef i64                 isize;

typedef float               f32;
typedef double              f64;

typedef uintptr_t           uptr;
typedef intptr_t            iptr;


#define RM_EP               0.0001f

struct vec2 {
    union {
        struct {
            f32 x;
            f32 y;
        };
        struct {
            f32 r;
            f32 g;
        };
        struct {
            f32 elements[2];
        };
    };

    vec2() {};
    vec2(f32 s) { x = s; y = s; };
    vec2(f32 _x, f32 _y) { x = _x; y =_y; };
    vec2(const vec2& v) { x = v.x; y = v.y; };
};

inline bool operator==(const vec2& lhs, const vec2& rhs) {
    return lhs.x == rhs.x 
        && lhs.y == rhs.y;
}

inline bool operator!=(const vec2& lhs, const vec2& rhs) {
    return !(lhs == rhs);
}

inline vec2 operator-(const vec2& rhs) {
    return vec2(-rhs.x, -rhs.y);
}

inline vec2 operator+(const vec2& lhs, const vec2& rhs) {
    vec2 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;

    return result;
}


inline vec2 operator+(const vec2& lhs, f32 rhs) {
    vec2 result;
    result.x = lhs.x + rhs;
    result.y = lhs.y + rhs;

    return result;
}

inline vec2& operator+=(vec2& lhs, const vec2& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;

    return lhs;
}

inline vec2& operator+=(vec2& lhs, f32 rhs) {
    lhs.x += rhs;
    lhs.y += rhs;

    return lhs;
}

inline vec2 operator-(const vec2& lhs, const vec2& rhs) {
    vec2 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;

    return result;
}

inline vec2 operator-(const vec2& lhs, f32 rhs) {
    vec2 result;
    result.x = lhs.x - rhs;
    result.y = lhs.y - rhs;

    return result;
}

inline vec2& operator-=(vec2& lhs, const vec2& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;

    return lhs;
}

inline vec2& operator-=(vec2& lhs, f32 rhs) {
    lhs.x -= rhs;
    lhs.y -= rhs;

    return lhs;
}

inline vec2 operator*(const vec2& lhs, f32 rhs) {
    vec2 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;

    return result;
}

inline vec2& operator*=(vec2& lhs, f32 rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;

    return lhs;
}

inline vec2 operator*(const vec2& lhs, const vec2& rhs) {
    vec2 result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;

    return result;
}

inline vec2& operator*=(vec2& lhs, const vec2& rhs) {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;

    return lhs;
}

inline vec2 operator/(const vec2& lhs, f32 rhs) {
    vec2 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;

    return result;
}

inline vec2& operator/=(vec2& lhs, f32 rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;

    return lhs;
}

struct vec3 {
    union {
        struct {
            f32 elements[3];
        };
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
        struct {
            vec2 xy;
            f32 _;
        };
        struct {
            f32 __;
            vec2 yz;
        };
        struct {
            f32 r;
            f32 g;
            f32 b;
        };
        struct {
            vec2 rg;
            f32 ___;
        };
        struct {
            f32 ____;
            vec2 gb;
        };
    };

    vec3() {};
    vec3(f32 s) { x = s; y = s; z = s; };
    vec3(f32 _x, f32 _y, f32 _z) { x = _x; y = _y; z = _z; };
    vec3(const vec2 &_xy, f32 _z) { xy = _xy; z = _z; };
    vec3(f32 _x, const vec2 &_yz) { x = _x; yz = _yz; };
    vec3(const vec3& v) { x = v.x; y = v.y; z = v.z; };
};

inline bool operator==(const vec3& lhs, const vec3& rhs) {
    return lhs.x == rhs.x 
        && lhs.y == rhs.y 
        && lhs.z == rhs.z;
}

inline bool operator!=(const vec3& lhs, const vec3& rhs) {
    return !(lhs == rhs);
}

inline vec3 operator-(const vec3& rhs) {
    return vec3(-rhs.x, -rhs.y, -rhs.z);
}

inline vec3 operator+(const vec3& lhs, const vec3& rhs) {
    vec3 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;

    return result;
}

inline vec3 operator+(const vec3& lhs, f32 rhs) {
    vec3 result;
    result.x = lhs.x + rhs;
    result.y = lhs.y + rhs;
    result.z = lhs.z + rhs;

    return result;
}

inline vec3& operator+=(vec3& lhs, const vec3& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;

    return lhs;
}

inline vec3& operator+=(vec3& lhs, f32 rhs) {
    lhs.x += rhs;
    lhs.y += rhs;
    lhs.z += rhs;

    return lhs;
}

inline vec3 operator-(const vec3& lhs, const vec3& rhs) {
    vec3 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;

    return result;
}

inline vec3 operator-(const vec3& lhs, f32 rhs) {
    vec3 result;
    result.x = lhs.x - rhs;
    result.y = lhs.y - rhs;
    result.z = lhs.z - rhs;

    return result;
}

inline vec3& operator-=(vec3& lhs, const vec3& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;

    return lhs;
}

inline vec3& operator-=(vec3& lhs, f32 rhs) {
    lhs.x -= rhs;
    lhs.y -= rhs;
    lhs.z -= rhs;

    return lhs;
}

inline vec3 operator*(const vec3& lhs, f32 rhs) {
    vec3 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    result.z = lhs.z * rhs;

    return result;
}

inline vec3 operator*(f32 lhs, const vec3& rhs) {
    vec3 result;
    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    result.z = lhs * rhs.z;

    return result;
}

inline vec3& operator*=(vec3& lhs, f32 rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;

    return lhs;
}

inline vec3 operator*(const vec3& lhs, const vec3& rhs) {
    vec3 result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    result.z = lhs.z * rhs.z;

    return result;
}

inline vec3& operator*=(vec3& lhs, const vec3& rhs) {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;

    return lhs;
}

inline vec3 operator/(const vec3& lhs, f32 rhs) {
    vec3 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;
    result.z = lhs.z / rhs;

    return result;
}

inline vec3& operator/=(vec3& lhs, f32 rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;

    return lhs;
}

struct vec4 {
    union {
        struct {
            f32 elements[4];
        };
        struct {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };
        struct {
            vec3 xyz;
            f32 _;
        };
        struct {
            vec2 xy;
            vec2 zw;
        };
        struct {
            f32 __;
            vec3 yzw;
        };

        struct {
            f32 r;
            f32 g;
            f32 b;
            f32 a;
        };
        struct {
            vec3 rgb;
            f32 ___;
        };
        struct {
            vec2 rg;
            vec2 ba;
        };
        struct {
            f32 ____;
            vec3 gba;
        };
    };
    vec4() {};
    vec4(f32 s) { x = s; y = s; z = s; w = s; };
    vec4(f32 _x, f32 _y, f32 _z, f32 _w) { x = _x; y = _y; z = _z; w = _w; };
    vec4(const vec2 &_xy, f32 _z, f32 _w) { xy = _xy; z = _z, w = _w; };
    vec4(const vec3 &_xyz, f32 _w) { xyz = _xyz; w = _w; };
    vec4(f32 _x, const vec3 &_yzw) { x = _x; yzw = _yzw; };
    vec4(const vec4& v) { x = v.x; y = v.y; z = v.z; w = v.w; };
};



inline bool operator==(const vec4& lhs, const vec4& rhs) {
    return lhs.x == rhs.x 
        && lhs.y == rhs.y 
        && lhs.z == rhs.z 
        && lhs.w == rhs.w;
}

inline bool operator!=(const vec4& lhs, const vec4& rhs) {
    return !(lhs == rhs);
}

inline vec4 operator-(const vec4& rhs) {
    return vec4(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

inline vec4 operator+(const vec4& lhs, const vec4& rhs) {
    vec4 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    result.w = lhs.w + rhs.w;

    return result;
}

inline vec4 operator+(const vec4& lhs, f32 rhs) {
    vec4 result;
    result.x = lhs.x + rhs;
    result.y = lhs.y + rhs;
    result.z = lhs.z + rhs;
    result.w = lhs.w + rhs;

    return result;
}

inline vec4& operator+=(vec4& lhs, const vec4& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;

    return lhs;
}

inline vec4& operator+=(vec4& lhs, f32 rhs) {
    lhs.x += rhs;
    lhs.y += rhs;
    lhs.z += rhs;
    lhs.w += rhs;

    return lhs;
}

inline vec4 operator-(const vec4& lhs, const vec4& rhs) {
    vec4 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    result.w = lhs.w - rhs.w;

    return result;
}

inline vec4 operator-(const vec4& lhs, f32 rhs) {
    vec4 result;
    result.x = lhs.x - rhs;
    result.y = lhs.y - rhs;
    result.z = lhs.z - rhs;
    result.w = lhs.w - rhs;

    return result;
}

inline vec4& operator-=(vec4& lhs, const vec4& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;

    return lhs;
}

inline vec4& operator-=(vec4& lhs, f32 rhs) {
    lhs.x -= rhs;
    lhs.y -= rhs;
    lhs.z -= rhs;
    lhs.w -= rhs;

    return lhs;
}

inline vec4 operator*(const vec4& lhs, f32 rhs) {
    vec4 result;

    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    result.z = lhs.z * rhs;
    result.w = lhs.w * rhs;

    return result;
}

inline vec4& operator*=(vec4& lhs, f32 rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    lhs.w *= rhs;

    return lhs;
}


inline vec4 operator*(const vec4& lhs, const vec4& rhs) {
    vec4 result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    result.z = lhs.z * rhs.z;
    result.w = lhs.w * rhs.w;

    return result;
}

inline vec4& operator*=(vec4& lhs, const vec4& rhs) {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    lhs.w *= rhs.w;

    return lhs;
}

inline vec4 operator/(const vec4& lhs, f32 rhs) {
    vec4 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;
    result.z = lhs.z / rhs;
    result.w = lhs.w / rhs;

    return result;
}

inline vec4& operator/=(vec4& lhs, f32 rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    lhs.w /= rhs;

    return lhs;
}


struct Ray {
    vec3 origin;
    vec3 direction;

    vec3 at(f32 t) const {
        return direction * t + origin;
    }
};

inline u32 pack_vec4_to_u32(vec4 in) {

    u32 r = (u32)(in.r * 255.0f);
    u32 g = (u32)(in.g * 255.0f);
    u32 b = (u32)(in.b * 255.0f);
    u32 a = (u32)(in.a * 255.0f);

    return r << 24 | g << 16 | b << 8 | a;
}

inline u32 rgba_to_argb(u32 in) {
    return in >> 8 | (in >> 0 & 0xff << 24);
}

inline vec4 rgba_to_argb(vec4 in) {
    vec4 out;

    out.r = in.a;
    out.g = in.r;
    out.b = in.g;
    out.a = in.b;

    return out;
}


inline f32 length_squared(const vec2& v) { return v.x * v.x + v.y * v.y; }
inline f32 length_squared(const vec3& v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
inline f32 length_squared(const vec4& v) { return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; }

inline f32 length(const vec2& v) {return sqrtf(length_squared(v)); }
inline f32 length(const vec3& v) {return sqrtf(length_squared(v)); }
inline f32 length(const vec4& v) {return sqrtf(length_squared(v)); }

inline vec2 normalize(const vec2& v) { return v / length(v); }
inline vec3 normalize(const vec3& v) { return v / length(v); }
inline vec4 normalize(const vec4& v) { return v / length(v); }

inline f32 dot(const vec2& a, const vec2& b) {
    return a.x * b.x + a.y * b.y; 
}

inline f32 dot(const vec3& a, const vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z; 
}

inline f32 dot(const vec4& a, const vec4& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; 
}

inline vec3 cross(const vec3& x, const vec3& y) {
    return vec3(
        x.y * y.z - y.y * x.z,
        x.z * y.x - y.z * x.x,
        x.x * y.y - y.x * x.y);
}


inline f32 linear_to_srgb(f32 c) { 
    return c > 0.0031308 ? 1.055f * powf(c, 1.0f/2.4f) - 0.055f : c * 12.92f; 
}


inline f32 srgb_to_linear(f32 c) { 
    return c > 0.04045 ? powf((c + 0.055f) / 1.055f, 2.4f) : c / 12.92f; 
}


inline vec4 linear_to_srgb(const vec4& c) { 
    vec4 result;

    result.x = linear_to_srgb(c.x);
    result.y = linear_to_srgb(c.y);
    result.z = linear_to_srgb(c.z);
    result.w = linear_to_srgb(c.w);

    return result;
}


inline vec4 srgb_to_linear(const vec4& c) {
    vec4 result;

    result.x = srgb_to_linear(c.x);
    result.y = srgb_to_linear(c.y);
    result.z = srgb_to_linear(c.z);
    result.w = srgb_to_linear(c.w);

    return result;
}


inline f32 gamma2_2(f32 c) {
    // return pow(c, 1.0 / 2.2);
    return 1.138f * sqrt(c) - 0.138f * c;
}


inline vec3 gamma2_2(const vec3& c) {
    vec3 result;
    result.x = gamma2_2(c.x);
    result.y = gamma2_2(c.y);
    result.z = gamma2_2(c.z);

    return result;
}


inline vec4 gamma2_2(const vec4& c) {
    vec4 result;
    result.x = gamma2_2(c.x);
    result.y = gamma2_2(c.y);
    result.z = gamma2_2(c.z);
    result.w = gamma2_2(c.w);

    return result;
}

#define PI 3.1415926535


inline f32 max(f32 a, f32 b) {
    return  a > b ? a : b; 
}

inline vec2 max(const vec2 &a, const vec2 &b) {
    return vec2(
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y );
}

inline vec3 max(const vec3 &a, const vec3 &b) {
    return vec3(
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y,
        a.z > b.z ? a.z : b.z );
}

inline vec4 max(const vec4 &a, const vec4 &b) {
    return vec4(
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y,
        a.z > b.z ? a.z : b.z,
        a.w > b.w ? a.w : b.w );
}

inline f32 min(f32 a, f32 b) {
    return  a < b ? a : b; 
}

inline vec2 min(const vec2 &a, const vec2 &b) {
    return vec2(
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y );
}

inline vec3 min(const vec3 &a, const vec3 &b) {
    return vec3(
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.z < b.z ? a.z : b.z );
}

inline vec4 min(const vec4 &a, const vec4 &b) {
    return vec4(
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.z < b.z ? a.z : b.z,
        a.w < b.w ? a.w : b.w );
}

inline f32 clamp(f32 v, f32 min, f32 max) { 
    return v < min ? min : v > max ? max : v; 
}

inline vec2 clamp(const vec2 &v, f32 min, f32 max) { 
    return vec2(
        clamp(v.x, min, max),
        clamp(v.y, min, max));
}

inline vec3 clamp(const vec3 &v, f32 min, f32 max) { 
    return vec3(
        clamp(v.x, min, max),
        clamp(v.y, min, max),
        clamp(v.z, min, max));
}

inline vec4 clamp(const vec4 &v, f32 min, f32 max) { 
    return vec4(
        clamp(v.x, min, max), 
        clamp(v.y, min, max), 
        clamp(v.z, min, max), 
        clamp(v.w, min, max)); 
}

inline f32 clamp01(f32 v) { 
    return clamp(v, 0.0f, 1.0f); 
}

inline vec2 clamp01(const vec2 &v) { 
    return clamp(v, 0.0f, 1.0f);
}

inline vec3 clamp01(const vec3 &v) { 
    return clamp(v, 0.0f, 1.0f);
}

inline vec4 clamp01(const vec4 &v) { 
    return clamp(v, 0.0f, 1.0f);
}


inline f32 radians(f32 a) { 
    return a * (f32)PI / 180.0f; 
}

inline f32 degrees(f32 a) { 
    return a * 180.0f / (f32)PI; 
}


inline f32 lerp(f32 a, f32 b, f32 t) {
    return a + (b - a) * t; 
}

inline vec2 lerp(const vec2& a, const vec2& b, f32 t) { 
    return a + (b - a) * t; 
}

inline vec3 lerp(const vec3& a, const vec3& b, f32 t) { 
    return a + (b - a) * t; 
}

inline vec4 lerp(const vec4& a, const vec4& b, f32 t) { 
    return a + (b - a) * t; 
}


inline f32 lerp_clamped(f32 a, f32 b, f32 t) {
    return a + (b - a) * clamp01(t); 
}

inline vec2 lerp_clamped(const vec2& a, const vec2& b, f32 t) { 
    return a + (b - a) * clamp01(t); 
}

inline vec3 lerp_clamped(const vec3& a, const vec3& b, f32 t) { 
    return a + (b - a) * clamp01(t); 
}

inline vec4 lerp_clamped(const vec4& a, const vec4& b, f32 t) { 
    return a + (b - a) * clamp01(t); 
}

inline f32 smoothstep(f32 edge0, f32 edge1, f32 x) { 
    if (x < edge0) return 0;
    if (x >= edge1) return 1;

    x = (x - edge0) / (edge1 - edge0);
   return x * x * (3 - 2 * x);
}

inline vec2 reflect(const vec2& dir , const vec2& normal) {
    return dir - (normal * dot(dir, normal)) * 2.0f;
}

inline vec3 reflect(const vec3& dir , const vec3& normal) {
    return dir - (normal * dot(dir, normal)) * 2.0f;
}

inline vec4 reflect(const vec4& dir , const vec4& normal) {
    return dir - (normal * dot(dir, normal)) * 2.0f;
}


inline f32 rm_sqrt(f32 v) {return sqrtf(v); }
inline vec2 rm_sqrt(const vec2& v) { return {rm_sqrt(v.x), rm_sqrt(v.y)}; }
inline vec3 rm_sqrt(const vec3& v) { return {rm_sqrt(v.x), rm_sqrt(v.y), rm_sqrt(v.z)}; }
inline vec4 rm_sqrt(const vec4& v) { return {rm_sqrt(v.x), rm_sqrt(v.y), rm_sqrt(v.z), rm_sqrt(v.w)}; }

// inline f32 pow(f32 a, f32 b) {return powf(a, b); }
// inline vec2 pow(const vec2& v, f32 b) { return {pow(v.x, b), pow(v.y, b)}; }
// inline vec3 pow(const vec3& v, f32 b) { return {pow(v.x, b), pow(v.y, b), pow(v.z, b)}; }
// inline vec4 pow(const vec4& v, f32 b) { return {pow(v.x, b), pow(v.y, b), pow(v.z, b), pow(v.w, b)}; }

// inline f32 log(f32 v) {return logf(v); }
// inline vec2 log(const vec2& v) { return {log(v.x), log(v.y)}; }
// inline vec3 log(const vec3& v) { return {log(v.x), log(v.y), log(v.z)}; }
// inline vec4 log(const vec4& v) { return {log(v.x), log(v.y), log(v.z), log(v.w)}; }

// inline f32 exp(f32 v) {return expf(v); }
// inline vec2 exp(const vec2& v) { return {exp(v.x), exp(v.y)}; }
// inline vec3 exp(const vec3& v) { return {exp(v.x), exp(v.y), exp(v.z)}; }
// inline vec4 exp(const vec4& v) { return {exp(v.x), exp(v.y), exp(v.z), exp(v.w)}; }

// inline f32 round(f32 v) {return roundf(v); }
// inline vec2 round(const vec2& v) { return {round(v.x), round(v.y)}; }
// inline vec3 round(const vec3& v) { return {round(v.x), round(v.y), round(v.z)}; }
// inline vec4 round(const vec4& v) { return {round(v.x), round(v.y), round(v.z), round(v.w)}; }

inline vec4 bilinear(
   const float &tx, 
   const float  &ty, 
   const vec4 &c1, 
   const vec4 &c2,
   const vec4 &c3,
   const vec4 &c4) {

    vec4 a = c1 * (1 - tx) + c2 * tx;
    vec4 b = c3 * (1 - tx) + c4 * tx;
    return a * (1 - ty) + b * ty;
}


inline f32 map_range(f32 value, f32 old_min, f32 old_max, f32 new_min, f32 new_max) {

    f32 old_range = old_max - old_min;
    f32 new_range = new_max - new_min; 

    return ((value - old_min) * new_range / old_range) + new_min;
}


// inline vec2 map_range(vec2 value, vec2 old_min, vec2 old_max, vec2 new_min, vec2 new_max) {

//     vec2 old_range = old_max - old_min;
//     vec2 new_range = new_max - new_min; 

//     return ((value - old_min) * new_range / old_range) + new_min;
// }


// inline vec3 map_range(vec3 value, vec3 old_min, vec3 old_max, vec3 new_min, vec3 new_max) {

//     vec3 old_range = old_max - old_min;
//     vec3 new_range = new_max - new_min; 

//     return ((value - old_min) * new_range / old_range) + new_min;
// }


// inline vec4 map_range(vec4 value, vec4 old_min, vec4 old_max, vec4 new_min, vec4 new_max) {

//     vec4 old_range = old_max - old_min;
//     vec4 new_range = new_max - new_min; 

//     return ((value - old_min) * new_range / old_range) + new_min;
// }


struct mat4 {
    union {
        struct {
            f32 els[16];
        };
        struct {
            f32 elements[4][4];
        };
        struct {
            vec4 rows[4];
        };
    };

    mat4(f32 s = 1.0f) {
        for (int y = 0; y < 4 * 4; y++)
            els[y] = 0.0f;
        rows[0].x = s;
        rows[1].y = s;
        rows[2].z = s;
        rows[3].w = s;
    }

    mat4(const mat4& other) {
        for (int y = 0; y < 4 * 4; y++) {
            els[y] = other.els[y];
        }
    }
};



struct mat3 {
    union {
        struct {
            f32 els[9];
        };
        struct {
            f32 elements[3][3];
        };
        struct {
            vec3 rows[3];
        };
    };

    mat3(f32 s = 1) {
        rows[0].x = s;
        rows[1].y = s;
        rows[2].z = s;
    }

    mat3(const mat3& other) {
        rows[0] = other.rows[0];
        rows[1] = other.rows[1];
        rows[2] = other.rows[2];
    }

    mat3(const mat4 &other) {
        rows[0] = other.rows[0].xyz;
        rows[1] = other.rows[1].xyz;
        rows[2] = other.rows[2].xyz;
    }

};


struct mat2 {
    union {
        struct {
            f32 els[4];
        };
        struct {
            f32 elements[2][2];
        };
        struct {
            vec2 rows[2];
        };
    };


    mat2(f32 s = 1) {
        rows[0].x = s;
        rows[1].y = s;
    }

    mat2(const mat2& other) {
        rows[0] = other.rows[0];
        rows[1] = other.rows[1];
    }

    mat2(const mat3& other) {
        rows[0] = other.rows[0].xy;
        rows[1] = other.rows[1].xy;
    }
};



inline vec3 operator*(const mat3 lhs, const vec3& rhs) {
    vec3 result(0.0f);


    for (u32 y = 0; y < 3; y++)
        for (u32 x = 0; x < 1; x++)
            for (u32 k = 0; k < 3; k++)
                result.elements[y] += lhs.els[k * 3 + y] * rhs.elements[k];


    return result;
}


inline mat3 operator*(const mat3& lhs, const mat3& rhs) {
    mat3 result(0.0f);


    for (u32 y = 0; y < 3; y++)
        for (u32 x = 0; x < 3; x++)
            for (u32 k = 0; k < 3; k++)
                result.els[y * 3 + x] += lhs.els[k * 3 + x] * rhs.els[y * 3 + k];


    return result;
}


inline vec4 operator*(const mat4& lhs, const vec4& rhs) {
    vec4 result = 0.0f;

    for (u32 y = 0; y < 4; y++)
        for (u32 x = 0; x < 1; x++)
            for (u32 k = 0; k < 4; k++)
                result.elements[y] += lhs.els[k * 4 + y] * rhs.elements[k];


    return result;
}


inline mat4 operator*(const mat4& lhs, const mat4& rhs) {
    mat4 result(0.0f);


    for (u32 y = 0; y < 4; y++)
        for (u32 x = 0; x < 4; x++)
            for (u32 k = 0; k < 4; k++)
                result.els[y * 4 + x] += lhs.els[k * 4 + x] * rhs.els[y * 4 + k];


    return result;
}


inline mat2 operator/(const mat2& lhs, f32 rhs) {
    mat2 result(0.0f);

    for (u32 i = 0; i < 4; i++) 
        result.els[i] = lhs.els[i] / rhs;

    return result;
}


inline mat3 operator/(const mat3& lhs, f32 rhs) {
    mat3 result(0.0f);

    for (u32 i = 0; i < 9; i++) 
        result.els[i] = lhs.els[i] / rhs;

    return result;
}


inline mat4 operator/(const mat4 & lhs, f32 rhs) {
    mat4 result(0.0f);

    for (u32 i = 0; i < 16; i++) 
        result.els[i] = lhs.els[i] / rhs;

    return result;
}

inline f32 determinant(const mat2& target)
{
    return  target.els[0] * target.els[3] -
            target.els[1] * target.els[2];
} 


inline f32 determinant(const mat3& target)
{
    return  target.elements[0][0] * (target.elements[1][1] * target.elements[2][2] - target.elements[1][2] * target.elements[2][1]) -
            target.elements[0][1] * (target.elements[1][0] * target.elements[2][2] - target.elements[2][0] * target.elements[1][2]) +
            target.elements[0][2] * (target.elements[1][0] * target.elements[2][1] - target.elements[2][0] * target.elements[1][1]) ;
} 


inline mat2 inverse(const mat2& target)
{
    mat2 result(0.0f);

    f32 det = determinant(target);
    result = result / det;

    return result;
}


inline mat3 inverse(const mat3& target)
{
    mat3 result(0.0f);

    f32 det = determinant(target);
    result = result / det;

    return result;
}


inline mat4 look_at(
    const vec3 &pos, 
    const vec3 &target, 
    const vec3 &up = vec3(0.0f, 1.0f, 0.0f)) 
{

    vec3 forward      = normalize(target - pos);
    vec3 side         = normalize(cross(up, forward));
    vec3 new_up       = cross(forward, side); 

    mat4 result(1.0f);

    result.elements[0][0] = side.x;
    result.elements[1][0] = side.y;
    result.elements[2][0] = side.z;

    result.elements[0][1] = new_up.x;
    result.elements[1][1] = new_up.y;
    result.elements[2][1] = new_up.z;

    result.elements[0][2] = forward.x;
    result.elements[1][2] = forward.y;
    result.elements[2][2] = forward.z;


    result.rows[3].x   = -dot(side, pos);
    result.rows[3].y   = -dot(new_up, pos);
    result.rows[3].z   = -dot(forward, pos);
    
    return result;
}


inline mat4 translate(const mat4& target, const vec3 &v) {
    mat4 result(target);

    // result.rows[3].xyz += v;
    result.rows[3].xyz = target.rows[0].xyz * v.x 
                        + target.rows[1].xyz * v.y 
                        + target.rows[2].xyz * v.z;

    return result;
}

inline mat4 scale(const mat4& target, f32 scale)
{
    mat4 result(target);

    result.rows[0] *= scale;
    result.rows[1] *= scale;
    result.rows[2] *= scale;

    return result;
}

inline mat4 rotate(const mat4& target, f32 angle, const vec3& v)
{
    f32 c    = cos(angle);
    f32 s    = sin(angle);
    f32 t    = 1.0f - c;

    vec3 axis(normalize(v));
    mat4 result;

    result.rows[0].x = t * axis.x * axis.x + c;
    result.rows[0].y = t * axis.x * axis.y + s * axis.z;
    result.rows[0].z = t * axis.x * axis.z - s * axis.y;

    result.rows[1].x = t * axis.x * axis.y - s * axis.z;
    result.rows[1].y = t * axis.y * axis.y + c;
    result.rows[1].z = t * axis.y * axis.z + s * axis.x;

    result.rows[2].x = t * axis.x * axis.z + s * axis.y;
    result.rows[2].y = t * axis.y * axis.z - s * axis.x;
    result.rows[2].z = t * axis.z * axis.z + c;

    result.rows[3]   = vec4(0.0f, 0.0f, 0.0f, 1.0f); 

    return target * result;
}


inline mat4 perspective(f32 fov, f32 aspect, f32 near, f32 far)
{
    mat4 result(0.0f);

    f32 half_tan = tan(fov / 2.0f);

    result.rows[0].x = 1.0f / (aspect * half_tan);
    result.rows[1].y = 1.0f / half_tan;
    result.rows[2].z = (far + near) / (far - near);
    result.rows[2].w = 1.0f;
    result.rows[3].z = -(2.0f * far * near) / (far - near);

    return result;
}


