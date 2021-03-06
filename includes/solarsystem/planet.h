#ifndef PLANET_H
#define PLANET_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		glm::vec3 position;
		unsigned int qtMoons;
		

	public:
		static float UA; // unidade astronomica, para distancia do sol
		static float days; // days, para tempo de rotação
		static float years; // anos, para o periodo orbital
		static bool plane; // Terra Plana
		static int worldSpeed; // Velocidade do mundo
		static bool pause; // O mundo está pausado
		static float time; // tempo para quando estiver pausado

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
			position = glm::vec3(0.0f, 0.0f, 0.0f);
			qtMoons = 0;
		}

		glm::mat4 render(){
			glm::mat4 matrix;
			float x;
			
			float t = time;

			// Calculo pro tempo de orbita
			if(years* t_orbit != 0)
				x = (t - begin)/(t_orbit * years);
			else
				x = 0;

			// Se movimenta pra posição do Sol
			matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
			// rotaciona
			matrix = glm::rotate(matrix, glm::radians(360.0f)* x, glm::vec3(0.0f, 1.0f, 0.0f));
			// se distancia do sol
			matrix = glm::translate(matrix, (distance * UA) * glm::vec3(1.0f, 0.0f, 0.0f));
			// retorna olhando pra frente
			matrix = glm::rotate(matrix, glm::radians(360.0f) * x, glm::vec3(0.0f, -1.0f, 0.0f));

			// Calculo pro tempo de rotação
			if(days * t_rotation != 0.0)
				x = (t - begin)/(t_rotation * days);
			else
				x = 0;

			// faz o movimento de rotação
			matrix = glm::rotate(matrix, glm::radians(360.0f) * x, glm::vec3(0.0f, 1.0f, 0.0f));
			// faz o Scale correto do planeta
			matrix = glm::scale(matrix, (Scale * size) * glm::vec3(1.0f, 1.0f, 1.0f));
		
			// atualiza posição do planeta (Para lua usar)
			position.x = matrix[3][0];
			position.y = matrix[3][1];
			position.z = matrix[3][2]; 

			if(plane && (Name == "Earth"))
				matrix = glm::scale(matrix, glm::vec3(1.0f, 0.1f, 1.0f));

			// retorna a matriz
			return matrix;
		}

		float getScale(){
			return Scale;
		}

		glm::vec3 getPosition(){
			return position;
		}

		void setMoons(unsigned int x){
			qtMoons = x;
		}

		unsigned int getMoons(){
			return qtMoons;
		}
};

float Planet::UA    	 = 1;
float Planet::days  	 = 5;
float Planet::years 	 = 10;
bool  Planet::plane 	 = false;
int   Planet::worldSpeed = 5;
bool  Planet::pause 	 = false;
float Planet::time 		 = glfwGetTime();

#endif