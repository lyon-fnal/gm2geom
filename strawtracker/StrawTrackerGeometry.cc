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
  //strawModuleSize( p.get<std::vector<double>>("strawModuleSize")),
  strawModuleType( p.get<std::vector<int>>("strawModuleType")),
  strawModuleWidth( p.get<double>("strawModuleWidth")),
  strawModuleManifoldWidth( p.get<double>("strawModuleManifoldWidth")),
  strawModuleHeight( p.get<double>("strawModuleHeight")),
  strawModuleManifoldHeight( p.get<double>("strawModuleManifoldHeight")),
  strawModuleManifoldThickness( p.get<double>("strawModuleManifoldThickness")),
  strawLayers( p.get<double>("strawLayers")),
  xPositionStraw0_16type( p.get<std::vector<double>>("xPositionStraw0_16type")),
  xPositionStraw0_24type( p.get<std::vector<double>>("xPositionStraw0_24type")),
  xPositionStraw0_32type( p.get<std::vector<double>>("xPositionStraw0_32type")),
  yPosition( p.get<std::vector<double>>("yPosition")),
  strawModuleExtension( p.get<std::vector<double>>("strawModuleExtension")),
  strawView( p.get<int>("strawView")),
  type16size( p.get<double>("type16size")),
  type24size( p.get<double>("type24size")),
  type32size( p.get<double>("type32size")),
  innerRadiusOfTheStraw( p.get<double>("innerRadiusOfTheStraw") ),
  outerRadiusOfTheStraw( p.get<double>("outerRadiusOfTheStraw") ),
  outerRadiusOfTheGas( p.get<double>("outerRadiusOfTheGas") ),
  outerRadiusOfTheWire( p.get<double>("outerRadiusOfTheWire") ),
  startAngleOfTheStraw( p.get<double>("startAngleOfTheStraw") * deg),
  spanningAngleOfTheStraw( p.get<double>("spanningAngleOfTheStraw") *deg ),
  distBtwnWires( p.get<double>("distBtwnWires") *mm ),
  layerAngle( p.get<double>("layerAngle") *deg),
  buildSupportPost( p.get<bool>("buildSupportPost")),
  supportPostRadius( p.get<double>("supportPostRadius") ),
  displayModule( p.get<bool>("displayModule")),
  moduleColor( p.get<std::vector<double>>("moduleColor")),
  manifoldColor( p.get<std::vector<double>>("manifoldColor")),
  displayModuleMaterial( p.get<bool>("displayModuleMaterial")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor")),
  gasColor( p.get<std::vector<double>>("gasColor")),
  wireColor( p.get<std::vector<double>>("wireColor"))
  
{
  
  strawModuleHeightHalf = strawModuleHeight/2;
  strawModuleWidthHalf = strawModuleWidth/2;
  strawModuleManifoldHeightHalf = strawModuleManifoldHeight/2;
  strawModuleManifoldWidthHalf = strawModuleManifoldWidth/2;

  //Calculate the straw parameters based on the size of the module and manifolds
  heightOfTheStraw = strawModuleHeight - 2*strawModuleManifoldHeight;
  lengthOfTheStraw = heightOfTheStraw/cos(layerAngle);
  halfLengthOfTheStraw = lengthOfTheStraw/2;
  halfHeightOfTheStraw = heightOfTheStraw/2;

  const gm2geom::VacGeometry vacg("vac");
  double distToNextModule;
  double distToExtFront = vacg.distToExtEdge - vacg.trackerExtensionL;
 
  if(strawModuleType.size() != 1) {
    strawModuleSpacing = (2*vacg.trackerExtBuildL[vacg.vacuumRegion] - strawModuleType.size()*strawModuleWidth)/(strawModuleType.size()-1);
  } 

  if(strawModuleSpacing <0 ){
    throw cet::exception("StrawTrackerGeometry")<<
    "The width of the vacuum extension is too small for the width of the modules.\nPlease extend the vacuum extension, make the width of the modules smaller or lower the number of modules.\nFix before running again." << std::endl;
  }

  std::map<int,double> strawModuleTypeSize;

  strawModuleTypeSize[16] = type16size;
  strawModuleTypeSize[24] = type24size;
  strawModuleTypeSize[32] = type32size;
  
  for (unsigned int i = 0 ; i < strawModuleType.size() ; i ++){

    strawModuleSizeHalf.push_back((strawModuleTypeSize[strawModuleType[i]]+strawModuleExtension[i])/2); 
    distToNextModule = strawModuleWidthHalf + i*(strawModuleWidth + strawModuleSpacing);
    strawModuleLocation.push_back( distToExtFront + vacg.trackerExtWallThick + distToNextModule);

  }
  
  // Get total offset from the original inner scallop vacuum line (not at the extension)
  // this is used for placement of the box and will become the tracker x coordinates.
  // In the tracker coodinates z is along original scallop line, 
  for (unsigned int i = 0 ; i < strawModuleType.size() ; i ++){
    distShift.push_back(vacg.outerWallThickness + vacg.trackerExtensionW + vacg.trackerExtWallThick - strawModuleSizeHalf[i]);
  }
  
  // Get the module y coordinate for each layer
  for (unsigned int i = 0; i<yPosition.size(); i++){
    yPosition[i] = yPosition[i] - strawModuleWidthHalf;
  }
  
  deltaX = halfHeightOfTheStraw*tan(layerAngle);
  numberOfModules = strawModuleType.size() * whichScallopLocations.size();
  numberOfPlanesPerScallop = strawModuleType.size()*strawView;
}

int gm2geom::StrawTrackerGeometry::InnerRow(WireID wire) const {
  return wire.getLayer() + wire.getView()*strawLayers; 
}

int gm2geom::StrawTrackerGeometry::TotalPlane(WireID wire) const {
  return wire.getModule()*strawView + wire.getView();
}

int gm2geom::StrawTrackerGeometry::TotalModuleNumber(WireID wire) const {
  int pos = std::find(whichScallopLocations.begin(), whichScallopLocations.end(), wire.getStation()) - whichScallopLocations.begin();
 return wire.getModule() + pos*strawModuleLocation.size();
  
}
double gm2geom::StrawTrackerGeometry::wireXPosition(WireID wire) const {
  
  int innerPlaneNumber = InnerRow(wire);
  
  int type = strawModuleType[wire.getModule()];
  int module = wire.getModule();
  double x=0;
  // Get the x position of the bottom of the first wire
  if (type == 16)  x =  xPositionStraw0_16type[innerPlaneNumber];
  if (type == 24)  x =  xPositionStraw0_24type[innerPlaneNumber];
  if (type == 32)  x =  xPositionStraw0_32type[innerPlaneNumber];
  
  x = x + strawModuleExtension[module];

  //From this find the position of the bottom of the wire in question
  x =  x + wire.getWire()*distBtwnWires;

  // Move it forward or backward according to the angle of the straws, depending
  // on the view.
  if (wire.getView() == u_view) {
       x = x + deltaX;
  }
  else {
      x = x-deltaX;
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
  // Get module position, and add in offsets for x.
  double x = wireXPosition(wire);
  // This is the center of the wire, so by definition it has y=0.
  double y = 0;
  // Get module position and add in the location of the module for z
  // We add wireYPosition because the tracker z is the module y, up to a
  // constant offset.
  double z = wireYPosition(wire) + strawModuleLocation[wire.getModule()];
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
  
  oss << "  strawModuleHeight="<< strawModuleHeight << "\n";
  oss << "  strawModuleType="; for (auto entry: strawModuleType) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawModuleLocations="; for (auto entry : strawModuleLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichScallopLocations="; for (auto entry : whichScallopLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  lengthOfStraw=" <<lengthOfTheStraw << "\n";
  oss << "  heightOfStraw=" <<heightOfTheStraw << "\n";
  mf::LogInfo("STRAWTRACKERGEOMETRY") << oss.str();
  
 
}
