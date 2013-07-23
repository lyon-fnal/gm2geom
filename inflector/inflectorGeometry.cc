/** @file inflectorGeometry.cc
    
    Implements the non-inlinable members of the inflectorGeometry
    class.

    @author Kevin Lynch
    @date 2009
    @author Brendan Kiburg 
    @date artified 2012
*/

#include "gm2geom/inflector/inflectorGeometry.hh"

// Rather than #including G4globals.hh, just declare we'll use the units we need
// from CLHEP/Units/SystemOfUnits.h.
#include "CLHEP/Units/SystemOfUnits.h"
using CLHEP::mm;
using CLHEP::degree;
using CLHEP::milliradian;

namespace gm2geom{

  // Length of mandrel (total - windows - conductor - endflanges)
  const double inflectorMandrelLength = 1696.*mm;
  
  const double windowWidth = 1.5*mm;
  const double conductorWidth = 2.4*mm;
  const double cryostatWindowThickness = 1.0*mm;

}

gm2geom::inflectorGeometry& gm2geom::inflectorGeometry::getInstance() {
  static inflectorGeometry ia;
  return ia;
}

gm2geom::inflectorGeometry::inflectorGeometry() :
  delta_(5.587*degree),
  gamma_(2.4*milliradian), 
  //  gamma_(45.*degree),
  //  gamma_(0.),
  zeta_(0.),
  //  zeta_(90.*degree),
  apertureDist_(77*mm),
  ipLaunch_(0.),
  oopLaunch_(0.),
  // constants
  windowThickness_(windowWidth),
  conductorThickness_(conductorWidth),
  cryostatThickness_(cryostatWindowThickness),
  mL_(inflectorMandrelLength/2.),
  iL_( inflectorMandrelLength/2.+
       windowThickness_+conductorThickness_+cryostatThickness_ )
{}
