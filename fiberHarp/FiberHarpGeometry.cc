#include "gm2geom/fiberHarp/FiberHarpGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

/** @file FiberHarpGeometry.cc

    Implements the geometry data for the fiber harps

    @author Peter Winter
    @date 2012
*/

// Rather than #including G4globals.hh, just declare we'll use the units we need
// from CLHEP/Units/SystemOfUnits.h.
#include "CLHEP/Units/SystemOfUnits.h"
using CLHEP::mm;
using CLHEP::m;
using CLHEP::deg;

#include <iostream>

gm2geom::FiberHarpGeometry::FiberHarpGeometry(std::string detName) :
  GeometryBase(detName),
  fiberLength( p.get<double>("fiberLength") * mm),
  fiberRadius( p.get<double>("fiberRadius") * mm),
  fiberSpacing( p.get<double>("fiberSpacing") * mm),
  supportLength( p.get<double>("supportLength") * mm),
  supportWidth( p.get<double>("supportWidth") * mm),
  supportDepth( p.get<double>("supportDepth") * mm),
  nFibers( p.get<double>("nFibers")),
  coll_z( p.get<double>("coll_z") * m),
  RMagicScale( p.get<double>("RMagicScale")),
  nHarps( p.get<int>("nHarps")),
  azimuthalPos( p.get<std::vector<double>>("azimuthalPos")),
  vacWallPos( p.get<std::vector<int>>("vacWallPos") ),
  harpType( p.get<std::vector<std::string>>("harpType") ),
  display( p.get<bool>("display")),
  fiberHarpColor( p.get<std::vector<double>>("fiberHarpColor"))
{
  for (auto& entry : azimuthalPos ){ 
    entry *= deg; 
  }

  if(azimuthalPos.size() != (unsigned int)nHarps) throw cet::exception("FiberHarpGeometry") << 
    "Wrong geometry input: Size of azimuthalPos array is " << azimuthalPos.size() << 
    " and not equals nHarps=" << nHarps << std::endl;
  if(vacWallPos.size() != (unsigned int)nHarps) throw cet::exception("FiberHarpGeometry") << 
    "Wrong geometry input: Size of vacWallPos array is " << vacWallPos.size() << 
    " and not equals nHarps=" << nHarps << std::endl;

  if(harpType.size() != (unsigned int)nHarps) throw cet::exception("FiberHarpGeometry") << 
    "Wrong geometry input: Size of harpType array is " << harpType.size() << 
    " and not equals nHarps=" << nHarps << std::endl;
  
  // Let's check that the harpType entries are all known
  for (auto entry :  harpType){
    if(entry != "HARP_X" && entry != "HARP_Y" && entry != "HARP_OFF" && entry != "HARP_BROKEN_Y")
      throw cet::exception("FiberHarpGeometry") << "harpType entry " << entry << 
	" not HARP_X, HARP_Y, HARP_OFF or HARP_BOKEN_Y!\n";
  } 
}


void gm2geom::FiberHarpGeometry::print(){
  std::ostringstream oss;

  mf::LogInfo("FiberHarpGeometry") << "Fiber harp geometry is" ;
  oss << "  fiberLength=" << fiberLength << "\n";
  oss << "  fiberRadius=" << fiberRadius << "\n";
  oss << "  fiberSpacing=" << fiberSpacing << "\n";
  oss << "  supportLength=" << supportLength << "\n";
  oss << "  supportWidth=" << supportWidth << "\n";
  oss << "  supportDepth=" << supportDepth << "\n";
  oss << "  nFibers=" << nFibers << "\n";
  oss << "  coll_z=" << coll_z << "\n";
  oss << "  RMagicScale=" << RMagicScale << "\n";
  oss << "  nHarps=" << nHarps << "\n";
  oss << "  azimuthalPos= "; for (auto entry : azimuthalPos) { oss << " " << entry; }; oss << "\n";
  oss << "  vacWallPos= "; for (auto entry : vacWallPos) { oss << " " << entry; }; oss << "\n";
  oss << "  harpType= "; for (auto entry : harpType) { oss << " " << entry; }; oss << "\n";
  oss << "  display=" << display << "\n";

  mf::LogInfo("FiberHarpGeometry") << oss.str();
}

