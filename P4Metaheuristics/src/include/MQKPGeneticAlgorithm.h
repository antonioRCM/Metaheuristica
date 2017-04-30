/*
 * MQKPGeneticAlgorithm.h
 *
 * Fichero que define la clase MQKPGeneticAlgorithm. Forma parte del cÃ³digo esqueleto para el problema de las mÃºltiples mochilas cuadrÃ¡ticas, ofrecido para las prÃ¡cticas de la asignatura MetaheurÃ­sticas del Grado de IngenierÃ­a InformÃ¡tica de la Universidad de CÃ³rdoba
 *
 * @author Carlos GarcÃ­a cgarcia@uco.es
 */

#ifndef INCLUDE_MQKPGENETICALGORITHM_H_
#define INCLUDE_MQKPGENETICALGORITHM_H_

#include <MQKPMetaheuristic.h>
#include <MQKPSolution.h>
#include <SelectionOperator.h>
#include <MQKPCrossoverOperator.h>
#include <MQKPMutationOperator.h>
#include <TournamentSelector.h>
#include <vector>
#include <iostream>

using namespace std;

/**
 * Clase que implementa un Algoritmo GenÃ©tico Generacional con Elitismo para el MQKP
 */
class MQKPGeneticAlgorithm: public MQKPMetaheuristic {
protected:
	/**
	 * Variables miembro de la clase:
	 *  _popSize TamaÃ±o de la poblaciÃ³n
	 *  _population Conjunto de individuos en la poblaciÃ³n
	 *  _selector Operador de selecciÃ³n
	 *  _crossoverOp Operador de cruce
	 *  _mutOp Operador de mutaciÃ³n
	 *  _instancia Instancia del problema abordada
	 */
	unsigned _popSize;
	vector<Solution*> _population;
	SelectionOperator *_selector = NULL;
	MQKPCrossoverOperator *_crossoverOp = NULL;
	MQKPMutationOperator *_mutOp = NULL;
	MQKPInstance *_instance = NULL;

	/**
	 * vectores donde se almacenan los resultados
	 *  _results valores fitness de las soluciones generadas
	 *  _popMeanResults Media de los valores fitness presentes en la poblaciÃ³n
	 *  _offMeanResults Media de los valores fitness de las nuevas soluciones generadas
	 *  _bestPerIterations Mejor valor en cada iteraciÃ³n
	 */
	vector<double> _results;
	vector<double> _popMeanResults;
	vector<double> _offMeanResults;
	vector<double> _bestPerIterations;

	/**
	 * FunciÃ³n que busca el Ã­ndice de la mejor soluciÃ³n en un vector
	 * @param[in] set Vector de soluciones
	 * @return Ã­ndice de la mejor soluciÃ³n
	 */
	unsigned indexBest(vector<Solution*> &set) {

		//HECHO buscar el Ã­ndice de la mejor soluciÃ³n en set

		// Inicialmente consideramos al primero el mejor elemento
		unsigned indexBest = 0;

		// Buscamos si hay algún elemento mejor que el primero
		for (size_t i = 1; i < set.size(); i++) {
			if (MQKPEvaluator::compare(set[i]->getFitness(), set[indexBest]->getFitness()) > 0) {
				indexBest = i;
			}
		}

		return indexBest;
	}

	/**
	 * FunciÃ³n que busca el Ã­ndice de la peor soluciÃ³n en un vector
	 * @param[in] set Vector de soluciones
	 * @return Ã­ndice de la peor soluciÃ³n
	 */
	unsigned indexWorst(vector<Solution*> &set) {

		//HECHO buscar el índice de la peor solución en set

		// Consideramos al primero como la peor solución
		unsigned indexWorst = 0;

		// Buscamos si existe alguna solución peor que el primero
		for (size_t i = 1; i < set.size(); i++) {
			if (MQKPEvaluator::compare(set[i]->getFitness(), set[indexWorst]->getFitness()) < 0) {
				indexWorst = i;
			}
		}

		return indexWorst;
	}

	/**
	 * FunciÃ³n que actualiza la nueva poblaciÃ³n, dado el conjunto de descendientes generado
	 * @param[in] offspring Vector de soluciones descendientes generadas
	 */
	void selectNewPopulation(vector<Solution*> &offspring) {

		/**
		 * HECHO
		 * La nueva poblaciÃ³n serÃ¡ la de descendientes, pero
		 * en caso de que la poblaciÃ³n actual tenga una mejor soluciÃ³n que la mejor en offspring,
		 * la mejor de la poblaciÃ³n actual reemplazarÃ¡ a la peor de offspring.
		 *
		 * 1. Encontrar el Ã­ndice de la mejor soluciÃ³n en _population
		 * 2. Encontrar el Ã­ndice de la mejor soluciÃ³n en offspring
		 * 3. Si la mejor soluciÃ³n de _population es mejor que la mejor soluciÃ³n de offspring
		 *   a. Encontrar el Ã­ndice de la peor soluciÃ³n en offspring
		 *   b. Hacer que dicha soluciÃ³n copie a la mejor de _population
		 * 4. Eliminar los individuos de la poblaciÃ³n actual (liberando memoria)
		 * 5. Almacenar los individuos de offspring en la poblaciÃ³n actual
		 */
		unsigned int indexBestPop = indexBest(_population);
		unsigned int indexBestOff = indexBest(offspring);

		// Comprobamos si el mejor actual, es mejor que el mejor de offspring
		if (MQKPEvaluator::compare(_population[indexBestPop]->getFitness(), offspring[indexBestOff]->getFitness()) > 0) {

			// Sustituimos el peor de offspring por el mejor actual
			unsigned int indexWorstOff = indexWorst(offspring);
			offspring[indexWorstOff]->copy(*_population[indexBestPop]);
		}

		//Eliminar los individuos de la poblaciÃ³n actual
		for (unsigned i = 0; i < _popSize; i++) {
			delete (_population.back());
			_population.pop_back();
		}

		//Copiar los hijos en la poblaciÃ³n actual
		unsigned offSize = (unsigned) offspring.size();

		for (unsigned i = 0; i < offSize; i++) {
			_population.push_back(offspring.back());
			offspring.pop_back();
		}
	}

	/**
	 * FunciÃ³n que evalÃºa las soluciones de un vector
	 * @param[in,out] set Conjunto de soluciones a evaluar. Una vez evaluados, les asigna el fitness
	 */
	void evaluate(vector<Solution*> &set) {

		for (Solution *sol : set) {
			MQKPSolution *s = (MQKPSolution*) sol;

			/**
			 * HECHO
			 * Se ha aÃ±adido una funcionalidad en Solution para detectar si su fitness ya estaba calculado,
			 * Útil para cuando el descendiente es copia del padre. Por tanto, sólo se evaluarán las soluciones
			 * que no tentan un fitness vÃ¡lido
			 */
			if (!(s->hasValidFitness())) {

				//Evaluar
				double fitness = MQKPEvaluator::computeFitness(*_instance, *s);
				_results.push_back(fitness);
				s->setFitness(fitness);

				//Actualizar la mejor soluciÃ³n
				if (MQKPEvaluator::compare(fitness, _bestSolution->getFitness()) > 0){
					_bestSolution->copy(*s);
				}
			}
		}
	}

	/**
	 * FunciÃ³n que inicializa la poblaciÃ³n del genÃ©tico
	 * @param[in] popSize TamaÃ±o de la poblaciÃ³n
	 */
	void initPopulation(unsigned popSize) {

		if (_instance == NULL) {
			cerr	<< "The evolutionary algorithm has not been initialised. At least, its _instance is NULL"
					<< endl;
			exit(1);
		}

		/**
		 * HECHO
		 * 1. Generar soluciones aleatorias
		 * 2. Evaluarlas
		 * 3. Actualizar la mejor soluciÃ³n _bestSolution
		 * 4. Insertarlas en la poblaciÃ³n
		 */
		for (unsigned i = 0; i < popSize; i++) {
			// Generamos una solución aleatoria
			MQKPSolution* sol = new MQKPSolution(_instance);
			MQKPSolGenerator::genRandomSol(*_instance, *sol);

			// Evaluamos y actualizamos la mejor
			double fitness = MQKPEvaluator::computeFitness(*_instance, *sol);
			sol->setFitness(fitness);
			if (MQKPEvaluator::compare(fitness, _bestSolution->getFitness()) > 0) {
				_bestSolution->copy(*sol);
			}
			_results.push_back(fitness);
			_population.push_back(sol);
		}
	}

	/**
	 * FunciÃ³n que calcula la media del fitness de un conjunto de soluciones
	 * @param[int] set Conjunto de soluciones del que obtener la media del fitness
	 * @return media del fitness de las soluciones
	 */
	double computeMeanFitness(vector<Solution*>&set) {
		double mean = 0.;
		unsigned numElements = (unsigned) set.size();
		double i_numElements = 1. / numElements;

		for (auto sol : set) {
			double fitness = sol->getFitness();
			mean += (fitness * i_numElements);
		}

		return mean;
	}

public:

	/**
	 * Constructor
	 */
	MQKPGeneticAlgorithm() {
		_popSize = 0;
		_bestSolution = NULL;
	}

	/**
	 * Destructor
	 */
	virtual ~MQKPGeneticAlgorithm() {

		for (unsigned i = 0; i < _popSize; i++) {
			delete (_population.back());
			_population.pop_back();
		}

		if (_bestSolution != NULL){
			delete _bestSolution;
			_bestSolution = NULL;
		}
	}

	/**
	 * FunciÃ³n que ejecuta el algoritmo genÃ©tico
	 * @param[in] stopCondition Objeto que define cuÃ¡ndo se llega a la condiciÃ³n de parada
	 */
	virtual void run(MQKPStopCondition &stopCondition) {

		/**
		 * HECHO
		 * 1. Inicializar la poblacion
		 * 2. Mientras no se alcance la condiciÃ³n de parada
		 *   a. Almacenar la media de la poblaciÃ³n actual y la mejor soluciÃ³n
		 *   b. Seleccionar los padres
		 *   c. Cruzar los padres
		 *   d. Mutar los descendientes
		 *   f. Almacenar la media de los descendientes
		 *   g. Seleccionar la nueva poblaciÃ³n
		 *
		 * 3. Almacenar la media de la poblaciÃ³nfinal y la mejor soluciÃ³n
		 */

		initPopulation(_popSize);

		while (stopCondition.reached() == false) {

			_popMeanResults.push_back(computeMeanFitness(_population));
			_bestPerIterations.push_back(
					_population.at(indexBest(_population))->getFitness());

			vector<Solution*> parents;
			_selector->select(_population, parents);

			vector<Solution*> offspring;
			_crossoverOp->cross(parents, offspring);
			_mutOp->mutate(offspring);

			evaluate(offspring); // Recalcula el fitness de cada solución
			
			_offMeanResults.push_back(computeMeanFitness(offspring));

			selectNewPopulation(offspring);
			stopCondition.notifyIteration();
		}

		_popMeanResults.push_back(computeMeanFitness(_population));
		_bestPerIterations.push_back(
				_population.at(indexBest(_population))->getFitness());
	}

	/**
	 * FunciÃ³n que inicializa el algoritmo
	 * @param[in] popSize TamaÃ±o de la poblaciÃ³n
	 * @param[in] instance Instancia del problema a abordar
	 */
	void initialise(unsigned popSize, MQKPInstance &instance) {
		_instance = &instance;

		if (popSize <= 0) {
			cerr << "The population size must be greater than 0" << endl;
			exit(1);
		}

		if (_bestSolution != NULL) {
			delete _bestSolution;
			_bestSolution = NULL;
		}

		_bestSolution = new MQKPSolution(*_instance);
		MQKPSolGenerator::genRandomSol(*_instance, *_bestSolution);
		double fitness = MQKPEvaluator::computeFitness(*_instance, *_bestSolution);
		_bestSolution->setFitness(fitness);

		_popSize = popSize;

		/**
		 * Se estÃ¡ configurando para que utilice torneo binario, los operadores genÃ©ticos implementados y dichas probabilidades,
		 * pero se podrÃ­an usar otros operadores simplemente cambiando el objeto.
		 */
		if (_crossoverOp == NULL) {
			_crossoverOp = new MQKPCrossoverOperator(0.8, *_instance);
		}

		if (_mutOp == NULL) {
			_mutOp = new MQKPMutationOperator((0.25 / _instance->getNumObjs()),
					*_instance);
		}

		if (_selector == NULL) {
			_selector = new TournamentSelector(2); //Se puede probar con varios valores de presiÃ³n selectiva
		}
	}

	/**
	 * FunciÃ³n que devuelve el vector de fitness de las soluciones generadas
	 */
	vector<double>& getResults() {
		return _results;
	}

	/**
	 * FunciÃ³n que asigna un nuevo operador de cruce
	 */
	void setCrossoverOp(MQKPCrossoverOperator* crossoverOp) {

		if (_crossoverOp != NULL)
			delete _crossoverOp;

		_crossoverOp = crossoverOp;
	}

	/**
	 * FunciÃ³n que asigna un nuevo operador de mutaciÃ³n
	 */
	void setMutOp(MQKPMutationOperator* mutOp) {

		if (_mutOp != NULL)
			delete _mutOp;

		_mutOp = mutOp;
	}

	/**
	 * FunciÃ³n que asigna un nuevo operador de selecciÃ³n
	 */
	void setSelector(SelectionOperator* selector) {

		if (_selector != NULL)
			delete _selector;

		_selector = selector;
	}

	/**
	 * FunciÃ³n que devuelve el vector con la media de las poblaciones de descendientes generadas en cada iteraciÃ³n
	 */
	const vector<double>& getOffMeanResults() const {
		return _offMeanResults;
	}

	/**
	 * FunciÃ³n que devuelve el vector con la media de la poblaciÃ³n actual en cada iteraciÃ³n
	 */
	const vector<double>& getPopMeanResults() const {
		return _popMeanResults;
	}

	/**
	 * FunciÃ³n que devuelve la mejor soluciÃ³n de la poblaciÃ³n en cada iteraciÃ³n
	 */
	const vector<double>& getBestsPerIterations() const {
		return _bestPerIterations;
	}
};

#endif /* INCLUDE_MQKPGENETICALGORITHM_H_ */
