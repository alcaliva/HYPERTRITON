#include "TBranch.h"
#include "TCanvas.h"
#include "AliPDG.h"
#include "AliPID.h"
#include "TTree.h"
#include "TFile.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"

TPaveText *Text ();

//________________________________________________________________________________________________________________________________________________________________
void hypertritonAnalysis ()  {
    
    
    //Get Input File
    TFile *inputFile = TFile::Open ("input/Input_HyperTriton.root");

    
    //Get Input Histograms
    TH1F *hEvents = (TH1F*) inputFile -> Get ("hEvents");
    Int_t Nevts = (Int_t) hEvents -> GetBinContent (8);
    printf ("Nevts = %d\n",Nevts);

    
    //HyperTriton Mass Spectrum
    TH1F *hMassHyperTriton = new TH1F ("hMassHyperTriton","",200,2.9,3.1);
    hMassHyperTriton -> SetStats(false);
    hMassHyperTriton -> GetXaxis() -> SetTitle("#it{m}_{#pi^{3}He} (GeV/#it{c}^{2})");
    hMassHyperTriton -> SetTitleSize(0.045,"x");
    hMassHyperTriton -> SetLabelSize(0.045,"x");
    hMassHyperTriton -> GetXaxis() -> SetTitleOffset(1.2);
    hMassHyperTriton -> GetYaxis() -> SetTitle("Counts");
    hMassHyperTriton -> SetTitleSize(0.045,"y");
    hMassHyperTriton -> SetLabelSize(0.045,"y");
    hMassHyperTriton -> GetYaxis() -> SetTitleOffset(1.2);
    hMassHyperTriton -> GetYaxis() -> CenterTitle();
    hMassHyperTriton -> SetLineStyle(1);
    hMassHyperTriton -> SetLineColor(1);
    hMassHyperTriton -> SetLineWidth(3);
    hMassHyperTriton -> SetMarkerStyle(20);
    hMassHyperTriton -> SetMarkerColor(1);
    hMassHyperTriton -> SetMarkerSize(1.0);
   
    
    //Variables for HyperTriton - First Daughter
    Double_t centrality;
    Double_t px_Daughter1;
    Double_t py_Daughter1;
    Double_t pz_Daughter1;
    Int_t    q_Daughter1;
    Double_t dcaxy_Daughter1;
    Int_t    nTPC_Clusters_Daughter1;
    Int_t    nTPC_FindableClusters_Daughter1;
    Int_t    nTPC_CrossedRows_Daughter1;
    Int_t    nTPC_Clusters_dEdx_Daughter1;
    Double_t chi2_TPC_Daughter1;
    Double_t nSigmaTPC_He3_Daughter1;
    Double_t nSigmaTPC_Pion_Daughter1;
    
    //Variables for HyperTriton - Second Daughter
    Double_t px_Daughter2;
    Double_t py_Daughter2;
    Double_t pz_Daughter2;
    Int_t    q_Daughter2;
    Double_t dcaxy_Daughter2;
    Int_t    nTPC_Clusters_Daughter2;
    Int_t    nTPC_FindableClusters_Daughter2;
    Int_t    nTPC_CrossedRows_Daughter2;
    Int_t    nTPC_Clusters_dEdx_Daughter2;
    Double_t chi2_TPC_Daughter2;
    Double_t nSigmaTPC_He3_Daughter2;
    Double_t nSigmaTPC_Pion_Daughter2;
    
    //Pair Variables
    Double_t cosPointingAngle;
    Double_t dcaV0Daughters;
    Double_t dcaV0ToVertex;
    Double_t radius;
    Double_t decayLength;
    
    //Get Reduced Tree
    TTree *reducedTree_HyperTriton = (TTree*) inputFile -> Get ("reducedTree_HyperTriton");
    reducedTree_HyperTriton -> SetBranchAddress("centrality",&centrality);
    reducedTree_HyperTriton -> SetBranchAddress("px_Daughter1",&px_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("py_Daughter1",&py_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("pz_Daughter1",&pz_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("q_Daughter1",&q_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("dcaxy_Daughter1",&dcaxy_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_Clusters_Daughter1",&nTPC_Clusters_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_FindableClusters_Daughter1",&nTPC_FindableClusters_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_CrossedRows_Daughter1",&nTPC_CrossedRows_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_Clusters_dEdx_Daughter1",&nTPC_Clusters_dEdx_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("chi2_TPC_Daughter1",&chi2_TPC_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("nSigmaTPC_He3_Daughter1",&nSigmaTPC_He3_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("nSigmaTPC_Pion_Daughter1",&nSigmaTPC_Pion_Daughter1);
    reducedTree_HyperTriton -> SetBranchAddress("px_Daughter2",&px_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("py_Daughter2",&py_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("pz_Daughter2",&pz_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("q_Daughter2",&q_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("dcaxy_Daughter2",&dcaxy_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_Clusters_Daughter2",&nTPC_Clusters_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_FindableClusters_Daughter2",&nTPC_FindableClusters_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_CrossedRows_Daughter2",&nTPC_CrossedRows_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("nTPC_Clusters_dEdx_Daughter2",&nTPC_Clusters_dEdx_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("chi2_TPC_Daughter2",&chi2_TPC_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("nSigmaTPC_He3_Daughter2",&nSigmaTPC_He3_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("nSigmaTPC_Pion_Daughter2",&nSigmaTPC_Pion_Daughter2);
    reducedTree_HyperTriton -> SetBranchAddress("cosPointingAngle",&cosPointingAngle);
    reducedTree_HyperTriton -> SetBranchAddress("dcaV0Daughters",&dcaV0Daughters);
    reducedTree_HyperTriton -> SetBranchAddress("dcaV0ToVertex",&dcaV0ToVertex);
    reducedTree_HyperTriton -> SetBranchAddress("radius",&radius);
    reducedTree_HyperTriton -> SetBranchAddress("decayLength",&decayLength);

    
    //Loop over tree entries
    for ( Int_t iEntry=0 ; iEntry<reducedTree_HyperTriton->GetEntries() ; iEntry++ )  {
        
        //Get Entry
        reducedTree_HyperTriton->GetEntry(iEntry);
        
    }
    
}
//________________________________________________________________________________________________________________________________________________________________
TPaveText *Text ()  {
    
    TPaveText *text = new TPaveText(0.4,0.75,0.7,0.88, "NDC");
    text -> SetTextSize(0.042);
    text -> SetTextFont(42);
    text -> SetTextColor(1);
    text -> SetFillColor(0);
    text -> SetBorderSize(0);
    text -> AddText ("#bf{ALICE}");
    text -> AddText ("Pb#font[122]{-}Pb, #sqrt{#it{s}_{NN}} = 5.02 TeV,  0#font[122]{-}90%");
    
    return text;
}
//________________________________________________________________________________________________________________________________________________________________
