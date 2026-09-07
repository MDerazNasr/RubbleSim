// tellt he compiler to include this file only once in each source file that
// uses it
#pragma once

// cpp time tools
#include <chrono>

// c reates a named area for our code. this avoids name comflicys with outher
// libraries
// struct declares a structured type
// GLFWwindow is the type GLFW uses to represent a window
//
// Thwe delcration tells the compiler that tgihs tpe exists without exposing the
// entire GLFW header inside your header
// * makes window a pointer. A pointer stores the memory address of another
// object
struct GLFWwindow;

namespace rubblesim {
class Application {
public:
  Application();
  int run();

private:
  // void = does not return value
  //
  // void means teh function returns no value
  // startup is the function name
  // () means the function recieve no arguments
  // These are declarations. They tell the compiler that these member functions
  // exist. Their actual code will be written in the source file
  bool startup();
  void tick();
  void shutdown();

  void update(double deltaTimeSeconds);
  void render();

  // stores whether the app should keep looping
  bool isRunning;
  GLFWwindow *window;
  int frameCount;
  double totalTimeSeconds;

  // steady_clock is a clock from the cpp library
  // time_point stores one moment in time
  // previousFrameTime stores when the prev grame started
  std::chrono::steady_clock::time_point previousFrameTime;
};

} // namespace rubblesim
