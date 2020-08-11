#!/bin/sh
## set tagRootTree value from crab submit jobs and then ./create_fileList_test_TauAnalyzer.sh tagRootTree sampleTypes algoTypes

tagRootTree=$1
sampleTypes=$2
algoTypes=$3
#tagRootTree=20190505
#sampleTypes=(Signal  Background)
#algoTypes=(L1TkElectronHPSPFTauAnalyzer L1TkMuonHPSPFTauAnalyzer)
#algoTypes=(L1HPSPFTauAnalyzer L1PFTauAnalyzer)
#algoTypes=(L1HPSPFTauAnalyzer L1NNTauAnalyzer)

for sampleType in ${sampleTypes[@]}
do

    for algoType in ${algoTypes[@]}
    do
        jobSubmitFile=submit_jobs_cmsRun_${algoType}_${sampleType}.sh
        rm $jobSubmitFile
        echo \#!/bin/sh | cat >>$jobSubmitFile
	
	fileList=fileList_NTuple_${sampleType}.list
	count1=0
	while read -r line; do
	    ((count1+=1))
	done < $fileList
	echo $count1
	nLineToAFile=250
	nFileLine=$(($count1 / $nLineToAFile))
	nFileLine=$(($nFileLine+1))
	echo $nFileLine

	for ((i_fileLine=1; i_fileLine<=$nFileLine; i_fileLine++))
	do
	    echo $i_fileLine
	    lineStart=$(($nLineToAFile*($i_fileLine-1)))
	    echo $lineStart
	    lineEnd=$(($nLineToAFile*$i_fileLine))
	    echo $lineEnd

	    #for algoType in ${algoTypes[@]}
	    #do
 		#jobSubmitFile=submit_jobs_cmsRun_${algoType}_${sampleType}.sh
		#rm $jobSubmitFile
		#echo \#!/bin/sh | cat >>$jobSubmitFile


           fileOut=test_${algoType}_${sampleType}_part_${i_fileLine}.py

	    rm $fileOut
	    echo $fileOut
	    
	    echo "import FWCore.ParameterSet.Config as cms" | cat >>$fileOut
	    
	    echo "process = cms.Process('Analyze')" | cat >>$fileOut
	    
	    echo "process.maxEvents = cms.untracked.PSet(" | cat >>$fileOut
	    printf "\t" test  | cat >>$fileOut
	    echo "input = cms.untracked.int32(-1)" | cat >>$fileOut
	    printf "\t" test  | cat >>$fileOut
	    echo ")" | cat >>$fileOut
	    
	    echo "process.source = cms.Source(\"PoolSource\"," | cat >>$fileOut
	    printf "\t" test  | cat >>$fileOut
	    echo "fileNames = cms.untracked.vstring(" | cat >>$fileOut
	    
	    
	    echo $i_fileLine
	    lineStart=$(($nLineToAFile*($i_fileLine-1)))
	    echo $lineStart
	    lineEnd=$(($nLineToAFile*$i_fileLine))
	    echo $lineEnd
	    count2=0
	    while read -r line; do
		((count2+=1))
		if [ $count2 -gt $lineStart ] && [ $count2 -le $lineEnd ]; then
		    dataset=$line

		    printf "\t" test  | cat >>$fileOut
		    echo "'file:$dataset'," | cat >>$fileOut
		else
		    continue
		fi
	    done < $fileList
	    
	    #while read -r line; do
		#dataset=$line
		#printf "\t" test  | cat >>$fileOut
		#echo "'file:$dataset'," | cat >>$fileOut
	    #done < $fileList
	    
	    printf "\t" test  | cat >>$fileOut
            echo ")" | cat >>$fileOut
	    echo ")" | cat >>$fileOut
	    
	    echo "process.analysisSequence = cms.Sequence()" | cat >>$fileOut 
	    echo "process.load(\"PhysicsTools.JetMCAlgos.TauGenJets_cfi\")" | cat >>$fileOut 
	    echo "process.tauGenJets.GenParticles = cms.InputTag('prunedGenParticles')" | cat >>$fileOut 
	    echo "process.analysisSequence += process.tauGenJets" | cat >>$fileOut 
	    
	    echo "process.load(\"PhysicsTools.JetMCAlgos.TauGenJetsDecayModeSelectorAllHadrons_cfi\")" | cat >>$fileOut 
	    echo "process.analysisSequence += process.tauGenJetsSelectorAllHadrons" | cat >>$fileOut 

	    echo "process.load(\"HLTTauAnalyzerPhase2.HLTTauAnalyzerPhase2.${algoType}Analyzer_cff\")" | cat >>$fileOut

	    echo "process.${algoType}Analyzer.histRootFileName = cms.string(\"hist_test_${algoType}Analyzer_${sampleType}_${tagRootTree}_part_${i_fileLine}.root\")" | cat >>$fileOut

	    echo "process.${algoType}Analyzer.bdtRootFileName = cms.string(\"bdt_test_${algoType}Analyzer_${sampleType}_${tagRootTree}_part_${i_fileLine}.root\")" | cat >>$fileOut
	    
	    echo "process.analysisSequence += process.AnalyzerSeq" | cat >>$fileOut

	    echo "process.p = cms.Path(" | cat >>$fileOut
	    printf "\t" test  | cat >>$fileOut
	    #echo "process.AnalyzerSeq"  | cat >>$fileOut
	    echo "process.analysisSequence"  | cat >>$fileOut
	    echo ")" | cat >>$fileOut
	    
	    echo "process.schedule = cms.Schedule(process.p)" | cat >>$fileOut
	    
	    echo "process.TFileService=cms.Service('TFileService',fileName=cms.string(\"rootTree_test_${algoType}Analyzer_${sampleType}_${tagRootTree}_part_${i_fileLine}.root\"))" | cat >>$fileOut

	    echo "cmsRun -p $fileOut > out_${algoType}_${sampleType}_part_${i_fileLine}.log &"  | cat >>$jobSubmitFile
	done
    done
done

chmod 755 $jobSubmitFile
