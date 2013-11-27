// This service is called NewGeometryService in order to avoid a (pretty
// ridiculous) conflict with the previous GeometryService in artg4. It is an
// interim solution, and will hopefully go away very soon.
#ifndef GM2GEOM_GEOMETRY_SERVICE_HH
#define GM2GEOM_GEOMETRY_SERVICE_HH

// Art                                                                        
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
namespace gm2geom {

  class GeometryService { 
  public: 
    GeometryService(fhicl::ParameterSet const&, art::ActivityRegistry&);
    ~GeometryService();

    fhicl::ParameterSet getDetParSet(std::string const &);
    int getInt(std::string const &);
    
  private:
    fhicl::ParameterSet p_;
  };

} //namespace

using gm2geom::GeometryService;
DECLARE_ART_SERVICE(GeometryService, LEGACY)


#endif //GM2GEOM_GEOMETRY_SERVICE_HH
