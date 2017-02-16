import FWCore.ParameterSet.Config as cms

process = cms.Process('Demo')

process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

#process.load('CTPPSValidation.TreeProducer.treeProducer_cfi')
process.load('CTPPSValidation.TreeProducer.diamondsTreeProducer_cfi')

# raw data source
process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(
 'root://eoscms.cern.ch:1094//eos/totem/data/ctpps/run284036.root',
    )
)

# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('output.root'),
    closeFileFast = cms.untracked.bool(True)
)

#process.out = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('myOutputFile.root'),
#    outputCommands = cms.untracked.vstring(
#        'drop *',
#        'keep *_*RawToDigi_*_*',
#    ),
#)

process.p = cms.Path(
    process.recoCTPPS
    #* process.ctppsTreeProducer
    * process.ctppsDiamondsTreeProducer
)

#process.e = cms.EndPath( process.out )
