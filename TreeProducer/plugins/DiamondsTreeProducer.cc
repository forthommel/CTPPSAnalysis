// -*- C++ -*-
//
// Package:    CTPPSAnalysis/TreeProducer
// Class:      DiamondsTreeProducer
// 
/**\class DiamondsTreeProducer DiamondsTreeProducer.cc CTPPSAnalysis/TreeProducer/plugins/DiamondsTreeProducer.cc

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

// diamonds
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

#include "TTree.h"
#include "TH2.h"

class DiamondsTreeProducer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
    explicit DiamondsTreeProducer( const edm::ParameterSet& );
    ~DiamondsTreeProducer();

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    virtual void beginJob() override;
    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
    virtual void endJob() override;

    void clearTree();

    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondDigi> > diamondDigiToken_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > diamondRecHitToken_;

    TTree* tree_;
    // diamond digis leaves
    std::vector<unsigned short> diam_digis_detid_, diam_digis_arm_, diam_digis_plane_, diam_digis_channelid_;
    std::vector<unsigned short> diam_digis_multiplehits_;
    std::vector<unsigned int> diam_digis_leadedge_, diam_digis_trailedge_, diam_digis_thresvolt_;

    // diamond rechits leaves
    std::vector<unsigned short> diam_rh_side_, diam_rh_plane_;
    std::vector<float> diam_rh_x_, diam_rh_y_;
    std::vector<float> diam_rh_x_size_, diam_rh_y_size_;
};

DiamondsTreeProducer::DiamondsTreeProducer( const edm::ParameterSet& iConfig ) :
  diamondDigiToken_     ( consumes< edm::DetSetVector<CTPPSDiamondDigi> >  ( iConfig.getParameter<edm::InputTag>( "diamondDigiLabel" ) ) ),
  diamondRecHitToken_   ( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >( iConfig.getParameter<edm::InputTag>( "diamondRecHitLabel" ) ) )
{
  usesResource( "TFileService" );
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>( "tree", "" );
}


DiamondsTreeProducer::~DiamondsTreeProducer()
{
}

void
DiamondsTreeProducer::clearTree()
{
  // diamonds leaves
  //--- digis
  diam_digis_detid_.clear(); diam_digis_arm_.clear(); diam_digis_plane_.clear(); diam_digis_channelid_.clear();
  diam_digis_multiplehits_.clear();
  diam_digis_leadedge_.clear(); diam_digis_trailedge_.clear(); diam_digis_thresvolt_.clear();
  //--- rechits
  diam_rh_side_.clear(); diam_rh_plane_.clear();
  diam_rh_x_.clear(); diam_rh_y_.clear();
  diam_rh_x_size_.clear(); diam_rh_y_size_.clear();
}

// ------------ method called for each event  ------------
void
DiamondsTreeProducer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  clearTree();

  //----- diamonds digis
  edm::Handle< edm::DetSetVector<CTPPSDiamondDigi> > diamondDigiColl;
  iEvent.getByToken( diamondDigiToken_, diamondDigiColl );

  for ( edm::DetSetVector<CTPPSDiamondDigi>::const_iterator diads=diamondDigiColl->begin(); diads!=diamondDigiColl->end(); diads++ ) {
    const CTPPSDiamondDetId detid( diads->detId() );
    for ( edm::DetSet<CTPPSDiamondDigi>::const_iterator diadigi=diads->begin(); diadigi!=diads->end(); diadigi++ ) {
      //--- detid-related information
      diam_digis_detid_.push_back( detid() );
      diam_digis_arm_.push_back( detid.arm() ); // 0 = 4-5 ; 1 = 5-6
      diam_digis_plane_.push_back( detid.plane() );
      diam_digis_channelid_.push_back( detid.det() );
      //--- hit-related information
      diam_digis_leadedge_.push_back( diadigi->getLeadingEdge() );
      diam_digis_trailedge_.push_back( diadigi->getTrailingEdge() );
      diam_digis_thresvolt_.push_back( diadigi->getThresholdVoltage() );
      diam_digis_multiplehits_.push_back( diadigi->getMultipleHit() );
    }
  }

  //----- diamonds rechits
  edm::Handle< edm::DetSetVector<CTPPSDiamondRecHit> > diamondRecHitsColl;
  iEvent.getByToken( diamondRecHitToken_, diamondRecHitsColl );

  for ( edm::DetSetVector<CTPPSDiamondRecHit>::const_iterator rhds=diamondRecHitsColl->begin(); rhds!=diamondRecHitsColl->end(); rhds++ ) {
    const CTPPSDiamondDetId detid( rhds->detId() );
    for ( edm::DetSet<CTPPSDiamondRecHit>::const_iterator rh=rhds->begin(); rh!=rhds->end(); rh++ ) {
      diam_rh_side_.push_back( detid.arm() ); // 0 = 45 ; 1 = 56
      diam_rh_plane_.push_back( detid.plane() );
      diam_rh_x_.push_back( rh->getX() );
      diam_rh_y_.push_back( rh->getY() );
      diam_rh_x_size_.push_back( rh->getXWidth() );
      diam_rh_y_size_.push_back( rh->getYWidth() );
    }
  }

  tree_->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void 
DiamondsTreeProducer::beginJob()
{
  tree_->Branch( "diamonds_digi_detid", &diam_digis_detid_ );
  tree_->Branch( "diamonds_digi_arm", &diam_digis_arm_ );
  tree_->Branch( "diamonds_digi_plane", &diam_digis_plane_ );
  tree_->Branch( "diamonds_digi_channelid", &diam_digis_channelid_ );
  tree_->Branch( "diamonds_digi_leading_edge", &diam_digis_leadedge_ );
  tree_->Branch( "diamonds_digi_trailing_edge", &diam_digis_trailedge_ );
  tree_->Branch( "diamonds_digi_threshold_voltage", &diam_digis_thresvolt_ );
  tree_->Branch( "diamonds_digi_multiple_hits", &diam_digis_multiplehits_ );

  tree_->Branch( "diamonds_rh_side", &diam_rh_side_ );
  tree_->Branch( "diamonds_rh_plane", &diam_rh_plane_ );
  tree_->Branch( "diamonds_rh_x", &diam_rh_x_ );
  tree_->Branch( "diamonds_rh_y", &diam_rh_y_ );
  tree_->Branch( "diamonds_rh_x_size", &diam_rh_x_size_ );
  tree_->Branch( "diamonds_rh_y_size", &diam_rh_y_size_ );
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiamondsTreeProducer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DiamondsTreeProducer::fillDescriptions( edm::ConfigurationDescriptions& descriptions ) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

//define this as a plug-in
DEFINE_FWK_MODULE( DiamondsTreeProducer );
