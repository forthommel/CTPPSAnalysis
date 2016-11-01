import FWCore.ParameterSet.Config as cms

process = cms.Process('Demo')

process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

#process.load('EventFilter.CTPPSRawToDigi.totemTriggerRawToDigi_cfi')

#process.load("EventFilter.CTPPSRawToDigi.totemRawToDigi_cff")

process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

#process.load('EventFilter.CTPPSRawToDigi.ctppsDiamondRawToDigi_cfi')
#process.ctppsDiamondRawToDigi.rawDataTag = cms.InputTag('rawDataCollector')
#process.ctppsDiamondRawToDigi.verbosity = cms.untracked.uint32(0)

#process.load('CondFormats.CTPPSReadoutObjects.TotemDAQMappingESSourceXML_cfi')
#process.TotemDAQMappingESSourceXML.mappingFileNames.append("CondFormats/CTPPSReadoutObjects/xml/ctpps_timing_diamond_215_mapping.xml")
#process.load('CondFormats.CTPPSReadoutObjects.DiamondDAQMappingESSourceXML_cfi')
#process.DiamondDAQMappingESSourceXML.mappingFileNames.append("CondFormats/CTPPSReadoutObjects/xml/ctpps_timing_diamond_215_mapping.xml")

process.load('CTPPSAnalysis.TreeProducer.treeProducer_cfi')

# raw data source
#process.source = cms.Source('NewEventStreamFileReader',
process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(
#        '/store/t0streamer/Minidaq/A/000/283/272/run283272_ls0001_streamA_StorageManager.dat')
#        '/store/t0streamer/Data/Physics/000/283/883/run283883_ls0124_streamPhysics_StorageManager.dat',
#        '/store/t0streamer/Data/Express/000/283/877/run283877_ls0400_streamExpress_StorageManager.dat',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/4EE44B0E-2499-E611-A155-02163E011938.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/4EFC1BAD-1A99-E611-AE33-02163E0133A8.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/500AEDFA-3199-E611-B113-FA163EEEACE8.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/50518DE8-1199-E611-B115-02163E01368D.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/508F899F-2A99-E611-A3F9-02163E011E4A.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/50A76128-2A99-E611-9728-FA163E7731F1.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/52059B20-2A99-E611-94FC-02163E011AAE.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/520FF010-1299-E611-AE8B-02163E011B51.root',
'/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/283/877/00000/5217A0A5-1A99-E611-A2BF-02163E01436A.root',

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
#    process.totemTriggerRawToDigi
#    * process.totemRPRawToDigi
     process.ctppsRawToDigi
    * process.recoCTPPS
#    * process.ctppsDiamondRawToDigi
    * process.ctppsTreeProducer
)

#process.e = cms.EndPath( process.out )
