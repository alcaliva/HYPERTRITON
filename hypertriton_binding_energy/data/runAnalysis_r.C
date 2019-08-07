#define ALIPHYSICS_VER  "vAN-20190805-1"
#define GRIDWorkingDir  "REDUCED_TREE_HYPERTRITON_PbPb_LHC18r"
#define AnalysisMacro   "AnalysisReducedTree"
#define AnalysisTask    "AliAnalysisTaskReducedTreeHypertriton"
#define TTL             30000
#define nRunsPerMaster  1


//______________________________________________________________________________________________________________________________________________________________________________________
void LoadAnalysisTask (Int_t iChunk, AliAnalysisManager *mgr)  {
    
    //Load Analysis Tasks
    gROOT->LoadMacro("AliAnalysisTaskReducedTreeHypertriton.cxx+g");
    
    //Input container
    AliAnalysisDataContainer *input = mgr -> GetCommonInputContainer();
    
    //Analysis Task: Minimum Bias
    AliAnalysisTaskReducedTreeHypertriton *task_kINT7 = new AliAnalysisTaskReducedTreeHypertriton  ("task_kINT7");
    task_kINT7 -> SelectCollisionCandidates (AliVEvent::kINT7);
    task_kINT7 -> SetCentrality (0.0,90.0);
    mgr -> AddTask(task_kINT7);
    AliAnalysisDataContainer *output_kINT7 = mgr -> CreateContainer("Input_kINT7",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_ReducedTree_Hypertriton_PbPb_LHC18r_kINT7%d.root",iChunk));
    mgr -> ConnectInput  (task_kINT7,0,input);
    mgr -> ConnectOutput (task_kINT7,1,output_kINT7);
    
    //Analysis Task: Central Collision
    AliAnalysisTaskReducedTreeHypertriton *task_kCentral = new AliAnalysisTaskReducedTreeHypertriton  ("task_kCentral");
    task_kCentral -> SelectCollisionCandidates (AliVEvent::kCentral);
    task_kCentral -> SetCentrality (0.0,10.0);
    mgr -> AddTask(task_kCentral);
    AliAnalysisDataContainer *output_kCentral = mgr -> CreateContainer("Input_kCentral",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_ReducedTree_Hypertriton_PbPb_LHC18r_kCentral%d.root",iChunk));
    mgr -> ConnectInput  (task_kCentral,0,input);
    mgr -> ConnectOutput (task_kCentral,1,output_kCentral);
    
    //Analysis Task: SemiCentral Collision
    AliAnalysisTaskReducedTreeHypertriton *task_kSemiCentral = new AliAnalysisTaskReducedTreeHypertriton  ("task_kSemiCentral");
    task_kSemiCentral -> SelectCollisionCandidates (AliVEvent::kSemiCentral);
    task_kSemiCentral -> SetCentrality (30.0,50.0);
    mgr -> AddTask(task_kSemiCentral);
    AliAnalysisDataContainer *output_kSemiCentral = mgr -> CreateContainer("Input_kSemiCentral",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_ReducedTree_Hypertriton_PbPb_LHC18r_kSemiCentral%d.root",iChunk));
    mgr -> ConnectInput  (task_kSemiCentral,0,input);
    mgr -> ConnectOutput (task_kSemiCentral,1,output_kSemiCentral);


}
//______________________________________________________________________________________________________________________________________________________________________________________
void runAnalysis_r (Int_t iChunk=0, const char *mode="full", Bool_t merge=true)  {
    
    //Load Libraries
    LoadLibraries();
    
    //Grid Connection
    TGrid::Connect("alien://");
    
    //Alien Handler
    AliAnalysisGrid *alienHandler = CreateAlienHandler (iChunk,mode,merge);
    if (!alienHandler) return;
    
    //Analysis Manager
    AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisManager");
    mgr->SetGridHandler(alienHandler);
    
    //Event Handler, PID & Centrality
    EventHandler(mgr);
    LoadPhysicsSelection ();
    LoadCentrality ();
    LoadEPSelection();
    LoadPIDResponse();
    
    //Analysis Task
    LoadAnalysisTask(iChunk,mgr);
    
    //Start analysis
    if (!mgr->InitAnalysis())  return;
    mgr->PrintStatus();
    mgr->StartAnalysis("grid");
};
//______________________________________________________________________________________________________________________________________________________________________________________
AliAnalysisGrid* CreateAlienHandler (Int_t iChunk,const char *mode, Bool_t merge )  {
    
    //Alien Handler
    AliAnalysisAlien *alien = new AliAnalysisAlien();
    SetAdditionalLibraries (alien);
    alien->SetOverwriteMode();
    alien->SetCheckCopy(false);
    alien->SetRunMode(mode);
    alien->SetNtestFiles(10);
    alien->SetAPIVersion("V1.1x");
    alien->SetAliPhysicsVersion(ALIPHYSICS_VER);
    alien->AddIncludePath("$ALICE_PHYSICS/include");
    alien->AddIncludePath("$ALICE_ROOT/include");
    alien->SetGridDataDir("/alice/data/2018/LHC18r");
    alien->SetDataPattern("*/pass1/*/AliESDs.root");
    alien->SetRunPrefix("000");
    SetInputRuns (iChunk,alien,mode);
    alien->SetNrunsPerMaster(nRunsPerMaster);
    alien->SetGridWorkingDir(Form("%s_%d",GRIDWorkingDir,iChunk));
    alien->SetGridOutputDir("OUTPUT");
    alien->SetAnalysisSource(Form("%s.cxx",AnalysisTask));
    alien->SetAdditionalLibs(Form("%s.cxx  %s.h",AnalysisTask,AnalysisTask));
    alien->SetMergeViaJDL(merge);
    alien->SetMaxMergeStages(2);
    alien->SetAnalysisMacro(Form("%s_%d.C",AnalysisMacro,iChunk));
    alien->SetSplitMaxInputFileNumber(50);
    alien->SetMasterResubmitThreshold(90);
    alien->SetTTL(TTL);
    alien->SetExecutable(Form("%s_%d.sh",AnalysisMacro,iChunk));
    alien->SetInputFormat("xml-single");
    alien->SetJDLName(Form("%s_%d.jdl",AnalysisMacro,iChunk));
    alien->SetPrice(1);
    alien->SetSplitMode("se");
    return alien;
}
//______________________________________________________________________________________________________________________________________________________________________________________
void EventHandler (AliAnalysisManager *mgr)  {
    
    AliESDInputHandler *esdH = new AliESDInputHandler();
    mgr->SetInputEventHandler(esdH);
}
//______________________________________________________________________________________________________________________________________________________________________________________
void LoadPhysicsSelection()  {
    
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask *physSel = AddTaskPhysicsSelection(false, true);
}
//______________________________________________________________________________________________________________________________________________________________________________________
void LoadPIDResponse ()  {
    
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    AliAnalysisTaskPIDResponse *taskPID = AddTaskPIDResponse(false);
}
//______________________________________________________________________________________________________________________________________________________________________________________
void LoadEPSelection ()  {
    
    AliVZEROEPSelectionTask *vzeroEPSelectionTask;
    gROOT->ProcessLine(".L $ALICE_ROOT/ANALYSIS/macros/AddTaskVZEROEPSelection.C");
    vzeroEPSelectionTask = AddTaskVZEROEPSelection();
}
//______________________________________________________________________________________________________________________________________________________________________________________
void LoadCentrality()  {
    
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
    AliMultSelectionTask *taskMult = AddTaskMultSelection(false);
    taskMult->SetSelectedTriggerClass(AliVEvent::kINT7);
}
//______________________________________________________________________________________________________________________________________________________________________________________
void SetInputRuns (Int_t iChunk, AliAnalysisAlien *alien, const char *mode)  {
    
    
    //Test Mode Option
    if (mode=="test") {alien->AddRunNumber(297595); return;}
    
    //Run List
    Int_t run0[] = { 297595, 297590, 297588, 297558, 297544, 297542, 297541, 297540, 297537, 297512,
                     297483, 297481, 297479, 297452, 297451, 297450, 297446, 297442, 297441, 297415,
                     297414, 297413, 297406, 297405, 297380 };
    
    Int_t run1[] = { 297379, 297372, 297367, 297366, 297363, 297336, 297335, 297333, 297332, 297317,
                     297315, 297312, 297311, 297310, 297278, 297222, 297221, 297218, 297196, 297195,
                     297193, 297133, 297132, 297129, 297128 };
    
    Int_t run2[] = { 297124, 297123, 297119, 297118, 297117, 297085, 297035, 297031, 296966, 296941,
                     296938, 296935, 296934, 296932, 296931, 296930, 296903, 296900, 296899, 296894,
                     296852, 296851, 296850, 296848, 296839 };
          
    Int_t run3[] = { 296838, 296836, 296835, 296799, 296794, 296793, 296790, 296787, 296786, 296785,
                     296784, 296781, 296752, 296694, 296693, 296691, 296690 };

    Int_t nRuns(0);
    if (iChunk==0) nRuns= sizeof(run0)/sizeof(Int_t);
    if (iChunk==1) nRuns= sizeof(run1)/sizeof(Int_t);
    if (iChunk==2) nRuns= sizeof(run2)/sizeof(Int_t);
    if (iChunk==3) nRuns= sizeof(run3)/sizeof(Int_t);

    nRuns = 1;
    for ( Int_t iRun=0 ; iRun<nRuns ; iRun++ )  {
        
        if (iChunk==0) alien->AddRunNumber(run0[iRun]);
        if (iChunk==1) alien->AddRunNumber(run1[iRun]);
        if (iChunk==2) alien->AddRunNumber(run2[iRun]);
        if (iChunk==3) alien->AddRunNumber(run3[iRun]);
    }
}
//______________________________________________________________________________________________________________________________________________________________________________________
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
//______________________________________________________________________________________________________________________________________________________________________________________
void SetAdditionalLibraries (AliAnalysisAlien *alien)  {
    
    alien->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/JETAN -I$ALICE_PHYSICS/PWG/Tools -g");
    
    alien->SetAdditionalLibs("AliAnalysisTaskFlowITSTPCTOFQCSP_Template.cxx AliAnalysisTaskFlowITSTPCTOFQCSP_Template.h libPWGHFhfe.so libCDB.so libSTEER.so  libCORRFW.so libPWGflowBase.so libPWGflowTasks.so libGui.so libProof.so libMinuit.so libXMLParser.so libRAWDatabase.so libRAWDatarec.so libCDB.so libSTEERBase.so libSTEER.so libTPCbase.so libTOFbase.so libTOFrec.so libTRDbase.so libVZERObase.so libVZEROrec.so libT0base.so libT0rec.so ");
}
//______________________________________________________________________________________________________________________________________________________________________________________


