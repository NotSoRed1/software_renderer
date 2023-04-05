#define _CRT_SECURE_NO_WARNINGS

#define __SOFTWARE_RENDERER_IMPLEMENTATION
#include "../src/software_renderer.h"


#define STB_IMAGE_IMPLEMENTATION
#include "ext/utils.h"
#include <vector>

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
    vec3 pos;
    vec3 normal;
    vec2 uv;
};

struct Variant {
    vec3 world_pos;
    vec3 normal;
    vec2 uv;
};

struct UniformBuffer {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 view_pos;
};



void load_obj_file(const char* file_path, std::vector<Vertex>* out)
{
	objl::Loader Loader;

	bool loadout = Loader.LoadFile(file_path);
	if (loadout) {
		for (int mesh = 0; mesh < Loader.LoadedMeshes.size(); mesh++) {
			objl::Mesh curMesh = Loader.LoadedMeshes[mesh];

			for (int idx = 0; idx < curMesh.Indices.size(); idx += 1) {

                Vertex vert;
                int i = curMesh.Indices[idx];

                vert.pos = {curMesh.Vertices[i].Position.X, curMesh.Vertices[i].Position.Y, -curMesh.Vertices[i].Position.Z};
                vert.normal = {curMesh.Vertices[i].Normal.X, curMesh.Vertices[i].Normal.Y, -curMesh.Vertices[i].Normal.Z};
                vert.uv = {curMesh.Vertices[i].TextureCoordinate.X, curMesh.Vertices[i].TextureCoordinate.Y};

                out->push_back(vert);
			}
		}
	} else {
        printf("Failed to load the model (%s)\n", file_path);
    }
}




vec3 pow5(const vec3 &v) {return v * v * v * v * v; }

vec3 f_schlick(vec3 f0, float u) {
    return f0 + (1.0f - f0) * pow5(1.0f - u);
}


f32 d_ggx(f32 ndoth, f32 roughness) {
    f32 a = ndoth * roughness;
    f32 k = roughness / max((1.0 - ndoth * ndoth + a * a), RM_EP);
    return k * k * (1.0f / PI);
}


f32 v_smith_ggx(f32 ndotv, f32 ndotl, f32 roughness) {
    f32 a = roughness * roughness;
    f32 a2 = a * a;
    f32 ggxl = ndotv * sqrt((-ndotl * a2 + ndotl) * ndotl + a2);
    f32 ggxv = ndotl * sqrt((-ndotv * a2 + ndotv) * ndotv + a2);

    return 0.5f / (ggxv + ggxl);
}


f32 v_smith_ggx_fast(f32 ndotv, f32 ndotl, f32 roughness) {
    f32 a = roughness * roughness;
    f32 ggxv = ndotl * (ndotv * (1.0f - a) + a);
    f32 ggxl = ndotv * (ndotl * (1.0f - a) + a);

    return 0.5f / (ggxv + ggxl);
}

sr_vec4 pbr_pixel_shader(SrVariant variants, SrGlobalRegistry* reg) {
    Variant* in = (Variant*)variants;

    UniformBuffer& ubo = *sr_get_uniform_buffer(UniformBuffer, reg, 0);
    vec3 view_pos  = ubo.view_pos;
    vec3 light_pos = ubo.view_pos;

    sr_vec4 albedo_t    = sr_texel(reg, 0, in->uv.x, in->uv.y);
    sr_vec4 roughness_t = sr_texel(reg, 1, in->uv.x, in->uv.y);
    sr_vec4 metalness_t = sr_texel(reg, 2, in->uv.x, in->uv.y);
    sr_vec4 occlusion_t = sr_texel(reg, 3, in->uv.x, in->uv.y);
    sr_vec4 emission_t  = sr_texel(reg, 4, in->uv.x, in->uv.y);

    vec3 albedo   = {albedo_t.x, albedo_t.y, albedo_t.z};
    vec3 emission = {emission_t.x, emission_t.y, emission_t.z};
    f32 roughness = clamp(roughness_t.x, 0.089f, 1.0f);
    f32 metalness = metalness_t.x;
    f32 occlusion = occlusion_t.x;

    vec3 radiance = vec3(1.0f, 1.0f, 1.0f) * 2.0f;

    vec3 normal       = normalize(in->normal);
    vec3 view_dir     = normalize(view_pos - in->world_pos);
    vec3 light_dir    = normalize(light_pos);
    vec3 halfway      = normalize(light_dir + view_dir);


    f32 ndoth = max(dot(normal, halfway), 0.0f);
    f32 ndotv = max(dot(normal, view_dir), 0.0f);
    f32 ndotl = max(dot(normal, light_dir), 0.0f);
    f32 ldoth = max(dot(halfway, view_dir), 0.0f);


    vec3 f0 = lerp(vec3(0.4f), albedo, metalness);
    f32 d  = d_ggx(ndoth, roughness);       
    f32 v  = v_smith_ggx(ndotv, ndotl, roughness);
    vec3 f = f_schlick(f0, ldoth);

    vec3 base_color = albedo * (1.0 - metalness);
    vec3 difuse = base_color / PI;
    vec3 specular = (d * v * f);
    vec3 final_color = (difuse + specular) * radiance * ndotl;


    vec3 ambient = vec3(0.03f) * albedo * occlusion;
    final_color += ambient + emission * 2.0f;
    final_color = gamma2_2(final_color);

    return {final_color.x, final_color.y, final_color.z, 1.0f};

    // vec3 uv;
    // uv.x = atan2(normal.x, normal.z) / (2 * PI) + 0.5f;
    // uv.y = normal.y * 0.5f + 0.5f;
}



sr_vec4 vertex_shader(SrVertex in, SrVariant out, SrGlobalRegistry* reg) {
    Vertex* vertex = (Vertex*)in;

    UniformBuffer& ubo = *sr_get_uniform_buffer(UniformBuffer, reg, 0);

    vec4 pos = vec4(vertex->pos, 1.0f);

    Variant variant;
    variant.world_pos = (ubo.model * vec4(vertex->pos, 1.0f)).xyz;
    variant.normal    = mat3(ubo.model) * vertex->normal;
    variant.uv        = vertex->uv;

    sr_upload_variant(out, variant);

    pos = ubo.proj * ubo.view * ubo.model * pos;
    return {pos.x, pos.y, pos.z, pos.w};
}



int main(void) {

    WNDCLASS window_class {};
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
    window_class.lpszClassName = "pbrShadingClassName";
    window_class.hInstance     = GetModuleHandle(0);
    window_class.lpfnWndProc   = window_proc;

    RegisterClass(&window_class);

    HWND hwnd = CreateWindowEx(
        0, window_class.lpszClassName, "pbrShading",
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


    std::vector<Vertex> buff;
    load_obj_file("./assets/models/helmet/helmet.obj", &buff);

    SrTexture textures[5];
    textures[0] = utils_load_texture_from_file("./assets/models/helmet/helmet_albedo.png");
    textures[1] = utils_load_texture_from_file("./assets/models/helmet/helmet_roughness.png");
    textures[2] = utils_load_texture_from_file("./assets/models/helmet/helmet_metalness.png");
    textures[3] = utils_load_texture_from_file("./assets/models/helmet/helmet_occlusion.png");
    textures[4] = utils_load_texture_from_file("./assets/models/helmet/helmet_emission.png");

    SrFramebufferSpec framebuffer_specs;
    framebuffer_specs.width  = width;
    framebuffer_specs.height = height;

    framebuffer = sr_framebuffer_create(framebuffer_specs);



    // creating the pipeline
    SrDepthInfo depth_info {};
    depth_info.depth_test_enabled  = true;
    depth_info.depth_write_enabled = true;
    depth_info.depth_compare_op    = SR_COMPARE_OP_LESS_OR_EQUAL;
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
    pipeline_specs.pixel_shader      = &pbr_pixel_shader;



    pipeline = sr_create_pipeline(pipeline_specs);


    UniformBuffer ubo {};
    ubo.view_pos = vec3(1.5f, 1.5f, -3.0f);
    ubo.view = look_at(ubo.view_pos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    ubo.model = rotate(translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    ubo.proj = perspective(radians(55.0f), (f32)width/ height, 0.1f, 100.0f);



    sr_pipeline_upload_uniform_buffer(&pipeline, &ubo, 0);
    sr_pipeline_upload_texture(&pipeline, &textures[0], 0);
    sr_pipeline_upload_texture(&pipeline, &textures[1], 1);
    sr_pipeline_upload_texture(&pipeline, &textures[2], 2);
    sr_pipeline_upload_texture(&pipeline, &textures[3], 3);
    sr_pipeline_upload_texture(&pipeline, &textures[4], 4);


    MSG msg = { };
    int vertices_count = buff.size();

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


        sr_draw(&pipeline, vertices_count, buff.data());

        sr_present(&framebuffer);

        printf("%.0f fps \n", 1.0 / ((clock() - start) * 1e-3));
    }



    // cleanup
    sr_framebuffer_free(&framebuffer);
    for (u32 i = 0; i < sizeof(textures) / sizeof(textures[0]); i++) {
        if (textures[i].buffer)
            sr_texture_free(&textures[i]);
    }


    return 0;
}
