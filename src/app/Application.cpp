#include "rubblesim/Application.h"
// bring in C++ standrad library toosl for printing text
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>

#include "rubblesim/sim/Transform.h"

namespace rubblesim {
// an unmaped namespace means the names in side arte onmly viidsiable inside
// this file
//
// That is useful for private local values
namespace {}
// defines the constructor declared in the header
Application::Application()
    // initilizes frameCount to zero before the constructor body runes
    // this initiliaztion style is called a memebr initializzer list
    // a constructor prepares an object when it is created
    : isRunning(true), window(nullptr), frameCount(0), totalTimeSeconds(0.0),
      previousFrameTime(std::chrono::steady_clock::now()) {}

/*
 * int Application::run() defines the run function.
 * std::cout prints text to the terminal.
 * std is the namespace for the C++ standard library.
 * << sends data into the output stream.
 * \n means newline.
 * while repeats code while a condition is true.
 * frameCount < 5 is the condition.
 * frameCount = frameCount + 1 increases the frame count by one.
 * return 0 means the program finished successfully.
 */
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
// const means the varibale cannot be change after being created
// auto means cpp fivures out the type automatically
// now() asks the clock for the current time
// const auto currentFrameTime = std::chrono::steady_clock::now();
// duration<double> stores an amount of time
// const std::chrono::duration<double> frameDelta =
// currentFrameTime - previousFrameTime;

// here it stores the time between this frame and the previous frame
// previousFrameTime = currentFrameTime;
// count
//.count() turns the tiem into a number
// const double deltaTimeSeconds = frameDelta.count();
// update(deltaTimeSeconds);
// render(deltaTimeSeconds);
bool Application::startup() {
  // glfwinit prepares GLFW
  if (glfwInit() == GLFW_FALSE) {
    // cerr is for printing error messages
    std::cerr << "Failed to initialize GLFW\n";
    return false;
  }
  // std::cout << "RubbleSim startup\n";
  // configures the winow before creating it
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  // recieves the width, height, title, monitor, and shared context
  // the two nullptr arguments create a normal window without sharing another
  // OpenGL context
  window = glfwCreateWindow(1280, 720, "RubbleSim", nullptr, nullptr);
  if (window == nullptr) {
    // the two nullptr arguments create a normal window without sharing another
    // OpenGL context
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  // gladLoadGL loads OpenGL function addresses
  // glfwGetProcAdress asks GLFW to find each fucntion in the graphiss driver
  // gladVersion stores the loaded OpenGL version
  const int gladVersion = gladLoadGL(glfwGetProcAddress);

  if (gladVersion == 0) {
    std::cerr << "Failed to load OpenGL\n";

    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();

    return false;
  }

  // creates a value using the defaults from the struct
  // const prevents this temp val from changing
  // .position accesses to the position member
  // .x, .y, .z access the 3 componehts of the vector
  const Transform arenaTransform{};

  std::cout << "Arena origin: " << arenaTransform.position.x << ", "
            << arenaTransform.position.y << ", " << arenaTransform.position.z
            << "\n";

  //...MAJOR extracts the major version
  //...MINOR extracts the minor version
  std::cout << "Loaded OpenGL" << GLAD_VERSION_MAJOR(gladVersion) << "."
            << GLAD_VERSION_MINOR(gladVersion) << "\n";
  glfwSwapInterval(1);

  std::cout << "RubbleSim started\n";
  return true;
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

  // & obtains the address of a variable so GLFW can write a value into it
  // glViewport tells OpenGL which pixel area it may draw into
  // glClearColor selects the background color using red, green, blue, and alpha
  // values between 0 and 1 F amrks each number as a float glClear fills: the
  // selected buffer GL_COLOR_BUFFER_BIT identifies the color buffer
  glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

  glViewport(0, 0, framebufferWidth, framebufferHeight);
  glClearColor(0.08F, 0.10F, 0.12F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(window);
}
} // namespace rubblesim
