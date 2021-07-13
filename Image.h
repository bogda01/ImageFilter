#ifndef IMAGE_H
#define IMAGE_H
#define HEADER_SIZE sizeof(uint32_t) * 8
#include <cstdio>
#include <stdint.h>
#include <string.h>

enum ImageType{
    PNG, JPG, BMP, TGA
};

class Image {
    public:
    uint8_t* data = NULL;
    size_t size = 0;
    int w;
    int h;
    int ch;

    Image(const char* filename);
    Image(int w, int h, int ch);
    Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageType getFileType (const char* filename);

    Image& greyscale_avg();
    Image& greyscale_lum();

    Image& colorMask (float r, float g, float b);

    Image& cryptImg(const char* msg);

    Image& decryptImg(char* buffer, size_t* msgLen);
};

#endif
