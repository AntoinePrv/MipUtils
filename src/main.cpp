#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN


using namespace std;


void insert_coefs_sparse(
	stringstream& rowCoef,
	const IloRange& row,
	const unordered_map<string, unsigned>& map
){
	for(IloExpr::LinearIterator it = row.getLinearIterator(); it.ok(); ++it){
		unsigned int i = map.at(it.getVar().getName());
		rowCoef << i << ":" << it.getCoef() << ",";
	}
}


void insert_coefs_dense(
	stringstream& rowCoef,
	const IloRange& row,
	const unordered_map<string, unsigned>& map
){
	unsigned int lastIndex = -1;
	for(IloExpr::LinearIterator it=row.getLinearIterator(); it.ok(); ++it){
		unsigned int index = map.at(it.getVar().getName());
		for(unsigned j=lastIndex+1; j<index; ++j)
			rowCoef << "0,";
		rowCoef << it.getCoef() << ",";
		lastIndex = index;
	}
	for(unsigned j=lastIndex+1; j<map.size(); ++j)
		rowCoef << "0,";
}


void insert_bounds(stringstream& constraint, const IloRange& ctr){
	double lb = ctr.getLB();
	double ub = ctr.getUB();

	if( lb == ub ){
		constraint << "0," << lb << endl;
		return;
	}

	stringstream otherConstraint;
	if( !isinf(ub) ){
		otherConstraint << constraint.rdbuf();
		otherConstraint << "-1," << ub << endl;
	}
	if( !isinf(lb) )
		constraint << "1," << lb << endl;

	constraint << otherConstraint.rdbuf();
}


inline string constraint_sparse(
	const IloRange& ctr,
	const unordered_map<string, unsigned>& map
){
	stringstream constraint;
	insert_coefs_sparse(constraint, ctr, map);
	insert_bounds(constraint, ctr);
	return constraint.str();
}


inline string constraint_dense(
	const IloRange& ctr,
	const unordered_map<string, unsigned>& map
){
	stringstream constraint;
	insert_coefs_dense(constraint, ctr, map);
	insert_bounds(constraint, ctr);
	return constraint.str();
}


int main (int argc, char **argv){
	char* filename = argv[1];

	IloEnv env;
	IloModel model(env, "Import Model");
	IloCplex cplex(env);
	cplex.setOut(env.getNullStream());

	IloObjective obj(env);
	IloNumVarArray vars(env);
	IloRangeArray ctrs(env);

	cplex.importModel(model, filename, obj, vars, ctrs);

	unordered_map<string, unsigned int> map;
	for(unsigned i=0; i<vars.getSize(); ++i)
		map[vars[i].getName()] = i;

	for(unsigned i=0; i<ctrs.getSize(); ++i)
		cout << constraint_dense(ctrs[i], map);

	env.end();
}
