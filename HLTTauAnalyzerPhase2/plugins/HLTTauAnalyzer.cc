// Class:      HLTTauAnalyzer
//
// Original Author:  Sandeep Bhowmik
//         Created:  Tue, 12 Mar 2019 18:38:39 GMT
//
#include "FWCore/Framework/interface/one/EDAnalyzer.h" 
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"     
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h" 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TTree.h>

#include "DataFormats/VertexReco/interface/Vertex.h"  
#include "DataFormats/L1Trigger/interface/Tau.h" 
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include <DataFormats/PatCandidates/interface/Tau.h> 

#include "DataFormats/TauReco/interface/PFTau.h"                // reco::PFTau
#include "DataFormats/TauReco/interface/PFTauFwd.h"             // reco::PFTauRef, reco::PFTauCollection
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"   // reco::PFTauDiscriminator


class HLTTauAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit HLTTauAnalyzer(const edm::ParameterSet&);
  ~HLTTauAnalyzer();
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  void Initialize();
  
  TTree *tree_;
  std::string treeName_;
  
  ULong64_t       indexevents_;
  Int_t           runNumber_;
  Int_t           lumi_;
  float MC_weight_;
  std::vector<float> genTauPt_;
  std::vector<float> genTauEta_;
  std::vector<float> genTauPhi_;
  std::vector<int> genTauCharge_;
  std::vector<Bool_t> isGenMatched_;
  std::vector<int> recoTauDecayMode_;
  std::vector<float> recoTauPt_;
  std::vector<float> recoTauEta_;
  std::vector<float> recoTauPhi_;
  std::vector<int> recoTauCharge_;
  std::vector<Bool_t> isRecoMatched_;
  std::vector<int> recoGMTauDecayMode_;
  std::vector<float> recoGMTauPt_;
  std::vector<float> recoGMTauEta_;
  std::vector<float> recoGMTauPhi_;
  std::vector<int> recoGMTauCharge_;
  std::vector<Bool_t> isRecoGMMatched_;
  std::vector<int> hltTauType_;
  std::vector<float> hltTauPt_;
  std::vector<float> hltTauEta_;
  std::vector<float> hltTauPhi_;
  std::vector<int> hltTauCharge_;
  std::vector<float> hltTauIso_;
  std::vector<Bool_t> hltTauTightIso_;
  std::vector<Bool_t> hltTauMediumIso_;
  std::vector<Bool_t> hltTauLooseIso_;
  std::vector<Bool_t> hltTauVLooseIso_;
  std::vector<Bool_t> hltTauTightRelIso_;
  std::vector<Bool_t> hltTauMediumRelIso_;
  std::vector<Bool_t> hltTauLooseRelIso_;
  std::vector<Bool_t> hltTauVLooseRelIso_;
  std::vector<float> hltTauZ_;
  std::vector<float> hltTauLeadTrackPt_;
  double genVertex_;

  bool createHistRoorFile_;
  std::string histRootFileName_;
  TFile* histRootFile_;
  TH1F* hist_genTauPt_;
  TH1F* hist_genTauEta_;
  TH1F* hist_genTauPhi_;
  TH1F* hist_isGenMatched_;
  TH1F* hist_recoTauPt_;
  TH1F* hist_recoTauEta_;
  TH1F* hist_recoTauPhi_;
  TH1F* hist_isRecoMatched_;
  TH1F* hist_recoGMTauPt_;
  TH1F* hist_recoGMTauEta_;
  TH1F* hist_recoGMTauPhi_;
  TH1F* hist_isRecoGMMatched_;
  TH1F* hist_hltTauPt_;
  TH1F* hist_hltTauEta_;
  TH1F* hist_hltTauPhi_;
  TH1F* hist_hltTauReso_vs_Gen_;
  TH1F* hist_hltTauReso_vs_Reco_;
  TH1F* hist_hltTauReso_vs_RecoGM_;

  // ----------member data ---------------------------

  bool debug_;
  bool isReco_;
  double min_pt_;
  double max_eta_;
  edm::EDGetTokenT<GenEventInfoProduct>            genTagToken_;
  edm::EDGetTokenT<std::vector<reco::GenJet>>      genTauToken_;
  edm::EDGetTokenT<std::vector<reco::Vertex>>      recoVertexToken_;
  edm::EDGetTokenT<std::vector<pat::Tau>>          recoTauToken_;
  edm::EDGetTokenT<pat::TauRefVector>              recoGMTauToken_;
  edm::EDGetTokenT<std::vector<reco::PFTau>>       hltTauToken_;
  edm::EDGetTokenT<reco::PFTauDiscriminator>       hltTauSumChargedIsoToken_;
};


HLTTauAnalyzer::HLTTauAnalyzer(const edm::ParameterSet& iConfig)
  : debug_          (iConfig.getUntrackedParameter<bool>("debug", false))
  , isReco_         (iConfig.getUntrackedParameter<bool>("isReco", false))
  , min_pt_         (iConfig.getUntrackedParameter<double>("min_pt", 0))
  , max_eta_        (iConfig.getUntrackedParameter<double>("max_eta", 3.0))
  , genTagToken_    (consumes<GenEventInfoProduct>          (iConfig.getParameter<edm::InputTag>("genTagToken")))
  , genTauToken_    (consumes<std::vector<reco::GenJet>>    (iConfig.getParameter<edm::InputTag>("genTauToken")))
  , recoVertexToken_(consumes<std::vector<reco::Vertex>>    (iConfig.getParameter<edm::InputTag>("recoVertexToken")))
  , recoTauToken_   (consumes<std::vector<pat::Tau>>        (iConfig.getParameter<edm::InputTag>("recoTauToken")))
  , recoGMTauToken_ (consumes<pat::TauRefVector>            (iConfig.getParameter<edm::InputTag>("recoGMTauToken")))
  , hltTauToken_    (consumes<std::vector<reco::PFTau>>     (iConfig.getParameter<edm::InputTag>("hltTauToken")))
  , hltTauSumChargedIsoToken_ (consumes<reco::PFTauDiscriminator> (iConfig.getParameter<edm::InputTag>("hltTauSumChargedIsoToken")))
{
   //now do what ever initialization is needed
  treeName_             = iConfig.getParameter<std::string>("treeName");
  edm::Service<TFileService> fs;
  tree_                 = fs -> make<TTree>(treeName_.c_str(), treeName_.c_str());
  createHistRoorFile_   = iConfig.getUntrackedParameter<bool>("createHistRoorFile", false);
  histRootFileName_     = iConfig.getParameter<std::string>("histRootFileName");
  histRootFile_         = new TFile(histRootFileName_.c_str(), "RECREATE");
  return;
}


HLTTauAnalyzer::~HLTTauAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HLTTauAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  Initialize();

  if(debug_){
    std::cout<<" Starting HLTTau Analyzer ............     "<< std::endl;  
  }
    using namespace edm;

   indexevents_ = iEvent.id().event();
   runNumber_ = iEvent.id().run();
   lumi_ = iEvent.luminosityBlock();

   edm::Handle<GenEventInfoProduct>        genEvt;
   try {iEvent.getByToken(genTagToken_,    genEvt);}
   catch (...) {;}
   if(genEvt.isValid()) MC_weight_ = genEvt->weight();

   edm::Handle<std::vector<reco::GenJet>>  genTauHandle;
   iEvent.getByToken(genTauToken_,         genTauHandle);

   for(auto genTau : *genTauHandle){
     if (fabs(genTau.eta())>max_eta_)
       continue;
     //if (fabs(genTau.pt())<min_pt_)
     //continue;
     genTauPt_.push_back(genTau.pt());
     genTauEta_.push_back(genTau.eta());
     genTauPhi_.push_back(genTau.phi());
     genTauCharge_.push_back(genTau.charge());

     hist_genTauPt_->Fill(genTau.pt());
     hist_genTauEta_->Fill(genTau.eta());
     hist_genTauPhi_->Fill(genTau.phi());

     if(debug_){
       std::cout<<" GenTau pt "<<genTau.pt()<<" eta "<< genTau.eta()<<" phi "<< genTau.phi()<<" charge "<< genTau.charge()<<std::endl;
     }
   }


   edm::Handle<std::vector<reco::PFTau>>      hltTauHandle;
   iEvent.getByToken(hltTauToken_,            hltTauHandle);

   edm::Handle<reco::PFTauDiscriminator>        hltTauSumChargedIso;
   iEvent.getByToken(hltTauSumChargedIsoToken_, hltTauSumChargedIso);

   size_t numHLTTaus = hltTauHandle->size();
   for (size_t idxHLTTau=0; idxHLTTau<numHLTTaus; ++idxHLTTau){
     reco::PFTauRef hltTau(hltTauHandle, idxHLTTau);
     hltTauPt_.push_back(hltTau->pt());
     hltTauEta_.push_back(hltTau->eta());
     hltTauPhi_.push_back(hltTau->phi());
     hltTauCharge_.push_back(hltTau->charge());
     double sumChargedIso = (*hltTauSumChargedIso)[hltTau];
     hltTauIso_.push_back(sumChargedIso);
     /*
     hltTauType_.push_back(hltTau->tauType());
     hltTauTightIso_.push_back(hltTau->passTightIso());
     hltTauMediumIso_.push_back(hltTau->passMediumIso());
     hltTauLooseIso_.push_back(hltTau->passLooseIso());
     hltTauVLooseIso_.push_back(hltTau->passVLooseIso());
     */
     if(hltTau->pt()!=0)
       {
         if(sumChargedIso/hltTau->pt() < 0.40)
           {
             hltTauVLooseRelIso_.push_back(true);
           }
         else
           {
             hltTauVLooseRelIso_.push_back(false);
           }
         if(sumChargedIso/hltTau->pt() < 0.20)
           {
             hltTauLooseRelIso_.push_back(true);
           }
         else
           {
             hltTauLooseRelIso_.push_back(false);
           }
         if(sumChargedIso/hltTau->pt() < 0.10)
           {
             hltTauMediumRelIso_.push_back(true);
           }
         else
           {
             hltTauMediumRelIso_.push_back(false);
           }
         if(sumChargedIso/hltTau->pt() < 0.05)
           {
             hltTauTightRelIso_.push_back(true);
           }
         else
           {
             hltTauTightRelIso_.push_back(false);
           }
       }
     double hltTauZ = 1000;
     double hltTauLeadTrackPt = 0;
     if (hltTau->leadPFChargedHadrCand().isNonnull() && hltTau->leadPFChargedHadrCand()->bestTrack()){
       hltTauZ = hltTau->leadPFChargedHadrCand()->bestTrack()->vertex().z();
       hltTauLeadTrackPt = hltTau->leadPFChargedHadrCand()->bestTrack()->pt();
     }
     hltTauZ_.push_back(hltTauZ);
     hltTauLeadTrackPt_.push_back(hltTauLeadTrackPt);

     hist_hltTauPt_->Fill(hltTau->pt());
     hist_hltTauEta_->Fill(hltTau->eta());
     hist_hltTauPhi_->Fill(hltTau->phi());
     if(debug_){
       std::cout<<" hltTau pt "<<hltTau->pt()<<" eta "<< hltTau->eta()<<" phi "<< hltTau->phi()<<" charge "<< hltTau->charge()<<std::endl;
       std::cout<<" hltTau Z "<< hltTauZ <<std::endl;
     }
   }

   if(isReco_){

     edm::Handle<std::vector<pat::Tau>>      recoTauHandle;
     iEvent.getByToken(recoTauToken_,        recoTauHandle);

     edm::Handle<pat::TauRefVector>          recoGMTauHandle;
     iEvent.getByToken(recoGMTauToken_,      recoGMTauHandle);

     for(auto recoTau : *recoTauHandle){
       if (fabs(recoTau.eta())>max_eta_)
	 continue;
       recoTauPt_.push_back(recoTau.pt());
       recoTauEta_.push_back(recoTau.eta());
       recoTauPhi_.push_back(recoTau.phi());
       recoTauCharge_.push_back(recoTau.charge());
       recoTauDecayMode_.push_back(recoTau.decayMode());

       hist_recoTauPt_->Fill(recoTau.pt());
       hist_recoTauEta_->Fill(recoTau.eta());
       hist_recoTauPhi_->Fill(recoTau.phi());
       
       if(debug_){
	 std::cout<<" RecoTau pt "<<recoTau.pt()<<" eta "<< recoTau.eta()<<" phi "<< recoTau.phi()<<" charge "<< recoTau.charge()<<" DecayMode "<< recoTau.decayMode()<<std::endl;
       }
     } //for(auto recoTau : *recoTauHandle){

     for(auto recoGMTau : *recoGMTauHandle){
       if (fabs(recoGMTau->eta())>max_eta_)
	 continue;
       recoGMTauPt_.push_back(recoGMTau->pt());
       recoGMTauEta_.push_back(recoGMTau->eta());
       recoGMTauPhi_.push_back(recoGMTau->phi());
       recoGMTauCharge_.push_back(recoGMTau->charge());
       recoGMTauDecayMode_.push_back(recoGMTau->decayMode());

       hist_recoGMTauPt_->Fill(recoGMTau->pt());
       hist_recoGMTauEta_->Fill(recoGMTau->eta());
       hist_recoGMTauPhi_->Fill(recoGMTau->phi());

       if(debug_){
	 std::cout<<" RecoGMTau pt "<<recoGMTau->pt()<<" eta "<< recoGMTau->eta()<<" phi "<< recoGMTau->phi()<<" charge "<< recoGMTau->charge()<<" DecayMode "<< recoGMTau->decayMode()<<std::endl;
       }
     } //for(auto recoGMTau : *recoGMTauHandle){
   } //if(isReco_){







   tree_ -> Fill();
}

void HLTTauAnalyzer::Initialize() {
  indexevents_ = 0;
  runNumber_ = 0;
  lumi_ = 0;
  MC_weight_ = 1;
  genTauPt_ .clear();
  genTauEta_ .clear();
  genTauPhi_ .clear();
  genTauCharge_ .clear();
  isGenMatched_ .clear();
  recoTauPt_ .clear();
  recoTauEta_ .clear();
  recoTauPhi_ .clear();
  recoTauCharge_ .clear();
  isRecoMatched_ .clear();
  recoTauDecayMode_ .clear();
  recoGMTauPt_ .clear();
  recoGMTauEta_ .clear();
  recoGMTauPhi_ .clear();
  recoGMTauCharge_ .clear();
  isRecoGMMatched_ .clear();
  recoGMTauDecayMode_ .clear();
  hltTauPt_ .clear();
  hltTauEta_ .clear();
  hltTauPhi_ .clear();
  hltTauCharge_ .clear();
  hltTauType_ .clear();
  hltTauIso_ .clear();
  hltTauTightIso_ .clear();
  hltTauMediumIso_ .clear();
  hltTauLooseIso_ .clear();
  hltTauVLooseIso_ .clear();
  hltTauTightRelIso_ .clear();
  hltTauMediumRelIso_ .clear();
  hltTauLooseRelIso_ .clear();
  hltTauVLooseRelIso_ .clear();
  hltTauZ_ .clear();
  hltTauLeadTrackPt_.clear();
  hltTauType_ .clear();
  genVertex_ = 0;
}


// ------------ method called once each job just before starting event loop  ------------
void
HLTTauAnalyzer::beginJob()
{
  tree_ -> Branch("EventNumber",&indexevents_,"EventNumber/l");
  tree_ -> Branch("RunNumber",&runNumber_,"RunNumber/I");
  tree_ -> Branch("lumi",&lumi_,"lumi/I");
  tree_ -> Branch("MC_weight",&MC_weight_,"MC_weight/F");
  tree_ -> Branch("genTauPt",  &genTauPt_);
  tree_ -> Branch("genTauEta", &genTauEta_);
  tree_ -> Branch("genTauPhi", &genTauPhi_);
  tree_ -> Branch("genTauCharge", &genTauCharge_);
  tree_ -> Branch("isGenMatched", &isGenMatched_);
  tree_ -> Branch("recoTauPt",  &recoTauPt_);
  tree_ -> Branch("recoTauEta", &recoTauEta_);
  tree_ -> Branch("recoTauPhi", &recoTauPhi_);
  tree_ -> Branch("recoTauCharge", &recoTauCharge_);
  tree_ -> Branch("isRecoMatched", &isRecoMatched_);
  tree_ -> Branch("recoTauDecayMode", &recoTauDecayMode_);
  tree_ -> Branch("recoGMTauPt",  &recoGMTauPt_);
  tree_ -> Branch("recoGMTauEta", &recoGMTauEta_);
  tree_ -> Branch("recoGMTauPhi", &recoGMTauPhi_);
  tree_ -> Branch("recoGMTauCharge", &recoGMTauCharge_);
  tree_ -> Branch("isRecoGMMatched", &isRecoGMMatched_);
  tree_ -> Branch("recoGMTauDecayMode", &recoGMTauDecayMode_);
  tree_ -> Branch("hltTauPt",  &hltTauPt_);
  tree_ -> Branch("hltTauEta", &hltTauEta_);
  tree_ -> Branch("hltTauPhi", &hltTauPhi_);
  tree_ -> Branch("hltTauCharge", &hltTauCharge_);
  tree_ -> Branch("hltTauType", &hltTauType_);
  tree_ -> Branch("hltTauIso", &hltTauIso_);
  tree_ -> Branch("hltTauTightIso", &hltTauTightIso_);
  tree_ -> Branch("hltTauMediumIso", &hltTauMediumIso_);
  tree_ -> Branch("hltTauLooseIso", &hltTauLooseIso_);
  tree_ -> Branch("hltTauVLooseIso", &hltTauVLooseIso_);
  tree_ -> Branch("hltTauTightRelIso", &hltTauTightRelIso_);
  tree_ -> Branch("hltTauMediumRelIso", &hltTauMediumRelIso_);
  tree_ -> Branch("hltTauLooseRelIso", &hltTauLooseRelIso_);
  tree_ -> Branch("hltTauVLooseRelIso", &hltTauVLooseRelIso_);
  tree_ -> Branch("hltTauZ", &hltTauZ_);
  tree_ -> Branch("hltTauLeadTrackPt",  &hltTauLeadTrackPt_);
  tree_ -> Branch("hltTauType", &hltTauType_);

  hist_genTauPt_ = new TH1F("genTauPt","genTauPt", 100, 0., 1000.);
  hist_genTauEta_ = new TH1F("genTauEta","genTauEta",50, -3., 3.);
  hist_genTauPhi_ = new TH1F("genTauPhi","genTauPhi",50, -3., 3.);
  hist_isGenMatched_ = new TH1F("isGenMatched","isGenMatched", 3, -1., 2.);
  hist_recoTauPt_ = new TH1F("recoTauPt","recoTauPt", 100, 0., 1000.);
  hist_recoTauEta_ = new TH1F("recoTauEta","recoTauEta",50, -3., 3.);
  hist_recoTauPhi_ = new TH1F("recoTauPhi","recoTauPhi",50, -3., 3.);
  hist_isRecoMatched_ = new TH1F("isRecoMatched","isRecoMatched", 3, -1., 2.);
  hist_recoGMTauPt_ = new TH1F("recoGMTauPt","recoGMTauPt", 100, 0., 1000.);
  hist_recoGMTauEta_ = new TH1F("recoGMTauEta","recoGMTauEta",50, -3., 3.);
  hist_recoGMTauPhi_ = new TH1F("recoGMTauPhi","recoGMTauPhi",50, -3., 3.);
  hist_isRecoGMMatched_ = new TH1F("isRecoGMMatched","isRecoGMMatched", 3, -1., 2.);
  hist_hltTauPt_ = new TH1F("hltTauPt","hltTauPt", 100, 0., 1000.);
  hist_hltTauEta_ = new TH1F("hltTauEta","hltTauEta",50, -3., 3.);
  hist_hltTauPhi_ = new TH1F("hltTauPhi","hltTauPhi",50, -3., 3.);
  hist_hltTauReso_vs_Gen_ = new TH1F("hltTauReso_vs_Gen","hltTauReso_vs_Gen", 60, 0., 3.);
  hist_hltTauReso_vs_Reco_ = new TH1F("hltTauReso_vs_Reco","hltTauReso_vs_Reco", 60, 0., 3.);
  hist_hltTauReso_vs_RecoGM_ = new TH1F("hltTauReso_vs_RecoGM","hltTauReso_vs_RecoGM", 60, 0., 3.);


  return;
}

// ------------ method called once each job just after ending the event loop  ------------
void
HLTTauAnalyzer::endJob()
{
  if(createHistRoorFile_){
    histRootFile_->cd();

    hist_genTauPt_->Write();
    hist_genTauEta_->Write();
    hist_genTauPhi_->Write();
    hist_isGenMatched_->Write();
    hist_recoTauPt_->Write();
    hist_recoTauEta_->Write();
    hist_recoTauPhi_->Write();
    hist_isRecoMatched_->Write();
    hist_recoGMTauPt_->Write();
    hist_recoGMTauEta_->Write();
    hist_recoGMTauPhi_->Write();
    hist_isRecoGMMatched_->Write();
    hist_hltTauPt_->Write();  
    hist_hltTauEta_->Write();
    hist_hltTauPhi_->Write();
    hist_hltTauReso_vs_Gen_->Write();
    hist_hltTauReso_vs_Reco_->Write();
    hist_hltTauReso_vs_RecoGM_->Write();

  //  histRootFile_->Write();
    histRootFile_->Close();
  }


  return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTTauAnalyzer);
