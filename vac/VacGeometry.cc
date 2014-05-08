// Geometry for the Vac

#include "gm2geom/vac/VacGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include <iostream>
#include <sstream>

// Rather than #including G4globals.hh, just declare we'll use the units we need
// from CLHEP/Units/SystemOfUnits.h. We also need conversions.hh for the sake of
// using inches (not crashing the spaceship...yet)
#include "CLHEP/Units/SystemOfUnits.h"
#include "gm2ringsim/common/conversions.hh"
using CLHEP::mm;
using CLHEP::deg;
using gm2ringsim::in;

using CLHEP::Hep2Vector;

gm2geom::VacGeometry::VacGeometry(std::string const & detName) :
  GeometryBase(detName),
  inflectorExtensionValue( p.get<double>("inflectorExtensionValue") * mm),
  topBottomWall( p.get<double>("topBottomWall") * mm),
  outerWallThickness( p.get<double>("outerWallThickness") * mm),
  torus_rmin( p.get<double>("torus_rmin") * in), //from conversions.hh
  torus_rmax( p.get<std::vector<double>>("torus_rmax") ),
  torus_sphi( p.get<double>("torus_sphi") * deg),
  torus_dphi( p.get<double>("torus_dphi") * deg),
  phi_a( p.get<double>("phi_a") * deg),
  phi_b( p.get<double>("phi_b") * deg),
  wallR( p.get<std::vector<double>>("wallR") ),
  wallPhi( p.get<std::vector<double>>("wallPhi") ),
  vacR( p.get<std::vector<double>>("vacR") ),
  vacPhi( p.get<std::vector<double>>("vacPhi") ),
  phi_q_subtractor( p.get<double>("phi_q_subtractor") * deg),
  set_phi_q( p.get<bool>("set_phi_q") ),
  phi_q( p.get<double>("phi_q") * deg),
  zz( p.get<std::vector<double>>("zz") ),
  ext( p.get<std::vector<double>>("ext") ),
  tracker_sphi( p.get<double>("tracker_sphi") * deg),
  tracker_dphi( p.get<double>("tracker_dphi") * deg),
  Frequency( p.get<int>("Frequency")),
  turn_sphi( p.get<double>("turn_sphi") * deg),
  turn_dphi( p.get<double>("turn_dphi") * deg),
  trackerExtensionL( p.get<double>("trackerExtensionL") * in),
  trackerExtensionW( p.get<double>("trackerExtensionW") * in),
  trackerExtPlacementFromCalo( p.get<double>("trackerExtPlacementFromCalo") *in),
  trackerExtWallThick( p.get<double>("trackerExtWallThick") *in),
  displayWall( p.get<bool>("displayWall")),
  wallColor( p.get<std::vector<double>>("wallColor")),
  displayVac( p.get<bool>("displayVac")),
  vacColor( p.get<std::vector<double>>("vacColor")),
  displayTracker( p.get<bool>("displayTracker")),
  trackerColor( p.get<std::vector<double>>("trackerColor")),
  displayTurnCounter( p.get<bool>("displayTurnCounter")),
  turnCounterColor( p.get<std::vector<double>>("turnCounterColor")),
  ZachIsAwesome_Z(topBottomWall-outerWallThickness),
  torus_z( {topBottomWall, topBottomWall - outerWallThickness} )
{
  for (auto& entry : torus_rmax ) { entry *= in; }
  for (auto& entry : wallR ) { entry *= in; }
  for (auto& entry : wallPhi ) { entry *= deg; }
  for (auto& entry : vacR ) { entry *= in; }
  for (auto& entry : vacPhi ) { entry *= deg; }
  for (auto& entry : zz ) { entry *= in; }
  for (auto& entry : ext ) { entry *= in; }
  
  // Derived quantities
  pt_a[wallRegion].setR(wallR[0]);
  pt_a[wallRegion].setPhi(wallPhi[0]);
  pt_b[wallRegion].setR(wallR[1]);
  pt_b[wallRegion].setPhi(wallPhi[1]);
  pt_c[wallRegion].setR(wallR[2]);
  pt_c[wallRegion].setPhi(wallPhi[2]);
  pt_o[wallRegion].setR(wallR[3]);
  pt_o[wallRegion].setPhi(wallPhi[3]);

  pt_a[vacuumRegion].setR(vacR[0]);
  pt_a[vacuumRegion].setPhi(vacPhi[0]);
  pt_b[vacuumRegion].setR(vacR[1]);
  pt_b[vacuumRegion].setPhi(vacPhi[1]);
  pt_c[vacuumRegion].setR(vacR[2]);
  pt_c[vacuumRegion].setPhi(vacPhi[2]);
  pt_o[vacuumRegion].setR(vacR[3]);
  pt_o[vacuumRegion].setPhi(wallPhi[3]);

  const double alpha = 1.05;
  for (unsigned int i = 0; i < 2; ++i ) {
    pt_d[i] = pt_b[i] + alpha*(pt_c[i] - pt_b[i]);
    
    xS[i] = (pt_a[i]-pt_b[i]).mag();
    z[i] = (pt_d[i]-pt_b[i]).mag()*std::cos(phi_b);
    xL[i] = xS[i] + z[i]*(std::tan(phi_b)+std::tan(phi_a));
    
    // Now, calculate inner scallop data (the little right angle bit)
    
    Hep2Vector what = (pt_c[i] - pt_b[i]).unit();
    Hep2Vector nhat = what;
    nhat.rotate(90.*deg);
    nhat = nhat.unit();
    
    pt_r[i] = pt_c[i] - ext[i]*nhat;
    
    double const c = std::cos(15.*deg), s = std::sin(15.*deg);
    double const m =
    (s*pt_c[i].x()-c*pt_c[i].y())/
    (c*nhat.y()-s*nhat.x());
    
    pt_p[i] = pt_c[i] + m*nhat;
    
    xI[i] = (pt_p[i]-pt_r[i]).mag();
    xO[i] = xI[i] - zz[i]*std::tan(phi_q);
  }
  
  //Extension building
  //determine how big to make the extension box for the walls and
  //the interior vacuum volume
    

  trackerExtBuildL[wallRegion] = trackerExtensionL/2; 
  trackerExtBuildW[wallRegion] = (trackerExtensionW + trackerExtWallThick + outerWallThickness)/2; 
  trackerExtBuildH[wallRegion] = torus_z[0]; 

  trackerExtBuildL[vacuumRegion] = trackerExtensionL/2 - trackerExtWallThick; 
  trackerExtBuildW[vacuumRegion] = trackerExtensionW/2 + outerWallThickness*2;
  trackerExtBuildH[vacuumRegion] = torus_z[1]; 
  trackerExtPlacementX = vacR[0];

  //convert scallop points to an (x,y) coordinate system 
  //in order to calculate the length of the scallop. The dimensions
  //for the scallop extension were given by Kevin Lynch in [r,theta]
  //
  //x = r*cos(theta)
  //y = r*sin(theta)
  //
  //Do this for both inner points of the scallop edge. Diagram and more
  //detailed documentation can be found in the gm2ringsim documentation. 

  double x_vac0 = vacR[0] * cos(vacPhi[0]);
  double y_vac0 = vacR[0] * sin(vacPhi[0]);

  double x_vac1 = vacR[1] * cos(vacPhi[1]);
  double y_vac1 = vacR[1] * sin(vacPhi[1]);

  scallopL = sqrt((x_vac0-x_vac1)*(x_vac0-x_vac1) + (y_vac0-y_vac1)*(y_vac0-y_vac1));

  distToExtEdge = scallopL - trackerExtPlacementFromCalo;
  distCenterExtAlongScallop = distToExtEdge - trackerExtBuildL[wallRegion];
  // trackers
  track_rMin = pt_a[vacuumRegion].r();
  turn_rMin = track_rMin;
  
  track_rMax = torus_rmax[vacuumRegion];
  turn_rMax = track_rMax;
}

double gm2geom::VacGeometry::inflectorExtension(unsigned int arcNum) const {
  double ret = 0.0;
  if (arcNum == 11) ret=inflectorExtensionValue;
  return ret;
}


void gm2geom::VacGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  std::ostringstream oss;
  oss << "  inflectorExtensionValue=" << inflectorExtensionValue << "\n";
  oss << "  topBottomWall=" << topBottomWall << "\n";
  oss << "  outerWallThickness=" << outerWallThickness << "\n";
  oss << "  torus_rmin=" << torus_rmin << "\n";
  oss << "  torus_rmax= "; for (auto entry : torus_rmax) { oss << " " << entry; }; oss << "\n";
  oss << "  torus_sphi=" << torus_sphi << "\n";
  oss << "  torus_dphi=" << torus_dphi << "\n";
  oss << "  phi_a=" << phi_a << "\n";
  oss << "  phi_b=" << phi_b << "\n";
  oss << "  wallR= "; for (auto entry : wallR) { oss << " " << entry; }; oss << "\n";
  oss << "  wallPhi= "; for (auto entry : wallPhi) { oss << " " << entry; }; oss << "\n";
  oss << "  vacR= "; for (auto entry : vacR) { oss << " " << entry; }; oss << "\n";
  oss << "  vacPhi= "; for (auto entry : vacPhi) { oss << " " << entry; }; oss << "\n";
  oss << "  phi_q_subtractor=" << phi_q_subtractor << "\n";
  oss << "  set_phi_q=" << set_phi_q << "\n";
  oss << "  phi_q=" << phi_q << "\n";
  oss << "  zz= "; for (auto entry : zz) { oss << " " << entry; }; oss << "\n";
  oss << "  ext= "; for (auto entry : ext) { oss << " " << entry; }; oss << "\n";
  oss << "  tracker_sphi=" << tracker_sphi << "\n";
  oss << "  tracker_dphi=" << tracker_dphi << "\n";
  oss << "  Frequency=" << Frequency << "\n";
  oss << "  turn_sphi=" << turn_sphi << "\n";
  oss << "  turn_dphi=" << turn_dphi << "\n";
  oss << "  wallColor= "; for (auto entry : wallColor) { oss << " " << entry; }; oss << "\n";
  oss << "  displayVac=" << displayVac << "\n";
  oss << "  vacColor= "; for (auto entry : vacColor) { oss << " " << entry; }; oss << "\n";
  oss << "  displayTracker=" << displayTracker << "\n";
  oss << "  trackerColor= "; for (auto entry : trackerColor) { oss << " " << entry; }; oss << "\n";
  oss << "  displayTurnCounter=" << displayTurnCounter << "\n";
  oss << "  turnCounterColor= "; for (auto entry : turnCounterColor) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("VACGEOM") << oss.str();
}

