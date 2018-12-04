#ifndef PLANET_H
#define PLANET_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include "sun.h"

#include <iostream>
#include <string>


using namespace std;

class Planet: public Sun {
	protected:
		float t_orbit;
		float t_rotation;

	public:
		Planet(string name, glm::vec3 position, glm::vec3 scale, float orbit, float rotation): Sun(name, position, scale){ 
			t_orbit = orbit;
			t_rotation = rotation;
		}

		float getOrbit(){
			return t_orbit;
		}

		float getRotation(){
			return t_rotation;
		}
};

#endif