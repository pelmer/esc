#include "tbb/blocked_range.h"

using namespace tbb;

class ApplyFit{

public:

	TrackV *_track_vectors;
	Station *_vStations;
	int _NStations;

	void operator()(const blocked_range<int> &range) const {

		for(int i = range.begin(); i != range.end(); ++i){
			Fit(_track_vectors[i], _vStations, _NStations);
		}

	}

	ApplyFit(TrackV *track_vectors, Station *vStations, int NStations){
		_track_vectors = track_vectors;
		_vStations = vStations;
		_NStations = NStations;
	}

};

