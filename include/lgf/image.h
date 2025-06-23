#pragma once
#include <png.h>
#include <vector>
#include <cstdio>
#include <iostream>
#include <glad/glad.h>

namespace LGF::Draw {
    class Image {
        private:
        std::vector<unsigned char> image_data;
        int width;
        int height;
        int load_png(const char* file_name);
        void texture();
        public:
        Image(const char* path);
        unsigned int textureID;
    };
};