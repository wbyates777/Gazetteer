/* GeoCoord 07/05/2025

 $$$$$$$$$$$$$$$$$$$$
 $   GeoCoord.cpp   $
 $$$$$$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:


 Helper class
 
 A geographical point (latitude, longitude) expressed in decimal degrees (not radians).
 
 Geodetic distances (in metres) between points specified by latitude and longitude are calculated using the Vincenty method.
 
 This class also supports geohash encoding and decoding of geographical positions - see https://en.wikipedia.org/wiki/Geohash 
 

 Note - Requires c++20 and std::format 
 The function 'std::string toString( void )' could be converted to std::snprintf
 The rest of the class works under c++11, c++17, etc
 
*/


#ifndef __GEOCOORD_H__
#include "GeoCoord.h"
#endif


#include <cmath>
#include <cassert>
#include <sstream>
#include <format> 

// the index for each char in m_char_map - 99 indicates 'no index'
constexpr unsigned int GeoCoord::m_char_index_table[75] = 
{ 
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 
    10, 11, 12, 13, 14, 15, 16, 99, 17, 18, 
    99, 19, 20, 99, 21, 22, 23, 24, 25, 26, 
    27, 28, 29, 30, 31 
};

// base-32 character map used for geohashing; notice a, i, l, o omitted
constexpr char GeoCoord::m_char_map[33] =  "0123456789bcdefghjkmnpqrstuvwxyz"; 

// ISO-6709 DEG format for lat/long '±DD.DDDD±DDD.DDDD' used by tzselect -c
const std::regex GeoCoord::m_format( R"(((\+|\-)(\d\d)(\.\d+)?)((\+|\-)(\d\d\d)(\.\d+)?))" );



std::istream&
operator>>( std::istream &istr, GeoCoord& c )
{
    double lat, lon;
    istr >> lat >> lon;
    
    c.lat(lat);
    c.lon(lon);
    
    return istr;
}

std::string
GeoCoord::toString( void ) const
// ISO-6709 DEG format for lat/long used by tzselect -c '±DD.DDDD±DDD.DDDD'
// https://en.wikipedia.org/wiki/ISO_6709
// could be converted to std::snprintf
// std::snprintf(buff, 32, "%+0*.*f", width, dp, m_lon);  where width = 8, dp =  3;
{
    const int dp = 4;
    std::stringstream sstr;

    sstr << std::format("{:+0{}.{}f}{:+0{}.{}f}", m_lat, dp + 4, dp, m_lon, dp + 5, dp);
    return sstr.str();
}


bool
GeoCoord::setGeoCoord( const std::string &str )
// ISO-6709 DEG format for lat/long used by tzselect -c '±DD.DDDD±DDD.DDDD'
// https://en.wikipedia.org/wiki/ISO_6709
{
    std::smatch match;
    if (std::regex_match(str, match, m_format)) 
    {
        m_lat = std::stod(match[1].str());
        m_lon = std::stod(match[5].str());
        return valid();
    }
    return false;
}

void
GeoCoord::normalize( void )
{
    while (m_lat < -90.0) 
        m_lat += 180.0;
    while (m_lat > 90.0) 
        m_lat -= 180.0;
    
    while (m_lon < -180.0) 
        m_lon += 360.0;
    while (m_lon > 180.0) 
        m_lon -= 360.0;
}


// A c++ rewrite of libgeohash see https://github.com/simplegeo/libgeohash
// original copyright notice (just in case).

/*
 *  geohash.c
 *  libgeohash
 *
 *  Created by Derek Smith on 10/6/09.
 *  Copyright (c) 2010, SimpleGeo
 *      All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:

 *  Redistributions of source code must retain the above copyright notice, this list
 *  of conditions and the following disclaimer. Redistributions in binary form must 
 *  reproduce the above copyright notice, this list of conditions and the following 
 *  disclaimer in the documentation and/or other materials provided with the distribution.
 *  Neither the name of the SimpleGeo nor the names of its contributors may be used
 *  to endorse or promote products derived from this software without specific prior 
 *  written permission. 
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 *  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 */


std::string
GeoCoord::geohash( double lat, double lon, int prec ) 
{
    if (prec < 1 || prec > 12)
        prec = 6;
    
    std::string hash;
    
    if (lat >= -90.0 && lat <= 90.0  &&  lon >= -180.0 && lon <= 180.0) 
    {
        hash.resize(prec);
        Interval lat_val = {90.0, -90.0}; 
        Interval lon_val = {180.0, -180.0};
        unsigned int hashChar = 0;
        bool is_even = true;
        int N = prec * 5;
        
        for (int i = 1; i <= N; ++i) 
        {
            Interval *val;
            double coord;
            
            if (is_even) 
            {
                val = &lon_val;
                coord = lon;                
            } 
            else 
            {
                val = &lat_val;
                coord = lat;   
            }

            hashChar <<= 1;
            
            double mid = (val->low + val->high) * 0.5;
            
            if (coord > mid)
            {
                val->low = mid;
                hashChar |= 0x01;
            } 
            else 
            {
                val->high = mid;
            }
            
            if (!(i % 5))
            {
                hash[(i - 1) / 5] = m_char_map[hashChar];
                hashChar = 0;
            }
   
            is_even = !is_even;
        }
    }
    
    return hash;
}

bool
GeoCoord::valid( const std::string &hash )
// check hash is a valid geohash code
{
    if (hash.empty())
        return false;
    
    if (hash.size() < 2 || hash.size() > 12)
        return false;
    
    for (const char c : hash)
    {
        if (c < '0' || c > 'z')
            return false;
            
        if (m_char_index_table[c - '0'] == 99)
            return false;
    }
    
    return true;
}

GeoCoord 
GeoCoord::geohash( const std::string &hash ) 
// assumes hash is a valid geohash code
{
    assert(hash.size() >= 2 && hash.size() <= 12);

    Interval lat_val = {90.0, -90.0}; 
    Interval lon_val = {180.0, -180.0};

    bool is_even = true;

    for (const char c : hash) 
    {
        // find the char_index for the hash character c
        assert(c >= '0' && c <= 'z');
        unsigned int char_index = m_char_index_table[c - '0'];
        assert(char_index >= 0 && char_index <= 31);

        // interpret the last 5 bits of the integer
        for (int j = 0; j < 5; ++j) 
        {
            Interval *val = (is_even) ? &lon_val : &lat_val;
            
            double delta = (val->high - val->low) * 0.5;
            
            if ((char_index << j) & 0x0010)
                val->low += delta;
            else val->high -= delta;
            
            is_even = !is_even;
        }
    }
    
    double mylat = lat_val.high - ((lat_val.high - lat_val.low) * 0.5); 
    double mylon = lon_val.high - ((lon_val.high - lon_val.low) * 0.5); 
    
   // coord.north = lat_val.high;
   // coord.east  = lon_val.high;
   // coord.south = lat_val.low;
   // coord.west  = lon_val.low;

    return GeoCoord(mylat, mylon);
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
/* Vincenty Inverse Solution of Geodesics on the Ellipsoid (c) Chris Veness 2002-2011             */
/*                                                                                                */
/* from: Vincenty inverse formula - T Vincenty, "Direct and Inverse Solutions of Geodesics on the */
/*       Ellipsoid with application of nested equations", Survey Review, vol XXII no 176, 1975    */
/*       http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

// http://www.movable-type.co.uk/scripts/latlong-vincenty.html

// https://en.wikipedia.org/wiki/Great-circle_distance
// https://en.wikipedia.org/wiki/Vincenty%27s_formulae
// https://en.wikipedia.org/wiki/World_Geodetic_System

// Table taken from GMT project. Radii in metres 
//   Name,          Date,  Eq. rad,    Pole rad,         Flattening
// { "WGS_84",      1984,  6378137.0,  6356752.31424518, 1.0/298.257223563 },
// { "OSU91A",      1991,  6378136.3,  6356751.61633668, 1.0/298.25722 },
// { "OSU86F",      1986,  6378136.2,  6356751.51667196, 1.0/298.25722 },
// { "Engelis",     1985,  6378136.05, 6356751.32272154, 1.0/298.2566 },
// { "SGS_85",      1985,  6378136.0,  6356751.30156878, 1.0/298.257 },
// { "TOPEX",       1990,  6378136.3,  6356751.60056294, 1.0/298.257 },
// { "MERIT_83",    1983,  6378137.0,  6356752.29821597, 1.0/298.257 },
// { "GRS_80",      1980,  6378137.0,  6356752.31414036, 1.0/298.257222101 },
// { "Hughes_1980", 1980,  6378273.0,  6356889.44820259, 1.0/298.2794 },

double
GeoCoord::dist( double lat1, double lon1, double lat2, double lon2 ) 
/**
* Calculates geodetic distance in metres between two points specified by latitude/longitude using 
* Vincenty inverse formula for ellipsoids
*/
{
    // convert degrees to and from radians
    constexpr double D2R = (M_PI / 180.0); 
    //constexpr double R2D = (180.0 / M_PI); 
    
    // { "WGS_84",  1984,  6378137.0, 6356752.31424518,  1.0/298.257223563 },
    const double a = 6378137.0;           // ellipsoid equatorial radius;    
    const double b = 6356752.31424518;    // ellipsoid polar radius;   
    constexpr double f = 1.0 / 298.257223563; // ellipsoid flattening;  
    
    double L = (lon2 - lon1) * D2R;
    double U1 = std::atan((1.0 - f) * std::tan(lat1 * D2R));
    double U2 = std::atan((1.0 - f) * std::tan(lat2 * D2R));
    double sinU1 = std::sin(U1), cosU1 = std::cos(U1);
    double sinU2 = std::sin(U2), cosU2 = std::cos(U2);
    
    double lambda = L; 
    double lambdaP;
    double cosSqAlpha;
    double sigma;
    double sinSigma;
    double cosSigma;
    double cos2SigmaM;
    double sinLambda;
    double cosLambda;
    
    int iterLimit = 5000;
    
    do 
    {
        sinLambda = std::sin(lambda);
        cosLambda = std::cos(lambda);
        
        sinSigma = std::sqrt( (cosU2 * sinLambda) * (cosU2 * sinLambda) + 
                             ((cosU1 * sinU2) - (sinU1 * cosU2 * cosLambda)) * ((cosU1 * sinU2) - (sinU1 * cosU2 * cosLambda)) );
        
        if (sinSigma == 0)
            return 0;  // co-incident points
        
        cosSigma = (sinU1 * sinU2) + (cosU1 * cosU2 * cosLambda);
        sigma = std::atan2(sinSigma, cosSigma);
        
        double sinAlpha = (cosU1 * cosU2 * sinLambda) / sinSigma;
        cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
        cos2SigmaM = cosSigma - ((2.0 * sinU1 * sinU2) / cosSqAlpha); 
        
        if (std::isnan(cos2SigmaM))  
            cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0 (§6)
        
        double C = f / (16.0 * cosSqAlpha * (4.0 + f * (4.0 - (3.0 * cosSqAlpha))));
        lambdaP = lambda;
        lambda = L + (1.0 - C) * f * sinAlpha *
        (sigma + (C * sinSigma * (cos2SigmaM + C * cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM))));    
    } 
    while ((std::fabs(lambda - lambdaP) > 1E-12) && (--iterLimit > 0));
    
    if (iterLimit == 0)
        return -1.0;  // formula failed to converge
    
    double uSq = cosSqAlpha * (a * a - b * b) / (b * b);
    double A = 1.0 + uSq / 16384.0 * (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0 * uSq)));
    double B = uSq / (1024.0 * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0 * uSq))));
    double deltaSigma = B * sinSigma * (cos2SigmaM + B / 4.0 * (cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM) -
                                                                B / 6.0 * cos2SigmaM * (-3.0 + 4.0 * sinSigma * sinSigma) * (-3.0 + 4.0 * cos2SigmaM * cos2SigmaM)));
    double s = b * A * (sigma - deltaSigma);
    
    
    // note: to return initial/final bearings in addition to distance, use something like:
    //fwdAz = std::atan2(cosU2 * sinLambda,  cosU1 * sinU2 - sinU1 * cosU2 * cosLambda) * R2D;
    //revAz = std::atan2(cosU1 * sinLambda, -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda) * R2D;
    
    return s;
    
}

//
//


