#ifndef RecoHi_HiEgammaAlgos_HiEgammaEnergyCorrAlgo
#define RecoHi_HiEgammaAlgos_HiEgammaEnergyCorrAlgo

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include <set>

class HiEgammaEnergyCorrAlgo{
 public:

  HiEgammaEnergyCorrAlgo() {}
  
  ~HiEgammaEnergyCorrAlgo() {}

  reco::BasicClusterCollection correctEnergies(reco::CaloClusterPtrVector & clusters, const EcalRecHitCollection* hits, bool excludeClusters);

 private:

  reco::CaloClusterPtrVector clusters_;

  reco::BasicCluster correctEnergy(const edm::Ptr<reco::CaloCluster> cluster_p,const EcalRecHitCollection* hits, bool excludeClusters, std::set<DetId> usedId);

};

#endif
