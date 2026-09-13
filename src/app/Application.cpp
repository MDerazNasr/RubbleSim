#include "rubblesim/Application.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

namespace {

constexpr const char* vertexShaderSource = R"(#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

void main() {
  gl_Position = vec4(position, 1.0);
  vertexColor = color;
}
)";

constexpr const char* fragmentShaderSource = R"(#version 330 core

in vec3 vertexColor;
out vec4 fragmentColor;

void main() {
  fragmentColor = vec4(vertexColor, 1.0);
}
)";

// Compiles one shader and returns zero when compilation fails.
unsigned int compileShader(unsigned int shaderType, const char* source) {
  const unsigned int shader = glCreateShader(shaderType);

  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int compilationSucceeded = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSucceeded);

  if (compilationSucceeded == GL_FALSE) {
    char errorMessage[1024];
    glGetShaderInfoLog(shader, sizeof(errorMessage), nullptr, errorMessage);

    std::cerr << "Shader compilation failed:\n" << errorMessage << "\n";

    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

} // namespace

namespace rubblesim {

Application::Application()
    : isRunning(true), window(nullptr), frameCount(0), totalTimeSeconds(0.0),
      previousFrameTime(std::chrono::steady_clock::now()), shaderProgram(0),
      vertexArray(0), vertexBuffer(0) {}

int Application::run() {
  if (!startup()) {
    return 1;
  }

  while (isRunning) {
    tick();
  }

  shutdown();
  return 0;
}

bool Application::startup() {
  if (glfwInit() == GLFW_FALSE) {
    std::cerr << "Failed to initialize GLFW\n";
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  window = glfwCreateWindow(1280, 720, "RubbleSim", nullptr, nullptr);

  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  const int gladVersion = gladLoadGL(glfwGetProcAddress);

  if (gladVersion == 0) {
    std::cerr << "Failed to load OpenGL\n";
    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
    return false;
  }

  if (!createTriangleResources()) {
    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
    return false;
  }

  std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(gladVersion) << "."
            << GLAD_VERSION_MINOR(gladVersion) << "\n";

  glfwSwapInterval(1);
  std::cout << "RubbleSim started\n";
  return true;
}

bool Application::createTriangleResources() {
  const unsigned int vertexShader =
      compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  const unsigned int fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  if (vertexShader == 0 || fragmentShader == 0) {
    if (vertexShader != 0) {
      glDeleteShader(vertexShader);
    }
    if (fragmentShader != 0) {
      glDeleteShader(fragmentShader);
    }
    return false;
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  int linkingSucceeded = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingSucceeded);

  if (linkingSucceeded == GL_FALSE) {
    char errorMessage[1024];
    glGetProgramInfoLog(shaderProgram, sizeof(errorMessage), nullptr,
                        errorMessage);

    std::cerr << "Shader linking failed:\n" << errorMessage << "\n";

    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
    return false;
  }

  // Each vertex stores three position values followed by three color values.
  const float vertices[] = {
      0.0F,  0.6F,  0.0F, 0.95F, 0.35F, 0.20F,
      -0.6F, -0.6F, 0.0F, 0.20F, 0.75F, 0.45F,
      0.6F,  -0.6F, 0.0F, 0.20F, 0.45F, 0.95F,
  };

  glGenVertexArrays(1, &vertexArray);
  glGenBuffers(1, &vertexBuffer);

  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return true;
}

void Application::destroyTriangleResources() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteVertexArrays(1, &vertexArray);
  glDeleteProgram(shaderProgram);

  vertexBuffer = 0;
  vertexArray = 0;
  shaderProgram = 0;
}

void Application::tick() {
  const auto currentFrameTime = std::chrono::steady_clock::now();
  const std::chrono::duration<double> frameDelta =
      currentFrameTime - previousFrameTime;

  previousFrameTime = currentFrameTime;
  const double deltaTimeSeconds = frameDelta.count();

  glfwPollEvents();

  if (glfwWindowShouldClose(window) == GLFW_TRUE) {
    isRunning = false;
  }

  update(deltaTimeSeconds);
  render();
}

void Application::shutdown() {
  destroyTriangleResources();

  glfwDestroyWindow(window);
  window = nullptr;
  glfwTerminate();

  std::cout << "RubbleSim shutting down\n";
}

void Application::update(double deltaTimeSeconds) {
  totalTimeSeconds = totalTimeSeconds + deltaTimeSeconds;
  frameCount = frameCount + 1;
}

void Application::render() {
  int framebufferWidth = 0;
  int framebufferHeight = 0;
  glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

  glViewport(0, 0, framebufferWidth, framebufferHeight);
  glClearColor(0.08F, 0.10F, 0.12F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);
  glBindVertexArray(vertexArray);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

  glfwSwapBuffers(window);
}

} // namespace rubblesim
