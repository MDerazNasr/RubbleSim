#include "rubblesim/Application.h"
// bring in C++ standrad library toosl for printing text
#include <iostream>

namespace rubblesim {
// defines the constructor declared in the header
Application::Application()
    // initilizes frameCount to zero before the constructor body runes
    // this initiliaztion style is called a memebr initializzer list
    // a constructor prepares an object when it is created
    : isRunning(true), frameCount(0) {}

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
    update();
    render();
  }

  std::cout << "RubbleSim shutting down\n";
  return 0;
}

void Application::update() {
  frameCount = frameCount + 1;

  if (frameCount >= 5) {
    isRunning = false;
  }
}

void Application::render() { std::cout << "Frame " << frameCount << "\n"; }
} // namespace rubblesim
