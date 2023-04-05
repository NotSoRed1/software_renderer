#define _CRT_SECURE_NO_WARNINGS

#define __SOFTWARE_RENDERER_IMPLEMENTATION
#include "../src/software_renderer.h"


#define STB_IMAGE_IMPLEMENTATION
#include "ext/utils.h"

#include "../backends/win32_backend.h"

#include <windows.h>
#include <time.h>

u32 width  = 800;
u32 height = 600;
bool running = true;

LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {

    switch (msg)
    {
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        } break;


        case WM_DESTROY:
        {
            PostQuitMessage(0);
            running = false;
            return 0;
        } break;

        case WM_SIZE:
        {
            width = LOWORD(l_param);
            height = HIWORD(l_param);

            return 0;
        } break;


        default:
        {
            return DefWindowProc(hwnd, msg, w_param, l_param);
        }
    }
}











struct Vertex {
    vec2 pos;
    vec2 uv;
};

struct Variant {
    vec2 uv;
};

inline SrTexture load_texture_from_file(const char* path) {
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



sr_vec4 pixel_shader(SrVariant variants, SrGlobalRegistry* reg) {
    Variant* in = (Variant*)variants;
    sr_vec4 texel = sr_texel(reg, 0, in->uv.x, in->uv.y);

    return texel;
}


sr_vec4 vertex_shader(SrVertex in, SrVariant out, SrGlobalRegistry* reg) {
    Vertex* vertex = (Vertex*)in;

    Variant variant;
    variant.uv = vertex->uv;
    sr_upload_variant(out, variant);

    vec4 pos = vec4(vertex->pos, 0.0f, 1.0f);
    return {pos.x, pos.y, pos.z, pos.w};
}



int main(void) {

    WNDCLASS window_class {};
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
    window_class.lpszClassName = "textureClassName";
    window_class.hInstance     = GetModuleHandle(0);
    window_class.lpfnWndProc   = window_proc;

    RegisterClass(&window_class);

    HWND hwnd = CreateWindowEx(
        0, window_class.lpszClassName, "texture",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        200, 200, width, height,
        NULL,
        NULL,
        GetModuleHandle(0),
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    sr_context_init(&hwnd);



    SrFramebuffer framebuffer;
    SrPipeline pipeline;


    Vertex buff[] = {
        // back face
        {{-0.5f,  0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
    };

    SrTexture texture = load_texture_from_file("./assets/textures/wood.png");
    // sr_texture_set_filter_mode(&texture, SR_FILTER_BILINEAR);


    SrFramebufferSpec framebuffer_specs;
    framebuffer_specs.width  = width;
    framebuffer_specs.height = height;

    framebuffer = sr_framebuffer_create(framebuffer_specs);



    // creating the pipeline
    SrDepthInfo depth_info {};
    depth_info.depth_test_enabled  = false;
    depth_info.depth_write_enabled = false;

    SrRasterizerInfo rasterizer_info {};
    rasterizer_info.front_face   = SR_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer_info.cull_mode    = SR_CULL_MODE_BACK_FACE;
    rasterizer_info.polygon_mode = SR_POLYGON_MODE_FILL;

    SrVertexInputInfo vertex_input_info {};
    vertex_input_info.byte_count = sizeof(Vertex);

    SrVariantsInfo variants_info {};
    variants_info.byte_count = sizeof(Variant);

    SrColorBlendInfo color_blend_info {};
    color_blend_info.blend_enabled    = false;

    SrPipelineSpec pipeline_specs {};
    pipeline_specs.primitve_type     = SR_PRIMITIVE_TYPE_TRIANGLE_LIST;
    pipeline_specs.depth_info        = depth_info;
    pipeline_specs.rasterizer_info   = rasterizer_info;
    pipeline_specs.vertex_input_info = vertex_input_info;
    pipeline_specs.variants_info     = variants_info;
    pipeline_specs.color_blend_info  = color_blend_info;
    pipeline_specs.framebuffer       = &framebuffer;
    pipeline_specs.vertex_shader     = &vertex_shader;
    pipeline_specs.pixel_shader      = &pixel_shader;



    pipeline = sr_create_pipeline(pipeline_specs);

    sr_pipeline_upload_texture(&pipeline, &texture, 0);


    MSG msg = { };
    int vertices_count = sizeof(buff) / sizeof(buff[0]);

    while (running) {

        f64 start = clock();

        if (PeekMessage(&msg, NULL, 0, 0, true) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (framebuffer.spec.width != width || framebuffer.spec.height != height)
            sr_framebuffer_resize(&framebuffer, width, height);

        sr_framebuffer_clear_color(&framebuffer, {0.04f, 0.04f, 0.04f, 1.0f});
        sr_framebuffer_clear_depth(&framebuffer, 1.0f);


        sr_draw(&pipeline, vertices_count, buff);

        sr_present(&framebuffer);

        printf("%.0f fps \n", 1.0 / ((clock() - start) * 1e-3));
    }


    // cleanup
    sr_framebuffer_free(&framebuffer);
    sr_texture_free(&texture);


    return 0;
}