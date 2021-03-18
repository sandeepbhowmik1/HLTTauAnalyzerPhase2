import os, subprocess, sys


# ----------- *** Start Modification *** -------------------------------------

tagNTuple = '20210211'
#tagRootTree = '20210208'
#tagRootTree = 'DeepTau_20210208'
#tagRootTree = 'ChargedIso_20210208'
#tagRootTree = 'VBFHToTauTau_ChargedIso_20210208'
#tagRootTree = 'MinBias_ChargedIso_20210208'
#tagRootTree = 'QCD20to30_ChargedIso_20210208' 
#tagRootTree = 'QCD30to50_ChargedIso_20210208'
#tagRootTree = 'QCD50to80_ChargedIso_20210208'
#tagRootTree = 'QCD80to120_ChargedIso_20210208'
#tagRootTree = 'QCD120to170_ChargedIso_20210208'
#tagRootTree = 'QCD170to300_ChargedIso_20210208'
#tagRootTree = 'QCD300to470_ChargedIso_20210208'
#tagRootTree = 'QCD470to600_ChargedIso_20210208'
#tagRootTree = 'QCD600toInf_ChargedIso_20210208'
#tagRootTree = 'DYToLL_ChargedIso_20210208'
#tagRootTree = 'WJetsToLNu_ChargedIso_20210208'
tagRootTree = 'TT_ChargedIso_20201219'

pathCrab_Signal = '/cms/store/user/sbhowmik/VBFHToTauTau_M125_14TeV_powheg_pythia8_correctedGridpack_tuneCP5/VBFHToTauTau_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'

#pathCrab_Background = '/cms/store/user/sbhowmik/MinBias_TuneCP5_14TeV-pythia8/MinBias_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_20to30_TuneCP5_14TeV_pythia8/QCDPt20to30_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_30to50_TuneCP5_14TeV_pythia8/QCDPt30to50_Phase2HLTTDRSummer20ReRECOMiniAOD_withNewMB_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_50to80_TuneCP5_14TeV_pythia8/QCDPt50to80_Phase2HLTTDRSummer20ReRECOMiniAOD_withNewMB_CMSSW_1117_'+tagNTuple+'/*/*' 
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_80to120_TuneCP5_14TeV_pythia8/QCDPt80to120_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_120to170_TuneCP5_14TeV_pythia8/QCDPt120to170_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_170to300_TuneCP5_14TeV_pythia8/QCDPt170to300_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_300to470_TuneCP5_14TeV_pythia8/QCDPt300to470_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_470to600_TuneCP5_14TeV_pythia8/QCDPt470to600_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/QCD_Pt_600oInf_TuneCP5_14TeV_pythia8/QCDPt600toInf_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/DYToLL_M-50_TuneCP5_14TeV-pythia8/DYJetsToLL_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
#pathCrab_Background = '/cms/store/user/sbhowmik/WJetsToLNu_TuneCP5_14TeV-amcatnloFXFX-pythia8/WJetsToLNu_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'
pathCrab_Background = '/cms/store/user/sbhowmik/TT_TuneCP5_14TeV-powheg-pythia8/TT_Phase2HLTTDRSummer20ReRECOMiniAOD_CMSSW_1117_'+tagNTuple+'/*/*'

pathRootTree = '/home/sbhowmik/RootTree/HLTTau/Phase2/'

workingDir = os.getcwd()

#sampleType=["Signal", "Background"]
#sampleType=["Signal"]
sampleType=["Background"] 
#algoType=["HLTTau"]
algoType=["L1andHLTTau"]
#fileType=["rootTree", "bdt", "hist"] 
fileType=["rootTree"]



# ------------ *** End Modification *** --------------------------------------



# ------------ Define command to execute -------------------------------------
def run_cmd(command):
  print "executing command = '%s'" % command
  p = subprocess.Popen(command, shell = True, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
  stdout, stderr = p.communicate()
  return stdout




# -----------Create file list of crab output root files ------------
'''
run_cmd('rm %s' % "*list")
scriptFile = os.path.join(workingDir, "script", "create_fileList_NTuple.sh")
run_cmd('bash %s %s %s' % (scriptFile, pathCrab_Signal, pathCrab_Background))

# -----------Create cfg file to run analyzer ------------

run_cmd('rm %s' % "test_*.py")
for i in range (0, len(sampleType)):
  for j in range (0, len(algoType)):
    scriptFile = os.path.join(workingDir, "script", "create_test_TauAnalyzer.sh")
    run_cmd('bash %s %s %s %s' % (scriptFile, tagRootTree, sampleType[i], algoType[j]))

# -----------cmsRun analyzer files -----------.   

run_cmd('rm %s' % "*.root")
for i in range (0, len(sampleType)):
  for j in range (0, len(algoType)):
    scriptFile = os.path.join(workingDir, "submit_jobs_cmsRun_"+algoType[j]+"_"+sampleType[i]+".sh")
    run_cmd('bash %s' % scriptFile)


# -----------manage output files -----------.
'''
for i in range (0, len(sampleType)):
  for j in range (0, len(algoType)):
    for k in range (0, len(fileType)):
      rootFiles=os.path.join(fileType[k]+"_test_"+algoType[j]+"Analyzer_"+sampleType[i]+"_"+tagRootTree+"_part_*.root")
      haddFile=os.path.join(fileType[k]+"_test_"+algoType[j]+"Analyzer_"+sampleType[i]+"_"+tagRootTree+".root")
      run_cmd('rm %s' % haddFile)
      run_cmd('hadd %s %s' % (haddFile, rootFiles))
      run_cmd('mv %s %s' % (haddFile, pathRootTree))

run_cmd('rm %s' % "*.root")
testDir = os.path.join(workingDir, "test")
run_cmd('mv %s %s' % ("*.list", testDir))
run_cmd('mv %s %s' % ("test_*.py", testDir))
run_cmd('mv %s %s' % ("submit_jobs_cmsRun*.sh", testDir))
run_cmd('rm %s' % "*.log")







#hadd rootTree_test_L1andHLTTauAnalyzer_Background_DeepTau_Stitching_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_MinBias_DeepTau_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD30to50_DeepTau_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD50to80_DeepTau_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD80to120_DeepTau_20210208.root  rootTree_test_L1andHLTTauAnalyzer_Background_QCD120to170_DeepTau_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD170to300_DeepTau_20210208.root

#hadd rootTree_test_L1andHLTTauAnalyzer_Background_ChargedIso_Stitching_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_MinBias_ChargedIso_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD30to50_ChargedIso_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD50to80_ChargedIso_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD80to120_ChargedIso_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD120to170_ChargedIso_20210208.root rootTree_test_L1andHLTTauAnalyzer_Background_QCD170to300_ChargedIso_20210208.root 
