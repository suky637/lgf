#include "lgf/text.h"
#include "lgf/files.h"

void LGF::Draw::Font::init(const char* fontFaceFile, int fontSize, const char* vertexShaderFile, const char* fragmentShaderFile) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "[CRITICAL] Error: Failed to init FreeType.\n";
        exit(1);
    }

    FT_Face face;
    if (FT_New_Face(ft, fontFaceFile, 0, &face)) {
        std::cerr << "[CRITICAL] Error: Font file couldn't be read or written, or it is unsupported.\n";
        exit(1);
    }
    else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "[CRITICAL] Error: Failed to load glyph.\n";
                exit(1);
            }

            unsigned int textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                textureID,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters.insert(std::pair<char, Character>(c, character));
        }
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: Failed to link program. message: " << infoLog << "\n";
    }

    glUseProgram(shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void LGF::Draw::Font::renderText(std::string text, float x, float y, float scale, glm::vec4 colour) {
    glUseProgram(shader);
    matrix();
    glUniform4f(glGetUniformLocation(shader, "textColour"), colour.x, colour.y, colour.z, colour.w);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void LGF::Draw::Font::matrix() {
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(window->proj * window->view));
}

LGF::Draw::Font::Font(const char* fontFaceFile, int fontSize, LGF::LGFWindow* window, const char* vertexShaderFile, const char* fragmentShaderFile) {
    this->window = window;
    this->init(fontFaceFile, fontSize, vertexShaderFile, fragmentShaderFile);
}

LGF::Draw::Font::Font(const char* fontFaceFile, int fontSize, LGF::LGFWindow* window) {
    this->window = window;
    this->init(fontFaceFile, fontSize, "shaders/text.vert", "shaders/text.frag");
}