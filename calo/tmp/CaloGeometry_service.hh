#ifndef CALO_GEOMETRY_SERVICE
#define CALO_GEOMETRY_SERVICE

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "gm2geom/calo/CalorimeterGeometry.hh"

namespace gm2geom {
    class CaloGeometryService {
    public:
        CaloGeometryService(fhicl::ParameterSet const&, art::ActivityRegistry& iReg);

        const std::shared_ptr<CalorimeterGeometry> geom() {return geom_;}
        void update();
        void preBeginRun(art::Run const&);
    private:
        std::shared_ptr<CalorimeterGeometry> geom_;
    };
}

#endif