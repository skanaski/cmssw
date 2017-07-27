#include <iostream>
#include <vector>
#include <memory>
#include <set>

#include "RecoHI/HiEgammaAlgos/interface/HiEgammaBkgSubAlgo.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"


reco::BasicClusterCollection HiEgammaBkgSubAlgo::correctEnergies(reco::CaloClusterPtrVector & clusters, const EcalRecHitCollection* hits, bool excludeClusters, bool isBarrel)
{
  // Create set of DetId used in basic clusters in order to exclude them from later calculations
  std::set<DetId> usedId;
  for (reco::CaloCluster_iterator it=clusters.begin(); it!=clusters.end(); it++)
    {
      const edm::Ptr<reco::CaloCluster> tmp_cluster=*it;
      std::vector< std::pair<DetId,float> > haf=tmp_cluster->hitsAndFractions();
      for (unsigned i=0;i<haf.size();i++) {usedId.insert(haf.at(i).first());}
    }

  // Applies energy correction to each cluster individually
  reco::BasicClusterCollection corrClusters;
  for (reco::CaloCluster_iterator it=clusters.begin(); it!=clusters.end(); it++)
    {
      if (isBarrel) corrClusters.push_back(correctBarrelEnergy(*it, hits, excludeClusters, usedId));
      else corrClusters.push_back(correctEndcapEnergy(*it, hits, excludeClusters, usedId));
    }
  
  return corrClusters;
}

reco::BasicCluster HiEgammaBkgSubAlgo::correctBarrelEnergy(const edm::Ptr<reco::CaloCluster> cluster_p,
								  const EcalRecHitCollection* hits,
								  bool excludeClusters,
								  std::set<DetId> usedId)
{
  // First determine ieta span of the cluster...
  const DetId& seedId=cluster_p->seed();

  int ieta_max=EBDetId(seedId).ieta();
  int ieta_min=ieta_max;

  std::vector< std::pair<DetId, float> > haf=cluster_p->hitsAndFractions();

  for(unsigned i=0;i<haf.size();i++)
    {
      DetId tmpId=haf.at(i).first();
      int ieta=EBDetId(tmpId).ieta();
      if (ieta>ieta_max) ieta_max=ieta;
      if (ieta<ieta_min) ieta_min=ieta;
    }
  
  // Then find the average energy per crystal in an iphi strip over the determined ieta range...
  double energy=0;
  int nCrys=0;

  for (EcalRecHitCollection::const_iterator it=hits->begin();it!=hits->end();it++)
    {
      DetId tmpId=it->id();
      if (excludeClusters && usedId.find(tmpId)!=usedId.end()) continue;
      int ieta=EBDetId(tmpId).ieta();
      if (ieta<=ieta_max && ieta>=ieta_min)
	{
	  energy+=it->energy();
	  nCrys++;
	}
    }
  energy/=nCrys;

  // And finally subtract the average energy, scaled by the area of the cluster, from the energy of the cluster itself
  double newEnergy=cluster_p->energy()-energy*haf.size();

  // Now that the new energy is determined, make a new cluster, identical to the original except for the new energy, and return it
  reco::BasicCluster corrCluster(newEnergy,
				 math::XYZPoint(cluster_p->position().X(),cluster_p->position().Y(),cluster_p->position().Z()),
				 cluster_p->caloID(),
				 haf,
				 cluster_p->algoID(),
				 seedId,
				 cluster_p->flags());
  return corrCluster;
}

reco::BasicCluster HiEgammaBkgSubAlgo::correctEndcapEnergy(const edm::Ptr<reco::CaloCluster> cluster_p,
							       const EcalRecHitCollection* hits,
							       bool excludeClusters,
							       std::set<DetId> usedId)
{
  // First determine radial span of the cluster...
  const DetId& seedId=cluster_p->seed();

  EEDetId tmpEEId=EEDetId(seedId);
  double r2_max=((double) tmpEEId.ix()-50.5)*((double) tmpEEId.ix()-50.5)+((double) tmpEEId.iy()-50.5)*((double) tmpEEId.iy()-50.5);
  double r2_min=r2_max;
  int zside=tmpEEId.zside();

  std::vector< std::pair<DetId, float> > haf=cluster_p->hitsAndFractions();

  for(unsigned i=0;i<haf.size();i++)
    {
      tmpEEId=EEDetId(haf.at(i).first());
      if (zside==tmpEEId.zside())
	{
	  double r2=((double) tmpEEId.ix()-50.5)*((double) tmpEEId.ix()-50.5)+((double) tmpEEId.iy()-50.5)*((double) tmpEEId.iy()-50.5);
	  if (r2>r2_max) r2_max=r2;
	  if (r2<r2_min) r2_min=r2;
	}
    }
  
  // Then find the average energy per crystal in an iphi strip over the determined ieta range...
  double energy=0;
  int nCrys=0;
  
  for (EcalRecHitCollection::const_iterator it=hits->begin();it!=hits->end();it++)
    {
      DetId tmpId=it->id();
      if (excludeClusters && usedId.find(tmpId)!=usedId.end()) continue;
      tmpEEId=EEDetId(tmpId);
      double r2=((double) tmpEEId.ix()-50.5)*((double) tmpEEId.ix()-50.5)+((double) tmpEEId.iy()-50.5)*((double) tmpEEId.iy()-50.5);
      if (r2<=r2_max && r2>=r2_min && zside==tmpEEId.zside())
	{
	  energy+=it->energy();
	  nCrys++;
	}
    }
  energy/=nCrys;
  
  // And finally subtract the average energy, scaled by the area of the cluster, from the energy of the cluster itself
  double newEnergy=cluster_p->energy()-energy*haf.size();
  
  // Now that the new energy is determined, make a new cluster, identical to the original except for the new energy, and return it
  reco::BasicCluster corrCluster(newEnergy,
				 math::XYZPoint(cluster_p->position().X(),cluster_p->position().Y(),cluster_p->position().Z()),
				 cluster_p->caloID(),
				 haf,
				 cluster_p->algoID(),
				 seedId,
				 cluster_p->flags());
  return corrCluster;
}
