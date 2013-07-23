// Geometry for the Arc

#include "gm2geom/arc/ArcGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include <iostream>

// Rather than #including G4globals.hh, just declare we'll use the units we need
// from CLHEP/Units/SystemOfUnits.h.
#include "CLHEP/Units/SystemOfUnits.h"
using CLHEP::mm;
using CLHEP::deg;

gm2geom::ArcGeometry::ArcGeometry(std::string const & detName) :
  GeometryBase(detName),
  arc_rMin(         p.get<double>("arc_rMin")         * mm),
  arc_rMax(         p.get<double>("arc_rMax")         * mm),
  arc11_rExtension( p.get<double>("arc11_rExtension") * mm),
  arc_z(            p.get<double>("arc_z")            * mm),
  arc_Sphi(         p.get<double>("arc_Sphi")         * deg),
  arc_Dphi(         p.get<double>("arc_Dphi")         * deg),
  display(          p.get<bool>("display")),
  arcColor(         p.get<std::vector<double>>("arcColor"))
  {}

void gm2geom::ArcGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("ArcGeometry") << "Arc geometry is" << "\n"
  << "    arc_rMin="         << arc_rMin << "\n"
  << "    arc_rMax="         << arc_rMax << "\n"
  << "    arc11_rExtension=" << arc11_rExtension << "\n"
  << "    arc_z="            << arc_z << "\n"
  << "    arc_Sphi="         << arc_Sphi << "\n"
  << "    arc_Dphi="         << arc_Dphi;
}

