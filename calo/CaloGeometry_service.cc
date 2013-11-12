#include "CaloGeometry_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

void gm2geom::CaloGeometryService::update() {
    geom_.reset(new gm2geom::CalorimeterGeometry("calorimeter"));
}

using gm2geom::CaloGeometryService;
DEFINE_ART_SERVICE(CaloGeometryService)