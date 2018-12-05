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
		float distance;
		float begin;
		

	public:
		static float UA; // unidade astronomica, para distancia do sol
		static float days; // days, para tempo de rotação
		static float years; // anos, para o periodo orbital

		/** Construtor de um Planeta
			* @param name - Nome do Planeta
			* @param scale - Usado para decidir a escala do planeta
			* @param orbit - Periodo Orbital
			* @param rotation - Periodo de rotação
			* @param distance - distância do Sol
			*/
		Planet(string name, float scale, float orbit, float rotation, float distance): Sun(name, scale){ 
			t_orbit = orbit;
			t_rotation = rotation;
			this->distance = distance;
			begin = glfwGetTime();
		}

		glm::mat4 render(float t){
			glm::mat4 matrix;
			float x;
			// interlo que faz uma volta em orbita
			if(years != 0)
				x = (t - begin)/(t_orbit * years);
			else
				x = 0;

			// Simula o movimento de translação do planeta ao redor do Sol
			matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
			matrix = glm::rotate(matrix, glm::radians(360.0f)* x, glm::vec3(0.0f, 1.0f, 0.0f));
			matrix = glm::translate(matrix, (distance * UA) * glm::vec3(1.0f, 0.0f, 0.0f));
			
			// retorna olhando pra frente
			matrix = glm::rotate(matrix, glm::radians(360.0f) * x, glm::vec3(0.0f, -1.0f, 0.0f));

			// interlo que faz uma volta de rotação
			if(days != 0.0)
				x = (t - begin)/(t_rotation * days);
			else
				x = 0;

			// faz o Scale correto do planeta
			matrix = glm::scale(matrix, (Scale * size) * glm::vec3(1.0f, 1.0f, 1.0f));

			// faz o movimento de rotação
			matrix = glm::rotate(matrix, glm::radians(360.0f) * x, glm::vec3(0.0f, 1.0f, 0.0f));

			// ajeita o angulo
			//matrix = glm::rotate(matrix, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			


			// retorna a matriz
			return matrix;
		}

		float getOrbit(){
			return t_orbit;
		}

		float getRotation(){
			return t_rotation;
		}
};

float Planet::UA = 1;
float Planet::days = 1;
float Planet::years = 0;


#endif