// Traceback Geometry

#ifndef STRAWTRACKERGEOMETRY_HH
#define STRAWTRACKERGEOMETRY_HH

#include "gm2geom/Core/GeometryBase.hh"

#include <iostream>
#include "CLHEP/Units/SystemOfUnits.h"
#include "gm2dataproducts/strawtracker/WireID.hh"

#include "CLHEP/Vector/ThreeVector.h"

using CLHEP::mm;
using CLHEP::deg;
using CLHEP::cm;

using gm2strawtracker::WireID;

namespace gm2geom {
  
  struct StrawTrackerGeometry : public gm2geom::GeometryBase {
    StrawTrackerGeometry(std::string const & detName="strawtracker");
    
    void print() const;
    
    int Plane(WireID wire);
    double wirePosition(WireID wire);
    double yPosition(WireID wire);
    CLHEP::Hep3Vector TrackerPosition(WireID wire);
    
    std::vector<int> whichScallopLocations;
    std::vector<double> strawStationLocation;
    std::vector<double> strawStationSize;
    double strawStationPiping;
    std::vector<double> strawStationOffset;
    std::vector<int> strawStationType;
    int strawView;
    int strawLayers;
    std::vector<double> strawStationSizeHalf;
    const double strawStationHeight;
    double strawStationHeightHalf;
    
    std::vector<double> strawStationWidth;
    std::vector<double> strawStationWidthHalf;
    
    double innerRadiusOfTheStraw;
    double outerRadiusOfTheStraw;
    double heightOfTheStraw;
    double lengthOfTheStraw;
    double halfHeightOfTheStraw;
    double halfLengthOfTheStraw;
    double startAngleOfTheStraw;
    double spanningAngleOfTheStraw;

    double dist_btwn_wires;
    double layer_angle;
    double delta_x;
    
    int number_of_stations; 
    std::vector<double> straw_station_center_from_edge;

    std::vector<double> x_position_straw0;
    std::vector<double> y_position;
    
    const bool displayStation;
    std::vector<double> stationColor;
    const bool displayStraw;
    std::vector<double> strawColor;
    
    
  };

}
#endif