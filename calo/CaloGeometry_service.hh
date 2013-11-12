#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "gm2geom/calo/CalorimeterGeometry.hh"

namespace gm2geom {
    class CaloGeometryService {
    public:
        CaloGeometryService(fhicl::ParameterSet const&, art::ActivityRegistry&) :
            geom_(new gm2geom::CalorimeterGeometry("calorimeter")) {}
        const std::shared_ptr<CalorimeterGeometry> geom() {return geom_;}
        void update();
    private:
        std::shared_ptr<CalorimeterGeometry> geom_;
    };
}