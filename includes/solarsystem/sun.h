#ifndef SUN_H
#define SUN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <./learnopengl/filesystem.h>
#include <./learnopengl/shader_m.h>
#include <./learnopengl/camera.h>
#include <./learnopengl/model.h>

#include <iostream>
#include <string>


using namespace std;

class Sun {
	protected:
		string Name; 
		//Model *Model;
		glm::vec3 Position;
		glm::vec3 Scale;
      
   public:
      Sun(string name, glm::vec3 position, glm::vec3 scale){
         Name = name;
         //Model = model;
         Position = position;
         Scale = scale;
      }//Sun

      string getName(){
      	return Name;
      }

      glm::vec3 getPosition(){
      	return Position;
      }

      glm::vec3 getScale(){
      	return Scale;
      }
};

#endif