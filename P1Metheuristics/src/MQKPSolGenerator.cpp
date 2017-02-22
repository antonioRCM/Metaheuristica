/*
 * MQKPSolGenerator.cpp
 *
 * Fichero que define los métodos de la clase MQKPInstance. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPSolGenerator.h"
#include "MQKPInstance.h"
#include "MQKPSolution.h"

void MQKPSolGenerator::genRandomSol(MQKPInstance &instance, MQKPSolution &solution){

	int numObjs = instance.getNumObjs();
	int numKnapsacks = instance.getNumKnapsacks();

	for (int i = 0; i < numObjs; i++){
		// Se generan identificadores de mochila entre 0 y el número de mochilas incluido
		// El valor 0 se guarda para elementos no guardados en una mochila

		int randomKnapsack = rand() % (numKnapsacks + 1);  //TODO Cambiar para seleccionar una de las mochilas disponibles
		solution.putObjectIn(i, randomKnapsack);
	}
}

