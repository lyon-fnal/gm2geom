// Geometry_service.cc
// This service is intended to give the user an interface between the
// fhicl input file and their detector_service
// 
// Developed by Brendan Kiburg, Adam Lyon Nov, 2012

#include "NewGeometry_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

gm2geom::NewGeometryService::NewGeometryService(fhicl::ParameterSet const & p,
					art::ActivityRegistry &)
  : p_ ( p ) 
{
}

gm2geom::NewGeometryService::~NewGeometryService(){

}

int gm2geom::NewGeometryService::getInt(std::string const & s){
  return p_.get<int>(s);
}


// The Geometry Service assumes that individual detectors will be inside of the
// fhicl Parameter Sets within Geometry . This method allows a user to extract
// the parameter set by name
fhicl::ParameterSet gm2geom::NewGeometryService::getDetParSet(std::string const & s){

  fhicl::ParameterSet p = p_.get<fhicl::ParameterSet>(s);
  return p;

}

using gm2geom::NewGeometryService;
DEFINE_ART_SERVICE(NewGeometryService)
