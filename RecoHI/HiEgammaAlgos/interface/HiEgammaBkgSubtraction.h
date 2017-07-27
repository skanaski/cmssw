#ifndef RecoHi_HiEgammaAlgos_HiEgammaBkgSubtraction
#define RecoHi_HiEgammaAlgos_HiEgammaBkgSubtraction

#include <memory>
#include <time.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "RecoHI/HiEgammaAlgos/interface/HiEgammaBkgSubAlgo.h"


class HiEgammaBkgSubtraction : public edm::stream::EDProducer<>
{
 public:

  HiEgammaBkgSubtraction(const edm::ParameterSet& ps);

  ~HiEgammaBkgSubtraction();

  virtual void produce(edm::Event&, const edm::EventSetup&) override;

 private:

  std::string barrelCorrClusterCollection_;
  std::string endcapCorrClusterCollection_;

  bool excludeClusters_;

  edm::EDGetTokenT<EcalRecHitCollection> barrelRecHits_;
  edm::EDGetTokenT<EcalRecHitCollection> endcapRecHits_;
  edm::EDGetTokenT<reco::BasicClusterCollection> barrelClustersToken;
  edm::EDGetTokenT<reco::BasicClusterCollection> endcapClustersToken;

  HiEgammaBkgSubAlgo* corrAlgo_p;

  void getClusterPtrVector(edm::Event& evt, const edm::EDGetTokenT<reco::BasicClusterCollection>& clustersToken, reco::CaloClusterPtrVector *);
  const EcalRecHitCollection* getCollection(edm::Event& evt, const edm::EDGetTokenT<EcalRecHitCollection>& token);

  void correctClusterEnergies(edm::Event& evt);
};

#endif
