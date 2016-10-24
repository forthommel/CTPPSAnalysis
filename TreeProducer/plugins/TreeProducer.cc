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
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"

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

    void clearTree();

    edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > stripsLocalTrackToken_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondDigi> > diamondDigiToken_;

    TTree* tree_;
    // strips leaves
    std::vector<unsigned short> strips_tracks_detid_;
    std::vector<float> strips_tracks_x0_, strips_tracks_y0_, strips_tracks_z0_;
    std::vector<float> strips_tracks_sigx0_, strips_tracks_sigy0_;
    std::vector<float> strips_tracks_tx_, strips_tracks_ty_;
    std::vector<float> strips_tracks_sigtx_, strips_tracks_sigty_;
    // diamonds leaves
    std::vector<unsigned short> diam_digis_detid_, diam_digis_channelid_;
    std::vector<unsigned int> diam_digis_leadedge_, diam_digis_trailedge_, diam_digis_thresvolt_;
};

TreeProducer::TreeProducer( const edm::ParameterSet& iConfig ) :
  stripsLocalTrackToken_( consumes< edm::DetSetVector<TotemRPLocalTrack> >( iConfig.getParameter<edm::InputTag>( "stripsLocalTrackLabel" ) ) ),
  diamondDigiToken_     ( consumes< edm::DetSetVector<CTPPSDiamondDigi> >      ( iConfig.getParameter<edm::InputTag>( "diamondDigiLabel" ) ) )
{
  usesResource( "TFileService" );
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>( "tree", "" );
}


TreeProducer::~TreeProducer()
{
}

void
TreeProducer::clearTree()
{
  // silicon strips leaves
  strips_tracks_detid_.clear();
  strips_tracks_x0_.clear(); strips_tracks_y0_.clear(); strips_tracks_z0_.clear();
  strips_tracks_sigx0_.clear(); strips_tracks_sigy0_.clear();
  strips_tracks_tx_.clear(); strips_tracks_ty_.clear();
  strips_tracks_sigtx_.clear(); strips_tracks_sigty_.clear();

  // diamonds leaves
  diam_digis_detid_.clear(); diam_digis_channelid_.clear();
  diam_digis_leadedge_.clear(); diam_digis_trailedge_.clear(); diam_digis_thresvolt_.clear();
}

// ------------ method called for each event  ------------
void
TreeProducer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  clearTree();

  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > stripsColl;
  iEvent.getByToken( stripsLocalTrackToken_, stripsColl );

  edm::Handle< edm::DetSetVector<CTPPSDiamondDigi> > diamondDigiColl;
  iEvent.getByToken( diamondDigiToken_, diamondDigiColl );

  for ( edm::DetSetVector<TotemRPLocalTrack>::const_iterator stripds=stripsColl->begin(); stripds!=stripsColl->end(); stripds++ ) {
    const unsigned short detid = stripds->detId();
    for ( edm::DetSet<TotemRPLocalTrack>::const_iterator striplt=stripds->begin(); striplt!=stripds->end(); striplt++ ) {
      if ( !striplt->isValid() ) continue;

      strips_tracks_detid_.push_back( detid );
      strips_tracks_x0_.push_back( striplt->getX0() );
      strips_tracks_sigx0_.push_back( striplt->getX0Sigma() );
      strips_tracks_y0_.push_back( striplt->getY0() );
      strips_tracks_sigy0_.push_back( striplt->getY0Sigma() );
      strips_tracks_z0_.push_back( striplt->getZ0() );
      strips_tracks_tx_.push_back( striplt->getTx() );
      strips_tracks_sigtx_.push_back( striplt->getTxSigma() );
      strips_tracks_ty_.push_back( striplt->getTy() );
      strips_tracks_sigty_.push_back( striplt->getTySigma() );
    }
  }

  for ( edm::DetSetVector<CTPPSDiamondDigi>::const_iterator diads=diamondDigiColl->begin(); diads!=diamondDigiColl->end(); diads++ ) {
    const unsigned short detid = diads->detId();
    for ( edm::DetSet<CTPPSDiamondDigi>::const_iterator diadigi=diads->begin(); diadigi!=diads->end(); diadigi++ ) {
      diam_digis_detid_.push_back( detid );
      diam_digis_channelid_.push_back( diadigi->getChannelId() );
      diam_digis_leadedge_.push_back( diadigi->getLeadingEdge() );
      diam_digis_trailedge_.push_back( diadigi->getLeadingEdge() );
      diam_digis_thresvolt_.push_back( diadigi->getThresholdVoltage() );
    }
  }

  tree_->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
TreeProducer::beginJob()
{
  tree_->Branch( "strips_track_detid", &strips_tracks_detid_ );
  tree_->Branch( "strips_track_x0", &strips_tracks_x0_ );
  tree_->Branch( "strips_track_y0", &strips_tracks_y0_ );
  tree_->Branch( "strips_track_z0", &strips_tracks_z0_ );
  tree_->Branch( "strips_track_x0_sig", &strips_tracks_sigx0_ );
  tree_->Branch( "strips_track_y0_sig", &strips_tracks_sigy0_ );
  tree_->Branch( "strips_track_Tx", &strips_tracks_tx_ );
  tree_->Branch( "strips_track_Ty", &strips_tracks_ty_ );
  tree_->Branch( "strips_track_Tx_sig", &strips_tracks_sigtx_ );
  tree_->Branch( "strips_track_Ty_sig", &strips_tracks_sigty_ );

  tree_->Branch( "diamonds_digi_detid", &diam_digis_detid_ );
  tree_->Branch( "diamonds_digi_channelid", &diam_digis_channelid_ );
  tree_->Branch( "diamonds_digi_leading_edge", &diam_digis_leadedge_ );
  tree_->Branch( "diamonds_digi_trailing_edge", &diam_digis_trailedge_ );
  tree_->Branch( "diamonds_digi_threshold_voltage", &diam_digis_thresvolt_ );
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
