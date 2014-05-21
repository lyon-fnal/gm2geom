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
    int TotalPlane(WireID wire) const;

    // Calculate the row number of the wire within a station
    int InnerRow(WireID wire) const;
    //Calculate the Total station number given a WireID. This is needed
    //for placing the straws within the stations
    int TotalStationNumber(WireID wire) const;

    // Get the position in x and y of the center of a given wire in the station 
    // coordinates, with y downstream, x outward, and z downwards.
    double wireXPosition(WireID wire) const;
    double wireYPosition(WireID wire) const;

    // Get the 3-vector position of the center of a given wire in the tracker
    // coordinates.
    CLHEP::Hep3Vector trackerPosition(WireID const& wire) const;
    // Get the 3-vector position of the wire in the tracker coordinates at
    // a given height (y)
    CLHEP::Hep3Vector trackerPositionAtHeight(WireID const& wire, double y) const;


    // Parameters taken from the FHiCL configuration file; see the configuration
    // file strawtracker.fcl for an explanation.
    // ----------------------------------------------------------------------
    // Station parameters
    std::vector<int> whichScallopLocations;
    std::vector<double> strawStationSize;
    std::vector<int> strawStationType;
    const double strawStationWidth;
    double strawStationManifoldWidth;
    const double strawStationHeight;
    const double strawStationManifoldHeight;
		const double strawStationManifoldThickness;
    int strawLayers;
    std::vector<double> xPositionStraw0;
    std::vector<double> yPosition;

    int strawView;
    // Straw parameters
    double innerRadiusOfTheStraw;
    double outerRadiusOfTheStraw;
		double outerRadiusOfTheGas;
		double outerRadiusOfTheWire;
    double startAngleOfTheStraw;
    double spanningAngleOfTheStraw;

    double distBtwnWires;
    double layerAngle;
    
    double supportPostRadius;
    double supportPostYPosition;


    // Display parameters
    const bool displayStation;
    std::vector<double> stationColor;
		std::vector<double> manifoldColor;
    const bool displayStationMaterial;
    const bool displayStraw;
    std::vector<double> strawColor;
    std::vector<double> gasColor;
    std::vector<double> wireColor;

    // Derived parameters - these aren't directly from the config file
    // ----------------------------------------------------------------------
    std::vector<double> strawStationLocation;
    double strawStationSpacing;
    // Half-lengths of the straw stations, required for Geant.
    // The distance (x, in tracker coordinates) from the flat edge of the
    // scallop to the straw station center.
    double strawStationHeightHalf;
    double strawStationManifoldHeightHalf;
    double strawStationManifoldWidthHalf;
    double strawStationWidthHalf;
    std::vector<double> strawStationSizeHalf;
    std::vector<double> distShift;

    // The total number of stations to be placed (counting multiple scallop
    // locations)
    int numberOfStations;

    // height of active region, *not* the length of the straw, determined 
    // from the height of the station - 2 times the height of the manifold
    double heightOfTheStraw; 
    
    // The total number of planes per tracker system
    int numberOfPlanesPerScallop;
    // Derived quantity lengthOfTheStraw is based on height of the active region
    // and the layer angle.
    double lengthOfTheStraw;
    // More derived quantities - self-explanatory.
    double halfHeightOfTheStraw;
    double halfLengthOfTheStraw;
    // The amount of translation in x necessary to get from the bottom of the
    // station to the center of the straw.
    double deltaX;
  };

}
#endif
