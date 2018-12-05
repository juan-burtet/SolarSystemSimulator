#ifndef SUN_H
#define SUN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include <string>


using namespace std;

class Sun {
	protected:
		string Name; 
		float Scale;

      
  public:
  	static float size; // size, para a escala do planeta

    Sun(string name, float scale){
      Name = name;
      Scale = scale;
    }//Sun

    glm::mat4 render(){
     	glm::mat4 matrix;
     	matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
			matrix = glm::rotate(matrix, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
     	matrix = glm::scale(matrix, (Scale * size) * glm::vec3(1.0f, 1.0f, 1.0f));
     	return matrix;
    }

    string getName(){
     	return Name;
    }

    float getScale(){
     	return Scale;
    }
};

float Sun::size = 0.000001;

#endif