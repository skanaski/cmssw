import FWCore.ParameterSet.Config as cms

# Corrects energies of basic clusters (currently only in barrel; endcap may be added later)
corrIslandBasicClusters = cms.EDProducer("HiEgammaEnergyCorrection",
                                         excludeClusters=cms.bool(True),
                                         barrelHits=cms.InputTag('ecalRecHit','EcalRecHitsEB'),
                                         barrelClusterCollection=cms.string('islandBarrelBasicClusters'),
                                         barrelClusterProducer=cms.string('islandBasicClusters'),
                                         barrelCorrClusterCollection=cms.string('islandBarrelCorrBasicClusters')
)
