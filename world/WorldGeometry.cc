// Geometry for the world

#include "gm2geom/world/WorldGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include <iostream>

// Rather than #including G4globals.hh, just declare we'll use the units we need
// from CLHEP/Units/SystemOfUnits.h.
#include "CLHEP/Units/SystemOfUnits.h"
using CLHEP::mm;

gm2geom::WorldGeometry::WorldGeometry(std::string const & detName) :
  GeometryBase(detName),
  world_x(p.get<double>("world_x") * mm),
  world_y(p.get<double>("world_y") * mm),
  world_z(p.get<double>("world_z") * mm),
  display( p.get<bool>("display")),
  worldColor( p.get<std::vector<double>>("worldColor"))
{}

void gm2geom::WorldGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("WorldGeometry") << "World geometry is"
                             << " world_x=" << world_x
                             << " world_y=" << world_y
                             << " world_z=" << world_z;  
}

