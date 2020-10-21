# HLTTauAnalyzerPhase2


cmsrel CMSSW_11_1_3

cd CMSSW_11_1_3/src

cmsenv

git cms-init

git cms-merge-topic -u cms-l1t-offline:l1t-phase2-v3.1.9

scram b -j 8



# To Analyze HLT Tau

git clone https://github.com/sandeepbhowmik1/HLTTauAnalyzerPhase2


# To get dataformat of HLT Tau

git clone https://github.com/HEP-KBFI/dataformats-phase2hltpftaus $CMSSW_BASE/src/DataFormats/Phase2HLTPFTaus


# Tallinn-internal code for Phase-2 HLT trigger studies

git clone https://github.com/veelken/hlttrigger-phase2hltpftauanalyzer $CMSSW_BASE/src/HLTrigger/TallinnHLTPFTauAnalyzer

cp -r /home/veelken/Phase2HLT/CMSSW_11_1_0/src/HLTrigger/TallinnHLTPFTauAnalyzer/data $CMSSW_BASE/src/HLTrigger/TallinnHLTPFTauAnalyzer/data

git clone https://github.com/veelken/hlttrigger-deeptautraining $CMSSW_BASE/src/HLTrigger/DeepTauTraining


# fix for RecoPFTauQualityCuts to allow maxDeltaZToLeadTrack Used

git cms-merge-topic veelken:CMSSW_11_1_x_phase2HLT


# To get dataformat of L1 HPS Tau

git clone https://github.com/sandeepbhowmik1/L1TauProducerPhase2-DataFormats $CMSSW_BASE/src/DataFormats/Phase2L1Taus


# Modification To compile for L1 Tau

git cms-addpkg DataFormats/L1TCorrelator

cp /home/sbhowmik/L1TauTrigger/L1TauProducerPhase2/CMSSW_11_1_2/src/DataFormats/L1TCorrelator/interface/TkPrimaryVertex.h $CMSSW_BASE/src/DataFormats/L1TCorrelator/interface

cp /home/sbhowmik/L1TauTrigger/L1TauProducerPhase2/CMSSW_11_1_2/src/DataFormats/L1TCorrelator/src/classes_def.xml $CMSSW_BASE/src/DataFormats/L1TCorrelator/src

git cms-addpkg DataFormats/L1Trigger

cp /home/sbhowmik/L1TauTrigger/L1TauProducerPhase2/CMSSW_11_1_2/src/DataFormats/L1Trigger/interface/Muon.h $CMSSW_BASE/src/DataFormats/L1Trigger/interface

cp /home/sbhowmik/L1TauTrigger/L1TauProducerPhase2/CMSSW_11_1_2/src/DataFormats/L1Trigger/src/classes_def.xml $CMSSW_BASE/src/DataFormats/L1Trigger/src


scram b -j 8


