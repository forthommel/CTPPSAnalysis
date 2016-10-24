import FWCore.ParameterSet.Config as cms

process = cms.Process('Demo')

process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load('EventFilter.CTPPSRawToDigi.totemTriggerRawToDigi_cfi')
process.totemTriggerRawToDigi.rawDataTag = cms.InputTag('rawDataCollector')

process.load('EventFilter.CTPPSRawToDigi.totemRPRawToDigi_cfi')
process.totemRPRawToDigi.rawDataTag = cms.InputTag('rawDataCollector')

process.load('EventFilter.CTPPSRawToDigi.diamondRPRawToDigi_cfi')
process.diamondRPRawToDigi.rawDataTag = cms.InputTag('rawDataCollector')
process.diamondRPRawToDigi.verbosity = cms.untracked.uint32(0)

process.load('CondFormats.CTPPSReadoutObjects.DiamondDAQMappingESSourceXML_cfi')
process.DiamondDAQMappingESSourceXML.mappingFileNames.append("CondFormats/CTPPSReadoutObjects/xml/ctpps_timing_diamond_215_mapping.xml")

process.load('CTPPSAnalysis.TreeProducer.treeProducer_cfi')

# raw data source
process.source = cms.Source('NewEventStreamFileReader',
    fileNames = cms.untracked.vstring(
#        '/store/t0streamer/Minidaq/A/000/283/272/run283272_ls0001_streamA_StorageManager.dat')
        '/store/t0streamer/Data/Physics/000/283/883/run283883_ls0124_streamPhysics_StorageManager.dat',
    )
)

# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('output.root'),
    closeFileFast = cms.untracked.bool(True)
)

process.p = cms.Path(
    process.totemTriggerRawToDigi
    * process.totemRPRawToDigi
    * process.diamondRPRawToDigi
    * process.ctppsTreeProducer
)
