#include "rubblesim/Application.h"
// bring in C++ standrad library toosl for printing text
#include <chrono>
#include <exception>
#include <iostream>

namespace rubblesim {
// an unmaped namespace means the names in side arte onmly viidsiable inside
// this file
//
// That is useful for private local values
namespace {
constexpr int maxFrameCount = 5;

}
// defines the constructor declared in the header
Application::Application()
    // initilizes frameCount to zero before the constructor body runes
    // this initiliaztion style is called a memebr initializzer list
    // a constructor prepares an object when it is created
    : isRunning(true), frameCount(0), totalTimeSeconds(0.0),
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
  startup();
  // std::cout << "Rubblesim starting\n";

  while (isRunning) {
    tick();
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
  }

  shutdown();
  // std::cout << "RubbleSim shutting down\n";
  return 0;
}

void Application::startup() { std::cout << "RubbleSim startup\n"; }

void Application::tick() {
  const auto currentFrameTime = std::chrono::steady_clock::now();

  const std::chrono::duration<double> frameDelta =
      currentFrameTime - previousFrameTime;
  previousFrameTime = currentFrameTime;

  const double deltaTimeSeconds = frameDelta.count();

  update(deltaTimeSeconds);
  render(deltaTimeSeconds);
}

void Application::shutdown() { std::cout << "RubbleSim shutting down\n"; }

void Application::update(double deltaTimeSeconds) {
  totalTimeSeconds = totalTimeSeconds + deltaTimeSeconds;
  frameCount = frameCount + 1;

  if (frameCount >= maxFrameCount) {
    isRunning = false;
  }
}

void Application::render(double deltaTimeSeconds) {
  std::cout << "Frame " << frameCount << " dt" << deltaTimeSeconds << " total "
            << totalTimeSeconds << "seconds\n";
}
} // namespace rubblesim
