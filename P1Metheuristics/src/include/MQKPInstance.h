/**
 * MQKPInstance.h
 * 
 * Fichero que define la clase MQKPInstance. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#ifndef __MQKPINSTANCE_H__
#define __MQKPINSTANCE_H__

#ifndef __MQKPSOLUTION_H__
#include "MQKPSolution.h"
#include "funciones.h"
#else
class MQKPSolution;
#endif

#include <time.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
//#include <funciones.h>

using namespace std;

/**
 * Clase que almacena la información de una instancia del problema MQKP
 */
class MQKPInstance {
protected:
	/*TODO Definir las variables miembro
	 * _numKnapsacks Entero que indica el número de mochilas que se va a considerar. Este no se lee del fichero, sino que lo establecéis vosotros
	 * _numObjs Entero donde se almacenará el número de objetos del problema
	 * _profits Matriz donde se van a almacenar los beneficios de los objetos según lo indicado arriba. HAY QUE RESERVAR SU MEMORIA
	 * _weights Vector con los pesos de los objetos. HAY QUE RESERVAR SU MEMORIA
	 * _capacities Vector con las capacidades de las mochilas. HAY QUE RESERVAR SU MEMORIA. Para evitar problemas, reservadlo con capacidad (1 + numKnapsacks) y utilizadlo desde el índice 1 en adelante
	 */

	int _numKnapsacks;					// Entero que indica el número de mochilas que se va a considerar. Este no se lee del fichero, sino que lo establecéis vosotros
	 int _numObjs;						// Entero donde se almacenará el número de objetos del problema
	 vector<vector<double> > _profits; 	// Matriz donde se van a almacenar los beneficios de los objetos según lo indicado arriba. HAY QUE RESERVAR SU MEMORIA
	 vector<double> _weights;			// Vector con los pesos de los objetos. HAY QUE RESERVAR SU MEMORIA
	 vector<double> _capacities;		// Vector con las capacidades de las mochilas. HAY QUE RESERVAR SU MEMORIA. Para evitar problemas, reservadlo con capacidad (1 + numKnapsacks) y utilizadlo desde el índice 1 en adelante


public:
	/**
	 * Constructor por defecto
	 */
	//MQKPInstance(int numMochilas, int numObjetos, vector<vector<double> > beneficios, vector<double> pesos, vector<double> capacidades);
	 MQKPInstance();
	/**
	 * Destructor
	 */
	~MQKPInstance();

	/**
	 * Función de lectura de un fichero de entrada
	 * @param[in] filename Nombre del fichero donde están los datos de la instancia
	 * @param[in] numKnapsacks Entero que indica el número de mochilas que se va a considerar. Este no se lee del fichero, sino que lo establecéis vosotros
	 */
	void readInstance(char *filename, int numKnapsacks);

	//TODO declarar los métodos (y definirlos en MQKPInstance.cpp) que devuelvan la información que se consulte acerca de la instancia del problema
	//Entre otros getNumObjs() y getNumKnapsacks()

	int getNumKnapsacks()const;
	int getNumObjs()const;

	void setNumKnapsacks(const int &numMochilas);
	void setNumObjs(const int &numObjetos);
	void setProfits(const vector<vector<double> > &beneficios);
	void setWeights(const vector<double> &pesos);
	void setCapacities(const vector<double> &capacidades);

	/**
	 * Función que devuelve por cuanto se viola la capacidad de la mochila que está más cargada de más
	 * @param[in] solution Referencia a un objeto que representa una solución al problema
	 * @return Máxima violación de las capacidades de las mochilas
	 */
	double getMaxCapacityViolation(MQKPSolution &solution);

	/**
	 * Función que calcula la suma de beneficios individuales y cuadráticos de los objetos en mochilas
	 * @param[in] solution Referencia a un objeto que representa una solución al problema
	 * @return Suma de los beneficios individuales y cuadráticos de los objetos en las mochilas
	 */
	double getSumProfits(MQKPSolution &solution);


};

#endif

