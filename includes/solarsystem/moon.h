#ifndef MOON_H
#define MOON_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sun.h"
#include "planet.h"

#include <iostream>
#include <string>

using namespace std;

class Moon: public Planet {
	protected:
		Planet *Origin;

	public:
		Moon(string name, float scale, float orbit, float rotation, float distance, Planet *origin): Planet(name, scale, orbit, rotation, distance){ 
			Origin = origin;
		}

		glm::mat4 render(float t){
			glm::mat4 matrix;
			float x;

			// Calculo pro tempo de orbita
			if(years != 0)
				x = (t - begin)/(t_orbit * years);
			else
				x = 0;

			// se movimenta até a origem
			matrix = glm::translate(matrix, getOrigin());
			// rotaciona 
			matrix = glm::rotate(matrix, glm::radians(360.0f)* x, glm::vec3(0.0f, 1.0f, 0.0f));
			// se distancia do planeta
			matrix = glm::translate(matrix, (distance * UA) * glm::vec3(1.0f, 0.0f, 0.0f));
			// retorna olhando pra frente
			matrix = glm::rotate(matrix, glm::radians(360.0f) * x, glm::vec3(0.0f, -1.0f, 0.0f));

			// Calculo pro tempo de rotação
			if(days != 0.0)
				x = (t - begin)/(t_rotation * days);
			else
				x = 0;

			// faz o movimento de rotação
			matrix = glm::rotate(matrix, glm::radians(360.0f) * x, glm::vec3(0.0f, 1.0f, 0.0f));
			// faz o Scale correto da lua
			matrix = glm::scale(matrix, (Scale * size) * glm::vec3(1.0f, 1.0f, 1.0f));
			// retorna a matriz
			return matrix;
		}

		glm::vec3 getOrigin(){
			return Origin->getPosition();
		}
};

#endif