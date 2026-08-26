#include "rubblesim/Application.h"
// bring in C++ standrad library toosl for printing text
#include <chrono>
#include <iostream>

namespace rubblesim {
// defines the constructor declared in the header
Application::Application()
    // initilizes frameCount to zero before the constructor body runes
    // this initiliaztion style is called a memebr initializzer list
    // a constructor prepares an object when it is created
    : isRunning(true), frameCount(0),
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
  std::cout << "Rubblesim starting\n";

  while (isRunning) {
    // const means the varibale cannot be change after being created
    // auto means cpp fivures out the type automatically
    // now() asks the clock for the current time
    const auto currentFrameTime = std::chrono::steady_clock::now();
    // duration<double> stores an amount of time
    const std::chrono::duration<double> frameDelta =
        currentFrameTime - previousFrameTime;

    // here it stores the time between this frame and the previous frame
    previousFrameTime = currentFrameTime;
    // count
    //.count() turns the tiem into a number
    const double deltaTimeSeconds = frameDelta.count();
    update(deltaTimeSeconds);
    render(deltaTimeSeconds);
  }

  std::cout << "RubbleSim shutting down\n";
  return 0;
}

void Application::update(double deltaTimeSeconds) {
  frameCount = frameCount + 1;

  if (frameCount >= 5) {
    isRunning = false;
  }
}

void Application::render(double deltaTimeSeconds) {
  std::cout << "Frame " << frameCount << " dt" << deltaTimeSeconds
            << "seconds\n";
}
} // namespace rubblesim
