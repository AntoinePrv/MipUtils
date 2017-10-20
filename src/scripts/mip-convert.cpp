#include <iostream>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN


int main (int argc, char **argv){
	if(argc < 3)
		std::cout << "Usage: mip-convert in-file out-file" << endl;
	else{
		// Initializating Cplex objects
		IloEnv env;
		IloModel model(env, "Imported Model");
		IloCplex cplex(model);
		cplex.setOut(env.getNullStream());

		// Reading model from file using Cplex
		cplex.importModel(model, argv[1]);
		// Exporting model
		cplex.exportModel(argv[2]);

		env.end();
	}
}
