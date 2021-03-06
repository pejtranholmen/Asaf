#pragma once
#include "../../../std.h"
enum class Methane_Model_Sw { off, on };
enum class Methane_Plant_Oxidation_Sw { off, on };
enum class Methane_Soil_Oxidation_Sw { off, on_orignal, New };
enum class Methane_Plant_Emission_Sw { off, on };
enum class N2O_YearlyFluxes_Sw { off, on };


class Ebullition {
public:
	Ebullition (double ratecoef, double thresholdconc) noexcept:
	ratecoef(ratecoef), Threshold(thresholdconc) {}

	double operator() (double waterconc) {
		return RateCoefFunc(waterconc);
	};
	double operator() (double waterconc, double wateramount) {
		return RateCoefFunc(waterconc) * wateramount;
	};


private:
	double RateCoefFunc(double waterconc) {
		return ratecoef * max(waterconc - Threshold, 0.);
	};
	double ratecoef, Threshold;
};
class HenryLawMethaneVolatility {
public:
	HenryLawMethaneVolatility(double Henry_cp, double HenryCoefTemp, double RefTemp ) noexcept 	:
	cp(Henry_cp), ctemp(HenryCoefTemp), reftemp(RefTemp)
	{		// Converts from mass/mass to mass per volume assuming the denisity of water to 10**6 g/m3
			//	changing the unit from g/m3 to g/liter 2008-10-10
		cp = Henry_cp *1000.; 
	}
	double operator() (double WaterConc, double Temp) noexcept {
		return WaterConc / (cp * exp(-ctemp * (1 / (Temp + 273.15) - 1 / (reftemp + 273.15))) * (Temp + 273.15) / 12.2);
	};
																															

private:
	double cp, ctemp, reftemp;

};
class MethaneBoundaryLayerFlux {
public:
	MethaneBoundaryLayerFlux(double ref_conc, double ResistanceAir) noexcept : ref_conc(ref_conc), ra(ResistanceAir) { ra /= Fix::SECPERDAY;};

	double operator() (double ConcSurface) noexcept {
		return ( ConcSurface- ref_conc) / ra;
	};
	double operator() (double ConcSurface, double TempSurface) {
		double MethaneMassConcAtm = ref_conc * Fix::MOL_RATIO_CH4_AIR * AirDensityTempFunc(TempSurface);
		double flux= (ConcSurface - MethaneMassConcAtm) / ra;
		if (TempSurface < 0.)
			return max(0., flux);
		else
			return flux;
	};



private:
	double ra, ref_conc;

};
class MethaneProduction {
public:
	MethaneProduction(double CH4_RateCoef, double RateCoefLitter1, double RateCoefLitter2, double RateCoefHumus) noexcept
	{
		clitter1 = CH4_RateCoef * RateCoefLitter1; clitter2 = CH4_RateCoef * RateCoefLitter2; chumus = CH4_RateCoef * RateCoefHumus;
	}
	double operator() (double Litter1, double Litter2, double Humus) {
		return clitter1 * Litter1 + clitter2 * Litter2 + chumus * Humus;
	}
	double operator() (double Litter1, double Humus) {
		return clitter1 * Litter1 + chumus * Humus;
	}


private:
	double clitter1, clitter2, chumus;

};
class MethaneEmissionByTranspiration {
public:
	MethaneEmissionByTranspiration(double CH4_LimitWaterUptake, double CH4_WaterPowerCoef, double CH4_WaterPlantCoef) :
		fluxthreshold(CH4_LimitWaterUptake), powercoef(CH4_WaterPowerCoef), plantcoef(CH4_WaterPlantCoef) {	}

	double operator() (double WaterUptakeRate, double Methane_Conc) {
		return WaterUptakeRate * Methane_Conc * PlantEffFunc(WaterUptakeRate);
	}
	double operator() (double WaterUptakeRate) {
		return PlantEffFunc(WaterUptakeRate);
	}

private:
	double PlantEffFunc(double UptakeRate) {
		double out;
		if (UptakeRate <= fluxthreshold)
			out = 1;
		else
			out = pow(fluxthreshold / UptakeRate, powercoef);
		return plantcoef * out;
	}

	double fluxthreshold, powercoef, plantcoef;

};
class MethanePlantOxidation {
public:
	MethanePlantOxidation(double CH4_PlantOxidationRate, double CH4_PlantOxChapeCoef) noexcept : coef(CH4_PlantOxidationRate) {
		f_RootRespRegulation = ExpSquaredDecline_Response(CH4_PlantOxChapeCoef);
	};
	double operator() (double RespirationRate) {
		return coef * f_RootRespRegulation(RespirationRate);
	};


private:
	function <double(double)> f_RootRespRegulation;
	double coef;

};

