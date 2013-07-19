#include "StrawTrackerGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include <iostream>
#include <sstream>
#include <math.h>



gm2geom::StrawTrackerGeometry::StrawTrackerGeometry(std::string const & detName) :
  GeometryBase(detName),
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
  
  strawStationHeightHalf = strawStationHeight/2;
  
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationSizeHalf.push_back(strawStationSize[i]/2);
    strawStationWidthHalf.push_back(strawStationWidth[i]/2);
    strawStationLocation[i] = 1436-strawStationLocation[i];
    
  }
  
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){

    strawStationCenterFromEdge.push_back(strawStationSizeHalf[i] + strawStationOffset[i] + strawStationPiping);
  }
  
  lengthOfTheStraw = heightOfTheStraw/cos(layerAngle);
  halfLengthOfTheStraw = lengthOfTheStraw/2;
  halfHeightOfTheStraw = heightOfTheStraw/2;
  
  for (unsigned int i = 0; i<yPosition.size(); i++){
    yPosition[i] = yPosition[i] - strawStationWidthHalf[0];
  }
  
  deltaX = halfHeightOfTheStraw*tan(layerAngle);
  numberOfStations = strawStationSize.size() * whichScallopLocations.size();
}


int gm2geom::StrawTrackerGeometry::Plane(WireID wire) {
  
  return wire.getStation()*(strawView+strawLayers) + wire.getView()*2 + wire.getLayer();
  
}


double gm2geom::StrawTrackerGeometry::wireXPosition(WireID wire){
  
  int plane = Plane(wire);
  
  double x =  xPositionStraw0[plane%4] + wire.getWire()*distBtwnWires;
  if (wire.getView() == 0) x = x - deltaX;
  else x = x+deltaX;
  
  return x;
}

double gm2geom::StrawTrackerGeometry::wireYPosition(WireID wire){
  
  int plane = Plane(wire);
  
  return yPosition[plane%4];
}


CLHEP::Hep3Vector gm2geom::StrawTrackerGeometry::trackerPosition(WireID wire){
  
  double x = wireXPosition(wire) + strawStationOffset[wire.getStation()] + strawStationPiping;
  double y = 0;
  double z = wireYPosition(wire) + strawStationLocation[wire.getStation()];
  CLHEP::Hep3Vector trackerPosition(x,y,z);
  return trackerPosition;
}

void gm2geom::StrawTrackerGeometry::print() const{
  std::ostringstream oss;
  
  oss << "  strawStationHeight="<< strawStationHeight << "\n";
  oss << "  strawStationSize="; for (auto entry: strawStationSize) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawStationLocations="; for (auto entry : strawStationLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichScallopLocations="; for (auto entry : whichScallopLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  lengthOfStraw=" <<lengthOfTheStraw << "\n";
  mf::LogInfo("STRAWTRACKERGEOMETRY") << oss.str();
  
 
}
