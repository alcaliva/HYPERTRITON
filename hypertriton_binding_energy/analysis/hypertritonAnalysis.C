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
    Double_t Nevts = 1e-6*hEvents -> GetBinContent (8);
    printf ("Nevts = %.0f M\n",Nevts);

    
    //HyperTriton Mass Spectrum
    TH1F *hMassHyperTriton = new TH1F ("hMassHyperTriton","",200,2.8,3.2);
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
    hMassHyperTriton -> GetXaxis() -> SetRangeUser (2.97,3.04);

    
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

    
    //reducedTree_HyperTriton ->Scan();
    
    //Mass of 3He and pion
    Double_t m3He  = AliPID::ParticleMass (AliPID::kHe3);
    Double_t mpion = AliPID::ParticleMass (AliPID::kPion);
    
    
    
    //Loop over tree entries
    for ( Int_t iEntry=0 ; iEntry<reducedTree_HyperTriton->GetEntries() ; iEntry++ )  {
        
        //Get Entry
        reducedTree_HyperTriton->GetEntry(iEntry);
        
        //Pair Cuts
        if (cosPointingAngle<0.995) continue;
        if (dcaV0Daughters>0.7) continue;
        if (radius<4) continue;
        if (dcaV0ToVertex>2.0) continue;
        
        //Single Track Cuts
        if (nTPC_Clusters_Daughter1<70) continue;
        if (nTPC_Clusters_Daughter2<70) continue;
        if ((chi2_TPC_Daughter1/(Double_t) nTPC_Clusters_Daughter1) > 5.0) continue;
        if ((chi2_TPC_Daughter2/(Double_t) nTPC_Clusters_Daughter2) > 5.0) continue;
        if ((Double_t)nTPC_CrossedRows_Daughter1/(Double_t)nTPC_FindableClusters_Daughter1<0.6) continue;
        if ((Double_t)nTPC_CrossedRows_Daughter2/(Double_t)nTPC_FindableClusters_Daughter2<0.6) continue;
        
        //PID on V0 Daughters
        if (TMath::Abs(nSigmaTPC_He3_Daughter1)<3.0 && TMath::Abs(nSigmaTPC_Pion_Daughter2)<3.0)  {
            
            //Invariant Mass Calculation
            TVector3 P1 (2.0*px_Daughter1,2.0*py_Daughter1,2.0*pz_Daughter1);
            TVector3 P2 (px_Daughter2,py_Daughter2,pz_Daughter2);
            TVector3 P = P1 + P2;
            
            Double_t E1 = TMath::Sqrt(m3He*m3He + P1.Mag2());
            Double_t E2 = TMath::Sqrt(mpion*mpion + P2.Mag2());
            Double_t m = TMath::Sqrt( (E1+E2)*(E1+E2) - P.Mag2() );
            
            hMassHyperTriton -> Fill (m);
        }
        
        
        if (TMath::Abs(nSigmaTPC_He3_Daughter2)<3.0 && TMath::Abs(nSigmaTPC_Pion_Daughter1)<3.0)  {

       
        //Invariant Mass Calculation
        TVector3 P1 (px_Daughter1,py_Daughter1,pz_Daughter1);
        TVector3 P2 (2.0*px_Daughter2,2.0*py_Daughter2,2.0*pz_Daughter2);
        TVector3 P = P1 + P2;
        
        Double_t E2 = TMath::Sqrt(m3He*m3He + P2.Mag2());
        Double_t E1 = TMath::Sqrt(mpion*mpion + P1.Mag2());
        Double_t m = TMath::Sqrt( (E1+E2)*(E1+E2) - P.Mag2() );
        
        hMassHyperTriton -> Fill (m);
        }
    }
    

    
    
    
    //Fit Options
    TVirtualFitter::SetMaxIterations (1e+05);
    
    
    
    //Fit Range
    const Double_t mFit_Min = 2.98;
    const Double_t mFit_Max = 3.05;
    
    
    
    
    
    
    TF1 *fit = new TF1 ("fit","[0]*exp(-0.5*((x-[1])/[2])**2) + [3] + [4]*x",2.9,3.1);
    fit -> SetLineStyle (2);
    fit -> SetLineColor (2);
    fit -> SetLineWidth (3);
    fit -> SetParameter (0,180.0);
    fit -> SetParameter (1,2.98);
    fit -> SetParameter (2,0.005);
    Int_t status = hMassHyperTriton -> Fit("fit","QSM0R",NULL,mFit_Min,mFit_Max);
        
    
    
    
    
    
    
    //Kinematic Threshold
    TLine *kinematicThreshold = new TLine (m3He+mpion,0,m3He+mpion,40);
    kinematicThreshold -> SetLineStyle(2);
    kinematicThreshold -> SetLineColor(2);
    kinematicThreshold -> SetLineWidth(3);

    
    
    
    //Plot Efficiency
    TCanvas *cMassHypertriton = new TCanvas ("cMassHypertriton","",900,700);
    cMassHypertriton -> cd();
    cMassHypertriton -> SetTickx(1);
    cMassHypertriton -> SetTicky(1);
    cMassHypertriton -> SetLeftMargin(0.12);
    cMassHypertriton -> SetRightMargin(0.03);
    cMassHypertriton -> SetTopMargin(0.05);
    cMassHypertriton -> SetBottomMargin(0.15);
    hMassHyperTriton -> Draw("E");
    kinematicThreshold -> Draw("same");
    fit -> Draw ("same");
    //Text() -> Draw();
    
    
    

    Double_t mDeuteron = AliPID::ParticleMass (AliPID::kDeuteron);
    Double_t mLambda   = 1.115683;
    Double_t mHypertriton = fit -> GetParameter(1);
    
    Double_t E  = mHypertriton - (mDeuteron+mLambda);
    Double_t DE = fit -> GetParError(1);
    printf ("DE = %.5f +/- %.5f \n",1000*E,1000*DE);

    
    
    
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
