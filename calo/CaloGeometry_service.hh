#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

namespace gm2geom {
	class CaloGeometryService {
	public:
		CaloGeometryService(fhicl::ParameterSet const&, art::ActivityRegistry&);
		void print();

		// Number of calorimeters (defaults to 24)
		const int nCalorimeters;
		
		// crystal array and inter-crystal gap (for "wrapping")
		const int nXtalRows;
		const int nXtalCols;
		const double wrappingGap; // "epsilon" in g2migtrace
		const double crystalCaloBuffer;

		// crystal dimensions
		const double xtalWidth;
		const double xtalHeight;
		const double xtalDepth;
		
		// Wrapping materials.  Different faces can have different wrappings
		const std::string sideWrapping;  // long faces
		const std::string frontWrapping; // upstream face
		const std::string backWrapping;  // back face (shared with photodetector)
		const bool frontWrappingHole; // true for a square hole in center of front
		                              //   wrapping; false for no hole
		const double frontWrappingHoleSize; // side length of the square hole
		
		// optical coupling and photodetector
		const std::string photodetectorShape; // round or square
		const double photodetectorSize;
		const double photodetectorDepth;   // "pmtDepth" in g2migtrace
		const double opticalCouplingDepth; // "epoxyDepth" in g2migtrace
		
		// diffuser
		const bool diffuser; // true creates diffuser plate in front of calo with
		                     // thickness "diffuserDepth"; false leaves empty space
		                     // with that thickness
		const double diffuserDepth;
		
		// viewing
		const bool displayCalorimeterBox;
		const bool displayWrappingVolumes;
		const bool displayCrystalArray;
		const std::vector<double> calorimeterColor;
		const std::vector<double> xtalColor;
		const std::vector<double> photodetectorColor;
		const std::vector<double> opticalCouplingColor;
		const std::vector<double> diffuserColor;
		const std::vector<double> wrappingColor;
		
		// placement inside mother volume
		const bool placeInStation;
		// ----> if true, reads in station geometry and does position adjustment
		//                to be at front edge of station
		// ----> if false (e.g., for test beam), placement is at center of mother volume
		const double positionOffsetX; // manually adjust the calo volume placement
		const double positionOffsetY; //    (use mother volume coordinate system)
		const double positionOffsetZ;
		
		// The following constants are derived

		// parameters of bounding calorimeter box.  Unlike in g2migtrace, we
		// define these parameters from crystal and array parameters
		// themselves so that they are guaranteed to just barely enclose the
		// crystals + wrappings + photodetector
		double radial;
		double vertical;
		double thickness;

	private:
		fhicl::ParameterSet p;
	};
}