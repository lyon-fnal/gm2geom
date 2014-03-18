// Implementation of the StrawTrackerGeometry struct. 

#include "StrawTrackerGeometry.hh"
#include "../vac/VacGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include <iostream>
#include <sstream>
#include <math.h>

// Rather than #including G4globals.hh, just declare we'll use the units we need
// from CLHEP/Units/SystemOfUnits.h.
#include "CLHEP/Units/SystemOfUnits.h"
#include "gm2ringsim/common/conversions.hh"
using CLHEP::mm;
using CLHEP::deg;
using CLHEP::cm;
using gm2ringsim::in;

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
  strawStationType( p.get<std::vector<int>>("strawStationType")),
  strawView( p.get<double>("strawView")),
  strawLayers( p.get<double>("strawLayers")),
  strawStationHeight( p.get<double>("strawStationHeight")),
  strawStationManifoldHeight( p.get<double>("strawStationManifoldHeight")),
  strawStationManifoldThickness( p.get<double>("strawStationManifoldThickness")),
  strawStationWidth( p.get<double>("strawStationWidth")),
  strawStationSpacing( p.get<double>("strawStationSpacing") *in),
  innerRadiusOfTheStraw( p.get<double>("innerRadiusOfTheStraw") ),
  outerRadiusOfTheStraw( p.get<double>("outerRadiusOfTheStraw") ),
  outerRadiusOfTheGas( p.get<double>("outerRadiusOfTheGas") ),
  outerRadiusOfTheWire( p.get<double>("outerRadiusOfTheWire") ),
  startAngleOfTheStraw( p.get<double>("startAngleOfTheStraw") * deg),
  spanningAngleOfTheStraw( p.get<double>("spanningAngleOfTheStraw") *deg ),
  distBtwnWires( p.get<double>("distBtwnWires") *mm ),
  layerAngle( p.get<double>("layerAngle") *deg),
  supportPostRadius( p.get<double>("supportPostRadius") ),
  supportPostYPosition( p.get<double>("supportPostYPosition") ),
  xPositionStraw0( p.get<std::vector<double>>("xPositionStraw0")),
  yPosition( p.get<std::vector<double>>("yPosition")),
  displayStation( p.get<bool>("displayStation")),
  stationColor( p.get<std::vector<double>>("stationColor")),
  manifoldColor( p.get<std::vector<double>>("manifoldColor")),
  displayStationMaterial( p.get<bool>("displayStationMaterial")),
  displayPipingMaterial( p.get<bool>("displayPipingMaterial")),
  pipingColor( p.get<std::vector<double>>("pipingColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor")),
  gasColor( p.get<std::vector<double>>("gasColor")),
  wireColor( p.get<std::vector<double>>("wireColor"))
  
{
  
  // A couple of derived quantities
  
  // Half-sizes of the edges of the stations, necessary for Geant4 placement.
  strawStationHeightHalf = strawStationHeight/2;
  strawStationManifoldHeightHalf = strawStationManifoldHeight/2;
  const gm2geom::VacGeometry vacg("vac");
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationSizeHalf.push_back(strawStationSize[i]/2);
    strawStationWidthHalf.push_back(strawStationWidth/2);
    strawStationLocation[i] = vacg.distToExtEdge -vacg.trackerExtWallThick-(i+1)*strawStationWidth/2+strawStationSpacing*i;
  }
  
  // Get total offset in tracker x coordinate.
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationCenterFromEdge.push_back(strawStationSizeHalf[i]);
  }
  
  // Some straw parameters
  heightOfTheStraw = strawStationHeight - 2*strawStationManifoldHeight;
  lengthOfTheStraw = heightOfTheStraw/cos(layerAngle);
  halfLengthOfTheStraw = lengthOfTheStraw/2;
  halfHeightOfTheStraw = heightOfTheStraw/2;
  
  // Get the station y coordinate for each layer.
  for (unsigned int i = 0; i<yPosition.size(); i++){
    yPositionLastStation.push_back(yPosition[i] - strawStationWidthHalf[strawStationWidthHalf.size()-1]);
    yPosition[i] = yPosition[i] - strawStationWidthHalf[0];
  }
  
  deltaX = halfHeightOfTheStraw*tan(layerAngle);
  numberOfStations = strawStationSize.size() * whichScallopLocations.size();
  numberOfPlanesPerScallop = strawStationSize.size()*(strawView+strawLayers);
}

// Calculate the total plane number of a wire. Does not use the wire# itself,
// just the station, view, and layer.
int gm2geom::StrawTrackerGeometry::Plane(WireID wire) const {
  return wire.getStation()*(strawView+strawLayers) + wire.getView()*2 + wire.getLayer();
}

int gm2geom::StrawTrackerGeometry::TotalStationNumber(WireID wire) const {
  int pos = std::find(whichScallopLocations.begin(), whichScallopLocations.end(), wire.getTrackerNumber()) - whichScallopLocations.begin();
 return wire.getStation() + pos*strawStationLocation.size();
  
}
// Calculate the position of the center of the wire in question in station
// coordinates, with y downstream, x outward, and z downwards.
double gm2geom::StrawTrackerGeometry::wireXPosition(WireID wire) const {
  
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
double gm2geom::StrawTrackerGeometry::wireYPosition(WireID wire) const {
  
  int plane = Plane(wire);
  
  // It's pretty easy; we just need the 'y' position of the layer.
  int lastStationPosition = strawStationType.size()-1;
  if (wire.getStation() == lastStationPosition) return yPositionLastStation[plane%4];
  else return yPosition[plane%4];
  
}


// Calculate the location of the center of the relevant wire in the tracker
// coordinates, with z downstream along the straight edge of the scallop, x
// perpendicular to z inwards in the median plane, and y upwards (defined as
// zero in the median plane).
CLHEP::Hep3Vector gm2geom::StrawTrackerGeometry::
trackerPosition(WireID const& wire) const{
  // Get station position, and add in offsets for x.
  double x = wireXPosition(wire);
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

// Get the 3-vector position of the wire in the tracker coordinates at
// a given height (y)
CLHEP::Hep3Vector gm2geom::StrawTrackerGeometry::
trackerPositionAtHeight(WireID const& wire, double y) const
{
    // Start with the center of the straw - y and z are the same.
    CLHEP::Hep3Vector wireCenter = trackerPosition(wire);

    // And add the offset due to the height to the x coordinate.
    double offset = y * tan(layerAngle);

    // Check if we should add or subtract (make it zero if we have an
    // na_view). Add for the u view, subtract for the v view.
    double multiple = (wire.getView() == u_view ? +1 : 
            (wire.getView() == v_view ? -1 : 0));

    // Actually do the update and return the result
    wireCenter.setX(wireCenter.x() + multiple * offset);
    return wireCenter;
}

// Print some straw geometry information
void gm2geom::StrawTrackerGeometry::print() const{
  std::ostringstream oss;
  
  oss << "  strawStationHeight="<< strawStationHeight << "\n";
  oss << "  strawStationSize="; for (auto entry: strawStationSize) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawStationLocations="; for (auto entry : strawStationLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichScallopLocations="; for (auto entry : whichScallopLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  lengthOfStraw=" <<lengthOfTheStraw << "\n";
  oss << "  heightOfStraw=" <<heightOfTheStraw << "\n";
  mf::LogInfo("STRAWTRACKERGEOMETRY") << oss.str();
  
 
}
