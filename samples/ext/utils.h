#pragma once

#include "../../src/software_renderer.h"

#include "stb_image.h"
#include "obj_loader.h"
#include "math.h"
#include "bmp.h"


inline SrTexture utils_load_texture_from_file(const char* path) {
    stbi_set_flip_vertically_on_load(true);

    sr_i32 width, height, channels;
    sr_u8* tex_buff= stbi_load(path, &width, &height, &channels, 0);

    if (!tex_buff) 
        assert(false && "Failed to load the texture correctly! \n");

    SrTextureSpec specs {};
    specs.filter        = SR_FILTER_NEAREST;
    specs.sampling_mode = SR_SAMPLING_MODE_REPEAT;
    specs.format        = (SrFormat)channels;
    specs.width         = width;
    specs.height        = height;

    SrTexture texture = sr_texture_create(specs, tex_buff);

    stbi_image_free(tex_buff);

    return texture;
}


inline void write_framebuffer_to_file(SrFramebuffer* fb, const char* file_path) {
    u32 size = fb->spec.width * fb->spec.height;

    u32* buffer = (u32*)malloc(size * 4);

    for (u32 i = 0; i < size; i++) {
        buffer[i] = pack_vec4_to_u32(rgba_to_argb(*(vec4*)&fb->color_buffer[i]));
    }

    write_bmp(file_path, (u8*)buffer, fb->spec.width, fb->spec.height);
    free(buffer);
}