// Arc Geometry

#ifndef ARCGEOMETRY_HH
#define ARCGEOMETRY_HH

#include "gm2geom/Core/GeometryBase.hh"

#include <iostream>
#include <string>

namespace gm2geom {
  
  struct ArcGeometry : public gm2geom::GeometryBase {
    ArcGeometry(std::string const &);
    void print() const;

    const std::string storage_field;
    const double arc_rMin;
    const double arc_rMax;
    const double arc11_rExtension;
    const double arc_z;
    const double arc_Sphi;
    const double arc_Dphi;
    const bool display;
    std::vector<double> arcColor;
  };
}

#endif
