#pragma once
#include "..\model\Soil\SoilModel.h"
#include "..\Model\Atmosphere\AtmModel.h"
#include "..\model\Plant\PlantModel.h"
#include "..\model\Structure\StructureModel.h"
#include "..\Model\SOIL_NC\SoilOrgModel.h"
#include "..\Model\PLANT_NC\PlantOrgModel.h"
using namespace std;
class ModelInfo;
class ModelMap;
class Structure;

class StateUpdate {

public:
	StateUpdate(double& x, double& tstep) : X(x), tstep(tstep) {}
	void operator+ (double& fluxin) { X += fluxin * tstep; }
	void operator- (double& fluxin) { X -= fluxin * tstep; }


private:
	double& X;
	double& tstep;

};
class StateSingleUpdate {
	StateSingleUpdate(double& x, double& fluxin, double& tstep) : X(x), fluxin(fluxin), tstep(tstep) {};

	void operator() () {
		X += fluxin * tstep;
	}


private:
	double X, fluxin;
	double tstep;

};
class StateVectorUpdate {
	StateVectorUpdate(double& x, vector<double>& fluxin, double& tstep) : X(x), fluxin(fluxin) {};

	void operator() () {
		for (double F : fluxin) {
			X += F * tstep;
		}
	}


private:
	double X;
	vector<double> fluxin;
	double tstep;

};

class VectorStateVectorUpdate {
	VectorStateVectorUpdate(vector<pair<double&, double&>>& fluxin, double& tstep) : fluxin(fluxin) {};

	void operator() () {
		for (pair<double&, double&> F : fluxin) {
			auto koll = F.first;
			F.first += F.second * tstep;
		}
	}


private:
	double X;
	vector<pair<double&, double&>> fluxin;
	double tstep;

};






class DefineModel {

public:
	DefineModel();
	~DefineModel();

	ModelMap* GetModelMap() { return p_ModelMap; };
	string GetDBPassword() { return postgrespassword; };


private:
	double XState;
	double Flux;
	vector<size_t*> m_sizet;

	ModelInfo* p_ModelInfo;
	ModelMap* p_ModelMap;
	Structure* p_ModelStructure;
	SoilModel soilmodel;
	AtmModel atmmodel;
	PlantModel plantmodel;
	StructureModel structuremodel;
	PlantOrgModel plantorgmodel;
	SoilOrgModel soilorgmodel;

	string postgrespassword{ "pe1950win" };


};
