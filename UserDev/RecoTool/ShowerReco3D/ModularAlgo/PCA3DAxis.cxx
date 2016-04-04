#ifndef PCA3DAXISMODULE_CXX
#define PCA3DAXISMODULE_CXX

#include "PCA3DAxis.h"

namespace showerreco {

void PCA3DAxis::do_reconstruction(const ProtoShower & proto_shower,
                                    Shower_t& resultShower) {

  // auto & clusters = proto_shower.params();


  // This is really simple.  Read in the 3D params and use that to
  // set the 3D axis of the shower.  Right now, no checks against the
  // 2D projection but should be added somewhere.
  
  // If there is no 3D params, throw an exception.
  // 
  if (proto_shower.hasCluster3D()){
    double dir = 1.;
    if ( proto_shower.hasVertex() ) {
      resultShower.fXYZStart = proto_shower.vertexes().front();

      TVector3 mean( proto_shower.params3D().mean_x, proto_shower.params3D().mean_y, proto_shower.params3D().mean_z );
      TVector3 distPos = mean - 0.5*resultShower.fLength*proto_shower.params3D().principal_dir - resultShower.fXYZStart;
      TVector3 distNeg = mean + 0.5*resultShower.fLength*proto_shower.params3D().principal_dir - resultShower.fXYZStart;
      if ( distPos.Mag() > distNeg.Mag() ) dir = -1.;
    }
    resultShower.fDCosStart = dir * proto_shower.params3D().principal_dir;
  }
  else{
    throw ShowerRecoException("PCA3DAxis requires 3D cluster but has none.");
  }
}


} //showerreco

#endif
