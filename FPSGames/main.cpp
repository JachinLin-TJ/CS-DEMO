#include"include/glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/learnopengl/camera.h"
#include "include/learnopengl/filesystem.h"
#include "include/learnopengl/model.h"
#include "include/learnopengl/shader_m.h"

#include "include/my/car.h"
#include "include/my/fixed_camera.h"
#include "include/stb_image.h"
#include <iostream>
#include "GLFW/glfw3.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "assimp.lib")
#define DEBUG_MODE
// ------------------------------------------
// 函数声明
// ------------------------------------------

GLFWwindow* windowInit();
bool init();
void depthMapFBOInit();
void skyboxInit();

void setDeltaTime();
void updateFixedCamera();

// 使用“&”引用性能更好
void renderLight(Shader& shader);
void renderGunAndCamera(Model& carModel, Model& cameraModel, Shader& shader);
void renderCamera(Model& model, glm::mat4 modelMatrix, Shader& shader);
void renderGun(Model& model, glm::mat4 modelMatrix, Shader& shader);
void renderGun(Model& model, Shader& shader);
void renderMap(Model& model, Shader& shader);
void renderSkyBox(Shader& shader);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);

void handleKeyInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned int loadCubemap(vector<std::string> faces);

// ------------------------------------------
// 全局变量
// ------------------------------------------

// 窗口尺寸
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// 渲染阴影时的窗口分辨率（会影响阴影的锯齿边情况）
const unsigned int SHADOW_WIDTH = 1024 * 10;
const unsigned int SHADOW_HEIGHT = 1024 * 10;

// 是否为线框图模式
bool isPolygonMode = false;

// 世界坐标系单位向量
glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
glm::vec3 WORLD_X(1.0f, 0.0f, 0.0f);

// 汽车
Car car(glm::vec3(0.0f, 0.05f, 0.0f));

// 相机
glm::vec3 cameraPos(0.0f, 2.0f, 5.0f);
Camera camera(cameraPos);
FixedCamera fixedCamera(cameraPos);


// 光照相关属性
glm::vec3 lightPos(-1.0f, 1.0f, -1.0f);
glm::vec3 lightDirection = glm::normalize(lightPos);
glm::mat4 lightSpaceMatrix;

// 深度Map的ID
unsigned int depthMap;
unsigned int depthMapFBO;

// 将鼠标设置在屏幕中心
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing 用来平衡不同电脑渲染水平所产生的速度变化
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 天空盒
unsigned int cubemapTexture;
unsigned int skyboxVAO, skyboxVBO;

// 天空盒顶点数据
const float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

// 天空盒的面数据
const vector<std::string> faces{
    FileSystem::getPath("asset/textures/skybox/right.tga"),
    FileSystem::getPath("asset/textures/skybox/left.tga"),
    FileSystem::getPath("asset/textures/skybox/top.tga"),
    FileSystem::getPath("asset/textures/skybox/bottom.tga"),
    FileSystem::getPath("asset/textures/skybox/front.tga"),
    FileSystem::getPath("asset/textures/skybox/back.tga")
};

// ------------------------------------------
// main函数
// ------------------------------------------

int main()
{
    // ------------------------------
    // 初始化
    // ------------------------------

    // 窗口初始化
    GLFWwindow* window = windowInit();
    // OpenGL初始化
    bool isInit = init();
    if (window == NULL || !isInit) {
        return -1;
    }
    // 深度Map的FBO配置
    depthMapFBOInit();
    // 天空盒的配置
    skyboxInit();

    // ------------------------------
    // 构建和编译着色器
    // ------------------------------

    // 为所有物体添加光照和阴影的shader
    // BUG:在地图上无法显示阴影
    Shader shader("shader/light_and_shadow.vs", "shader/light_and_shadow.fs");
    // 从太阳平行光角度生成深度信息的shader 
    //Shader depthShader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");
    // 天空盒shader
    Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");

    // ------------------------------
    // 模型加载
    // ------------------------------

    // 相机模型
    Model cameraModel(FileSystem::getPath("asset/models/obj/camera-cube/camera-cube.obj"));
    // 地图模型
    Model mapModel(FileSystem::getPath("asset/models/obj/Map/test.obj"));
    // 敌人
    Model enemyModel(FileSystem::getPath("asset/models/obj/Enemy/Pirate.obj"));

    // 枪械模型
    // Model another_rifleModel(FileSystem::getPath("asset/models/obj/ACPGun/Handgun_obj.obj"));
    // Model another_rifleModel(FileSystem::getPath("asset/models/obj/DragonSniper/AWP_Dragon_Lore.obj"));
    Model rifleModel(FileSystem::getPath("asset/models/obj/AK47/AK47.obj"));


    // ---------------------------------
    // shader 纹理配置
    // ---------------------------------

    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMap", 15); // 这里的15是指"GL_TEXTURE15"，需要与后面的对应

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // ---------------------------------
    // 循环渲染
    // ---------------------------------

    while (!glfwWindowShouldClose(window)) {
        // 计算一帧的时间长度以便于使帧绘制速度均匀
        setDeltaTime();

        // 监听按键
        handleKeyInput(window);

        // 渲染背景
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---------------------------------
        // 渲染获得场景的深度信息
        // ---------------------------------

        // 定义光源视见体，即阴影生成范围的正交投影矩阵
        glm::mat4 lightProjection = glm::ortho(
            -200.0f, 200.0f,
            -200.0f, 200.0f,
            -200.0f, 200.0f);
        // TODO lightPos跟随相机位置进行移动，使相机周围的地方总会生成影子
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), WORLD_UP);
        lightSpaceMatrix = lightProjection * lightView;

        // 从光源角度渲染整个场景
        //depthShader.use();
        //depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // 改变视口大小以便于进行深度的渲染
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        // 使用深度shader渲染生成场景
        glClear(GL_DEPTH_BUFFER_BIT);

        //renderGunAndCamera(rifleModel, cameraModel, depthShader);
        //renderMap(mapModel, depthShader);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 复原视口
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---------------------------------
        // 模型渲染
        // ---------------------------------

        shader.use();

        // 设置光照相关属性
        renderLight(shader);
        
        car.UpdateDelayYaw();
        car.UpdateDelayPitch();
        car.UpdateDelayPosition();


        updateFixedCamera();

        // 使用shader渲染Gun和Camera（层级模型）
        renderGunAndCamera(rifleModel, cameraModel, shader);
    
        // 渲染地图
        renderMap(mapModel, shader);

        // --------------
        // 最后再渲染天空盒

        // 改变深度测试，使深度等于1.0时为无穷远
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        renderSkyBox(skyboxShader);
        // 复原深度测试
        glDepthFunc(GL_LESS);

        // 交换缓冲区和调查IO事件（按下的按键,鼠标移动等）
        glfwSwapBuffers(window);

        // 轮询事件
        glfwPollEvents();
    }

    // 关闭glfw
    glfwTerminate();
    return 0;
}

// ------------------------------------------
// 其他函数
// ------------------------------------------


// ---------------------------------
// 初始化
// ---------------------------------

GLFWwindow* windowInit()
{
    // 初始化配置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, u8"FPSDemo", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        system("pause");
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouseButton_callback);
    

    // 令GLFW捕捉用户的鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

bool init()
{
    // 加载所有OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        system("pause");
        return false;
    }

    // 配置全局openGL状态
    glEnable(GL_DEPTH_TEST);

    return true;
}

// 深度图配置
void depthMapFBOInit()
{
    glGenFramebuffers(1, &depthMapFBO);
    // 创建深度纹理
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // 把生成的深度纹理作为帧缓冲的深度缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 天空盒配置
void skyboxInit()
{
    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // 纹理加载
    cubemapTexture = loadCubemap(faces);
}

// ---------------------------------
// 时间相关函数
// ---------------------------------

// 计算一帧的时间长度
void setDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}



// ---------------------------------
// 相机位置更新
// ---------------------------------


void updateFixedCamera()
{
    // 自动逐渐复原Zoom为默认值
    camera.ZoomRecover();
    // 处理相机相对于车坐标系下的向量坐标转换为世界坐标系下的向量
    float angle = glm::radians(-car.getMidValYaw());
    glm::mat4 rotateMatrix(
        cos(angle), 0.0, sin(angle), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(angle), 0.0, cos(angle), 0.0,
        0.0, 0.0, 0.0, 1.0);
    glm::vec3 rotatedPosition = glm::vec3(rotateMatrix * glm::vec4(fixedCamera.getPosition(), 1.0));

    camera.FixView(rotatedPosition + car.getMidValPosition(), fixedCamera.getYaw() + car.getMidValYaw(),car.getMidValPitch());
}

// ---------------------------------
// 渲染函数
// ---------------------------------

// 设置光照相关属性
void renderLight(Shader& shader)
{
    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("lightDirection", lightDirection);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}




void renderGunAndCamera(Model& gunModel, Model& cameraModel, Shader& shader)
{
    // 视图转换
    glm::mat4 viewMatrix = camera.GetViewMatrix();
    shader.setMat4("view", viewMatrix);
    // 投影转换
    glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
    shader.setMat4("projection", projMatrix);

    // -------
    // 层级建模

    // 模型转换
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, car.getMidValPosition());
    modelMatrix = glm::rotate(modelMatrix, glm::radians(car.getDelayYaw() / 2), WORLD_UP);

    // 渲染枪支
    renderGun(gunModel, modelMatrix, shader);

    // 由于mat4作函数参数时为值传递，故不需要备份modelMatrix

    // 渲染相机
    renderCamera(cameraModel, modelMatrix, shader);
}


void renderCamera(Model& model, glm::mat4 modelMatrix, Shader& shader)
{
    modelMatrix = glm::rotate(modelMatrix, glm::radians(fixedCamera.getYaw() + car.getYaw() / 2), WORLD_UP);
    modelMatrix = glm::translate(modelMatrix, cameraPos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));

    // 应用变换矩阵
    shader.setMat4("model", modelMatrix);

    model.Draw(shader);
}



void GunRotate(glm::mat4 & modelMatrix,const glm::vec3  &Point, float degree)
{
    modelMatrix = glm::translate(modelMatrix, Point);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), WORLD_X);
    modelMatrix = glm::translate(modelMatrix, -Point);

}
void renderGun(Model& model, glm::mat4 modelMatrix, Shader& shader)
{
    modelMatrix = glm::rotate(modelMatrix, glm::radians(car.getYaw() - car.getDelayYaw() / 2), WORLD_UP);
    GunRotate(modelMatrix, glm::vec3(0.0f, 0.0f, 4.0f), (car.getPitch() - car.getDelayPitch() / 2));

    // modelMatrix = glm::rotate(modelMatrix, glm::radians(-(car.getPitch() - car.getDelayPitch() / 2)), WORLD_X);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 1.5f, 4.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-180.0f), WORLD_UP);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));

    // GunRotate(modelMatrix, glm::vec3(0.0f, 0.0f, 10.0f), -(car.getPitch() - car.getDelayPitch() / 2));
    // 
    // 应用变换矩阵
    shader.setMat4("model", modelMatrix);

    model.Draw(shader);
}

void renderGun(Model& model, Shader& shader)
{
    // 视图转换
    glm::mat4 viewMatrix = camera.GetViewMatrix();
    shader.setMat4("view", viewMatrix);
    // 模型转换
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 1.5f, -7.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-180.0f), WORLD_UP);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
    shader.setMat4("model", modelMatrix);
    // 投影转换
    glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
    shader.setMat4("projection", projMatrix);

    model.Draw(shader);
}
void renderMap(Model& model, Shader& shader)
{
    // 视图转换
    glm::mat4 viewMatrix = camera.GetViewMatrix();
    shader.setMat4("view", viewMatrix);
    // 模型转换
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    shader.setMat4("model", modelMatrix);
    // 投影转换
    glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
    shader.setMat4("projection", projMatrix);

    model.Draw(shader);
}

void renderSkyBox(Shader& shader)
{
    // viewMatrix 通过构造，移除相机的移动
    glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    // 投影
    glm::mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);

    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projMatrix);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// ---------------------------------
// 键盘/鼠标监听
// ---------------------------------

void handleKeyInput(GLFWwindow* window)
{
    // esc退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
   
    //移动
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        car.ProcessKeyboard(CAR_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        car.ProcessKeyboard(CAR_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        car.ProcessKeyboard(CAR_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        car.ProcessKeyboard(CAR_BACKWARD, deltaTime);

    // 回调监听按键（一个按键只会触发一次事件）
    glfwSetKeyCallback(window, key_callback);
}

// 按键回调函数，使得一次按键只触发一次事件
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //预留给换弹
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {  
#ifdef DEBUG_MODE
        std::cout << "Pressed R" << std::endl;
#endif
        ;
    }
    
}

// 鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //if (!isCameraFixed) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // 坐标翻转以对应坐标系

        lastX = xpos;
        lastY = ypos;

        car.ProcessMouseMovement(xoffset, yoffset);
    //}
}
//鼠标按键
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            //射击
#ifdef DEBUG_MODE
            std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
#endif
            break;
        }
    return;
}


// ---------------------------------
// 窗口相关函数
// ---------------------------------

// 改变窗口大小的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保窗口匹配的新窗口尺寸
    glViewport(0, 0, width, height);
}

// ---------------------------------
// 加载相关函数
// ---------------------------------

// 将六份纹理加载为一个cubemap纹理
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
