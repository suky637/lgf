#include "lgf/quad.h"
#include "lgf/files.h"
#include <iostream>


namespace LGF {
    namespace Draw {
        namespace Primitives {
            float rectangle[] = {
                -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
                 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
                 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
                 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
                -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
                -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
            };
        };
    };
};

void LGF::Draw::Quad::linkWindow(LGF::LGFWindow* window) {
    this->window = window;
}

void LGF::Draw::Quad::init(const char* vertexShaderFile, const char* fragmentShaderFile) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LGF::Draw::Primitives::rectangle), LGF::Draw::Primitives::rectangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderSource = LGF::IO::readFile(vertexShaderFile);
    const char* vertexShaderSourceC = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderSource = LGF::IO::readFile(fragmentShaderFile);
    const char* fragmentShaderSourceC = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR: Failed to link program. message: " << infoLog << "\n";
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    model = glm::mat4(1.f);

    glUniform4f(glGetUniformLocation(shaderProgram, "colour"), 1.f, 1.f, 1.f, 1.f);
}
LGF::Draw::Quad::Quad(const char* vertexShaderFile, const char* fragmentShaderFile) {
    this->init(vertexShaderFile, fragmentShaderFile);
}


LGF::Draw::Quad::Quad(LGF::Draw::QuadType type) {
    if (type == LGF::Draw::QuadType::ROUNDED) {
        this->init("shaders/rounded.vert", "shaders/rounded.frag");
    } else if (type == LGF::Draw::QuadType::ROUGH) {
        this->init("shaders/rough.vert", "shaders/rough.frag");
    }
}

unsigned int LGF::Draw::Quad::getShaderID() {
    return shaderProgram;
}

void LGF::Draw::Quad::setRect(const glm::vec2& pos, const glm::vec2& size) {
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(pos, 0.f));
    model = glm::scale(model, glm::vec3(size, 1.f));
}

void LGF::Draw::Quad::render() {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "proj"), 1, GL_FALSE, glm::value_ptr(window->proj));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(window->view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

LGF::Draw::Quad::~Quad() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}