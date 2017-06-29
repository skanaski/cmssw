// C/C++ headers
#include <iostream>
#include <vector>
#include <memory>

// Framework
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

// Reconstruction Classes
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

// Class header file
#include "RecoHI/HiEgammaAlgos/interface/HiEgammaEnergyCorrection.h"


HiEgammaEnergyCorrection::HiEgammaEnergyCorrection(const edm::ParameterSet& ps)
{
  barrelCorrClusterCollection_=ps.getParameter<std::string>("barrelCorrClusterCollection");
  produces<reco::BasicClusterCollection>(barrelCorrClusterCollection_);

  excludeClusters_=ps.getParameter<bool>("excludeClusters");

  barrelClustersToken=consumes<reco::BasicClusterCollection>(edm::InputTag(ps.getParameter<std::string>("barrelClusterProducer"),ps.getParameter<std::string>("barrelClusterCollection")));

  barrelRecHits_=consumes<EcalRecHitCollection>(ps.getParameter<edm::InputTag>("barrelHits"));
  
  corrAlgo_p=new HiEgammaEnergyCorrAlgo();
}

HiEgammaEnergyCorrection::~HiEgammaEnergyCorrection() {delete corrAlgo_p;}

void HiEgammaEnergyCorrection::produce(edm::Event& evt, const edm::EventSetup& es)
{
  correctClusterEnergies(evt, barrelRecHits_, barrelClustersToken, barrelCorrClusterCollection_);
}

void HiEgammaEnergyCorrection::getClusterPtrVector(edm::Event& evt, const edm::EDGetTokenT<reco::BasicClusterCollection>& clustersToken, reco::CaloClusterPtrVector * clusterPtrVector_p)
{  
  edm::Handle<reco::BasicClusterCollection> bccHandle;

  evt.getByToken(clustersToken, bccHandle);

  if (!(bccHandle.isValid()))
    {
      edm::LogError("HiSuperClusterProducerError") << "could not get a handle on the BasicCluster Collection!";
      clusterPtrVector_p = 0;
    }

  const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();
  for (unsigned int i = 0; i < clusterCollection_p->size(); i++)
    {
      clusterPtrVector_p->push_back(reco::CaloClusterPtr(bccHandle, i));
    }
}

const EcalRecHitCollection* HiEgammaEnergyCorrection::getCollection(edm::Event& evt, const edm::EDGetTokenT<EcalRecHitCollection>& token)
{
  edm::Handle<EcalRecHitCollection> rhcHandle;
  evt.getByToken(token, rhcHandle);
  return rhcHandle.product(); 
}

void HiEgammaEnergyCorrection::correctClusterEnergies(edm::Event& evt,
			    const edm::EDGetTokenT<EcalRecHitCollection>& rhc_token,
			    const edm::EDGetTokenT<reco::BasicClusterCollection>& clustersToken,
			    std::string corrClusterCollection)
{
  // Get rec hits
  const EcalRecHitCollection* rhc_p=getCollection(evt, rhc_token);

  // Get vector of basic clusters
  reco::CaloClusterPtrVector* clusterPtrVector_p=new reco::CaloClusterPtrVector;
  getClusterPtrVector(evt, clustersToken, clusterPtrVector_p);

  // Correct their energies
  reco::BasicClusterCollection corrClusterPtrVector;
  corrClusterPtrVector=corrAlgo_p->correctEnergies(*clusterPtrVector_p, rhc_p, excludeClusters_);

  // Make autopointer
  auto corrClusters=std::make_unique<reco::BasicClusterCollection>();
  corrClusters->assign(corrClusterPtrVector.begin(), corrClusterPtrVector.end());

  // And put them back in the event
  evt.put(std::move(corrClusters), corrClusterCollection);
}

DEFINE_FWK_MODULE(HiEgammaEnergyCorrection);
