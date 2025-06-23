#pragma once

#include <glad/glad.h>
#include <unordered_map>
#include <string>

#include "lgf/window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace LGF::Draw {
        struct Character {
            unsigned int textureID;
            glm::ivec2 Size;
            glm::ivec2 Bearing;
            unsigned int Advance;
        };        
        class Font {
            public:
            Font(const char* fontFaceFile, int fontSize, LGF::LGFWindow* window, const char* vertexShaderFile, const char* fragmentShaderFile);
            Font(const char* fontFaceFile, int fontSize, LGF::LGFWindow* window);
            void renderText(std::string text, float x, float y, float scale, glm::vec4 colour);
            glm::vec2 getTextSize(const std::string& text, float scale);
            private:
            void matrix();
            void init(const char* fontFaceFile, int fontSize, const char* vertexShaderFile, const char* fragmentShaderFile);
            std::unordered_map<char, Character> characters{};
            unsigned int shader;
            unsigned int VAO;
            unsigned int VBO;
            LGF::LGFWindow* window;
        };
};