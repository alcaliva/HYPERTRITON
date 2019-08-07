#include "TBranch.h"
#include "TCanvas.h"
#include "AliPDG.h"
#include "TTree.h"
#include "TFile.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"


//________________________________________________________________________________________________________________________________________________________________
void saveTree (const char *data="kINT7")  {
    
    
    //Get Input File
    TFile *inputFile = TFile::Open (Form("input/Input_ReducedTree_Hypertriton_PbPb_%s.root",data));
    TList *inputList = (TList*) inputFile -> Get (Form("Input_%s",data));

    
    //Get Input Histogram
    TH1F *hEvents = (TH1F*) inputList -> FindObject ("hEvents");

    //Get Reduced Tree
    TTree *reducedTree_HyperTriton = (TTree*) inputList -> FindObject ("reducedTree_HyperTriton");
    
    //Write Output
    TFile *outputFile = new TFile (Form("input/Input_HyperTriton_%s.root",data),"recreate");
    outputFile -> cd();
    hEvents -> Write();
    reducedTree_HyperTriton -> Write();
    outputFile -> Close();

    
    printf ("output file created!! \n");
    
}
//________________________________________________________________________________________________________________________________________________________________
