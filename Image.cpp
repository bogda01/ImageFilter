#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char* filename){
    if(read(filename)){
        printf("Read %s\n", filename);
        size = w * h * ch;
    }
    else{
        printf("Failed to read %s\n", filename);
    }
}

Image::Image(int w, int h, int ch): w(w), h(h), ch(ch){
    size = w * h * ch;
    data = new uint8_t[size];
}

Image::Image(const Image& img): Image(img.w, img.h, img.ch){
    memcpy(data, img.data, size);
}

Image::~Image(){
    stbi_image_free(data);
}

bool Image::read(const char* filename){
    data = stbi_load(filename, &w, &h, &ch, 0);
    return data != NULL;
}

bool Image::write(const char* filename){
    ImageType type = getFileType(filename);
    int ok;
    switch(type){
        case PNG:
            ok = stbi_write_png(filename, w, h, ch, data, w * ch);
            break;
        case BMP:
            ok = stbi_write_bmp(filename, w, h, ch, data);
            break;
        case JPG:
            ok = stbi_write_jpg(filename, w, h, ch, data, 100);
            break;
        case TGA:
            ok = stbi_write_tga(filename, w, h, ch, data);
            break;
    }
    return ok != 0;
}

ImageType Image::getFileType (const char* filename){
    const char* ext = strrchr(filename, '.');
    if(ext != nullptr){
        if(strcmp(ext, ".png") == 0){
            return PNG;
        }
        else if(strcmp(ext, ".jpg") == 1){
            return JPG;
        }
        else if(strcmp(ext, ".bmp") == 2){
            return BMP;
        }
        else if(strcmp(ext, ".tga") == 3){
            return TGA;
        }
    }
    return PNG;
}

Image& Image::greyscale_avg(){
    if(ch < 3)
    {
        printf("Image %p is already greyscaled because it has less than 3 channels.", this);
    }
    else{
        for(int i = 0; i < size; i += ch){
            int grey = (data[i] + data[i+1] + data[i+2]) / 3;
            memset(data+i, grey, 3);
        }
    }
    return *this;
}

Image& Image::greyscale_lum(){
    if(ch < 3)
    {
        printf("Image %p is already greyscaled because it has less than 3 channels.", this);
    }
    else{
        for(int i = 0; i < size; i += ch){
            int grey = 0.2126*data[i] + 0.7152*data[i+1] + 0.0722*data[i+2];
            memset(data+i, grey, 3);
        }
    }
    return *this;
}

Image& Image::colorMask (float r, float g, float b){
    if(ch < 3){
        printf("\e[31m[ERROR]This image has %d ch Color mask needs at least 3 channels to be performed!\e[0m\n", ch);
    }
    else{
        for(int i = 0; i < size; i += ch){
            data[i] *= r;
            data[i+1] *= g;
            data[i+2] *= b;
        }
    }
    return *this;
}

Image& Image::cryptImg(const char* msg){
    uint32_t len = strlen(msg) * 8;

    if(len + HEADER_SIZE > size){
        printf("\e[31m[ERROR] This message is to large (%lu bits / %zu bits)\e[0m\n", len+HEADER_SIZE, size);
        return *this;
    }

    for(uint32_t i = 0; i < HEADER_SIZE; i++){
        data[i] &= 0xFE;
        data[i] |= (len >> (HEADER_SIZE - 1 - i)) & 1UL;
    }

    for(uint32_t i = 0; i < len; i++){
        data[i+HEADER_SIZE] &= 0xFE;
        data[i+HEADER_SIZE] |= (msg[i / 8] >> ((len - 1 - i) % 8)) & 1;
    }
    return *this;
}

Image& Image::decryptImg(char* buffer, size_t* msgLen){
    uint32_t len = 0;
    for(uint32_t i = 0; i < HEADER_SIZE; i++){
        len = (len << 1)|(data[i] & 1);
    }
    *msgLen = len / 8;

    for(uint32_t i = 0; i < len; i++){
        buffer[i / 8] = (buffer[i / 8] << 1)|(data[i+HEADER_SIZE] & 1);
    }
    return *this;
}
