/*
 * MQKPIteratedGreedy.cpp
 *
 * Fichero que define las funciones de la clase MQKPIteratedGreedy. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include <MQKPIteratedGreedy.h>
#include <MQKPObjectAssignmentOperation.h>
#include <MQKPSolution.h>
#include <iostream>

using namespace std;

void MQKPIteratedGreedy::chooseOperation(
		MQKPObjectAssignmentOperation& operation) {

	int bestObj = 0;
	int bestKnapsack = 0;
	double bestDensity = 0;
	double bestDeltaFitness = 0;
	bool initialisedBestDensity = false;
	unsigned numObjs = _instance->getNumObjs();
	unsigned numKnapsacks = _instance->getNumKnapsacks();

	/**
	 * HECHO
	 * Recorrer todos los objetos que no están en ninguna mochila
	 *   Recorrer todas las mochilas
	 *     Calcular el delta del fitness de asignar dicho objeto a dicha mochila y la densidad
	 *     Almacenar dicha asignación como la mejor en caso de que sea la de mayor densidad
	 */

	for (unsigned i = 0; i < numObjs; i++) {

		int indexObj = i;

		// Este método devuelve cero si el elemento con índice indexObj no está en ninguna mochila
		if (_sol->whereIsObject(indexObj) == 0) { //HECHO comprobar que no está en ninguna mochila

			// Los índices de mochila van desde 1 hasta el número de mochilas
			for (unsigned j = 1; j <= numKnapsacks; j++) { //HECHO para todas las mochilas disponibles (saltarse la 0)

				//HECHO Calcular delta fitness, densidad como deltaFitness dividido por el peso, y actualizar la mejor opción
				int indexKnapsack = j;

				double deltaFitness = MQKPEvaluator::computeDeltaFitness(*_instance, *_sol, indexObj, indexKnapsack);
				double density = deltaFitness/(_instance->getWeight(indexObj));

				if (density > bestDensity || initialisedBestDensity == false) {
					initialisedBestDensity = true;
					bestDensity = density;
					bestObj = indexObj;
					bestKnapsack = indexKnapsack;
					bestDeltaFitness = deltaFitness;
				}
			}
		}
	}

	operation.setValues(bestObj, bestKnapsack, bestDeltaFitness);
}

void MQKPIteratedGreedy::rebuild() {

	/** Seleccionar la primera operación */
	MQKPObjectAssignmentOperation operation;
	chooseOperation(operation);

	/** HECHO
	 * Mientras la operación tenga un incremento de fitness positivo, operation.getDeltaFitness(),
	 *  1. aplicar la operación en _sol
	 *  2. Almacenar el fitness de la solución en _results (para las gráficas)
	 *  3. seleccionar una nueva operación
	 */
	while (operation.getDeltaFitness() > 0) {
		operation.apply(*_sol);
		_results.push_back(MQKPEvaluator::computeFitness(*_instance, *_sol));
		chooseOperation(operation);
	}
}

void MQKPIteratedGreedy::destroy() {

	/**
	 * HECHO
	 * Recorrer los objetos y sacarlos de su mochila con probabilidad _alpha
	 */

	unsigned numObjs = _instance->getNumObjs();

	for (unsigned i = 0; i < numObjs; i++){

		double randSample = ((double)(rand())) / RAND_MAX;

		// Cambia de mochila con probabilidad _alpha
		if (randSample < _alpha){
			_sol->putObjectIn(i, 0);
		}
	}

	double fitness = MQKPEvaluator::computeFitness(*_instance, *_sol);
	_sol->setFitness(fitness);
	_results.push_back(_sol->getFitness());
}

void MQKPIteratedGreedy::initialise(double alpha, MQKPInstance& instance) {
	_sol = new MQKPSolution(instance);
	_bestSolution = new MQKPSolution(instance);
	_bestSolution->copy(*_sol);
	_instance = &instance;
	_alpha = alpha;
}

void MQKPIteratedGreedy::run(MQKPStopCondition& stopCondition) {

	if (_sol == NULL) {
		cerr << "IG was not initialised" << endl;
		exit(-1);
	}

	/** Crear la primera solución */
	rebuild();

	if (MQKPEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0)
		_bestSolution->copy(*_sol);

	/**
	 * HECHO
	 * Mientras no se alcance la condición de parada
	 *  1. Destruir parcialmente la solución
	 *  2. Reconstruir la solución
	 *  3. Almacenar el nuevo fitness en _results para las gráficas
	 *  4. Actualizar la mejor solución y volver a ella si la nueva es peor
	 */

	while (stopCondition.reached() == false) {
		destroy();
		rebuild();
		_results.push_back(_sol->getFitness());

		if (MQKPEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0)
			_bestSolution->copy(*_sol); // Actualiza la mejor solución
		else
			_sol->copy(*_bestSolution); // Restaura la solución anterior

		stopCondition.notifyIteration();
	}
}


