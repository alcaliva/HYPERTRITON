#ifndef AliAnalysisTaskHypertritonBindingEnergyMC_cxx
#define AliAnalysisTaskHypertritonBindingEnergyMC_cxx


#include "AliAnalysisTaskSE.h"
#include "AliMCEventHandler.h"
#include "AliAnalysisUtils.h"
#include "AliAnalysisTask.h"
#include "AliESDtrackCuts.h"
#include "AliPIDResponse.h"
#include "AliESDVertex.h"
#include "AliEventCuts.h"
#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "AliMCEvent.h"
#include "TParticle.h"
#include "TVector3.h"
#include "AliStack.h"
#include "AliESDv0.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"


//____________________________________________________________________________________________________________________________________
class AliAnalysisTaskHypertritonBindingEnergyMC : public AliAnalysisTaskSE {
    
public:
    AliAnalysisTaskHypertritonBindingEnergyMC();
    AliAnalysisTaskHypertritonBindingEnergyMC(const char *name);
    virtual ~AliAnalysisTaskHypertritonBindingEnergyMC();
    
    
    //Analysis Cuts
    void SetAnalysisCuts (
                           Double_t CentralityMin,
                           Double_t CentralityMax,
                           Double_t PtPionMin,
                           Double_t PtPionMax,
                           Double_t Pt3HeMin,
                           Double_t Pt3HeMax,
                           Double_t EtaMax,
                           Int_t    NumberOfClustersTPCMin,
                           Int_t    NumberOfClustersTPCdEdxMin,
                           Double_t ChiSquarePerTPCclsMax,
                           Bool_t   UseOnTheFlyV0Finder,
                           Double_t DCAxyPos_Min,
                           Double_t DCAxyNeg_Min,
                           Double_t DCAV0daughtersMax,
                           Double_t ChiSquareV0Max,
                           Double_t DecayRadiusMin,
                           Double_t CosPointingAngleMin,
                           Double_t TPCnsigmaPionMax,
                           Double_t TPCnsigma3HeMax)  {
    
        fCentralityMin                 = CentralityMin;
        fCentralityMax                 = CentralityMax;
        fPtPionMin                     = PtPionMin;
        fPtPionMax                     = PtPionMax;
        fPt3HeMin                      = Pt3HeMin;
        fPt3HeMax                      = Pt3HeMax;
        fEtaMax                        = EtaMax;
        fNumberOfClustersTPCMin        = NumberOfClustersTPCMin;
        fNumberOfClustersTPCdEdxMin    = NumberOfClustersTPCdEdxMin;
        fChiSquarePerTPCclsMax         = ChiSquarePerTPCclsMax;
        fUseOnTheFlyV0Finder           = UseOnTheFlyV0Finder;
        fDCAxyPos_Min                  = DCAxyPos_Min;
        fDCAxyNeg_Min                  = DCAxyNeg_Min;
        fDCAV0daughtersMax             = DCAV0daughtersMax;
        fChiSquareV0Max                = ChiSquareV0Max;
        fDecayRadiusMin                = DecayRadiusMin;
        fCosPointingAngleMin           = CosPointingAngleMin;
        fTPCnsigmaPionMax              = TPCnsigmaPionMax;
        fTPCnsigma3HeMax               = TPCnsigma3HeMax;
    }

    virtual void   UserCreateOutputObjects();
    virtual void   UserExec (Option_t *option);
    
    Bool_t   GetEvent ();
    Bool_t   PassedV0QualityCuts          (AliESDv0 *V0);
    Bool_t   PassedTrackQualityCuts_Pos   (AliESDtrack *track);
    Bool_t   PassedTrackQualityCuts_Neg   (AliESDtrack *track);
    Bool_t   IsPionCandidate              (AliESDtrack *track);
    Bool_t   Is3HeCandidate               (AliESDtrack *track);
    Double_t InvariantMassHypertriton     (TVector3 Ppos, TVector3 Pneg);
    Double_t InvariantMassAntiHypertriton (TVector3 Ppos, TVector3 Pneg);

    virtual void   Terminate(Option_t *);
    
private:
    AliESDEvent       *fESDevent;//!
    AliMCEvent        *fMCevent;//!
    AliStack          *fStack;//!
    AliESDtrackCuts   *fESDtrackCuts_Pos;//!
    AliESDtrackCuts   *fESDtrackCuts_Neg;//!
    AliPIDResponse    *fPIDResponse;//!
    AliEventCuts       fESDeventCuts;//
    AliAnalysisUtils  *fUtils;//!
    TList             *fOutputList;//!
    TList             *fQAList;//!

    
    //Analysis Cuts
    Double_t fCentralityMin;//
    Double_t fCentralityMax;//
    Double_t fPtPionMin;//
    Double_t fPtPionMax;//
    Double_t fPt3HeMin;//
    Double_t fPt3HeMax;//
    Double_t fEtaMax;//
    Int_t    fNumberOfClustersTPCMin;//
    Int_t    fNumberOfClustersTPCdEdxMin;//
    Double_t fChiSquarePerTPCclsMax;//
    Bool_t   fUseOnTheFlyV0Finder;//
    Double_t fDCAxyPos_Min;//
    Double_t fDCAxyNeg_Min;//
    Double_t fDCAV0daughtersMax;//
    Double_t fChiSquareV0Max;//
    Double_t fDecayRadiusMin;//
    Double_t fCosPointingAngleMin;//
    Double_t fTPCnsigmaPionMax;//
    Double_t fTPCnsigma3HeMax;//
    
    
    
    
    //Number of Events
    TH1F *hEvents;//!

    //p_{T} Correction
    TH3F *h3He_DeltaPt_vs_Pt_vs_Eta;//!
    TH3F *hAnti3He_DeltaPt_vs_Pt_vs_Eta;//!
    TH3F *hPiPlus_DeltaPt_vs_Pt_vs_Eta;//!
    TH3F *hPiMinus_DeltaPt_vs_Pt_vs_Eta;//!

    //Mass Shift
    TH2F *hHypertritonMass_vs_Pt;//!
    TH2F *hAntiHypertritonMass_vs_Pt;//!
    
    
    
    
    AliAnalysisTaskHypertritonBindingEnergyMC(const AliAnalysisTaskHypertritonBindingEnergyMC&);
    AliAnalysisTaskHypertritonBindingEnergyMC& operator=(const AliAnalysisTaskHypertritonBindingEnergyMC&);
    
    ClassDef(AliAnalysisTaskHypertritonBindingEnergyMC, 1);
};
//____________________________________________________________________________________________________________________________________

#endif
