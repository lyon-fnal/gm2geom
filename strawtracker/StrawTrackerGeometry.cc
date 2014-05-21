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
  strawStationSize( p.get<std::vector<double>>("strawStationSize")),
  strawStationType( p.get<std::vector<int>>("strawStationType")),
  strawStationWidth( p.get<double>("strawStationWidth")),
  strawStationManifoldWidth( p.get<double>("strawStationManifoldWidth")),
  strawStationHeight( p.get<double>("strawStationHeight")),
  strawStationManifoldHeight( p.get<double>("strawStationManifoldHeight")),
  strawStationManifoldThickness( p.get<double>("strawStationManifoldThickness")),
  strawLayers( p.get<double>("strawLayers")),
  xPositionStraw0( p.get<std::vector<double>>("xPositionStraw0")),
  yPosition( p.get<std::vector<double>>("yPosition")),
  strawView( p.get<double>("strawView")),
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
  displayStation( p.get<bool>("displayStation")),
  stationColor( p.get<std::vector<double>>("stationColor")),
  manifoldColor( p.get<std::vector<double>>("manifoldColor")),
  displayStationMaterial( p.get<bool>("displayStationMaterial")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor")),
  gasColor( p.get<std::vector<double>>("gasColor")),
  wireColor( p.get<std::vector<double>>("wireColor"))
  
{
  
  strawStationHeightHalf = strawStationHeight/2;
  strawStationWidthHalf = strawStationWidth/2;
  strawStationManifoldHeightHalf = strawStationManifoldHeight/2;
  strawStationManifoldWidthHalf = strawStationManifoldWidth/2;

  //Calculate the straw parameters based on the size of the station and manifolds
  heightOfTheStraw = strawStationHeight - 2*strawStationManifoldHeight;
  lengthOfTheStraw = heightOfTheStraw/cos(layerAngle);
  halfLengthOfTheStraw = lengthOfTheStraw/2;
  halfHeightOfTheStraw = heightOfTheStraw/2;

  const gm2geom::VacGeometry vacg("vac");
  double distToNextStation;
  strawStationSpacing = (2*vacg.trackerExtBuildL[vacg.vacuumRegion] - strawStationType.size()*strawStationWidth)/(strawStationType.size()-1);
  //Set the half size of the station and determine where the center
  //of each station is positioned in the x coordinate system (of the tracker)
 
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){

    strawStationSizeHalf.push_back(strawStationSize[i]/2);
    distToNextStation = strawStationWidthHalf + i*(strawStationWidth + strawStationSpacing);
    strawStationLocation.push_back( vacg.distToExtEdge -vacg.trackerExtWallThick-distToNextStation);

  }

  //The vector 'strawStationLocation' above is set by the first element in the vector 
  //being the station closest to the calorimeter. In actuality we want this to be the 
  //last station and thus the order of the vector needs to be reversed to take this 
  //into account. 

  std::reverse(strawStationLocation.begin(), strawStationLocation.end());  
  
  // Get total offset from the original inner scallop vacuum line (not at the extension)
  // this is used for placement of the box and will become the tracker x coordinates.
  // In the tracker coodinates z is along original scallop line, 
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    distShift.push_back(vacg.trackerExtensionW + vacg.outerWallThickness -  strawStationSizeHalf[i]);
  }
  
  // Get the station y coordinate for each layer
  for (unsigned int i = 0; i<yPosition.size(); i++){
    yPosition[i] = yPosition[i] - strawStationWidthHalf;
  }
  
  deltaX = halfHeightOfTheStraw*tan(layerAngle);
  numberOfStations = strawStationSize.size() * whichScallopLocations.size();
  numberOfPlanesPerScallop = strawStationSize.size()*strawView;
}

int gm2geom::StrawTrackerGeometry::InnerRow(WireID wire) const {
  return wire.getLayer() + wire.getView()*strawLayers; 
}

int gm2geom::StrawTrackerGeometry::TotalPlane(WireID wire) const {
  return wire.getStation()*strawView + wire.getView();
}

int gm2geom::StrawTrackerGeometry::TotalStationNumber(WireID wire) const {
  int pos = std::find(whichScallopLocations.begin(), whichScallopLocations.end(), wire.getTrackerNumber()) - whichScallopLocations.begin();
 return wire.getStation() + pos*strawStationLocation.size();
  
}
double gm2geom::StrawTrackerGeometry::wireXPosition(WireID wire) const {
  
  int innerPlaneNumber = InnerRow(wire);

  // Get the x position of the bottom of the wire
  double x =  xPositionStraw0[innerPlaneNumber] + wire.getWire()*distBtwnWires;

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

double gm2geom::StrawTrackerGeometry::wireYPosition(WireID wire) const {
  
  int innerPlaneNumber = InnerRow(wire);

  // It's pretty easy; we just need the 'y' position of the layer.
  return yPosition[innerPlaneNumber];
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
