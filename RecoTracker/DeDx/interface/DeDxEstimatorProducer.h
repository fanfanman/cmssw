#ifndef DeDxEstimatorProducer_H
#define DeDxEstimatorProducer_H
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTracker/DeDx/interface/BaseDeDxEstimator.h"
#include "RecoTracker/DeDx/interface/TrajectorySateOnDetInfosProducer.h"


//
// class decleration
//

class DeDxEstimatorProducer : public edm::EDProducer {
   public:
      explicit DeDxEstimatorProducer(const edm::ParameterSet&);
      ~DeDxEstimatorProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      void produce_from_tsodi     (edm::Event& iEvent, const edm::EventSetup& iSetup, edm::ESHandle<TrackerGeometry> tkGeom);
      void produce_from_trajectory(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::ESHandle<TrackerGeometry> tkGeom);
      
      // ----------member data ---------------------------
      BaseDeDxEstimator *               m_estimator;
      edm::InputTag                     m_TsodiTag;
      edm::InputTag                     m_trajTrackAssociationTag;
      edm::InputTag                     m_tracksTag;
      bool                              m_FromTrajectory;
      TrajectorySateOnDetInfosProducer* m_TSODIProducer;
};

#endif

