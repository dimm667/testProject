#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderprogram.h"
#include "camera.h"
#include "texture.h"
#include "model.h"
#include "cube.h"
#include "fpsestimator.h"
#include "voxel.h"
#include "voxexporter.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

//#define STB_IMAGE_IMPLEMENTATION
//#include "deps/stb_image.h"

using namespace std;

struct Character {
    GLuint      textureID;
    glm::ivec2  size;
    glm::ivec2  bearing;
    GLuint      advance;
};

std::map<GLchar, Character> Characters;

//float vertices[] = {
//    // positions         // colors
//     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
//    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
//     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
//};

const GLuint screenWidth = 800;
const GLuint screenHeight = 600;

//float vertices[] = {
//    // positions          // colors           // texture coords
//     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
//};

//float vertices[] = {
//    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

//    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

//    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

//    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

//    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//};

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};


unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

//glm::vec3 cameraPosition(0.0F, 0.0F, 3.0F);
//glm::vec3 cameraFront(0.0F, 0.0F, -1.0F);
//glm::vec3 cameraUp(0.0F, 1.0F, 0.0F);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame



//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true,
//    // и приложение после этого закроется
//    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, GL_TRUE);

//    float cameraSpeed = 25.0f * deltaTime; // adjust accordingly
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        cameraPosition += cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        cameraPosition -= cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//}

Camera camera;

void processInput(GLFWwindow * window)
{
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true,
    // и приложение после этого закроется
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    Camera_Movement direction;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction = Camera_Movement::FORWARD;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction = Camera_Movement::BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction = Camera_Movement::LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction = Camera_Movement::RIGHT;

    camera.processKeyboard(direction, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    static float lastX = 400.0f, lastY = 300.0f;
    static float yaw = 0.0f, pitch = 0.0f;
    static bool firstMouse = true;

    if(firstMouse) // this bool variable is initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    camera.ProcessMouseMovement(xoffset, yoffset);
    lastX = xpos;
    lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


void RenderText(ShaderProgram &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, GLuint VAO, GLuint VBO)
{
    // Activate corresponding render state
    s.use();
    s.setUniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main()
{
    //Инициализация GLFW
    glfwInit();
    //Настройка GLFW
    //Задается минимальная требуемая версия OpenGL.
    //Мажорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Минорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Выключение возможности изменения размера окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    int vpWidth, vpHeight;
    glfwGetFramebufferSize(window, &vpWidth, &vpHeight);

    glViewport(0, 0, vpWidth, vpHeight);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    ShaderProgram modelShaderProgram({{"shaders/vertex.glsl", GL_VERTEX_SHADER},
                                         {"shaders/fragmentDiffuseOnly.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram cubeShaderProgram({{"shaders/vertexSimply.glsl", GL_VERTEX_SHADER},
                                       {"shaders/fragmentMaterial.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram cubeReflectionShaderProgram({{"shaders/vertexReflectSkyBox.glsl", GL_VERTEX_SHADER},
                                                  {"shaders/fragmentReflectSkyBox.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram cubeRefractionShaderProgram({{"shaders/vertexReflectSkyBox.glsl",GL_VERTEX_SHADER},
                                                  {"shaders/fragmentRefractSkyBox.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram glyphShaderProgram({{"shaders/vertexGlyph.glsl", GL_VERTEX_SHADER},
                                         {"shaders/fragmentGlyph.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram skyBoxShaderProgram({{"shaders/vertexSkyBox.glsl", GL_VERTEX_SHADER},
                                          {"shaders/fragmentSkyBox.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram boxInstancingShaderProgram({{"shaders/vertexInstancing.glsl", GL_VERTEX_SHADER},
                                            {"shaders/fragmentMaterialVertex.glsl", GL_FRAGMENT_SHADER}});

//    Model model("models/nanosuit/nanosuit.obj");

    Texture diffuseMap("textures/container2.png");
    Texture specularMap("textures/container2_specular.png");
    TextureCubeMap environment({"textures/skybox/right.jpg",
                                "textures/skybox/left.jpg",
                                "textures/skybox/top.jpg",
                                "textures/skybox/bottom.jpg",
                                "textures/skybox/front.jpg",
                                "textures/skybox/back.jpg"});



//    GLuint lightVAO;
//    glGenVertexArrays(1, &lightVAO);
//    glBindVertexArray(lightVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
    //    glBindVertexArray(0);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ------
    std:: vector<Cube> cubes;

    for(unsigned int x = 0; x < 100; ++x)
        for(unsigned int y = 0; y < 100; ++y)
            cubes.push_back(Cube(1.0F, glm::vec3(x * 1.0F, 0.5F, y * 1.0F), diffuseMap, specularMap));
//    for(unsigned int idx = 0; idx < 10; ++idx)
//    {
//        cubes.push_back(Cube(1.0F, glm::vec3(idx * 1.0F, 0.5F, idx * 0.0F), diffuseMap, specularMap));
//    }
    // ------
    SkyBox skyBox(environment);
    // ------


    //------------------- fonts  -----------------------
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

    FT_Face face;
    if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        char ch = static_cast<char>(c);
        if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
        {
            throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);




    glm::mat4 projectionGlyph = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //--------------------------------------------------

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FpsEstimator fps;

    // draw in wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);

    vox::VoxExporter voxModel("models/nature.vox");

    Image heightMap("textures/Heightmap.png");

    VoxelArray vArray(voxModel.getModel(0).getNumVoxels());
    unsigned int idx = 0;
    for(auto voxel : voxModel.getModel(0).voxels.voxel)
    {
        glm::vec3 position(static_cast<float>(voxel.component.x) * 0.1f,
                           static_cast<float>(voxel.component.z) * 0.1f,
                           static_cast<float>(voxel.component.y) * 0.1f);
        glm::vec3 color(0.5f);
        vArray.set(idx++, {position, color});
    }
//    VoxelArray2d vArray(heightMap.getWidth(), heightMap.getHeight());

//    for(unsigned int xIdx = 0; xIdx < heightMap.getWidth(); ++xIdx)
//        for(unsigned int yIdx = 0; yIdx < heightMap.getHeight(); ++yIdx)
//        {
//            float height = 0.0f;//std::round(heightMap.get(Image::PixelDataType::R, xIdx, yIdx) * 10.0f) * .1f;
//            vArray.set(xIdx, yIdx, {glm::vec3(xIdx * 0.1f, height, yIdx * 0.1f), glm::vec3(0.5f)});
////            vArray.set(xIdx, yIdx, {glm::vec3(xIdx * 0.1f, xIdx * 0.1f, yIdx * 0.1f), glm::vec3(0.5f)});
//        }





    while(!glfwWindowShouldClose(window))
    {
        fps.update();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);


        glfwPollEvents();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 localView = camera.getView();
        glm::mat4 projection = glm::perspective(camera.getFieldOfView(), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
//        glm::mat4 projection = glm::ortho(-static_cast<float>(screenWidth/2), static_cast<float>(screenWidth/2), -static_cast<float>(screenHeight/2), static_cast<float>(screenHeight/2), 0.1f, 1.0f);
//        glm::vec3 lightPos(1.0f, 1.0f * sin(glfwGetTime()), 2.0f);

//        glDepthFunc(GL_LEQUAL);
//        skyBoxShaderProgram.use();
//        skyBoxShaderProgram.setUniform("view", glm::mat4(glm::mat3(camera.getView())));
//        skyBoxShaderProgram.setUniform("projection", projection);
//        skyBox.draw(skyBoxShaderProgram);
//        glDepthFunc(GL_LESS);

//        boxInstancingShaderProgram.use();
//        boxInstancingShaderProgram.setUniform("view", localView);
//        boxInstancingShaderProgram.setUniform("projection", projection);
//        vArray.draw(boxInstancingShaderProgram);

        glm::vec3 pointLightPositions[] = {
//            glm::vec3( 0.7f,  1.0f * sin(glfwGetTime()),  2.0f),
//            glm::vec3( 1.0f * sin(glfwGetTime()), -3.3f, -4.0f),
//            glm::vec3(-4.0f,  2.0f, -12.0f),
//            glm::vec3( 1.0f * sin(glfwGetTime()),  0.0f, -1.0f * sin(glfwGetTime()))
            glm::vec3( 10.0f,  2.1f,  10.0f),
            glm::vec3( 10.0f,  2.1f,  10.0f),
            glm::vec3( 10.0f,  2.1f,  10.0f),
            glm::vec3( 10.0f,  2.1f,  10.0f),
        };

//        modelShaderProgram.use();
//        modelShaderProgram.setUniform("viewPos", camera.getPosition());
//        modelShaderProgram.setUniform("projection", projection);
//        modelShaderProgram.setUniform("view", localView);

//        environment.activate(GL_TEXTURE0);
//        cubeRefractionShaderProgram.use();
//        cubeRefractionShaderProgram.setUniform("camPosition", camera.getPosition());
//        cubeRefractionShaderProgram.setUniform("skyBox", environment.getBindUnit());
//        cubeRefractionShaderProgram.setUniform("projection", projection);
//        cubeRefractionShaderProgram.setUniform("view", localView);

//        glm::mat4 modelMtx(1.0f);
//        modelMtx = glm::translate(modelMtx, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
//        modelMtx = glm::scale(modelMtx, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
////        modelShaderProgram.setUniform("model", modelMtx);
//        cubeRefractionShaderProgram.setUniform("model", modelMtx);

//        model.draw(cubeRefractionShaderProgram);

        boxInstancingShaderProgram.use();
        boxInstancingShaderProgram.setUniform("viewPos", camera.getPosition());
        boxInstancingShaderProgram.setUniform("projection", projection);
        boxInstancingShaderProgram.setUniform("view", localView);
        boxInstancingShaderProgram.setUniform("directionalLight.direction", glm::vec3(-0.5f, -1.0f, -0.5f));
        boxInstancingShaderProgram.setUniform("directionalLight.ambient",  glm::vec3(0.1f, 0.1f, 0.1f));
        boxInstancingShaderProgram.setUniform("directionalLight.diffuse",  glm::vec3(0.2f, 0.2f, 0.2f));
        boxInstancingShaderProgram.setUniform("directionalLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        for(unsigned int idx = 0; idx < 4; idx++)
        {
            std::string variable = std::string("pointLights[") + std::to_string(idx) + std::string("].");
            boxInstancingShaderProgram.setUniform((variable + std::string("position")).c_str(), pointLightPositions[idx]);
            boxInstancingShaderProgram.setUniform((variable + std::string("ambient")).c_str(), glm::vec3(0.0f, 0.0f, 0.0f));
            boxInstancingShaderProgram.setUniform((variable + std::string("diffuse")).c_str(), glm::vec3(0.5f, 0.5f, 0.5f));
            boxInstancingShaderProgram.setUniform((variable + std::string("specular")).c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
            boxInstancingShaderProgram.setUniform((variable + std::string("constant")).c_str(), 1.0f);
            boxInstancingShaderProgram.setUniform((variable + std::string("linear")).c_str(), 0.01f);
            boxInstancingShaderProgram.setUniform((variable + std::string("quadratic")).c_str(), 0.12f);
        }

        boxInstancingShaderProgram.setUniform("spotLight.ambient",  glm::vec3(0.0f, 0.0f, 0.0f));
        boxInstancingShaderProgram.setUniform("spotLight.diffuse",  glm::vec3(0.0f, 0.0f, 0.0f)); // darken the light a bit to fit the scene
        boxInstancingShaderProgram.setUniform("spotLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
        boxInstancingShaderProgram.setUniform("spotLight.position", camera.getPosition());
        boxInstancingShaderProgram.setUniform("spotLight.direction", camera.getDirection());
        boxInstancingShaderProgram.setUniform("spotLight.cutOff",  glm::cos(glm::radians(12.5f)));
        boxInstancingShaderProgram.setUniform("spotLight.outerCutOff",  glm::cos(glm::radians(17.5f)));
        boxInstancingShaderProgram.setUniform("spotLight.constant",  1.0f);
        boxInstancingShaderProgram.setUniform("spotLight.linear",    0.09f);
        boxInstancingShaderProgram.setUniform("spotLight.quadratic", 0.032f);

        vArray.draw(boxInstancingShaderProgram);


//        for(auto cube : cubes)
//        {
//            cube.draw(cubeShaderProgram);
////            cube.draw(cubeRefractionShaderProgram);
//        }

////        shaderProgram.setUniform("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
////        shaderProgram.setUniform("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
////        shaderProgram.setUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
////        //shaderProgram.setUniform("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
////        //shaderProgram.setUniform("light.position", lightPos);
////        shaderProgram.setUniform("light.position", camera.getPosition());
////        shaderProgram.setUniform("light.direction", camera.getDirection());
////        shaderProgram.setUniform("light.cutOff",  glm::cos(glm::radians(12.5f)));
////        shaderProgram.setUniform("light.outerCutOff",  glm::cos(glm::radians(17.5f)));
////        shaderProgram.setUniform("light.constant",  1.0f);
////        shaderProgram.setUniform("light.linear",    0.09f);
////        shaderProgram.setUniform("light.quadratic", 0.032f);

//        glBindVertexArray(VAO);

//        for(unsigned int i = 0; i < 10; i++)
//        {
//          glm::mat4 localModel(1.0f);
//          localModel = glm::translate(localModel, cubePositions[i]);
//          float angle = 20.0f * i;
//          localModel = glm::rotate(localModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//          shaderProgram.setUniform("model", localModel);

//          glDrawArrays(GL_TRIANGLES, 0, 36);
//        }

//        // drawing lamp
//        shaderLamp.use();
//        shaderLamp.setUniform("projection", projection);
//        shaderLamp.setUniform("view", localView);
//        for(unsigned int idx = 0; idx < 4; idx++)
//        {
//            glm::mat4 model(1.0f);
//            model = glm::translate(model, pointLightPositions[idx]);
//            model = glm::scale(model, glm::vec3(0.2f));
//            shaderLamp.setUniform("model", model);

//            glBindVertexArray(lightVAO);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }

//        glBindVertexArray(0);



        glDisable(GL_DEPTH_TEST);
        glyphShaderProgram.use();
        glyphShaderProgram.setUniform("projection", projectionGlyph);
        glyphShaderProgram.setUniform("text", 0);
//        RenderText(glyphShaderProgram, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), VAO, VBO);
        RenderText(glyphShaderProgram, std::string("fps: ") + std::to_string(static_cast<int>(fps.get())), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), VAO, VBO);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
    }

    // Properly de-allocate all resources once they've outlived their purpose
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
