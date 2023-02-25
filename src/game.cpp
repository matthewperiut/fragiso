#include "game.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

extern Game game;

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    game.width = width;
    game.height = height;

    float aspect_ratio = float(width)/float(height);
    float scale = 1;

    if (game.maintain_width)
    {
        scale = width / game.current_width;
        game.current_height = height/scale;

        game.width = scale * game.current_width;
        game.height = scale * game.current_height;
    }
    else
    {

    }

    game.regenFBO();
}

void Game::init() {
    // Initialize GLFW and create a window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "Isometric", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // Initialize GLEW
    glewExperimental = true;
    glewInit();

    // Get textures
    int number;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &number);
    std::cout << "Number of Texture Image Units " << number << std::endl;

    int size;
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &size);
    std::cout << "3D Texture Size " << size << "x" << size << std::endl;

    int max_layers;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
    std::cout << "Max Array Texture Layers " << max_layers << std::endl;

    int max_samplers;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_samplers);
    std::cout << "Max Samplers " << max_samplers << std::endl;

    // Set the clear color to fill the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Create the vertex buffer object and vertex array object for the rectangle
    GLfloat vertices[] = {
            1.0f,  1.0f, 0.0f,  // top right
            1.0f, -1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,  // bottom left
            -1.0f,  1.0f, 0.0f   // top left
    };
    GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3,   // First Triangle
            1, 2, 3    // Second Triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    // Read and build the shader pixel_program
    readShader(pixel_program, "shaders/vertex.vert", "shaders/fragment.frag");
    //readShader(pixel_program, "shaders/blue.vert", "shaders/blue.frag");
    GLint isLinked = 0;
    glGetProgramiv(pixel_program, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(pixel_program, 512, nullptr, infoLog);
        std::cout << "Linking Error: " << infoLog << std::endl;
        validateProgram(pixel_program);
    }

    // Unbind the VAO
    // glBindVertexArray(0);



    sendVoxelShapeToFragmentShader(shape);

    //Pixel pixel = shape.getPixel(2,2,2);
    //std::cout << "Pixel " << int(pixel.r) << " " << int(pixel.g) << " " << int(pixel.b) << std::endl;

    createFBO();

    std::cout << pixel_program << std::endl;

    if(glewIsSupported("GL_ARB_debug_output"))
    {
        glDebugMessageCallback(openglCallbackFunction, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
    }

    glfwSetWindowSizeCallback(window, windowSizeCallback);
}

void Game::regenFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

    // Delete previous texture
    glDeleteTextures(1, &fbo_texture);

    // Generate texture handle
    glGenTextures(1, &fbo_texture);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, fbo_texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Allocate texture memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, current_width, current_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Attach texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Failed to resize FBO" << std::endl;
    }
}

void Game::createFBO() {
    // Generate FBO handle
    glGenFramebuffers(1, &fbo_handle);

    // Bind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

    // Generate texture handle
    glGenTextures(1, &fbo_texture);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, fbo_texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Allocate texture memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, current_width, current_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Attach texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);

    // Check if FBO is complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        // Handle FBO creation error
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::loop() {
    bool toggle = false;
    bool pressed1 = false;
    bool pressed2 = false;
    bool pressed3 = false;
    bool go = false;
    bool reverse = false;
    int num = 0;
    int ct = 0;

    auto start_time = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {

        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            shape_name = "vox/monu1.vox";
        }
        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            shape_name = "vox/teapot.vox";
        }
        if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            shape_name = "vox/sphere.vox";
        }
        if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            shape_name = "vox/test.vox";
        }

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !pressed1)
        {
            pressed1 = true;
            shape = convertMagicaVoxelToVoxelShape(shape_name.c_str());
            sendVoxelShapeToFragmentShader(shape);

        }
        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
        {
            pressed1 = false;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.y -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.y += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.x += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.x -= 1;
        }
        sendCamera();
        render();

        // going to be useful
        // glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, shape.xSize, shape.ySize, shape.zSize, GL_RGBA, GL_UNSIGNED_BYTE, shape.data);




        glfwSwapBuffers(window);
        glfwPollEvents();
        //fps();
    }
}

void Game::fps()
{
    static std::vector<double> frame_times;
    static auto start = std::chrono::high_resolution_clock::now();


    auto end = std::chrono::high_resolution_clock::now();
    double elapsed_time = std::chrono::duration<double>(end - start).count();
    frame_times.push_back(elapsed_time);

    if (frame_times.size() >= 100) {
        double sum = 0;
        for (auto t : frame_times) sum += t;
        double average_time = sum / frame_times.size();
        double std_dev = 0;
        for (auto t : frame_times) std_dev += (t - average_time) * (t - average_time);
        std_dev = sqrt(std_dev / frame_times.size());

        double fps = 1 / average_time;
        std::cout << "FPS: " << fps << " +/- " << std_dev * fps << std::endl;
        frame_times.clear();
    }

    start = end;
}

void Game::render() {
    // Apply the shader pixel_program and render a rectangle
    glUseProgram(pixel_program);

    glBindVertexArray(VAO);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scale*fbo_handle);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if (scale)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_handle);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, current_width, current_height,
                          0, 0, width, height,
                          GL_COLOR_BUFFER_BIT,
                          GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }

    glBindVertexArray(0);
}

void Game::readShader(GLuint& program, const char* vertexPath, const char* fragmentPath) {
    // Create the shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertexPath, std::ios::in);
    if (vertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(vertexShaderStream, Line))
            vertexShaderCode += "\n" + Line;
        vertexShaderStream.close();
    } else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexPath);
        getchar();
        exit(-1);
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragmentPath, std::ios::in);
    if (fragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(fragmentShaderStream, Line))
            fragmentShaderCode += "\n" + Line;
        fragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertexPath);
    char const *VertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);

    // Check Vertex Shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(vertexShader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragmentPath);
    char const *FragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);

    // Check Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1
        );
        glGetShaderInfoLog(fragmentShader, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    // Link the program
    printf("Linking program\n");
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check the program
    glGetProgramiv(program, GL_LINK_STATUS, &Result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void sendUniform3iSafely(GLuint program, std::string name, int x, int y, int z)
{
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if(location == -1)
        std::cout << "Uniform " + name + " not found or not active" << std::endl;
    else
    {
        glUniform3f(location, x, y, z);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "(" + name + ") Error: " << error << std::endl;
        }
    }
}

void Game::sendCamera()
{
    glUseProgram(pixel_program);
    GLuint location = glGetUniformLocation(pixel_program, "cameraPosition");

    if(location == -1)
        std::cout << "cameraPosition not found in shader" << std::endl;
    else
    {
        glUniform2f(location, camera.x, camera.y);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "(cameraPosition) Error: " << error << std::endl;
        }
    }

}

void Game::sendVoxelShapeToFragmentShader(VoxelShape& voxelShape) const {
    sendUniform3iSafely(pixel_program, "voxelShapeSize", voxelShape.xSize, voxelShape.ySize, voxelShape.zSize);

    // Create the 3D texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, shape.xSize, shape.ySize, shape.zSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, shape.data);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    GLuint textureUnit = 0;
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_3D, textureID);

    // Bind the texture to a uniform sampler in the fragment shader
    GLint voxelShapeUniform = glGetUniformLocation(pixel_program, "voxelShape");
    glUniform1i(voxelShapeUniform, textureUnit);
}

void Game::validateProgram(GLuint programToValidate)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Validate the program
    glValidateProgram(programToValidate);

    // Check the program
    glGetProgramiv(programToValidate, GL_VALIDATE_STATUS, &Result);
    glGetProgramiv(programToValidate, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(programToValidate, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
}