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
#include "RecoHI/HiEgammaAlgos/interface/HiEgammaBkgSubtraction.h"


HiEgammaBkgSubtraction::HiEgammaBkgSubtraction(const edm::ParameterSet& ps)
{
  barrelCorrClusterCollection_=ps.getParameter<std::string>("barrelCorrClusterCollection");
  produces<reco::BasicClusterCollection>(barrelCorrClusterCollection_);
  endcapCorrClusterCollection_=ps.getParameter<std::string>("endcapCorrClusterCollection");
  produces<reco::BasicClusterCollection>(endcapCorrClusterCollection_);

  excludeClusters_=ps.getParameter<bool>("excludeClusters");

  barrelClustersToken=consumes<reco::BasicClusterCollection>(edm::InputTag(ps.getParameter<std::string>("barrelClusterProducer"),ps.getParameter<std::string>("barrelClusterCollection")));
  endcapClustersToken=consumes<reco::BasicClusterCollection>(edm::InputTag(ps.getParameter<std::string>("endcapClusterProducer"),ps.getParameter<std::string>("endcapClusterCollection")));

  barrelRecHits_=consumes<EcalRecHitCollection>(ps.getParameter<edm::InputTag>("barrelHits"));
  endcapRecHits_=consumes<EcalRecHitCollection>(ps.getParameter<edm::InputTag>("endcapHits"));
  
  corrAlgo_p=new HiEgammaBkgSubAlgo();
}

HiEgammaBkgSubtraction::~HiEgammaBkgSubtraction() {delete corrAlgo_p;}

void HiEgammaBkgSubtraction::produce(edm::Event& evt, const edm::EventSetup& es)
{
  correctClusterEnergies(evt);
}

void HiEgammaBkgSubtraction::getClusterPtrVector(edm::Event& evt, const edm::EDGetTokenT<reco::BasicClusterCollection>& clustersToken, reco::CaloClusterPtrVector * clusterPtrVector_p)
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

const EcalRecHitCollection* HiEgammaBkgSubtraction::getCollection(edm::Event& evt, const edm::EDGetTokenT<EcalRecHitCollection>& token)
{
  edm::Handle<EcalRecHitCollection> rhcHandle;
  evt.getByToken(token, rhcHandle);
  return rhcHandle.product(); 
}

void HiEgammaBkgSubtraction::correctClusterEnergies(edm::Event& evt)
{
  // Get rec hits
  const EcalRecHitCollection* eb_rhc_p=getCollection(evt, barrelRecHits_);

  // Get vector of basic clusters
  reco::CaloClusterPtrVector* clusterPtrVector_p=new reco::CaloClusterPtrVector;
  getClusterPtrVector(evt, barrelClustersToken, clusterPtrVector_p);

  // Correct their energies
  reco::BasicClusterCollection corrClusterPtrVector;
  corrClusterPtrVector=corrAlgo_p->correctEnergies(*clusterPtrVector_p, eb_rhc_p, excludeClusters_, true);

  // Make autopointer
  auto corrClusters=std::make_unique<reco::BasicClusterCollection>();
  corrClusters->assign(corrClusterPtrVector.begin(), corrClusterPtrVector.end());

  // And put them back in the event
  evt.put(std::move(corrClusters), barrelCorrClusterCollection_);

  // And repeat for endcaps
  // Get rec hits
  const EcalRecHitCollection* ee_rhc_p=getCollection(evt, endcapRecHits_);

  // Get vector of basic clusters
  clusterPtrVector_p=new reco::CaloClusterPtrVector;
  getClusterPtrVector(evt, endcapClustersToken, clusterPtrVector_p);

  // Correct their energies
  corrClusterPtrVector=corrAlgo_p->correctEnergies(*clusterPtrVector_p, ee_rhc_p, excludeClusters_, false);

  // Make autopointer
  corrClusters=std::make_unique<reco::BasicClusterCollection>();
  corrClusters->assign(corrClusterPtrVector.begin(), corrClusterPtrVector.end());

  // And put them back in the event
  evt.put(std::move(corrClusters), endcapCorrClusterCollection_);
}

DEFINE_FWK_MODULE(HiEgammaBkgSubtraction);
