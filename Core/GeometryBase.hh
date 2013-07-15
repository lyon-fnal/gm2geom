#ifndef GM2GEOM_GEOMETRY_BASE_HH
#define GM2GEOM_GEOMETRY_BASE_HH

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "gm2geom/Core/NewGeometry_service.hh"

#include "fhiclcpp/ParameterSet.h"

#include <string>
namespace gm2geom {

  struct GeometryBase { 
    
    GeometryBase(std::string const & detName) : 
      name(detName),
      p() 
    {
      art::ServiceHandle <gm2geom::NewGeometryService> g;
      p = g->getDetParSet(detName);
      
    }
    
    virtual ~GeometryBase() {}
    
    
    std::string name;
    fhicl::ParameterSet p;
    
  };
}


#endif // GM2GEOM_GEOMETRY_BASE_HH

