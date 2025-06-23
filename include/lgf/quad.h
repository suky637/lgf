#pragma once
#include <glad/glad.h>
#include <lgf/window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <lgf/image.h>

namespace LGF::Draw {
        enum class QuadType {
            ROUNDED,
            IMAGE_ROUNDED
        };
        namespace Primitives {
            extern float rectangle[];
        };
        class Quad {
            public:
            Quad(const char* vertexShaderFile, const char* fragmentShaderFile);
            Quad(QuadType type);
            ~Quad();
            void linkWindow(LGF::LGFWindow* window);
            unsigned int getShaderID();
            void render();
            void setRect(const glm::vec2& pos, const glm::vec2& size);
            glm::mat4 model;
            unsigned int VAO, VBO, shaderProgram;
            QuadType type;
            LGF::LGFWindow* window;
            private:
            void init(const char* vertexShaderFile, const char* fragmentShaderFile);
        };
        class ImageQuad {
            public:
            void render();
            ImageQuad(Image* img);
            Quad quad;
            private:
            Image* image;
        };
};