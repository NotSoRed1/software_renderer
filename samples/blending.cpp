#define _CRT_SECURE_NO_WARNINGS

#define __SOFTWARE_RENDERER_IMPLEMENTATION
#include "../src/software_renderer.h"


#define STB_IMAGE_IMPLEMENTATION
#include "ext/utils.h"
#include "../backends/win32_backend.h"


#include "windows.h"
#include "time.h"

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
    vec3 pos;
    vec4 color;
};

struct Variant {
    vec4 color;
};



sr_vec4 pixel_shader(SrVariant variants, SrGlobalRegistry* reg) {
    Variant* in = (Variant*)variants;

    return {in->color.x, in->color.y, in->color.z, in->color.w};
}


sr_vec4 vertex_shader(SrVertex in, SrVariant out, SrGlobalRegistry* reg) {
    Vertex* vertex = (Vertex*)in;

    mat4& model      = *sr_get_uniform_buffer(mat4, reg, 0);
    mat4& proj       = *sr_get_uniform_buffer(mat4, reg, 1);
    mat4& view       = *sr_get_uniform_buffer(mat4, reg, 2);

    vec4 pos = vec4(vertex->pos, 1.0f);

    Variant variant;
    variant.color = vertex->color;
    sr_upload_variant(out, variant);


    pos = proj * view * model * pos;
    return {pos.x, pos.y, pos.z, pos.w};
}

int main(void) {

    WNDCLASS window_class {};
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
    window_class.lpszClassName = "blendingClassName";
    window_class.hInstance     = GetModuleHandle(0);
    window_class.lpfnWndProc   = window_proc;

    RegisterClass(&window_class);

    HWND hwnd = CreateWindowEx(
        0, window_class.lpszClassName, "blending",
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


    Vertex buff[] = {
        // back face
        {{-0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},

        // front face
        {{-0.5f,  0.5f,-0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}},
        {{-0.5f, -0.5f,-0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}},
        {{ 0.5f,  0.5f,-0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}},
        {{-0.5f, -0.5f,-0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}},
        {{ 0.5f, -0.5f,-0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}},
        {{ 0.5f,  0.5f,-0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}},
    };


    SrFramebuffer framebuffer;
    SrPipeline pipeline;

    SrFramebufferSpec framebuffer_specs;
    framebuffer_specs.width  = width;
    framebuffer_specs.height = height;

    framebuffer = sr_framebuffer_create(framebuffer_specs);



    // creating the pipeline
    SrDepthInfo depth_info {};
    depth_info.depth_compare_op    = SR_COMPARE_OP_LESS_OR_EQUAL;
    depth_info.depth_test_enabled  = true;
    depth_info.depth_write_enabled = true;
    depth_info.min_depth           = 0.0f;
    depth_info.max_depth           = 1.0f;

    SrRasterizerInfo rasterizer_info {};
    rasterizer_info.front_face   = SR_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer_info.cull_mode    = SR_CULL_MODE_BACK_FACE;
    rasterizer_info.polygon_mode = SR_POLYGON_MODE_FILL;

    SrVertexInputInfo vertex_input_info {};
    vertex_input_info.byte_count = sizeof(Vertex);

    SrVariantsInfo variants_info {};
    variants_info.byte_count = sizeof(Variant);

    SrColorBlendInfo color_blend_info {};
    color_blend_info.blend_enabled    = true;
    color_blend_info.src_blend_factor = SR_BLEND_FACTOR_SRC_ALPHA;
    color_blend_info.dst_blend_factor = SR_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_info.blend_op         = SR_BLEND_OP_ADD;


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


        vec3 camera_pos = vec3(1.3f, 1.5f, -3.0f);
        // f32 c = start * 0.001f;
        // vec3 camera_pos = vec3(sinf(c) * 3, 1.5f, cosf(c) * 3);
        mat4 model      = rotate(translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)), radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
        mat4 proj       = perspective(radians(55.0f), (f32)width/ height, 0.1f, 100.0f);
        mat4 view       = look_at(camera_pos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

        sr_pipeline_upload_uniform_buffer(&pipeline, &model, 0);
        sr_pipeline_upload_uniform_buffer(&pipeline, &proj, 1);
        sr_pipeline_upload_uniform_buffer(&pipeline, &view, 2);



        sr_framebuffer_clear_color(&framebuffer, {0.04f, 0.04f, 0.04f, 1.0f});
        sr_framebuffer_clear_depth(&framebuffer, 1.0f);

        sr_draw(&pipeline, vertices_count, buff);

        sr_present(&framebuffer);

        printf("%.0f fps \n", 1.0 / ((clock() - start) * 1e-3));
    }

    write_framebuffer_to_file(&framebuffer, "build/blending.bmp");


    // cleanup
    sr_framebuffer_free(&framebuffer);


    return 0;
}