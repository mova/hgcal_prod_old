#ifndef TrackReco_HelixParameters_h
#define TrackReco_HelixParameters_h
/*----------------------------
 $Id: HelixParameters.h,v 1.1 2005/11/22 13:51:44 llista Exp $
 Helix Track Parametrization

 Author: Luca Lista, INFN
------------------------------*/
#include <CLHEP/Geometry/Vector3D.h>
#include <CLHEP/Geometry/Point3D.h>
#include "DataFormats/TrackReco/interface/Error.h"
#include "DataFormats/TrackReco/interface/Vector.h"
#include <cmath>

namespace reco {
  namespace helix {
    enum index { i_d0 = 0, i_phi0, i_omega, i_dz, i_tanDip }; 
    typedef HepGeom::Vector3D<double> Vector;
    typedef HepGeom::Point3D<double> Point;
    typedef Error<6> PosMomError;
 
    class Parameters {
    public:
      Parameters() { }
      Parameters( const double * par ) : par_( par ) { }
      typedef reco::Vector<5>::index index;
      template<index i>
      double get() const { return par_.get<i>(); }
      template<index i>
      double & get() { return par_.get<i>(); }
      double operator()( index i ) const { return par_( i ); }
      double & operator()( index i ) { return par_( i ); }
      double d0() const { return par_.get<i_d0>(); }
      double phi0() const { return par_.get<i_phi0>(); }
      double omega() const { return par_.get<i_omega>(); }
      double dz() const { return par_.get<i_dz>(); }
      double tanDip() const { return par_.get<i_tanDip>(); }
      double & d0() { return par_.get<i_d0>(); }
      double & phi0() { return par_.get<i_phi0>(); }
      double & omega() { return par_.get<i_omega>(); }
      double & dz() { return par_.get<i_dz>(); }
      double & tanDip() { return par_.get<i_tanDip>(); }
      int charge() const;
      double pt() const;
      Vector momentum() const;
      Point vertex() const;
      
    private:
      reco::Vector<5> par_;
    };
  
    class Covariance {
    public:
      Covariance() {} 
      Covariance( const double * err ) : err_( err ) { }
      typedef Error<5>::index index;
      /* those methods templates don't compile under LCG reflex dicts.
      template<index i, index j>
      double get() const { return err_.get<i, j>(); }
      template<index i, index j>
      double & get() { return err_.get<i, j>(); }
      */
      double operator()( index i, index j ) const { return err_( i, j ); }
      double & operator()( index i, index j ) { return err_ ( i, j ); }
      double d0Error() const { return sqrt( err_.get<i_d0, i_d0>() ); }
      double phi0Error() const { return sqrt( err_.get<i_phi0, i_phi0>() ); }
      double omegaError() const { return sqrt( err_.get<i_omega, i_omega>() ); }
      double dzError() const { return sqrt( err_.get<i_dz, i_dz>() ); }
      double tanDipError() const { return sqrt( err_.get<i_tanDip, i_tanDip>() ); }

    private:
      Error<5> err_;
    };
    
    void setFromCartesian( int q, const Point &, const Vector &, 
			   const PosMomError & ,
			   Parameters &, Covariance & ); 

    PosMomError posMomError( const Parameters &, const Covariance & );

    inline int Parameters::charge() const {
      return omega() > 0 ? +1 : -1;
    }
    
    inline double Parameters::pt() const {
      return 1./ fabs( omega() );
    }
    
    inline Vector Parameters::momentum() const {
      double p_t = pt();
      return Vector( - p_t * sin( phi0() ), p_t * cos( phi0() ), p_t * tanDip() );
    }
    
    inline Point Parameters::vertex() const {
      return Point( d0() * cos( phi0() ), d0() * sin( phi0() ), dz() );
    }

  }
}


#endif
