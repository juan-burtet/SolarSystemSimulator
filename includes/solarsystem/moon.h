#ifndef MOON_H
#define MOON_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include "planet.h"

#include <iostream>
#include <string>


using namespace std;

class Moon: public Planet {
	protected:
		Planet *Origin;

	public:
		Moon(string name, glm::vec3 position, glm::vec3 scale, float orbit, float rotation, Planet *origin): Planet(name, position, scale, orbit, rotation){ 
			Origin = origin;
		}

		glm::vec3 getOrigin(){
			return Origin.getPosition();
		}
};

#endif