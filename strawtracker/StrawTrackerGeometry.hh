// This geometry class, derived from gm2geom's GeometryBase, is used to extract
// information from a configuration file and can then be called to retrieve that
// information. In particular, it holds configuration parameters for the
// placement of the straw stations and straws, and provides methods to get
// things like the coordinates of the center of each straw in the tracker
// coordinate system.

#ifndef STRAWTRACKERGEOMETRY_HH
#define STRAWTRACKERGEOMETRY_HH

#include "gm2geom/Core/GeometryBase.hh"
#include "gm2dataproducts/strawtracker/WireID.hh"

#include "CLHEP/Vector/ThreeVector.h"
#include <iostream>

using gm2strawtracker::WireID;

namespace gm2geom {
  
  struct StrawTrackerGeometry : public gm2geom::GeometryBase {
    StrawTrackerGeometry(std::string const & detName="strawtracker");
    
    // Print information about the straw tracker configuration.
    void print() const;
    
    // Calculate the plane number of the straw in question: the first plane is
    // the one furthest upstream in the upstream-most station, and the last is
    // the plane closest to the calorimeter station.
    int Plane(WireID wire);

    // Get the position (in x and y) of the center of a given wire in the
    // station coordinates.
    double wireXPosition(WireID wire);
    double wireYPosition(WireID wire);

    // Get the 3-vector position of the center of a given wire in the tracker
    // coordinates.
    CLHEP::Hep3Vector trackerPosition(WireID wire);


    // Parameters taken from the FHiCL configuration file; see the configuration
    // file strawtracker.fcl for an explanation.
    // ----------------------------------------------------------------------
    std::vector<int> whichScallopLocations;
    std::vector<double> strawStationLocation;
    std::vector<double> strawStationSize;
    double strawStationPiping;
    std::vector<double> strawStationOffset;
    std::vector<int> strawStationType;
    int strawView;
    int strawLayers;
    // Half the straw station sizes
    std::vector<double> strawStationSizeHalf;
    const double strawStationHeight;
    // Half the straw station height
    double strawStationHeightHalf;
    
    std::vector<double> strawStationWidth;
    std::vector<double> strawStationWidthHalf;
    
    double innerRadiusOfTheStraw;
    double outerRadiusOfTheStraw;
    double heightOfTheStraw;
    // Derived quantity lenghtOfTheStraw is based on height of the active region
    // and the layer angle.
    double lengthOfTheStraw;
    // More derived quantities - self-explanatory.
    double halfHeightOfTheStraw;
    double halfLengthOfTheStraw;
    // Back to parameters
    double startAngleOfTheStraw;
    double spanningAngleOfTheStraw;

    double distBtwnWires;
    double layerAngle;
    // The amount of translation in x necessary to get from the bottom of the
    // station to the center of the straw.
    double deltaX;
    
    // The total number of stations to be placed (counting multiple scallop
    // locations)
    int numberOfStations;

    // More parameters from the configuration file.
    std::vector<double> strawStationCenterFromEdge;

    std::vector<double> xPositionStraw0;
    std::vector<double> yPosition;
    
    const bool displayStation;
    std::vector<double> stationColor;
    const bool displayStraw;
    std::vector<double> strawColor;
  };

}
#endif
