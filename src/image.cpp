#include "lgf/image.h"

int LGF::Draw::Image::load_png(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return 1;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) { 
        fclose(fp); 
        return 1; 
    }

    png_infop info = png_create_info_struct(png);
    if (!info) { 
        fclose(fp);
        png_destroy_read_struct(&png, nullptr, nullptr);
        return 2;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(fp);
        return 3;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    png_byte colour_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (colour_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (colour_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

    if (colour_type == PNG_COLOR_TYPE_RGB || colour_type == PNG_COLOR_TYPE_GRAY || colour_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 255, PNG_FILLER_AFTER);
    }
    if (colour_type == PNG_COLOR_TYPE_GRAY || colour_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    size_t rowbytes = png_get_rowbytes(png, info);
    image_data.resize(rowbytes * height);
    std::vector<png_bytep> rows(height);

    for (int y = 0; y < height; ++y) {
        rows[height - 1 - y] = image_data.data() + y * rowbytes;
    }

    std::cout << "Image Data Size: " << image_data.size() << " bytes\n";

    png_read_image(png, rows.data());
    
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);
    return 0;
}

void LGF::Draw::Image::texture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);
}

LGF::Draw::Image::Image(const char* filename) {
    int error = this->load_png(filename);
    if (error) {
        std::cerr << "ERROR: failed to load image. filename: " << filename << "\n";
        exit(1);
    }
    this->texture();
}