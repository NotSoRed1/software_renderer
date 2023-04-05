
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// #include "./math.h"

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


struct __attribute__((packed)) BmpHeader {
    u16 buffer_type;    
    u32 buffer_size;
    u32 _reserved;
    u32 pixels_offset;
    
    u32 size;
    u32 width;
    u32 height;
    u16 planes;
    u16 bit_count;
    u32 compression;
    u32 size_image;
    u32 x_pxls_per_meter;
    u32 y_pxls_per_meter;
    u32 clr_used;
    u32 clr_important;

};


struct Buffer {
    u8* buffer;
    usize size;

};


inline void buffer_memset(Buffer* buffer, u8 value) {
    for (u32 i = 0; i < buffer->size; i++)
        buffer->buffer[i] = value;
}


inline Buffer new_buffer(usize size) {
    Buffer buffer;

    buffer.buffer = (u8*)malloc(size);
    buffer.size = size;
    buffer_memset(&buffer, 0);

    return buffer;
}


inline void free_buffer(Buffer* buff) {
    free(buff->buffer);
    buff->size = 0;
}


inline Buffer read_entire_file(const char* source, const char* method) {
    FILE* file = fopen(source, method);

    fseek(file, 0, SEEK_END);
    usize size = ftell(file);
    fseek(file, 0, SEEK_SET);

    u8* buffer= (u8*)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);

    Buffer ret = {buffer, size};
    return ret;
}


inline void write_bmp(const char* src, u8* buffer, u32 width, u32 height) {
    FILE* file = fopen(src, "wb");

    u32 size = width * height * 4;

    BmpHeader header = {};
    header.buffer_type   = 0x4D42;
    header.buffer_size   = sizeof(BmpHeader) + size;
    header.pixels_offset = sizeof(BmpHeader);
    header.size          = sizeof(BmpHeader) - 14;
    header.width         = width;
    header.height        = height;
    header.planes        = 1;
    header.bit_count     = 32;
    header.compression   = 0;
    header.size_image    = size;
    header.x_pxls_per_meter = 0;
    header.y_pxls_per_meter = 0;
    header.clr_important    = 0;
    header.clr_important    = 0;


    fwrite(&header, sizeof(BmpHeader), 1, file);
    fwrite(buffer, size, 1, file);

    fclose(file);
}
