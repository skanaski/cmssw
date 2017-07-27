import FWCore.ParameterSet.Config as cms

# Corrects energies of basic clusters (currently only in barrel; endcap may be added later)
corrIslandBasicClusters = cms.EDProducer("HiEgammaBkgSubtraction",
                                         excludeClusters=cms.bool(True),
                                         barrelHits=cms.InputTag('ecalRecHit','EcalRecHitsEB'),
                                         endcapHits=cms.InputTag('ecalRecHit','EcalRecHitsEE'),
                                         barrelClusterCollection=cms.string('islandBarrelBasicClusters'),
                                         endcapClusterCollection=cms.string('islandEndcapBasicClusters'),
                                         barrelClusterProducer=cms.string('islandBasicClusters'),
                                         endcapClusterProducer=cms.string('islandBasicClusters'),
                                         barrelCorrClusterCollection=cms.string('islandBarrelCorrBasicClusters'),
                                         endcapCorrClusterCollection=cms.string('islandEndcapCorrBasicClusters')
)
