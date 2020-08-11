import FWCore.ParameterSet.Config as cms

genMatchedTaus = cms.EDFilter("genMatchTauFilter",
        taus = cms.InputTag("slimmedTaus")
    )

goodTaus = cms.EDFilter("PATTauRefSelector",
        #src = cms.InputTag("slimmedTaus"),
        src = cms.InputTag("genMatchedTaus"),
        cut = cms.string(
        'pt > 20 && abs(eta) < 2.4 '
        '&& abs(charge) > 0 && abs(charge) < 2 '
        '&& tauID("decayModeFinding") > 0.5 '
        #'&& tauID("chargedIsoPtSum") < 2.5'
        '&& tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") > 0.5'
        #'&& tauID("byMediumIsolationMVArun2v1DBoldDMwLT") > 0.5 '
        #'&& tauID("againstMuonTight3") > 0.5 '
        #'&& tauID("againstElectronVLooseMVA6") > 0.5 '
        ),
        filter = cms.bool(False)
)

genVertexProducer = cms.EDProducer("GenVertexProducer",
  #src = cms.InputTag('prunedGenParticles'),
  src = cms.InputTag('genParticles'),
  pdgIds = cms.vint32(-15, +15) # CV: use { -15, +15 } for signal, empty list for background                                    
) 

HLTTauAnalyzer = cms.EDAnalyzer("HLTTauAnalyzer",
                                        debug              = cms.untracked.bool(False),
                                        isReco             = cms.untracked.bool(True),
                                        min_pt             = cms.untracked.double(0),
                                        max_eta            = cms.untracked.double(3.0),
                                        genTagToken        = cms.InputTag("generator"),
                                        genVertexToken     = cms.InputTag("genVertexProducer", "z0"),
                                        genTauToken        = cms.InputTag("tauGenJetsSelectorAllHadrons"),
                                        recoVertexToken    = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                        recoTauToken       = cms.InputTag("slimmedTaus"),
                                        recoGMTauToken     = cms.InputTag("goodTaus"),
                                        #hltTauToken        = cms.InputTag("hltHpsSelectedPFTausTrackPt1MediumChargedIsolationReg"),
                                        #hltTauToken        = cms.InputTag("hltSelectedHpsPFTausWithOfflineVertices"),
                                        #hltTauToken        = cms.InputTag("hltSelectedPFTausWithOfflineVertices"),
                                        #hltTauToken        = cms.InputTag("hltHpsPFTausPassingTrackPt8HitsMaxDeltaZWithOnlineVertices"),
                                        #hltTauToken        = cms.InputTag("hltSelectedHpsPFTaus8HitsMaxDeltaZWithOnlineVertices"),
                                        hltTauToken        = cms.InputTag("hltSelectedHpsPFTaus8HitsMaxDeltaZWithOfflineVertices"),
                                        #hltTauSumChargedIsoToken = cms.InputTag("hltHpsPFTauChargedIsoPtSumWithOfflineVertices"),
                                        #hltTauSumChargedIsoToken = cms.InputTag("hltPFTauChargedIsoPtSumWithOfflineVertices"),
                                        #hltTauSumChargedIsoToken = cms.InputTag("hltHpsPFTauChargedIsoPtSum8HitsMaxDeltaZWithOnlineVertices"),
                                        #hltTauSumChargedIsoToken = cms.InputTag("hltHpsPFTauChargedIsoPtSum8HitsMaxDeltaZWithOfflineVertices"),
                                        hltTauSumChargedIsoToken = cms.InputTag("hltSelectedHpsPFTauChargedIsoPtSum8HitsMaxDeltaZWithOfflineVertices"),
                                        treeName           = cms.string("HLTTauAnalyzer"),
                                        fillBDT            = cms.untracked.bool(True),
                                        bdtRootFileName    = cms.string("bdt_test_HLTTauAnalyzer.root"),
                                        treeBDTName        = cms.string("HLTTauAnalyzer"),
                                        createHistRoorFile = cms.untracked.bool(False),
                                        histRootFileName   = cms.string("hist_test_HLTTauAnalyzer.root"),
                                        applyBDT           = cms.untracked.bool(True),
                                        bdtInputFileName   = cms.string("HLTTauAnalyzerPhase2/HLTTauAnalyzerPhase2/data/L1HPSPFTau_XGB_testVars_default_6Var_20200304_4.xml"),  # _4
                                        )


AnalyzerSeq = cms.Sequence(
    #genVertexProducer +
    genMatchedTaus +
    goodTaus       +
    HLTTauAnalyzer
)
