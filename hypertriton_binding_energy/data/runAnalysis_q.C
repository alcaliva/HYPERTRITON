#define ALIPHYSICS_VER  "vAN-20190805-1"
#define GRIDWorkingDir  "REDUCED_TREE_HYPERTRITON_PbPb_LHC18q"
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
    AliAnalysisDataContainer *output_kINT7 = mgr -> CreateContainer("Input_kINT7",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_ReducedTree_Hypertriton_PbPb_LHC18q_kINT7%d.root",iChunk));
    mgr -> ConnectInput  (task_kINT7,0,input);
    mgr -> ConnectOutput (task_kINT7,1,output_kINT7);
    
    //Analysis Task: Central Collision
    AliAnalysisTaskReducedTreeHypertriton *task_kCentral = new AliAnalysisTaskReducedTreeHypertriton  ("task_kCentral");
    task_kCentral -> SelectCollisionCandidates (AliVEvent::kCentral);
    task_kCentral -> SetCentrality (0.0,10.0);
    mgr -> AddTask(task_kCentral);
    AliAnalysisDataContainer *output_kCentral = mgr -> CreateContainer("Input_kCentral",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_ReducedTree_Hypertriton_PbPb_LHC18q_kCentral%d.root",iChunk));
    mgr -> ConnectInput  (task_kCentral,0,input);
    mgr -> ConnectOutput (task_kCentral,1,output_kCentral);
    
    //Analysis Task: SemiCentral Collision
    AliAnalysisTaskReducedTreeHypertriton *task_kSemiCentral = new AliAnalysisTaskReducedTreeHypertriton  ("task_kSemiCentral");
    task_kSemiCentral -> SelectCollisionCandidates (AliVEvent::kSemiCentral);
    task_kSemiCentral -> SetCentrality (30.0,50.0);
    mgr -> AddTask(task_kSemiCentral);
    AliAnalysisDataContainer *output_kSemiCentral = mgr -> CreateContainer("Input_kSemiCentral",TList::Class(),AliAnalysisManager::kOutputContainer,Form("Input_ReducedTree_Hypertriton_PbPb_LHC18q_kSemiCentral%d.root",iChunk));
    mgr -> ConnectInput  (task_kSemiCentral,0,input);
    mgr -> ConnectOutput (task_kSemiCentral,1,output_kSemiCentral);


}
//______________________________________________________________________________________________________________________________________________________________________________________
void runAnalysis_q (Int_t iChunk=0, const char *mode="full", Bool_t merge=true)  {
    
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
    alien->SetGridDataDir("/alice/data/2018/LHC18q");
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
    if (mode=="test") {alien->AddRunNumber(296623); return;}
    
    
    //Run List
    Int_t run0[] = { 296623, 296622, 296621, 296619, 296618, 296616, 296615, 296594, 296553, 296552,
                     296551, 296550, 296549, 296548, 296547, 296516, 296512, 296511, 296510, 296509,
                     296472, 296433, 296424, 296423, 296420 };
    
    Int_t run1[] = { 296419, 296415, 296414, 296383, 296381, 296380, 296379, 296378, 296377, 296376,
                     296375, 296312, 296309, 296307, 296304, 296303, 296280, 296279, 296273, 296270 };
        
    Int_t run2[] = { 296269, 296247, 296246, 296244, 296243, 296242, 296241, 296240, 296198, 296197,
                     296196, 296195, 296194, 296192, 296191, 296143, 296142, 296135, 296134, 296133 };
            
    Int_t run3[] = { 296132, 296123, 296074, 296068, 296066, 296065, 296063, 296062, 296060, 296016,
                     295947, 295945, 295943, 295942, 295941, 295937, 295936, 295913, 295910, 295909 };

    Int_t run4[] = { 295908, 295881, 295861, 295860, 295859, 295856, 295855, 295854, 295853, 295831,
                     295829, 295826, 295825, 295822, 295819, 295818, 295816, 295791, 295788, 295786 };
    
    Int_t run5[] = { 295763, 295762, 295759, 295758, 295755, 295754, 295725, 295723, 295721, 295719,
                     295718, 295717, 295714, 295712, 295677, 295676, 295675, 295673, 295671, 295668,
                     295667, 295666, 295615, 295612, 295611, 295610, 295589, 295588, 295587, 295586,
                     295585 };
    
    
    
    Int_t nRuns(0);
    if (iChunk==0) nRuns= sizeof(run0)/sizeof(Int_t);
    if (iChunk==1) nRuns= sizeof(run1)/sizeof(Int_t);
    if (iChunk==2) nRuns= sizeof(run2)/sizeof(Int_t);
    if (iChunk==3) nRuns= sizeof(run3)/sizeof(Int_t);
    if (iChunk==4) nRuns= sizeof(run4)/sizeof(Int_t);
    if (iChunk==5) nRuns= sizeof(run5)/sizeof(Int_t);

    nRuns = 1;
    for ( Int_t iRun=0 ; iRun<nRuns ; iRun++ )  {
        
        if (iChunk==0) alien->AddRunNumber(run0[iRun]);
        if (iChunk==1) alien->AddRunNumber(run1[iRun]);
        if (iChunk==2) alien->AddRunNumber(run2[iRun]);
        if (iChunk==3) alien->AddRunNumber(run3[iRun]);
        if (iChunk==4) alien->AddRunNumber(run4[iRun]);
        if (iChunk==5) alien->AddRunNumber(run5[iRun]);
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


