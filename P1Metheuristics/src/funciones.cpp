#include "funciones.h"

std::vector<std::string> cargarFichero(const std::string& nomfich){
	std::string linea;
	std::vector <std::string> lineas;
	std::ifstream f(nomfich.c_str());

	while(std::getline(f,linea)){
			lineas.push_back(linea);
	}
	return lineas;
}

std::vector <double> stringToDouble(const std::string str){
	std::stringstream iss(str);

	int num;
	std::vector<double> nums;
	while ( iss >> num ){
 		nums.push_back( num );
	}
	return nums;
}
