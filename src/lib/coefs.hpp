#ifndef COEFS_HEADER
#define COEFS_HEADER


#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN


using namespace std;

void process(
	string filename,
	ostream& out,
	bool sparse=true,
	bool keep_empty=true
);

unsigned count_constraints(const IloRangeArray ctrs, bool keep_empty);

void insert_coefs_sparse(
	stringstream& rowCoef,
	IloExpr::LinearIterator it,
	const unordered_map<string, unsigned>& map
);

void insert_coefs_dense(
	stringstream& rowCoef,
	IloExpr::LinearIterator it,
	const unordered_map<string, unsigned>& map
);

void insert_bounds(stringstream& constraint, const IloRange& ctr);

inline string constraint_sparse(
	const IloRange& ctr,
	const unordered_map<string, unsigned>& map
){
	stringstream constraintCoefs;
	insert_coefs_sparse(constraintCoefs, ctr.getLinearIterator(), map);
	insert_bounds(constraintCoefs, ctr);
	return constraintCoefs.str();
}

inline string constraint_dense(
	const IloRange& ctr,
	const unordered_map<string, unsigned>& map
){
	stringstream constraintCoefs;
	insert_coefs_dense(constraintCoefs, ctr.getLinearIterator(), map);
	insert_bounds(constraintCoefs, ctr);
	return constraintCoefs.str();
}

inline string objective_sparse(
	const IloObjective& obj,
	const unordered_map<string, unsigned>& map
){
	stringstream objectiveCoefs;
	insert_coefs_sparse(objectiveCoefs, obj.getLinearIterator(), map);
	objectiveCoefs << endl;
	return objectiveCoefs.str();
}

inline string objective_dense(
	const IloObjective& obj,
	const unordered_map<string, unsigned>& map
){
	stringstream objectiveCoefs;
	insert_coefs_dense(objectiveCoefs, obj.getLinearIterator(), map);
	objectiveCoefs << endl;
	return objectiveCoefs.str();
}

string variables(const IloNumVarArray& vars);


#endif
