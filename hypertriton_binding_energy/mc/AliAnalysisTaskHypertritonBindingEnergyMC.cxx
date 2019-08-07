#include "AliAnalysisTaskHypertritonBindingEnergyMC.h"
#include "AliInputEventHandler.h"
#include "AliAnalysisManager.h"
#include "AliMCEventHandler.h"
#include "AliAnalysisTaskSE.h"
#include "AliAnalysisUtils.h"
#include "AliMultSelection.h"
#include "AliMultEstimator.h"
#include "AliAnalysisTask.h"
#include "AliPIDResponse.h"
#include "TLorentzVector.h"
#include "AliCentrality.h"
#include "AliEventplane.h"
#include "AliEventCuts.h"
#include "TDatabasePDG.h"
#include "AliESDVertex.h"
#include "AliESDtrack.h"
#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "AliStack.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TVectorD.h"
#include "AliESDv0.h"
#include "TRandom.h"
#include "TChain.h"
#include "AliLog.h"
#include "TMath.h"
#include "TList.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"

ClassImp(AliAnalysisTaskHypertritonBindingEnergyMC)

//_________________________________________________________________________________________________________________________________________________________________________________________________
AliAnalysisTaskHypertritonBindingEnergyMC::AliAnalysisTaskHypertritonBindingEnergyMC():
AliAnalysisTaskSE(),
fESDevent(NULL),
fMCevent(NULL),
fStack(NULL),
fESDtrackCuts_Pos(NULL),
fESDtrackCuts_Neg(NULL),
fPIDResponse(NULL),
fESDeventCuts(),
fUtils(NULL),
fOutputList(NULL),
fQAList(NULL),
hEvents(NULL),
h3He_DeltaPt_vs_Pt_vs_Eta(NULL),
hAnti3He_DeltaPt_vs_Pt_vs_Eta(NULL),
hPiPlus_DeltaPt_vs_Pt_vs_Eta(NULL),
hPiMinus_DeltaPt_vs_Pt_vs_Eta(NULL),
hHypertritonMass_vs_Pt(NULL),
hAntiHypertritonMass_vs_Pt(NULL),
fCentralityMin(0),
fCentralityMax(90),
fPtPionMin(0),
fPtPionMax(0),
fPt3HeMin(0),
fPt3HeMax(0),
fEtaMax(0),
fNumberOfClustersTPCMin(0),
fNumberOfClustersTPCdEdxMin(0),
fChiSquarePerTPCclsMax(0),
fUseOnTheFlyV0Finder(0),
fDCAxyPos_Min(0),
fDCAxyNeg_Min(0),
fDCAV0daughtersMax(0),
fChiSquareV0Max(0),
fDecayRadiusMin(0),
fCosPointingAngleMin(0),
fTPCnsigmaPionMax(0),
fTPCnsigma3HeMax(0)
{}
//_________________________________________________________________________________________________________________________________________________________________________________________________
AliAnalysisTaskHypertritonBindingEnergyMC::AliAnalysisTaskHypertritonBindingEnergyMC(const char *name):
AliAnalysisTaskSE(name),
fESDevent(NULL),
fMCevent(NULL),
fStack(NULL),
fESDtrackCuts_Pos(NULL),
fESDtrackCuts_Neg(NULL),
fPIDResponse(NULL),
fESDeventCuts(),
fUtils(NULL),
fOutputList(NULL),
fQAList(NULL),
hEvents(NULL),
h3He_DeltaPt_vs_Pt_vs_Eta(NULL),
hAnti3He_DeltaPt_vs_Pt_vs_Eta(NULL),
hPiPlus_DeltaPt_vs_Pt_vs_Eta(NULL),
hPiMinus_DeltaPt_vs_Pt_vs_Eta(NULL),
hHypertritonMass_vs_Pt(NULL),
hAntiHypertritonMass_vs_Pt(NULL),
fCentralityMin(0),
fCentralityMax(90),
fPtPionMin(0),
fPtPionMax(0),
fPt3HeMin(0),
fPt3HeMax(0),
fEtaMax(0),
fNumberOfClustersTPCMin(0),
fNumberOfClustersTPCdEdxMin(0),
fChiSquarePerTPCclsMax(0),
fUseOnTheFlyV0Finder(0),
fDCAxyPos_Min(0),
fDCAxyNeg_Min(0),
fDCAV0daughtersMax(0),
fChiSquareV0Max(0),
fDecayRadiusMin(0),
fCosPointingAngleMin(0),
fTPCnsigmaPionMax(0),
fTPCnsigma3HeMax(0)
{
    fUtils = new AliAnalysisUtils();
    DefineInput (0, TChain::Class());
    DefineOutput(1, TList::Class());
    DefineOutput(2, TList::Class());
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
AliAnalysisTaskHypertritonBindingEnergyMC::~AliAnalysisTaskHypertritonBindingEnergyMC()  {
    
    fOutputList -> Clear();
    fQAList     -> Clear();
    delete fESDevent;
    delete fMCevent;
    delete fStack;
    delete fESDtrackCuts_Pos;
    delete fESDtrackCuts_Neg;
    delete fPIDResponse;
    delete fUtils;
    delete fOutputList;
    delete fQAList;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
void AliAnalysisTaskHypertritonBindingEnergyMC::UserCreateOutputObjects()  {
    
    //Output List
    fOutputList = new TList();
    fOutputList -> SetOwner();
    
    //QA List
    fQAList = new TList();
    fQAList -> SetOwner();
    
    //QA Plots of Event Selection
    fESDeventCuts.AddQAplotsToList(fQAList);

    
    //Number of Events
    hEvents = new TH1F ("hEvents","",15,0,15);
    fOutputList -> Add(hEvents);

    //p_{T} Correction
    h3He_DeltaPt_vs_Pt_vs_Eta     = new TH3F ("h3He_DeltaPt_vs_Pt_vs_Eta","",90,1,10,400,-1,1,18,-0.9,0.9);
    hAnti3He_DeltaPt_vs_Pt_vs_Eta = new TH3F ("hAnti3He_DeltaPt_vs_Pt_vs_Eta","",90,1,10,400,-1,1,18,-0.9,0.9);
    hPiPlus_DeltaPt_vs_Pt_vs_Eta  = new TH3F ("hPiPlus_DeltaPt_vs_Pt_vs_Eta","",100,0,2,200,-0.1,0.1,18,-0.9,0.9);
    hPiMinus_DeltaPt_vs_Pt_vs_Eta = new TH3F ("hPiMinus_DeltaPt_vs_Pt_vs_Eta","",100,0,2,200,-0.1,0.1,18,-0.9,0.9);
    h3He_DeltaPt_vs_Pt_vs_Eta     -> Sumw2();
    hAnti3He_DeltaPt_vs_Pt_vs_Eta -> Sumw2();
    hPiPlus_DeltaPt_vs_Pt_vs_Eta  -> Sumw2();
    hPiMinus_DeltaPt_vs_Pt_vs_Eta -> Sumw2();
    fOutputList -> Add (h3He_DeltaPt_vs_Pt_vs_Eta);
    fOutputList -> Add (hAnti3He_DeltaPt_vs_Pt_vs_Eta);
    fOutputList -> Add (hPiPlus_DeltaPt_vs_Pt_vs_Eta);
    fOutputList -> Add (hPiMinus_DeltaPt_vs_Pt_vs_Eta);

    //Mass Shift
    hHypertritonMass_vs_Pt     = new TH2F ("hHypertritonMass_vs_Pt","",90,1,10,1000,2.95,3.05);
    hAntiHypertritonMass_vs_Pt = new TH2F ("hAntiHypertritonMass_vs_Pt","",90,1,10,1000,2.95,3.05);
    hHypertritonMass_vs_Pt     -> Sumw2();
    hAntiHypertritonMass_vs_Pt -> Sumw2();
    fOutputList -> Add (hHypertritonMass_vs_Pt);
    fOutputList -> Add (hAntiHypertritonMass_vs_Pt);

    
    //Track Cuts Object
    fESDtrackCuts_Pos = new AliESDtrackCuts ("fESDtrackCuts_Pos");
    fESDtrackCuts_Neg = new AliESDtrackCuts ("fESDtrackCuts_Neg");

    
    PostData(1, fOutputList);
    PostData(2, fQAList);
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
void AliAnalysisTaskHypertritonBindingEnergyMC::UserExec(Option_t *)  {
    

    //Get Input Event
    if ( !GetEvent ()) return;
    
    
    //Load PID Response
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    AliInputEventHandler *inputHandler = (AliInputEventHandler*) (mgr->GetInputEventHandler());
    fPIDResponse = inputHandler->GetPIDResponse();
    
    
    //Loop Over Reconstructed V0s
    for ( Int_t iV0=0 ; iV0<fESDevent->GetNumberOfV0s() ; iV0++ ) {

        //Get V0 Candidate
        AliESDv0 *V0 = (AliESDv0*)fESDevent->GetV0(iV0);
        if (!V0) continue;
        if ( fUseOnTheFlyV0Finder && !V0->GetOnFlyStatus()) continue;
        if (!fUseOnTheFlyV0Finder &&  V0->GetOnFlyStatus()) continue;
        if (!PassedV0QualityCuts(V0)) continue;

        //Get V0 Daughters
        AliESDtrack *posTrack = (AliESDtrack*) fESDevent->GetTrack(V0->GetPindex());
        AliESDtrack *negTrack = (AliESDtrack*) fESDevent->GetTrack(V0->GetNindex());
        if (!posTrack) continue;
        if (!negTrack) continue;
        if (posTrack->Charge() == negTrack->Charge()) continue;
        if (posTrack->GetID()  == negTrack->GetID() ) continue;

        //Track Quality Cuts
        if (!PassedTrackQualityCuts_Pos (posTrack)) continue;
        if (!PassedTrackQualityCuts_Neg (negTrack)) continue;
        
        //Momentum Components of V0 Daughters
        Double_t posMomentum[3] = { 0.0, 0.0, 0.0 };
        Double_t negMomentum[3] = { 0.0, 0.0, 0.0 };
        V0->GetPPxPyPz(posMomentum[0],posMomentum[1],posMomentum[2]);
        V0->GetNPxPyPz(negMomentum[0],negMomentum[1],negMomentum[2]);
        TVector3 Ppos (posMomentum[0],posMomentum[1],posMomentum[2]);
        TVector3 Pneg (negMomentum[0],negMomentum[1],negMomentum[2]);
        
        //MC Particles
        TParticle *particlePos = fMCEvent->Particle(TMath::Abs(posTrack->GetLabel()));
        TParticle *particleNeg = fMCEvent->Particle(TMath::Abs(negTrack->GetLabel()));
        if( !particlePos) continue;
        if( !particleNeg) continue;

        //Parent Selection
        Int_t labelParentPos = particlePos -> GetMother(0);
        Int_t labelParentNeg = particleNeg -> GetMother(0);
        if (labelParentPos<0 || labelParentNeg<0) continue;
        if (labelParentPos != labelParentNeg)     continue;
        TParticle *parent = fMCEvent->Particle (labelParentPos);
        if ( TMath::Abs(parent->GetPdgCode()) != 1010010030 ) continue;
        
        
        //p_{T} Correction
        if (TMath::Abs(particlePos->GetPdgCode())==1000020030)  h3He_DeltaPt_vs_Pt_vs_Eta     -> Fill (2.0*Ppos.Pt(),particlePos->Pt()-2.0*Ppos.Pt(),Ppos.Eta());
        if (TMath::Abs(particleNeg->GetPdgCode())==1000020030)  hAnti3He_DeltaPt_vs_Pt_vs_Eta -> Fill (2.0*Pneg.Pt(),particleNeg->Pt()-2.0*Pneg.Pt(),Pneg.Eta());
        if (TMath::Abs(particlePos->GetPdgCode())==211)  hPiPlus_DeltaPt_vs_Pt_vs_Eta  -> Fill (Ppos.Pt(),particlePos->Pt()-Ppos.Pt(),Ppos.Eta());
        if (TMath::Abs(particleNeg->GetPdgCode())==211)  hPiMinus_DeltaPt_vs_Pt_vs_Eta -> Fill (Pneg.Pt(),particleNeg->Pt()-Pneg.Pt(),Pneg.Eta());

        
        
        //Mass Shift
        if (TMath::Abs(particlePos->GetPdgCode())==1000020030 && TMath::Abs(particleNeg->GetPdgCode())==211)  {
            
            TVector3 Pv0Tot = 2.0*Ppos+Pneg;
            Double_t mass = InvariantMassHypertriton (2.0*Ppos,Pneg);
            hHypertritonMass_vs_Pt -> Fill (Pv0Tot.Pt(),mass);
        }
        
        //Mass Shift
        if (TMath::Abs(particlePos->GetPdgCode())==211 && TMath::Abs(particleNeg->GetPdgCode())==1000020030)  {
            
            TVector3 Pv0Tot = 2.0*Pneg+Ppos;
            Double_t mass = InvariantMassAntiHypertriton (Ppos,2.0*Pneg);
            hAntiHypertritonMass_vs_Pt -> Fill (Pv0Tot.Pt(),mass);
        }
    }

    
    PostData(1, fOutputList);
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
Bool_t AliAnalysisTaskHypertritonBindingEnergyMC::GetEvent ()  {
    
    
    //Get Input Event
    fESDevent = dynamic_cast <AliESDEvent*> (InputEvent());
    if (!fESDevent) return false;
    fMCEvent = MCEvent();
    if (!fMCEvent) return false;
    fStack = fMCEvent->Stack();
    if (!fStack) return false;
    hEvents -> Fill(0.5);
    
    
    //Standard Event Cuts
    if (!fESDeventCuts.AcceptEvent(fESDevent)) {
        PostData(2, fQAList);
        return false;
    }
    hEvents -> Fill(1.5);
    
    
    //Primary Vertex
    AliESDVertex *vertex = (AliESDVertex*) fESDevent->GetPrimaryVertex();
    if ( !vertex ) return false;
    if ( TMath::Abs(vertex->GetZ() ) > 10.0 ) return false;
    if ( vertex->GetNContributors() < 1 ) return false;
    hEvents -> Fill(2.5);

    
    //Centrality
    Double_t centralityPercentile = 0;
    AliMultSelection *multiplicitySelection = (AliMultSelection*) fESDevent->FindListObject("MultSelection");
    if( !multiplicitySelection) return false;
    centralityPercentile = multiplicitySelection->GetMultiplicityPercentile("V0M");
    if (centralityPercentile<fCentralityMin || centralityPercentile>=fCentralityMax ) return false;
    hEvents -> Fill(3.5);
    
    return true;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
Bool_t AliAnalysisTaskHypertritonBindingEnergyMC::PassedTrackQualityCuts_Pos (AliESDtrack *track)  {
    
    fESDtrackCuts_Pos -> SetAcceptKinkDaughters(false);
    fESDtrackCuts_Pos -> SetMinNClustersTPC(fNumberOfClustersTPCMin);
    fESDtrackCuts_Pos -> SetRequireTPCRefit(true);
    fESDtrackCuts_Pos -> SetMaxChi2PerClusterTPC(fChiSquarePerTPCclsMax);
    fESDtrackCuts_Pos -> SetEtaRange (-fEtaMax,fEtaMax);
    fESDtrackCuts_Pos -> SetMinDCAToVertexXY (fDCAxyPos_Min);
    if ( track->GetTPCsignalN() < fNumberOfClustersTPCdEdxMin ) return false;
    if ( !fESDtrackCuts_Pos->AcceptTrack (track) ) return false;
    
    //MC Particle
    TParticle *particle = fMCEvent->Particle(TMath::Abs(track->GetLabel()));
    Double_t pdg = TMath::Abs(particle->GetPdgCode());
    
    //Pion
    if (pdg==211)  {
        if (track->Pt()<fPtPionMin)   return false;
        if (track->Pt()>fPtPionMax)   return false;
        if ( !IsPionCandidate(track)) return false;
    }
    
    //3He
    if (pdg==1000020030)  {
        if (2.0*track->Pt()<fPt3HeMin) return false;
        if (2.0*track->Pt()>fPt3HeMax) return false;
        if ( !Is3HeCandidate(track))   return false;
    }
    
    return true;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
Bool_t AliAnalysisTaskHypertritonBindingEnergyMC::PassedTrackQualityCuts_Neg (AliESDtrack *track)  {
    
    fESDtrackCuts_Neg -> SetAcceptKinkDaughters(false);
    fESDtrackCuts_Neg -> SetMinNClustersTPC(fNumberOfClustersTPCMin);
    fESDtrackCuts_Neg -> SetRequireTPCRefit(true);
    fESDtrackCuts_Neg -> SetMaxChi2PerClusterTPC(fChiSquarePerTPCclsMax);
    fESDtrackCuts_Neg -> SetEtaRange (-fEtaMax,fEtaMax);
    fESDtrackCuts_Neg -> SetMinDCAToVertexXY (fDCAxyNeg_Min);
    if ( track->GetTPCsignalN() < fNumberOfClustersTPCdEdxMin ) return false;
    if ( !fESDtrackCuts_Neg->AcceptTrack (track) ) return false;
    
    //MC Particle
    TParticle *particle = fMCEvent->Particle(TMath::Abs(track->GetLabel()));
    Double_t pdg = TMath::Abs(particle->GetPdgCode());
    
    //Pion
    if (pdg==211)  {
        if (track->Pt()<fPtPionMin)   return false;
        if (track->Pt()>fPtPionMax)   return false;
        if ( !IsPionCandidate(track)) return false;
    }
    
    //3He
    if (pdg==1000020030)  {
        if (2.0*track->Pt()<fPt3HeMin) return false;
        if (2.0*track->Pt()>fPt3HeMax) return false;
        if ( !Is3HeCandidate(track))   return false;
    }
    
    return true;
}


//_________________________________________________________________________________________________________________________________________________________________________________________________
Double_t AliAnalysisTaskHypertritonBindingEnergyMC::InvariantMassHypertriton (TVector3 P1, TVector3 P2)  {

    //Mass Daughters
    Double_t m3He  = AliPID::ParticleMass (AliPID::kHe3);
    Double_t mpion = AliPID::ParticleMass (AliPID::kPion);

    //Invariant Mass Calculation
    TVector3 P = P1 + P2;
    
    Double_t E1 = TMath::Sqrt(m3He*m3He + P1.Mag2());
    Double_t E2 = TMath::Sqrt(mpion*mpion + P2.Mag2());
    Double_t m = TMath::Sqrt( (E1+E2)*(E1+E2) - P.Mag2() );
    
    return m;
}

//_________________________________________________________________________________________________________________________________________________________________________________________________
Double_t AliAnalysisTaskHypertritonBindingEnergyMC::InvariantMassAntiHypertriton (TVector3 P1, TVector3 P2)  {
    
    //Mass Daughters
    Double_t m3He  = AliPID::ParticleMass (AliPID::kHe3);
    Double_t mpion = AliPID::ParticleMass (AliPID::kPion);
    
    //Invariant Mass Calculation
    TVector3 P = P1 + P2;
    
    Double_t E1 = TMath::Sqrt(mpion*mpion + P1.Mag2());
    Double_t E2 = TMath::Sqrt(m3He*m3He + P2.Mag2());
    Double_t m = TMath::Sqrt( (E1+E2)*(E1+E2) - P.Mag2() );
    
    return m;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________

Bool_t AliAnalysisTaskHypertritonBindingEnergyMC::PassedV0QualityCuts (AliESDv0 *V0)  {
    
    if (V0->GetDcaV0Daughters()>fDCAV0daughtersMax) return false;
    if (V0->GetRr()<fDecayRadiusMin) return false;
    if (V0->GetV0CosineOfPointingAngle()<fCosPointingAngleMin) return false;
    if (V0->GetChi2V0()>fChiSquareV0Max) return false;
    
    return true;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
Bool_t AliAnalysisTaskHypertritonBindingEnergyMC::IsPionCandidate (AliESDtrack *track)  {
    
    Double_t nsigmaTPC = fPIDResponse -> NumberOfSigmasTPC (track,AliPID::kPion);
    if (TMath::Abs(nsigmaTPC) > fTPCnsigmaPionMax) return false;
    
    return true;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
Bool_t AliAnalysisTaskHypertritonBindingEnergyMC::Is3HeCandidate (AliESDtrack *track)  {
    
    Double_t nsigmaTPC = fPIDResponse -> NumberOfSigmasTPC (track,AliPID::kHe3);
    if (TMath::Abs(nsigmaTPC) > fTPCnsigma3HeMax) return false;
    
    return true;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
void AliAnalysisTaskHypertritonBindingEnergyMC::Terminate(Option_t *)  {
    
    fOutputList = dynamic_cast<TList*> (GetOutputData(1));
    if (!fOutputList) return;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________

