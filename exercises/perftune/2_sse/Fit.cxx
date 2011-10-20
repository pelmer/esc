
#include <iostream>
#include <iomanip>
#include <fstream>

#include "Stopwatch.h"
#include "Fit.h"
#include <malloc.h>

/* Use Intel TBB for multithreading */
#ifdef TBB
#include "openlab_mod/multithread.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
using namespace tbb;
int tasks = 16;	/* #threads <= #tasks */
#endif

using namespace std;

const int MaxNTracks = 20000;
const int NFits = 1;//0000;
const int Ntimes = 100;

Station vStations[8];
Track vTracks[MaxNTracks];
MCTrack vMCTracks[MaxNTracks];
int NStations = 0;
int NTracks = 0;
int NTracksV = 0;



FieldRegion field0;

void ReadInput(){

  fstream FileGeo, FileTracks;

  FileGeo.open( "geo.dat", ios::in );
  FileTracks.open( "tracks.dat", ios::in );
  {
    FieldVector H[3];
    Fvec_t Hz[3];
    for( int i=0; i<3; i++){
      float Bx, By, Bz, z;
      FileGeo >> z >> Bx >> By >> Bz;
      Hz[i] = z; H[i].X = Bx;   H[i].Y = By; H[i].Z = Bz; 
      cout<<"Input Magnetic field:"<<z<<" "<<Bx<<" "<<By<<" "<<Bz<<endl;
    }
    field0.Set(H[0],Hz[0], H[1],Hz[1], H[2],Hz[2] );
  }
  FileGeo >> NStations;
  cout<<"Input "<<NStations<<" Stations:"<<endl;
  for( int i=0; i<NStations; i++ ){
    int ist;    
    FileGeo >> ist;
    if( ist!=i ) break;
    Station &st = vStations[i];
    FileGeo >> st.z >> st.thick>> st.RL;
#ifdef X87
    cout<<"    "<<st.z <<" "<<st.thick<<" "<<st.RL<<", ";
#else
    cout<<"    "<<st.z[0] <<" "<<st.thick[0]<<" "<<st.RL[0]<<", ";
#endif
    st.zhit = st.z - st.thick/2.;
    st.RadThick = st.thick/st.RL;
    st.logRadThick = log(st.RadThick);
    int N=0;
    FileGeo >> N;
    cout<<N<<" field coeff."<<endl;
    for( int i=0; i<N; i++ ) FileGeo >> st.Map.X[i];
    for( int i=0; i<N; i++ ) FileGeo >> st.Map.Y[i];
    for( int i=0; i<N; i++ ) FileGeo >> st.Map.Z[i];
    //for( int i=0; i<10; i++ ){ st.Map.X[i] = st.Map.Y[i] = st.Map.Z[i] = 0.;}
    //st.Map.X[0]=Bx;
    //st.Map.Y[0]=By;
    //st.Map.Z[0]=Bz;
  }
  {
    Fvec_t Hy0 = vStations[NStations-1].Map.Y[0];
    Fvec_t z0  = vStations[NStations-1].z;
    Fvec_t sy = 0., Sy = 0.;
    for( int i=NStations-1; i>=0; i-- ){
      Station &st = vStations[i];
      Fvec_t dz = st.z-z0;
      Fvec_t Hy = vStations[i].Map.Y[0];
      Sy += dz*sy + dz*dz*Hy/2.;
      sy += dz*Hy;
      st.Sy = Sy;
      z0 = st.z;
    }
  }

  FileGeo.close();

  NTracks = 0;
  while( !FileTracks.eof() ){
    
    int itr;
    FileTracks>>itr;
    //if( itr!=NTracks ) break;
    if( NTracks>=MaxNTracks ) break;

    Track &t = vTracks[NTracks];
    MCTrack &mc = vMCTracks[NTracks];
    FileTracks >> mc.MC_x   >> mc.MC_y  >> mc.MC_z 
	       >> mc.MC_px >> mc.MC_py >> mc.MC_pz >> mc.MC_q
	       >> t.NHits;
    for( int i=0; i<t.NHits; i++ ){
      int ist;
      FileTracks >> ist;
      t.vHits[i].ista = ist;
      FileTracks >> t.vHits[i].x >> t.vHits[i].y;
    }
    if( t.NHits==NStations )   NTracks++;
  }
  FileTracks.close();
  NTracksV = NTracks/vecN;
  NTracks =  NTracksV*vecN;
}

void WriteOutput(){
  
  fstream Out, Diff;

  Out.open( "fit.dat", ios::out );
  Diff.open( "fitdiff.dat", ios::out );
  
  for( int it=0, itt=0; itt<NTracks; itt++ ){
    Track &t = vTracks[itt];
    MCTrack &mc = vMCTracks[itt];

    // convert matrix 
    double C[15];    
    {
      Single_t *tC = (Single_t *) &t.C;
      for( int i=0, n=0; i<5; i++)
	for( int j=0; j<=i; j++, n++ ){
	  C[n]=0;
	  //for(int k=0; k<5;k++) C[n]+=tC[i*5+k] * tC[j*5+k];
	  C[n] = tC[n];
	}
    }

    bool ok = 1;
    for( int i=0; i<6; i++ ){
		ok = ok && finite(t.T[i]);
	}
    for( int i=0; i<15; i++ ) ok = ok && finite(C[i]);

    if(!ok){ cout<<" infinite "<<endl; continue; }

    Out <<it<<endl<<"   "
	<< " " << mc.MC_x  << " " << mc.MC_y  << " " << mc.MC_z 
	<< " " << mc.MC_px << " " << mc.MC_py << " " << mc.MC_pz 
	<< " " << mc.MC_q<<endl;
    Out<<"   ";
    for( int i=0; i<6; i++ ) Out<< " " <<t.T[i];
    Out<<endl<<"   ";
    for( int i=0; i<15; i++ ) Out<< " " <<C[i];
    Out<<endl;

    float tmc[6] = { mc.MC_x, mc.MC_y, mc.MC_px/mc.MC_pz, mc.MC_py/mc.MC_pz, 
	            mc.MC_q/sqrt(mc.MC_px*mc.MC_px+mc.MC_py*mc.MC_py+mc.MC_pz*mc.MC_pz), mc.MC_z};
    Diff <<it<<endl;
    Diff<<"   ";
    for( int i=0; i<6; i++ ) Diff<< " " <<t.T[i]-tmc[i];
    Diff<<endl<<"   ";
    for( int i=0; i<15; i++ ) Diff<< " " <<C[i];
    Diff<<endl;
    it++;
  }
  Out.close();
  Diff.close();  
}


void FitTracksV(){

  double TimeTable[Ntimes];

  #if defined (DOUBLE_PRECISION) || defined (X87)
  TrackV *TracksV;
  TracksV = (TrackV *)malloc((MaxNTracks / vecN + 1) * sizeof(TrackV));
  #else
  TrackV TracksV[MaxNTracks/vecN+1];
  #endif

  Fvec_t Z0[MaxNTracks/vecN+1];
  cout<<"Prepare data..."<<endl;
  Stopwatch timer1;

  for( int iV=0; iV<NTracksV; iV++ ){ // loop on set of 4 tracks
    if( iV*vecN%100==0 ) cout<<iV*vecN<<endl;
    TrackV &t = TracksV[iV];
    for( int ist=0; ist<NStations; ist++ ){
      HitV &h = t.vHits[ist];
      h.x = 0.;
      h.y = 0.;
      h.w = 0.;
      h.H.X = 0.;
      h.H.Y = 0.;
      h.H.Z = 0.;
    }
    for( int it=0; it<vecN; it++ ){
      Track &ts = vTracks[iV*vecN+it];
#ifdef X87
      Z0[iV] = vMCTracks[iV*vecN+it].MC_z;
#else
      Z0[iV][it] = vMCTracks[iV*vecN+it].MC_z;
#endif
      for( int ih=0; ih<ts.NHits; ih++ ){
		Hit &hs = ts.vHits[ih];
		HitV &h = t.vHits[hs.ista];
#ifdef X87
		h.x = hs.x;
		h.y = hs.y;
		h.w = 1.;
#else
		h.x[it] = hs.x;
		h.y[it] = hs.y;
		h.w[it] = 1.;
#endif
      }
    }
    for( int ist=0; ist<NStations; ist++ ){
      HitV &h = t.vHits[ist];
      vStations[ist].Map.GetField(h.x, h.y, h.H);
    }
  }
  timer1.Stop();
  cout<<"Start fit..."<<endl;
  Stopwatch timer;
  Stopwatch timer2;
  for( int times=0; times<Ntimes; times++){
    timer2.Start();
    //for( int ifit=0; ifit<NFits; ifit++){
      //int iV=0;
#ifdef TBB
	parallel_for(blocked_range<int>(0, NTracksV, NTracksV / tasks), ApplyFit(TracksV, vStations, NStations));
#else
    for( int iV=0; iV<NTracksV; iV++ ){ // loop on set of 4 tracks
		 Fit( TracksV[iV], vStations, NStations );      
     //}
    }
#endif
    timer2.Stop();
    TimeTable[times]=timer2.RealTime();
  }
  timer.Stop();
        
  
  for( int iV=0; iV<NTracksV; iV++ ){ // loop on set of 4 tracks
    TrackV &t = TracksV[iV];
    ExtrapolateALight( t.T, t.C, Z0[iV], TracksV[iV].T[4], field0 );
  }
  
  double realtime=0;
  fstream TimeFile;
  TimeFile.open( "time.dat", ios::out );
  for( int times=0; times<Ntimes; times++ ){
    TimeFile << TimeTable[times]*1.e6/(NTracks*NFits)<<endl;
    realtime += TimeTable[times]*1.e6/(NTracks*NFits);
  }  
  TimeFile.close();
  realtime /= Ntimes;

  cout<<"Preparation time/track = "<<timer1.CpuTime()*1.e6/NTracks/NFits<<" [us]"<<endl;
  cout<<"CPU  fit time/track = "<<timer.CpuTime()*1.e6/(NTracks*NFits)/Ntimes<<" [us]"<<endl;
  cout<<"Real fit time/track = "<<realtime <<" [us]"<<endl;
  cout<<"Total fit time = "<<timer.CpuTime()<<" [sec]"<<endl;
  cout<<"Total fit real time = "<<timer.RealTime()<<" [sec]"<<endl;

  
  for( int iV=0; iV<NTracksV; iV++ ){ // loop on set of 4 tracks
    TrackV &t = TracksV[iV];
    for( int it=0; it<vecN; it++ ){
      Track &ts = vTracks[iV*vecN+it];
      Fvec_t *C = (Fvec_t*)&t.C;
      Single_t *sC = (Single_t*) &ts.C;
#ifdef X87
      for( int i=0; i<6; i++ ) ts.T[i] = t.T[i];
#else
      for( int i=0; i<6; i++ ) ts.T[i] = t.T[i][it];
#endif
#ifdef SQRT_FILTER
      for( int i=0,n=0; i<5; i++ )
	for( int j=0; j<=i; j++,n++){
	  sC[n]=0;
	  for(int k=0;k<5;k++) sC[n]+= C[i*5+k][it]*C[j*5+k][it];
	}
#else
#ifdef X87
      for( int i=0; i<15; i++ ) sC[i] = C[i];      
#else
      for( int i=0; i<15; i++ ) sC[i] = C[i][it];      
#endif
#endif
    }    
  }
  
}



int main(int argc, char *argv[]){
#ifdef TBB
  task_scheduler_init init;
  tasks = atoi( argv[1] );
#endif
  //cout<<sizeof(float)<<" "<<sizeof(Fvec_t)<<endl;
  ReadInput();
  FitTracksV();
  WriteOutput();
  cout<<"track size = "<<sizeof(TrackV)<<endl;
  cout<<"char size = "<<sizeof(char)<<endl;
  return 0;
}
