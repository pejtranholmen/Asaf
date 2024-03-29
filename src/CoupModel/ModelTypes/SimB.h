#pragma once
#ifndef MISSING
#define MISSING -1.1E38f
#define NUM_SERIES 200
#define NUM_BAND 16
#define NUM_TIME 50
#endif
#include <vector>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>


using namespace std;

enum sumtype { INITIAL, FINAL, MINVALUE, MAXVALUE, MEANVALUE, ACCVALUE };
enum valtype { R2VALUE, INTERCEPT, SLOPE, MERROR, RMSE, LOGLIKE, NSE };
enum vartype { NO_USE, DATEAXIS, MULTIRUN_NUM, PGVAR, SUMVAR, VALVAR, PARVAR, PGREPIT, PGYEAR, PGMONTH, PGDAY, PGHOUR, PGDEPTH, PGVARVALID, GEO_LAT, GEO_LONG, GEO_ALT, VAR_MIN, VAR_MAX, VAR_MISSING, VAR_FIRST, VAR_LAST, VAR_MEAN, VALVARSUM };

enum class DATA_TYPES {
	ANYDATA, SIM_X, SIM_T, SIM_G, SIM_D, SIM_VAL,
	SIM_PERFORMANCE, SIM_PAR, SIM_OUTSUM, SIM_SUMVAR, PGDATA, MEASURED_VAL, SIM_OUTSUM_X, SIM_OUTSUM_Y
};
enum class CHART_TYPES { SCATTER, TIME_SERIE, FREQUENCE, CUM_FREQUENCE, CONTOUR_CHART };
enum class TimeResolution { WITHIN_DAY, DAILY_MEAN, HOURLY, TEN_MIN };
enum class SIMB_COMP{NOVALUE, LE, LT, GE, GT, EQ, NEQ, ON, OFF};
enum class HIST_INFO {RUN_INFO, DATA_BASE_INFO, SWITCH_INFO, PARSINGLE_INFO, PARTABLE_INFO,NUMBER_ELEMENTS, MODELFILE_INFO};
struct OUTPUT {size_t LocalIndex; float Initial; float Final; float Min; float Max; float Mean; float Accumulated; size_t ValidFlagIndex; float OptInitial; size_t PgFileIndex; size_t PgMultiFileIndex;int StoreFlag;}	;
static vector<string> simType_StringId = { "Ps","X","T","G","D","Sw","F", "Ps", "Xs","Ts", "Gs", "Ds", "P", "Table","DB","Func","CSV" };
static vector<string> Sw_StringOptions = { "O1","O2","O3", "O4", "O5", "O6", "O7", "O8","O9","O10","O11", "O12", "O13", "O14", "O15", "O16" , "O17", "O18" };
enum simtype {PAR, STATE, FLOW, AUX, DRIVE, SWITCH, PGFILE, PAR_SINGLE, STATE_SINGLE, FLOW_SINGLE,AUX_SINGLE, DRIVE_SINGLE, PAR_TABLE, TABLE, DB, FUNCTION, CSVFILE};
enum datatype{SINGLE, SIMPROFILE, MPROFILE,SINGLEPLANT, PLANT, MYCON,OTHER, WATERPIPE, HEATBOX, ATMBOUNDARY, SURFACEPOOL, WATERPOND, 
	IRRIGATIONCONTAINER,ROADSURFACE, SOILSURFACE,SOILBOTTOM, OBSERVED, TOTPROFILE,ALL_SOIL_LAYERS,ALL_SOIL_BOUNDARIES, ALLPLANTS,ACCUMULATOR, FILEDEFINEDPARAMETERS};
enum elements {
	WATER, HEAT, CARBON, NITROGEN, PHOSPHORUS, RADIATION, SNOW, SALT, METHANE, VAPOUR, GENERAL, TEMPERATURE, WATERPOTENTIAL, ATMOSPHERE,
	AIR, PRECIPITATION, CLIMATE, ORGANIC, DOC, DON, DOP, MINERAL_N, MINERAL_P, AMMONIUM, NITRATE, MYCORRHIZA, DENITRIFIERS, NITRIFIERS, ANAROBIC_GAS, OXYGEN, MANURE, PLANT_SHAPE, DISTANCE, SOILCO2, TIME, AREA, PLANT_STRESS, WIND,
	GLOBALRADIATION, LONGWAVEINCOMINGRADIATION, HUMIDITY, GHG, PLANT_BIOMASS , SOILGAS_NO, SOILGAS_N2O , COS
};
enum fysprocess{WATERUPTAKE, EVAPOTRANSPIRATION, TRANSPIRATION, SOILEVAPORATION,POTTRANSPIRATION, INTERCEPTION, TURBULENCE, SURFACERUNOFF,
	RESISTANCE,SNOW_MELTING,
	REDISTRIBUTION,STORAGE,TRANSPORT, INFILTRATION, DRAINAGE, PERCOLATION, RUNOFF,NOFPROC,INITF, SHRINKING, FYS_SHAPE, 
	EXTRACTION, HEATPIPE, HEATSOURCE,LOWERBOUNDARY,UPPERBOUNDARY,WATERSOURCE,SALTSOURCE,
	EBAL_PARTITIONING,HEAT_CONDUCTION, HEAT_CONVECTION, TECH_REGULATOR, EMISSION,ADSORPTION,REFLECTION, MASSBALANCE, SENSOR, GEOPOS, FROST_BOUNDARY, HYSTERESIS,IRRIGATION, BUDGET,
	GEOMETRIC, SCATTERING, SOIL_FREEZING, DIFFUSION};
enum bioprocess{DECOMPOSITION,MINERALISATION, NITRIFICATION, DENITRIFICATION, RESPIRATION, PRODUCTION,OXIDATION, REALLOCATION, PHENOLOGY, SOIL_GAS_DYNAMICS,
	PHOTOSYNTHESIS, LITTERFALL,NOBPROC, INITB, INIT_P,FORCING_BIOLOGY, ORGANICTRANSFORMATION, RATIO,CONCENTRATION, PLANTORGUPTAKE, PLANT_NUPTAKE, PLANT_PUPTAKE, SORPTION, HARVEST,
	GRAZING, MYCON_NUPTAKE, SOILMIXING, EMERGENCE, PLANT_MANAGEMENT, NFIXATION, LIVING, NON_LIVING, GROWTH, WOODEATING, WEATHERING, METHANE_OXIDATION, METHANE_PRODUCTION, METHANE_TRANSPORT,
	SOIL_GAS_EQUILIBRIUM, COS_TRANSPORT
};
enum userlevel{SIMPLE, NORMAL, DETAILED};
enum MR_METHOD{LINEAR_CHANGE, LOG_CHANGE, STOCHASTIC_LIN_CHANGE, STOCHASTIC_LOG_CHANGE, TABLE_VAL, DB_RECORD, SAME_AS, REL_SAME_AS, OP_REL_SAME_AS, BAYESIAN_CAL, CONNECTED, BAYESIAN_CAL_LOG, NELDER_MEAD};

enum class UNIT_TYPES {
	MEGA_HEATFLOW_UNIT,
	HEATFLOW_UNIT, HEATSOURCE_UNIT, HEATSTORE_UNIT, MEGA_HEATSTORE_VOL_UNIT,
	TEMP_UNIT, TEMPSUM_UNIT, HEATCOND_UNIT, MASS_UNIT, CARBON_MASS_UNIT, CARBON_MASSFLOW_UNIT, NITROGEN_MASS_UNIT, NITROGEN_MASSFLOW_UNIT, MASSFLOW_UNIT, WIND_SPEED_UNIT,
	WATERFLOW_UNIT, WATERCONTENT_UNIT, PRESSUREHEAD_UNIT, AREA_UNIT, MASSDENSITY_UNIT, KILO_MASSDENSITY_UNIT, INVERSE_MASS_UNIT,
	mMASS_UNIT, mMASSFLOW_UNIT, WATERMASS_UNIT, PRESSURE_UNIT, LENGTH_UNIT, LENGTH_PER_TIME_UNIT, AERODYNAMIC_RESISTANCE_UNIT,
	MIKRO_MOL_SEC_RATE, AREA_PER_TIME_UNIT, CM_AREA_PER_SEC_TIME_UNIT, GAS_MASS_UNIT, GAS_CONC_UNIT, YEAR_AGE_UNIT, PERCENTAGE, TIME_UNIT, ENERGY_MOLE,
	mMASS_LITER_UNIT, mikro_MASS_LITER_UNIT, NO_UNIT, RATECOEF_UNIT, INVERSE_WATERFLOW_UNIT, INVERSE_SQUARED_FLUX_PER_VOLUME, MASS_RATIO_UNIT, VOLUME_RATIO_UNIT,
	MIKRO_MASS_RATIO, MIKRO_MASS_FLOW_UNIT, MIKRO_MASS_RATE, TIME_MINUTE_UNIT, ANGLE_UNIT , THERMAL_CONDUCTIVY_UNIT, INVERSE_AREA_UNIT, DAYOFYEAR_UNIT, INVERSE_TEMP_UNIT

};
struct DEPTH_type {
	size_t NumLay=0;
	std::vector<double> Dist;
};

enum class ENSEMBLE_TYPE { NO, ALL, ACCEPTED };




class SimB;
class CommonModelInfo;
struct SIMB {SimB* pBase=nullptr;
	size_t TabIndex=string::npos;
};
struct MR_PAR{
	int Dimension=0;
	double Min=0.;
	double Max=0.;
	double Start=0.;
	double RelChange=0.;
	enum MR_METHOD Method=MR_METHOD::LINEAR_CHANGE;
	SIMB MR_Dependent;
};
struct PLOT_PARAM {
	size_t Tot_Num_Of_Curves=0; // Correspnds to Num_Of_Curves*Num_Of_Opt 
	size_t Num_Of_Curves=0;
	size_t Num_Of_Points=0;
	bool LogXScale=false;
	bool LogYScale=false;
	int X_ScalingCoef = 0;
	int Y_ScalingCoef = 0;
	double X_Min=0.;
	double X_Max=0.;
	string X_MaxPar;
	string X_Label;
	string Y_Label;
	size_t Num_Of_X=0;
	size_t Num_Of_Opt=0;
	vector<string> LegendsVec;
	vector<size_t> Opt_Vec;
	vector<float> X2_Vec;
	vector<float> X3_Vec;
	vector<float> X4_Vec;
	vector<float> X5_Vec;
};
struct PLOT_PARAM_DEP {
	vector<string> Ps_Names;
	vector<string> P_Names;
};
enum DB_types
{
	Plant_Hydrology,
	Soil_Properties,
	Soil_Evaporation,
	Snow,
	Soil_Freezing,
	Soil_Water_Flows,
	Plant_Growth,
	Soil_Manage,
	Soil_Organic,
	Soil_InOrganic
};

class CE; class Sw; class NewMap;
struct Condition {
	Sw*	variable;
	enum  SIMB_COMP op;
	int		value;
};

struct definitions {enum datatype data; enum elements elem; enum fysprocess fproc; enum bioprocess bproc; enum userlevel ulev;};

class SimB
	{
	public:
		SimB(void);
		SimB(enum simtype,enum datatype, enum elements,enum fysprocess fproc, enum bioprocess bproc, enum userlevel);
		SimB(enum simtype,enum datatype, enum elements, enum userlevel);
		SimB(enum simtype,enum datatype, enum elements);
		SimB(enum simtype, definitions);
		SimB(enum simtype);
		virtual ~SimB(void);
		void SetEnable(bool value) {_enabled=value;};
		void SetDBEnable(bool value) {_DBenabled=value;};
		void SetModuleNo(size_t value) {_ModuleNo=value;};
		void SetName(string str) {_name=str;};
		void SetConditions(string str) {cond=str;};
		void SetUnit(string str) {_unit=str;};
		//void SetDefValue(double value) { _orgFValue=value;};
		virtual bool IsEnabled() {if(_enabled) return false; return true;};
		bool IsDBEnabled() {return _DBenabled;};
		bool Is_PTable() {if(_stype==PAR_TABLE) return true; return false;};
		bool Is_DB() {if(_stype==DB) return true; return false;};
		bool Is_P() {if(_stype==PAR_SINGLE) return true; return false;};
		bool Is_Vector() {if (int(_stype)<=4) return true; return false;};
		size_t GetType() {return size_t(_stype);};
		string GetTypeName() const;
		int  GetUserLevel() const {return int(_def.ulev);};
		size_t  GetGroupNo() const {return _ModuleNo;};
		virtual bool IsOldValue() {return false;};
		virtual int GetIntValue() {return -1;};
		virtual string GetOption(size_t ) {return "";};
		virtual string GetName() {return _name;};
		virtual void SetDBIntValue(size_t value) {_DBIntValue=value;};
		virtual size_t GetDBIntValue() {return _DBIntValue;}; 
		virtual int MR_Get_Dim() {return -1;};
		virtual int MR_Get_Dim(size_t ) {return -1;};
		virtual MR_METHOD MR_Get_Method() {return STOCHASTIC_LIN_CHANGE;};
		virtual MR_METHOD MR_Get_Method(size_t ) {return STOCHASTIC_LIN_CHANGE;};
		virtual double GetMinValue() {return MISSING;};
		virtual double GetMinValue(size_t ) {return MISSING;};
		virtual double GetWarnMinValue() {return MISSING;};
		virtual double GetWarnMinValue(size_t ) {return MISSING;};
		virtual double GetWarnMaxValue() {return -MISSING;};
		virtual double GetWarnMaxValue(size_t ) {return -MISSING;};
		virtual double GetMaxValue() {return -MISSING;};
		virtual double GetMaxValue(size_t ) {return -MISSING;};
		virtual double GetValue() {return MISSING;};
		virtual double GetValue(size_t ) {return MISSING;};
		virtual double GetDBValue() {return MISSING;};
		virtual double GetDBValue(size_t) {return MISSING;};
		virtual size_t GetPgFileIndex(size_t ) {return 0;};

		virtual size_t GetValidFlagIndex(size_t ) const {return string::npos;};
		virtual size_t GetValidFlagIndex() const {return string::npos;};

		virtual void SetStoreFlag(bool) {};
		virtual void SetMultiStoreFlag(bool) {};
		virtual void SetStoreFlag(size_t , bool) {};
		virtual void SetMultiStoreFlag(size_t, bool) {};
		virtual bool GetStoreFlag(size_t ) {return false;};
		virtual bool GetMultiStoreFlag(size_t ) {return false;};
		virtual size_t GetNumberOfSelectedFlags(bool ) const {return 0;};
		virtual size_t GetNumberOfFlags(bool ) const {return 0;};
		virtual int GetOriginalIntValue() {return _orgIntValue;};
		void SetGroupNo(size_t value) { _ModuleNo = value; };

		virtual double GetOriginalValue() {return MISSING;};
		
		virtual double GetOriginalValue(size_t index) {if(index<_orgFVector.size()) return _orgFVector[index]; return MISSING;};
		size_t GetOriginalSize() {return _orgFVector.size();};
		//int GetGroupCategory() {return ::GetGroupCategory(GetGroup());};
		size_t GetFysProcess() {return _def.fproc;};
		size_t GetBioProcess() {return _def.bproc;};
		string GetFysProcessName() const;
		string GetBioProcessName() const;

		enum elements GetElement() { return _def.elem; };
		enum datatype GetDataType() {return _def.data;};

		string GetDepName(size_t) const;
		virtual string GetUnit() const {return _unit;};		
		string GetDepUnit(size_t) const;	   			// Get name of dependent variables 
		string GetGroup();
		string GetProcessName() const;
		string GetElementName() const;

		enum simtype GetSim() {return _stype;};
		void SetSimType(enum simtype value) {_stype=value;};
		bool	IsTrue(size_t) const;	
		virtual void	Notice();
		virtual void	SetIsOriginalValue(){m_IsNotOriginal=false;};	
		virtual void	SetNotOriginalValue(){m_IsNotOriginal=true;};			// Sets the original value to the current value
		virtual void SetOriginalValue(double ) {;};
		void SetOriginalValue(vector<double> value) {_orgFVector.resize(value.size()); for(size_t i=0; i<value.size();i++) {_orgFVector[i]=value[i];};};

		virtual bool	IsNotOriginalValue() const {return m_IsNotOriginal;};	
		void SetBackOriginalValue() {m_IsNotOriginal=false;};

		string GetCondition(size_t) const;
		void	AddToNoticeList(SimB*); 
		void	BuildDependencies(NewMap*);

		void AddHistoryIndex(size_t value) {m_HistoryIndex[0].push_back(value); };
		void AddHistoryIndex(size_t index, size_t value) { if(index<100) m_HistoryIndex[index].push_back(value);};
		size_t GetHistoryIndex(size_t index) {	if(index<100) return m_HistoryIndex[0][index];	return 0; };
		size_t GetHistoryIndex(size_t index1, size_t index2) {	if(index2<m_HistoryIndex[index1].size()) return m_HistoryIndex[index1][index2]; return 0; };
		size_t GetNumOfHistoryIndex() {	return m_HistoryIndex[0].size();};
		size_t GetNumOfHistoryIndex(size_t index1) { if(index1<100)	return m_HistoryIndex[index1].size(); return 0;};
		void SetReadOnly(bool value) { m_ReadOnly = value; };
		bool IsReadOnly() { return m_ReadOnly; };


		virtual bool SetAnnimateChart(int , int ) {return false;};
		virtual bool SetAnnimateChart(size_t , int , int) {return false;};
		CommonModelInfo* getCommonModelPointer() { return m_pCommonModelInfo; };
		void setCommonModelPointer(CommonModelInfo* p) { m_pCommonModelInfo = p; };
	protected:

		SimB*	GetDepPtr(size_t) const;
		void	SendNotice() const;	
		NewMap	*pfMap; 
		string	_unit;
		size_t _DBIntValue;
		int _orgIntValue;
		CommonModelInfo* m_pCommonModelInfo;
		std::vector<double> _orgFVector;
	private:
		bool  m_IsNotOriginal;
		bool m_ReadOnly;

		bool _enabled, _DBenabled;


		

		string _name;
	
		definitions _def;
		enum simtype _stype;
		size_t _ModuleNo;
		string	cond;	
		vector<Condition> conditions_Array;		// Array of conditions
		vector<SimB*>	noticelist_Array;
		vector<size_t>m_HistoryIndex[100];
		void Decode(string);		 	// Decodes condition string
		SIMB_COMP OpDecode(string,bool*) const;		// Decodes operators
		int	ValDecode(string) const;	  	// Decodes string to integer number
	};

