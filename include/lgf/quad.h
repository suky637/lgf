#pragma once
#include <glad/glad.h>
#include <lgf/window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace LGF {
    namespace Draw {
        enum class QuadType {
            ROUNDED,
            ROUGH
        };
        namespace Primitives {
            extern float rectangle[];
        };
        class Quad {
            public:
            Quad(const char* vertexShaderFile, const char* fragmentShaderFile);
            Quad(QuadType type);
            void linkWindow(LGF::LGFWindow* window);
            unsigned int getShaderID();
            void render();
            void setRect(const glm::vec2& pos, const glm::vec2& size);
            glm::mat4 model;
            private:
            LGF::LGFWindow* window;
            void init(const char* vertexShaderFile, const char* fragmentShaderFile);
            unsigned int VAO, VBO, shaderProgram;
        };
    };
};