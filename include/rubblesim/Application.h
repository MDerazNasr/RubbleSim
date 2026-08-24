// tellt he compiler to include this file only once in each source file that
// uses it
#pragma once

// c reates a named area for our code. this avoids name comflicys with outher
// libraries
namespace rubblesim {
class Application {
public:
  Application();
  int run();

private:
  int frameCount;
};

} // namespace rubblesim
