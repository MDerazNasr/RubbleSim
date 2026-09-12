#pragma once
#include <glm/vec3.hpp>

namespace rubblesim {

// struct creates a type that groups related values
// struct memebrs are public unless specified
struct Transform {
  // glm::vec3 stores 3 flating point numbers
  // They normally represet the X,Y,Z components of a value
  // position describes where an object is located
  // rotationRadians describes rotation around the X,Y,Z axes
  // radians are the angle unit used by GLM
  // 1 complete rotation is approx 6.28318 rad
  // scale describes the size multiplier along each of the acis
  // values inside the struct are default memeber initzialiesr, every nrwe
  // transform geinfs with these values unless diff ones are given
  glm::vec3 position{0.0F, 0.0F, 0.0F};
  glm::vec3 rotationRadians{0.0F, 0.0F, 0.0F};
  glm::vec3 scale{1.0F, 1.0F, 1.0F};
};

} // namespace rubblesim
