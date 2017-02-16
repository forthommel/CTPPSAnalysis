import FWCore.ParameterSet.Config as cms
process = cms.Process("CTPPS")

process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(
        'root://eoscms.cern.ch:1094//eos/totem/data/ctpps/run284036.root',
    ),
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# diamonds mapping
process.totemDAQMappingESSourceXML_TimingDiamond = cms.ESSource("TotemDAQMappingESSourceXML",
  verbosity = cms.untracked.uint32(0),
  subSystem = cms.untracked.string("TimingDiamond"),
  configuration = cms.VPSet(
    # before diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("1:min - 283819:max"),
      mappingFileNames = cms.vstring(),
      maskFileNames = cms.vstring()
    ),
    # after diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("283820:min - 999999999:max"),
      mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond.xml"),
      maskFileNames = cms.vstring()
    )
  )
)

# raw-to-digi conversion
process.load('EventFilter.CTPPSRawToDigi.ctppsDiamondRawToDigi_cfi')
process.ctppsDiamondRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

# rechits production
process.load('Geometry.VeryForwardGeometry.geometryRP_cfi')
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi')

# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('output.root'),
    closeFileFast = cms.untracked.bool(True)
)

# tree producer
process.load('CTPPSValidation.TreeProducer.diamondsTreeProducer_cfi')

# execution configuration
process.p = cms.Path(
    process.ctppsDiamondRawToDigi *
    process.ctppsDiamondRecHits *
    process.ctppsDiamondsTreeProducer
)

