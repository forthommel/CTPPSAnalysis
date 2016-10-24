// -*- C++ -*-
//
// Package:    CTPPSAnalysis/TreeProducer
// Class:      TreeProducer
// 
/**\class TreeProducer TreeProducer.cc CTPPSAnalysis/TreeProducer/plugins/TreeProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Mon, 24 Oct 2016 13:04:07 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/CTPPSDigi/interface/DiamondDigi.h"

#include "TTree.h"

class TreeProducer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
    explicit TreeProducer( const edm::ParameterSet& );
    ~TreeProducer();

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    virtual void beginJob() override;
    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
    virtual void endJob() override;

    edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > stripsLocalTrackToken_;
    edm::EDGetTokenT< edm::DetSetVector<DiamondDigi> > diamondDigiToken_;

    TTree* tree_;
    std::vector<float> strips_tracks_x0_, strips_tracks_y0_, strips_tracks_z0_;
    std::vector<float> strips_tracks_sigx0_, strips_tracks_sigy0_;
    std::vector<float> strips_tracks_tx_, strips_tracks_ty_;
    std::vector<float> strips_tracks_sigtx_, strips_tracks_sigty_;
};

TreeProducer::TreeProducer( const edm::ParameterSet& iConfig ) :
  stripsLocalTrackToken_( consumes< edm::DetSetVector<TotemRPLocalTrack> >( iConfig.getParameter<edm::InputTag>( "stripsLocalTrackLabel" ) ) ),
  diamondDigiToken_     ( consumes< edm::DetSetVector<DiamondDigi> >      ( iConfig.getParameter<edm::InputTag>( "diamondDigiLabel" ) ) )
{
  usesResource( "TFileService" );
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>( "tree", "" );
}


TreeProducer::~TreeProducer()
{
}

// ------------ method called for each event  ------------
void
TreeProducer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripsColl;
  iEvent.getByToken( stripsLocalTrackToken_, stripsColl );

  edm::Handle< edm::DetSetVector<DiamondDigi> > diamondDigiColl;
  iEvent.getByToken( diamondDigiToken_, diamondDigiColl );

  for ( edm::DetSetVector<TotemRPLocalTrack>::const_iterator stripds=stripsColl->begin(); stripds!=stripsColl->end(); stripds++ ) {
    edm::LogError("TreeProducer") << stripds->detId();
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
TreeProducer::beginJob()
{
  tree_->Branch( "strips_track_x0", &strips_tracks_x0_ );
  tree_->Branch( "strips_track_y0", &strips_tracks_y0_ );
  tree_->Branch( "strips_track_z0", &strips_tracks_z0_ );
  tree_->Branch( "strips_track_x0_sig", &strips_tracks_sigx0_ );
  tree_->Branch( "strips_track_y0_sig", &strips_tracks_sigy0_ );
  tree_->Branch( "strips_track_Tx", &strips_tracks_tx_ );
  tree_->Branch( "strips_track_Ty", &strips_tracks_ty_ );
  tree_->Branch( "strips_track_Tx_sig", &strips_tracks_sigtx_ );
  tree_->Branch( "strips_track_Ty_sig", &strips_tracks_sigty_ );

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TreeProducer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TreeProducer::fillDescriptions( edm::ConfigurationDescriptions& descriptions ) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

//define this as a plug-in
DEFINE_FWK_MODULE( TreeProducer );
