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
#include "imageloader.h"
#include "uniformobject.h"
#include "viewport.h"

#include "shaders/UniformStructures/matrices.h"

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
    GLint      advance;
};

std::map<GLchar, Character> Characters;

const GLuint screenWidth = 800;
const GLuint screenHeight = 600;

struct TDebugFlags
{
    bool enableDirectShadowComputing;
    bool enablePointShadowComputing;
    bool enableSceneRendering;
} debug_flags = {true, true, true};

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

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


Camera camera(glm::vec3(1.0f, 0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0, 0.0);

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

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        debug_flags.enableDirectShadowComputing = !debug_flags.enableDirectShadowComputing;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        debug_flags.enablePointShadowComputing = !debug_flags.enablePointShadowComputing;

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        debug_flags.enableSceneRendering = !debug_flags.enableSceneRendering;


    camera.processKeyboard(direction, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    static float lastX = 400.0f, lastY = 300.0f;
    static float yaw = 0.0f, pitch = 0.0f;
    static bool firstMouse = true;

    bool buttonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if(firstMouse && buttonPressed) // this bool variable is initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
//        camera.ProcessMouseMovement(xpos, ypos);
    }
    else if(!firstMouse && !buttonPressed)
    {
        firstMouse = true;
    }

    if(buttonPressed)
    {
        float xoffset = lastX - xpos;
        float yoffset = ypos - lastY; // reversed since y-coordinates range from bottom to top
        camera.ProcessMouseMovement(xoffset * 0.1f, yoffset);
        lastX = xpos;
        lastY = ypos;
    }


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

    Viewport mainVeiwPort(0, 0, vpWidth, vpHeight);
//    glViewport(0, 0, vpWidth, vpHeight);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Uniform bufers
    TViewProjMatrices viewProjMatrix;
    UniformObject<TViewProjMatrices> uniformObj(viewProjMatrix);

    // uniform proxys
    auto veiwProxy = uniformObj.getProxy<glm::mat4, offsetof(TViewProjMatrices, view), sizeof(glm::mat4)>();
    auto projectionProxy = uniformObj.getProxy<glm::mat4, offsetof(TViewProjMatrices, projection), sizeof(glm::mat4)>();
    auto projViewProxy = uniformObj.getProxy<glm::mat4, offsetof(TViewProjMatrices, proj_view), sizeof(glm::mat4)>();


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
    uniformObj.bindToShader(skyBoxShaderProgram, "TViewProjMatrices");

    ShaderProgram boxInstancingShaderProgram({{"shaders/vertexInstancing2.glsl", GL_VERTEX_SHADER},
                                            {"shaders/fragmentMaterialVertex.glsl", GL_FRAGMENT_SHADER}});
    uniformObj.bindToShader(boxInstancingShaderProgram, "TViewProjMatrices");


    ShaderProgram directionShadowShaderProgram({{"shaders/vertexDirectionLight.glsl", GL_VERTEX_SHADER},
                                            {"shaders/fragmentDirectionLight.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram textureShaderProgram({{"shaders/vertexQuad.glsl", GL_VERTEX_SHADER},
                                        {"shaders/fragmentQuad.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram pointLightShadowShaderProgram({{"shaders/vertexPointLightShadow.glsl", GL_VERTEX_SHADER},
                                                 {"shaders/geometryPointLightShadow.glsl", GL_GEOMETRY_SHADER},
                                                 {"shaders/fragmentPointLightShadow.glsl", GL_FRAGMENT_SHADER}});

    ShaderProgram defferedGeometryPassShaderProgram({{"shaders/vertexInstancing2.glsl", GL_VERTEX_SHADER},
                                              {"shaders/fragmentDefferedShading.glsl", GL_FRAGMENT_SHADER}});
    uniformObj.bindToShader(defferedGeometryPassShaderProgram, "TViewProjMatrices");

    ShaderProgram defferedLightingPassShaderProgram({{"shaders/vertexQuad.glsl", GL_VERTEX_SHADER},
                                                    {"shaders/fragmentMaterialVertexDeffered.glsl", GL_FRAGMENT_SHADER}});

//    Model model("models/nanosuit/nanosuit.obj");

    TextureImage diffuseMap("textures/container2.png");
    TextureImage specularMap("textures/container2_specular.png");
    TextureCubeMap environment({"textures/skybox/lake/right.jpg",
                                "textures/skybox/lake/left.jpg",
                                "textures/skybox/lake/top.jpg",
                                "textures/skybox/lake/bottom.jpg",
                                "textures/skybox/lake/front.jpg",
                                "textures/skybox/lake/back.jpg"});

//    TextureCubeMap environment({"textures/skybox/sor_cwd/cwd_rt.jpg",
//                                "textures/skybox/sor_cwd/cwd_lf.jpg",
//                                "textures/skybox/sor_cwd/cwd_up.jpg",
//                                "textures/skybox/sor_cwd/cwd_dn.jpg",
//                                "textures/skybox/sor_cwd/cwd_ft.jpg",
//                                "textures/skybox/sor_cwd/cwd_bk.jpg"});



//    GLuint lightVAO;
//    glGenVertexArrays(1, &lightVAO);
//    glBindVertexArray(lightVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//        glBindVertexArray(0);

    float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            };
    // setup plane VAO
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
            glm::ivec2(static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows)),
            glm::ivec2(static_cast<int>(face->glyph->bitmap_left), static_cast<int>(face->glyph->bitmap_top)),
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
//    glDisable(GL_BLEND);

    FpsEstimator fps;

    // draw in wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);

    vox::VoxExporter voxModel("models/castle.vox");

    ImageLoader heightMap("textures/myHeightmap.png");
//    ImageLoader heightMap("textures/5275-heightmap-test.png");

    // ============== voxel Model loader =======================
//    VoxelArray vArray(voxModel.getModel(0).getNumVoxels());
//    unsigned int idx = 0;
//    for(auto voxel : voxModel.getModel(0).voxels.voxel)
//    {
//        glm::vec3 position(static_cast<float>(voxel.component.x) * 0.1f,
//                           static_cast<float>(voxel.component.z) * 0.1f,
//                           static_cast<float>(voxel.component.y) * 0.1f);
//        vox::Color vColor = voxModel.getColor(voxel.component.colorIdx);
//        glm::vec3 color(static_cast<float>(vColor.component.r) / 255.0f,
//                        static_cast<float>(vColor.component.g) / 255.0f,
//                        static_cast<float>(vColor.component.b) / 255.0f);
//        vArray.set(idx++, {position, color});
//    }
    // ==========================================================

    // ============== ground surface loader =====================
    float voxelSize = 0.1f;
    float numberOfLevels = 10.0F;
//    VoxelArray2d vArray(voxelSize);
    VoxelArray3d vArray(voxelSize);
    for(unsigned int xIdx = 0; xIdx < heightMap.getWidth(); ++xIdx)
        for(unsigned int yIdx = 0; yIdx < heightMap.getHeight(); ++yIdx)
        {
            unsigned int height = std::round(heightMap.getPixel(ImageLoader::PixelDataType::R, xIdx, yIdx) * numberOfLevels);
            float color = heightMap.getPixel(ImageLoader::PixelDataType::R, xIdx, yIdx);
//            vArray.set(xIdx, yIdx, {glm::vec3(xIdx, height, yIdx) * voxelSize, glm::vec3(0.0f, height/numberOfLevels, 1.0f - height/numberOfLevels)});
            vArray.setHeigth(xIdx, yIdx, height, glm::vec3(0.0, color, 1.0 - color));

//            vArray.set(xIdx, yIdx, {glm::vec3(xIdx * 0.1f, xIdx * 0.1f, yIdx * 0.1f), glm::vec3(0.5f)});
        }
//    vArray.setHeigth(0, 0, 0, glm::vec3(0.5, 0.5, 0.5), glm::vec3(3.0, 1.0, 1.0));
//    vArray.setHeigth(1, 1, 0, glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 2.0, 1.0));
//    vArray.setHeigth(2, 2, 0, glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 1.0, 4.0));

    vArray.construct();
    // ==========================================================


    // --------------------------direction light shadow----------------------------------------------
//    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    const GLuint SHADOW_WIDTH = 256, SHADOW_HEIGHT = 256;
    Viewport shadowsRenderViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    FrameBuffer directLightShadowFB(GL_FRAMEBUFFER);
    TextureBuffer directLightShadow(SHADOW_WIDTH, SHADOW_HEIGHT/*, directLightShadowFB*/);
//    directLightShadowFB.bind();
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, directLightShadow.getId(), 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    directLightShadowFB.unbind();
    directLightShadowFB.attachTextures2D({{.attachTo = GL_DEPTH_ATTACHMENT, .texture = directLightShadow.getId()}});

    float near_plane = 0.0f, far_plane = 30.0f;
    glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(glm::vec3( 10.0f, 20.0f,  10.0f),
                                      glm::vec3( 0.0f, 0.0f,  0.0f),
                                      glm::vec3( 0.0f, 1.0f,  0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    // ----------------------------------------------------------------------------------------------

    // ------------------- point light shadows ------------------------------------------------------
    FrameBuffer pointLightShadowFB(GL_FRAMEBUFFER);
    TextureCubeMapBuffer pointLightShadow(SHADOW_WIDTH, SHADOW_HEIGHT, pointLightShadowFB);


    float aspect = (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT;
    float near_p = 0.1f;
    float far_p = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_p, far_p);
    std::vector<glm::mat4> shadowTransforms;

    glm::vec3 lightPos( 3.0f,  3.0f,  3.0f);

    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                     glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));


    //-----------------------------------------------------------------------------------------------


    // deffered shading prepare
    FrameBuffer defferedShadingFB(GL_FRAMEBUFFER);
    TextureBuffer positionBuff(screenWidth, screenHeight, {.internalFormat=GL_RGB16F, .pixelFormat=GL_RGB, .pixelType=GL_FLOAT});
    TextureBuffer normalBuff(screenWidth, screenHeight, {.internalFormat=GL_RGB16F, .pixelFormat=GL_RGB, .pixelType=GL_FLOAT});
    TextureBuffer albedoSpecBuff(screenWidth, screenHeight, {.internalFormat=GL_RGBA, .pixelFormat=GL_RGBA, .pixelType=GL_UNSIGNED_BYTE});

    RenderBuffer defferedShadingRB(screenWidth, screenHeight, GL_DEPTH_COMPONENT);
    defferedShadingFB.attachTextures2D({{GL_COLOR_ATTACHMENT0, positionBuff.getId()}, {GL_COLOR_ATTACHMENT1, normalBuff.getId()}, {GL_COLOR_ATTACHMENT2, albedoSpecBuff.getId()}});
    defferedShadingFB.attachRenderBuffer(defferedShadingRB, GL_DEPTH_ATTACHMENT);

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

        glm::vec3 pointLightPositions[] = {lightPos,
//            glm::vec3( 0.7f,  1.0f * sin(glfwGetTime()),  2.0f),
//            glm::vec3( 1.0f * sin(glfwGetTime()), -3.3f, -4.0f),
//            glm::vec3(-4.0f,  2.0f, -12.0f),
//            glm::vec3( 1.0f * sin(glfwGetTime()),  0.0f, -1.0f * sin(glfwGetTime()))
//            glm::vec3( 10.0f,  2.1f,  10.0f),
//            glm::vec3( 10.0f,  2.1f,  10.0f),
//            glm::vec3( 10.0f,  2.1f,  10.0f),
//            glm::vec3( 10.0f,  2.1f,  10.0f),
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


        if(debug_flags.enableDirectShadowComputing)
        {
            // render directional light shadow map
            glCullFace(GL_FRONT);
            shadowsRenderViewport.activate();
            directLightShadowFB.bind();
            glClear(GL_DEPTH_BUFFER_BIT);
            directionShadowShaderProgram.use();
            directionShadowShaderProgram.setUniform("lightSpaceMatrix", lightSpaceMatrix);
            vArray.draw();
            directLightShadowFB.unbind();
            glCullFace(GL_BACK);
        }

        if(debug_flags.enablePointShadowComputing)
        {
            // render point light shadow map
            shadowsRenderViewport.activate();
            pointLightShadowFB.bind();
            glClear(GL_DEPTH_BUFFER_BIT);
            pointLightShadowShaderProgram.use();
            pointLightShadowShaderProgram.setUniform("far_plane", far_p);
            pointLightShadowShaderProgram.setUniform("lightPos", lightPos);
            for (unsigned int i = 0; i < 6; ++i)
                pointLightShadowShaderProgram.setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            vArray.draw();
            pointLightShadowFB.unbind();
        }

        // render scene
        mainVeiwPort.activate();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getView();
        glm::mat4 viewSkybox = glm::mat4(glm::mat3(camera.getView()));    // for skybbox
        glm::mat4 projection = glm::perspective(camera.getFieldOfView(), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

        glm::mat4 pv = projection * viewSkybox;
#if 0
        // TODO: think about this workaround
        projViewProxy = pv;



        glDepthFunc(GL_LEQUAL);
        skyBoxShaderProgram.use();
        skyBox.draw(skyBoxShaderProgram);
        glDepthFunc(GL_LESS);
#endif

        pv = projection * view;
        // TODO: think about this workaround
        projViewProxy = pv;
#if 0   // geometry pass of the deffered shading
        {
            bool isEnabled = glIsEnabled(GL_BLEND);
            glDisable(GL_BLEND);
            defferedShadingFB.bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            defferedGeometryPassShaderProgram.use();
            defferedGeometryPassShaderProgram.setUniform("lightSpaceMatrix", lightSpaceMatrix);
            vArray.draw();
            defferedShadingFB.unbind();
            if(isEnabled) glEnable(GL_BLEND);
        }
#endif
#if 0

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        positionBuff.activate(GL_TEXTURE0);
        textureShaderProgram.use();
        textureShaderProgram.setUniform("screenTexture", positionBuff.getBindUnitID());
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
#endif

#if 0   // lighting pass of the deffered shading
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        positionBuff.activate(GL_TEXTURE0);
        normalBuff.activate(GL_TEXTURE1);
        albedoSpecBuff.activate(GL_TEXTURE2);

        defferedLightingPassShaderProgram.use();
        defferedLightingPassShaderProgram.setUniform("gPosition", positionBuff.getBindUnitID());
        defferedLightingPassShaderProgram.setUniform("gNormal", normalBuff.getBindUnitID());
        defferedLightingPassShaderProgram.setUniform("gAlbedoSpec", albedoSpecBuff.getBindUnitID());

        defferedLightingPassShaderProgram.setUniform("viewPos", camera.getPosition());
        defferedLightingPassShaderProgram.setUniform("directionalLight.direction", glm::vec3(-0.5f, -1.0f, -0.5f));
        defferedLightingPassShaderProgram.setUniform("directionalLight.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
        defferedLightingPassShaderProgram.setUniform("directionalLight.diffuse",  glm::vec3(0.0f, 0.0f, 0.0f));
        defferedLightingPassShaderProgram.setUniform("directionalLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

        for(unsigned int idx = 0; idx < 1; idx++)
        {
            std::string variable = std::string("pointLights[") + std::to_string(idx) + std::string("].");
            defferedLightingPassShaderProgram.setUniform((variable + std::string("position")).c_str(), pointLightPositions[idx]);
            defferedLightingPassShaderProgram.setUniform((variable + std::string("ambient")).c_str(), glm::vec3(0.1f, 0.1f, 0.1f));
            defferedLightingPassShaderProgram.setUniform((variable + std::string("diffuse")).c_str(), glm::vec3(0.5f, 0.5f, 0.5f));
            defferedLightingPassShaderProgram.setUniform((variable + std::string("specular")).c_str(), glm::vec3(0.7f, 0.7f, 0.7f));
            defferedLightingPassShaderProgram.setUniform((variable + std::string("constant")).c_str(), 1.0f);
            defferedLightingPassShaderProgram.setUniform((variable + std::string("linear")).c_str(), 0.01f);
            defferedLightingPassShaderProgram.setUniform((variable + std::string("quadratic")).c_str(), 0.12f);
        }

        defferedLightingPassShaderProgram.setUniform("spotLight.ambient",  glm::vec3(0.0f, 0.0f, 0.0f));
        defferedLightingPassShaderProgram.setUniform("spotLight.diffuse",  glm::vec3(0.0f, 0.0f, 0.0f)); // darken the light a bit to fit the scene
        defferedLightingPassShaderProgram.setUniform("spotLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
        defferedLightingPassShaderProgram.setUniform("spotLight.position", camera.getPosition());
        defferedLightingPassShaderProgram.setUniform("spotLight.direction", camera.getDirection());
        defferedLightingPassShaderProgram.setUniform("spotLight.cutOff",  glm::cos(glm::radians(12.5f)));
        defferedLightingPassShaderProgram.setUniform("spotLight.outerCutOff",  glm::cos(glm::radians(17.5f)));
        defferedLightingPassShaderProgram.setUniform("spotLight.constant",  1.0f);
        defferedLightingPassShaderProgram.setUniform("spotLight.linear",    0.09f);
        defferedLightingPassShaderProgram.setUniform("spotLight.quadratic", 0.032f);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
#endif

        if(debug_flags.enableSceneRendering)
        {
            boxInstancingShaderProgram.use();
            boxInstancingShaderProgram.setUniform("lightSpaceMatrix", lightSpaceMatrix);
            boxInstancingShaderProgram.setUniform("viewPos", camera.getPosition());
            boxInstancingShaderProgram.setUniform("directionalLight.direction", glm::vec3(-0.5f, -1.0f, -0.5f));
            boxInstancingShaderProgram.setUniform("directionalLight.ambient",  glm::vec3(0.3f, 0.3f, 0.3f));
            boxInstancingShaderProgram.setUniform("directionalLight.diffuse",  glm::vec3(0.0f, 0.0f, 0.0f));
            boxInstancingShaderProgram.setUniform("directionalLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

            for(unsigned int idx = 0; idx < 1; idx++)
            {
                std::string variable = std::string("pointLights[") + std::to_string(idx) + std::string("].");
                boxInstancingShaderProgram.setUniform((variable + std::string("position")).c_str(), pointLightPositions[idx]);
                boxInstancingShaderProgram.setUniform((variable + std::string("ambient")).c_str(), glm::vec3(0.1f, 0.1f, 0.1f));
                boxInstancingShaderProgram.setUniform((variable + std::string("diffuse")).c_str(), glm::vec3(0.5f, 0.5f, 0.5f));
                boxInstancingShaderProgram.setUniform((variable + std::string("specular")).c_str(), glm::vec3(0.7f, 0.7f, 0.7f));
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

            directLightShadow.activate(GL_TEXTURE0);
            pointLightShadow.activate(GL_TEXTURE1);

            boxInstancingShaderProgram.setUniform("shadowMap", directLightShadow.getBindUnitID());
            boxInstancingShaderProgram.setUniform("depthMap", pointLightShadow.getBindUnitID());
            boxInstancingShaderProgram.setUniform("far_plane", far_p);

            vArray.draw();
        }

#if 1
        glDisable(GL_DEPTH_TEST);
        glyphShaderProgram.use();
        glyphShaderProgram.setUniform("projection", projectionGlyph);
        glyphShaderProgram.setUniform("text", 0);
        static unsigned long long count = 0;
        RenderText(glyphShaderProgram, std::string("fps: ") + std::to_string(static_cast<int>(fps.get())), 25.0f, 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f), VAO, VBO);
        RenderText(glyphShaderProgram, std::string("cnt: ") + std::to_string(count++), 25.0f, 60.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f), VAO, VBO);
        RenderText(glyphShaderProgram, std::string("direct shadow: ") + (debug_flags.enableDirectShadowComputing ? "enabled" : "disabled"), 25.0f, 75.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f), VAO, VBO);
        RenderText(glyphShaderProgram, std::string("point shadow: ") + (debug_flags.enablePointShadowComputing ? "enabled" : "disabled"), 25.0f, 90.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f), VAO, VBO);
        RenderText(glyphShaderProgram, std::string("scene rendering: ") + (debug_flags.enableSceneRendering ? "enabled" : "disabled"), 25.0f, 105.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f), VAO, VBO);
        glEnable(GL_DEPTH_TEST);
#endif
        GLenum  error = glGetError();

        if(error != GL_NO_ERROR)
        {
//            throw std::runtime_error("OpenGL error: " + std::to_string(error));
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
