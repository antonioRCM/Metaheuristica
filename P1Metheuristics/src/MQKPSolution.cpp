/*
 * MQKPSolution.cpp
 *
 * Fichero que define los métodos de la clase MQKPSolution. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPSolution.h"
#include "MQKPInstance.h"

MQKPSolution::MQKPSolution(MQKPInstance &instance) {
	//TODO !!!HECHO!!! completar inicializando las variables miembro. Inicialmente, todos los objetos estarán fuera de las mochilas ( = 0)

	for(int i=0;i<=instance.getNumObjs();i++){
			_sol.push_back(0);
		}
		_numObjs = instance.getNumObjs();
		_fitness = 0.0;
}

MQKPSolution::~MQKPSolution() {
	//TODO !!!HECHO!!! completar
	_sol.erase(_sol.begin(),_sol.end());
}

void MQKPSolution::putObjectIn(int object, int knapsack){
	//TODO !!!HECHO!!! completar la función
		_sol[object]=knapsack;

}

int MQKPSolution::whereIsObject(int object){
	//TODO !!!HECHO!!! completar la función
		return _sol[object];
}

