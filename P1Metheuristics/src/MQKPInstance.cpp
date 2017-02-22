/*
 * MQKPInstance.cpp
 *
 * Fichero que define los métodos de la clase MQKPInstance. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPInstance.h"
#include "MQKPSolution.h"

//MQKPInstance::MQKPInstance(int numMochilas, int numObjetos, vector<vector<double> > beneficios, vector<double> pesos, vector<double> capacidades) {
MQKPInstance::MQKPInstance() {
	_numKnapsacks = 5;
	_numObjs = 0;
	//setNumKnapsacks(5);
	//setNumObjs(5);
	/*setProfits(beneficios);
	setWeights(pesos);
	setCapacities(capacidades);*/
}

MQKPInstance::~MQKPInstance() {

	for(int i=0; i<getNumObjs(); i++)
		_profits[i].clear();

	_profits.clear();
	_weights.clear();
	_capacities.clear();
}

int MQKPInstance::getNumKnapsacks() const {return _numKnapsacks;}
int MQKPInstance::getNumObjs() const {return _numObjs;}

void MQKPInstance::setNumKnapsacks(const int &numMochilas) {

	if(numMochilas>0)
		_numKnapsacks = numMochilas;
	else {
		cout << "\nError al intentar modificar el número de mochilas. Debe de ser un número positivo y mayor que cero.\n";
		exit(EXIT_FAILURE);
	}
}

void MQKPInstance::setNumObjs(const int &numObjetos) {

	if(numObjetos>0)
		_numObjs = numObjetos;
	else {
		cout << "\nError al intentar modificar el número de objetos. Debe de ser un número positivo y mayor que cero.\n";
		exit(EXIT_FAILURE);
	}
}

void MQKPInstance::setProfits(const vector<vector<double> > &beneficios) {_profits = beneficios;}
void MQKPInstance::setWeights(const vector<double> &pesos) {_weights = pesos;}
void MQKPInstance::setCapacities(const vector<double> &capacidades) {_capacities = capacidades;}

double MQKPInstance::getMaxCapacityViolation(MQKPSolution &solution) {

	double *sumWeights = new double[this->_numKnapsacks + 1];

	for (int j = 1; j <= this->_numKnapsacks; j++) {
		sumWeights[j] = 0;
	}

	int objectPlace; // Mochila en la que se encuentra el objeto i-ésimo. (Será 0 en caso de no estar en ninguna mochila)

	for (int i = 0; i < this->_numObjs; i++) {

		/*TODO Completar
		 * 1. Obtener la mochila en la que se encuentra el objeto i-ésimo
		 * 2. Si se encuentra en una mochila válida (mayor que 0), incrementar con el peso del objeto el valor correspondiente en sumWeights.
		 */
		objectPlace = solution.whereIsObject(i);

		if(objectPlace > 0) {
			sumWeights[objectPlace] += this->_weights[i];
		}


	}

	double maxCapacityViolation = 0; //Inicializamos la máxima violación de alguna mochila a 0, que significa que no hay ninguna violación

	double localCapacityViolation; 	 // Variable auxiliar que contiene el exceso de peso en una mochila.

	for (int j = 1; j <= this->_numKnapsacks; j++) {

		/*TODO Completar
		 * 1. Calcular la violación en la mochila j-ésima
		 * 2. Actualizar maxCapacityViolation en su caso
		 */
		localCapacityViolation = sumWeights[j] - this->_capacities[j];
		if(localCapacityViolation > maxCapacityViolation)
			maxCapacityViolation = localCapacityViolation;

	}

	delete[] sumWeights;
	return maxCapacityViolation;
}

double MQKPInstance::getSumProfits(MQKPSolution &solution) {

	double sumProfits = 0.;

	/*TODO COMPLETAR
	 * Doble bucle para cada par de objetos
	 * Todo objeto incluido en alguna mochila (> 0) debe sumar su beneficio individual
	 * Todo par de objetos incluidos en la misma mochila (y > 0) debe sumar su beneficio conjunto. IMPORTANTE, sumar los pares (i,j) sólo una vez, es decir, si se suma (i, j), no se debe sumar (j, i)
	 */

	for(int i = 0; i < this->getNumKnapsacks(); i++) {
		for(int j = i; j < this->getNumKnapsacks(); j++) {

			if((solution.whereIsObject(i) == solution.whereIsObject(j)) and (solution.whereIsObject(i)>0))
				sumProfits += _profits[i][j];
		}
	}

	return sumProfits;
}

void MQKPInstance::readInstance(char *filename, int numKnapsacks) {

	/*
	 * TODO completar esta función:
	 *   1. leer el número de objetos
	 *   2. reservar la memoria de vectores y matrices
	 *   3. leer beneficios y pesos de los objetos según lo comentado arriba
	 *   4. Calcular las capacidades de las mochilas:
	 *      . Calcular la suma de los pesos de todos los objetos
	 *      . Multiplicar por 0.8
	 *      . Dividir el resultado anterior entre el número de mochilas. Eso será la capacidad de cada mochila
	 */
	vector <string> fich;
	vector <double> fila, diagonal;
	string nom = filename;
	int i, j, numElem, aux;
	double pesoT = 0, cap;
	vector <vector <double> > ben;
	vector <double> pes;
	vector <double> capacidad;


	fich = cargarFichero(nom);

	/* *
	 * Número de elementos
	 */
	fila = stringToDouble(fich[1]);
	numElem = fila[0];
	setNumObjs(numElem);

	diagonal = stringToDouble(fich[2]);	// Beneficios individuales

	/* *
	 * Reserva de memoria
	 */
	pes.resize(getNumObjs());

	ben.resize(getNumObjs());
	for(i=0; i<(int)ben.size(); i++) {
		ben[i].resize(getNumObjs());
	}



	/* *
	 * Lectura de beneficios
	 */
	for(i=0; i<getNumObjs(); i++) {
		aux=0;
		ben[i][i] = diagonal[i];
		fila = stringToDouble(fich[3+i]);
		for(j=i+1; j<getNumObjs(); j++) {
			ben[i][j] = fila[aux];
			ben[j][i] = ben[i][j];
			aux++;
		}
		fila.resize(0);
	}

	setProfits(ben);


	/* *
	 * Lectura de pesos
	 */
	fila = stringToDouble(fich[5+getNumObjs()]);

	for(i=0; i<getNumObjs(); i++) {
		pes[i] = fila[i];
		pesoT += fila[i];
	}

	setWeights(pes);

	/* *
	 * Cálculo de las capacidades
	 */
	cap = (pesoT*0.8)/getNumKnapsacks();

	/* *
	 * Reserva e inicialización del vector de capacidades
	 */
	capacidad.resize(getNumKnapsacks()+1);

	for(i=0; i<(getNumKnapsacks()+1); i++)
		capacidad[i] = cap;

	setCapacities(capacidad);
}


