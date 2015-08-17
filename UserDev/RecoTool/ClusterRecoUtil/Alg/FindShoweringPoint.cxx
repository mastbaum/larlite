#ifndef FINDSHOWERINGPOINT_CXX
#define FINDSHOWERINGPOINT_CXX

#include "FindShoweringPoint.h"
#include "CRUException.h"
#include "LArUtil/GeometryHelper.h"
#include <map>

namespace cluster {

  void FindShoweringPoint::do_params_fill(cluster_params & cluster, bool verbose){

    // Geometry Utilities
    auto geomHelper = ::larutil::GeometryHelper::GetME();

    // get hit list
    auto const hits = cluster.hit_vector;

    // get start point
    auto const& start = cluster.start_point;
    
    // get end point
    auto const& end   = cluster.end_point;

    // get ordered hit list (from closest to start point to furthest)
    // save only those that are < 10 cm away
    // Map[dist] -> position of hit in hit vector
    std::map<double,size_t> hitmap;
    for(size_t i=0; i < hits.size(); i++){
      // distance to start point:
      double distSq = ( (hits[i].w-cluster.start_point.w)*(hits[i].w-cluster.start_point.w) +
			(hits[i].t-cluster.start_point.t)*(hits[i].t-cluster.start_point.t) );

      if ( distSq < 25 ){
	// check that the point is towards the shower direction (and not backwards)
	// this assumes we have got the right direction
	// the full cosine is not necessary. All we need is the sign.
	// so we just use the numerator from "GetCosAngleBetweenLines" function in GeometryHelper
	// for 3 points
	if ( ( (hits[i].w-start.w)*(end.w-start.w) + (hits[i].t-start.t)*(end.t-start.t) ) > 0 ) 
	  hitmap.insert(std::pair<double,size_t>(distSq,i));
      }
    }

    // loop through hits in distance-order to start-point
    // keep track of the cosine of the angle between
    // the line connecting points (n -> n+1) and (n+1 -> n+2)
    // if this cosine falls below a certain value -> stop and
    // mark the last point explored as the showering start point
    Point2D showering_start = start;
    size_t nPoints = 0;
    for (size_t n=0; n < hitmap.size()-2; n++){
      std::cout << "points: " << hitmap[n] << ", " << hitmap[n+1] << ", " << hitmap[n+2] << std::endl;
      double cos = geomHelper->GetCosAngleBetweenLines(hits[hitmap[n]],hits[hitmap[n+1]],
						       hits[hitmap[n+1]],hits[hitmap[n+2]]);
      if (cos > 0.7)
      	showering_start = hits[hitmap[n+2]];
      else
	break;
      nPoints += 1;
    }

    cluster.showering_point = showering_start;
    
    return;
  }
  
} // cluster

#endif
