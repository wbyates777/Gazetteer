/* GeoCoord 07/05/2025

 $$$$$$$$$$$$$$$$$$
 $   GeoCoord.h   $
 $$$$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Helper class
 
 A geographical coordinate point (latitude, longitude) expressed in decimal degrees (not radians).
 
 1) Calculates geodetic distances (in metres) between points using the Vincenty formulae.
 see https://en.wikipedia.org/wiki/Vincenty%27s_formulae
 
 2) Support for the public domain Geohash string encoding and decoding of geographical positions 
 see https://en.wikipedia.org/wiki/Geohash 

 
 Example 1
 
     double lat = 57.64911;
     double lon = 10.40744;
     
     std::string code = GeoCoord::geohash(lat, lon, 11);
     std::cout << "The geohash for (" << lat << ", " << lon << ") is " << code << " and has length " << code.size() << std::endl;
     std::cout << "The geohash for (57.64911, 10.40744) should be u4pruydqqvj" << std::endl;
     
     GeoCoord out = GeoCoord::geohash(code);
     std::cout << "GeoCoord position from geohash is " << out.lat() << " " << out.lon() << std::endl;
     std::cout << "GeoCoord tzselect format from geohash is " << out.toString() << std::endl << std::endl;
  
 Example 2 
 
     double lat = 10.123;
     double lon = -3.14156;
     
     GeoCoord p1(lat, lon);
     std::cout << p1 << std::endl;
     std::cout << p1.toString() << std::endl;
     
     GeoCoord p2;
     p2.setGeoCoord( p1.toString() );
     std::cout << p2 << std::endl;
 
 Example 3
 
     // New York (NYC)
     double lat1 = 40.7127;
     double lon1 = -74.005997;

     // London (LON)
     double lat2 = 51.4893;
     double lon2 = -0.14405499;
 
     std::cout << "The distance between NYC and LON is " << GeoCoord::dist(lat1,lon1, lat2,lon2) / 1000.0 << " km" << std::endl;
 

*/


#ifndef __GEOCOORD_H__
#define __GEOCOORD_H__


#include <string>
#include <utility>
#include <iostream>
#include <regex> 

class GeoCoord
{

public:

    //
    //  GeoCoord - signed pair of doubles - (latitude, longitude) - in degrees - aka decimal degrees
    //

    GeoCoord( void ): m_lat(0.0), m_lon(0.0) {} // note initial 'undefined' point is valid point (0,0)
    GeoCoord( const std::string &str ): m_lat(0.0), m_lon(0.0) { setGeoCoord(str); } // ISO-6709 DEG format
    GeoCoord( double latitude, double longitude ) : m_lat(latitude), m_lon(longitude) {}
    GeoCoord( const std::pair<double, double> &pos ) : m_lat(pos.first), m_lon(pos.second) {}
    ~GeoCoord( void ) {  m_lat = m_lon = 0.0; };

    // (first.latitude, second.longitude)
    operator std::pair<double, double>( void ) const { return std::pair<double,double>( m_lat, m_lon); } 
   
    
    double 
    lat( void ) const { return m_lat; }

    void 
    lat( double d ) { m_lat = d; }
    
    
    double 
    lon( void ) const { return m_lon; }
    
    void 
    lon( double d ) { m_lon = d; }
    
    
    bool
    setGeoCoord( double lat, double lon ) { m_lat = lat; m_lon = lon; return valid(); }
    
    bool
    setGeoCoord( const std::pair<double, double> &pos ) { m_lat = pos.first; m_lon = pos.second; return valid(); }
    
    // read ISO-6709 DEG format for (lat,lon) '±DD.DDDD±DDD.DDDD' used by tzselect -c
    bool
    setGeoCoord( const std::string &str );
    
    // write ISO-6709 DEG format for (lat,lon) '±DD.DDDD±DDD.DDDD' used by tzselect -c
    std::string
    toString( void ) const;
    
    // read Geohash
    bool
    setGeoHash( const std::string &str ) { *this = geohash(str); return valid(); }
    
    // write Geohash
    std::string
    toGeohash( int prec = 12 ) const { return geohash(m_lat, m_lon, prec); }
    
    void
    normalize( void ); 
    
    bool
    operator==( const GeoCoord &rhs ) const { return m_lat == rhs.m_lat && m_lon == rhs.m_lon; }
    
    bool
    valid( void ) const { return valid(m_lat, m_lon); }
    
    //
    //
    //
    
    static bool
    valid( double lat, double lon ) { return ((lat >= -90.0 && lat <= 90.0) && (lon >= -180.0 && lon <= 180.0)); }
    
    // distance in metres between geographical points in degrees -  Vincenty method
    static double
    dist( const GeoCoord &p1, const GeoCoord &p2 ) { return dist(p1.m_lat, p1.m_lon, p2.m_lat, p2.m_lon); }

    static double
    dist( double lat1, double lon1, double lat2, double lon2 );
    

    // encode/decode Geohash string - points in degrees
    static std::string
    geohash( const GeoCoord &pos, int prec = 12 ) { return geohash(pos.m_lat, pos.m_lon, prec); }
    
    static std::string
    geohash( double lat, double lon, int prec = 12 );
    
    // assumes hash str is a valid geohash code
    static GeoCoord 
    geohash( const std::string &hash );
  
    static bool
    valid( const std::string &hash );
    
private:
   
    
    typedef struct IntervalStruct { double high;  double low; } Interval;
    
    double m_lat;
    double m_lon;

    static const unsigned int m_char_index_table[75];
    static const char         m_char_map[33]; 
    static const std::regex   m_format;

};

// output "lat lon" -- Note this will not (in general) match ISO_6709 DEG format (for each point)
// as positive sign '+' and leading zeros may be missing i.e '3' vs '+03' lat or '+003' lon
inline std::ostream&
operator<<( std::ostream &ostr, const GeoCoord& c )
{
    ostr << c.lat() << ' ' << c.lon();
    return ostr;
}

std::istream&
operator>>( std::istream &istr, GeoCoord& c );

#endif


