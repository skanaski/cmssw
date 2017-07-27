#ifndef RecoHi_HiEgammaAlgos_HiEgammaBkgSubAlgo
#define RecoHi_HiEgammaAlgos_HiEgammaBkgSubAlgo

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include <set>

class HiEgammaBkgSubAlgo{
 public:

  HiEgammaBkgSubAlgo() {}
  
 ~HiEgammaBkgSubAlgo() {}

 reco::BasicClusterCollection correctEnergies(reco::CaloClusterPtrVector & clusters, const EcalRecHitCollection* hits, bool excludeClusters, bool isBarrel);

 private:

  reco::CaloClusterPtrVector clusters_;

  reco::BasicCluster correctBarrelEnergy(const edm::Ptr<reco::CaloCluster> cluster_p,const EcalRecHitCollection* hits, bool excludeClusters, std::set<DetId> usedId);
  reco::BasicCluster correctEndcapEnergy(const edm::Ptr<reco::CaloCluster> cluster_p,const EcalRecHitCollection* hits, bool excludeClusters, std::set<DetId> usedId);

};

#endif
