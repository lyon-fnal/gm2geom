// Implementation of the StrawTrackerGeometry struct. 

#include "StrawTrackerGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include <iostream>
#include <sstream>
#include <math.h>

// Rather than #including G4globals.hh, just declare we'll use the units we need
// from CLHEP/Units/SystemOfUnits.h.
#include "CLHEP/Units/SystemOfUnits.h"
using CLHEP::mm;
using CLHEP::deg;
using CLHEP::cm;

// Use the view labels defined in WireID
using gm2strawtracker::u_view;
using gm2strawtracker::v_view;
using gm2strawtracker::na_view;

// Constructor takes a name and uses that to create its base GeometryBase class,
// which gets the parameters for the given name.
gm2geom::StrawTrackerGeometry::StrawTrackerGeometry(std::string const & detName) :
  GeometryBase(detName),
    // Extract FHiCL parameters.
  whichScallopLocations( p.get<std::vector<int>>("whichScallopLocations")),
  strawStationLocation( p.get<std::vector<double>>("strawStationLocation")),
  strawStationSize( p.get<std::vector<double>>("strawStationSize")),
  strawStationPiping( p.get<double>("strawStationPiping")),
  strawStationOffset( p.get<std::vector<double>>("strawStationOffset")),
  strawStationType( p.get<std::vector<int>>("strawStationType")),
  strawView( p.get<double>("strawView")),
  strawLayers( p.get<double>("strawLayers")),
  strawStationHeight( p.get<double>("strawStationHeight")),
  strawStationWidth( p.get<std::vector<double>>("strawStationWidth")),
  innerRadiusOfTheStraw( p.get<double>("innerRadiusOfTheStraw") ),
  outerRadiusOfTheStraw( p.get<double>("outerRadiusOfTheStraw") ),
  heightOfTheStraw( p.get<double>("heightOfTheStraw") * cm),
  startAngleOfTheStraw( p.get<double>("startAngleOfTheStraw") * deg),
  spanningAngleOfTheStraw( p.get<double>("spanningAngleOfTheStraw") *deg ),
  distBtwnWires( p.get<double>("distBtwnWires") *mm ),
  layerAngle( p.get<double>("layerAngle") *deg),
  xPositionStraw0( p.get<std::vector<double>>("xPositionStraw0")),
  yPosition( p.get<std::vector<double>>("yPosition")),
  displayStation( p.get<bool>("displayStation")),
  stationColor( p.get<std::vector<double>>("stationColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor"))
  
{
  
  // A couple of derived quantities
  
  // Half-sizes of the edges of the stations, necessary for Geant4 placement.
  strawStationHeightHalf = strawStationHeight/2;
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationSizeHalf.push_back(strawStationSize[i]/2);
    strawStationWidthHalf.push_back(strawStationWidth[i]/2);
    strawStationLocation[i] = 1436-strawStationLocation[i];
  }
  
  // Get total offset in tracker x coordinate.
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationCenterFromEdge.push_back(strawStationSizeHalf[i] + strawStationOffset[i] + strawStationPiping);
  }
  
  // Some straw parameters
  lengthOfTheStraw = heightOfTheStraw/cos(layerAngle);
  halfLengthOfTheStraw = lengthOfTheStraw/2;
  halfHeightOfTheStraw = heightOfTheStraw/2;
  
  // Get the station y coordinate for each layer.
  for (unsigned int i = 0; i<yPosition.size(); i++){
    yPosition[i] = yPosition[i] - strawStationWidthHalf[0];
  }
  
  deltaX = halfHeightOfTheStraw*tan(layerAngle);
  numberOfStations = strawStationSize.size() * whichScallopLocations.size();
  numberOfPlanesPerStation = strawStationSize.size()*(strawView+strawLayers);
}

// Calculate the total plane number of a wire. Does not use the wire# itself,
// just the station, view, and layer.
int gm2geom::StrawTrackerGeometry::Plane(WireID wire) {
  return wire.getStation()*(strawView+strawLayers) + wire.getView()*2 + wire.getLayer();
}


// Calculate the position of the center of the wire in question in station
// coordinates, with y downstream, x outward, and z downwards.
double gm2geom::StrawTrackerGeometry::wireXPosition(WireID wire){
  
  int plane = Plane(wire);
  
  // Get the x position of the bottom of the wire
  double x =  xPositionStraw0[plane%4] + wire.getWire()*distBtwnWires;

  // Move it forward or backward according to the angle of the straws, depending
  // on the view.
  if (wire.getView() == u_view) {
       x = x - deltaX;
  }
  else {
      x = x+deltaX;
  }
  
  return x;
}

// Calculate the y coordinate of the center of the relevant wire in question in
// station (Geant4) coordinates, with y downstream, x outward, and z downwards.
double gm2geom::StrawTrackerGeometry::wireYPosition(WireID wire){
  
  int plane = Plane(wire);
  
  // It's pretty easy; we just need the 'y' position of the layer.
  return yPosition[plane%4];
}


// Calculate the location of the center of the relevant wire in the tracker
// coordinates, with z downstream along the straight edge of the scallop, x
// perpendicular to z inwards in the median plane, and y upwards (defined as
// zero in the median plane).
CLHEP::Hep3Vector gm2geom::StrawTrackerGeometry::trackerPosition(WireID wire){
  // Get station position, and add in offsets for x.
  double x = wireXPosition(wire) + strawStationOffset[wire.getStation()] + strawStationPiping;
  // This is the center of the wire, so by definition it has y=0.
  double y = 0;
  // Get station position and add in the location of the station for z
  // We add wireYPosition because the tracker z is the station y, up to a
  // constant offset.
  double z = wireYPosition(wire) + strawStationLocation[wire.getStation()];
  // combine these components and return the resulting vector.
  CLHEP::Hep3Vector trackerPosition(x,y,z);
  return trackerPosition;
}

// Print some straw geometry information
void gm2geom::StrawTrackerGeometry::print() const{
  std::ostringstream oss;
  
  oss << "  strawStationHeight="<< strawStationHeight << "\n";
  oss << "  strawStationSize="; for (auto entry: strawStationSize) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawStationLocations="; for (auto entry : strawStationLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichScallopLocations="; for (auto entry : whichScallopLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  lengthOfStraw=" <<lengthOfTheStraw << "\n";
  mf::LogInfo("STRAWTRACKERGEOMETRY") << oss.str();
  
 
}
