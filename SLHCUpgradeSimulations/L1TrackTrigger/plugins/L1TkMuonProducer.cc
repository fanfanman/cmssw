// -*- C++ -*-
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1DTPlusTrackTrigger/interface/DTBtiTrigger.h"
#include "DataFormats/L1DTPlusTrackTrigger/interface/DTTSPhiTrigger.h"
#include "DataFormats/L1DTPlusTrackTrigger/interface/DTTSThetaTrigger.h"
#include "DataFormats/L1DTPlusTrackTrigger/interface/DTMatch.h"

#include "DataFormats/L1TrackTrigger/interface/L1TkMuonParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkMuonParticleFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkPrimaryVertex.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <string>
#include "TMath.h"

using namespace l1extra;

// class declaration
class L1TkMuonParticleProducer : public edm::EDProducer
{
  public:
    typedef TTTrack< Ref_PixelDigi_ > L1TkTrackType;
    typedef std::vector< L1TkTrackType > L1TkTrackCollectionType;

    explicit L1TkMuonParticleProducer( const edm::ParameterSet& );
    ~L1TkMuonParticleProducer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob();
    virtual void produce( edm::Event&, const edm::EventSetup& );
    virtual void endJob();
};

// constructors and destructor
L1TkMuonParticleProducer::L1TkMuonParticleProducer( const edm::ParameterSet& iConfig )
{
  produces< L1TkMuonParticleCollection >("DTMatchInwardsPriority");
  produces< L1TkMuonParticleCollection >("DTMatchInwardsAverage");
  produces< L1TkMuonParticleCollection >("DTMatchInwardsMajorityFullTk");
  produces< L1TkMuonParticleCollection >("DTMatchInwardsMajority");
  produces< L1TkMuonParticleCollection >("DTMatchInwardsMixedMode");
  produces< L1TkMuonParticleCollection >("DTMatchInwardsTTTrack");
  produces< L1TkMuonParticleCollection >("DTMatchInwardsTTTrackFullReso");
}

L1TkMuonParticleProducer::~L1TkMuonParticleProducer() {}

// method called to produce the data
void L1TkMuonParticleProducer::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  using namespace edm;
  using namespace std;

std::cout << " --- MANU enter in L1TkMuonParticleProducer " << std::endl;

  std::auto_ptr< L1TkMuonParticleCollection > outputDTMatchInwardsPriority( new L1TkMuonParticleCollection );
  std::auto_ptr< L1TkMuonParticleCollection > outputDTMatchInwardsAverage( new L1TkMuonParticleCollection );
  std::auto_ptr< L1TkMuonParticleCollection > outputDTMatchInwardsMajorityFullTk( new L1TkMuonParticleCollection );
  std::auto_ptr< L1TkMuonParticleCollection > outputDTMatchInwardsMajority( new L1TkMuonParticleCollection );
  std::auto_ptr< L1TkMuonParticleCollection > outputDTMatchInwardsMixedMode( new L1TkMuonParticleCollection );
  std::auto_ptr< L1TkMuonParticleCollection > outputDTMatchInwardsTTTrack( new L1TkMuonParticleCollection );
  std::auto_ptr< L1TkMuonParticleCollection > outputDTMatchInwardsTTTrackFullReso( new L1TkMuonParticleCollection );

  // input from DT+Tk trigger (Padova-Trento)
  edm::Handle< std::vector< DTMatch > >     DTMatchHandle;
  iEvent.getByLabel( "DTPlusTrackProducer", DTMatchHandle );
  std::vector< DTMatch >::const_iterator    iterDTMatch;

  int iMu = 0;
  for ( iterDTMatch = DTMatchHandle->begin();
        iterDTMatch != DTMatchHandle->end();
        ++iterDTMatch )
  {

std::cout << " here a DT match " << std::endl;

    // create the reference to DTMatch FIXME
    edm::Ptr< DTMatch > dtMatchRef( DTMatchHandle, iMu++ );

    // get the bx
    int bx = iterDTMatch->getDTBX();
std::cout << " bx =  " << bx << std::endl;

// MANU CHECK !
    //if ( bx != 0 ) continue;

std::cout << " passes bx " << std::endl;

    // check the different options

std::cout << " iterDTMatch->getPtPriorityBin() > 0. " << iterDTMatch->getPtPriorityBin() << std::endl;

    if ( iterDTMatch->getPtPriorityBin() > 0. )
    {
      float tkIso = -999; // dummy

      // build momentum from PtBin
      // no TTTrack used for this method: direction comes from
      // the DT object (TSPhi and TSTheta)
      float pt = iterDTMatch->getPtPriorityBin();
      float px = pt * cos( iterDTMatch->getDTDirection().phi() );
      float py = pt * sin( iterDTMatch->getDTDirection().phi() );
      float pz = pt * tan( iterDTMatch->getDTDirection().theta() );
      float e = sqrt( px*px + py*py + pz*pz ); // massless particle 
      math::XYZTLorentzVector candP4(px, py, pz, e);

      // create the candidate
      L1TkMuonParticle candMu( candP4, dtMatchRef, tkIso );

      // put it in the collection
      outputDTMatchInwardsPriority->push_back( candMu );
    }

    if ( iterDTMatch->getPtAverageBin() > 0. )
    {
      float tkIso = -999; // dummy

      // build momentum from PtBin
      // no TTTrack used for this method: direction comes from
      // the DT object (TSPhi and TSTheta)
      float pt = iterDTMatch->getPtAverageBin();
      float px = pt * cos( iterDTMatch->getDTDirection().phi() );
      float py = pt * sin( iterDTMatch->getDTDirection().phi() );
      float pz = pt * tan( iterDTMatch->getDTDirection().theta() );
      float e = sqrt( px*px + py*py + pz*pz ); // massless particle 
      math::XYZTLorentzVector candP4(px, py, pz, e);

      // create the candidate
      L1TkMuonParticle candMu( candP4, dtMatchRef, tkIso );

      // put it in the collection
      outputDTMatchInwardsAverage->push_back( candMu );
    }

    if ( iterDTMatch->getPtMajorityFullTkBin() > 0. )
    {
      float tkIso = -999; // dummy

      // build momentum from PtBin
      // no TTTrack used for this method: direction comes from
      // the DT object (TSPhi and TSTheta)
      float pt = iterDTMatch->getPtMajorityFullTkBin();
      float px = pt * cos( iterDTMatch->getDTDirection().phi() );
      float py = pt * sin( iterDTMatch->getDTDirection().phi() );
      float pz = pt * tan( iterDTMatch->getDTDirection().theta() );
      float e = sqrt( px*px + py*py + pz*pz ); // massless particle 
      math::XYZTLorentzVector candP4(px, py, pz, e);

      // create the candidate
      L1TkMuonParticle candMu( candP4, dtMatchRef, tkIso );

      // put it in the collection
      outputDTMatchInwardsMajorityFullTk->push_back( candMu );
    }

    if ( iterDTMatch->getPtMajorityBin() > 0. )
    {
      float tkIso = -999; // dummy

      // build momentum from PtBin
      // no TTTrack used for this method: direction comes from
      // the DT object (TSPhi and TSTheta)
      float pt = iterDTMatch->getPtMajorityBin();
      float px = pt * cos( iterDTMatch->getDTDirection().phi() );
      float py = pt * sin( iterDTMatch->getDTDirection().phi() );
      float pz = pt * tan( iterDTMatch->getDTDirection().theta() );
      float e = sqrt( px*px + py*py + pz*pz ); // massless particle 
      math::XYZTLorentzVector candP4(px, py, pz, e);

      // create the candidate
      L1TkMuonParticle candMu( candP4, dtMatchRef, tkIso );

      // put it in the collection
      outputDTMatchInwardsMajority->push_back( candMu );
    }

    if ( iterDTMatch->getPtMixedModeBin() > 0. )
    {
      float tkIso = -999; // dummy

      // build momentum from PtBin
      // no TTTrack used for this method: direction comes from
      // the DT object (TSPhi and TSTheta)
      float pt = iterDTMatch->getPtMixedModeBin();
      float px = pt * cos( iterDTMatch->getDTDirection().phi() );
      float py = pt * sin( iterDTMatch->getDTDirection().phi() );
      float pz = pt * tan( iterDTMatch->getDTDirection().theta() );
      float e = sqrt( px*px + py*py + pz*pz ); // massless particle 
      math::XYZTLorentzVector candP4(px, py, pz, e);

      // create the candidate
      L1TkMuonParticle candMu( candP4, dtMatchRef, tkIso );

      // put it in the collection
      outputDTMatchInwardsMixedMode->push_back( candMu );
    }

std::cout << " iterDTMatch->getPtTTTrackBin() > 0. " << iterDTMatch->getPtTTTrackBin() << std::endl;

    if ( iterDTMatch->getPtTTTrackBin() > 0. )
    {
      float tkIso = -999; // dummy

      // build momentum from PtBin
      // TTTrack used for this method: direction comes from the TTTrack
      float pt = iterDTMatch->getPtTTTrackBin();
      GlobalVector tkMomentum = iterDTMatch->getPtMatchedTrackPtr()->getMomentum();
      float px = pt * cos( tkMomentum.phi() );
      float py = pt * sin( tkMomentum.phi() );
      float pz = pt * tan( tkMomentum.theta() );
      float e = sqrt( px*px + py*py + pz*pz ); // massless particle 
      math::XYZTLorentzVector candP4(px, py, pz, e);

      // create the candidate
      L1TkMuonParticle candMu( candP4, dtMatchRef, tkIso );

      // put it in the collection
      outputDTMatchInwardsTTTrack->push_back( candMu );
    }

    if ( iterDTMatch->getPtMatchedTrackPtr().isNull() == false )
    {
      float tkIso = -999; // dummy

      // build momentum from PtBin
      // TTTrack used for this method: direction comes from the TTTrack
      GlobalVector tkMomentum = iterDTMatch->getPtMatchedTrackPtr()->getMomentum();
      float px = tkMomentum.x();
      float py = tkMomentum.y();
      float pz = tkMomentum.z();
      float e = sqrt( px*px + py*py + pz*pz ); // massless particle 
      math::XYZTLorentzVector candP4(px, py, pz, e);

      // create the candidate
      L1TkMuonParticle candMu( candP4, dtMatchRef, tkIso );

      // put it in the collection
      outputDTMatchInwardsTTTrackFullReso->push_back( candMu );
    }


  }  // end loop over Padova-Trento objects

  // store the collections
  iEvent.put( outputDTMatchInwardsPriority , "DTMatchInwardsPriority");
  iEvent.put( outputDTMatchInwardsAverage , "DTMatchInwardsAverage");
  iEvent.put( outputDTMatchInwardsMajorityFullTk , "DTMatchInwardsMajorityFullTk");
  iEvent.put( outputDTMatchInwardsMajority , "DTMatchInwardsMajority");
  iEvent.put( outputDTMatchInwardsMixedMode , "DTMatchInwardsMixedMode");
  iEvent.put( outputDTMatchInwardsTTTrack , "DTMatchInwardsTTTrack" );
  iEvent.put( outputDTMatchInwardsTTTrackFullReso, "DTMatchInwardsTTTrackFullReso" );


}

// method called once each job just before starting event loop
void L1TkMuonParticleProducer::beginJob() {}

// method called once each job just after ending the event loop
void L1TkMuonParticleProducer::endJob() {}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TkMuonParticleProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


//define this as a plug-in
DEFINE_FWK_MODULE(L1TkMuonParticleProducer);

