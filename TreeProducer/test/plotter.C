bool contains( const vector< pair<float,float> >& vec, const float& elem )
{
  for ( vector< pair<float,float> >::const_iterator it=vec.begin(); it!=vec.end(); it++ ) {
    if ( it->first==elem ) { return true; }
  }
  return false;
}

struct sorter
{
  inline bool operator()( const pair<float,float>& lhs, const pair<float,float>& rhs )
  {
    return ( lhs.first<rhs.first );
  }
};

void plotter()
{
  TFile f( "output.root" );
  TTree* t = ( TTree* )f.Get( "ctppsDiamondsTreeProducer/tree" );

  vector<unsigned short>* side = new vector<unsigned short>,
                        *plane = new vector<unsigned short>;
  vector<float>* x = new vector<float>,
                *y = new vector<float>,
                *x_width = new vector<float>,
                *y_width = new vector<float>;
  t->SetBranchAddress( "diamonds_rh_side", &side );
  t->SetBranchAddress( "diamonds_rh_plane", &plane );
  t->SetBranchAddress( "diamonds_rh_x", &x );
  t->SetBranchAddress( "diamonds_rh_y", &y );
  t->SetBranchAddress( "diamonds_rh_x_size", &x_width );
  t->SetBranchAddress( "diamonds_rh_y_size", &y_width );

  // loop to extract the pixels size and position
  vector< pair<float,float> > pos[8];
  for ( unsigned long i=0; i<t->GetEntries(); i++ ) {
    t->GetEntry( i );
    for ( unsigned short j=0; j<plane->size(); j++ ) { // loop over the hits
      const unsigned short pl_id = side->at( j )*4 + plane->at( j );
      bool has_element = contains( pos[pl_id], x->at( j ) );
      if ( !has_element ) {
        pos[pl_id].push_back( pair<float,float>( x->at( j ), x_width->at( j ) ) );
      }
    }
  }

  // summary of sizes/positions
  for ( unsigned short i=0; i<8; i++ ) { // loop over planes
    sort( pos[i].begin(), pos[i].end(), sorter() );
    cout << " ===== PLANE " << i << " =====" << endl;
    for ( unsigned short j=0; j<pos[i].size(); j++ ) {
      cout << "  x = " << pos[i][j].first << "\thalf-width = " << pos[i][j].second << "\twidth = " << pos[i][j].second*2 << endl;
    }
  }

}
