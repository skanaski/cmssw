#ifndef RecoHi_HiEgammaAlgos_HiEgammaEnergyCorrection
#define RecoHi_HiEgammaAlgos_HiEgammaEnergyCorrection

#include <memory>
#include <time.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "RecoHI/HiEgammaAlgos/interface/HiEgammaEnergyCorrAlgo.h"


class HiEgammaEnergyCorrection : public edm::stream::EDProducer<>
{
 public:

  HiEgammaEnergyCorrection(const edm::ParameterSet& ps);

  ~HiEgammaEnergyCorrection();

  virtual void produce(edm::Event&, const edm::EventSetup&) override;

 private:

  std::string barrelCorrClusterCollection_;

  bool excludeClusters_;

  edm::EDGetTokenT<EcalRecHitCollection> barrelRecHits_;
  edm::EDGetTokenT<reco::BasicClusterCollection> barrelClustersToken;

  HiEgammaEnergyCorrAlgo* corrAlgo_p;

  void getClusterPtrVector(edm::Event& evt, const edm::EDGetTokenT<reco::BasicClusterCollection>& clustersToken, reco::CaloClusterPtrVector *);
  const EcalRecHitCollection* getCollection(edm::Event& evt, const edm::EDGetTokenT<EcalRecHitCollection>& token);

  void correctClusterEnergies(edm::Event& evt,
			      const edm::EDGetTokenT<EcalRecHitCollection>& rhc_token,
			      const edm::EDGetTokenT<reco::BasicClusterCollection>& clustersToken,
			      std::string corrClusterCollection);
};

#endif
