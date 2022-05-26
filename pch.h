#pragma once
#define _AFXDLL

#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.web.syndication.h>

#include <winrt/base.h>
#include <winrt/Windows.UI.Composition.h>
#include <stddef.h>
//#include <restclient-cpp/restclient.h>
#include <pugixml.hpp>
//#include <xlnt/xlnt.hpp>
#include <nlohmann/json.hpp>

#include <pqxx/pqxx>

#include <windows.h>
#include "./src/CoupModel/Simulator/DefineModel.h"
#include "./src/CoupModel/NewBase/Doc.h"

#include <direct.h>
#include ".\src\CoupModel\newbase\SimArchive.h"
#include ".\src\CoupModel\soildata\plotpf\PfCurve.h"
#include ".\src\CoupModel\model\FunctionProperties.h"


#include ".\src\CoupModel\SoilData\GetSoilStorage.h"
#include ".\src\CoupModel\PG\PG.H"

#include "./src/CoupModel/Util/CoupModel_DB_Link.hpp"



#include <thread>