import FWCore.ParameterSet.Config as cms

ctppsDiamondsTreeProducer = cms.EDAnalyzer('DiamondsTreeProducer',
    diamondDigiLabel = cms.InputTag('ctppsDiamondRawToDigi', 'TimingDiamond'),
    diamondRecHitLabel = cms.InputTag('ctppsDiamondRecHits'),
)
