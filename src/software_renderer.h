#ifndef __SOFTWARE_RENDERER_H
#define __SOFTWARE_RENDERER_H




#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef uint8_t                sr_u8;
typedef uint16_t               sr_u16;
typedef uint32_t               sr_u32;
typedef uint64_t               sr_u64;

typedef int8_t                 sr_i8;
typedef int16_t                sr_i16;
typedef int32_t                sr_i32;
typedef int64_t                sr_i64;

typedef sr_u64                 sr_usize;
typedef sr_i64                 sr_isize;

typedef float                  sr_f32;
typedef double                 sr_f64;

typedef uintptr_t              sr_uptr;
typedef intptr_t               sr_iptr;


#define SR_EP                  0.0001f
#define SR_MAX_TEXTURES_SLOTS  32
#define SR_MAX_UNIFORMS_SLOTS  64

#define sr_min(a, b)           (a < b ? a : b)
#define sr_max(a, b)           (a > b ? a : b)

typedef void*                  SrVertex;
typedef void*                  SrVariant;



// ==================================================================
// =========================== MATHS ================================
// ==================================================================


typedef struct {
    sr_f32 x;
    sr_f32 y;

} sr_vec2;

typedef struct {
    sr_f32 x;
    sr_f32 y;
    sr_f32 z;

} sr_vec3;

typedef struct {
    sr_f32 x;
    sr_f32 y;
    sr_f32 z;
    sr_f32 w;

} sr_vec4;






// ==================================================================
// ========================== TEXTURE ===============================
// ==================================================================



typedef enum {
    SR_FORMAT_R    = 1,
    SR_FORMAT_RG   = 2,
    SR_FORMAT_RGB  = 3,
    SR_FORMAT_RGBA = 4,

} SrFormat;



typedef enum {
    SR_FILTER_NEAREST,
    SR_FILTER_BILINEAR,

} SrFilter;



typedef enum {
    SR_SAMPLING_MODE_REPEAT,
    SR_SAMPLING_MODE_CLAMP_TO_EDGE,

} SrSamplingMode;



typedef struct {
    SrFormat          format; 
    SrFilter          filter;
    SrSamplingMode    sampling_mode;
    sr_u32            width;
    sr_u32            height;

} SrTextureSpec;



typedef struct {
    SrTextureSpec spec;
    sr_vec4*    buffer;

} SrTexture;



SrTexture sr_texture_create(SrTextureSpec specs, sr_u8* buffer);


void sr_texture_set_sampling_mode(SrTexture* texture, SrSamplingMode sampling_mode);


void sr_texture_set_filter_mode(SrTexture* texture, SrFilter filter);


void sr_texture_free(SrTexture* texture);


sr_vec4 sr_texture_get_pixel(SrTexture* texture, sr_u32 x, sr_u32 y);


sr_vec4 sr_texture_sample(SrTexture* texture, sr_f32 u, sr_f32 v);





// ==================================================================
// ======================== FRAMEBUFFER =============================
// ==================================================================



// TODO(redone): add support for multiple buffers and formats
typedef struct {
    sr_u32   width;
    sr_u32   height;
    // SrFormat color_format;
    // SrFormat depth_format;

} SrFramebufferSpec;



typedef struct {
    SrFramebufferSpec spec;
    sr_vec4*          color_buffer;
    sr_f32*           depth_buffer;

} SrFramebuffer;



SrFramebuffer sr_framebuffer_create(SrFramebufferSpec spec);


void sr_framebuffer_resize(SrFramebuffer* fb, sr_u32 width, sr_u32 height);


void sr_framebuffer_set_color(SrFramebuffer* fb, sr_u32 x, sr_u32 y, sr_vec4 color);


void sr_framebuffer_set_depth(SrFramebuffer* fb, sr_u32 x, sr_u32 y, sr_f32 value);


void sr_framebuffer_clear_color(SrFramebuffer* fb, sr_vec4 color );


void sr_framebuffer_clear_depth(SrFramebuffer* fb, sr_f32 value);


sr_vec4 sr_framebuffer_get_color(SrFramebuffer* fb, sr_u32 x, sr_u32 y);


sr_f32 sr_framebuffer_get_depth(SrFramebuffer* fb, sr_u32 x, sr_u32 y);


void sr_framebuffer_free(SrFramebuffer* fb);






// ==================================================================
// ========================= PIPELINE ===============================
// ==================================================================



typedef enum {
    SR_POLYGON_MODE_FILL,
    // SR_POLYGON_MODE_LINE,
    // SR_POLYGON_MODE_POINT,

} SrPolygonMode;



typedef enum {
    SR_CULL_MODE_FRONT_FACE,
    SR_CULL_MODE_BACK_FACE,

} SrCullMode;



typedef enum {
    SR_FRONT_FACE_CLOCKWISE,
    SR_FRONT_FACE_COUNTER_CLOCKWISE,

} SrFrontFace;



//TODO(redone): add support for the other primitive types
typedef enum {
    SR_PRIMITIVE_TYPE_TRIANGLE_LIST,
    // SR_PRIMITIVE_TYPE_TRIANGLE_STRIP,
    // SR_PRIMITIVE_TYPE_POINT_LIST,
    // SR_PRIMITIVE_TYPE_LINE_LIST,
    // SR_PRIMITIVE_TYPE_LINE_STRIP,

} SrPrimitiveType;



typedef enum {
    SR_COMPARE_OP_LESS             = 0,
    SR_COMPARE_OP_EQUAL            = 1,
    SR_COMPARE_OP_LESS_OR_EQUAL    = 2,
    SR_COMPARE_OP_GREATER          = 3,
    SR_COMPARE_OP_NOT_EQUAL        = 4,
    SR_COMPARE_OP_GREATER_OR_EQUAL = 5,

} SrCompareOp;



typedef enum {
    SR_BLEND_OP_ADD      = 0,
    SR_BLEND_OP_SUBTRACT = 1,
    SR_BLEND_OP_MIN      = 2,
    SR_BLEND_OP_MAX      = 3,

} SrBlendOp;



typedef enum {
    SR_BLEND_FACTOR_ONE                 = 1,
    SR_BLEND_FACTOR_ZERO                = 0,
    SR_BLEND_FACTOR_SRC_COLOR           = 2,
    SR_BLEND_FACTOR_ONE_MINUS_SRC_COLOR = 3,
    SR_BLEND_FACTOR_DST_COLOR           = 4,
    SR_BLEND_FACTOR_ONE_MINUS_DST_COLOR = 5,
    SR_BLEND_FACTOR_SRC_ALPHA           = 6,
    SR_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA = 7,
    SR_BLEND_FACTOR_DST_ALPHA           = 8,
    SR_BLEND_FACTOR_ONE_MINUS_DST_ALPHA = 9,

} SrBlendFactor;




typedef struct {
    SrPolygonMode polygon_mode;
    SrCullMode    cull_mode;
    SrFrontFace   front_face;
    // sr_f32        line_width;
    // bool          line_smooth;

} SrRasterizerInfo;



typedef struct {
    bool        depth_test_enabled;
    bool        depth_write_enabled;
    SrCompareOp depth_compare_op;
    sr_f32      min_depth;
    sr_f32      max_depth;

} SrDepthInfo;




typedef struct {
    bool        blend_enabled;
    SrBlendFactor src_blend_factor;
    SrBlendFactor dst_blend_factor;
    SrBlendOp     blend_op;

} SrColorBlendInfo;




typedef struct {
    void*       data;
    // const char* name;

} SrUniform;



typedef struct {
    SrTexture*     textures[SR_MAX_TEXTURES_SLOTS];
    SrUniform      uniforms[SR_MAX_UNIFORMS_SLOTS];

} SrGlobalRegistry;



// TODO(redone): add support for multiple variants
// and properly support Variants interpolation
typedef struct {
    sr_usize byte_count;

} SrVariantsInfo;



typedef struct {
    sr_usize byte_count;

} SrVertexInputInfo;



#define VertexFunctionPtr sr_vec4 (*vertex_shader)(SrVertex, SrVariant, SrGlobalRegistry*)
#define PixelFunctionPtr  sr_vec4 (*pixel_shader)(SrVariant, SrGlobalRegistry*)



typedef struct {
    SrPrimitiveType   primitve_type;
    SrRasterizerInfo  rasterizer_info;
    SrDepthInfo       depth_info;
    SrFramebuffer*    framebuffer;
    SrVariantsInfo    variants_info;
    SrVertexInputInfo vertex_input_info;
    SrColorBlendInfo  color_blend_info;
    VertexFunctionPtr; 
    PixelFunctionPtr;

} SrPipelineSpec;



typedef struct {
    SrPipelineSpec     spec;
    SrGlobalRegistry   registry;

} SrPipeline;





SrPipeline sr_create_pipeline(SrPipelineSpec specs);


void sr_pipeline_upload_texture(SrPipeline* pipeline, SrTexture* texture, sr_usize texture_slot);


void sr_pipeline_upload_uniform_buffer(SrPipeline* pipeline, void* data, sr_u32 slot);



#define sr_get_uniform_buffer(type, reg, slot) ( assert(slot < SR_MAX_UNIFORMS_SLOTS), \
                                                (type*)reg->uniforms[slot].data )


#define sr_upload_variant(out, variant) (memcpy(out, &variant, sizeof(variant)))


#define sr_texel(reg, idx, u, v) (sr_texture_sample((reg)->textures[idx], u, v))


void sr_draw(SrPipeline* pipeline, sr_usize vertices_count, void* buff);









// ================================================================
// ================================================================
// ======================= IMPLEMENTATION =========================
// ================================================================
// ================================================================



// #define __SOFTWARE_RENDERER_IMPLEMENTATION
#ifdef __SOFTWARE_RENDERER_IMPLEMENTATION

static sr_vec2 sr_vec2_sub(sr_vec2 lhs, sr_vec2 rhs) {
    return sr_vec2 {
        .x = lhs.x - rhs.x, 
        .y = lhs.y - rhs.y
    };
}



static sr_vec4 sr_vec4_add(sr_vec4 lhs, sr_vec4 rhs) {
    return sr_vec4 {
        .x = lhs.x + rhs.x, 
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z,
        .w = lhs.w + rhs.w
    };
}



static sr_vec4 sr_vec4_mul_s(sr_vec4 lhs, sr_f32 rhs) {
    return sr_vec4 {
        .x = lhs.x * rhs, 
        .y = lhs.y * rhs,
        .z = lhs.z * rhs,
        .w = lhs.w * rhs
    };
}



static sr_f32 sr_clamp(sr_f32 target, sr_f32 min, sr_f32 max) {
    return target < min ? min: target > max ? max : target;
}



static sr_vec4 sr_clamp01_vec4(sr_vec4 v) {
    return sr_vec4 {
        .x = sr_clamp(v.x, 0.0f, 1.0f),
        .y = sr_clamp(v.y, 0.0f, 1.0f),
        .z = sr_clamp(v.z, 0.0f, 1.0f),
        .w = sr_clamp(v.w, 0.0f, 1.0f),
    };
}


static sr_vec4 sr_bilinear(sr_f32 tx, sr_f32 ty, sr_vec4 c1, sr_vec4 c2, sr_vec4 c3, sr_vec4 c4) {

    // a = c1 * (1 - tx) + c2 * tx
    // b = c3 * (1 - tx) + c4 * tx
    // return a * (1 - ty) + b * ty
    sr_vec4 a = sr_vec4_add(sr_vec4_mul_s(c1, 1 - tx), sr_vec4_mul_s(c2, tx));
    sr_vec4 b = sr_vec4_add(sr_vec4_mul_s(c3, 1 - tx), sr_vec4_mul_s(c4, tx));
    return sr_vec4_add(sr_vec4_mul_s(a, 1 - ty), sr_vec4_mul_s(b, ty));

}



static sr_f32 sr_edge_function(sr_vec4 p1, sr_vec4 p2, sr_vec4 p) {
    return (p1.y - p2.y) * p.x + (p2.x - p1.x) * p.y + (p1.x * p2.y - p1.y * p2.x);
}



SrTexture sr_texture_create(SrTextureSpec specs, sr_u8* buffer) {
    SrTexture texture;
    texture.spec = specs;

    texture.buffer = (sr_vec4*)malloc(specs.width * specs.height * sizeof(sr_vec4));
    sr_u32 channels = texture.spec.format;

    for (sr_u32 i = 0; i < specs.width * specs.height; i += 1) {
        sr_u32 idx = i * channels;


        switch (specs.format) {
            case SR_FORMAT_R:
            {
                texture.buffer[i].x = buffer[idx+0] / 255.0f;
                texture.buffer[i].y = 0.0f;
                texture.buffer[i].z = 0.0f;
                texture.buffer[i].w = 1.0f;
            }
            break;
            case SR_FORMAT_RG:
            {
                texture.buffer[i].x = buffer[idx+0] / 255.0f;
                texture.buffer[i].y = buffer[idx+1] / 255.0f;
                texture.buffer[i].z = 0.0f;
                texture.buffer[i].w = 1.0f;
            }
            break;
            case SR_FORMAT_RGB:
            {
                texture.buffer[i].x = buffer[idx+0] / 255.0f;
                texture.buffer[i].y = buffer[idx+1] / 255.0f;
                texture.buffer[i].z = buffer[idx+2] / 255.0f;
                texture.buffer[i].w = 1.0f;
            }
            break;
            case SR_FORMAT_RGBA:
            {
                texture.buffer[i].x = buffer[idx+0] / 255.0f;
                texture.buffer[i].y = buffer[idx+1] / 255.0f;
                texture.buffer[i].z = buffer[idx+2] / 255.0f;
                texture.buffer[i].w = buffer[idx+3] / 255.0f;
            }
            break;
        }

    }

    return texture;
}


void sr_texture_set_sampling_mode(SrTexture* texture, SrSamplingMode sampling_mode) {
    assert(texture);

    texture->spec.sampling_mode = sampling_mode;
}


void sr_texture_set_filter_mode(SrTexture* texture, SrFilter filter) {
    assert(texture);

    texture->spec.filter = filter;
}



sr_vec4 sr_texture_get_pixel(SrTexture* texture, sr_u32 x, sr_u32 y) {
    assert((x < texture->spec.width) && (y < texture->spec.height));
    
    return texture->buffer[y * texture->spec.width + x];
}



void sr_texture_free(SrTexture* texture) {
    free(texture->buffer);
}



sr_vec4 sr_texture_sample(SrTexture* texture, sr_f32 u, sr_f32 v) {

    sr_u32 width = texture->spec.width - 1;
    sr_u32 height = texture->spec.height - 1;

    switch (texture->spec.filter) {
        case SR_FILTER_NEAREST:
        {
            u = round(u * width);
            v = round(v * height);

            sr_u32 x = u;
            sr_u32 y = v;

            switch (texture->spec.sampling_mode) {
                case SR_SAMPLING_MODE_CLAMP_TO_EDGE: {
                    x = x < 0 ? 0 : x > width ? width : x;
                    y = y < 0 ? 0 : y > height ? height : y;
                    break;
                }
                case SR_SAMPLING_MODE_REPEAT: {
                    x = x % width;
                    y = y % height;
                    break;
                }
            }

            return sr_texture_get_pixel(texture, x, y);

        }
        case SR_FILTER_BILINEAR:
        {
            sr_vec2 curr   = sr_vec2 {.x = u * width, .y = v * height};

            switch (texture->spec.sampling_mode) {
                case SR_SAMPLING_MODE_CLAMP_TO_EDGE: {
                    curr.x = curr.y < 0.0f ? 0.0f : curr.x > width ? width : curr.x;
                    curr.y = curr.y < 0.0f ? 0.0f : curr.y > height ? height : curr.y;
                    break;
                }
                case SR_SAMPLING_MODE_REPEAT: {
                    curr.x = fmodf(curr.x, width);
                    curr.y = fmodf(curr.y, height);
                    break;
                }
            }

            sr_vec2 cell   = sr_vec2{.x = floorf(curr.x), .y = floorf(curr.y)};
            sr_vec2 offset = sr_vec2_sub(curr, cell);


            sr_vec4 c1 = sr_texture_get_pixel(texture, cell.x + 0, cell.y + 0);
            sr_vec4 c2 = sr_texture_get_pixel(texture, cell.x + 0, cell.y + 1);
            sr_vec4 c3 = sr_texture_get_pixel(texture, cell.x + 1, cell.y + 0);
            sr_vec4 c4 = sr_texture_get_pixel(texture, cell.x + 1, cell.y + 1);

            return sr_bilinear(offset.x, offset.y, c1, c2, c3, c4);
        }

    }

}




SrFramebuffer sr_framebuffer_create(SrFramebufferSpec spec) {
    SrFramebuffer framebuffer = {spec};

    sr_u32 size = spec.width * spec.height;

    framebuffer.color_buffer = (sr_vec4*)malloc(size * sizeof(sr_vec4));
    framebuffer.depth_buffer = (sr_f32*)malloc(size * sizeof(sr_f32));

    memset(framebuffer.color_buffer, 0, size * sizeof(sr_vec4));
    memset(framebuffer.depth_buffer, 0, size * sizeof(sr_f32));

    return framebuffer;
}

void sr_framebuffer_resize(SrFramebuffer* fb, sr_u32 width, sr_u32 height) {
    free(fb->color_buffer);
    free(fb->depth_buffer);

    fb->spec.width = width; 
    fb->spec.height = height; 
    fb->color_buffer = (sr_vec4*)malloc(width * height * sizeof(sr_vec4));
    fb->depth_buffer = (sr_f32*)malloc(width * height * sizeof(sr_f32));
}

void sr_framebuffer_set_color(SrFramebuffer* fb, sr_u32 x, sr_u32 y, sr_vec4 color) {
    sr_u32 index = y * fb->spec.width + x;

    fb->color_buffer[index] = sr_clamp01_vec4(color);
}



void sr_framebuffer_set_depth(SrFramebuffer* fb, sr_u32 x, sr_u32 y, sr_f32 value) {
    sr_u32 index = y * fb->spec.width + x;

    fb->depth_buffer[index] = value;
}



void sr_framebuffer_clear_color(SrFramebuffer* fb, sr_vec4 color ) {
    sr_u32 size = fb->spec.width * fb->spec.height;

    for (sr_u32 i = 0; i < size; i++)
        fb->color_buffer[i] = color;
}



void sr_framebuffer_clear_depth(SrFramebuffer* fb, sr_f32 value) {
    sr_u32 size = fb->spec.width * fb->spec.height;

    for (sr_u32 i = 0; i < size; i++)
        fb->depth_buffer[i] = value;
}



sr_vec4 sr_framebuffer_get_color(SrFramebuffer* fb, sr_u32 x, sr_u32 y) {
    return fb->color_buffer[y * fb->spec.width + x];
}



sr_f32 sr_framebuffer_get_depth(SrFramebuffer* fb, sr_u32 x, sr_u32 y) {
    return fb->depth_buffer[y * fb->spec.width + x];
}



void sr_framebuffer_free(SrFramebuffer* fb) {
    free(fb->color_buffer);
    free(fb->depth_buffer);
}



static sr_f32 sr_compute_blend_factor(sr_f32 src, sr_f32 dst, sr_f32 src_a, sr_f32 dst_a, SrBlendFactor blend_factor) {
    switch (blend_factor) {
        case SR_BLEND_FACTOR_ZERO:                 return 0.0f;
        case SR_BLEND_FACTOR_ONE:                  return 1.0f;
        case SR_BLEND_FACTOR_SRC_COLOR:            return src;
        case SR_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:  return 1.0f - src;
        case SR_BLEND_FACTOR_DST_COLOR:            return dst;
        case SR_BLEND_FACTOR_ONE_MINUS_DST_COLOR:  return 1.0f - dst;
        case SR_BLEND_FACTOR_SRC_ALPHA:            return src_a;
        case SR_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:  return 1.0f - src_a;
        case SR_BLEND_FACTOR_DST_ALPHA:            return dst_a;
        case SR_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:  return 1.0f - dst_a;
    }
}



static sr_f32 sr_compute_blend_op(sr_f32 lhs, sr_f32 rhs, SrBlendOp blend_op) {
    switch (blend_op) {
        case SR_BLEND_OP_ADD:      return lhs + rhs;
        case SR_BLEND_OP_SUBTRACT: return lhs - rhs;
        case SR_BLEND_OP_MIN:      return sr_min(lhs, rhs);
        case SR_BLEND_OP_MAX:      return sr_max(lhs, rhs);
    }
}



static bool sr_compute_depth_compare_op(SrPipeline* pipeline, sr_f32 new_depth, sr_u32 x, sr_u32 y) {
    SrFramebuffer* fb = pipeline->spec.framebuffer;

    sr_f32 old_depth = fb->depth_buffer[y * fb->spec.width + x];

    if (!pipeline->spec.depth_info.depth_test_enabled)
        return true;

    bool test = true;
    switch (pipeline->spec.depth_info.depth_compare_op) {
        case SR_COMPARE_OP_LESS:             test = new_depth < old_depth; break;
        case SR_COMPARE_OP_EQUAL:            test = new_depth == old_depth; break;
        case SR_COMPARE_OP_LESS_OR_EQUAL:    test = new_depth <= old_depth; break;
        case SR_COMPARE_OP_GREATER:          test = new_depth > old_depth; break;
        case SR_COMPARE_OP_NOT_EQUAL:        test = new_depth != old_depth; break;
        case SR_COMPARE_OP_GREATER_OR_EQUAL: test = new_depth >= old_depth; break;
    }

    return test 
        && new_depth <= pipeline->spec.depth_info.max_depth 
        && new_depth >= pipeline->spec.depth_info.min_depth;
}



static sr_vec3 sr_get_front_face_indices(SrFrontFace ff) {
    switch (ff) {
      case SR_FRONT_FACE_COUNTER_CLOCKWISE: return sr_vec3{.x = 0, .y = 1, .z = 2};
      case SR_FRONT_FACE_CLOCKWISE :        return sr_vec3{.x = 2, .y = 1, .z = 0};
    }
}



static bool sr_is_point_inside_polygon(SrRasterizerInfo* info, sr_f32 e1, sr_f32 e2, sr_f32 e3) {
    switch (info->polygon_mode) {
        case SR_POLYGON_MODE_FILL : 
        {
            return e1 > -SR_EP 
                && e2 > -SR_EP 
                && e3 > -SR_EP;
        }
    }
}



static bool sr_should_cull(sr_f32 area, SrCullMode cull_mode) {
    switch (cull_mode) {
        case SR_CULL_MODE_FRONT_FACE: return !(area < SR_EP);
        case SR_CULL_MODE_BACK_FACE: return (area < SR_EP);
    }
}



static void sr_interpolate_variant(void* out, void* curr, sr_u32 size, sr_f32 u, sr_f32 v, sr_f32 w, sr_f32 z) {

    sr_f32* ptr1 = (sr_f32*)((sr_u8*)curr);
    sr_f32* ptr2 = (sr_f32*)((sr_u8*)curr + size);
    sr_f32* ptr3 = (sr_f32*)((sr_u8*)curr + size + size);

    sr_f32* out_ptr = (sr_f32*)out;

    for (sr_u32 i = 0; i < size / 4; i++)
        out_ptr[i] = (ptr1[i] * u + ptr2[i] * v + ptr3[i] * w) / z;

}



SrPipeline sr_create_pipeline(SrPipelineSpec specs) {
    SrPipeline pipeline;
    pipeline.spec = specs;
    return pipeline;
}



void sr_pipeline_upload_texture(SrPipeline* pipeline, SrTexture* texture, sr_usize texture_slot) {
    assert(texture_slot < SR_MAX_TEXTURES_SLOTS);

    pipeline->registry.textures[texture_slot] = texture;
}



void sr_pipeline_upload_uniform_buffer(SrPipeline* pipeline, void* data, sr_u32 slot) {
    assert(slot < SR_MAX_UNIFORMS_SLOTS);

    pipeline->registry.uniforms[slot].data = data;
}


void sr_draw(SrPipeline* pipeline, sr_usize vertices_count, void* buff) {

    sr_u32 width  = pipeline->spec.framebuffer->spec.width;
    sr_u32 height = pipeline->spec.framebuffer->spec.height;
    sr_u32 vertex_stride   = pipeline->spec.vertex_input_info.byte_count;
    sr_u32 variants_stride = pipeline->spec.variants_info.byte_count;

    

    SrVariant rm_variants     = malloc(variants_stride  * vertices_count);
    SrVariant current_variant = malloc(variants_stride);
    sr_vec4* rm_positions      = (sr_vec4*)malloc(sizeof(sr_vec4) * vertices_count);


    sr_u8* vertex_ptr   = (sr_u8*)buff;
    sr_u8* variants_ptr = (sr_u8*)rm_variants;


    // vertex pass
    for (sr_u32 i = 0; i < vertices_count; i++) {


        // getting the vertex shader output
        sr_vec4 pos = pipeline->spec.vertex_shader(
                            &vertex_ptr[i * vertex_stride], 
                            &variants_ptr[i * variants_stride], 
                            &pipeline->registry );


        // converting to NDC coordinates
        pos.x /= pos.w;
        pos.y /= pos.w;
        pos.z /= pos.w;



        // converting from NDC coordintates to the Screen coordinates
        rm_positions[i].x = (pos.x * 0.5f + 0.5f) * (width); 
        rm_positions[i].y = (pos.y * 0.5f + 0.5f) * (height);
        rm_positions[i].z = pos.z;
        rm_positions[i].w = pos.w;

    }



    // rasteration pass
    for (sr_u32 i = 0; i < vertices_count; i += 3) {

        sr_vec3 idx = sr_get_front_face_indices(pipeline->spec.rasterizer_info.front_face);
        sr_vec4 p1 = rm_positions[i + (sr_u32)idx.x];
        sr_vec4 p2 = rm_positions[i + (sr_u32)idx.y];
        sr_vec4 p3 = rm_positions[i + (sr_u32)idx.z];


        sr_f32 area = sr_edge_function(p1, p2, p3); 
        sr_f32 ooa = area == 0 ? 0.001f : 1.0f / area;


        // face culling
        if (sr_should_cull(area, pipeline->spec.rasterizer_info.cull_mode))
            continue;


        // calculating some constants that will be used later to update 
        // the edge functions
        sr_f32 dy12 = p1.y - p2.y;    sr_f32 dx21 = p2.x - p1.x; 
        sr_f32 dy23 = p2.y - p3.y;    sr_f32 dx32 = p3.x - p2.x;
        sr_f32 dy31 = p3.y - p1.y;    sr_f32 dx13 = p1.x - p3.x;


        // getting the bounding box of the triangle 
        sr_f32 min_x = round(sr_clamp(sr_min(p1.x, sr_min(p2.x, p3.x)) - 0.5f, 0.0f, (sr_f32)width  - 1));
        sr_f32 min_y = round(sr_clamp(sr_min(p1.y, sr_min(p2.y, p3.y)) - 0.5f, 0.0f, (sr_f32)height - 1));
        sr_f32 max_x = round(sr_clamp(sr_max(p1.x, sr_max(p2.x, p3.x)) + 0.5f, 0.0f, (sr_f32)width  - 1));
        sr_f32 max_y = round(sr_clamp(sr_max(p1.y, sr_max(p2.y, p3.y)) + 0.5f, 0.0f, (sr_f32)height - 1));



        // pre calculating the edge functions so we update them using DDA 
        // instead of recalculating them every pixel
        sr_f32 edge1 = sr_edge_function(p2, p3, sr_vec4 {min_x, min_y, 0.0f, 0.0f});
        sr_f32 edge2 = sr_edge_function(p3, p1, sr_vec4 {min_x, min_y, 0.0f, 0.0f});
        sr_f32 edge3 = sr_edge_function(p1, p2, sr_vec4 {min_x, min_y, 0.0f, 0.0f});


        for (sr_u32 y = min_y; y <= max_y; y += 1) {

            sr_f32 e1 = edge1;
            sr_f32 e2 = edge2;
            sr_f32 e3 = edge3;


            for (sr_u32 x = min_x; x <= max_x; x += 1) {

                if (sr_is_point_inside_polygon(&pipeline->spec.rasterizer_info, e1, e2, e3)) {

                    // normalizing the barycentric coordinates so we can use
                    // them to interpolate the attributes 
                    sr_f32 u = e1 * ooa;
                    sr_f32 v = e2 * ooa;
                    sr_f32 w = e3 * ooa;

                    // inter_pos = p1 * u + p2 * v + p3 * w
                    sr_vec4 inter_pos = sr_vec4_add(sr_vec4_add(sr_vec4_mul_s(p1, u), 
                                    sr_vec4_mul_s(p2, v)), sr_vec4_mul_s(p3, w));

                    sr_f32 curr_depth = inter_pos.z;

                    if (sr_compute_depth_compare_op(pipeline, curr_depth, x, y)) {

                        if (pipeline->spec.depth_info.depth_write_enabled) {
                            sr_framebuffer_set_depth(pipeline->spec.framebuffer, x, y, curr_depth);
                        }

                        sr_f32 z = u / p1.w + v / p2.w + w / p3.w;
                        u /= p1.w;
                        v /= p2.w;
                        w /= p3.w;


                        sr_interpolate_variant(current_variant, &variants_ptr[i * variants_stride], 
                                            variants_stride, u, v, w, z);


                        sr_vec4 new_color = pipeline->spec.pixel_shader(current_variant, &pipeline->registry);


                        if (!pipeline->spec.color_blend_info.blend_enabled) {
                            sr_framebuffer_set_color(pipeline->spec.framebuffer, x, y, new_color);

                        } else {

                            sr_vec4 final_color {}; 
                            sr_vec4 old_color = sr_framebuffer_get_color(pipeline->spec.framebuffer, x, y);

                            SrBlendFactor src_blend_factor = pipeline->spec.color_blend_info.src_blend_factor;
                            SrBlendFactor dst_blend_factor = pipeline->spec.color_blend_info.dst_blend_factor;
                            SrBlendOp blend_op             = pipeline->spec.color_blend_info.blend_op;

                            sr_f32* old_c = (sr_f32*)&old_color.x;
                            sr_f32* new_c = (sr_f32*)&new_color.x;
                            sr_f32* final_c = (sr_f32*)&final_color.x;

                            for (sr_u32 i = 0; i < 4; i++) {
                                sr_f32 src = sr_compute_blend_factor(new_c[i], old_c[i], 
                                                            new_color.w, old_color.w, src_blend_factor);
                                sr_f32 dst = sr_compute_blend_factor(new_c[i], old_c[i], 
                                                            new_color.w, old_color.w, dst_blend_factor);

                                final_c[i] = sr_compute_blend_op(src * new_c[i], dst * old_c[i], blend_op);
                            }

                            sr_framebuffer_set_color(pipeline->spec.framebuffer, x, y, final_color);
                        }
                    }
                }

                e1 += dy23;
                e2 += dy31;
                e3 += dy12;
            }

            edge1 += dx32;
            edge2 += dx13;
            edge3 += dx21;
        }  
    }

    free(rm_positions);
    free(rm_variants);
    free(current_variant);
}





#endif // __SOFTWARE_RENDERER_IMPLEMENTATION




#ifdef __cplusplus
}
#endif






#endif // __SOFTWARE_RENDERER_H