#ifndef INFLECTORGEOMETRY_HH
#define INFLECTORGEOMETRY_HH

/** @file inflectorGeometry.hh

  Declares a Meyer's singleton, inflectorGeometry, to hold globally
  consistent values of various inflector parameters. 

  This class was implemented in the original g2MIGTRACE, and it is now used by
  the InflectorGeom struct, the standard geometry implementation in the new
  (artized) g-2 simulation.

  @author Kevin Lynch
  @date 2009
  */


/** A Meyer's singleton that stores the inflector geometry that is
  needed by multiple classes: inflectorConstruction, inflectorField,
  inflectorMessenger, g2SingleParticleSource.

  Angles here use the standard Geant4, right handed system, not the
  g2MIGTRACE downstream(clockwise) system.

*/
namespace gm2geom{
    class inflectorGeometry {
    public:
        static inflectorGeometry& getInstance();

        /** Gets the azimuthal position from g2MIGTRACE angle=0.  positive
          moves upstream. */
        double delta() const { return delta_; }
        /** Sets the azimuthal position from g2MIGTRACE angle=0.  positive
          moves upstream. */
        double delta(double d) { double t = delta_; delta_ = d; return t; }

        /** Gets the inflector swing angle from tangent ... positive rotates
          beam direction outward. */
        double gamma() const { return gamma_; }
        /** Sets the inflector swing angle from tangent ... positive rotates
          beam direction outward. */
        double gamma(double d) { double t = gamma_; gamma_ = d; return t; }

        /** Gets the inflector tilt angle from plane of ring ... moves
          rotates the beam upward out of the plane. */
        double zeta() const { return zeta_; }
        /** Sets the inflector tilt angle from plane of ring ... moves
          rotates the beam upward out of the plane. */
        double zeta(double d) { double t = zeta_; zeta_ = d; return t; }

        /** Alias for gamma() */
        double swing_angle() const { return gamma(); }
        /** Alias for gamma() */
        double swing_angle(double d) { return gamma(d); }

        /** Alias for zeta() */
        double tilt_angle() const { return zeta(); }
        /** Alias for zeta() */
        double tilt_angle(double d) { return zeta(d); }

        /** Gets the radial distance from muon orbit to inflector aperture
          center. */
        double aperture_off() const { return apertureDist_; }
        /** Sets the radial distance from muon orbit to inflector aperture
          center. */
        double aperture_off(double d){ 
            double t = apertureDist_; 
            apertureDist_ = d; 
            return t;
        }

        /** Gets the projection of the launch angle onto the symmetry plane
          ... same geometrical meaning as gamma(). */
        double inplane_angle() const { return ipLaunch_; }
        /** Sets the projection of the launch angle onto the symmetry plane
          ... same geometrical meaning as gamma(). */
        double inplane_angle(double ipLaunch) 
        { double t = ipLaunch_; ipLaunch_ = ipLaunch; return t; }

        /** Gets the projection of the launch angle onto the vertical axis
          ... same geometrical meaning as zeta(). */
        double outplane_angle() const { return oopLaunch_; }
        /** Sets the projection of the launch angle onto the vertical axis
          ... same geometrical meaning as zeta(). */
        double outplane_angle(double oopLaunch) 
        { double t = oopLaunch_; oopLaunch_ = oopLaunch; return t; }

        /** Gets the inflector mandrel half length; not modifiable. */
        double mandrel_half_length() const { return mL_; }

        /** Gets the inflector mandrel length; not modifiable. */
        double mandrel_length() const { return 2.*mL_; }

        /** Gets the full inflector half length; not modifiable. */
        double half_length() const { return iL_; }

        /** Gets the full inflector length; not modifiable. */
        double length() const { return 2.*iL_; }

        /** Gets the inflector window thickness; not modifiable. */
        double window_thickness() const { return windowThickness_; }

        /** Gets the inflector conductor thickness; not modifiable. */
        double conductor_thickness() const { return conductorThickness_; }

        /** Gets the inflector cryostat thickness; not modifiable. */
        double cryostat_thickness() const { return cryostatThickness_; }

    public:
        inflectorGeometry();
        ~inflectorGeometry(){}

        // don't implement these!
        inflectorGeometry(inflectorGeometry const&);
        inflectorGeometry& operator=(inflectorGeometry const&);

    private:
        double delta_, gamma_, zeta_;
        double apertureDist_, ipLaunch_, oopLaunch_;

        // shared, immutable constants

        // thicknesses
        double const windowThickness_, conductorThickness_, cryostatThickness_;
        // mandrel and full inflector half lengths
        double const mL_, iL_;
    };

} // namespace gm2geom

#endif // INFLECTORGEOMETRY_HH
