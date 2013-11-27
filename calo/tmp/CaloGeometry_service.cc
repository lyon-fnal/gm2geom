#include "CaloGeometry_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "gm2geom/Core/Geometry_service.hh"
#include "art/Framework/Services/Registry/ServiceHandle.h"

gm2geom::CaloGeometryService::CaloGeometryService(fhicl::ParameterSet const&, art::ActivityRegistry& ) :  //iReg) :
  geom_(new gm2geom::CalorimeterGeometry("calorimeter"))
{
  //FIXME watchPreBeginRun not part of art 1.08.09
  // need to figure this out
  //art::ServiceHandle <gm2geom::GeometryService> g; // ensure that GeometryService gets constructed first
  //iReg.watchPreBeginRun(this, &CaloGeometryService::preBeginRun);
}

void gm2geom::CaloGeometryService::update() {
    geom_.reset(new gm2geom::CalorimeterGeometry("calorimeter"));
}

void gm2geom::CaloGeometryService::preBeginRun( art::Run const& ) { update();}

using gm2geom::CaloGeometryService;
DEFINE_ART_SERVICE(CaloGeometryService)
