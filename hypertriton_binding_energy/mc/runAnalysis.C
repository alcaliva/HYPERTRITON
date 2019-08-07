#define ALIPHYSICS_VER  "vAN-20190804-1"
#define GRIDWorkingDir  "HYPERTRITON_MC"
#define AnalysisMacro   "AnalysisHypertriton"
#define AnalysisTask    "AliAnalysisTaskHypertritonBindingEnergyMC"
#define TTL             20000
#define nRunsPerMaster  40


//_____________________________________________________________________________________________________________________________________________________________________
void LoadAnalysisTask (Int_t iMC, AliAnalysisManager *mgr)  {
    
   
    //Analysis Cuts
    Double_t CentralityMin =  0.0;
    Double_t CentralityMax = 90.0;
    Double_t PtPionMin =  0.2;
    Double_t PtPionMax = 1.2;
    Double_t Pt3HeMin =  1.5;
    Double_t Pt3HeMax = 50.0;
    Double_t EtaMax = 0.9;
    Int_t    NumberOfClustersTPCMin = 70;
    Int_t    NumberOfClustersTPCdEdxMin = 50;
    Double_t ChiSquarePerNDFMax = 5.0;
    Bool_t   UseOnTheFlyV0Finder = true;
    Double_t DCAxyPos_Min = 0.0;
    Double_t DCAxyNeg_Min = 0.0;
    Double_t DCAV0daughtersMax = 0.5;
    Double_t ChiSquareV0Max = 50.0;
    Double_t DecayRadiusMin = 3.0;
    Double_t CosPointingAngleMin = 0.95;
    Double_t TPCnsigmaPionMax = 3.0;
    Double_t TPCnsigma3HeMax  = 3.0;
    

    
    //Load Analysis Tasks
    gROOT->LoadMacro("AliAnalysisTaskHypertritonBindingEnergyMC.cxx+g");
    
    //Input Container
    AliAnalysisDataContainer *input = mgr->GetCommonInputContainer();
    
    //Analysis Task (On-The-Fly V0)
    AliAnalysisTaskHypertritonBindingEnergyMC *task0 = new AliAnalysisTaskHypertritonBindingEnergyMC  ("taskHypertriton0");
    task0 -> AliAnalysisTaskHypertritonBindingEnergyMC::SetAnalysisCuts (CentralityMin,CentralityMax,PtPionMin,PtPionMax,Pt3HeMin,Pt3HeMax,EtaMax,NumberOfClustersTPCMin,NumberOfClustersTPCdEdxMin,ChiSquarePerNDFMax,true,DCAxyPos_Min,DCAxyNeg_Min,DCAV0daughtersMax,ChiSquareV0Max,DecayRadiusMin,CosPointingAngleMin,TPCnsigmaPionMax,TPCnsigma3HeMax);
    mgr -> AddTask(task0);
    AliAnalysisDataContainer *output0 = mgr -> CreateContainer("InputOnTheFly",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_Hypertriton_PbPb_OnTheFlyV0_MCproduction%d.root",iMC));
    AliAnalysisDataContainer *outputQA0 = mgr -> CreateContainer("InputQA0",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_QA0_MC%d.root",iMC));
    mgr -> ConnectInput (task0,0,input);
    mgr -> ConnectOutput(task0,1,output0);
    mgr -> ConnectOutput(task0,2,outputQA0);
    
    
    
    //Analysis Task (Offline V0)
    AliAnalysisTaskHypertritonBindingEnergyMC *task1 = new AliAnalysisTaskHypertritonBindingEnergyMC  ("taskHypertriton1");
    task1 -> AliAnalysisTaskHypertritonBindingEnergyMC::SetAnalysisCuts (CentralityMin,CentralityMax,PtPionMin,PtPionMax,Pt3HeMin,Pt3HeMax,EtaMax,NumberOfClustersTPCMin,NumberOfClustersTPCdEdxMin,ChiSquarePerNDFMax,false,DCAxyPos_Min,DCAxyNeg_Min,DCAV0daughtersMax,ChiSquareV0Max,DecayRadiusMin,CosPointingAngleMin,TPCnsigmaPionMax,TPCnsigma3HeMax);
    mgr -> AddTask(task1);
    AliAnalysisDataContainer *output1 = mgr -> CreateContainer("InputOffline",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_Hypertriton_PbPb_OfflineV0_MCproduction%d.root",iMC));
    AliAnalysisDataContainer *outputQA1 = mgr -> CreateContainer("InputQA1",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_QA1_MC%d.root",iMC));
    mgr -> ConnectInput (task1,0,input);
    mgr -> ConnectOutput(task1,1,output1);
    mgr -> ConnectOutput(task1,2,outputQA1);
}
//_____________________________________________________________________________________________________________________________________________________________________
void runAnalysis (Int_t iMC=0, const char *mode="full", Bool_t merge=true)  {
    
    //Load Libraries
    LoadLibraries();
    
    //Grid Connection
    TGrid::Connect("alien://");
    
    //Alien Handler
    AliAnalysisGrid *alienHandler = CreateAlienHandler (iMC,mode,merge);
    if (!alienHandler) return;
    
    //Analysis Manager
    AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisManager");
    mgr->SetGridHandler(alienHandler);
    
    //Physics Selection
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask *physSel = AddTaskPhysicsSelection(true);
    
    //Event Handler & PID
    EventHandler(mgr);
    LoadCentrality();
    LoadPIDResponse();
    
    //Analysis Task
    LoadAnalysisTask (iMC,mgr);
    
    //Start Analysis
    if (!mgr->InitAnalysis())  return;
    mgr->PrintStatus();
    mgr->StartAnalysis("grid");
}
//_____________________________________________________________________________________________________________________________________________________________________
AliAnalysisGrid* CreateAlienHandler (Int_t iMC, const char *mode, bool merge)  {
    
    //Alien Handler
    AliAnalysisAlien *alien = new AliAnalysisAlien();
    SetAdditionalLibraries (alien);
    alien->SetOverwriteMode();
    alien->SetCheckCopy(false);
    alien->SetRunMode(mode);
    alien->SetNtestFiles(10);
    alien->SetAPIVersion("V1.1x");
    alien->SetAliPhysicsVersion(ALIPHYSICS_VER);
    SetPath (iMC,alien);
    alien->SetDataPattern("*AliESDs.root");
    SetInputRuns (alien,mode);
    alien->SetNrunsPerMaster(nRunsPerMaster);
    alien->SetGridWorkingDir (Form("%s_%d",GRIDWorkingDir,iMC));
    alien->SetGridOutputDir("OUTPUT");
    alien->SetAnalysisSource(Form("%s.cxx",AnalysisTask));
    alien->SetAdditionalLibs(Form("%s.cxx %s.h",AnalysisTask,AnalysisTask));
    alien->SetMergeViaJDL(merge);
    alien->SetMaxMergeStages(2);
    alien->SetAnalysisMacro(Form("%s.C",AnalysisMacro));
    alien->SetSplitMaxInputFileNumber(50);
    alien->SetMasterResubmitThreshold(90);
    alien->SetTTL(TTL);
    alien->SetExecutable(Form("%s.sh",AnalysisMacro));
    alien->SetInputFormat("xml-single");
    alien->SetJDLName(Form("%s.jdl",AnalysisMacro));
    alien->SetPrice(1);
    alien->SetSplitMode("se");
    return alien;
}
//_____________________________________________________________________________________________________________________________________________________________________
void EventHandler (AliAnalysisManager *mgr)  {
    
    //ESD Input Handler
    AliESDInputHandler *inputHandler = new AliESDInputHandler();
    mgr->SetInputEventHandler(inputHandler);
    
    //MC Event Handler
    AliMCEventHandler *mcHandler = new AliMCEventHandler();
    mcHandler->SetReadTR(false);
    mgr->SetMCtruthEventHandler(mcHandler);
}
//_____________________________________________________________________________________________________________________________________________________________________
void LoadPIDResponse ()  {
    
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    AliAnalysisTaskPIDResponse *taskPID = AddTaskPIDResponse(true);
}
//_____________________________________________________________________________________________________________________________________________________________________
void LoadCentrality ()  {
    
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
    AliMultSelectionTask *taskMult = AddTaskMultSelection(false);
}
//_____________________________________________________________________________________________________________________________________________________________________
void SetPath (Int_t iMC, AliAnalysisAlien *alien)  {
    
    const char *path_mc_production;
    if (iMC==0) path_mc_production="/alice/sim/2019/LHC19d2a/";//Centrality  0-10% (min. bias)
    if (iMC==1) path_mc_production="/alice/sim/2019/LHC19d2b/";//Centrality 10-50% (min. bias)
    if (iMC==2) path_mc_production="/alice/sim/2019/LHC19d2c/";//Centrality 50-90% (min. bias)
    
    alien->SetGridDataDir(path_mc_production);

}
//_____________________________________________________________________________________________________________________________________________________________________
void SetInputRuns (AliAnalysisAlien *alien, const char *mode)  {
    
    //Test Mode
    if (mode=="test") { alien -> AddRunNumber(295589); return; }
    
    //Run List
    Int_t run[] = { 295589,295610,295611,295612,295615,295665,295666,295667,295668,295671,295673,295675,295676,295677,295712,295714,295717,295718,295719,295720,
                    295721,295723,295725,295753,295754,295755,295756,295758,295759,295762,295763,295786,295788,295791,295816,295818,295819,295822,295825,295826,
                    295829,295831,295853,295854,295855,295856,295859,295860,295861,295881,295908,295909,295910,295913,295936,295937,295941,295942,295943,295945,
                    295947,296016,296060,296061,296062,296063,296065,296066,296068,296074,296123,296132,296133,296134,296135,296142,296143,296191,296192,296194,
                    296195,296196,296197,296198,296240,296241,296242,296243,296244,296246,296247,296269,296270,296273,296279,296280,296303,296304,296307,296309,
                    296312,296375,296376,296377,296378,296379,296380,296381,296383,296414,296415,296419,296420,296423,296424,296433,296472,296509,296510,296511,
                    296512,296514,296516,296547,296548,296549,296550,296551,296552,296553,296594,296615,296616,296618,296619,296621,296622,296623,296690,296691,
                    296693,296694,296749,296750,296752,296781,296784,296785,296786,296787,296790,296793,296794,296799,296835,296836,296838,296839,296848,296849,
                    296850,296851,296852,296890,296894,296899,296900,296903,296930,296931,296932,296934,296935,296938,296941,296966,297029,297031,297035,297085,
                    297117,297118,297119,297123,297124,297128,297129,297132,297133,297193,297194,297195,297196,297218,297219,297221,297222,297278,297310,297311,
                    297312,297315,297317,297332,297333,297335,297336,297363,297366,297367,297372,297379,297380,297405,297406,297413,297414,297415,297441,297442,
                    297446,297450,297451,297452,297479,297481,297483,297512,297537,297540,297541,297542,297544,297558,297588,297590,297595,297624 };

    Int_t nRuns = sizeof(run)/sizeof(Int_t);

    //nRuns=1;
    for ( Int_t i=0; i<nRuns; i++ )
        alien->AddRunNumber(run[i]);
}
//_____________________________________________________________________________________________________________________________________________________________________
void LoadLibraries()
{
    gSystem->Load("libCore.so");
    gSystem->Load("libTree.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libVMC.so");
    gSystem->Load("libPhysics.so");
    gSystem->Load("libMinuit.so");
    gSystem->Load("libGui.so");
    gSystem->Load("libXMLParser.so");
    gSystem->Load("libSTEERBase.so");
    gSystem->Load("libESD.so");
    gSystem->Load("libCDB.so");
    gSystem->Load("libAOD.so");
    gSystem->Load("libANALYSIS.so");
    gSystem->Load("libANALYSISalice.so");
    
    gROOT->ProcessLine(".include $ALICE_PHYSICS/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/ANALYSIS/");
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
    gROOT->ProcessLine(".include $ROOTSYS/include");
    
    gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/JETAN -I$ALICE_PHYSICS/PWG/Tools -g");
}
//_____________________________________________________________________________________________________________________________________________________________________
void SetAdditionalLibraries (AliAnalysisAlien *alien)  {
    
    alien->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/JETAN -I$ALICE_PHYSICS/PWG/Tools -g");
    
    alien->SetAdditionalLibs("AliAnalysisTaskFlowITSTPCTOFQCSP_Template.cxx AliAnalysisTaskFlowITSTPCTOFQCSP_Template.h libPWGHFhfe.so libCDB.so libSTEER.so  libCORRFW.so libPWGflowBase.so libPWGflowTasks.so libGui.so libProof.so libMinuit.so libXMLParser.so libRAWDatabase.so libRAWDatarec.so libCDB.so libSTEERBase.so libSTEER.so libTPCbase.so libTOFbase.so libTOFrec.so libTRDbase.so libVZERObase.so libVZEROrec.so libT0base.so libT0rec.so ");
}
//_____________________________________________________________________________________________________________________________________________________________________
