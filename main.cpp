////
////  main.cpp
////  MSimCoup2
////
////  Created by Asaf Shveki on 07/07/2019.
////
//
#include <iostream>
#include <fstream>
#include <exception>

#include <cstdio>
#include <direct.h>
//#include <unistd.h>
#include <stdio.h>
#include <wchar.h>
#define _countof(array) (sizeof(array) / sizeof(array[0]))

//#include "./stdafx.h"
#include "src/CoupModel/NewBase/Doc.h"
#include "src/CoupModel/Util/FUtil.h"
//#include <windows.h>
//#include<conio.h>

//#include <direct.h>

#include "src/CoupModel/NewBase/SimArchive.h"
#include "src/CoupModel/SoilData/PlotPF/PFCurve.h"
#include "src/CoupModel/Model/FunctionProperties.h"
#include "src/CoupModel/Util/Register.h"

#include "src/CoupModel/SoilData/GetSoilStorage.h"

#include "src/CoupModel/PG/FileImport/PGFileImport.h"

#include <thread>
#define BUFSIZE 4096

#include <sstream>

#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;
#include <boost/tokenizer.hpp>

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

void SimProc(size_t i, Doc *pDoc, bool Multi)
{
    if (Multi) {
        pDoc->CheckAndUpdateFileName(true);
        pDoc->MakeMultiRun();
    }
    else {
        pDoc->CheckAndUpdateFileName(false);
        pDoc->MakeSingleRun();
    }
}

Doc* CreateDoc(size_t i, string str){
    Doc *pDoc;
    pDoc = new Doc();
    pDoc->SetCurrentFileName(str);

    bool makesim;
    makesim = pDoc->ReadDocFile(pDoc->GetCurrentSimFile());

    if(!makesim) {
        cout << "\n";
        cout << "Simulation file not correctly read - Check File !";
        cin >> str;
        return NULL;
    }

    return pDoc;
}


bool MakeMulti(size_t i, Doc *pDoc) {
    string str = "N";
    bool out;
    out = false;
    if (i == 0) {
        if (pDoc->m_MultiRun_Array.size() > 0) {
            cout << "\n";
            if ((str.find("Y") || str.find('y')) != string::npos) out = true;
            pDoc->SetMultiSimulation();
        }
    }
    else if (pDoc->m_MultiRun_Array.size() > 0) {
        Ps *pPs;
        pPs = pDoc->GetPsPointer("Random Seed");
        double v= pPs->GetValue()+i+0.00001;
        pPs->SetValue(v);
        out = true;
        pDoc->SetMultiSimulation();
    }
    return out;
}

size_t MergeAllFiles(vector<Doc*> pDocs) {

    /*    Doc *pMainDoc;
     pMainDoc = pDocs[0];
     bool valid = false;

     pMainDoc->UnLockMultiStorageFile();
     pMainDoc->m_MStorage.Reset();
     pMainDoc->m_MStorage.OpenFile(pMainDoc->GetMBinFileName());
     for (size_t i = 1; i < pDocs.size(); i++) {
     valid=pMainDoc->m_MStorage.AddExistingFile(pDocs[i]->GetMBinFileName());
     }
     pMainDoc->m_MStorage.UpdateAcceptedRuns(true, true);
     pMainDoc->m_MStorage.Reset();
     size_t itotrun=pMainDoc->MR_Get_TotalNumberofRuns();
     size_t ifiles = pMainDoc->m_MStorage.GetNumberOfAddedFiles(pMainDoc->GetMBinFileName());
     size_t koll;
     for (size_t i = 0; i < ifiles; i++) {

     koll=pMainDoc->m_MStorage.GetNumberOfAddedRuns(i);

     }

     pMainDoc->m_MStorage.Save();
     /*    pMainDoc->m_MStorage.Reset();
     pMainDoc->m_MStorage.OpenFile(pMainDoc->GetMBinFileName());
     itotrun = pMainDoc->MR_Get_TotalNumberofRuns();
     ifiles = pMainDoc->m_MStorage.GetNumberOfAddedFiles(pMainDoc->GetMBinFileName());

     MultiStorage *pStorage;
     pStorage = new MultiStorage();
     pStorage->Init(pMainDoc->m_MStorage.m_pNewMap);
     //    pMainDoc->UnLockMultiStorageFile();
     pStorage->m_Lock = false;
     ifiles = pStorage->GetNumberOfAddedFiles(pMainDoc->GetMBinFileName());
     delete pStorage;
     */



    //pDocs[0]->WriteDocFile();

    return pDocs.size() - 1;
}

string handleNewLines(const char* arg) {
    
    string str = "";
    
    int idx = 0;
    while ( arg[idx] != '\0' ){
        if ( (arg[idx]=='\\') && (arg[idx+1]=='n') ){
            str = str + "\n";
            idx+=2;
        }else{
            str = str + arg[idx];
            idx++;
        }
    }
    
    return str;
}

string createInputBinFile(string csvFileName) {

    PGFileImport File;
    string binfilename = "";
    
//    ofstream myfile;
//    myfile.open (fileName);
//    myfile << csvFileContent;
//    myfile.close();

    if(File.ImportFromFile(csvFileName, false, 0)) {

        binfilename=File.GetFileName();

        File.CloseFile();
    }

    return binfilename;
}

string arrayToString(std::vector <float> v) {
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); ++i)
    {
        if(i != 0)
            ss << ",";
        ss << v[i];
    }
    std::string s = ss.str();
    
    return s;
}

void readJson(string jsonFileName, Doc *pDoc) {
    
    std::ifstream i(jsonFileName);
    if (i.good()) {
        json j;
        i >> j;

        cout << "setting simfile parameters:" << endl;

        for(int x=0; x < j["params"].size(); x++)
        {
            cout << j["params"][x]["description"] << endl;

            if (j["params"][x]["type"] == "TABLE") {
                
                Tab* pTabi = dynamic_cast<Tab*>(pDoc->GetPtr(TABLE,j["params"][x]["name"]));
                if (pTabi != NULL) {
                    
                    cout << pTabi->GetTitle(j["params"][x]["var_index"]) << endl;
                    P * pP = (P*)pTabi->GetP_Pointer(j["params"][x]["var_index"]);
                    cout << pP->GetValue(j["params"][x]["value_index"]) << endl;
                    pP->SetValue(j["params"][x]["value_index"], j["params"][x]["value"]);
                    cout << pP->GetValue(j["params"][x]["value_index"]) << endl;
                }
            } else if (j["params"][x]["type"] == "PAR_SINGLE") {
                
                Ps* pP = (Ps*)pDoc->GetPsPointer(j["params"][x]["name"]);
                if (pP != NULL) {
                    cout << pP->GetValue() << endl;
                    pP->SetValue(j["params"][x]["value"]);
                    cout << pP->GetValue() << endl;
                }
                
            } else if (j["params"][x]["type"] == "SWITCH") {
                
                Sw* pSw=dynamic_cast<Sw*>(pDoc->GetPtr(SWITCH,j["params"][x]["name"]));
                cout << pSw->GetIntValue()<< endl;
                pSw->SetIntValue(j["params"][x]["value"]);
                cout << pSw->GetIntValue()<< endl;
            } else if (j["params"][x]["type"] == "RUN_INFO") {
                
                if (j["params"][x]["name"] == "longMinStart") {
                    cout << pDoc->RunOpt.longMinStart << endl;
                    pDoc->RunOpt.longMinStart = PGUtil::MinutConv(j["params"][x]["value"]);
                    cout << pDoc->RunOpt.longMinStart << endl;
                } else if (j["params"][x]["name"] == "longMinEnd") {
                    cout << pDoc->RunOpt.longMinEnd << endl;
                    pDoc->RunOpt.longMinEnd = PGUtil::MinutConv(j["params"][x]["value"]);
                    cout << pDoc->RunOpt.longMinEnd << endl;
                } else if (j["params"][x]["name"] == "longMinPreStart") {
                    cout << pDoc->RunOpt.longMinPreStart << endl;
                    pDoc->RunOpt.longMinPreStart = PGUtil::MinutConv(j["params"][x]["value"]);
                    cout << pDoc->RunOpt.longMinPreStart << endl;
                } else if (j["params"][x]["name"] == "longMinPostEnd") {
                    cout << pDoc->RunOpt.longMinPostEnd << endl;
                    pDoc->RunOpt.longMinPostEnd = PGUtil::MinutConv(j["params"][x]["value"]);
                    cout << pDoc->RunOpt.longMinPostEnd << endl;
                }
            }
        }
    }
    
}




int main(int argc, const char * argv[]) {
       
    /*
     Args:
     0 - run file url
     1 - meteorological file name (XXX.txt)
     2 - sciroot sensor file name (XXX.txt)
     3 - fertilization file name (XXX.txt)
     4 - sim file name
     5 - should calibrate
     */
    
    try
    {
        
        cout << "Starting The Coup Model \n";
        
        // General initializing
        using namespace std;
        using namespace boost;
        char *buf;
        buf = getcwd(NULL, 0);
        string path = buf;
        path = "/tmp/CoupModel/";
        auto start = std::chrono::system_clock::now();
        SimArchive arch;
        CommonModelInfo info;
        GetSoilStorage m_MascurSois;
        PFCurve curve;
        FunctionProperties func;
        Register reg;
        
        // General variables
        string simFileName = argv[4];

        // Converting CSV content to bin file
        string meteofileName = argv[1];
        string scirootSensorfileName = argv[2];
        string fertilizaersFileName = argv[3];
        bool shouldCalibrate = ((string)argv[5] != "0");
        
        string binMeteofilename = createInputBinFile(path + meteofileName);
        string binScirootSensorfilename = createInputBinFile(path + scirootSensorfileName);
        string binFertilizationDatafilename = createInputBinFile(path + fertilizaersFileName);
        
        // Adding path var
        string simFilePath = path + simFileName;
        
        // Setting global OPTIONS
        FUtil::WriteProfileStringStd("WorkingDirectory", path);
        FUtil::WriteProfileStringStd("UserDirectory", path);
        FUtil::WriteProfileStringStd("UserSubDirectory", path);
        FUtil::WriteProfileStringStd("DataBaseDirectory", path);
        FUtil::WriteProfileInt("SimulationRunNo", 1);

        // Creating the DOC file out of the SIM file
        int i = 0;
        Doc *pDoc = CreateDoc(i, simFilePath);
        if(pDoc==NULL) {
            cout << "Something whent wrong when reading your file";
            return 0;
        }
        
        // run calibration
        if (shouldCalibrate) {
            SimProc(i, pDoc, true);
            
            // Keep the normal way the model run
            pDoc->CreateNewDocFromCurrentDoc();
            
            // Save the model in a new name for future runs
            //pDoc->CreateNewDocFromCurrentDoc(simFilePath); Not supported 2020 -pej
        }
        
        readJson("/tmp/CoupModel/par.json", pDoc);
            
        // Run single run
        SimProc(i, pDoc, false);
        
        // Get the output file
        string outputFileName = ((ModelFiles*)pDoc)->GetNewOutputFileName();
        CPG* cpgFile = new CPG();
        cpgFile->Open(outputFileName);

        cout << "All Runs have now been completed \n";
        
        // printing the time took to run the model
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "finished computation at " << std::ctime(&end_time)
        << "elapsed time: " << elapsed_seconds.count() << "s\n";

        // Printing Return value sign, from this part all the data is in JSON format
        cout << "#*#";
        
    //    string outputVariables[7] = { "Acc_MinN_Storage", "AccMinNLeaching", "N Total Plant", "N Tot MinN Soil", "PressureHead", "N Plant AboveG", "AccDenitrification"};
    //    string outputVariablesIDs[7] = { "1", "1", "1", "1", "20- 25 cm 1", "1","1"};
    //
        
        size_t timeinterval = cpgFile->GetNormalTimeInterval();
        string startDate = cpgFile->GetDate(1);

        cpgFile->ExportToFile(path + "LastOutput.csv", false, false);
        
        // Read output parameters from CSV
        string data(path + argv[6]);
        ifstream in(data.c_str());
            if (!in.is_open()) return 1;
        
        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
        vector< string > vec;
        string line;
        int loopIndex = 0;
        
        // Return value
        cout << "{";
        while (getline(in,line))
            {
                Tokenizer tok(line);
                vec.assign(tok.begin(),tok.end());

                // vector now contains strings from one row, output to cout here
    //            copy(vec.begin(), vec.end(), ostream_iterator<string>(cout, "|"));
                
                string currentParameter = vec[0];
                string currentParameterId = vec[1];
                size_t currentIndex = cpgFile->GetVarIndex(currentParameter, currentParameterId) + 1;
                
                if (currentIndex != 0) {
                    
                    if (loopIndex != 0) { // building the JSON
                        cout << ",";
                    }
                    
                    std::vector <float> currentVector = cpgFile->GetVarVector(currentIndex);
                    string parameterValues = arrayToString(currentVector);

        //            Gs* pGsPointer = pDoc->GetGsPointer(currentParameter);
        //            if (pGsPointer != NULL) {
        //                double currentValue = pGsPointer->GetValue();
        //            }
                    
                    cout << "'" + currentParameter + "': {" <<
                        "'startDate':" << "'" << startDate << "'" << "," <<
                        "'timeIntervalMinutes':" << "'" << timeinterval << "'" << "," <<
                        "'values':" << "'" << parameterValues << "'" << "}";
                    
                    loopIndex++;
                }
                
                
                
                
            }
        
        cout << "}";
        
        delete cpgFile;
        delete pDoc;

        
    }
    catch (const std::exception& e) // reference to the base of a polymorphic object
    {
        cout << "#err#";
        std::cout << e.what(); // information from length_error printed
    }
        
    return 0;
}

