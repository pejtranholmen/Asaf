cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)
project(PostgresXmlDB LANGUAGES CXX)

configure_file(CoupConfig.h.in CoupConfig.h)
add_compile_definitions(NO_MS_GUI COUPSTD MS_CODE)


list(APPEND EXTRA_LIB_DIRS src/coupmodel/ModelTypes
						   src/coupmodel/NewBase
						   src/coupmodel/PG
						   src/coupmodel/Model
						   src/coupmodel/Util
						   src/coupmodel/Simulator
)
list(APPEND EXTRA_INCLUDE_DIRS 
	src/coupmodel/PG
	src/coupmodel/ModelTypes 
	src/coupmodel/NewBase
	src/coupmodel/Model
	src/coupmodel/Util
	src/coupmodel/Simulator
)


#add_subdirectory(src/CoupModel/PG)
#add_subdirectory(src/coupmodel/modeltypes)
#add_subdirectory(src/coupmodel/NewBase)
#add_subdirectory(src/coupmodel/Model)
#add_subdirectory(src/coupmodel/Util)
#add_subdirectory(src/coupmodel/Simulator)



# Use shared libraries
set(BUILD_SHARED_LIBS ON CACHE STRING "Link to shared libraries by default.")


#find_package(restclient-cpp CONFIG REQUIRED)
#find_package(pugixml CONFIG REQUIRED)
#find_package(nlohmann_json CONFIG REQUIRED)
#find_package(libpqxx CONFIG REQUIRED)
#find_package(xlnt CONFIG REQUIRED)


add_executable(${PROJECT_NAME} 
		main.cpp
        src/coupmodel/PG/PG.cpp 
		src/coupmodel/PG/PG.h
		src/coupmodel/Simulator/ModelInfo.cpp
		src/coupmodel/Simulator/ModelInfo.h
		src/coupmodel/newbase/CommonModelInfo.cpp
		src/coupmodel/newbase/CommonModelInfo.h
        src/coupmodel/Util/FUtil.hpp
        src/coupmodel/Util/Constants.cpp
        src/coupmodel/Util/Constants.h
        src/coupmodel/Util/ModelUtil.cpp
        src/coupmodel/Util/ModelUtil.h
        src/coupmodel/Util/ReadWriteTypes.cpp
        src/coupmodel/Util/ReadWriteTypes.h
        src/coupmodel/Util/StatUtil.cpp
        src/coupmodel/Util/StatUtil.h
        src/coupmodel/Util/MySingle.cpp
        src/coupmodel/Util/MySingle.h
        src/coupmodel/util/CoupModel_Db_Link.hpp
       
        src/CoupModel/DB/DB_Storage.cpp
        src/CoupModel/DB/DB_Storage.h
        src/CoupModel/DB/GetDB.cpp
        src/CoupModel/DB/GetDB.h
        src/CoupModel/SoilData/PlotPF/GausKreuger.cpp
        src/CoupModel/SoilData/PlotPF/GausKreuger.h
        src/CoupModel/SoilData/PlotPF/Geodesi.cpp
        src/CoupModel/SoilData/PlotPF/GetPFCurve.cpp
        src/CoupModel/SoilData/PlotPF/GetPFCurve.h
        src/CoupModel/SoilData/PlotPF/PFCurve.cpp
        src/CoupModel/SoilData/PlotPF/PFCurve.h
        src/CoupModel/SoilData/PlotPF/PFStorage.cpp
        src/CoupModel/SoilData/PlotPF/PFStorage.h
        src/CoupModel/SoilData/GetSoilStorage.cpp
        src/CoupModel/SoilData/GetSoilStorage.h
        src/CoupModel/SoilData/SoilStorage.cpp
        src/CoupModel/SoilData/SoilStorage.h
        src/CoupModel/MultiRun/MultiStorageData.cpp
        src/CoupModel/MultiRun/MultiStorageData.h
        src/CoupModel/MultiRun/MultiStorageStatData.cpp
        src/CoupModel/MultiRun/MultiStorageStatData.h
        src/CoupModel/MultiRun/Residuals.cpp
        src/CoupModel/MultiRun/Residuals.h
        src/CoupModel/MultiRun/ResidualsBase.cpp
        src/CoupModel/MultiRun/ResidualsBase.h
        src/CoupModel/MultiRun/SelectedAddEnsembles.cpp
        src/CoupModel/MultiRun/SelectedAddEnsembles.h
        src/CoupModel/MultiRun/SelectedEnsembles.cpp
        src/CoupModel/MultiRun/SelectedEnsembles.h

        src/coupmodel/ModelTypes/CDB.cpp
        src/coupmodel/ModelTypes/CDB.h
        src/coupmodel/ModelTypes/CRunInfo.cpp
        src/coupmodel/ModelTypes/D.cpp
        src/coupmodel/ModelTypes/D.h
        src/coupmodel/ModelTypes/Ds.cpp
        src/coupmodel/ModelTypes/Ds.h
        src/coupmodel/ModelTypes/F.cpp
        src/coupmodel/ModelTypes/F.h
        src/coupmodel/ModelTypes/FCSV.cpp
        src/coupmodel/ModelTypes/FCSV.h
        src/coupmodel/ModelTypes/Func.cpp
        src/coupmodel/ModelTypes/Func.h
        src/coupmodel/ModelTypes/G.cpp
        src/coupmodel/ModelTypes/G.h
        src/coupmodel/ModelTypes/Gs.cpp
        src/coupmodel/ModelTypes/Gs.h
        src/coupmodel/ModelTypes/NE.cpp
        src/coupmodel/ModelTypes/NE.h
        src/coupmodel/ModelTypes/OutBase.cpp
        src/coupmodel/ModelTypes/OutBase.h
        src/coupmodel/ModelTypes/OutSingle.cpp
        src/coupmodel/ModelTypes/OutSingle.h
        src/coupmodel/ModelTypes/OutVector.cpp
        src/coupmodel/ModelTypes/OutVector.h
        src/coupmodel/ModelTypes/P.cpp
        src/coupmodel/ModelTypes/P.h
        src/coupmodel/ModelTypes/Par.cpp
        src/coupmodel/ModelTypes/Par.h
        src/coupmodel/ModelTypes/Ps.cpp
        src/coupmodel/ModelTypes/Ps.h
        src/coupmodel/ModelTypes/RunInfo.h
        src/coupmodel/ModelTypes/SimB.cpp
        src/coupmodel/ModelTypes/SimB.h
        src/coupmodel/ModelTypes/Sw.cpp
        src/coupmodel/ModelTypes/Sw.h
        src/coupmodel/ModelTypes/T.cpp
        src/coupmodel/ModelTypes/T.h
        src/coupmodel/ModelTypes/Tab.cpp
        src/coupmodel/ModelTypes/Tab.h
        src/coupmodel/ModelTypes/Ts.cpp
        src/coupmodel/ModelTypes/Ts.h
        src/coupmodel/ModelTypes/X.cpp
        src/coupmodel/ModelTypes/X.h
        src/coupmodel/ModelTypes/Xs.cpp
        src/coupmodel/ModelTypes/Xs.h

        src/CoupModel/Model/FunctFactory.cpp
        src/CoupModel/Model/FunctFactory.h

        src/coupmodel/model/Atmosphere/Functors/IrrigationWater_Functors.h
        src/coupmodel/model/Atmosphere/Functors/Met_Functors.h
        src/coupmodel/model/Atmosphere/Functors/Radiation_Functors.h
        src/coupmodel/model/Atmosphere/Functors/SnowPack_Functors.h
        src/coupmodel/model/Atmosphere/Functors/SoilEvaporation_Functors.h
        src/coupmodel/model/Atmosphere/Functors/SurfaceWater_Functors.h
        src/coupmodel/model/Atmosphere/AtmModel.cpp
        src/coupmodel/model/Atmosphere/AtmModel.h
        src/coupmodel/model/Atmosphere/Irrigation.cpp
        src/coupmodel/model/Atmosphere/Irrigation.h
        src/coupmodel/model/Atmosphere/Irrigation_Functions.cpp
        src/coupmodel/model/Atmosphere/Irrigation_Functions.h
        src/coupmodel/model/Atmosphere/Meteorological_Data.cpp
        src/coupmodel/model/Atmosphere/Meteorological_Data.h
        src/coupmodel/model/Atmosphere/Meteorological_Data_Functions.cpp
        src/coupmodel/model/Atmosphere/Meteorological_Data_Functions.h
        src/coupmodel/model/Atmosphere/Radiation.cpp
        src/coupmodel/model/Atmosphere/Radiation.h
        src/coupmodel/model/Atmosphere/Radiation_Functions.cpp
        src/coupmodel/model/Atmosphere/Radiation_Functions.h
        src/coupmodel/model/Atmosphere/SnowPack.cpp
        src/coupmodel/model/Atmosphere/SnowPack.h
        src/coupmodel/model/Atmosphere/SnowPack_Functions.cpp
        src/coupmodel/model/Atmosphere/SnowPack_Functions.h
        src/coupmodel/model/Atmosphere/SoilEvaporation.cpp
        src/coupmodel/model/Atmosphere/SoilEvaporation.h
        src/coupmodel/model/Atmosphere/SoilEvaporation_Functions.cpp
        src/coupmodel/model/Atmosphere/SoilEvaporation_Functions.h
        src/coupmodel/model/Atmosphere/Surface_Water.cpp
        src/coupmodel/model/Atmosphere/Surface_Water.h
        src/coupmodel/model/Atmosphere/Surface_Water_Functions.cpp
        src/coupmodel/model/Atmosphere/Surface_Water_Functions.h
        src/coupmodel/model/HBV/Functor/HBV_Functor.h
        src/coupmodel/model/HBV/HBV.cpp
        src/coupmodel/model/HBV/HBV.h
        src/coupmodel/model/HBV/HBV_Functions.cpp
        src/coupmodel/model/HBV/HBV_Functions.h
        src/coupmodel/model/HBV/HBVModel.cpp
        src/coupmodel/model/HBV/HBVModel.h
        src/coupmodel/model/PLANT/Functors/InterCeption_Functors.h
        src/coupmodel/model/PLANT/Functors/Plant_Functors.h
        src/coupmodel/model/PLANT/Functors/PotTranspiration_Functors.h
        src/coupmodel/model/PLANT/Functors/Salt_Functors.h
        src/coupmodel/model/PLANT/Functors/WaterUptake_Functors.h
        src/coupmodel/model/PLANT/Interception.cpp
        src/coupmodel/model/PLANT/Interception.h
        src/coupmodel/model/PLANT/Plant.cpp
        src/coupmodel/model/PLANT/Plant.h
        src/coupmodel/model/PLANT/PlantModel.cpp
        src/coupmodel/model/PLANT/PlantModel.h
        src/coupmodel/model/PLANT/Potential_Transpiration.cpp
        src/coupmodel/model/PLANT/Potential_Transpiration.h
        src/coupmodel/model/PLANT/Salt.cpp
        src/coupmodel/model/PLANT/Salt.h
        src/coupmodel/model/PLANT/Salt_Functions.cpp
        src/coupmodel/model/PLANT/Salt_Functions.h
        src/coupmodel/model/PLANT/WaterUptake.cpp
        src/coupmodel/model/PLANT/WaterUptake.h
        src/coupmodel/model/PLANT_NC/Functors/Allocation/AlloLeafMass.h
        src/coupmodel/model/PLANT_NC/Functors/Allocation/AlloRootMass.h
        src/coupmodel/model/PLANT_NC/Functors/Allocation/AlloTemplate.h
        src/coupmodel/model/PLANT_NC/Functors/BiomassDescription/BioMass.h
        src/coupmodel/model/PLANT_NC/Functors/GrowthStages/GrowthStages.h
        src/coupmodel/model/PLANT_NC/Functors/LitterFall/LitterTemplate.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/GPP_Drivers.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/LogisticGrowth.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/PhotoGreenIndexResponse.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/PhotoNLeafResponse.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/PhotoPLeafResponse.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/PhotoRadEff.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/PhotoTemp.h
        src/coupmodel/model/PLANT_NC/Functors/PhotoSynthesis/PhotoTempResponseDirect.h
        src/coupmodel/model/PLANT_NC/Functors/Respiration/RespirationTemplate.h
        src/coupmodel/model/PLANT_NC/Functors/UptakeFromSoil/PlantUptake.h
        src/coupmodel/model/PLANT_NC/Functors/NC_Plant_Functors.h
        src/coupmodel/model/PLANT_NC/NC_Drivings.cpp
        src/coupmodel/model/PLANT_NC/NC_Drivings.h
        src/coupmodel/model/PLANT_NC/NC_Drivings_Functions.cpp
        src/coupmodel/model/PLANT_NC/NC_Drivings_Functions.h
        src/coupmodel/model/PLANT_NC/NC_Plant.cpp
        src/coupmodel/model/PLANT_NC/NC_Plant.h
        src/coupmodel/model/PLANT_NC/NC_Plant_Functions.cpp
        src/coupmodel/model/PLANT_NC/NC_Plant_Functions.h
        src/coupmodel/model/PLANT_NC/PlantOrgModel.cpp
        src/coupmodel/model/PLANT_NC/PlantOrgModel.h
        src/coupmodel/model/SOIL/Functors/Drainage_Functor.h
        src/coupmodel/model/SOIL/Functors/HydFunctors.h
        src/coupmodel/model/SOIL/Functors/Profile_Functors.h
        src/coupmodel/model/SOIL/Functors/Thermal_Functors.h
        src/coupmodel/model/SOIL/Hydraulic.cpp
        src/coupmodel/model/SOIL/Hydraulic.h
        src/coupmodel/model/SOIL/Hydraulic_Functions.cpp
        src/coupmodel/model/SOIL/Hydraulic_Functions.h
        src/coupmodel/model/SOIL/PedoFunctions.h
        src/coupmodel/model/SOIL/Profile.cpp
        src/coupmodel/model/SOIL/Profile.h
        src/coupmodel/model/SOIL/Profile_Functions.cpp
        src/coupmodel/model/SOIL/Profile_Functions.h
        src/coupmodel/model/SOIL/Soil_Drainage.cpp
        src/coupmodel/model/SOIL/Soil_Drainage.h
        src/coupmodel/model/SOIL/Soil_Drainage_Functions.cpp
        src/coupmodel/model/SOIL/Soil_Drainage_Functions.h
        src/coupmodel/model/SOIL/Soil_Frost.cpp
        src/coupmodel/model/SOIL/Soil_Frost.h
        src/coupmodel/model/SOIL/Soil_Frost_Functions.cpp
        src/coupmodel/model/SOIL/Soil_Frost_Functions.h
        src/coupmodel/model/SOIL/Soil_HeatF.cpp
        src/coupmodel/model/SOIL/Soil_HeatF.h
        src/coupmodel/model/SOIL/Soil_HeatF_Functions.cpp
        src/coupmodel/model/SOIL/Soil_HeatF_Functions.h
        src/coupmodel/model/SOIL/Soil_HeatPump.cpp
        src/coupmodel/model/SOIL/Soil_HeatPump.h
        src/coupmodel/model/SOIL/Soil_HeatPump_Functions.cpp
        src/coupmodel/model/SOIL/Soil_HeatPump_Functions.h
        src/coupmodel/model/SOIL/Soil_WaterF.cpp
        src/coupmodel/model/SOIL/Soil_WaterF.h
        src/coupmodel/model/SOIL/Soil_WaterF_Functions.cpp
        src/coupmodel/model/SOIL/Soil_WaterF_Functions.h
        src/coupmodel/model/SOIL/SoilModel.cpp
        src/coupmodel/model/SOIL/SoilModel.h
        src/coupmodel/model/SOIL/Thermal.cpp
        src/coupmodel/model/SOIL/Thermal.h
        src/coupmodel/model/SOIL/Thermal_Functions.cpp
        src/coupmodel/model/SOIL/Thermal_Functions.h
        src/coupmodel/model/SOIL/ThermalCoef.cpp
        src/coupmodel/model/SOIL/ThermalCoef.h
        src/coupmodel/model/SOIL_NC/Functors/BasicGas_Functors.h
        src/coupmodel/model/SOIL_NC/Functors/Methane_Functors.h
        src/coupmodel/model/SOIL_NC/Functors/NC_GasFluxFunctors.h
        src/coupmodel/model/SOIL_NC/Functors/NOxide_Functors.h
        src/coupmodel/model/SOIL_NC/Functors/NC_ExternalInputs_Functors.h
        src/coupmodel/model/SOIL_NC/Functors/NC_Minerals_Functors.h
        src/coupmodel/model/SOIL_NC/Functors/NC_Organic_Functors.h
        src/coupmodel/model/SOIL_NC/Functors/NC_Soil_Functors.h
        src/coupmodel/model/SOIL_NC/Functors/P_Minerals_Functors.h
        src/coupmodel/model/SOIL_NC/AbioticResponses.cpp
        src/coupmodel/model/SOIL_NC/AbioticResponses.h
        src/coupmodel/model/SOIL_NC/NC_ExternalInputs.cpp
        src/coupmodel/model/SOIL_NC/NC_ExternalInputs.h
        src/coupmodel/model/SOIL_NC/NC_ExternalInputs_Functions.cpp
        src/coupmodel/model/SOIL_NC/NC_ExternalInputs_Functions.h
        src/coupmodel/model/SOIL_NC/NC_GasFlux.cpp
        src/coupmodel/model/SOIL_NC/NC_GasFlux.h
        src/coupmodel/model/SOIL_NC/NC_GasFlux_Functions.cpp
        src/coupmodel/model/SOIL_NC/NC_GasFlux_Functions.h
        src/coupmodel/model/SOIL_NC/NC_Mineral.cpp
        src/coupmodel/model/SOIL_NC/NC_Mineral.h
        src/coupmodel/model/SOIL_NC/NC_Mineral_Functions.cpp
        src/coupmodel/model/SOIL_NC/NC_Mineral_Functions.h
        src/coupmodel/model/SOIL_NC/NC_Organic.cpp
        src/coupmodel/model/SOIL_NC/NC_Organic.h
        src/coupmodel/model/SOIL_NC/NC_Organic_Functions.cpp
        src/coupmodel/model/SOIL_NC/NC_Organic_Functions.h
        src/coupmodel/model/SOIL_NC/NC_SoilManagement.cpp
        src/coupmodel/model/SOIL_NC/NC_SoilManagement.h
        src/coupmodel/model/SOIL_NC/NC_SoilManagement_Functions.cpp
        src/coupmodel/model/SOIL_NC/NC_SoilManagement_Functions.h
        src/coupmodel/model/SOIL_NC/P_ExternalInputs.cpp
        src/coupmodel/model/SOIL_NC/P_ExternalInputs.h
        src/coupmodel/model/SOIL_NC/P_ExternalInputs_Functions.cpp
        src/coupmodel/model/SOIL_NC/P_ExternalInputs_Functions.h
        src/coupmodel/model/SOIL_NC/P_Mineral.cpp
        src/coupmodel/model/SOIL_NC/P_Mineral.h
        src/coupmodel/model/SOIL_NC/P_Mineral_Functions.cpp
        src/coupmodel/model/SOIL_NC/P_Mineral_Functions.h
        src/coupmodel/model/SOIL_NC/SoilOrgModel.cpp
        src/coupmodel/model/SOIL_NC/SoilOrgModel.h
        src/coupmodel/model/Structure/Functors/Structure_Functors.h
        src/coupmodel/model/Structure/Functors/Technical_Functors.h
        src/coupmodel/model/Structure/Additional_BioticVariables.cpp
        src/coupmodel/model/Structure/Additional_BioticVariables.h
        src/coupmodel/model/Structure/Additional_BioticVariables_Functions.cpp
        src/coupmodel/model/Structure/Additional_BioticVariables_Functions.h
        src/coupmodel/model/Structure/Additional_Variables.cpp
        src/coupmodel/model/Structure/Additional_Variables.h
        src/coupmodel/model/Structure/Additional_Variables_Functions.cpp
        src/coupmodel/model/Structure/Additional_Variables_Functions.h
        src/coupmodel/model/Structure/Numerical.cpp
        src/coupmodel/model/Structure/Numerical.h
        src/coupmodel/model/Structure/Numerical_Functions.cpp
        src/coupmodel/model/Structure/Numerical_Functions.h
        src/coupmodel/model/Structure/Structure.cpp
        src/coupmodel/model/Structure/Structure.h
        src/coupmodel/model/Structure/Structure_Functions.cpp
        src/coupmodel/model/Structure/Structure_Functions.h
        src/coupmodel/model/Structure/StructureModel.cpp
        src/coupmodel/model/Structure/StructureModel.h
        src/coupmodel/model/Structure/Technical.cpp
        src/coupmodel/model/Structure/Technical.h
        src/coupmodel/model/Structure/Technical_Functions.cpp
        src/coupmodel/model/Structure/Technical_Functions.h
        src/coupmodel/model/FunctionProperties.cpp
        src/coupmodel/model/FunctionProperties.h
        src/coupmodel/model/Basic_Functors.h
        src/coupmodel/model/GlobalInfo.cpp
        src/coupmodel/model/GlobalInfo.h
        src/coupmodel/model/ModelFunctions.cpp
        src/coupmodel/model/ModelFunctions.h
        src/coupmodel/model/PhysConst.cpp
        src/coupmodel/model/PhysConst.h
        src/coupmodel/model/SimModule.cpp
        src/coupmodel/model/SimModule.h

        src/coupmodel/NewBase/Bayes/BayesStatistics.cpp
        src/coupmodel/NewBase/Bayes/BayesStatistics.h
        src/coupmodel/NewBase/ParValueChangeRead/ChangeFromFile.cpp
        src/coupmodel/NewBase/ParValueChangeRead/ChangeFromFile.h
        src/coupmodel/NewBase/CH.cpp
        src/coupmodel/NewBase/CH.h
        src/coupmodel/NewBase/CommonModelInfo.cpp
        src/coupmodel/NewBase/CommonModelInfo.h
        src/coupmodel/NewBase/Doc.cpp
        src/coupmodel/NewBase/Doc.h
        src/coupmodel/NewBase/DB.cpp
        src/coupmodel/NewBase/DB.h
        src/coupmodel/NewBase/History.cpp
        src/coupmodel/NewBase/History.h
        src/coupmodel/NewBase/ModelBase.cpp
        src/coupmodel/NewBase/ModelBase.h
        src/coupmodel/NewBase/ModelFiles.cpp
        src/coupmodel/NewBase/ModelFiles.h
        src/coupmodel/NewBase/ModelMap.cpp
        src/coupmodel/NewBase/ModelMap.h
        src/coupmodel/NewBase/MR.cpp
        src/coupmodel/NewBase/MR.h
        src/coupmodel/NewBase/NewMap.cpp
        src/coupmodel/NewBase/Sim.cpp
        src/coupmodel/NewBase/Sim.h
        src/coupmodel/NewBase/SimArchive.cpp
        src/coupmodel/NewBase/SimArchive.h
        src/coupmodel/PG/NoaaCoord.cpp
        src/coupmodel/PG/NoaaCoord.h
        src/coupmodel/PG/PG.CPP
        src/coupmodel/PG/PG.H
        src/coupmodel/PG/PG_MEMORY.cpp
        src/coupmodel/PG/PG_MEMORY.h
        src/coupmodel/PG/PG_Stream.cpp
        src/coupmodel/PG/PG_Stream.h
        src/coupmodel/PG/PGBase.cpp
        src/coupmodel/PG/PGBase.h
        src/coupmodel/PG/PGFile.cpp
        src/coupmodel/PG/PGFile.h
        src/coupmodel/PG/PGUtil.cpp
        src/coupmodel/PG/PGUtil.h
        src/coupmodel/PG/PGView.cpp
        src/coupmodel/PG/PGView.h
        src/coupmodel/Simulator/DefineModel.cpp
        src/coupmodel/Simulator/DefineModel.h
        src/coupmodel/Simulator/ModelConst.cpp
        src/coupmodel/Simulator/ModelConst.h
        src/coupmodel/Simulator/ModelInfo.cpp
        src/coupmodel/Simulator/ModelInfo.h
        src/coupmodel/Simulator/Simulator.cpp
        src/coupmodel/Simulator/Simulator.h
        src/coupmodel/SoilData/PlotPF/GausKreuger.cpp
        src/coupmodel/SoilData/PlotPF/GausKreuger.h
        src/coupmodel/SoilData/PlotPF/Geodesi.cpp
        src/coupmodel/SoilData/PlotPF/GetPFCurve.cpp
        src/coupmodel/SoilData/PlotPF/GetPFCurve.h
        src/coupmodel/SoilData/PlotPF/PFCurve.cpp
        src/coupmodel/SoilData/PlotPF/PFCurve.h
        src/coupmodel/SoilData/PlotPF/PFStorage.cpp
        src/coupmodel/SoilData/PlotPF/PFStorage.h
        src/coupmodel/SoilData/GetSoilStorage.cpp
        src/coupmodel/SoilData/GetSoilStorage.h
        src/coupmodel/SoilData/SoilStorage.cpp
        src/coupmodel/SoilData/SoilStorage.h


        src/CoupModel/Validation/SmartFlexDataAdmin.cpp
        src/CoupModel/Validation/SmartFlexDataAdmin.h
        src/CoupModel/Validation/SmartFlexibleValidation.cpp
        src/CoupModel/Validation/SmartFlexibleValidation.h
        src/CoupModel/Validation/ValidationData.cpp
        src/CoupModel/Validation/ValidationData.h

        src/CoupModel/mcmc/Density.cpp
        src/CoupModel/mcmc/Density.h
        src/CoupModel/mcmc/intervals.cpp
        src/CoupModel/mcmc/intervals.h
        src/CoupModel/mcmc/lot.cpp
        src/CoupModel/mcmc/lot.h
        src/CoupModel/mcmc/ratio.cpp
        src/CoupModel/mcmc/ratio.h
        src/CoupModel/mcmc/statistics.cpp
        src/CoupModel/mcmc/statistics.h
        src/CoupModel/mcmc/util.cpp
        src/CoupModel/mcmc/util.h)


 #"src/CoupModel/Util/MultiStorage_DB_Link.hpp" "src/CoupModel/Util/DB_Doc.hpp" "pch.h") #add app's main sourcefile



target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}  
#	PRIVATE pugixml::shared
#	PRIVATE xlnt::xlnt
#	PRIVATE	nlohmann_json::nlohmann_json
#    PRIVATE libpqxx::pqxx
)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES DESTINATION include)


