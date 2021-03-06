#include "coefs.hpp"


void process(string filename, ostream& out, bool sparse, bool keep_empty){
	// Initializating Cplex objects
	IloEnv env;
	IloModel model(env, "Imported Model");
	IloCplex cplex(env);
	cplex.setOut(env.getNullStream());
	IloObjective obj(env);
	IloNumVarArray vars(env);
	IloRangeArray ctrs(env);

	// Reading model from file using Cplex
	cplex.importModel(model, filename.c_str(), obj, vars, ctrs);

	// Initializating a map name to index
	unordered_map<string, unsigned int> map;
	for(unsigned i=0; i<vars.getSize(); ++i)
		map[vars[i].getName()] = i;

	// Header
	// Number of actual constraints may change
	out << count_constraints(ctrs, keep_empty) << "," << vars.getSize() << endl;

	// Outputs objective
	if(sparse)
		out << objective_sparse(obj, map);
	else
		out << objective_dense(obj, map);

	// Outputs constraints
	for(unsigned i=0; i<ctrs.getSize(); ++i){
		if(!keep_empty && !ctrs[i].getLinearIterator().ok())
			continue;
		if(sparse)
			out << constraint_sparse(ctrs[i], map);
		else
			out << constraint_dense(ctrs[i], map);
	}

	// Outputs variables bounds and type
	out << variables(vars);

	env.end();
}


unsigned count_constraints(const IloRangeArray ctrs, bool keep_empty){
	unsigned cnt = 0;
	for(unsigned i=0; i<ctrs.getSize(); ++i){
		IloRange ctr = ctrs[i];
		if(!keep_empty && !ctrs[i].getLinearIterator().ok())
			continue;
		double lb = ctr.getLB();
		double ub = ctr.getUB();
		if( !isinf(lb) ) ++cnt;
		if( !isinf(ub) ) ++cnt;
		if( lb == ub ) --cnt;
	}
	return cnt;
}


void insert_coefs_sparse(
	stringstream& rowCoef,
	IloExpr::LinearIterator it,
	const unordered_map<string, unsigned>& map
){
	bool first = true;
	for(; it.ok(); ++it){
		if(!first)
			rowCoef << ",";
		else
			first = false;
		unsigned int i = map.at(it.getVar().getName());
		rowCoef << i << ":" << it.getCoef();
	}
}


void insert_coefs_dense(
	stringstream& rowCoef,
	IloExpr::LinearIterator it,
	const unordered_map<string, unsigned>& map
){
	bool first = true;
	unsigned int lastIndex = -1;
	for(; it.ok(); ++it){
		unsigned int index = map.at(it.getVar().getName());
		for(unsigned j=lastIndex+1; j<index; ++j){
			if(!first) rowCoef << ","; else first = false;
			rowCoef << 0;
		}
		if(!first) rowCoef << ","; else first = false;
		rowCoef << it.getCoef();
		lastIndex = index;
	}
	for(unsigned j=lastIndex+1; j<map.size(); ++j){
		if(!first) rowCoef << ","; else first = false;
		rowCoef << "0,";
	}
}


void insert_bounds(stringstream& constraint, const IloRange& ctr){
	double lb = ctr.getLB();
	double ub = ctr.getUB();

	// Case equality
	if( lb == ub ){
		constraint << ",0," << lb << endl;
		return;
	}

	// Case two bounds
	if (!isinf(ub) && !isinf(lb)){
		stringstream otherConstraint;
		otherConstraint << constraint.rdbuf();
		constraint << ",1," << ub << endl;
		otherConstraint  << ",-1," << lb << endl;
		constraint << otherConstraint.rdbuf();
		return;
	}

	// Case one constraint
	if( !isinf(ub) )
		constraint << ",1," << ub << endl;
	if( !isinf(lb) )
		constraint << ",-1," << lb << endl;
}


string variables(const IloNumVarArray& vars){
	stringstream constraintCoefs;
	for(int i=0; i<vars.getSize(); ++i){
		constraintCoefs << vars[i].getType() << ",";
		constraintCoefs << vars[i].getLB() << ",";
		constraintCoefs << vars[i].getUB() << endl;
	}
	return constraintCoefs.str();
}
