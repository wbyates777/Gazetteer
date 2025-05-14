/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   City.cpp - code   $
 $$$$$$$$$$$$$$$%$$$$$$$
 
 by W.B. Yates    
 History: Identifies a city (not an airport) using a large part of the IATA airport code list. 
 This class can represent 1935 distinct cities including the capital cities of the world and the US state capitals.
 This is enough to describe every city associated with a Market Identification Code (MIC).  
 
 While duplicating the binary chop function (and necessary tables) is inelegant, 
 the code is efficient and duplication (with minor changes) has the merit of being simple to implement. 
 It is also straight forward to add and remove code sets.
 A more general solution must take into account the differing lengths of the keys and values. 

 
 */

#ifndef __CITY_H__
#include "City.h"
#endif

#include <cassert>

    
std::ostream&
operator<<( std::ostream &ostr, const City &c )
{
    ostr << c.to3Code();
    return ostr;
}

std::istream&
operator>>( std::istream &istr, City &c )
{
    std::string str;
    istr >> str;
    c.setCity( str );
    return istr;
}

//
//
//

bool
City::set3City( const std::string &str )
// https://en.wikipedia.org/wiki/Binary_search_algorithm
{    
    assert(str.size() == 3);

    const int index = str[0] - 'A'; // 'A' = 65;
    //assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_city = City::XXX;
        return false;
    }
    
    int low   = m_search3[index]; 
    int high  = m_search3[index + 1]; 
    int mid   = ((high + low) >> 1);
    int i;
    
    while (low < high) 
    {
        const char * const cty = m_codes3[mid];
        
        for (i = 1; i < 3; ++i)
        {
            const char &a = str[i];
            const char &b = cty[i];
            
            if (a < b)
            {
                high = mid; 
                break;
            }
            
            if (a > b)
            {
                low = mid + 1;
                break;
            }
        }
        
        if (i == 3)
        {
            m_city = m_toISO3[mid]; 
            return true;
        }
        
        mid = ((high + low) >> 1);
    }
    
    m_city = City::XXX; // NOCITY
    return false;
}


bool
City::set5City( const std::string &str )
// https://en.wikipedia.org/wiki/Binary_search_algorithm
{    
    assert(str.size() == 5);
    
    const int index = str[0] - 'A'; // 'A' = 65;
    //assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_city = City::XXX;
        return false;
    }
    
    int low   = m_search5[index]; 
    int high  = m_search5[index + 1]; 
    int mid   = ((high + low) >> 1);
    int i;
    
    while (low < high) 
    {
        const char * const cty = m_codes5[mid];
        
        for (i = 1; i < 5; ++i)
        {
            const char &a = str[i];
            const char &b = cty[i];
            
            if (a < b)
            {
                high = mid; 
                break;
            }
            
            if (a > b)
            {
                low = mid + 1;
                break;
            }
        }
        
        if (i == 5)
        {
            m_city = m_toISO5[mid]; 
            return true;
        }
        
        mid = ((high + low) >> 1);
    }
    
    m_city = City::XXX; // NOCITY
    return false;
}

bool
City::setCity( const std::string &str )
{    
    //assert(str.size() == 3 || str.size() == 5);

    if (str.size() == 3)
        return set3City(str);

    if (str.size() == 5)
        return set5City(str);

    m_city = City::XXX; // NOCITY
    return false;
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
City::dist( double lat1, double lon1, double lat2, double lon2 ) 
/**
* Calculates geodetic distance in metres between two points specified by latitude/longitude using 
* Vincenty inverse formula for ellipsoids
*/
{
   const double D2R = (M_PI / 180.0); 
   // const double R2D = (180.0 / M_PI); 
   
   // { "WGS_84",  1984,  6378137.0, 6356752.31424518,  1.0/298.257223563 },
   const double a = 6378137.0;           // ellipsoid equatorial radius;    
   const double b = 6356752.31424518;    // ellipsoid polar radius;   
   const double f = 1.0 / 298.257223563; // ellipsoid flattening;  
   
   double L = (lon2 - lon1) * D2R;
   double U1 = std::atan((1.0 - f) * std::tan(lat1 * D2R));
   double U2 = std::atan((1.0 - f) * std::tan(lat2 * D2R));
   double sinU1 = std::sin(U1), cosU1 = std::cos(U1);
   double sinU2 = std::sin(U2), cosU2 = std::cos(U2);
   
   double lambda = L, lambdaP;
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
   
   return s;
   
   // note: to return initial/final bearings in addition to distance, use something like:
   // double fwdAz = std::atan2(cosU2 * sinLambda,  cosU1 * sinU2 - sinU1 * cosU2 * cosLambda);
   // double revAz = std::atan2(cosU1 * sinLambda, -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda);
   // return std::pair<double,double>( fwdAz * R2D, revAz * R2D );
}


// A c++ rewrite of  libgeohash see https://github.com/simplegeo/libgeohash

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

/* Normal 32 characer map used for geohashing */
const char City::m_char_map[33] =  "0123456789bcdefghjkmnpqrstuvwxyz"; 


// the index for each char in m_char_map - 100 indicates 'no index'
const char GeoPoint::m_char_index[75] = 
{ 
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
    10, 11, 12, 13, 14, 15, 16, 100, 17, 18, 
    100, 19, 20, 100, 21, 22, 23, 24, 25, 26, 
    27, 28, 29, 30, 31 
};  


std::string
City::geohash( double lat, double lon, int precision ) 
{
    if (precision < 1 || precision > 16)
        precision = 6;
    
    std::string hash;
    
    if (lat >= -90.0 && lat <= 90.0  &&  lon >= -180.0 && lon <= 180.0) 
    {
        hash.resize(precision);
        
        Interval lat_val = {90.0, -90.0}; 
        Interval lon_val = {180.0, -180.0};

        unsigned int hashChar = 0;
        bool is_even = true;
        precision *= 5;
   
        for (int i = 1; i <= precision; ++i) 
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
            
            double mid = (val->low + val->high) * 0.5;
            
            hashChar <<= 1;
            
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

GeoPoint 
City::geohash(const std::string &hash) 
{
    if (hash.empty())
        return GeoPoint();
    
    Interval lat_val = {90.0, -90.0}; 
    Interval lon_val = {180.0, -180.0};

    bool is_even = true;

    for (char c : hash) 
    {
        // find the char_index for the hash character c
        assert(c >= '0' && c <= 'z');
        unsigned int char_index = m_char_index[c - '0'];
        assert(char_index >= 0 && char_index <= 31);
     
        // interpret the last 5 bits of the integer
        for (int j = 0; j < 5; ++j) 
        {
            Interval *val = (is_even) ? &lon_val : &lat_val;
            
            double delta = (val->high - val->low) * 0.5;
            
            if ( (char_index << j) & 0x0010 )
                val->low += delta;
            else val->high -= delta;
            
            is_even = !is_even;
        }
    }
    
    double lat = lat_val.high - ((lat_val.high - lat_val.low) * 0.5); 
    double lon = lon_val.high - ((lon_val.high - lon_val.low) * 0.5); 
    
   // coord.north = lat_interval.high;
   // coord.east  = lon_interval.high;
   // coord.south = lat_interval.low;
   // coord.west  = lon_interval.low;

    return GeoPoint(lat, lon);
}


//
//
//

// this speeds up setCity a bit
// we could accelerate access to key cities i.e. NYC, LON, LCG - see Currency

const short City::m_search3[28] = {
    1, 127, 264, 383, 445, 495, 550, 628, 699, 752, 844, 920, 1024, 1186, 1232, 1289, 1397, 1407, 1470, 1624, 1711, 1767, 1811, 1838, 1848, 1919, 1936, -1 
};

    
const short City::m_search5[28] = {
    1, 182, 276, 452, 513, 575, 650, 747, 767, 880, 925, 945, 961, 1060, 1140, 1142, 1232, 1233, 1266, 1320, 1378, 1852, 1876, 1879, 1881, 1884, 1936, -1
};

const short City::m_fromISO[MAXCITY] = { 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    2, 3, 5, 6, 7, 1, 8, 9, 10, 11, 
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 
    52, 53, 54, 55, 56, 57, 4, 58, 59, 60, 
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 
    81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 
    91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 
    101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 
    121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 
    131, 132, 134, 135, 136, 137, 138, 139, 140, 141, 
    142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 
    162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 
    172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 
    182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 
    202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 
    212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 
    222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 
    232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 
    242, 243, 244, 245, 246, 247, 248, 249, 250, 133, 
    251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 
    261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 
    271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 
    281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 
    291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 
    301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 
    311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 
    321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 
    331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 
    341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 
    351, 352, 353, 355, 356, 357, 358, 359, 360, 361, 
    362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 
    372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 
    382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 
    392, 393, 394, 395, 396, 397, 398, 399, 401, 402, 
    403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 
    413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 
    423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 
    433, 400, 434, 435, 436, 437, 438, 439, 440, 441, 
    442, 443, 444, 494, 445, 446, 447, 451, 448, 449, 
    450, 452, 453, 454, 455, 456, 457, 458, 459, 460, 
    461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 
    471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 
    481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 
    491, 492, 493, 495, 496, 497, 498, 499, 500, 501, 
    503, 502, 504, 505, 506, 507, 508, 509, 510, 511, 
    512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 
    522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 
    532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 
    542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 
    552, 553, 554, 555, 556, 557, 558, 560, 561, 562, 
    563, 564, 565, 566, 567, 568, 569, 570, 571, 559, 
    572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 
    582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 
    592, 593, 594, 595, 596, 597, 601, 598, 600, 602, 
    603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 
    613, 614, 615, 616, 617, 618, 599, 619, 620, 621, 
    622, 623, 624, 625, 626, 627, 628, 630, 631, 632, 
    633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 
    643, 644, 645, 629, 646, 647, 648, 649, 650, 651, 
    652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 
    662, 0, 663, 664, 665, 666, 667, 668, 669, 670, 
    671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 
    681, 682, 683, 685, 686, 687, 688, 689, 690, 691, 
    692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 
    702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 
    712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 
    722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 
    732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 
    742, 743, 744, 745, 746, 747, 748, 749, 750, 751, 
    752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 
    762, 763, 354, 764, 765, 766, 767, 768, 769, 770, 
    771, 772, 773, 774, 775, 776, 777, 778, 779, 780, 
    781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 
    791, 792, 793, 794, 795, 796, 797, 798, 799, 800, 
    801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 
    811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 
    821, 822, 823, 824, 0, 826, 827, 828, 825, 829, 
    830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 
    840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 
    850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 
    860, 861, 862, 863, 864, 865, 866, 867, 868, 869, 
    870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 
    880, 881, 882, 883, 884, 885, 886, 887, 888, 889, 
    890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 
    900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 
    910, 911, 912, 913, 914, 915, 916, 917, 918, 919, 
    920, 921, 922, 923, 924, 925, 926, 927, 928, 929, 
    930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 
    940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 
    950, 951, 953, 954, 955, 956, 957, 958, 959, 960, 
    961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 
    971, 972, 973, 974, 975, 976, 977, 978, 979, 980, 
    981, 982, 983, 984, 985, 986, 987, 988, 989, 990, 
    991, 992, 993, 994, 995, 996, 997, 998, 999, 1000, 
    1001, 1022, 1002, 1003, 1004, 1005, 1006, 1007, 952, 1008, 
    1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 
    1019, 1020, 1021, 1023, 1024, 1025, 1026, 1027, 1028, 1029, 
    1030, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039, 
    1098, 1040, 1041, 1042, 1043, 1044, 1046, 1047, 1048, 1049, 
    1050, 1051, 1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 
    1060, 1061, 1062, 1063, 1064, 1065, 1066, 1067, 1068, 1069, 
    1070, 1071, 1072, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 
    1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087, 1088, 1089, 
    1090, 1091, 1092, 1093, 1094, 1095, 1096, 1097, 1099, 1100, 
    1101, 1102, 1103, 1104, 1105, 1106, 1107, 1108, 1109, 1110, 
    1111, 1112, 1113, 1114, 1116, 1117, 1118, 1119, 1120, 1115, 
    1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 
    1131, 1132, 1133, 1134, 1135, 1136, 1137, 1138, 1139, 1140, 
    1141, 1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1150, 
    1151, 1152, 1153, 1154, 1155, 1156, 1157, 1158, 1159, 1160, 
    1161, 1162, 1163, 1164, 1165, 1166, 1809, 1167, 1168, 1169, 
    1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 
    1180, 1181, 1182, 1183, 1184, 1185, 1186, 1187, 1188, 1189, 
    1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 
    1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1209, 1210, 
    1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 
    1221, 1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 
    1231, 1232, 1233, 1234, 1235, 1252, 1236, 1237, 1238, 1239, 
    1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1248, 1249, 
    1250, 1251, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260, 
    1261, 1262, 1263, 1264, 1265, 1266, 1267, 1045, 1268, 1269, 
    1270, 1271, 1272, 1273, 1274, 1275, 1276, 1277, 1278, 1279, 
    1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 
    1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 
    1300, 1301, 1302, 1303, 1304, 1305, 1306, 1307, 1308, 1309, 
    1310, 1311, 1312, 1314, 1315, 1316, 1317, 1318, 1319, 1320, 
    1321, 1322, 1323, 1324, 1325, 1326, 1327, 1328, 1329, 1330, 
    1331, 1332, 1333, 1334, 1335, 1336, 1337, 1338, 1339, 1340, 
    1341, 1342, 1343, 1344, 1345, 1346, 1347, 1348, 1349, 1350, 
    1351, 1352, 1353, 1354, 1355, 1356, 1357, 1358, 1359, 1360, 
    1361, 1362, 1363, 1364, 1365, 1366, 1367, 1368, 1369, 1370, 
    1371, 1372, 1373, 1374, 1375, 1376, 1377, 1378, 1379, 1380, 
    1381, 1382, 1383, 1384, 1385, 1386, 1387, 1388, 1389, 1390, 
    1391, 1392, 1393, 1394, 1395, 1396, 1397, 1398, 1399, 1400, 
    1401, 1402, 1403, 1404, 1405, 1406, 1410, 1407, 1408, 1409, 
    1411, 1412, 1413, 1414, 1415, 1416, 1417, 1418, 1419, 1420, 
    1421, 1422, 1423, 1424, 1425, 1426, 1427, 1428, 1429, 1430, 
    1431, 1432, 1433, 1434, 1435, 1436, 1437, 1438, 1439, 1440, 
    1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 
    1451, 1452, 1453, 1454, 1455, 1456, 1457, 1458, 1459, 1460, 
    1461, 1462, 1463, 1464, 1465, 1466, 1467, 1468, 1469, 1470, 
    1471, 1473, 1474, 1475, 1476, 1477, 1478, 0, 1479, 1480, 
    1481, 1482, 1483, 1484, 1485, 1486, 1487, 1488, 1489, 1490, 
    1491, 1492, 1493, 1494, 1495, 1496, 1497, 1498, 1499, 1500, 
    1501, 1502, 1503, 1504, 1505, 1506, 1507, 1508, 1509, 1510, 
    1511, 1512, 1513, 1514, 1515, 1516, 1517, 1518, 1519, 684, 
    1520, 1521, 1522, 1523, 1524, 1526, 1549, 1527, 1528, 1529, 
    1530, 1531, 1532, 1533, 1534, 1535, 1536, 1537, 1538, 1539, 
    1525, 1540, 1541, 1542, 1543, 1544, 1545, 1546, 1547, 1548, 
    1550, 1551, 1552, 1553, 1554, 1555, 1556, 1472, 1557, 1558, 
    1559, 1560, 1561, 1562, 1563, 1564, 1565, 1566, 1567, 1568, 
    1569, 1570, 1571, 1572, 1573, 1574, 1575, 1576, 1577, 1578, 
    1579, 1580, 1581, 1582, 1583, 1584, 1585, 1841, 1586, 1587, 
    1588, 1589, 1590, 1591, 1592, 1593, 1594, 1595, 1596, 1597, 
    1598, 1599, 1600, 1601, 1602, 1603, 1604, 1605, 1606, 1609, 
    1607, 1608, 1610, 1611, 1612, 1613, 1614, 1615, 1616, 1617, 
    1618, 1619, 1620, 1621, 1622, 1623, 1624, 1625, 1626, 1627, 
    1628, 1629, 1630, 1631, 1632, 1633, 1635, 1636, 1637, 1638, 
    1639, 1640, 1641, 1642, 1313, 0, 1634, 1643, 1644, 1645, 
    1646, 1647, 1648, 1649, 1650, 1651, 1652, 1653, 1654, 1208, 
    1655, 1656, 1657, 1658, 1659, 1660, 1661, 1662, 1663, 1664, 
    1665, 1666, 1667, 1668, 1669, 1670, 1671, 1672, 1673, 1674, 
    1675, 1676, 1677, 1678, 1679, 1680, 1681, 1682, 1683, 1684, 
    1685, 1686, 1687, 1688, 1689, 1690, 1691, 1692, 1693, 1694, 
    1695, 1696, 1697, 1698, 1699, 1700, 1701, 1702, 1703, 1704, 
    1705, 1706, 1707, 1708, 1709, 1710, 1711, 1712, 1713, 1714, 
    1715, 1716, 1717, 1718, 1719, 1720, 1721, 1722, 1723, 1724, 
    1725, 1726, 1727, 1728, 1729, 1730, 1731, 1732, 1733, 1734, 
    1735, 1736, 1737, 1738, 1739, 1740, 1741, 1742, 1743, 1744, 
    1745, 1746, 1747, 1748, 1749, 1750, 1751, 1752, 1753, 1754, 
    1755, 1756, 1757, 1758, 1759, 1760, 1761, 1762, 1763, 1764, 
    1765, 1766, 1772, 1767, 1768, 1769, 1770, 1771, 1773, 1774, 
    1775, 1776, 1777, 1778, 1779, 1780, 1781, 1782, 1783, 1784, 
    1785, 1786, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1787, 
    1795, 1796, 1797, 1798, 1799, 1800, 1801, 1802, 1803, 1804, 
    1805, 1806, 1807, 1808, 1810, 1811, 1812, 1813, 1814, 1815, 
    1816, 1817, 1818, 1819, 1820, 1821, 1822, 1823, 1824, 1825, 
    1826, 1827, 1828, 1829, 1830, 1831, 1832, 1833, 1834, 1835, 
    1836, 1837, 1838, 1839, 1840, 1842, 1843, 1844, 1845, 1846, 
    1847, 1848, 1849, 1850, 1851, 1852, 1853, 1855, 1856, 1857, 
    1858, 1859, 1860, 1861, 1862, 0, 1863, 1864, 1865, 1866, 
    1867, 1868, 1869, 1870, 1871, 1872, 1873, 1874, 1875, 1876, 
    1877, 1878, 1879, 1880, 1881, 1882, 1883, 1884, 1885, 1886, 
    1887, 1888, 1889, 1890, 1891, 1892, 1893, 1894, 1895, 1896, 
    1897, 1898, 1899, 1900, 1901, 1902, 1854, 1903, 1904, 1905, 
    1906, 1907, 1908, 1909, 1910, 1911, 1912, 1913, 1914, 1915, 
    1916, 1917, 1918, 1919, 1920, 1921, 1922, 1923, 1924, 1925, 
    1926, 1927, 1928, 1929, 1930, 1931, 1932, 1933, 1934, 1935
};

const short City::m_toISO3[NUMCITY] = { NOCITY, 
    AAB, AAC, AAE, AAL, AAN, AAR, AAT, ABD, ABE, ABI, 
    ABJ, ABM, ABQ, ABR, ABS, ABV, ABX, ABY, ABZ, ACA, 
    ACC, ACE, ACH, ACI, ACK, ACT, ACV, ACY, ADA, ADD, 
    ADE, ADF, ADL, ADQ, ADY, ADZ, AEH, AER, AES, AGA, 
    AGB, AGP, AGS, AGU, AGZ, AHB, AHN, AHO, AHU, AJA, 
    AJF, AJN, AJU, AJY, AKL, AKN, AKT, ALA, ALB, ALC, 
    ALF, ALG, ALH, ALJ, ALO, ALP, ALV, ALW, ALY, AMA, 
    AMD, AMM, AMS, ANB, ANC, ANK, ANP, ANR, ANU, AOI, 
    AOJ, AOK, AOO, APF, APW, AQJ, ARB, ARH, ARK, ASB, 
    ASE, ASI, ASJ, ASK, ASM, ASP, ASR, ASU, ASW, ATH, 
    ATL, ATO, ATQ, ATW, ATY, ATZ, AUA, AUE, AUG, AUH, 
    AUR, AUS, AVL, AVP, AXA, AXS, AXT, AYL, AYQ, AYR, 
    AYT, AYU, AYW, AZB, AZO, AZS, BAH, BAK, BAQ, BBI, 
    BBR, BBT, BBU, BBY, BCN, BCO, BCT, BCV, BDA, BDB, 
    BDL, BDO, BDQ, BDR, BDS, BDU, BEB, BEG, BEH, BEL, 
    BEN, BER, BES, BET, BEW, BEY, BFD, BFL, BFN, BFO, 
    BFS, BGA, BGF, BGI, BGM, BGO, BGR, BGU, BGW, BGY, 
    BHC, BHE, BHM, BHO, BHQ, BHV, BHX, BIA, BIL, BIO, 
    BIQ, BIS, BIV, BJI, BJL, BJM, BJS, BJV, BJX, BJZ, 
    BKI, BKK, BKO, BKW, BKX, BLA, BLF, BLI, BLK, BLL, 
    BLQ, BLR, BLT, BLZ, BME, BMG, BMI, BMP, BNA, BNE, 
    BNJ, BNK, BNN, BNP, BNX, BOB, BOD, BOG, BOH, BOI, 
    BOJ, BOM, BON, BOO, BOS, BOY, BPT, BQK, BQN, BRC, 
    BRD, BRE, BRI, BRL, BRN, BRR, BRS, BRU, BSB, BSL, 
    BSR, BTM, BTR, BTS, BTU, BTV, BUD, BUE, BUF, BUG, 
    BUQ, BUR, BVB, BWI, BWN, BWT, BXS, BYK, BYU, BZE, 
    BZL, BZN, BZV, CAB, CAE, CAG, CAI, CAK, CAL, CAN, 
    CAS, CAY, CBB, CBG, CBR, CBU, CCJ, CCK, CCR, CCS, 
    CCU, CCZ, CDC, CDV, CEB, CEC, CED, CEN, CEQ, CES, 
    CFE, CFG, CFN, CFS, CFU, CGA, CGB, CGN, CGO, CGP, 
    CGQ, CGR, CGU, CHA, CHC, CHG, CHI, CHO, CHQ, CHS, 
    CIC, CID, CIP, CIW, CJB, CJC, CJL, CJS, CKB, CKG, 
    CKY, CLD, CLE, CLJ, CLL, CLM, CLO, CLQ, CLT, CLY, 
    CMB, CME, CMF, CMH, CMI, CMQ, CMX, CND, CNF, CNQ, 
    CNS, CNX, COD, COK, CON, COO, COR, COS, CPD, CPH, 
    CPR, CPT, CRF, CRG, CRP, CRW, CSG, CSI, CSN, CST, 
    CTA, CTG, CTN, CTU, CUL, CUN, CUR, CUU, CVG, CVM, 
    CVQ, CVT, CWA, CWB, CWL, CXI, CXT, CYS, CYU, CZA, 
    CZL, CZM, DAB, DAC, DAL, DAM, DAN, DAR, DAY, DBO, 
    DBQ, DBV, DBY, DCA, DCF, DDI, DEC, DEL, DEN, DET, 
    DHA, DHN, DIL, DJB, DJE, DJJ, DJO, DKI, DKR, DLA, 
    DLC, DLG, DLH, DLM, DLY, DMM, DND, DNK, DNR, DNZ, 
    DOD, DOH, DOM, DOV, DPO, DPS, DRB, DRO, DRS, DRW, 
    DSK, DSM, DTM, DUB, DUD, DUJ, DUR, DUS, DUT, DVL, 
    DXB, DYA, DYU, DZA, EAS, EAT, EAU, EBA, EBB, EBJ, 
    EBN, EBU, EDI, EGC, EGE, EGS, EIN, EIS, EKI, EKO, 
    ELH, ELL, ELM, ELP, ELS, ELY, EMA, EMD, EMY, ENA, 
    ENC, ENF, EPC, EPR, ERC, ERF, ERI, ERZ, ES2, ESC, 
    ESF, ESH, ESR, ETH, EUG, EUN, EVE, EVN, EVV, EWN, 
    EWR, EXT, EYW, EZT, FAE, FAI, FAO, FAR, FAT, FAY, 
    FBM, FCA, FCI, FDF, FDH, FEZ, FFT, FHU, FIE, FIH, 
    FJR, FKB, FKI, FKL, FKS, FLG, FLL, FLN, FLO, FLR, 
    FMN, FMO, FMY, FNA, FNC, FNI, FNJ, FNT, FOD, FOE, 
    FOR, FOU, FPO, FRA, FRI, FRJ, FRO, FRU, FRW, FSC, 
    FSD, FSM, FSP, FUE, FUK, FUN, FUT, FWA, FYV, GAD, 
    GAJ, GAU, GBE, GC0, GCC, GCI, GCM, GCN, GDH, GDL, 
    GDN, GDT, GDV, GEG, GEO, GET, GEX, GFF, GFK, GGG, 
    GGW, GHB, GIB, GIL, GJL, GJT, GKL, GLA, GLH, GLT, 
    GNB, GND, GNE, GNV, GOA, GOH, GOI, GOJ, GON, GOO, 
    GOT, GOU, GOV, GOZ, GPA, GPT, GPZ, GRB, GRH, GRJ, 
    GRN, GRO, GRQ, GRR, GRX, GRZ, GS0, GSI, GSO, GSP, 
    GTE, GTF, GTI, GTN, GUA, GUC, GUM, GVA, GWD, GWE, 
    GWT, GWY, GYE, GYN, GYP, GZA, GZT, HAC, HAG, HAH, 
    HAJ, HAM, HAN, HAP, HAR, HAU, HAV, HBA, HDD, HDN, 
    HDY, HEL, HER, HFA, HFT, HGH, HGO, HHH, HIB, HII, 
    HIJ, HIR, HIS, HKD, HKG, HKT, HKY, HLA, HLH, HLN, 
    HLT, HLZ, HMO, HNA, HNH, HNK, HNL, HNS, HOG, HOM, 
    HON, HOQ, HOR, HOU, HPN, HRB, HRE, HRG, HRK, HRL, 
    HS0, HSV, HTA, HTB, HTI, HTS, HUF, HUH, HUI, HUX, 
    HUY, HVB, HVN, HVR, HWN, HYA, HYD, HYG, IAG, IBZ, 
    ICN, ICT, IDA, IDR, IEV, IFL, IGH, IGR, IJX, IKT, 
    ILE, ILG, ILI, ILM, ILP, ILY, ILZ, IND, INI, INL, 
    INN, INU, INV, IOM, IOU, IPC, IPL, IPT, IQT, IRG, 
    IRO, ISA, ISB, ISG, ISL, ISO, ISP, IST, ITH, ITO, 
    IUE, IVC, IVL, IXB, IXC, IXG, IXJ, IXR, IXW, IYK, 
    IZM, JAA, JAC, JAD, JAF, JAG, JAI, JAK, JAL, JAM, 
    JAN, JAQ, JAU, JBR, JCB, JCK, JCM, JDF, JDH, JDZ, 
    JED, JEE, JEF, JER, JGA, JGB, JGN, JGS, JHB, JHE, 
    JHG, JHM, JHQ, JHW, JIB, JIJ, JIL, JIM, JIN, JIP, 
    JIR, JIU, JIW, JJI, JJN, JKG, JKH, JKR, JKT, JKV, 
    JLN, JLR, JLS, JMB, JMK, JMO, JMS, JMU, JNA, JNB, 
    JNG, JNI, JNU, JNX, JNZ, JOE, JOI, JOL, JON, JOS, 
    JPA, JQE, JRH, JRO, JRS, JSA, JSI, JSM, JST, JTI, 
    JTR, JUB, JUI, JUJ, JUL, JUM, JUN, JUT, JUV, JVL, 
    JWA, JXN, JYV, KAJ, KAN, KAO, KBL, KBY, KCC, KCE, 
    KCH, KCM, KCZ, KDD, KDU, KEL, KEM, KGC, KGD, KGI, 
    KGL, KGS, KHH, KHI, KHJ, KHV, KID, KIJ, KIK, KIM, 
    KIN, KIR, KIV, KIW, KKN, KLA, KLR, KLU, KLW, KLX, 
    KLZ, KMI, KMJ, KMP, KMQ, KNS, KNU, KNX, KOA, KOI, 
    KOJ, KOK, KRB, KRK, KRN, KRP, KRS, KRT, KSD, KSL, 
    KSU, KTA, KTB, KTM, KTN, KTR, KTT, KUA, KUF, KUH, 
    KUL, KUO, KVA, KWI, KWL, KWM, KYA, KYS, KZN, LAD, 
    LAE, LAF, LAI, LAN, LAP, LAR, LAS, LAW, LAX, LBA, 
    LBB, LBE, LBI, LBQ, LBU, LBV, LCA, LCG, LCH, LDC, 
    LDE, LDK, LDY, LEA, LEB, LED, LEI, LEJ, LEK, LEQ, 
    LER, LEU, LEX, LEY, LFT, LFW, LGB, LGG, LGK, LHE, 
    LI0, LIF, LIG, LIH, LIJ, LIL, LIM, LIQ, LIS, LIT, 
    LJU, LKL, LKO, LLA, LLW, LMM, LMP, LMT, LNK, LNO, 
    LNS, LNY, LNZ, LON, LOS, LPA, LPB, LPL, LPP, LRD, 
    LRE, LRF, LRH, LRM, LRT, LSE, LSI, LST, LSY, LSZ, 
    LTO, LUD, LUG, LUJ, LUM, LUN, LUX, LVO, LWB, LWK, 
    LWO, LWS, LWT, LXR, LYH, LYO, LYP, LYR, LYS, LYX, 
    LZC, LZN, LZR, MAA, MAD, MAF, MAH, MAJ, MAN, MAO, 
    MAR, MAU, MAZ, MBA, MBH, MBJ, MBM, MBS, MBX, MCE, 
    MCI, MCL, MCM, MCN, MCO, MCP, MCT, MCW, MCY, MCZ, 
    MDC, MDE, MDL, MDQ, MDY, MDZ, MED, MEE, MEI, MEL, 
    MEM, MES, MEX, MEZ, MFE, MFF, MFG, MFM, MFN, MFO, 
    MFQ, MFR, MFU, MGA, MGB, MGF, MGH, MGM, MGQ, MGW, 
    MHE, MHH, MHQ, MHT, MIA, MID, MIK, MIL, MIM, MIR, 
    MJC, MJD, MJL, MJN, MJQ, MJT, MJV, MKC, MKE, MKG, 
    MKK, MKL, MKR, MKY, MLA, MLB, MLE, MLH, MLI, MLM, 
    MLS, MLU, MLW, MLX, MMA, MME, MMG, MMJ, MMK, MMM, 
    MMY, MNI, MNL, MOB, MOD, MOT, MOV, MOW, MOZ, MPA, 
    MPD, MPL, MPM, MQL, MQM, MQQ, MQT, MRB, MRS, MRU, 
    MRV, MRY, MRZ, MSE, MSL, MSN, MSO, MSP, MSQ, MSR, 
    MST, MSU, MSY, MTH, MTJ, MTL, MTM, MTO, MTS, MTT, 
    MTY, MUB, MUC, MUE, MUH, MUX, MVB, MVD, MVR, MVY, 
    MVZ, MWD, MWH, MXL, MYA, MYD, MYJ, MYQ, MYR, MYY, 
    MZF, MZG, MZM, MZT, MZY, NAA, NAG, NAJ, NAN, NAP, 
    NAS, NAT, NBO, NCE, NCL, NCS, NCY, NDB, NDJ, NDU, 
    NEV, NGE, NGO, NGS, NIC, NIM, NKC, NKU, NKW, NLA, 
    NLD, NLK, NLP, NLV, NNG, NOC, NOP, NOU, NQY, NRA, 
    NRK, NSA, NSN, NSO, NST, NTE, NTL, NTY, NUE, NWI, 
    NYC, OAG, OAJ, OAK, OAX, ODA, ODB, ODE, ODS, OGG, 
    OHD, OHT, OIT, OKA, OKC, OKD, OKJ, OKU, OLB, OLF, 
    OLM, OLO, OLP, OMA, OMD, OME, OMH, OMO, OND, ONT, 
    OOL, OOM, OPO, ORB, ORF, ORH, ORK, ORN, ORS, OSA, 
    OSH, OSI, OSL, OSM, OTH, OTZ, OUA, OUD, OUH, OUK, 
    OUL, OUZ, OVB, OVD, OWB, OXB, OXR, OZZ, PAC, PAD, 
    PAH, PAP, PAR, PAT, PBC, PBH, PBI, PBM, PBO, PBZ, 
    PC0, PCT, PDL, PDT, PDX, PEG, PEI, PEN, PER, PEW, 
    PFN, PFO, PFR, PGA, PGF, PGV, PHC, PHE, PHF, PHL, 
    PHW, PHX, PIA, PIB, PIH, PIR, PIS, PIT, PJG, PKB, 
    PKW, PLB, PLN, PLO, PLZ, PMD, PMI, PMO, PMR, PMV, 
    PMW, PN0, PNH, PNI, PNL, PNQ, PNR, PNS, POA, POG, 
    POM, POP, POR, POS, POU, POZ, PPG, PPP, PPT, PQI, 
    PQQ, PRG, PRN, PRY, PS0, PSA, PSC, PSD, PSE, PSG, 
    PSI, PSP, PSR, PSY, PTG, PTJ, PUB, PUF, PUG, PUJ, 
    PUQ, PUS, PUW, PUY, PVD, PVH, PVK, PVR, PWM, PXM, 
    PXO, PYB, PYX, PZB, PZH, PZO, QCB, QGF, QIC, QLI, 
    QML, QMN, QNB, QND, QPA, QSA, RAI, RAJ, RAK, RAN, 
    RAP, RAR, RAZ, RB0, RBA, RBR, RCB, RDD, RDG, RDM, 
    RDU, RDZ, REC, REG, REK, REU, RFD, RFP, RGI, RGN, 
    RHI, RHO, RIC, RIO, RIX, RIZ, RJK, RKD, RKS, RKT, 
    RLT, RMF, RMI, RNB, RNE, RNN, RNO, RNS, ROA, ROC, 
    ROK, ROM, ROR, ROS, ROT, ROU, ROV, RRG, RSA, RSD, 
    RST, RTB, RTM, RUH, RUN, RVN, RWI, RWP, RYK, SAF, 
    SAH, SAI, SAL, SAN, SAO, SAP, SAT, SAV, SBA, SBH, 
    SBK, SBN, SBP, SBU, SBW, SBY, SBZ, SCC, SCE, SCF, 
    SCK, SCN, SCQ, SCU, SDF, SDH, SDJ, SDL, SDQ, SDR, 
    SDS, SDT, SDY, SEA, SEB, SEL, SEN, SEW, SEZ, SFA, 
    SFG, SFJ, SFN, SFO, SGD, SGF, SGN, SGU, SGY, SHA, 
    SHD, SHE, SHJ, SHR, SHS, SHV, SID, SIN, SIP, SIS, 
    SIT, SIX, SJC, SJD, SJJ, SJO, SJT, SJU, SJY, SKB, 
    SKD, SKG, SKP, SKS, SKV, SKX, SKZ, SLA, SLB, SLC, 
    SLE, SLL, SLP, SLS, SLU, SLZ, SMA, SMF, SMI, SMX, 
    SNA, SNC, SNN, SNS, SOF, SOG, SOI, SON, SOT, SOU, 
    SPC, SPI, SPN, SPS, SPU, SPY, SRA, SRB, SRL, SRQ, 
    SRZ, SSA, SSG, SSH, SSL, STI, STL, STO, STP, STR, 
    STS, STT, STV, STX, SUB, SUF, SUL, SUM, SUN, SUV, 
    SUX, SVD, SVG, SVL, SVQ, SVX, SWF, SWP, SWR, SWS, 
    SXB, SXL, SXM, SXR, SYD, SYR, SYY, SZD, SZF, SZG, 
    SZK, SZX, SZZ, TAB, TAK, TAM, TAO, TAS, TAY, TBS, 
    TBU, TCA, TCB, TCI, TCL, TCU, TED, TEM, TEQ, TER, 
    TEU, TEX, TGD, TGU, TGV, TGZ, THE, THR, TIA, TIF, 
    TIJ, TIP, TIS, TIV, TKA, TKS, TKU, TLH, TLL, TLS, 
    TLV, TMP, TMS, TMW, TMZ, TNA, TNG, TNR, TOD, TOL, 
    TOS, TOU, TOV, TOY, TPA, TPR, TPS, TRD, TRI, TRN, 
    TRO, TRS, TRV, TRW, TRZ, TSB, TSE, TSF, TSN, TSV, 
    TTN, TUC, TUK, TUL, TUN, TUP, TUS, TUU, TVC, TVF, 
    TVL, TWB, TWF, TWU, TXK, TYN, TYO, TYR, TYS, TZX, 
    UAH, UAK, UAP, UBA, UBJ, UBP, UCA, UCT, UDE, UDI, 
    UDJ, UDR, UEE, UET, UFA, UGC, UGO, UHE, UII, UIN, 
    UIO, UIP, UIT, UKB, UKI, UKY, ULB, ULD, ULN, ULU, 
    UMD, UME, UMR, UNI, UNK, UNT, UPG, UPL, UPN, UPP, 
    URB, URC, URG, URM, URZ, USH, USN, UTC, UTH, UTN, 
    UTP, UTT, UUD, UVE, UVF, UVL, VAA, VAN, VAP, VAR, 
    VAS, VAT, VBS, VBY, VCE, VDE, VDZ, VEL, VER, VFA, 
    VGO, VID, VIE, VIJ, VIS, VIT, VIV, VIX, VLC, VLD, 
    VLI, VLL, VLN, VLU, VNO, VNS, VOG, VPS, VRA, VRB, 
    VRK, VRN, VSA, VST, VTE, VVO, VXO, VYD, VYV, VZN, 
    WAW, WDH, WEI, WEL, WGA, WHK, WHM, WIC, WIE, WLB, 
    WLG, WLS, WMB, WNS, WOL, WPK, WRE, WRG, WRL, WRO, 
    WUH, WUN, WUX, WVB, WYA, WYN, WYS, XCH, XIY, XLB, 
    XLX, XMH, XMN, XPK, XRY, XSI, XXX, YAK, YAO, YAT, 
    YBE, YCB, YDF, YEA, YEV, YFA, YFB, YFC, YFO, YGL, 
    YGW, YGX, YHR, YHZ, YIF, YIH, YKA, YKM, YKS, YLR, 
    YLW, YMM, YMQ, YNB, YOK, YOP, YOW, YPN, YPR, YQB, 
    YQD, YQG, YQM, YQR, YQT, YQX, YRB, YSJ, YSM, YSR, 
    YTH, YTO, YUD, YUM, YUX, YVB, YVO, YVP, YVQ, YVR, 
    YWG, YWK, YXE, YXJ, YXN, YXS, YXT, YXU, YXY, YYC, 
    YYD, YYJ, YYQ, YYR, YYT, YYZ, YZF, YZP, ZAD, ZAG, 
    ZAZ, ZBO, ZCL, ZIH, ZKE, ZLO, ZND, ZNE, ZQN, ZRH, 
    ZSA, ZSS, ZTH, ZTM, ZYL
};


////
const short City::m_toISO5[NUMCITY] = { NOCITY, 
    ADALV, AEAAN, AEAUH, AEDXB, AEFJR, AERKT, AESHJ, AFJAA, AFKBL, AFURZ, 
    AGSJO, AIVAL, ALTIA, AMEVN, AOBUG, AOCAB, AOJMB, AOLAD, AOUGO, ARBRC, 
    ARBUE, ARCNQ, ARCOR, ARIGR, ARJNI, ARJSM, ARJUJ, ARMDQ, ARMDZ, ARROS, 
    ARRSA, ARSFN, ARSLA, ARTUC, ARUSH, ASPPG, ATGRZ, ATINN, ATKLU, ATLNZ, 
    ATSZG, ATVIE, AUABM, AUABX, AUADL, AUALH, AUASP, AUAYQ, AUAYR, AUBDB, 
    AUBHQ, AUBLT, AUBME, AUBMP, AUBNE, AUBNK, AUBWT, AUCBR, AUCED, AUCES, 
    AUCFS, AUCMQ, AUCNS, AUCOM, AUCPD, AUCSI, AUCTN, AUCVQ, AUCXT, AUDBO, 
    AUDBY, AUDDI, AUDKI, AUDPO, AUDRB, AUDRW, AUDYA, AUEMD, AUEPR, AUGET, 
    AUGEX, AUGFF, AUGKL, AUGLT, AUGOC, AUGOO, AUGOV, AUGTE, AUGYP, AUHBA, 
    AUHIS, AUHLT, AUHNK, AUHTI, AUHVB, AUIFL, AUIGH, AUIRG, AUISA, AUJAD, 
    AUJCK, AUJHQ, AUJUN, AUKBY, AUKCE, AUKGC, AUKGI, AUKNS, AUKNX, AUKRB, 
    AUKTA, AUKTR, AUKWM, AULDC, AULEA, AULER, AULNO, AULRE, AULST, AULSY, 
    AULVO, AULZR, AUMBH, AUMCY, AUMEL, AUMGB, AUMIM, AUMKR, AUMKY, AUMMG, 
    AUMMM, AUMOV, AUMQL, AUMRZ, AUMTL, AUMYA, AUNAA, AUNRA, AUNSA, AUNSO, 
    AUNTL, AUNWM, AUOAG, AUOLP, AUORS, AUPBO, AUPER, AUPHE, AUPLO, AUPPP, 
    AUPQQ, AUPTJ, AUPUG, AUROK, AUSIX, AUSOI, AUSYD, AUTCA, AUTEM, AUTIS, 
    AUTMW, AUTPR, AUTRO, AUTSV, AUTWB, AUUEE, AUUMR, AUWAM, AUWEI, AUWGA, 
    AUWMB, AUWOL, AUWSI, AUWUN, AUWYA, AUWYN, AUZBO, AWORJ, AXMHQ, AZBAK, 
    AZNAJ, BABNX, BAOMO, BASJJ, BBBGI, BDBZL, BDCGP, BDDAC, BDZYL, BEANR, 
    BEBRU, BEGNE, BELEU, BELGG, BFBOY, BFOUA, BGBOJ, BGGOZ, BGJAM, BGRDU, 
    BGSLS, BGSOF, BGTGV, BGVAR, BGVID, BHAMH, BIBJM, BJCOO, BLSBH, BMBDA, 
    BNBWN, BOCBB, BOLPB, BOSCS, BOSRB, BQKRA, BRAJU, BRBEL, BRBHZ, BRBSB, 
    BRBVB, BRCBA, BRCGR, BRCWB, BRFLN, BRFOR, BRGYN, BRJCA, BRJCB, BRJDF, 
    BRJLS, BRJNA, BRJOI, BRJPA, BRJTI, BRMAO, BRMCA, BRMCZ, BRMGF, BRMON, 
    BRNAT, BRPMW, BRPOA, BRPVH, BRRBR, BRREC, BRRIO, BRSAO, BRSLZ, BRSRA, 
    BRSSA, BRTHE, BRUBB, BRUDI, BRURG, BRVIX, BRXXX, BSCCZ, BSELH, BSFPO, 
    BSGHB, BSMHH, BSNAS, BSRSD, BSTCB, BSZSA, BTTHI, BWFRW, BWGBE, BWJWA, 
    BWMUB, BWPKW, BYMSQ, BZBMP, BZBZE, CACAL, CACHV, CAEDM, CAFAL, CAFCM, 
    CAFRE, CAFSJ, CAFSM, CAGAN, CAGOO, CAHAB, CAHAL, CAIQL, CAKML, CAKWL, 
    CALOD, CALRA, CAMIS, CAMNT, CAMTR, CANVK, CAOTT, CAPME, CAPRG, CAPRR, 
    CAQUE, CARBL, CAREB, CAREG, CASAK, CASAU, CASJB, CASJF, CASSP, CATHU, 
    CATOR, CAVAN, CAVIC, CAWND, CAWNP, CAXPK, CAXSI, CAXXX, CAYAT, CAYBE, 
    CAYCB, CAYDF, CAYEV, CAYFO, CAYGL, CAYGW, CAYGX, CAYHR, CAYMM, CAYQD, 
    CAYTH, CAYUD, CAYVB, CAYVO, CAYVQ, CAYWK, CAYXN, CAYXT, CAYXY, CAYYD, 
    CAYZF, CAZKE, CAZTM, CCCCK, CDFBM, CDFIH, CDFKI, CDLIQ, CFBBT, CFBBY, 
    CFBGF, CFBGU, CFBIV, CFCRF, CFIRO, CFODA, CGBZV, CGPNR, CHATR, CHBRN, 
    CHBSL, CHGVA, CHLUG, CHLZN, CHZRH, CIABJ, CIASK, CIBYK, CIDJO, CIHGO, 
    CIMJC, CISPY, CIZSS, CKRAR, CLCJC, CLESR, CLIPC, CLPUQ, CLSCL, CLVAP, 
    CMDLA, CMGOU, CMMVR, CMNGE, CMYAO, CNAAT, CNBJS, CNCDU, CNCGC, CNCQI, 
    CNCYN, CNDAL, CNGGZ, CNGUI, CNHAZ, CNHLH, CNHRN, CNJDZ, CNJGN, CNJIL, 
    CNJIN, CNJIU, CNJJI, CNJMU, CNJNA, CNJNG, CNJOG, CNJZI, CNLUM, CNNHN, 
    CNNIN, CNQIN, CNRZH, CNSGH, CNSHY, CNSNZ, CNTNJ, CNTYU, CNURM, CNWXS, 
    CNXAM, CNXYA, CNYIC, CNZGZ, COBAQ, COBGA, COBOG, COCLO, COCTG, COMDE, 
    COPEI, COSAC, COSSL, CRSJO, CRUPA, CUCFG, CUHAV, CUHOG, CUSCU, CUVRA, 
    CVRAI, CVSID, CWWIL, CXFFC, CYAKT, CYLCA, CYLMS, CYNIC, CYPFO, CZPRG, 
    CZUHE, DEAGB, DEBER, DEBON, DEBRE, DEBYU, DECGN, DECOT, DEDRS, DEDTM, 
    DEDUS, DEERF, DEFDH, DEFRA, DEGWT, DEHAJ, DEHAM, DEHOQ, DEJUI, DEKAE, 
    DEKEL, DELEJ, DEMUC, DENUE, DEOLO, DEOSN, DEPAD, DEQGT, DEREG, DESCN, 
    DESTR, DESWR, DEWIB, DJJIB, DKAAB, DKAAL, DKAAR, DKBLL, DKCPH, DKEBJ, 
    DKGRN, DKHOR, DKKRP, DKODE, DKRAN, DKRNN, DKSGD, DKSKR, DKSLB, DKTED, 
    DMDOM, DMRSU, DOAZS, DOCDC, DOPCN, DOPOP, DOSDQ, DZAAE, DZALG, DZCZL, 
    DZGJL, DZORN, ECGYE, ECJIP, ECSNC, ECUIO, EETLL, EGAAC, EGABS, EGALY, 
    EGASW, EGATZ, EGAUE, EGCAI, EGEMY, EGHRG, EGLXR, EGMUH, EGPSD, EGRMF, 
    EGSEW, EGSKV, EGSSH, EGUVL, EHEUN, ERASM, ESACE, ESAGP, ESALC, ESBCN, 
    ESBIO, ESBJZ, ESEAS, ESFUE, ESGRO, ESIBZ, ESJAE, ESJRZ, ESLCG, ESLEI, 
    ESLPG, ESMAD, ESMAH, ESMJV, ESODB, ESOVO, ESPMI, ESRUS, ESSBP, ESSCQ, 
    ESSDR, ESSPC, ESSVQ, ESTCI, ESVDH, ESVGO, ESVIT, ESVLC, ESVLL, ESZAZ, 
    ETADD, ETBCO, ETJIJ, ETJIM, FIENF, FIES2, FIHEL, FIIVL, FIJOE, FIJYV, 
    FIKAJ, FIKAO, FIKEM, FIKHJ, FIKOK, FIKTT, FIKUO, FILPP, FIMIK, FIOUL, 
    FIPOR, FIRVN, FISJY, FISOT, FISVL, FITKU, FITMP, FIVAA, FIVRK, FJCST, 
    FJNAN, FJSUV, FKPSY, FMPNI, FOTOR, FRAJA, FRAUR, FRBBI, FRBES, FRBIA, 
    FRBIQ, FRBOD, FRCEQ, FRCFE, FRCLY, FRCMF, FRDNR, FREBU, FREGC, FRENC, 
    FRFNI, FRFRJ, FRFSC, FRGNB, FRLAI, FRLDE, FRLIG, FRLIO, FRLLE, FRLRH, 
    FRLRT, FRMLH, FRMPL, FRMRS, FRMZM, FRNCE, FRNCY, FRNTE, FRPAR, FRPGF, 
    FRPIS, FRPUF, FRRDZ, FRRNE, FRRNS, FRSBK, FRSXB, FRTLS, FRUIP, GALBQ, 
    GALBV, GAMFF, GAMJL, GAMVE, GAOYE, GAPOG, GBABD, GBAYL, GBBBC, GBBEL, 
    GBBHM, GBBLK, GBBOH, GBBRR, GBBRS, GBBSH, GBCBT, GBCDF, GBCMG, GBCVT, 
    GBDUN, GBEDI, GBEXE, GBFIE, GBFOA, GBGLW, GBHUY, GBINV, GBISY, GBKNM, 
    GBKWL, GBLBA, GBLDY, GBLEQ, GBLER, GBLIV, GBLON, GBLSI, GBLYX, GBMME, 
    GBMNC, GBNCS, GBNQY, GBNRW, GBNTG, GBOUK, GBSHE, GBSND, GBSOU, GBSTO, 
    GBSWA, GBUNT, GBWCK, GDGND, GETBS, GFCAY, GGACI, GGSPT, GHACC, GIGIB, 
    GLGOH, GLJUV, GLSFJ, GLUAK, GLUMD, GMBJL, GNCKY, GNLEK, GPBBR, GQSSG, 
    GRAOK, GRARX, GRATH, GRCFU, GRCHQ, GRHER, GRJKH, GRJMK, GRJNX, GRJSI, 
    GRJTR, GRKGS, GRKLX, GRKVA, GRMJT, GRPVK, GRRHO, GRSKG, GRSMI, GRZTH, 
    GSXXX, GTGUA, GUAGA, GUHGT, GWOXB, GYGEO, HKHKG, HNJUT, HNRTB, HNSAP, 
    HNSDH, HNTGU, HNUII, HRDBV, HROSI, HRPUY, HRRJK, HRSPU, HRVLN, HRVZN, 
    HRZAD, HRZAG, HTJAK, HTJEE, HTPAP, HUBUD, IDAYW, IDBDO, IDDJB, IDDJJ, 
    IDDPS, IDJKT, IDMDC, IDMES, IDSUB, IDUPG, IECFN, IEDUB, IEGWY, IEKIR, 
    IELMK, IENOC, IEORK, IESLI, ILETH, ILHFA, ILTLV, ILXXX, IMDGS, INAMD, 
    INATQ, INBBI, INBHO, INBLR, INBOM, INBRC, INCCJ, INCCU, INCJB, INCOK, 
    INGAW, INGDH, INGOI, INHYD, INICD, INIDR, INIXB, INIXC, INIXG, INIXJ, 
    INIXR, INIXW, INJAI, INJDH, INJGA, INJGB, INJRH, INJSA, INJUC, INKNU, 
    INLKO, INMAA, INMYQ, INNAG, INPAT, INPNQ, INPYB, INQNB, INRAJ, INSTV, 
    INSXR, INTRI, INTRV, INUDR, INVNS, INXXX, IODGA, IQBGW, IQBSR, IQKIK, 
    IQOSM, IRABD, IRTHR, IRUIM, ISEGS, ISREY, ITAHO, ITAOI, ITBDS, ITBGO, 
    ITBIE, ITBLQ, ITBRC, ITBRI, ITCAG, ITCTA, ITEBA, ITFLR, ITGOA, ITLMP, 
    ITMIL, ITNAP, ITOLB, ITPDA, ITPEG, ITPMO, ITPNL, ITPSA, ITPSR, ITREG, 
    ITRMI, ITROM, ITSUF, ITTPS, ITTRN, ITTRS, ITTRV, ITVCE, ITVRN, JESTH, 
    JMKIN, JMMBJ, JOAMM, JOAQJ, JPAOJ, JPASJ, JPAXT, JPCHB, JPFKF, JPFUK, 
    JPGAJ, JPHAC, JPHIJ, JPHKD, JPISG, JPKCZ, JPKIJ, JPKMI, JPKMJ, JPKMQ, 
    JPKOJ, JPKUH, JPMMJ, JPMMY, JPMRW, JPMYJ, JPNGO, JPNGS, JPOIT, JPOKA, 
    JPOKJ, JPOSA, JPSDJ, JPSHS, JPSOS, JPSPK, JPTAK, JPTKS, JPTOY, JPTYO, 
    JPUBJ, JPUKB, JPUKY, JPYOK, KEMBA, KEMYD, KENBO, KGFRU, KHPNH, KICXI, 
    KITRW, KMAJN, KMYVA, KNNEV, KNSKB, KPFNJ, KRINC, KRPUS, KRSEL, KRUSN, 
    KWKWI, KYGCM, KZALA, KZAST, LAVTE, LBBEY, LCCAS, LCVIF, LIVDZ, LKCMB, 
    LKJAF, LRMLW, LSMSU, LTVIL, LUEZT, LULUX, LVRIX, LYBEN, LYSEB, LYTIP, 
    MAAGA, MAAHU, MACAS, MAFEZ, MAOUD, MAOZZ, MARAK, MARBA, MATNG, MCMON, 
    MDKIV, METGD, METIV, MFMAR, MGMJN, MGTNR, MHMAJ, MHUIT, MKOHD, MKSKP, 
    MLBKO, MMMDL, MMRGN, MNULN, MOMFM, MPSPN, MQFDF, MRNDB, MRNKC, MROUZ, 
    MSBRD, MTMLA, MUEBN, MUPLU, MURRG, MVMLE, MWBLZ, MWLLW, MX2NB, MXACA, 
    MXAGU, MXCEN, MXCJS, MXCME, MXCOL, MXCUL, MXCUN, MXCUU, MXCVM, MXCZA, 
    MXCZM, MXEPC, MXGDL, MXHMO, MXHTO, MXJAL, MXLAP, MXLCB, MXLEN, MXLMM, 
    MXLRO, MXLZC, MXMEX, MXMID, MXMLM, MXMTT, MXMTY, MXMXL, MXMZT, MXNLD, 
    MXOAX, MXPBC, MXPVR, MXPXM, MXSLP, MXSRL, MXTAM, MXTGZ, MXTIJ, MXVER, 
    MXVSA, MXZCL, MXZIH, MXZLO, MYBKI, MYBTU, MYJHB, MYKCH, MYKUA, MYKUL, 
    MYLBU, MYLGK, MYMYY, MYPEN, MYSBW, MYTOD, MYTWU, MZBEW, MZMPM, NAKAT, 
    NAKMP, NALUD, NANDU, NAOKU, NAOMD, NAOND, NASWP, NATSB, NAWDH, NCILP, 
    NCIOU, NCLIF, NCMEE, NCNOU, NCTOU, NEAJY, NEMFQ, NENIM, NERLT, NEZND, 
    NFNLK, NGABV, NGJOS, NGKAN, NGLOS, NGPHC, NIMGA, NLAMS, NLEIN, NLGRQ, 
    NLHAG, NLHTB, NLLEY, NLMST, NLRTM, NLUDE, NLUTC, NOAES, NOALF, NOBDU, 
    NOBGO, NOBNN, NOBOO, NOEVE, NOFRO, NOHAU, NOHFT, NOKKN, NOKRS, NOKSU, 
    NOLKL, NOOSL, NOSOG, NOSVG, NOTOS, NOTRD, NPJIR, NPJKR, NPJMO, NPJUM, 
    NPKTM, NRYRN, NUALO, NZAKL, NZBHE, NZCHC, NZDUD, NZHLZ, NZIVC, NZMFN, 
    NZMON, NZNSN, NZPMR, NZROT, NZTEU, NZWHK, NZWLG, NZWRE, NZZQN, OMMCT, 
    OMSLL, PAJQE, PAPAC, PEIQT, PEJAU, PEJJI, PEJUL, PELIM, PFBOB, PFHUH, 
    PFMAU, PFMOZ, PFPPT, PFRFP, PFRGI, PFUAH, PFUAP, PFXMH, PGAYU, PGAZB, 
    PGJAQ, PGLAE, PGMFO, PGPOM, PHCEB, PHCYU, PHJOL, PHMKC, PHMNL, PHNOP, 
    PHPSG, PKBHV, PKBNP, PKCJL, PKDSK, PKGIL, PKGWD, PKHDD, PKISB, PKJAG, 
    PKJIW, PKKDD, PKKDU, PKKHI, PKLHE, PKLYP, PKMFG, PKMJD, PKMPD, PKMPR, 
    PKMUX, PKMWD, PKOHT, PKPEW, PKPJG, PKPSI, PKPZH, PKRAZ, PKRWP, PKRYK, 
    PKSDT, PKSKZ, PKSUL, PKTUK, PKUET, PKWNS, PLGDN, PLKRK, PLPOZ, PLSZZ, 
    PLWAW, PLWRO, PMFSP, PNPCN, PRBQN, PRMAZ, PRPSE, PRSJU, PSGZA, PSNAB, 
    PTFAO, PTFNC, PTHOR, PTLIS, PTOPO, PTPDL, PTPXO, PTSMA, PTTER, PWROR, 
    PYASU, QADOH, RERUN, ROBUH, ROCLJ, ROCND, ROSBZ, RSBEG, RSINI, RSNVS, 
    RUAER, RUARH, RUGOJ, RUHTA, RUIKT, RUKGD, RUKHV, RUKUF, RUKZN, RULED, 
    RUMMK, RUMOW, RUMRV, RUOVB, RUROV, RUSKX, RUUCT, RUUFA, RUUUD, RUVLU, 
    RUVOG, RUVVO, RUYEK, RUYKS, RWKGL, SAAJF, SADHA, SADMM, SAJED, SAKMX, 
    SAMED, SARUH, SATIF, SATUU, SAYNB, SBGSI, SBHIR, SCVIC, SDKRT, SDKSL, 
    SEGOT, SEHEL, SEJKG, SEKID, SEKLR, SEKRN, SEKSD, SELDK, SELLA, SEMMA, 
    SENRK, SEORB, SERNB, SESDL, SESTO, SEUME, SEVBY, SEVST, SEVXO, SGSIN, 
    SHSHN, SILJU, SIMBX, SJLYR, SKBTS, SKILZ, SLFNA, SMSAI, SNDKR, SOMGQ, 
    SRPBM, SSJUB, STTMS, SVSAL, SXPHI, SYALP, SYDAM, SZMTS, SZQMN, TCGDT, 
    TDAEH, TDMQQ, TDNDJ, TFPFR, TGLFW, THBKK, THCNX, THHDY, THHKT, THNKT, 
    THPYX, THUBP, THUTH, THUTP, TJDYU, TKNKU, TLDIL, TMAGT, TNDJE, TNMIR, 
    TNSFA, TNTUN, TOTBU, TRADA, TRADB, TRADI, TRANK, TRASR, TRAYT, TRBXN, 
    TRDLM, TRDNZ, TRERC, TRERZ, TRGZT, TRIST, TRKCM, TRKSY, TRKYA, TRMDN, 
    TRMLX, TRMSR, TRSSX, TRTEK, TRTZX, TRVAN, TRVAS, TTPOS, TTSCR, TVFUN, 
    TWKHH, TWMZG, TWTPE, TZARK, TZDAR, TZDOD, TZJRO, UADNK, UAHRK, UAIEV, 
    UALWO, UANLV, UAODS, UASIP, UAUZH, UGEBB, UGJIN, UGKLA, UGULU, UMJON, 
    UMMDY, US5SV, USABI, USABQ, USABR, USABY, USACK, USACT, USADQ, USAEV, 
    USAFL, USAGS, USAHN, USAIY, USAKH, USAKN, USALB, USALO, USALW, USAMA, 
    USANB, USANC, USANP, USAOO, USAPF, USARB, USASE, USATL, USATO, USATW, 
    USATY, USAUG, USAUS, USAUW, USAWN, USAZO, USB2L, USBAL, USBCT, USBDR, 
    USBEH, USBET, USBFD, USBFL, USBGR, USBHM, USBIL, USBIS, USBJI, USBKW, 
    USBKX, USBLI, USBMG, USBNA, USBOI, USBOS, USBPT, USBRD, USBRL, USBTM, 
    USBTR, USBTV, USBUF, USBUR, USBXS, USBZN, USCAE, USCCR, USCDC, USCDV, 
    USCEC, USCGA, USCHA, USCHI, USCHO, USCHS, USCIC, USCID, USCKB, USCLD, 
    USCLE, USCLL, USCLM, USCLT, USCMH, USCMI, USCMX, USCOD, USCON, USCOS, 
    USCPR, USCRP, USCRW, USCSG, USCSL, USCSN, USCVG, USCYS, USDAB, USDAL, 
    USDAN, USDAY, USDBQ, USDEC, USDEN, USDET, USDHN, USDLG, USDLH, USDRO, 
    USDSM, USDUJ, USDUT, USDVL, USEAT, USEAU, USEKA, USEKI, USEKO, USELM, 
    USELP, USELY, USENA, USERI, USESC, USESF, USEUG, USEVV, USEWN, USEYW, 
    USFAI, USFAR, USFAT, USFBG, USFCA, USFCI, USFFT, USFHU, USFKL, USFLG, 
    USFLL, USFLO, USFMN, USFMY, USFNT, USFOD, USFRI, USFSD, USFSM, USFWA, 
    USFWB, USFYV, USGAD, USGBO, USGCC, USGCN, USGDV, USGEG, USGFK, USGJT, 
    USGLH, USGNV, USGPT, USGPZ, USGRB, USGRH, USGRR, USGSG, USGTF, USGUC, 
    USGV9, USHAR, USHFD, USHHH, USHII, USHKY, USHLN, USHNH, USHNL, USHNS, 
    USHNU, USHOM, USHON, USHOU, USHRL, USHSV, USHUF, USHVN, USHVR, USHYA, 
    USHYG, USIAG, USIBB, USICT, USIDA, USIDO, USIFP, USIJX, USILE, USILG, 
    USILI, USILM, USIND, USINL, USIPT, USISN, USITH, USITO, USIYK, USJAC, 
    USJAN, USJAX, USJBR, USJEC, USJEF, USJHM, USJHW, USJKV, USJLN, USJMS, 
    USJNU, USJOY, USJST, USJVL, USJXN, USK5T, USKCC, USKHH, USKLW, USKTB, 
    USKTN, USKUA, USLAF, USLAN, USLAR, USLAS, USLAW, USLAX, USLBB, USLBE, 
    USLCH, USLEX, USLFT, USLGB, USLIJ, USLIT, USLIU, USLMT, USLN7, USLNK, 
    USLNS, USLNY, USLRD, USLRF, USLSE, USLTS, USLUI, USLUL, USLWB, USLWS, 
    USLWT, USLYH, USLYO, USMAF, USMCE, USMCL, USMCN, USMCW, USMEI, USMEM, 
    USMES, USMFR, USMGM, USMGW, USMHE, USMHT, USMIA, USMJQ, USMKC, USMKE, 
    USMKG, USMKL, USMLB, USMLI, USMLS, USMLU, USMOB, USMOD, USMOT, USMQT, 
    USMRB, USMSN, USMSO, USMSY, USMTH, USMTJ, USMTM, USMTO, USMUE, USMVY, 
    USMWH, USMY3, USMYR, USNNS, USNYC, USOAJ, USOAK, USOGG, USOKC, USOLF, 
    USOLM, USOMA, USOME, USONT, USORF, USORH, USORL, USOSH, USOTH, USOTZ, 
    USOUI, USOWB, USOXN, USPAH, USPBI, USPCT, USPDT, USPDX, USPFN, USPGA, 
    USPGV, USPHL, USPHX, USPIA, USPIH, USPIR, USPIT, USPKB, USPLB, USPNS, 
    USPOU, USPQI, USPSC, USPSG, USPSP, USPUB, USPUW, USPVD, USPWM, USPXN, 
    USQPL, USQTR, USRAG, USRAP, USRDD, USRDG, USRFD, USRHI, USRIC, USRKD, 
    USRKS, USRMD, USRNO, USROA, USROC, USRST, USRWI, USSAC, USSAF, USSAN, 
    USSAT, USSAV, USSBA, USSBN, USSBS, USSBY, USSCC, USSCE, USSCK, USSDY, 
    USSEA, USSFO, USSGF, USSGM, USSGU, USSGY, USSHR, USSHV, USSIT, USSJC, 
    USSJT, USSLC, USSLE, USSMX, USSNA, USSNS, USSPI, USSPS, USSSI, USSTL, 
    USSTP, USSTS, USSTZ, USSUX, USSWF, USSYR, USSZH, USTCL, USTKA, USTLH, 
    USTOL, USTOP, USTPA, USTTN, USTUL, USTUP, USTUZ, USTVC, USTVF, USTVL, 
    USTWF, USTXC, USTXK, USTYR, USTYS, USUCA, USUIN, USUKI, USUNK, USUPP, 
    USUSH, USVAC, USVDZ, USVEL, USVIS, USVIV, USVLD, USVRB, USVYV, USWAS, 
    USWBE, USWLB, USWPK, USWRG, USWRL, USWTS, USWYS, USXEZ, USXHO, USXLX, 
    USYAK, USYKM, USYUM, USZGO, USZLV, USZOR, UYMVD, UZSKD, UZTAS, UZTER, 
    UZUGC, VAVAT, VCCAN, VCKTN, VCUNI, VEBLA, VECCS, VECGU, VEMAR, VEMGT, 
    VEPZO, VEURM, VEVLN, VGEIS, VGTOV, VGVIJ, VISTT, VISTX, VNHAN, VNHUI, 
    VNSGN, VUDLY, VUSAN, VUULB, VUVLI, WFFUT, WFMAU, WSAPW, XXPRN, XXXXX, 
    YEADE, YESAH, YTMAM, ZAADY, ZAAGZ, ZAALJ, ZABFN, ZACPT, ZADUR, ZAELL, 
    ZAELS, ZAGRJ, ZAHLA, ZAJNB, ZAKIM, ZAKLZ, ZALUJ, ZAMBM, ZAMEZ, ZAMGH, 
    ZAMZF, ZAMZY, ZANCS, ZANLP, ZAOUH, ZAPBZ, ZAPHW, ZAPIB, ZAPLZ, ZAPOL, 
    ZAPRY, ZAPZB, ZARCB, ZASBU, ZASIS, ZASZK, ZATCU, ZAULD, ZAUTN, ZAUTT, 
    ZAVYD, ZAWEL, ZAWVB, ZMCIP, ZMKIW, ZMLUN, ZMMFU, ZMNLA, ZWBFO, ZWBUQ, 
    ZWGWE, ZWHRE, ZWHWN, ZWMVZ, ZWVFA
};

const float City::m_position[NUMCITY][2] = { { 0.0, 0.0 }, // NOCITY
    {55.0446, 9.42097},{31.1515, 33.8311},{36.8982, 7.75493},{57.0463, 9.92153},{24.2249, 55.7452},{56.1496, 10.2134},{47.1002, 88.4342},{30.3636, 48.2591},{40.6022, -75.4713},{32.4464, -99.7476},
    {5.32036, -4.01611},{-10.8895, 142.389},{35.0841, -106.651},{45.465, -98.4878},{22.3371, 31.6258},{9.06433, 7.4893},{-36.0738, 146.914},{42.6512, -73.755},{57.1482, -2.09281},{16.868, -99.894},
    {5.55711, -0.201238},{29.0397, -13.6363},{47.4904, 9.55405},{49.717, -2.19689},{41.2728, -70.0952},{31.5492, -97.1475},{40.7907, -124.167},{39.3643, -74.4229},{37.1438, 35.4984},{9, 38.75},
    {12.8333, 44.9167},{37.7602, 38.2773},{-34.9282, 138.6},{57.79, -152.407},{-22.6806, 29.1072},{4.90199, -75.7013},{13.828, 20.8284},{43.5855, 39.7231},{62.7671, 6.16883},{30.4205, -9.58385},
    {48.3668, 10.8987},{36.7213, -4.42164},{33.471, -81.9748},{22, -102.5},{-29.2371, 18.8407},{18.3, 42.7333},{33.9598, -83.3764},{40.56, 8.31515},{35.2451, -3.93019},{41.9264, 8.7376},
    {17.4031, 42.8518},{-12.2266, 44.4107},{-10.9162, -37.0775},{16.9726, 7.99074},{-36.5413, 174.551},{58.7552, -156.519},{35.3283, 34.0435},{43.2364, 76.9457},{31.5782, -84.1557},{38.3436, -0.488171},
    {69.9666, 23.2733},{36.7754, 3.06019},{-35.0248, 117.884},{-28.5948, 16.4826},{42.4983, -92.3329},{36.1992, 37.1637},{42.5069, 1.52125},{46.2055, -118.517},{31.1992, 29.8952},{35.2072, -101.834},
    {23.0216, 72.5797},{31.9516, 35.924},{52.3731, 4.89245},{33.6625, -85.8283},{61.2163, -149.895},{39.716, 32.706},{38.9786, -76.4928},{51.2211, 4.39971},{17.1185, -61.8449},{43.4801, 13.2188},
    {40.8869, 140.59},{35.6137, 27.1193},{40.5187, -78.3947},{26.1422, -81.7943},{-13.8345, -171.765},{29.5266, 35.0075},{42.2814, -83.7485},{64.543, 40.5371},{-3.36968, 36.6881},{37.9405, 58.3823},
    {39.1911, -106.824},{-15.9277, -5.71609},{28.3777, 129.494},{6.82001, -5.2776},{15.339, 38.9327},{-23.6984, 133.881},{38.6582, 35.5546},{-25.28, -57.6344},{24.0911, 32.8973},{37.9756, 23.7348},
    {33.749, -84.3903},{39.3556, -82.061},{31.6343, 74.8737},{44.2614, -88.407},{44.8992, -97.1153},{27.1759, 31.2006},{12.5269, -70.0357},{28.8984, 33.2033},{44.317, -69.7734},{24.4538, 54.3774},
    {44.9285, 2.44331},{30.2711, -97.7437},{35.5954, -82.5508},{41.2465, -75.8817},{18.2146, -63.0518},{34.6381, -99.334},{39.6899, 140.343},{51.8161, -0.813038},{-25.3456, 131.037},{-19.5759, 147.404},
    {36.8865, 30.703},{-6.3415, 145.904},{-1.15668, 132.465},{-10.2989, 149.338},{42.2917, -85.5872},{19.2037, -69.339},{26.2235, 50.5822},{40.3756, 49.8328},{10.9939, -74.7927},{20.2603, 85.8395},
    {16.0001, -61.7333},{4.25934, 15.7878},{44.4361, 26.1027},{5.768, 20.6756},{41.3829, 2.17743},{5.78599, 36.5652},{26.3587, -80.0831},{17.2502, -88.77},{32.2937, -64.7815},{-24.8653, 152.352},
    {41.7646, -72.6909},{-6.92155, 107.611},{22.2973, 73.1943},{41.167, -73.2048},{40.6359, 17.6884},{69.0656, 18.5159},{57.4464, -7.33777},{44.8178, 20.4569},{42.1167, -86.4542},{-1.45056, -48.4682},
    {32.1288, 20.0817},{52.517, 13.3889},{48.3905, -4.48601},{60.7922, -161.756},{-19.8287, 34.8418},{33.8892, 35.5026},{41.7747, -76.5265},{35.3739, -119.019},{-29.1164, 26.2155},{-20.136, 31.2863},
    {54.5964, -5.93018},{7.16698, -73.1047},{4.36351, 18.5836},{13.0978, -59.6184},{42.1157, -75.9588},{60.3943, 5.32592},{44.8016, -68.7713},{4.74353, 22.8233},{33.3062, 44.3872},{45.7567, 9.75422},
    {35.1689, -114.639},{-41.5119, 173.955},{33.5207, -86.8024},{23.2585, 77.402},{-31.965, 141.451},{29.3946, 71.6639},{52.4797, -1.90269},{42.6994, 9.45092},{45.7875, -108.496},{43.263, -2.935},
    {43.4711, -1.55273},{46.8083, -100.784},{6.53927, 21.9872},{47.4785, -94.8908},{13.441, -16.5628},{-3.36381, 29.3675},{40.1906, 116.412},{37.0344, 27.4307},{21.0793, -101.59},{38.8896, -6.98014},
    {5.97801, 116.073},{13.8246, 100.622},{12.6133, -7.98474},{37.7782, -81.1882},{44.3764, -96.7925},{10.1326, -64.682},{37.2698, -81.2223},{48.7544, -122.479},{53.8179, -3.05098},{55.7247, 9.11958},
    {44.4938, 11.3426},{12.9768, 77.5901},{-23.4738, 148.785},{-15.7863, 35.0036},{-17.9567, 122.224},{39.167, -86.5343},{40.4798, -88.9939},{-20.8139, 149.277},{36.1623, -86.7743},{-27.469, 153.023},
    {50.7359, 7.10066},{-28.8657, 153.566},{65.4749, 12.2116},{32.9857, 70.6036},{44.7721, 17.1918},{-16.5043, -151.737},{44.8412, -0.580036},{4.65295, -74.0836},{50.7201, -1.87991},{43.6008, -116.242},
    {42.4937, 27.4721},{18.9734, 72.8281},{12.1473, -68.274},{67.3095, 13.9154},{42.3554, -71.0605},{11.1766, -4.29614},{30.086, -94.1018},{31.15, -81.4915},{18.4298, -67.1542},{-41.1334, -71.3098},
    {46.358, -94.2008},{53.0758, 8.80716},{41.1258, 16.862},{40.8127, -91.102},{46.9485, 7.45217},{57.0018, -7.46969},{51.4538, -2.5973},{50.855, 4.35123},{-10.3333, -53.2},{47.5581, 7.58783},
    {30.4952, 47.8091},{46.0131, -112.537},{30.4494, -91.187},{48.1517, 17.1093},{3.18743, 113.047},{44.2602, -72.5751},{47.4814, 19.1461},{-34.6076, -58.4371},{42.8867, -78.8784},{-12.9105, 14.0357},
    {-20.1054, 28.5427},{34.1812, -118.307},{2.82085, -60.672},{39.2909, -76.6108},{4.88955, 114.942},{-41.0524, 145.906},{33.2567, -116.378},{7.69061, -5.02984},{49.9427, 11.5763},{17.5001, -88.2003},
    {22.4934, 90.3548},{45.6794, -111.044},{-4.18751, 15.2628},{-5.0564, 12.3212},{34.0003, -81.0331},{39.2172, 9.11331},{30.0444, 31.2357},{41.0831, -81.5185},{55.4242, -5.6051},{23.1302, 113.259},
    {33.5945, -7.62003},{4.93711, -52.3258},{-17.3936, -66.1569},{52.2055, 0.118664},{-35.2976, 149.101},{51.7567, 14.3357},{11.2451, 75.7755},{-12.1459, 96.8423},{37.9769, -122.034},{10.5061, -66.9146},
    {22.5726, 88.3639},{25.4165, -77.8783},{37.6774, -113.062},{60.5439, -145.759},{10.2931, 123.902},{41.8528, -124.15},{-32.1264, 133.674},{27.4847, -109.934},{43.5515, 7.01344},{-32.8317, 151.351},
    {45.7775, 3.08194},{22.146, -80.4531},{54.9208, -7.95239},{-30.2962, 153.114},{39.6237, 19.9234},{55.4764, -133.148},{-15.5987, -56.0991},{50.9384, 6.95997},{34.7487, 113.619},{22.3338, 91.8344},
    {43.8844, 125.318},{-20.464, -54.6163},{8.32238, -62.6897},{35.0457, -85.3095},{-43.531, 172.636},{41.5755, 120.439},{41.8756, -87.6244},{38.0293, -78.4767},{35.5121, 24.0192},{33.1571, -94.9694},
    {39.7285, -121.837},{41.9759, -91.6704},{-13.6391, 32.6458},{12.7158, -61.3253},{11.0018, 76.9628},{-22.4624, -68.9272},{35.8509, 71.7902},{31.7373, -106.486},{39.2806, -80.3445},{30.0552, 107.875},
    {9.51706, -13.6998},{33.1581, -117.351},{41.4997, -81.6937},{46.7694, 23.59},{30.6184, -96.3456},{48.1181, -123.431},{3.45203, -76.5329},{19.1667, -104},{35.2272, -80.8431},{42.5677, 8.75905},
    {6.93886, 79.8542},{18.6517, -91.8149},{45.5663, 5.92036},{39.9623, -83.0007},{40.1165, -88.2431},{-22.839, 147.155},{47.1269, -88.581},{44.1729, 28.6215},{-19.9227, -43.9451},{-28.9911, -57.8126},
    {-16.9207, 145.772},{18.7883, 98.9859},{44.5263, -109.056},{9.96743, 76.2454},{43.2072, -71.5375},{6.49907, 2.62534},{-31.4167, -64.1834},{38.834, -104.825},{-29.0134, 134.754},{55.6867, 12.5701},
    {42.8494, -106.3},{-33.929, 18.4174},{4.94182, 15.8688},{30.3322, -81.6556},{27.7635, -97.4033},{32.7884, -79.9399},{32.4611, -84.988},{-28.8632, 153.048},{39.1698, -119.758},{-17.7353, 177.129},
    {37.5024, 15.0874},{10.4267, -75.5443},{-15.3718, 144.903},{30.6599, 104.063},{24.7979, -107.393},{21.1618, -86.851},{12.11, -68.9371},{28.5, -106},{39.1015, -84.5125},{23.739, -99.1441},
    {-24.8826, 113.658},{52.4082, -1.51048},{44.9596, -89.6298},{-25.4296, -49.2713},{51.4817, -3.17919},{1.872, -157.384},{-20.0714, 146.271},{41.14, -104.82},{10.8551, 121.008},{20.6829, -88.5687},
    {36.3642, 6.60843},{20.4318, -86.9203},{29.2108, -81.0228},{23.7644, 90.389},{32.7763, -96.7969},{33.5131, 36.3096},{36.588, -79.3917},{-6.81608, 39.2804},{39.7589, -84.1916},{-32.2479, 148.603},
    {42.5006, -90.6648},{42.6502, 18.0925},{-27.1823, 151.263},{38.895, -77.0365},{15.424, -61.3419},{-20.2558, 148.814},{39.8454, -88.9524},{28.6139, 77.209},{39.7392, -104.985},{42.3316, -83.0466},
    {26.2967, 50.1202},{31.2237, -85.3934},{-8.55368, 125.578},{-1.63947, 102.945},{33.7736, 10.8862},{-2.53875, 140.704},{6.87669, -6.45161},{-33.2334, 151.561},{14.6934, -17.4479},{4.04294, 9.7062},
    {38.9182, 121.628},{59.0397, -158.458},{46.7729, -92.1251},{36.7672, 28.8003},{-18.8167, 169},{26.4368, 50.104},{56.4606, -2.97019},{48.6626, 34.9502},{48.632, -2.05802},{41.0918, 29.7895},
    {-6.17912, 35.7468},{25.2856, 51.5264},{15.5477, -61.2936},{39.1582, -75.5244},{-41.1798, 146.361},{-8.45602, 115.27},{-17.3032, 123.629},{37.2395, -107.822},{51.0493, 13.7381},{-12.4604, 130.841},
    {31.8275, 70.9091},{41.591, -93.6047},{51.5142, 7.46528},{53.3494, -6.26056},{-45.8741, 170.504},{41.1187, -78.7622},{-29.8618, 31.0099},{51.2254, 6.77631},{53.8868, -166.542},{48.1128, -98.8651},
    {25.0743, 55.1885},{-22.5265, 148.317},{38.5857, 68.7604},{-12.7804, 45.228},{43.3224, -1.98389},{47.4235, -120.31},{44.8113, -91.4985},{42.7902, 10.3403},{0.0611715, 32.4699},{55.4665, 8.45208},
    {-20.2418, 57.4823},{45.4401, 4.38731},{55.9533, -3.18837},{44.8535, 0.487531},{39.644, -106.38},{65.262, -14.4035},{51.4486, 5.45012},{18.4257, -64.6232},{41.6013, -85.8591},{41.1958, -115.327},
    {25.3777, -76.7843},{-23.6697, 27.74},{42.0898, -76.8077},{31.7551, -106.488},{-33.0192, 27.8999},{39.2481, -114.892},{52.9534, -1.14965},{-23.5263, 148.162},{28.0998, 30.7585},{60.5544, -151.258},
    {48.6937, 6.18341},{68.3957, 23.6983},{19.5994, -99.0487},{-33.8582, 121.893},{39.6073, 39.2013},{50.9778, 11.0287},{42.1295, -80.0853},{39.9063, 41.2728},{60.2048, 24.6568},{45.7456, -87.0647},
    {31.291, -92.4616},{50.8215, -0.140056},{-26.2462, -69.626},{29.5569, 34.9498},{44.0505, -123.095},{26.7301, -12.9767},{68.4969, 16.907},{40.1777, 44.5126},{37.9705, -87.5716},{35.1085, -77.0441},
    {40.7216, -74.0475},{50.7256, -3.52695},{24.5548, -81.8021},{49.496, 5.98503},{62.012, -6.768},{64.8378, -147.717},{37.0163, -7.93518},{46.8772, -96.7898},{36.7394, -119.785},{35.1403, -79.1541},
    {-10.7367, 25.4982},{48.2022, -114.315},{37.56, -122.269},{14.6028, -61.0677},{47.65, 9.48009},{34.0347, -5.01619},{38.2, -84.8667},{31.5273, -110.361},{59.5326, -1.63104},{-4.3197, 15.3424},
    {25.3367, 56.1727},{49.0069, 8.40342},{0.518402, 25.2057},{39.9259, -77.7486},{37.7545, 140.459},{35.1988, -111.652},{26.1223, -80.1434},{-27.5973, -48.5496},{34.1984, -79.7672},{43.7699, 11.2556},
    {36.7291, -108.205},{52.272, 8.04764},{26.6406, -81.8723},{8.479, -13.268},{32.6497, -16.9087},{43.8374, 4.36007},{39.0168, 125.747},{43.0162, -83.69},{42.5044, -94.191},{39.049, -95.6776},
    {-3.73045, -38.5218},{60.1338, -2.0831},{26.5357, -78.6954},{50.1106, 8.68209},{39.1715, -96.7799},{43.433, 6.73602},{61.6003, 5.03496},{42.8778, 74.6067},{-21.1664, 27.5025},{41.4875, 9.12829},
    {43.5488, -96.7308},{35.388, -94.4265},{46.7775, -56.1769},{28.4004, -14.0049},{33.6251, 130.618},{-8.51996, 179.198},{-14.2769, -178.142},{41.08, -85.1386},{36.0626, -94.1574},{34.0128, -86.003},
    {38.4747, 140.083},{26.1806, 91.7539},{-24.6581, 25.9088},{23.1629, 72.6887},{44.2911, -105.502},{49.4568, -2.539},{19.2954, -81.3808},{36.0977, -112.097},{23.2233, 72.6492},{20.672, -103.338},
    {54.3483, 18.654},{21.4608, -71.14},{47.1053, -104.712},{47.6572, -117.424},{6.81374, -58.1624},{-28.7703, 114.615},{-38.1493, 144.36},{-34.2871, 146.055},{47.9252, -97.0307},{32.5007, -94.7405},
    {48.1956, -106.636},{25.2841, -76.33},{36.1286, -5.34748},{35.9208, 74.314},{36.7292, 5.96078},{39.064, -108.551},{-23.1734, 150.957},{55.8612, -4.25017},{33.4001, -91.0551},{-23.8432, 151.256},
    {45.1876, 5.73578},{12.0535, -61.7518},{51.0538, 3.72501},{29.652, -82.325},{44.4073, 8.93386},{64.1767, -51.7359},{15.3005, 74.0855},{56.3265, 44.0051},{41.3502, -72.0762},{-28.4634, 150.289},
    {57.7072, 11.967},{9.30707, 13.3935},{-12.1928, 136.684},{43.1223, 25.6888},{38.1652, 21.4087},{30.3674, -89.0928},{47.2372, -93.5302},{44.5126, -88.0126},{41.0265, -73.6285},{-33.9597, 22.4587},
    {55.7571, 8.92776},{41.9793, 2.81994},{53.2191, 6.56801},{42.9632, -85.6679},{37.9557, -3.49206},{47.0709, 15.4383},{-54.2835, -36.4946},{-9.59842, 160.149},{36.0726, -79.792},{34.8514, -82.3985},
    {-13.9805, 136.587},{47.5049, -111.292},{54.3851, 13.3106},{-43.595, 170.142},{14.6425, -90.5131},{38.6477, -107.06},{13.4727, 144.752},{46.2018, 6.1466},{25.148, 62.3255},{-19.4616, 29.8206},
    {54.9065, 8.30717},{53.2744, -9.04906},{-2.19006, -79.8869},{-16.6809, -49.2533},{-26.19, 152.66},{-23.3284, 32.8066},{36.9666, 37.4074},{33.1025, 139.808},{52.08, 4.31135},{-11.6931, 43.2543},
    {52.3745, 9.73855},{53.5673, 9.94173},{21.0283, 105.854},{-20.3467, 148.956},{40.2663, -76.8861},{59.4682, 5.08276},{23.1353, -82.359},{-42.8825, 147.328},{25.4075, 68.3613},{40.4848, -106.832},
    {6.9341, 100.388},{60.1675, 24.9427},{35.3391, 25.1333},{32.8191, 34.9984},{70.6413, 23.8361},{30.249, 120.205},{9.45747, -5.63425},{32.1618, -80.7513},{47.4272, -92.9377},{34.4775, -114.338},
    {34.3917, 132.452},{-9.4378, 159.962},{-20.0502, 148.888},{41.7688, 140.729},{22.2793, 114.163},{7.9366, 98.3529},{35.7333, -81.3443},{-25.9397, 27.9262},{46.0698, 122.087},{46.5927, -112.036},
    {-37.7434, 142.024},{-37.7879, 175.282},{29.0948, -110.969},{39.7021, 141.155},{58.1097, -135.443},{-18.344, 146.236},{21.3045, -157.856},{59.0831, -135.343},{20.9116, -75.7892},{59.6441, -151.54},
    {44.3631, -98.2144},{50.3219, 11.9179},{38.5796, -28.7096},{29.7589, -95.3677},{41.034, -73.7629},{45.7594, 126.628},{-17.8567, 31.0602},{27.2226, 33.8307},{49.9923, 36.231},{26.1908, -97.6961},
    {55.8612, 9.84448},{34.7298, -86.5859},{52.0334, 113.501},{51.6889, 5.30312},{-20.3546, 148.956},{38.4193, -82.4452},{39.4667, -87.4139},{-16.7286, -151.013},{16.4639, 107.586},{15.8313, -96.3198},
    {51.2379, -0.74109},{-25.2986, 152.854},{41.3082, -72.925},{48.5537, -109.678},{-19.189, 26.7401},{41.6535, -70.2824},{17.3606, 78.4741},{55.2081, -132.827},{43.0844, -79.0615},{38.9744, 1.41975},
    {37.456, 126.705},{37.6922, -97.3375},{43.4888, -112.036},{22.7204, 75.8682},{50.45, 30.5241},{-17.5242, 146.031},{-18.6476, 146.16},{-39.0259, -67.6081},{39.7343, -90.2288},{56.637, 104.719},
    {31.1171, -97.7278},{39.7459, -75.5466},{59.7532, -154.823},{34.2257, -77.9447},{-22.6051, 167.469},{55.7586, -6.16567},{49.2235, 18.7393},{39.7683, -86.1583},{43.3211, 21.8959},{48.601, -93.4106},
    {47.2654, 11.3928},{-0.546903, 166.922},{57.479, -4.22574},{54.1498, -4.4779},{-22.4305, 166.805},{-27.1285, -109.336},{33.0305, -115.36},{41.2493, -77.0028},{-3.74937, -73.2444},{-13.0187, 143.212},
    {10.2962, 22.781},{-20.729, 139.493},{33.6938, 73.0651},{24.3439, 124.186},{48.1465, -103.622},{35.9588, -78.5434},{40.8515, -73.0994},{41.0064, 28.9759},{42.4396, -76.4968},{19.7074, -155.082},
    {-19.0534, -169.919},{-46.4118, 168.347},{68.6588, 27.5348},{26.6989, 88.32},{30.7334, 76.7797},{15.8573, 74.5069},{32.7186, 74.8581},{23.3701, 85.325},{22.8015, 86.203},{35.6521, -117.818},
    {38.4237, 27.1428},{34.4302, 70.4601},{43.684, -110.444},{-32.1024, 115.872},{9.66509, 80.0093},{28.2813, 68.4364},{26.9155, 75.819},{18.2353, -72.5375},{17.7708, -92.8022},{42.4839, 26.5106},
    {32.2999, -90.183},{-5.57064, 151.491},{-11.7752, -75.5},{35.8272, -90.695},{-27.1737, -51.5065},{-20.6568, 141.745},{-11.1852, -40.5112},{-21.761, -43.3501},{26.2968, 73.0351},{29.2713, 117.173},
    {21.581, 39.1654},{18.6446, -74.1139},{38.5774, -92.1724},{49.1857, -2.11023},{22.4732, 70.0552},{19.0871, 82.0236},{39.7722, 98.2885},{27.0431, 114.903},{1.50356, 103.75},{56.0442, 12.7037},
    {22.0026, 100.771},{20.9656, -156.672},{-20.2896, 148.789},{42.0965, -79.238},{11.8146, 42.8453},{9.35085, 42.8004},{43.729, 126.2},{7.67562, 36.8479},{0.575157, 33.2805},{-1.51582, -80.6125},
    {27.6674, 86.2728},{29.6654, 115.947},{25.0503, 61.7466},{-7.272, -76.7509},{30.6013, 104.114},{57.7826, 14.1657},{38.3758, 26.0647},{26.7281, 85.9364},{-6.17525, 106.827},{31.9638, -95.2705},
    {37.0842, -94.5133},{31.3324, 75.5769},{-20.2672, -50.5492},{-14.6902, 16.0306},{37.4514, 25.3923},{28.7838, 83.7305},{46.9105, -98.7084},{47.0216, 132.044},{-15.4875, -44.3611},{-26.205, 28.0497},
    {35.4125, 116.585},{-34.5934, -60.9462},{43.9532, -90.1218},{37.06, 25.4708},{41.4944, 121.615},{62.6006, 29.7622},{-26.3045, -48.8487},{6.05366, 121},{16.7289, -169.534},{9.91751, 8.89794},
    {-7.1216, -34.882},{7.51792, -78.1624},{26.7578, 94.208},{-3.07641, 37.354},{31.7885, 35.2188},{26.9218, 70.9191},{39.1693, 23.4553},{-44.0515, -70.4706},{40.3267, -78.922},{-17.8784, -51.7204},
    {36.4071, 25.4567},{4.84592, 31.5959},{53.6784, 6.99561},{-23.3161, -65.7595},{-15.4932, -70.1356},{29.2389, 82.198},{-24.8334, 143.062},{14.5322, -86.3218},{72.7847, -56.147},{42.683, -89.0227},
    {-24.6004, 24.7303},{42.2416, -84.4256},{62.2417, 25.7496},{64.2241, 27.7334},{11.8948, 8.53641},{65.9646, 29.1883},{34.526, 69.1777},{-32.738, 134.616},{56.014, -132.828},{-20.5571, 147.825},
    {1.55741, 110.344},{37.783, 36.8307},{33.568, 133.539},{27.8, 66.6167},{35.2875, 75.6074},{54.3227, 10.1356},{65.7333, 24.5667},{-35.6525, 137.637},{54.7046, 20.4566},{-30.7464, 121.473},
    {-1.95085, 30.0615},{36.7937, 27.0848},{22.6203, 120.312},{24.8547, 67.0207},{62.4317, 22.1842},{48.4813, 135.076},{56.0294, 14.1567},{37.6452, 138.767},{35.4719, 44.3954},{-28.7383, 24.7642},
    {17.9712, -76.7928},{52.1453, -9.5174},{47.0245, 28.8323},{-12.8104, 28.2068},{69.7271, 30.0449},{0.317714, 32.5814},{56.6629, 16.3662},{46.6239, 14.3076},{55.5524, -133.097},{37.0373, 22.1107},
    {-29.6778, 17.0708},{32.0977, 131.295},{32.645, 130.634},{-26.5813, 18.1363},{36.4033, 136.45},{-39.8706, 143.976},{26.4609, 80.3218},{-15.7731, 128.739},{19.6472, -155.997},{58.9816, -2.96037},
    {31.5216, 130.547},{63.8391, 23.1337},{-17.4876, 140.84},{50.0469, 19.9972},{68.1667, 19.5},{56.3111, 9.17068},{58.0856, 7.93186},{15.5636, 32.5349},{59.3809, 13.5028},{16.0432, 36.0181},
    {63.2168, 7.54526},{-20.737, 116.848},{55.6878, -132.522},{27.7083, 85.3206},{55.3431, -131.647},{-14.4646, 132.264},{67.652, 24.9095},{3.79856, 103.322},{53.1986, 50.114},{42.9907, 144.382},
    {3.1517, 101.694},{62.8241, 27.5932},{40.9369, 24.4123},{29.3797, 47.9734},{25.2387, 110.175},{-15.275, 141.818},{38.0212, 32.5225},{40.4558, 42.998},{55.7824, 49.1242},{-8.82727, 13.244},
    {-6.7302, 147.001},{40.4191, -86.8919},{48.7322, -3.4588},{42.7338, -84.5554},{19.3638, -98.9463},{41.3114, -105.591},{36.1673, -115.149},{34.6087, -98.3903},{34.0537, -118.243},{53.7974, -1.54379},
    {33.5856, -101.847},{40.3173, -79.384},{43.9278, 2.1479},{-0.69426, 10.2236},{5.3174, 115.219},{0.408652, 9.44188},{34.9236, 33.6236},{43.371, -8.39594},{30.2305, -93.217},{-20.4447, 149.043},
    {43.0941, -0.0464975},{58.472, 13.1369},{54.9979, -7.32131},{-17.578, 145.695},{43.643, -72.2516},{59.9387, 30.3162},{36.8414, -2.46281},{51.3406, 12.3747},{11.7615, -12.0119},{50.1028, -5.67056},
    {-27.9141, 120.698},{50.8792, 4.70117},{38.0464, -84.497},{52.5367, 5.36104},{30.2262, -92.0178},{6.13042, 1.21583},{33.769, -118.192},{50.6451, 5.57361},{6.37004, 99.7929},{31.5657, 74.3142},
    {47.1393, 9.5228},{-20.9329, 167.166},{45.8354, 1.26448},{21.977, -159.369},{58.0971, -135.465},{50.6366, 3.06353},{-12.0621, -77.0365},{2.14739, 21.5157},{38.7078, -9.13659},{34.7465, -92.2896},
    {46.05, 14.5069},{70.0525, 24.9607},{26.8381, 80.9346},{65.5831, 22.146},{-13.9875, 33.7681},{25.7928, -108.99},{35.5111, 12.5963},{42.2249, -121.782},{40.8089, -96.7078},{-28.4162, 121.003},
    {40.0807, -76.2411},{20.8261, -156.92},{48.3059, 14.2862},{51.4893, -0.144055},{6.45506, 3.39418},{27.958, -15.6062},{-16.4955, -68.1336},{53.4072, -2.99168},{61.0584, 28.1863},{27.5031, -99.5076},
    {-23.4378, 144.259},{34.8662, -92.1101},{46.1591, -1.15204},{18.4163, -68.9232},{47.7477, -3.36609},{43.8123, -91.2514},{59.8696, -1.28621},{-41.4341, 147.137},{-28.7936, 153.267},{44.5107, 14.5037},
    {26.0098, -111.345},{-26.65, 15.1573},{46.005, 8.95203},{-31.363, 29.573},{24.4337, 98.5881},{-15.3576, 29.1653},{49.8159, 6.12967},{-27.9396, 125.445},{37.8017, -80.4456},{60.1531, -1.14273},
    {49.842, 24.0316},{46.4196, -117.022},{47.0639, -109.428},{25.7021, 32.6472},{37.4138, -79.1423},{38.345, -98.2017},{31.4221, 73.0923},{78.2232, 15.6464},{45.7578, 4.83201},{50.951, 0.906538},
    {26.0493, -108.798},{47.0505, 8.30547},{-14.6683, 145.461},{13.0837, 80.2702},{40.4167, -3.70358},{31.8369, -102.01},{39.8895, 4.26621},{7.09099, 171.382},{53.4795, -2.24511},{-3.13163, -59.9825},
    {10.6498, -71.6444},{-16.4461, -152.258},{18.2011, -67.1391},{-4.05052, 39.6672},{-25.5376, 152.702},{18.4725, -77.9217},{-31.3169, 29.9661},{43.42, -83.949},{46.5576, 15.6456},{37.1642, -120.768},
    {39.1001, -94.5781},{63.0691, -151.006},{43.7311, 7.41976},{32.8407, -83.6324},{28.5421, -81.379},{0.0401529, -51.057},{23.5882, 58.3829},{43.1525, -93.2018},{-26.6544, 153.093},{-9.64768, -35.7339},
    {1.49006, 124.841},{6.24434, -75.5736},{21.9813, 96.0824},{-37.9977, -57.5483},{28.2418, -177.375},{-34.7871, -68.4382},{24.4712, 39.6111},{-21.5044, 167.984},{32.3643, -88.7037},{-37.8142, 144.963},
    {35.146, -90.0518},{3.58967, 98.6738},{19.4326, -99.1332},{-22.3454, 30.0269},{26.2041, -98.2301},{-1.554, 13.2206},{34.3735, 73.4702},{22.1899, 113.538},{-44.619, 167.869},{-5.57744, 151.792},
    {13.5012, 7.10253},{42.3264, -122.872},{-13.1107, 31.7978},{12.1544, -86.2738},{-37.8247, 140.782},{-23.4253, -51.9382},{-30.8642, 30.3686},{32.367, -86.3007},{2.03493, 45.3419},{39.6297, -79.9559},
    {43.7095, -98.0254},{26.5383, -77.061},{60.1024, 19.9413},{42.9956, -71.4548},{25.7742, -80.1936},{20.9671, -89.6237},{61.6878, 27.2727},{45.4642, 9.18963},{-36.8882, 149.91},{35.6055, 10.7877},
    {7.40908, -7.55017},{27.3217, 68.1366},{-1.8627, 11.0587},{-15.7181, 46.3173},{43.67, -95.1501},{39.1037, 26.5548},{37.9924, -1.13054},{14.5607, 121.028},{43.035, -87.9225},{43.2485, -86.1293},
    {21.0899, -157.011},{35.6144, -88.8177},{-25.3063, 118.929},{-21.142, 149.187},{35.899, 14.5137},{28.1065, -80.6372},{4.17799, 73.5107},{47.7467, 7.33899},{41.5058, -90.5137},{19.7027, -101.192},
    {46.4085, -105.841},{32.5102, -92.1032},{6.32803, -10.7978},{38.3487, 38.3191},{55.6053, 13.0002},{54.5094, -1.42479},{-28.2756, 117.957},{36.2382, 137.969},{68.9707, 33.075},{-22.8103, 148.701},
    {24.814, 125.306},{16.7891, -62.2087},{14.5906, 120.98},{30.6913, -88.0438},{37.6391, -120.997},{48.2325, -101.296},{-22.003, 148.043},{55.6256, 37.6064},{-17.5333, -149.839},{-17.5093, 24.2759},
    {25.7092, 69.1286},{43.6112, 3.87673},{-25.9662, 32.5675},{-34.1953, 142.15},{37.3611, 40.8959},{8.56546, 16.0738},{46.4482, -87.6306},{39.4563, -77.964},{43.2962, 5.36995},{-20.1625, 57.5028},
    {44.2107, 43.135},{36.2231, -121.388},{-29.4617, 149.841},{51.2071, 0.721036},{34.7437, -87.6533},{43.0748, -89.3838},{46.8701, -113.995},{44.9773, -93.2655},{53.9025, 27.5618},{41.6298, 33.2514},
    {50.8512, 5.69098},{-29.5817, 27.8243},{29.976, -90.0782},{24.7138, -81.0904},{38.4375, -108.24},{-32.7317, 151.551},{55.1194, -131.575},{39.4843, -88.3773},{-26.4959, 31.3696},{17.9928, -94.5567},
    {25.6802, -100.315},{-19.9861, 23.4224},{48.1371, 11.5754},{20.0083, -155.648},{29.9388, 26.4895},{30.1978, 71.472},{-1.6428, 13.4339},{-34.9059, -56.1913},{10.7183, 14.4117},{41.4386, -70.6076},
    {-20.7454, 31.3341},{32.5706, 71.524},{47.1301, -119.278},{32.6405, -115.475},{-35.9098, 150.082},{-3.2166, 40.1166},{33.8395, 132.765},{12.3052, 76.6554},{33.6956, -78.89},{4.39274, 113.988},
    {-31.0278, 29.9227},{23.5692, 119.581},{49.1197, 6.17636},{23.2036, -106.421},{-34.1832, 22.1536},{-30.3278, 149.557},{21.1498, 79.0821},{39.1868, 45.4569},{-17.7993, 177.418},{40.8359, 14.2488},
    {25.0782, -77.3383},{-5.8054, -35.2081},{-1.30261, 36.8288},{43.7009, 7.26839},{54.9738, -1.61316},{-27.7628, 29.9239},{45.8992, 6.12888},{20.9127, -17.0503},{12.1192, 15.0503},{-17.918, 19.756},
    {17.1509, -62.5853},{7.37631, 13.5961},{35.1851, 136.9},{33.1155, 129.787},{35.1747, 33.3639},{13.5248, 2.10982},{18.0619, -15.9311},{-9.20047, -171.848},{-7.33836, 72.4718},{-12.9769, 28.6069},
    {27.4951, -99.5074},{-29.0564, 167.96},{-25.4819, 30.9847},{46.9759, 31.994},{22.8193, 108.363},{53.7948, -8.91935},{10.2881, 123.972},{-22.2745, 166.442},{50.4134, -5.08488},{-34.5926, 146.7},
    {58.5909, 16.1904},{-26.3181, 152.931},{-41.2711, 173.284},{-32.0469, 150.867},{8.67723, 99.731},{47.2186, -1.55414},{-32.9193, 151.78},{-25.175, 27.0642},{49.4539, 11.0773},{52.6286, 1.2924},
    {40.7127, -74.006},{-33.282, 149.097},{34.7495, -77.4208},{37.8045, -122.271},{17, -96.5},{8.07158, 22.3962},{37.8846, -4.77601},{55.3997, 10.3852},{46.4843, 30.7323},{20.8748, -156.453},
    {41.117, 20.8017},{33.5967, 71.44},{33.2394, 131.61},{26.5708, 128.026},{35.473, -97.5171},{43.0619, 141.354},{34.8581, 133.776},{-18.8136, 17.0498},{40.9233, 9.50274},{48.0906, -105.641},
    {47.0451, -122.895},{53.139, 8.2146},{-30.4402, 136.876},{41.2587, -95.9384},{-28.5519, 16.4284},{64.5128, -165.424},{37.5483, 45.0667},{43.3436, 17.8077},{-17.9083, 15.977},{34.0658, -117.648},
    {-28.0024, 153.415},{-36.2356, 149.126},{41.1495, -8.61079},{59.3786, 14.9883},{36.8448, -76.2864},{42.2626, -71.8019},{51.8971, -8.46547},{35.7044, -0.650298},{-18.6176, 146.495},{34.6199, 135.49},
    {44.0207, -88.5409},{45.5549, 18.6954},{59.9133, 10.739},{36.3416, 43.1291},{43.4065, -124.224},{66.8982, -162.598},{12.3682, -1.52709},{34.6779, -1.92931},{-33.5903, 22.2042},{60.4208, -0.770331},
    {65.0119, 25.4717},{22.7374, -12.4729},{54.9678, 82.9516},{43.3619, -5.84836},{37.7742, -87.1133},{11.8702, -15.6077},{34.1976, -119.18},{30.9202, -6.91092},{8.97145, -79.5342},{51.7177, 8.75265},
    {37.0834, -88.6},{18.5473, -72.3396},{48.8589, 2.32004},{25.6093, 85.1235},{18.8333, -98},{27.4714, 89.6337},{26.7154, -80.0533},{5.82418, -55.1663},{-23.2006, 117.671},{-34.0528, 23.3694},
    {14.5605, 121.076},{40.3497, -74.6597},{37.7393, -25.669},{45.4823, -122.737},{45.5202, -122.674},{43.107, 12.403},{4.81432, -75.6947},{5.4065, 100.256},{-31.9559, 115.861},{34.0124, 71.5787},
    {30.1587, -85.6603},{34.7744, 32.4232},{-49.3537, 70.2436},{36.9148, -111.457},{42.6985, 2.89531},{35.6132, -77.3725},{4.76766, 7.01885},{-20.3112, 118.58},{36.9775, -76.4298},{39.9527, -75.1635},
    {-23.9485, 31.1376},{33.4484, -112.074},{40.6939, -89.5891},{31.6941, -89.1306},{42.862, -112.451},{44.3684, -100.351},{46.5803, 0.340196},{40.4417, -79.9901},{26.9616, 64.1153},{39.2667, -81.5621},
    {-22.1754, 27.9687},{44.6928, -73.4556},{45.5528, -84.7839},{-34.7212, 135.859},{-33.9618, 25.6187},{34.5793, -118.117},{39.5696, 2.65007},{38.1112, 13.3524},{-40.3563, 175.611},{11.0206, -63.9074},
    {-10.1838, -48.3336},{-25.0667, -130.1},{11.5683, 104.922},{6.92074, 158.163},{36.8315, 11.945},{18.5214, 73.8545},{-4.87764, 11.9749},{30.4213, -87.2169},{-30.0325, -51.2304},{-0.715197, 8.77879},
    {-9.47433, 147.16},{19.7894, -70.6941},{61.4866, 21.7969},{10.6573, -61.518},{41.7066, -73.9284},{52.4083, 16.9335},{-14.2755, -170.705},{-20.4018, 148.583},{-17.5374, -149.566},{46.6812, -68.0155},
    {-31.43, 152.91},{50.0875, 14.4213},{42.6639, 21.1641},{-25.7459, 28.1879},{32.2205, 35.2569},{43.7159, 10.4019},{46.2307, -119.092},{31.2632, 32.3055},{18.0116, -66.614},{56.8134, -132.957},
    {25.2644, 63.4723},{33.7722, -116.495},{42.3103, 13.9576},{-51.6931, -57.8565},{-23.9058, 29.4614},{-38.3456, 141.604},{38.1876, -104.535},{43.2958, -0.368567},{-32.4909, 137.764},{18.5566, -68.3692},
    {-53.1626, -70.9078},{35.18, 129.075},{46.7304, -117.174},{44.8702, 13.8455},{41.824, -71.4128},{-8.74945, -63.8735},{38.9572, 20.7516},{20.6407, -105.22},{43.659, -70.257},{15.8693, -97.0726},
    {33.0658, -16.3362},{18.8989, 82.5591},{12.9367, 100.886},{-29.6, 30.3789},{31.3397, 69.4477},{8.27503, -62.7558},{35.609, 140.125},{-29.6826, -51.4687},{45.567, 8.08691},{34.6853, 33.0333},
    {33.1486, 73.7482},{-26.3257, 31.1447},{22.5587, 72.9627},{45.2551, 19.8452},{45.4077, 11.8734},{41.5421, 2.1139},{14.9163, -23.5095},{22.3053, 70.8028},{31.6258, -7.98916},{56.4355, 10.0426},
    {44.0806, -103.228},{-21.2075, -159.771},{33.8572, 73.7606},{49.0195, 12.0975},{34.0224, -6.83402},{-9.97654, -67.8221},{-28.7708, 32.0578},{40.5864, -122.392},{40.3353, -75.9279},{44.2726, -121.174},
    {35.7804, -78.6391},{44.3511, 2.57285},{-8.05849, -34.8848},{38.1035, 15.6398},{64.146, -21.9422},{41.1556, 1.10761},{42.2714, -89.094},{-16.8194, -151.448},{-15.1209, -147.695},{16.7967, 96.161},
    {45.6366, -89.4121},{36.1725, 27.9194},{37.5385, -77.4343},{-22.911, -43.2094},{56.9494, 24.1052},{35.4179, 119.521},{45.3268, 14.4422},{44.1048, -69.1092},{41.5861, -109.219},{25.7738, 55.9382},
    {18.7391, 7.39333},{25.07, 34.8938},{43.9471, 12.6308},{56.3, 15.2833},{46.0346, 4.07292},{55.0995, 14.7061},{39.5261, -119.813},{48.1113, -1.68002},{37.271, -79.9414},{43.1573, -77.6152},
    {-23.3782, 150.513},{41.8933, 12.4829},{7.50064, 134.624},{-32.9594, -60.6617},{-38.1381, 176.253},{43.8351, 25.958},{47.2217, 39.7096},{-19.7195, 63.4176},{-36.6203, -64.2906},{24.8645, -76.1585},
    {44.0234, -92.463},{16.3167, -86.5375},{51.9244, 4.47775},{24.6389, 46.716},{-20.88, 55.4481},{66.5026, 25.7312},{35.9424, -77.7957},{33.5914, 73.0535},{28.4176, 70.3036},{35.6876, -105.938},
    {15.3539, 44.2059},{43.9364, 12.4467},{13.699, -89.1914},{32.7174, -117.163},{-23.5507, -46.6334},{15.4936, -88.0078},{29.4246, -98.4951},{32.079, -81.0921},{34.4221, -119.703},{17.8957, -62.8508},
    {48.5141, -2.76033},{41.6834, -86.25},{35.354, -120.376},{-29.6661, 17.8831},{2.29221, 111.877},{38.366, -75.601},{45.7974, 24.1519},{70.245, -148.398},{40.7944, -77.8616},{33.4942, -111.926},
    {37.9577, -121.291},{49.2344, 6.99638},{42.9057, -8.51752},{22.8894, -83.0539},{38.2542, -85.7594},{14.771, -88.7786},{38.2678, 140.869},{62.3908, 17.3071},{18.4802, -69.9421},{43.462, -3.80997},
    {38.0673, 138.351},{34.7447, 72.3563},{47.7167, -104.156},{47.6038, -122.33},{26.9956, 15.1065},{37.5667, 126.978},{51.5388, 0.712814},{29.2035, 25.5192},{-4.62321, 55.4524},{34.7232, 10.3358},
    {18.0669, -63.0849},{67.0087, -50.6913},{-30.3155, -61.1645},{37.779, -122.42},{54.9089, 9.7898},{37.2082, -93.2923},{10.7764, 106.701},{37.1042, -113.584},{59.6226, -135.409},{31.2323, 121.469},
    {38.6512, -78.6882},{41.8026, 123.428},{25.2312, 54.219},{44.7948, -106.822},{33.9577, 130.942},{32.5135, -93.7478},{16.7205, -22.9387},{1.28992, 103.852},{44.9521, 34.1025},{-27.7994, 22.9857},
    {57.4086, -135.46},{-32.5664, 151.173},{37.3362, -121.891},{21.1661, -100.936},{43.852, 18.3867},{9.93254, -84.0796},{31.465, -100.441},{18.4653, -66.1167},{62.7954, 22.8442},{17.2961, -62.7223},
    {39.655, 66.9757},{40.6403, 22.9353},{41.9962, 21.4319},{55.2431, 9.25358},{28.5675, 33.9536},{54.1867, 45.1838},{27.6962, 68.8589},{-25.1077, -64.3495},{56.1695, 9.54951},{40.7596, -111.887},
    {44.9392, -123.033},{17.0175, 54.1011},{22.5, -100.495},{44.1183, 27.26},{14.0096, -60.9902},{-2.52953, -44.2964},{36.9726, -25.1026},{38.5811, -121.494},{37.7246, 26.8193},{34.9531, -120.436},
    {33.75, -117.87},{-2.20793, -80.9672},{52.6613, -8.63012},{36.6744, -121.655},{42.6977, 23.3217},{61.3298, 6.72036},{-20.2683, 148.838},{-15.1563, 167.051},{67.3665, 26.6303},{50.9025, -1.40419},
    {28.6819, -17.7663},{39.799, -89.644},{15.2107, 145.752},{33.9005, -98.5021},{43.5116, 16.44},{5.01415, -6.94022},{-27.8644, -54.4779},{-15.2741, -63.011},{26.6817, -105.457},{27.3366, -82.5309},
    {-17.7835, -63.1821},{-12.9823, -38.4813},{3.74188, 8.77407},{27.8644, 34.2954},{10.8329, -74.1195},{-33.4378, -70.6505},{38.628, -90.191},{59.3251, 18.0711},{44.9497, -93.0931},{48.7785, 9.18001},
    {38.4405, -122.714},{18.3411, -64.9328},{21.2095, 72.8317},{17.729, -64.759},{-7.24597, 112.738},{38.9645, 16.3021},{28.6422, 69.1916},{13.4732, 144.765},{43.6962, -114.353},{-18.1416, 178.442},
    {42.4967, -96.4059},{13.1562, -61.228},{58.97, 5.73181},{61.869, 28.8797},{37.3886, -5.99534},{56.8391, 60.6082},{41.5034, -74.0104},{-22.6768, 14.5314},{53.6288, 11.4148},{51.6196, -3.94592},
    {48.5846, 7.75071},{54.193, -8.73054},{18.0251, -63.0483},{34.0747, 74.8204},{-33.7685, 150.957},{43.0481, -76.1474},{58.2084, -6.38812},{53.3807, -1.47023},{41.2946, 36.3321},{47.7981, 13.0465},
    {-24.9612, 31.59},{22.5446, 114.055},{53.4302, 14.551},{11.1853, -60.735},{34.3426, 134.047},{22.2158, -97.8578},{36.0638, 120.378},{41.3123, 69.2787},{25.0375, 121.564},{41.6935, 44.8014},
    {-21.1343, -175.202},{-19.6482, 134.19},{26.6775, -77.2895},{28.2936, -16.6214},{33.2096, -87.5675},{-29.1979, 26.8322},{56.9562, 8.68487},{-34.383, 147.469},{40.9913, 27.3676},{38.7212, -27.2176},
    {-45.4145, 167.717},{37.9375, -107.812},{42.4415, 19.2621},{14.1057, -87.204},{43.2512, 26.5728},{16.7538, -93.116},{-5.08746, -42.805},{35.6893, 51.3896},{41.3281, 19.8184},{21.2703, 40.4158},
    {32.5317, -117.02},{32.8967, 13.1778},{-10.5786, 142.219},{42.4304, 18.6988},{62.2676, -150.03},{33.9196, 134.251},{60.4518, 22.2671},{30.4381, -84.2809},{59.4372, 24.7454},{43.6045, 1.44425},
    {32.0853, 34.7818},{61.498, 23.7603},{0.338924, 6.7313},{-31.0901, 150.929},{37.2442, 67.2831},{36.6507, 117.114},{35.7642, -5.84296},{-18.91, 47.5256},{2.80651, 104.172},{41.6529, -83.5378},
    {69.6516, 18.9559},{-20.8033, 165.156},{18.4211, -64.6388},{36.6468, 137.218},{27.9478, -82.4584},{-22.6893, 117.797},{38.0174, 12.516},{63.4304, 10.3952},{36.5338, -82.3334},{45.0678, 7.68249},
    {-31.9129, 152.461},{45.6496, 13.7773},{8.48823, 76.9475},{1.48455, 172.969},{10.805, 78.687},{-19.2484, 17.7087},{51.1282, 71.4307},{45.8067, 12.2063},{39.3033, 117.416},{-19.2569, 146.824},
    {40.2203, -74.7659},{-26.5644, -64.8824},{26.0028, 63.0506},{36.1563, -95.9928},{33.8439, 9.40014},{34.0789, -84.4131},{32.2229, -110.975},{28.3252, 37.6329},{44.7606, -85.6165},{48.1172, -96.1771},
    {39.0885, -120.05},{-27.561, 151.953},{42.5704, -114.46},{4.24352, 117.885},{33.4254, -94.0431},{37.87, 112.545},{35.6841, 139.774},{30.7564, -94.3985},{35.9604, -83.921},{41.0055, 39.7301},
    {-8.90966, -139.552},{61.1556, -45.4239},{-9.40108, -140.08},{-19.7508, -47.9367},{33.9519, 131.247},{15.188, 105.327},{43.1009, -75.2327},{63.5624, 53.6842},{51.6591, 5.61486},{-18.9188, -48.2768},
    {48.6224, 22.3023},{24.5787, 73.6863},{-42.0801, 145.555},{30.1958, 67.0172},{54.7261, 55.9475},{41.5753, 60.5999},{-6.95168, 15.4799},{49.0681, 17.4664},{16.0966, -86.9434},{39.9356, -91.4099},
    {-0.220164, -78.5123},{47.996, -4.10248},{5.85139, 169.623},{34.6932, 135.194},{39.1502, -123.208},{35.021, 135.756},{-16.3325, 168.301},{-28.2919, 31.4294},{47.9409, 106.918},{3.0199, 32.3883},
    {70.6749, -52.1269},{63.8257, 20.2631},{-31.1999, 136.825},{12.5972, -61.4335},{63.8731, -160.788},{60.7595, -0.896019},{-5.1343, 119.412},{10.8682, -85.1446},{19.4061, -102.034},{20.2685, -155.851},
    {-20.8836, -51.3714},{43.4198, 87.3195},{-29.7561, -57.0868},{5.35668, -62.6698},{32.813, 66.0183},{-54.8069, -68.3073},{35.5392, 129.312},{52.0907, 5.12156},{17.5212, 102.668},{-28.4563, 21.2419},
    {12.9356, 100.883},{-31.5891, 28.7891},{51.8358, 107.584},{1.59979, 11.5751},{13.773, -60.958},{25.3897, 30.5552},{63.0958, 21.6159},{38.325, 43.659},{-32.5976, -70.853},{43.2074, 27.9167},
    {39.4192, 37.1012},{41.9034, 12.4529},{45.7796, 10.4259},{57.6379, 18.298},{45.4372, 12.3346},{27.8097, -17.9151},{61.1299, -146.349},{40.4557, -109.528},{19.3333, -96.6667},{-17.9229, 25.8477},
    {42.1964, -8.71178},{43.8013, 22.6795},{48.2084, 16.3725},{18.4654, -64.4155},{36.3302, -119.292},{42.8465, -2.6724},{38.9014, -77.2652},{-20.3201, -40.3377},{39.4697, -0.376335},{30.8327, -83.2785},
    {-17.7415, 168.315},{41.6521, -4.72856},{10.17, -68.0004},{56.3424, 30.5279},{54.687, 25.2829},{25.3356, 83.0076},{48.6484, 44.385},{30.4086, -86.6026},{23.1496, -81.26},{27.6387, -80.3975},
    {62.3176, 27.8681},{45.4385, 10.9924},{18.0018, -92.933},{59.6111, 16.5464},{17.9641, 102.613},{43.1151, 131.886},{56.8787, 14.8094},{-27.7705, 30.7886},{37.906, -122.545},{46.308, 16.3378},
    {52.2337, 21.0714},{-22.5776, 17.0773},{-12.6387, 141.871},{-27.9823, 26.738},{-35.115, 147.368},{-37.9519, 176.995},{-39.6202, 143.984},{58.4426, -3.09158},{50.082, 8.24166},{56.3057, -133.618},
    {-41.2888, 174.777},{-13.282, -176.174},{-38.3826, 142.481},{26.3996, 68.0377},{-34.4244, 150.894},{28.5978, -81.351},{-35.7275, 174.319},{56.2046, -132.043},{44.0168, -107.956},{51.109, 17.0327},
    {30.5951, 114.3},{-25.3747, 122.377},{31.5777, 120.295},{-34.1712, 22.1207},{-33.0382, 137.584},{-15.4868, 128.124},{44.6632, -111.101},{-10.4913, 105.617},{34.3312, 108.704},{58.6228, -101.486},
    {41.0534, -73.5387},{-14.3984, -145.941},{24.4801, 118.085},{55.748, -101.312},{36.6817, -6.13774},{56.7791, -98.9278},{0, 0},{59.5727, -139.578},{3.86899, 11.5213},{52.9244, -82.427},
    {59.5691, -108.616},{69.1178, -105.06},{49.183, -57.432},{53.5462, -113.491},{73.6585, -118.441},{52.213, -81.6824},{63.7493, -68.5214},{45.9348, -66.6556},{54.7698, -101.879},{-13.5402, -71.9302},
    {55.2821, -77.7563},{56.345, -94.7062},{50.4686, -59.6161},{44.6486, -63.5859},{51.2253, -58.6495},{30.6941, 111.28},{50.6758, -120.339},{46.6016, -120.511},{62.0274, 129.732},{56.4615, -100.017},
    {49.8879, -119.496},{56.7292, -111.389},{45.5032, -73.5698},{24.0889, 38.0667},{35.4444, 139.637},{58.5025, -119.4},{45.4209, -75.6901},{49.8179, -64.3515},{54.3127, -130.325},{52.4761, -71.8259},
    {53.8224, -101.241},{42.3167, -83.0373},{46.098, -64.8001},{50.4488, -104.617},{49.8097, -88.4812},{48.9592, -54.6188},{74.6791, -94.8423},{45.2788, -66.058},{60.0059, -111.89},{73.0349, -84.5407},
    {55.7433, -97.8635},{43.6535, -79.3839},{56.553, -76.5458},{32.6927, -114.628},{68.7927, -81.244},{48.3062, -65.5717},{48.1023, -77.7876},{58.1067, -68.4039},{65.2824, -126.833},{49.2609, -123.114},
    {49.8955, -97.1385},{52.9, -66.8702},{52.1318, -106.661},{45.2773, -66.0714},{62.1734, -92.5864},{53.9129, -122.745},{54.5173, -128.6},{42.9832, -81.2434},{60.7216, -135.055},{51.0456, -114.058},
    {54.7792, -127.176},{48.4283, -123.365},{58.7693, -94.1737},{53.3333, -60.4167},{53.8301, -112.333},{43.5896, -79.6444},{62.4541, -114.377},{53.2435, -131.828},{44.1169, 15.2353},{45.8426, 15.9622},
    {41.6521, -0.880943},{-20.0121, 148.246},{23.0824, -103.209},{16.8076, -92.8987},{52.2874, -81.6564},{19.1277, -104.284},{13.8063, 8.98917},{-23.3574, 119.733},{-45.0322, 168.661},{47.3745, 8.54104},
    {24.032, -74.49},{4.949, -6.09217},{37.7891, 20.7901},{55.8609, -92.0898},{24.7359, 91.6852}
};

const unsigned char City::m_capital[NUMCITY] = { 0, // NOCITY
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 
    0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 
    1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 
    0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 
    0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 
    0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
    0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 
    0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 
    0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 
    0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 
    0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 
    1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0
};

const char * const City::m_codes3[NUMCITY] = { "NOCITY", 
    "AAB", "AAC", "AAE", "AAL", "AAN", "AAR", "AAT", "ABD", "ABE", "ABI", 
    "ABJ", "ABM", "ABQ", "ABR", "ABS", "ABV", "ABX", "ABY", "ABZ", "ACA", 
    "ACC", "ACE", "ACH", "ACI", "ACK", "ACT", "ACV", "ACY", "ADA", "ADD", 
    "ADE", "ADF", "ADL", "ADQ", "ADY", "ADZ", "AEH", "AER", "AES", "AGA", 
    "AGB", "AGP", "AGS", "AGU", "AGZ", "AHB", "AHN", "AHO", "AHU", "AJA", 
    "AJF", "AJN", "AJU", "AJY", "AKL", "AKN", "AKT", "ALA", "ALB", "ALC", 
    "ALF", "ALG", "ALH", "ALJ", "ALO", "ALP", "ALV", "ALW", "ALY", "AMA", 
    "AMD", "AMM", "AMS", "ANB", "ANC", "ANK", "ANP", "ANR", "ANU", "AOI", 
    "AOJ", "AOK", "AOO", "APF", "APW", "AQJ", "ARB", "ARH", "ARK", "ASB", 
    "ASE", "ASI", "ASJ", "ASK", "ASM", "ASP", "ASR", "ASU", "ASW", "ATH", 
    "ATL", "ATO", "ATQ", "ATW", "ATY", "ATZ", "AUA", "AUE", "AUG", "AUH", 
    "AUR", "AUS", "AVL", "AVP", "AXA", "AXS", "AXT", "AYL", "AYQ", "AYR", 
    "AYT", "AYU", "AYW", "AZB", "AZO", "AZS", "BAH", "BAK", "BAQ", "BBI", 
    "BBR", "BBT", "BBU", "BBY", "BCN", "BCO", "BCT", "BCV", "BDA", "BDB", 
    "BDL", "BDO", "BDQ", "BDR", "BDS", "BDU", "BEB", "BEG", "BEH", "BEL", 
    "BEN", "BER", "BES", "BET", "BEW", "BEY", "BFD", "BFL", "BFN", "BFO", 
    "BFS", "BGA", "BGF", "BGI", "BGM", "BGO", "BGR", "BGU", "BGW", "BGY", 
    "BHC", "BHE", "BHM", "BHO", "BHQ", "BHV", "BHX", "BIA", "BIL", "BIO", 
    "BIQ", "BIS", "BIV", "BJI", "BJL", "BJM", "BJS", "BJV", "BJX", "BJZ", 
    "BKI", "BKK", "BKO", "BKW", "BKX", "BLA", "BLF", "BLI", "BLK", "BLL", 
    "BLQ", "BLR", "BLT", "BLZ", "BME", "BMG", "BMI", "BMP", "BNA", "BNE", 
    "BNJ", "BNK", "BNN", "BNP", "BNX", "BOB", "BOD", "BOG", "BOH", "BOI", 
    "BOJ", "BOM", "BON", "BOO", "BOS", "BOY", "BPT", "BQK", "BQN", "BRC", 
    "BRD", "BRE", "BRI", "BRL", "BRN", "BRR", "BRS", "BRU", "BSB", "BSL", 
    "BSR", "BTM", "BTR", "BTS", "BTU", "BTV", "BUD", "BUE", "BUF", "BUG", 
    "BUQ", "BUR", "BVB", "BWI", "BWN", "BWT", "BXS", "BYK", "BYU", "BZE", 
    "BZL", "BZN", "BZV", "CAB", "CAE", "CAG", "CAI", "CAK", "CAL", "CAN", 
    "CAS", "CAY", "CBB", "CBG", "CBR", "CBU", "CCJ", "CCK", "CCR", "CCS", 
    "CCU", "CCZ", "CDC", "CDV", "CEB", "CEC", "CED", "CEN", "CEQ", "CES", 
    "CFE", "CFG", "CFN", "CFS", "CFU", "CGA", "CGB", "CGN", "CGO", "CGP", 
    "CGQ", "CGR", "CGU", "CHA", "CHC", "CHG", "CHI", "CHO", "CHQ", "CHS", 
    "CIC", "CID", "CIP", "CIW", "CJB", "CJC", "CJL", "CJS", "CKB", "CKG", 
    "CKY", "CLD", "CLE", "CLJ", "CLL", "CLM", "CLO", "CLQ", "CLT", "CLY", 
    "CMB", "CME", "CMF", "CMH", "CMI", "CMQ", "CMX", "CND", "CNF", "CNQ", 
    "CNS", "CNX", "COD", "COK", "CON", "COO", "COR", "COS", "CPD", "CPH", 
    "CPR", "CPT", "CRF", "CRG", "CRP", "CRW", "CSG", "CSI", "CSN", "CST", 
    "CTA", "CTG", "CTN", "CTU", "CUL", "CUN", "CUR", "CUU", "CVG", "CVM", 
    "CVQ", "CVT", "CWA", "CWB", "CWL", "CXI", "CXT", "CYS", "CYU", "CZA", 
    "CZL", "CZM", "DAB", "DAC", "DAL", "DAM", "DAN", "DAR", "DAY", "DBO", 
    "DBQ", "DBV", "DBY", "DCA", "DCF", "DDI", "DEC", "DEL", "DEN", "DET", 
    "DHA", "DHN", "DIL", "DJB", "DJE", "DJJ", "DJO", "DKI", "DKR", "DLA", 
    "DLC", "DLG", "DLH", "DLM", "DLY", "DMM", "DND", "DNK", "DNR", "DNZ", 
    "DOD", "DOH", "DOM", "DOV", "DPO", "DPS", "DRB", "DRO", "DRS", "DRW", 
    "DSK", "DSM", "DTM", "DUB", "DUD", "DUJ", "DUR", "DUS", "DUT", "DVL", 
    "DXB", "DYA", "DYU", "DZA", "EAS", "EAT", "EAU", "EBA", "EBB", "EBJ", 
    "EBN", "EBU", "EDI", "EGC", "EGE", "EGS", "EIN", "EIS", "EKI", "EKO", 
    "ELH", "ELL", "ELM", "ELP", "ELS", "ELY", "EMA", "EMD", "EMY", "ENA", 
    "ENC", "ENF", "EPC", "EPR", "ERC", "ERF", "ERI", "ERZ", "ES2", "ESC", 
    "ESF", "ESH", "ESR", "ETH", "EUG", "EUN", "EVE", "EVN", "EVV", "EWN", 
    "EWR", "EXT", "EYW", "EZT", "FAE", "FAI", "FAO", "FAR", "FAT", "FAY", 
    "FBM", "FCA", "FCI", "FDF", "FDH", "FEZ", "FFT", "FHU", "FIE", "FIH", 
    "FJR", "FKB", "FKI", "FKL", "FKS", "FLG", "FLL", "FLN", "FLO", "FLR", 
    "FMN", "FMO", "FMY", "FNA", "FNC", "FNI", "FNJ", "FNT", "FOD", "FOE", 
    "FOR", "FOU", "FPO", "FRA", "FRI", "FRJ", "FRO", "FRU", "FRW", "FSC", 
    "FSD", "FSM", "FSP", "FUE", "FUK", "FUN", "FUT", "FWA", "FYV", "GAD", 
    "GAJ", "GAU", "GBE", "GC0", "GCC", "GCI", "GCM", "GCN", "GDH", "GDL", 
    "GDN", "GDT", "GDV", "GEG", "GEO", "GET", "GEX", "GFF", "GFK", "GGG", 
    "GGW", "GHB", "GIB", "GIL", "GJL", "GJT", "GKL", "GLA", "GLH", "GLT", 
    "GNB", "GND", "GNE", "GNV", "GOA", "GOH", "GOI", "GOJ", "GON", "GOO", 
    "GOT", "GOU", "GOV", "GOZ", "GPA", "GPT", "GPZ", "GRB", "GRH", "GRJ", 
    "GRN", "GRO", "GRQ", "GRR", "GRX", "GRZ", "GS0", "GSI", "GSO", "GSP", 
    "GTE", "GTF", "GTI", "GTN", "GUA", "GUC", "GUM", "GVA", "GWD", "GWE", 
    "GWT", "GWY", "GYE", "GYN", "GYP", "GZA", "GZT", "HAC", "HAG", "HAH", 
    "HAJ", "HAM", "HAN", "HAP", "HAR", "HAU", "HAV", "HBA", "HDD", "HDN", 
    "HDY", "HEL", "HER", "HFA", "HFT", "HGH", "HGO", "HHH", "HIB", "HII", 
    "HIJ", "HIR", "HIS", "HKD", "HKG", "HKT", "HKY", "HLA", "HLH", "HLN", 
    "HLT", "HLZ", "HMO", "HNA", "HNH", "HNK", "HNL", "HNS", "HOG", "HOM", 
    "HON", "HOQ", "HOR", "HOU", "HPN", "HRB", "HRE", "HRG", "HRK", "HRL", 
    "HS0", "HSV", "HTA", "HTB", "HTI", "HTS", "HUF", "HUH", "HUI", "HUX", 
    "HUY", "HVB", "HVN", "HVR", "HWN", "HYA", "HYD", "HYG", "IAG", "IBZ", 
    "ICN", "ICT", "IDA", "IDR", "IEV", "IFL", "IGH", "IGR", "IJX", "IKT", 
    "ILE", "ILG", "ILI", "ILM", "ILP", "ILY", "ILZ", "IND", "INI", "INL", 
    "INN", "INU", "INV", "IOM", "IOU", "IPC", "IPL", "IPT", "IQT", "IRG", 
    "IRO", "ISA", "ISB", "ISG", "ISL", "ISO", "ISP", "IST", "ITH", "ITO", 
    "IUE", "IVC", "IVL", "IXB", "IXC", "IXG", "IXJ", "IXR", "IXW", "IYK", 
    "IZM", "JAA", "JAC", "JAD", "JAF", "JAG", "JAI", "JAK", "JAL", "JAM", 
    "JAN", "JAQ", "JAU", "JBR", "JCB", "JCK", "JCM", "JDF", "JDH", "JDZ", 
    "JED", "JEE", "JEF", "JER", "JGA", "JGB", "JGN", "JGS", "JHB", "JHE", 
    "JHG", "JHM", "JHQ", "JHW", "JIB", "JIJ", "JIL", "JIM", "JIN", "JIP", 
    "JIR", "JIU", "JIW", "JJI", "JJN", "JKG", "JKH", "JKR", "JKT", "JKV", 
    "JLN", "JLR", "JLS", "JMB", "JMK", "JMO", "JMS", "JMU", "JNA", "JNB", 
    "JNG", "JNI", "JNU", "JNX", "JNZ", "JOE", "JOI", "JOL", "JON", "JOS", 
    "JPA", "JQE", "JRH", "JRO", "JRS", "JSA", "JSI", "JSM", "JST", "JTI", 
    "JTR", "JUB", "JUI", "JUJ", "JUL", "JUM", "JUN", "JUT", "JUV", "JVL", 
    "JWA", "JXN", "JYV", "KAJ", "KAN", "KAO", "KBL", "KBY", "KCC", "KCE", 
    "KCH", "KCM", "KCZ", "KDD", "KDU", "KEL", "KEM", "KGC", "KGD", "KGI", 
    "KGL", "KGS", "KHH", "KHI", "KHJ", "KHV", "KID", "KIJ", "KIK", "KIM", 
    "KIN", "KIR", "KIV", "KIW", "KKN", "KLA", "KLR", "KLU", "KLW", "KLX", 
    "KLZ", "KMI", "KMJ", "KMP", "KMQ", "KNS", "KNU", "KNX", "KOA", "KOI", 
    "KOJ", "KOK", "KRB", "KRK", "KRN", "KRP", "KRS", "KRT", "KSD", "KSL", 
    "KSU", "KTA", "KTB", "KTM", "KTN", "KTR", "KTT", "KUA", "KUF", "KUH", 
    "KUL", "KUO", "KVA", "KWI", "KWL", "KWM", "KYA", "KYS", "KZN", "LAD", 
    "LAE", "LAF", "LAI", "LAN", "LAP", "LAR", "LAS", "LAW", "LAX", "LBA", 
    "LBB", "LBE", "LBI", "LBQ", "LBU", "LBV", "LCA", "LCG", "LCH", "LDC", 
    "LDE", "LDK", "LDY", "LEA", "LEB", "LED", "LEI", "LEJ", "LEK", "LEQ", 
    "LER", "LEU", "LEX", "LEY", "LFT", "LFW", "LGB", "LGG", "LGK", "LHE", 
    "LI0", "LIF", "LIG", "LIH", "LIJ", "LIL", "LIM", "LIQ", "LIS", "LIT", 
    "LJU", "LKL", "LKO", "LLA", "LLW", "LMM", "LMP", "LMT", "LNK", "LNO", 
    "LNS", "LNY", "LNZ", "LON", "LOS", "LPA", "LPB", "LPL", "LPP", "LRD", 
    "LRE", "LRF", "LRH", "LRM", "LRT", "LSE", "LSI", "LST", "LSY", "LSZ", 
    "LTO", "LUD", "LUG", "LUJ", "LUM", "LUN", "LUX", "LVO", "LWB", "LWK", 
    "LWO", "LWS", "LWT", "LXR", "LYH", "LYO", "LYP", "LYR", "LYS", "LYX", 
    "LZC", "LZN", "LZR", "MAA", "MAD", "MAF", "MAH", "MAJ", "MAN", "MAO", 
    "MAR", "MAU", "MAZ", "MBA", "MBH", "MBJ", "MBM", "MBS", "MBX", "MCE", 
    "MCI", "MCL", "MCM", "MCN", "MCO", "MCP", "MCT", "MCW", "MCY", "MCZ", 
    "MDC", "MDE", "MDL", "MDQ", "MDY", "MDZ", "MED", "MEE", "MEI", "MEL", 
    "MEM", "MES", "MEX", "MEZ", "MFE", "MFF", "MFG", "MFM", "MFN", "MFO", 
    "MFQ", "MFR", "MFU", "MGA", "MGB", "MGF", "MGH", "MGM", "MGQ", "MGW", 
    "MHE", "MHH", "MHQ", "MHT", "MIA", "MID", "MIK", "MIL", "MIM", "MIR", 
    "MJC", "MJD", "MJL", "MJN", "MJQ", "MJT", "MJV", "MKC", "MKE", "MKG", 
    "MKK", "MKL", "MKR", "MKY", "MLA", "MLB", "MLE", "MLH", "MLI", "MLM", 
    "MLS", "MLU", "MLW", "MLX", "MMA", "MME", "MMG", "MMJ", "MMK", "MMM", 
    "MMY", "MNI", "MNL", "MOB", "MOD", "MOT", "MOV", "MOW", "MOZ", "MPA", 
    "MPD", "MPL", "MPM", "MQL", "MQM", "MQQ", "MQT", "MRB", "MRS", "MRU", 
    "MRV", "MRY", "MRZ", "MSE", "MSL", "MSN", "MSO", "MSP", "MSQ", "MSR", 
    "MST", "MSU", "MSY", "MTH", "MTJ", "MTL", "MTM", "MTO", "MTS", "MTT", 
    "MTY", "MUB", "MUC", "MUE", "MUH", "MUX", "MVB", "MVD", "MVR", "MVY", 
    "MVZ", "MWD", "MWH", "MXL", "MYA", "MYD", "MYJ", "MYQ", "MYR", "MYY", 
    "MZF", "MZG", "MZM", "MZT", "MZY", "NAA", "NAG", "NAJ", "NAN", "NAP", 
    "NAS", "NAT", "NBO", "NCE", "NCL", "NCS", "NCY", "NDB", "NDJ", "NDU", 
    "NEV", "NGE", "NGO", "NGS", "NIC", "NIM", "NKC", "NKU", "NKW", "NLA", 
    "NLD", "NLK", "NLP", "NLV", "NNG", "NOC", "NOP", "NOU", "NQY", "NRA", 
    "NRK", "NSA", "NSN", "NSO", "NST", "NTE", "NTL", "NTY", "NUE", "NWI", 
    "NYC", "OAG", "OAJ", "OAK", "OAX", "ODA", "ODB", "ODE", "ODS", "OGG", 
    "OHD", "OHT", "OIT", "OKA", "OKC", "OKD", "OKJ", "OKU", "OLB", "OLF", 
    "OLM", "OLO", "OLP", "OMA", "OMD", "OME", "OMH", "OMO", "OND", "ONT", 
    "OOL", "OOM", "OPO", "ORB", "ORF", "ORH", "ORK", "ORN", "ORS", "OSA", 
    "OSH", "OSI", "OSL", "OSM", "OTH", "OTZ", "OUA", "OUD", "OUH", "OUK", 
    "OUL", "OUZ", "OVB", "OVD", "OWB", "OXB", "OXR", "OZZ", "PAC", "PAD", 
    "PAH", "PAP", "PAR", "PAT", "PBC", "PBH", "PBI", "PBM", "PBO", "PBZ", 
    "PC0", "PCT", "PDL", "PDT", "PDX", "PEG", "PEI", "PEN", "PER", "PEW", 
    "PFN", "PFO", "PFR", "PGA", "PGF", "PGV", "PHC", "PHE", "PHF", "PHL", 
    "PHW", "PHX", "PIA", "PIB", "PIH", "PIR", "PIS", "PIT", "PJG", "PKB", 
    "PKW", "PLB", "PLN", "PLO", "PLZ", "PMD", "PMI", "PMO", "PMR", "PMV", 
    "PMW", "PN0", "PNH", "PNI", "PNL", "PNQ", "PNR", "PNS", "POA", "POG", 
    "POM", "POP", "POR", "POS", "POU", "POZ", "PPG", "PPP", "PPT", "PQI", 
    "PQQ", "PRG", "PRN", "PRY", "PS0", "PSA", "PSC", "PSD", "PSE", "PSG", 
    "PSI", "PSP", "PSR", "PSY", "PTG", "PTJ", "PUB", "PUF", "PUG", "PUJ", 
    "PUQ", "PUS", "PUW", "PUY", "PVD", "PVH", "PVK", "PVR", "PWM", "PXM", 
    "PXO", "PYB", "PYX", "PZB", "PZH", "PZO", "QCB", "QGF", "QIC", "QLI", 
    "QML", "QMN", "QNB", "QND", "QPA", "QSA", "RAI", "RAJ", "RAK", "RAN", 
    "RAP", "RAR", "RAZ", "RB0", "RBA", "RBR", "RCB", "RDD", "RDG", "RDM", 
    "RDU", "RDZ", "REC", "REG", "REK", "REU", "RFD", "RFP", "RGI", "RGN", 
    "RHI", "RHO", "RIC", "RIO", "RIX", "RIZ", "RJK", "RKD", "RKS", "RKT", 
    "RLT", "RMF", "RMI", "RNB", "RNE", "RNN", "RNO", "RNS", "ROA", "ROC", 
    "ROK", "ROM", "ROR", "ROS", "ROT", "ROU", "ROV", "RRG", "RSA", "RSD", 
    "RST", "RTB", "RTM", "RUH", "RUN", "RVN", "RWI", "RWP", "RYK", "SAF", 
    "SAH", "SAI", "SAL", "SAN", "SAO", "SAP", "SAT", "SAV", "SBA", "SBH", 
    "SBK", "SBN", "SBP", "SBU", "SBW", "SBY", "SBZ", "SCC", "SCE", "SCF", 
    "SCK", "SCN", "SCQ", "SCU", "SDF", "SDH", "SDJ", "SDL", "SDQ", "SDR", 
    "SDS", "SDT", "SDY", "SEA", "SEB", "SEL", "SEN", "SEW", "SEZ", "SFA", 
    "SFG", "SFJ", "SFN", "SFO", "SGD", "SGF", "SGN", "SGU", "SGY", "SHA", 
    "SHD", "SHE", "SHJ", "SHR", "SHS", "SHV", "SID", "SIN", "SIP", "SIS", 
    "SIT", "SIX", "SJC", "SJD", "SJJ", "SJO", "SJT", "SJU", "SJY", "SKB", 
    "SKD", "SKG", "SKP", "SKS", "SKV", "SKX", "SKZ", "SLA", "SLB", "SLC", 
    "SLE", "SLL", "SLP", "SLS", "SLU", "SLZ", "SMA", "SMF", "SMI", "SMX", 
    "SNA", "SNC", "SNN", "SNS", "SOF", "SOG", "SOI", "SON", "SOT", "SOU", 
    "SPC", "SPI", "SPN", "SPS", "SPU", "SPY", "SRA", "SRB", "SRL", "SRQ", 
    "SRZ", "SSA", "SSG", "SSH", "SSL", "STI", "STL", "STO", "STP", "STR", 
    "STS", "STT", "STV", "STX", "SUB", "SUF", "SUL", "SUM", "SUN", "SUV", 
    "SUX", "SVD", "SVG", "SVL", "SVQ", "SVX", "SWF", "SWP", "SWR", "SWS", 
    "SXB", "SXL", "SXM", "SXR", "SYD", "SYR", "SYY", "SZD", "SZF", "SZG", 
    "SZK", "SZX", "SZZ", "TAB", "TAK", "TAM", "TAO", "TAS", "TAY", "TBS", 
    "TBU", "TCA", "TCB", "TCI", "TCL", "TCU", "TED", "TEM", "TEQ", "TER", 
    "TEU", "TEX", "TGD", "TGU", "TGV", "TGZ", "THE", "THR", "TIA", "TIF", 
    "TIJ", "TIP", "TIS", "TIV", "TKA", "TKS", "TKU", "TLH", "TLL", "TLS", 
    "TLV", "TMP", "TMS", "TMW", "TMZ", "TNA", "TNG", "TNR", "TOD", "TOL", 
    "TOS", "TOU", "TOV", "TOY", "TPA", "TPR", "TPS", "TRD", "TRI", "TRN", 
    "TRO", "TRS", "TRV", "TRW", "TRZ", "TSB", "TSE", "TSF", "TSN", "TSV", 
    "TTN", "TUC", "TUK", "TUL", "TUN", "TUP", "TUS", "TUU", "TVC", "TVF", 
    "TVL", "TWB", "TWF", "TWU", "TXK", "TYN", "TYO", "TYR", "TYS", "TZX", 
    "UAH", "UAK", "UAP", "UBA", "UBJ", "UBP", "UCA", "UCT", "UDE", "UDI", 
    "UDJ", "UDR", "UEE", "UET", "UFA", "UGC", "UGO", "UHE", "UII", "UIN", 
    "UIO", "UIP", "UIT", "UKB", "UKI", "UKY", "ULB", "ULD", "ULN", "ULU", 
    "UMD", "UME", "UMR", "UNI", "UNK", "UNT", "UPG", "UPL", "UPN", "UPP", 
    "URB", "URC", "URG", "URM", "URZ", "USH", "USN", "UTC", "UTH", "UTN", 
    "UTP", "UTT", "UUD", "UVE", "UVF", "UVL", "VAA", "VAN", "VAP", "VAR", 
    "VAS", "VAT", "VBS", "VBY", "VCE", "VDE", "VDZ", "VEL", "VER", "VFA", 
    "VGO", "VID", "VIE", "VIJ", "VIS", "VIT", "VIV", "VIX", "VLC", "VLD", 
    "VLI", "VLL", "VLN", "VLU", "VNO", "VNS", "VOG", "VPS", "VRA", "VRB", 
    "VRK", "VRN", "VSA", "VST", "VTE", "VVO", "VXO", "VYD", "VYV", "VZN", 
    "WAW", "WDH", "WEI", "WEL", "WGA", "WHK", "WHM", "WIC", "WIE", "WLB", 
    "WLG", "WLS", "WMB", "WNS", "WOL", "WPK", "WRE", "WRG", "WRL", "WRO", 
    "WUH", "WUN", "WUX", "WVB", "WYA", "WYN", "WYS", "XCH", "XIY", "XLB", 
    "XLX", "XMH", "XMN", "XPK", "XRY", "XSI", "XXX", "YAK", "YAO", "YAT", 
    "YBE", "YCB", "YDF", "YEA", "YEV", "YFA", "YFB", "YFC", "YFO", "YGL", 
    "YGW", "YGX", "YHR", "YHZ", "YIF", "YIH", "YKA", "YKM", "YKS", "YLR", 
    "YLW", "YMM", "YMQ", "YNB", "YOK", "YOP", "YOW", "YPN", "YPR", "YQB", 
    "YQD", "YQG", "YQM", "YQR", "YQT", "YQX", "YRB", "YSJ", "YSM", "YSR", 
    "YTH", "YTO", "YUD", "YUM", "YUX", "YVB", "YVO", "YVP", "YVQ", "YVR", 
    "YWG", "YWK", "YXE", "YXJ", "YXN", "YXS", "YXT", "YXU", "YXY", "YYC", 
    "YYD", "YYJ", "YYQ", "YYR", "YYT", "YYZ", "YZF", "YZP", "ZAD", "ZAG", 
    "ZAZ", "ZBO", "ZCL", "ZIH", "ZKE", "ZLO", "ZND", "ZNE", "ZQN", "ZRH", 
    "ZSA", "ZSS", "ZTH", "ZTM", "ZYL"
};


const char * const City::m_codes5[NUMCITY] = { "NOCITY", 
    "ADALV", "AEAAN", "AEAUH", "AEDXB", "AEFJR", "AERKT", "AESHJ", "AFJAA", "AFKBL", "AFURZ", 
    "AGSJO", "AIVAL", "ALTIA", "AMEVN", "AOBUG", "AOCAB", "AOJMB", "AOLAD", "AOUGO", "ARBRC", 
    "ARBUE", "ARCNQ", "ARCOR", "ARIGR", "ARJNI", "ARJSM", "ARJUJ", "ARMDQ", "ARMDZ", "ARROS", 
    "ARRSA", "ARSFN", "ARSLA", "ARTUC", "ARUSH", "ASPPG", "ATGRZ", "ATINN", "ATKLU", "ATLNZ", 
    "ATSZG", "ATVIE", "AUABM", "AUABX", "AUADL", "AUALH", "AUASP", "AUAYQ", "AUAYR", "AUBDB", 
    "AUBHQ", "AUBLT", "AUBME", "AUBMP", "AUBNE", "AUBNK", "AUBWT", "AUCBR", "AUCED", "AUCES", 
    "AUCFS", "AUCMQ", "AUCNS", "AUCOM", "AUCPD", "AUCSI", "AUCTN", "AUCVQ", "AUCXT", "AUDBO", 
    "AUDBY", "AUDDI", "AUDKI", "AUDPO", "AUDRB", "AUDRW", "AUDYA", "AUEMD", "AUEPR", "AUGET", 
    "AUGEX", "AUGFF", "AUGKL", "AUGLT", "AUGOC", "AUGOO", "AUGOV", "AUGTE", "AUGYP", "AUHBA", 
    "AUHIS", "AUHLT", "AUHNK", "AUHTI", "AUHVB", "AUIFL", "AUIGH", "AUIRG", "AUISA", "AUJAD", 
    "AUJCK", "AUJHQ", "AUJUN", "AUKBY", "AUKCE", "AUKGC", "AUKGI", "AUKNS", "AUKNX", "AUKRB", 
    "AUKTA", "AUKTR", "AUKWM", "AULDC", "AULEA", "AULER", "AULNO", "AULRE", "AULST", "AULSY", 
    "AULVO", "AULZR", "AUMBH", "AUMCY", "AUMEL", "AUMGB", "AUMIM", "AUMKR", "AUMKY", "AUMMG", 
    "AUMMM", "AUMOV", "AUMQL", "AUMRZ", "AUMTL", "AUMYA", "AUNAA", "AUNRA", "AUNSA", "AUNSO", 
    "AUNTL", "AUNWM", "AUOAG", "AUOLP", "AUORS", "AUPBO", "AUPER", "AUPHE", "AUPLO", "AUPPP", 
    "AUPQQ", "AUPTJ", "AUPUG", "AUROK", "AUSIX", "AUSOI", "AUSYD", "AUTCA", "AUTEM", "AUTIS", 
    "AUTMW", "AUTPR", "AUTRO", "AUTSV", "AUTWB", "AUUEE", "AUUMR", "AUWAM", "AUWEI", "AUWGA", 
    "AUWMB", "AUWOL", "AUWSI", "AUWUN", "AUWYA", "AUWYN", "AUZBO", "AWORJ", "AXMHQ", "AZBAK", 
    "AZNAJ", "BABNX", "BAOMO", "BASJJ", "BBBGI", "BDBZL", "BDCGP", "BDDAC", "BDZYL", "BEANR", 
    "BEBRU", "BEGNE", "BELEU", "BELGG", "BFBOY", "BFOUA", "BGBOJ", "BGGOZ", "BGJAM", "BGRDU", 
    "BGSLS", "BGSOF", "BGTGV", "BGVAR", "BGVID", "BHAMH", "BIBJM", "BJCOO", "BLSBH", "BMBDA", 
    "BNBWN", "BOCBB", "BOLPB", "BOSCS", "BOSRB", "BQKRA", "BRAJU", "BRBEL", "BRBHZ", "BRBSB", 
    "BRBVB", "BRCBA", "BRCGR", "BRCWB", "BRFLN", "BRFOR", "BRGYN", "BRJCA", "BRJCB", "BRJDF", 
    "BRJLS", "BRJNA", "BRJOI", "BRJPA", "BRJTI", "BRMAO", "BRMCA", "BRMCZ", "BRMGF", "BRMON", 
    "BRNAT", "BRPMW", "BRPOA", "BRPVH", "BRRBR", "BRREC", "BRRIO", "BRSAO", "BRSLZ", "BRSRA", 
    "BRSSA", "BRTHE", "BRUBB", "BRUDI", "BRURG", "BRVIX", "BRXXX", "BSCCZ", "BSELH", "BSFPO", 
    "BSGHB", "BSMHH", "BSNAS", "BSRSD", "BSTCB", "BSZSA", "BTTHI", "BWFRW", "BWGBE", "BWJWA", 
    "BWMUB", "BWPKW", "BYMSQ", "BZBMP", "BZBZE", "CACAL", "CACHV", "CAEDM", "CAFAL", "CAFCM", 
    "CAFRE", "CAFSJ", "CAFSM", "CAGAN", "CAGOO", "CAHAB", "CAHAL", "CAIQL", "CAKML", "CAKWL", 
    "CALOD", "CALRA", "CAMIS", "CAMNT", "CAMTR", "CANVK", "CAOTT", "CAPME", "CAPRG", "CAPRR", 
    "CAQUE", "CARBL", "CAREB", "CAREG", "CASAK", "CASAU", "CASJB", "CASJF", "CASSP", "CATHU", 
    "CATOR", "CAVAN", "CAVIC", "CAWND", "CAWNP", "CAXPK", "CAXSI", "CAXXX", "CAYAT", "CAYBE", 
    "CAYCB", "CAYDF", "CAYEV", "CAYFO", "CAYGL", "CAYGW", "CAYGX", "CAYHR", "CAYMM", "CAYQD", 
    "CAYTH", "CAYUD", "CAYVB", "CAYVO", "CAYVQ", "CAYWK", "CAYXN", "CAYXT", "CAYXY", "CAYYD", 
    "CAYZF", "CAZKE", "CAZTM", "CCCCK", "CDFBM", "CDFIH", "CDFKI", "CDLIQ", "CFBBT", "CFBBY", 
    "CFBGF", "CFBGU", "CFBIV", "CFCRF", "CFIRO", "CFODA", "CGBZV", "CGPNR", "CHATR", "CHBRN", 
    "CHBSL", "CHGVA", "CHLUG", "CHLZN", "CHZRH", "CIABJ", "CIASK", "CIBYK", "CIDJO", "CIHGO", 
    "CIMJC", "CISPY", "CIZSS", "CKRAR", "CLCJC", "CLESR", "CLIPC", "CLPUQ", "CLSCL", "CLVAP", 
    "CMDLA", "CMGOU", "CMMVR", "CMNGE", "CMYAO", "CNAAT", "CNBJS", "CNCDU", "CNCGC", "CNCQI", 
    "CNCYN", "CNDAL", "CNGGZ", "CNGUI", "CNHAZ", "CNHLH", "CNHRN", "CNJDZ", "CNJGN", "CNJIL", 
    "CNJIN", "CNJIU", "CNJJI", "CNJMU", "CNJNA", "CNJNG", "CNJOG", "CNJZI", "CNLUM", "CNNHN", 
    "CNNIN", "CNQIN", "CNRZH", "CNSGH", "CNSHY", "CNSNZ", "CNTNJ", "CNTYU", "CNURM", "CNWXS", 
    "CNXAM", "CNXYA", "CNYIC", "CNZGZ", "COBAQ", "COBGA", "COBOG", "COCLO", "COCTG", "COMDE", 
    "COPEI", "COSAC", "COSSL", "CRSJO", "CRUPA", "CUCFG", "CUHAV", "CUHOG", "CUSCU", "CUVRA", 
    "CVRAI", "CVSID", "CWWIL", "CXFFC", "CYAKT", "CYLCA", "CYLMS", "CYNIC", "CYPFO", "CZPRG", 
    "CZUHE", "DEAGB", "DEBER", "DEBON", "DEBRE", "DEBYU", "DECGN", "DECOT", "DEDRS", "DEDTM", 
    "DEDUS", "DEERF", "DEFDH", "DEFRA", "DEGWT", "DEHAJ", "DEHAM", "DEHOQ", "DEJUI", "DEKAE", 
    "DEKEL", "DELEJ", "DEMUC", "DENUE", "DEOLO", "DEOSN", "DEPAD", "DEQGT", "DEREG", "DESCN", 
    "DESTR", "DESWR", "DEWIB", "DJJIB", "DKAAB", "DKAAL", "DKAAR", "DKBLL", "DKCPH", "DKEBJ", 
    "DKGRN", "DKHOR", "DKKRP", "DKODE", "DKRAN", "DKRNN", "DKSGD", "DKSKR", "DKSLB", "DKTED", 
    "DMDOM", "DMRSU", "DOAZS", "DOCDC", "DOPCN", "DOPOP", "DOSDQ", "DZAAE", "DZALG", "DZCZL", 
    "DZGJL", "DZORN", "ECGYE", "ECJIP", "ECSNC", "ECUIO", "EETLL", "EGAAC", "EGABS", "EGALY", 
    "EGASW", "EGATZ", "EGAUE", "EGCAI", "EGEMY", "EGHRG", "EGLXR", "EGMUH", "EGPSD", "EGRMF", 
    "EGSEW", "EGSKV", "EGSSH", "EGUVL", "EHEUN", "ERASM", "ESACE", "ESAGP", "ESALC", "ESBCN", 
    "ESBIO", "ESBJZ", "ESEAS", "ESFUE", "ESGRO", "ESIBZ", "ESJAE", "ESJRZ", "ESLCG", "ESLEI", 
    "ESLPG", "ESMAD", "ESMAH", "ESMJV", "ESODB", "ESOVO", "ESPMI", "ESRUS", "ESSBP", "ESSCQ", 
    "ESSDR", "ESSPC", "ESSVQ", "ESTCI", "ESVDH", "ESVGO", "ESVIT", "ESVLC", "ESVLL", "ESZAZ", 
    "ETADD", "ETBCO", "ETJIJ", "ETJIM", "FIENF", "FIES2", "FIHEL", "FIIVL", "FIJOE", "FIJYV", 
    "FIKAJ", "FIKAO", "FIKEM", "FIKHJ", "FIKOK", "FIKTT", "FIKUO", "FILPP", "FIMIK", "FIOUL", 
    "FIPOR", "FIRVN", "FISJY", "FISOT", "FISVL", "FITKU", "FITMP", "FIVAA", "FIVRK", "FJCST", 
    "FJNAN", "FJSUV", "FKPSY", "FMPNI", "FOTOR", "FRAJA", "FRAUR", "FRBBI", "FRBES", "FRBIA", 
    "FRBIQ", "FRBOD", "FRCEQ", "FRCFE", "FRCLY", "FRCMF", "FRDNR", "FREBU", "FREGC", "FRENC", 
    "FRFNI", "FRFRJ", "FRFSC", "FRGNB", "FRLAI", "FRLDE", "FRLIG", "FRLIO", "FRLLE", "FRLRH", 
    "FRLRT", "FRMLH", "FRMPL", "FRMRS", "FRMZM", "FRNCE", "FRNCY", "FRNTE", "FRPAR", "FRPGF", 
    "FRPIS", "FRPUF", "FRRDZ", "FRRNE", "FRRNS", "FRSBK", "FRSXB", "FRTLS", "FRUIP", "GALBQ", 
    "GALBV", "GAMFF", "GAMJL", "GAMVE", "GAOYE", "GAPOG", "GBABD", "GBAYL", "GBBBC", "GBBEL", 
    "GBBHM", "GBBLK", "GBBOH", "GBBRR", "GBBRS", "GBBSH", "GBCBT", "GBCDF", "GBCMG", "GBCVT", 
    "GBDUN", "GBEDI", "GBEXE", "GBFIE", "GBFOA", "GBGLW", "GBHUY", "GBINV", "GBISY", "GBKNM", 
    "GBKWL", "GBLBA", "GBLDY", "GBLEQ", "GBLER", "GBLIV", "GBLON", "GBLSI", "GBLYX", "GBMME", 
    "GBMNC", "GBNCS", "GBNQY", "GBNRW", "GBNTG", "GBOUK", "GBSHE", "GBSND", "GBSOU", "GBSTO", 
    "GBSWA", "GBUNT", "GBWCK", "GDGND", "GETBS", "GFCAY", "GGACI", "GGSPT", "GHACC", "GIGIB", 
    "GLGOH", "GLJUV", "GLSFJ", "GLUAK", "GLUMD", "GMBJL", "GNCKY", "GNLEK", "GPBBR", "GQSSG", 
    "GRAOK", "GRARX", "GRATH", "GRCFU", "GRCHQ", "GRHER", "GRJKH", "GRJMK", "GRJNX", "GRJSI", 
    "GRJTR", "GRKGS", "GRKLX", "GRKVA", "GRMJT", "GRPVK", "GRRHO", "GRSKG", "GRSMI", "GRZTH", 
    "GSXXX", "GTGUA", "GUAGA", "GUHGT", "GWOXB", "GYGEO", "HKHKG", "HNJUT", "HNRTB", "HNSAP", 
    "HNSDH", "HNTGU", "HNUII", "HRDBV", "HROSI", "HRPUY", "HRRJK", "HRSPU", "HRVLN", "HRVZN", 
    "HRZAD", "HRZAG", "HTJAK", "HTJEE", "HTPAP", "HUBUD", "IDAYW", "IDBDO", "IDDJB", "IDDJJ", 
    "IDDPS", "IDJKT", "IDMDC", "IDMES", "IDSUB", "IDUPG", "IECFN", "IEDUB", "IEGWY", "IEKIR", 
    "IELMK", "IENOC", "IEORK", "IESLI", "ILETH", "ILHFA", "ILTLV", "ILXXX", "IMDGS", "INAMD", 
    "INATQ", "INBBI", "INBHO", "INBLR", "INBOM", "INBRC", "INCCJ", "INCCU", "INCJB", "INCOK", 
    "INGAW", "INGDH", "INGOI", "INHYD", "INICD", "INIDR", "INIXB", "INIXC", "INIXG", "INIXJ", 
    "INIXR", "INIXW", "INJAI", "INJDH", "INJGA", "INJGB", "INJRH", "INJSA", "INJUC", "INKNU", 
    "INLKO", "INMAA", "INMYQ", "INNAG", "INPAT", "INPNQ", "INPYB", "INQNB", "INRAJ", "INSTV", 
    "INSXR", "INTRI", "INTRV", "INUDR", "INVNS", "INXXX", "IODGA", "IQBGW", "IQBSR", "IQKIK", 
    "IQOSM", "IRABD", "IRTHR", "IRUIM", "ISEGS", "ISREY", "ITAHO", "ITAOI", "ITBDS", "ITBGO", 
    "ITBIE", "ITBLQ", "ITBRC", "ITBRI", "ITCAG", "ITCTA", "ITEBA", "ITFLR", "ITGOA", "ITLMP", 
    "ITMIL", "ITNAP", "ITOLB", "ITPDA", "ITPEG", "ITPMO", "ITPNL", "ITPSA", "ITPSR", "ITREG", 
    "ITRMI", "ITROM", "ITSUF", "ITTPS", "ITTRN", "ITTRS", "ITTRV", "ITVCE", "ITVRN", "JESTH", 
    "JMKIN", "JMMBJ", "JOAMM", "JOAQJ", "JPAOJ", "JPASJ", "JPAXT", "JPCHB", "JPFKF", "JPFUK", 
    "JPGAJ", "JPHAC", "JPHIJ", "JPHKD", "JPISG", "JPKCZ", "JPKIJ", "JPKMI", "JPKMJ", "JPKMQ", 
    "JPKOJ", "JPKUH", "JPMMJ", "JPMMY", "JPMRW", "JPMYJ", "JPNGO", "JPNGS", "JPOIT", "JPOKA", 
    "JPOKJ", "JPOSA", "JPSDJ", "JPSHS", "JPSOS", "JPSPK", "JPTAK", "JPTKS", "JPTOY", "JPTYO", 
    "JPUBJ", "JPUKB", "JPUKY", "JPYOK", "KEMBA", "KEMYD", "KENBO", "KGFRU", "KHPNH", "KICXI", 
    "KITRW", "KMAJN", "KMYVA", "KNNEV", "KNSKB", "KPFNJ", "KRINC", "KRPUS", "KRSEL", "KRUSN", 
    "KWKWI", "KYGCM", "KZALA", "KZAST", "LAVTE", "LBBEY", "LCCAS", "LCVIF", "LIVDZ", "LKCMB", 
    "LKJAF", "LRMLW", "LSMSU", "LTVIL", "LUEZT", "LULUX", "LVRIX", "LYBEN", "LYSEB", "LYTIP", 
    "MAAGA", "MAAHU", "MACAS", "MAFEZ", "MAOUD", "MAOZZ", "MARAK", "MARBA", "MATNG", "MCMON", 
    "MDKIV", "METGD", "METIV", "MFMAR", "MGMJN", "MGTNR", "MHMAJ", "MHUIT", "MKOHD", "MKSKP", 
    "MLBKO", "MMMDL", "MMRGN", "MNULN", "MOMFM", "MPSPN", "MQFDF", "MRNDB", "MRNKC", "MROUZ", 
    "MSBRD", "MTMLA", "MUEBN", "MUPLU", "MURRG", "MVMLE", "MWBLZ", "MWLLW", "MX2NB", "MXACA", 
    "MXAGU", "MXCEN", "MXCJS", "MXCME", "MXCOL", "MXCUL", "MXCUN", "MXCUU", "MXCVM", "MXCZA", 
    "MXCZM", "MXEPC", "MXGDL", "MXHMO", "MXHTO", "MXJAL", "MXLAP", "MXLCB", "MXLEN", "MXLMM", 
    "MXLRO", "MXLZC", "MXMEX", "MXMID", "MXMLM", "MXMTT", "MXMTY", "MXMXL", "MXMZT", "MXNLD", 
    "MXOAX", "MXPBC", "MXPVR", "MXPXM", "MXSLP", "MXSRL", "MXTAM", "MXTGZ", "MXTIJ", "MXVER", 
    "MXVSA", "MXZCL", "MXZIH", "MXZLO", "MYBKI", "MYBTU", "MYJHB", "MYKCH", "MYKUA", "MYKUL", 
    "MYLBU", "MYLGK", "MYMYY", "MYPEN", "MYSBW", "MYTOD", "MYTWU", "MZBEW", "MZMPM", "NAKAT", 
    "NAKMP", "NALUD", "NANDU", "NAOKU", "NAOMD", "NAOND", "NASWP", "NATSB", "NAWDH", "NCILP", 
    "NCIOU", "NCLIF", "NCMEE", "NCNOU", "NCTOU", "NEAJY", "NEMFQ", "NENIM", "NERLT", "NEZND", 
    "NFNLK", "NGABV", "NGJOS", "NGKAN", "NGLOS", "NGPHC", "NIMGA", "NLAMS", "NLEIN", "NLGRQ", 
    "NLHAG", "NLHTB", "NLLEY", "NLMST", "NLRTM", "NLUDE", "NLUTC", "NOAES", "NOALF", "NOBDU", 
    "NOBGO", "NOBNN", "NOBOO", "NOEVE", "NOFRO", "NOHAU", "NOHFT", "NOKKN", "NOKRS", "NOKSU", 
    "NOLKL", "NOOSL", "NOSOG", "NOSVG", "NOTOS", "NOTRD", "NPJIR", "NPJKR", "NPJMO", "NPJUM", 
    "NPKTM", "NRYRN", "NUALO", "NZAKL", "NZBHE", "NZCHC", "NZDUD", "NZHLZ", "NZIVC", "NZMFN", 
    "NZMON", "NZNSN", "NZPMR", "NZROT", "NZTEU", "NZWHK", "NZWLG", "NZWRE", "NZZQN", "OMMCT", 
    "OMSLL", "PAJQE", "PAPAC", "PEIQT", "PEJAU", "PEJJI", "PEJUL", "PELIM", "PFBOB", "PFHUH", 
    "PFMAU", "PFMOZ", "PFPPT", "PFRFP", "PFRGI", "PFUAH", "PFUAP", "PFXMH", "PGAYU", "PGAZB", 
    "PGJAQ", "PGLAE", "PGMFO", "PGPOM", "PHCEB", "PHCYU", "PHJOL", "PHMKC", "PHMNL", "PHNOP", 
    "PHPSG", "PKBHV", "PKBNP", "PKCJL", "PKDSK", "PKGIL", "PKGWD", "PKHDD", "PKISB", "PKJAG", 
    "PKJIW", "PKKDD", "PKKDU", "PKKHI", "PKLHE", "PKLYP", "PKMFG", "PKMJD", "PKMPD", "PKMPR", 
    "PKMUX", "PKMWD", "PKOHT", "PKPEW", "PKPJG", "PKPSI", "PKPZH", "PKRAZ", "PKRWP", "PKRYK", 
    "PKSDT", "PKSKZ", "PKSUL", "PKTUK", "PKUET", "PKWNS", "PLGDN", "PLKRK", "PLPOZ", "PLSZZ", 
    "PLWAW", "PLWRO", "PMFSP", "PNPCN", "PRBQN", "PRMAZ", "PRPSE", "PRSJU", "PSGZA", "PSNAB", 
    "PTFAO", "PTFNC", "PTHOR", "PTLIS", "PTOPO", "PTPDL", "PTPXO", "PTSMA", "PTTER", "PWROR", 
    "PYASU", "QADOH", "RERUN", "ROBUH", "ROCLJ", "ROCND", "ROSBZ", "RSBEG", "RSINI", "RSNVS", 
    "RUAER", "RUARH", "RUGOJ", "RUHTA", "RUIKT", "RUKGD", "RUKHV", "RUKUF", "RUKZN", "RULED", 
    "RUMMK", "RUMOW", "RUMRV", "RUOVB", "RUROV", "RUSKX", "RUUCT", "RUUFA", "RUUUD", "RUVLU", 
    "RUVOG", "RUVVO", "RUYEK", "RUYKS", "RWKGL", "SAAJF", "SADHA", "SADMM", "SAJED", "SAKMX", 
    "SAMED", "SARUH", "SATIF", "SATUU", "SAYNB", "SBGSI", "SBHIR", "SCVIC", "SDKRT", "SDKSL", 
    "SEGOT", "SEHEL", "SEJKG", "SEKID", "SEKLR", "SEKRN", "SEKSD", "SELDK", "SELLA", "SEMMA", 
    "SENRK", "SEORB", "SERNB", "SESDL", "SESTO", "SEUME", "SEVBY", "SEVST", "SEVXO", "SGSIN", 
    "SHSHN", "SILJU", "SIMBX", "SJLYR", "SKBTS", "SKILZ", "SLFNA", "SMSAI", "SNDKR", "SOMGQ", 
    "SRPBM", "SSJUB", "STTMS", "SVSAL", "SXPHI", "SYALP", "SYDAM", "SZMTS", "SZQMN", "TCGDT", 
    "TDAEH", "TDMQQ", "TDNDJ", "TFPFR", "TGLFW", "THBKK", "THCNX", "THHDY", "THHKT", "THNKT", 
    "THPYX", "THUBP", "THUTH", "THUTP", "TJDYU", "TKNKU", "TLDIL", "TMAGT", "TNDJE", "TNMIR", 
    "TNSFA", "TNTUN", "TOTBU", "TRADA", "TRADB", "TRADI", "TRANK", "TRASR", "TRAYT", "TRBXN", 
    "TRDLM", "TRDNZ", "TRERC", "TRERZ", "TRGZT", "TRIST", "TRKCM", "TRKSY", "TRKYA", "TRMDN", 
    "TRMLX", "TRMSR", "TRSSX", "TRTEK", "TRTZX", "TRVAN", "TRVAS", "TTPOS", "TTSCR", "TVFUN", 
    "TWKHH", "TWMZG", "TWTPE", "TZARK", "TZDAR", "TZDOD", "TZJRO", "UADNK", "UAHRK", "UAIEV", 
    "UALWO", "UANLV", "UAODS", "UASIP", "UAUZH", "UGEBB", "UGJIN", "UGKLA", "UGULU", "UMJON", 
    "UMMDY", "US5SV", "USABI", "USABQ", "USABR", "USABY", "USACK", "USACT", "USADQ", "USAEV", 
    "USAFL", "USAGS", "USAHN", "USAIY", "USAKH", "USAKN", "USALB", "USALO", "USALW", "USAMA", 
    "USANB", "USANC", "USANP", "USAOO", "USAPF", "USARB", "USASE", "USATL", "USATO", "USATW", 
    "USATY", "USAUG", "USAUS", "USAUW", "USAWN", "USAZO", "USB2L", "USBAL", "USBCT", "USBDR", 
    "USBEH", "USBET", "USBFD", "USBFL", "USBGR", "USBHM", "USBIL", "USBIS", "USBJI", "USBKW", 
    "USBKX", "USBLI", "USBMG", "USBNA", "USBOI", "USBOS", "USBPT", "USBRD", "USBRL", "USBTM", 
    "USBTR", "USBTV", "USBUF", "USBUR", "USBXS", "USBZN", "USCAE", "USCCR", "USCDC", "USCDV", 
    "USCEC", "USCGA", "USCHA", "USCHI", "USCHO", "USCHS", "USCIC", "USCID", "USCKB", "USCLD", 
    "USCLE", "USCLL", "USCLM", "USCLT", "USCMH", "USCMI", "USCMX", "USCOD", "USCON", "USCOS", 
    "USCPR", "USCRP", "USCRW", "USCSG", "USCSL", "USCSN", "USCVG", "USCYS", "USDAB", "USDAL", 
    "USDAN", "USDAY", "USDBQ", "USDEC", "USDEN", "USDET", "USDHN", "USDLG", "USDLH", "USDRO", 
    "USDSM", "USDUJ", "USDUT", "USDVL", "USEAT", "USEAU", "USEKA", "USEKI", "USEKO", "USELM", 
    "USELP", "USELY", "USENA", "USERI", "USESC", "USESF", "USEUG", "USEVV", "USEWN", "USEYW", 
    "USFAI", "USFAR", "USFAT", "USFBG", "USFCA", "USFCI", "USFFT", "USFHU", "USFKL", "USFLG", 
    "USFLL", "USFLO", "USFMN", "USFMY", "USFNT", "USFOD", "USFRI", "USFSD", "USFSM", "USFWA", 
    "USFWB", "USFYV", "USGAD", "USGBO", "USGCC", "USGCN", "USGDV", "USGEG", "USGFK", "USGJT", 
    "USGLH", "USGNV", "USGPT", "USGPZ", "USGRB", "USGRH", "USGRR", "USGSG", "USGTF", "USGUC", 
    "USGV9", "USHAR", "USHFD", "USHHH", "USHII", "USHKY", "USHLN", "USHNH", "USHNL", "USHNS", 
    "USHNU", "USHOM", "USHON", "USHOU", "USHRL", "USHSV", "USHUF", "USHVN", "USHVR", "USHYA", 
    "USHYG", "USIAG", "USIBB", "USICT", "USIDA", "USIDO", "USIFP", "USIJX", "USILE", "USILG", 
    "USILI", "USILM", "USIND", "USINL", "USIPT", "USISN", "USITH", "USITO", "USIYK", "USJAC", 
    "USJAN", "USJAX", "USJBR", "USJEC", "USJEF", "USJHM", "USJHW", "USJKV", "USJLN", "USJMS", 
    "USJNU", "USJOY", "USJST", "USJVL", "USJXN", "USK5T", "USKCC", "USKHH", "USKLW", "USKTB", 
    "USKTN", "USKUA", "USLAF", "USLAN", "USLAR", "USLAS", "USLAW", "USLAX", "USLBB", "USLBE", 
    "USLCH", "USLEX", "USLFT", "USLGB", "USLIJ", "USLIT", "USLIU", "USLMT", "USLN7", "USLNK", 
    "USLNS", "USLNY", "USLRD", "USLRF", "USLSE", "USLTS", "USLUI", "USLUL", "USLWB", "USLWS", 
    "USLWT", "USLYH", "USLYO", "USMAF", "USMCE", "USMCL", "USMCN", "USMCW", "USMEI", "USMEM", 
    "USMES", "USMFR", "USMGM", "USMGW", "USMHE", "USMHT", "USMIA", "USMJQ", "USMKC", "USMKE", 
    "USMKG", "USMKL", "USMLB", "USMLI", "USMLS", "USMLU", "USMOB", "USMOD", "USMOT", "USMQT", 
    "USMRB", "USMSN", "USMSO", "USMSY", "USMTH", "USMTJ", "USMTM", "USMTO", "USMUE", "USMVY", 
    "USMWH", "USMY3", "USMYR", "USNNS", "USNYC", "USOAJ", "USOAK", "USOGG", "USOKC", "USOLF", 
    "USOLM", "USOMA", "USOME", "USONT", "USORF", "USORH", "USORL", "USOSH", "USOTH", "USOTZ", 
    "USOUI", "USOWB", "USOXN", "USPAH", "USPBI", "USPCT", "USPDT", "USPDX", "USPFN", "USPGA", 
    "USPGV", "USPHL", "USPHX", "USPIA", "USPIH", "USPIR", "USPIT", "USPKB", "USPLB", "USPNS", 
    "USPOU", "USPQI", "USPSC", "USPSG", "USPSP", "USPUB", "USPUW", "USPVD", "USPWM", "USPXN", 
    "USQPL", "USQTR", "USRAG", "USRAP", "USRDD", "USRDG", "USRFD", "USRHI", "USRIC", "USRKD", 
    "USRKS", "USRMD", "USRNO", "USROA", "USROC", "USRST", "USRWI", "USSAC", "USSAF", "USSAN", 
    "USSAT", "USSAV", "USSBA", "USSBN", "USSBS", "USSBY", "USSCC", "USSCE", "USSCK", "USSDY", 
    "USSEA", "USSFO", "USSGF", "USSGM", "USSGU", "USSGY", "USSHR", "USSHV", "USSIT", "USSJC", 
    "USSJT", "USSLC", "USSLE", "USSMX", "USSNA", "USSNS", "USSPI", "USSPS", "USSSI", "USSTL", 
    "USSTP", "USSTS", "USSTZ", "USSUX", "USSWF", "USSYR", "USSZH", "USTCL", "USTKA", "USTLH", 
    "USTOL", "USTOP", "USTPA", "USTTN", "USTUL", "USTUP", "USTUZ", "USTVC", "USTVF", "USTVL", 
    "USTWF", "USTXC", "USTXK", "USTYR", "USTYS", "USUCA", "USUIN", "USUKI", "USUNK", "USUPP", 
    "USUSH", "USVAC", "USVDZ", "USVEL", "USVIS", "USVIV", "USVLD", "USVRB", "USVYV", "USWAS", 
    "USWBE", "USWLB", "USWPK", "USWRG", "USWRL", "USWTS", "USWYS", "USXEZ", "USXHO", "USXLX", 
    "USYAK", "USYKM", "USYUM", "USZGO", "USZLV", "USZOR", "UYMVD", "UZSKD", "UZTAS", "UZTER", 
    "UZUGC", "VAVAT", "VCCAN", "VCKTN", "VCUNI", "VEBLA", "VECCS", "VECGU", "VEMAR", "VEMGT", 
    "VEPZO", "VEURM", "VEVLN", "VGEIS", "VGTOV", "VGVIJ", "VISTT", "VISTX", "VNHAN", "VNHUI", 
    "VNSGN", "VUDLY", "VUSAN", "VUULB", "VUVLI", "WFFUT", "WFMAU", "WSAPW", "XXPRN", "XXXXX", 
    "YEADE", "YESAH", "YTMAM", "ZAADY", "ZAAGZ", "ZAALJ", "ZABFN", "ZACPT", "ZADUR", "ZAELL", 
    "ZAELS", "ZAGRJ", "ZAHLA", "ZAJNB", "ZAKIM", "ZAKLZ", "ZALUJ", "ZAMBM", "ZAMEZ", "ZAMGH", 
    "ZAMZF", "ZAMZY", "ZANCS", "ZANLP", "ZAOUH", "ZAPBZ", "ZAPHW", "ZAPIB", "ZAPLZ", "ZAPOL", 
    "ZAPRY", "ZAPZB", "ZARCB", "ZASBU", "ZASIS", "ZASZK", "ZATCU", "ZAULD", "ZAUTN", "ZAUTT", 
    "ZAVYD", "ZAWEL", "ZAWVB", "ZMCIP", "ZMKIW", "ZMLUN", "ZMMFU", "ZMNLA", "ZWBFO", "ZWBUQ", 
    "ZWGWE", "ZWHRE", "ZWHWN", "ZWMVZ", "ZWVFA"
};


const char * const City::m_codes5Print[NUMCITY] = { "NOCITY", 
    "DKAAB", "EGAAC", "DZAAE", "DKAAL", "AEAAN", "DKAAR", "CNAAT", "IRABD", "USAWN", "USABI", 
    "CIABJ", "AUABM", "USABQ", "USABR", "EGABS", "NGABV", "AUABX", "USALB", "GBABD", "MXACA", 
    "GHACC", "ESACE", "CHATR", "GGACI", "USACK", "USACT", "USEKA", "USAIY", "TRADA", "ETADD", 
    "YEADE", "TRADI", "AUADL", "USADQ", "ZAADY", "COSAC", "TDAEH", "RUAER", "NOAES", "MAAGA", 
    "DEAGB", "ESAGP", "USAGS", "MXAGU", "ZAAGZ", "SAKMX", "USAHN", "ITAHO", "MAAHU", "FRAJA", 
    "SAAJF", "KMAJN", "BRAJU", "NEAJY", "NZAKL", "USAKN", "CYAKT", "KZALA", "USABY", "ESALC", 
    "NOALF", "DZALG", "AUALH", "ZAALJ", "USALO", "SYALP", "ADALV", "USALW", "EGALY", "USAMA", 
    "INAMD", "JOAMM", "NLAMS", "USANB", "USANC", "TRANK", "USANP", "BEANR", "AGSJO", "ITAOI", 
    "JPAOJ", "GRAOK", "USAOO", "USAPF", "WSAPW", "JOAQJ", "USARB", "RUARH", "TZARK", "TMAGT", 
    "USASE", "SHSHN", "JPASJ", "CIASK", "ERASM", "AUASP", "TRASR", "PYASU", "EGASW", "GRATH", 
    "USATL", "USATO", "INATQ", "USATW", "USATY", "EGATZ", "AWORJ", "EGAUE", "USAUG", "AEAUH", 
    "FRAUR", "USAUS", "USAEV", "USWBE", "AIVAL", "USLTS", "JPAXT", "GBAYL", "AUAYQ", "AUAYR", 
    "TRAYT", "PGAYU", "IDAYW", "PGAZB", "USAZO", "DOAZS", "BHAMH", "AZBAK", "COBAQ", "INBBI", 
    "GPBBR", "CFBBT", "ROBUH", "CFBBY", "ESBCN", "ETBCO", "USBCT", "BZBMP", "BMBDA", "AUBDB", 
    "USHFD", "IDBDO", "INBRC", "USBDR", "ITBDS", "NOBDU", "GBBBC", "RSBEG", "USBEH", "BRBEL", 
    "LYBEN", "DEBER", "FRBES", "USBET", "MZBEW", "LBBEY", "USBFD", "USBFL", "ZABFN", "ZWBFO", 
    "GBBEL", "COBGA", "CFBGF", "BBBGI", "USJOY", "NOBGO", "USBGR", "CFBGU", "IQBGW", "ITBGO", 
    "USIFP", "NZBHE", "USBHM", "INBHO", "AUBHQ", "PKBHV", "GBBHM", "FRBIA", "USBIL", "ESBIO", 
    "FRBIQ", "USBIS", "CFBIV", "USBJI", "GMBJL", "BIBJM", "CNBJS", "TRBXN", "MXLEN", "ESBJZ", 
    "MYBKI", "THBKK", "MLBKO", "USBKW", "USBKX", "VEBLA", "USAFL", "USBLI", "GBBLK", "DKBLL", 
    "ITBLQ", "INBLR", "AUBLT", "MWBLZ", "AUBME", "USBMG", "USB2L", "AUBMP", "USBNA", "AUBNE", 
    "DEBON", "AUBNK", "NOBNN", "PKBNP", "BABNX", "PFBOB", "FRBOD", "COBOG", "GBBOH", "USBOI", 
    "BGBOJ", "INBOM", "BQKRA", "NOBOO", "USBOS", "BFBOY", "USBPT", "USSSI", "PRBQN", "ARBRC", 
    "USBRD", "DEBRE", "ITBRI", "USBRL", "CHBRN", "GBBRR", "GBBRS", "BEBRU", "BRBSB", "CHBSL", 
    "IQBSR", "USBTM", "USBTR", "SKBTS", "MYBTU", "USBTV", "HUBUD", "ARBUE", "USBUF", "AOBUG", 
    "ZWBUQ", "USBUR", "BRBVB", "USBAL", "BNBWN", "AUBWT", "USBXS", "CIBYK", "DEBYU", "BZBZE", 
    "BDBZL", "USBZN", "CGBZV", "AOCAB", "USCAE", "ITCAG", "EGCAI", "USAKH", "GBCBT", "CNGGZ", 
    "MACAS", "GFCAY", "BOCBB", "GBCMG", "AUCBR", "DECOT", "INCCJ", "CCCCK", "USCCR", "VECCS", 
    "INCCU", "BSCCZ", "USCDC", "USCDV", "PHCEB", "USCEC", "AUCED", "MXCEN", "FRCEQ", "AUCES", 
    "FRCFE", "CUCFG", "IECFN", "AUCFS", "GRCFU", "USCGA", "BRCBA", "DECGN", "CNZGZ", "BDCGP", 
    "CNCGC", "BRCGR", "VECGU", "USCHA", "NZCHC", "CNCYN", "USCHI", "USCHO", "GRCHQ", "USCHS", 
    "USCIC", "USCID", "ZMCIP", "VCCAN", "INCJB", "CLCJC", "PKCJL", "MXCJS", "USCKB", "CNCQI", 
    "GNCKY", "USCLD", "USCLE", "ROCLJ", "USCLL", "USCLM", "COCLO", "MXCOL", "USCLT", "FRCLY", 
    "LKCMB", "MXCME", "FRCMF", "USCMH", "USCMI", "AUCMQ", "USCMX", "ROCND", "BRBHZ", "ARCNQ", 
    "AUCNS", "THCNX", "USCOD", "INCOK", "USCON", "BJCOO", "ARCOR", "USCOS", "AUCPD", "DKCPH", 
    "USCPR", "ZACPT", "CFCRF", "USJAX", "USCRP", "USCRW", "USCSG", "AUCSI", "USCSN", "FJCST", 
    "ITCTA", "COCTG", "AUCTN", "CNCDU", "MXCUL", "MXCUN", "CWWIL", "MXCUU", "USCVG", "MXCVM", 
    "AUCVQ", "GBCVT", "USAUW", "BRCWB", "GBCDF", "KICXI", "AUCXT", "USCYS", "PHCYU", "MXCZA", 
    "DZCZL", "MXCZM", "USDAB", "BDDAC", "USDAL", "SYDAM", "USDAN", "TZDAR", "USDAY", "AUDBO", 
    "USDBQ", "HRDBV", "AUDBY", "USWAS", "DMRSU", "AUDDI", "USDEC", "INICD", "USDEN", "USDET", 
    "SADHA", "USDHN", "TLDIL", "IDDJB", "TNDJE", "IDDJJ", "CIDJO", "AUDKI", "SNDKR", "CMDLA", 
    "CNDAL", "USDLG", "USDLH", "TRDLM", "VUDLY", "SADMM", "GBDUN", "UADNK", "FRDNR", "TRDNZ", 
    "TZDOD", "QADOH", "DMDOM", "USXEZ", "AUDPO", "IDDPS", "AUDRB", "USDRO", "DEDRS", "AUDRW", 
    "PKDSK", "USDSM", "DEDTM", "IEDUB", "NZDUD", "USDUJ", "ZADUR", "DEDUS", "USDUT", "USDVL", 
    "AEDXB", "AUDYA", "TJDYU", "YTMAM", "ESEAS", "USEAT", "USEAU", "ITEBA", "UGEBB", "DKEBJ", 
    "MUEBN", "FREBU", "GBEDI", "FREGC", "USVAC", "ISEGS", "NLEIN", "VGEIS", "USEKI", "USEKO", 
    "BSELH", "ZAELL", "USELM", "USELP", "ZAELS", "USELY", "GBNTG", "AUEMD", "EGEMY", "USENA", 
    "FRENC", "FIENF", "MXEPC", "AUEPR", "TRERC", "DEERF", "USERI", "TRERZ", "FIES2", "USESC", 
    "USESF", "GBBSH", "CLESR", "ILETH", "USEUG", "EHEUN", "NOEVE", "AMEVN", "USEVV", "USEWN", 
    "USJEC", "GBEXE", "USEYW", "LUEZT", "FOTOR", "USFAI", "PTFAO", "USFAR", "USFAT", "USFBG", 
    "CDFBM", "USFCA", "USFCI", "MQFDF", "DEFDH", "MAFEZ", "USFFT", "USFHU", "GBFIE", "CDFIH", 
    "AEFJR", "DEKAE", "CDFKI", "USFKL", "JPFKF", "USFLG", "USFLL", "BRFLN", "USFLO", "ITFLR", 
    "USFMN", "DEOSN", "USFMY", "SLFNA", "PTFNC", "FRFNI", "KPFNJ", "USFNT", "USFOD", "USTOP", 
    "BRFOR", "GBFOA", "BSFPO", "DEFRA", "USFRI", "FRFRJ", "NOFRO", "KGFRU", "BWFRW", "FRFSC", 
    "USFSD", "USFSM", "PMFSP", "ESFUE", "JPFUK", "TVFUN", "WFFUT", "USFWA", "USFYV", "USGAD", 
    "JPGAJ", "INGAW", "BWGBE", "INXXX", "USGCC", "GGSPT", "KYGCM", "USGCN", "INGDH", "MXGDL", 
    "PLGDN", "TCGDT", "USGDV", "USGEG", "GYGEO", "AUGET", "AUGEX", "AUGFF", "USGFK", "USZLV", 
    "USGSG", "BSGHB", "GIGIB", "PKGIL", "DZGJL", "USGJT", "AUGKL", "GBGLW", "USGLH", "AUGLT", 
    "FRGNB", "GDGND", "BEGNE", "USGNV", "ITGOA", "GLGOH", "INGOI", "RUGOJ", "USZGO", "AUGOO", 
    "SEGOT", "CMGOU", "AUGOV", "BGGOZ", "GRARX", "USGPT", "USGPZ", "USGRB", "USGRH", "ZAGRJ", 
    "DKGRN", "ESGRO", "NLGRQ", "USGRR", "ESJAE", "ATGRZ", "GSXXX", "SBGSI", "USGBO", "USGV9", 
    "AUGTE", "USGTF", "DEQGT", "NZMON", "GTGUA", "USGUC", "GUHGT", "CHGVA", "PKGWD", "ZWGWE", 
    "DEGWT", "IEGWY", "ECGYE", "BRGYN", "AUGYP", "PSGZA", "TRGZT", "JPHAC", "NLHAG", "KMYVA", 
    "DEHAJ", "DEHAM", "VNHAN", "AUWSI", "USHAR", "NOHAU", "CUHAV", "AUHBA", "PKHDD", "USSBS", 
    "THHDY", "FIHEL", "GRHER", "ILHFA", "NOHFT", "CNHAZ", "CIHGO", "USHHH", "USIBB", "USHII", 
    "JPHIJ", "SBHIR", "AUHIS", "JPHKD", "HKHKG", "THHKT", "USHKY", "ZAHLA", "CNHLH", "USHLN", 
    "AUHLT", "NZHLZ", "MXHMO", "JPMRW", "USHNH", "AUHNK", "USHNL", "USHNS", "CUHOG", "USHOM", 
    "USHON", "DEHOQ", "PTHOR", "USHOU", "USWTS", "CNHRN", "ZWHRE", "EGHRG", "UAHRK", "USHRL", 
    "DKHOR", "USHSV", "RUHTA", "NLHTB", "AUHTI", "USHNU", "USHUF", "PFHUH", "VNHUI", "MXHTO", 
    "GBHUY", "AUHVB", "USHVN", "USHVR", "ZWHWN", "USHYA", "INHYD", "USHYG", "USIAG", "ESIBZ", 
    "KRINC", "USICT", "USIDA", "INIDR", "UAIEV", "AUIFL", "AUIGH", "ARIGR", "USIJX", "RUIKT", 
    "USILE", "USILG", "USILI", "USILM", "NCILP", "GBISY", "SKILZ", "USIND", "RSINI", "USINL", 
    "ATINN", "NRYRN", "GBINV", "IMDGS", "NCIOU", "CLIPC", "USIDO", "USIPT", "PEIQT", "AUIRG", 
    "CFIRO", "AUISA", "PKISB", "JPISG", "USISN", "USK5T", "USXHO", "TRIST", "USITH", "USITO", 
    "NUALO", "NZIVC", "FIIVL", "INIXB", "INIXC", "INIXG", "INIXJ", "INIXR", "INIXW", "USIYK", 
    "TRADB", "AFJAA", "USJAC", "AUJAD", "LKJAF", "PKJAG", "INJAI", "HTJAK", "MXJAL", "BGJAM", 
    "USJAN", "PGJAQ", "PEJAU", "USJBR", "BRJCB", "AUJCK", "BRJCA", "BRJDF", "INJDH", "CNJDZ", 
    "SAJED", "HTJEE", "USJEF", "JESTH", "INJGA", "INJGB", "CNJGN", "CNJIN", "MYJHB", "SEHEL", 
    "CNJOG", "USJHM", "AUJHQ", "USJHW", "DJJIB", "ETJIJ", "CNJIL", "ETJIM", "UGJIN", "ECJIP", 
    "NPJIR", "CNJIU", "PKJIW", "PEJJI", "CNJJI", "SEJKG", "GRJKH", "NPJKR", "IDJKT", "USJKV", 
    "USJLN", "INJUC", "BRJLS", "AOJMB", "GRJMK", "NPJMO", "USJMS", "CNJMU", "BRJNA", "ZAJNB", 
    "CNJNG", "ARJNI", "USJNU", "GRJNX", "CNJZI", "FIJOE", "BRJOI", "PHJOL", "UMJON", "NGJOS", 
    "BRJPA", "PAJQE", "INJRH", "TZJRO", "ILXXX", "INJSA", "GRJSI", "ARJSM", "USJST", "BRJTI", 
    "GRJTR", "SSJUB", "DEJUI", "ARJUJ", "PEJUL", "NPJUM", "AUJUN", "HNJUT", "GLJUV", "USJVL", 
    "BWJWA", "USJXN", "FIJYV", "FIKAJ", "NGKAN", "FIKAO", "AFKBL", "AUKBY", "USKCC", "AUKCE", 
    "MYKCH", "TRKCM", "JPKCZ", "PKKDD", "PKKDU", "DEKEL", "FIKEM", "AUKGC", "RUKGD", "AUKGI", 
    "RWKGL", "GRKGS", "TWKHH", "PKKHI", "FIKHJ", "RUKHV", "SEKID", "JPKIJ", "IQKIK", "ZAKIM", 
    "JMKIN", "IEKIR", "MDKIV", "ZMKIW", "NOKKN", "UGKLA", "SEKLR", "ATKLU", "USKLW", "GRKLX", 
    "ZAKLZ", "JPKMI", "JPKMJ", "NAKMP", "JPKMQ", "AUKNS", "INKNU", "AUKNX", "USKHH", "GBKWL", 
    "JPKOJ", "FIKOK", "AUKRB", "PLKRK", "SEKRN", "DKKRP", "NOKRS", "SDKRT", "SEKSD", "SDKSL", 
    "NOKSU", "AUKTA", "USKTB", "NPKTM", "USKTN", "AUKTR", "FIKTT", "MYKUA", "RUKUF", "JPKUH", 
    "MYKUL", "FIKUO", "GRKVA", "KWKWI", "CNGUI", "AUKWM", "TRKYA", "TRKSY", "RUKZN", "AOLAD", 
    "PGLAE", "USLAF", "FRLAI", "USLAN", "MXLAP", "USLAR", "USLAS", "USLAW", "USLAX", "GBLBA", 
    "USLBB", "USLBE", "FRBBI", "GALBQ", "MYLBU", "GALBV", "CYLCA", "ESLCG", "USLCH", "AULDC", 
    "FRLDE", "SELDK", "GBLDY", "AULEA", "USLN7", "RULED", "ESLEI", "DELEJ", "GNLEK", "GBLEQ", 
    "AULER", "BELEU", "USLEX", "NLLEY", "USLFT", "TGLFW", "USLGB", "BELGG", "MYLGK", "PKLHE", 
    "LIVDZ", "NCLIF", "FRLIG", "USLIU", "USLIJ", "FRLLE", "PELIM", "CDLIQ", "PTLIS", "USLIT", 
    "SILJU", "NOLKL", "INLKO", "SELLA", "MWLLW", "MXLMM", "ITLMP", "USLMT", "USLNK", "AULNO", 
    "USLNS", "USLNY", "ATLNZ", "GBLON", "NGLOS", "ESLPG", "BOLPB", "GBLIV", "FILPP", "USLRD", 
    "AULRE", "USLRF", "FRLRH", "DOCDC", "FRLRT", "USLSE", "GBLSI", "AULST", "AULSY", "HRVLN", 
    "MXLRO", "NALUD", "CHLUG", "ZALUJ", "CNLUM", "ZMLUN", "LULUX", "AULVO", "USLWB", "GBLER", 
    "UALWO", "USLWS", "USLWT", "EGLXR", "USLYH", "USLYO", "PKLYP", "SJLYR", "FRLIO", "GBLYX", 
    "MXLZC", "CHLZN", "AULZR", "INMAA", "ESMAD", "USMAF", "ESMAH", "MHMAJ", "GBMNC", "BRMAO", 
    "VEMAR", "PFMAU", "PRMAZ", "KEMBA", "AUMBH", "JMMBJ", "ZAMBM", "USSGM", "SIMBX", "USMCE", 
    "USMKC", "USMCL", "MCMON", "USMCN", "USORL", "BRMCA", "OMMCT", "USMCW", "AUMCY", "BRMCZ", 
    "IDMDC", "COMDE", "MMMDL", "ARMDQ", "UMMDY", "ARMDZ", "SAMED", "NCMEE", "USMEI", "AUMEL", 
    "USMEM", "IDMES", "MXMEX", "ZAMEZ", "USTXC", "GAMFF", "PKMFG", "MOMFM", "NZMFN", "PGMFO", 
    "NEMFQ", "USMFR", "ZMMFU", "NIMGA", "AUMGB", "BRMGF", "ZAMGH", "USMGM", "SOMGQ", "USMGW", 
    "USMHE", "BSMHH", "AXMHQ", "USMHT", "USMIA", "MXMID", "FIMIK", "ITMIL", "AUMIM", "TNMIR", 
    "CIMJC", "PKMJD", "GAMJL", "MGMJN", "USMJQ", "GRMJT", "ESMJV", "PHMKC", "USMKE", "USMKG", 
    "USKUA", "USMKL", "AUMKR", "AUMKY", "MTMLA", "USMLB", "MVMLE", "FRMLH", "USMLI", "MXMLM", 
    "USMLS", "USMLU", "LRMLW", "TRMLX", "SEMMA", "GBMME", "AUMMG", "JPMMJ", "RUMMK", "AUMMM", 
    "JPMMY", "MSBRD", "PHMNL", "USMOB", "USMOD", "USMOT", "AUMOV", "RUMOW", "PFMOZ", "NAKAT", 
    "PKMPD", "FRMPL", "MZMPM", "AUMQL", "TRMDN", "TDMQQ", "USMQT", "USMRB", "FRMRS", "MUPLU", 
    "RUMRV", "USMY3", "AUMRZ", "GBKNM", "USUSH", "USMSN", "USMSO", "USMES", "BYMSQ", "TRMSR", 
    "NLMST", "LSMSU", "USMSY", "USMTH", "USMTJ", "AUMTL", "USMTM", "USMTO", "SZMTS", "MXMTT", 
    "MXMTY", "BWMUB", "DEMUC", "USMUE", "EGMUH", "PKMUX", "GAMVE", "UYMVD", "CMMVR", "USMVY", 
    "ZWMVZ", "PKMWD", "USMWH", "MXMXL", "AUMYA", "KEMYD", "JPMYJ", "INMYQ", "USMYR", "MYMYY", 
    "ZAMZF", "TWMZG", "FRMZM", "MXMZT", "ZAMZY", "AUNAA", "INNAG", "AZNAJ", "FJNAN", "ITNAP", 
    "BSNAS", "BRNAT", "KENBO", "FRNCE", "GBNCS", "ZANCS", "FRNCY", "MRNDB", "TDNDJ", "NANDU", 
    "KNNEV", "CMNGE", "JPNGO", "JPNGS", "CYNIC", "NENIM", "MRNKC", "TKNKU", "IODGA", "ZMNLA", 
    "MXNLD", "NFNLK", "ZANLP", "UANLV", "CNNIN", "IENOC", "PHNOP", "NCNOU", "GBNQY", "AUNRA", 
    "SENRK", "AUNSA", "NZNSN", "AUNSO", "THNKT", "FRNTE", "AUNTL", "ZAPIB", "DENUE", "GBNRW", 
    "USNYC", "AUOAG", "USOAJ", "USOAK", "MXOAX", "CFODA", "ESODB", "DKODE", "UAODS", "USOGG", 
    "MKOHD", "PKOHT", "JPOIT", "JPOKA", "USOKC", "JPSPK", "JPOKJ", "NAOKU", "ITOLB", "USOLF", 
    "USOLM", "DEOLO", "AUOLP", "USOMA", "NAOMD", "USOME", "IRUIM", "BAOMO", "NAOND", "USONT", 
    "AUGOC", "AUCOM", "PTOPO", "SEORB", "USORF", "USORH", "IEORK", "DZORN", "AUORS", "JPOSA", 
    "USOSH", "HROSI", "NOOSL", "IQOSM", "USOTH", "USOTZ", "BFOUA", "MAOUD", "ZAOUH", "GBOUK", 
    "FIOUL", "MROUZ", "RUOVB", "ESOVO", "USOWB", "GWOXB", "USOXN", "MAOZZ", "PAPAC", "DEPAD", 
    "USPAH", "HTPAP", "FRPAR", "INPAT", "MXPBC", "BTTHI", "USPBI", "SRPBM", "AUPBO", "ZAPBZ", 
    "PHPSG", "USPCT", "PTPDL", "USPDT", "USPDX", "ITPEG", "COPEI", "MYPEN", "AUPER", "PKPEW", 
    "USPFN", "CYPFO", "TFPFR", "USPGA", "FRPGF", "USPGV", "NGPHC", "AUPHE", "USNNS", "USPHL", 
    "ZAPHW", "USPHX", "USPIA", "USLUL", "USPIH", "USPIR", "FRPIS", "USPIT", "PKPJG", "USPKB", 
    "BWPKW", "USPLB", "USPXN", "AUPLO", "ZAPLZ", "USQPL", "ESPMI", "ITPMO", "NZPMR", "VEMGT", 
    "BRPMW", "PNPCN", "KHPNH", "FMPNI", "ITPNL", "INPNQ", "CGPNR", "USPNS", "BRPOA", "GAPOG", 
    "PGPOM", "DOPOP", "FIPOR", "TTPOS", "USPOU", "PLPOZ", "ASPPG", "AUPPP", "PFPPT", "USPQI", 
    "AUPQQ", "CZPRG", "XXPRN", "ZAPRY", "PSNAB", "ITPSA", "USPSC", "EGPSD", "PRPSE", "USPSG", 
    "PKPSI", "USPSP", "ITPSR", "FKPSY", "ZAPOL", "AUPTJ", "USPUB", "FRPUF", "AUPUG", "DOPCN", 
    "CLPUQ", "KRPUS", "USPUW", "HRPUY", "USPVD", "BRPVH", "GRPVK", "MXPVR", "USPWM", "MXPXM", 
    "PTPXO", "INPYB", "THPYX", "ZAPZB", "PKPZH", "VEPZO", "JPCHB", "BRMON", "ITBIE", "CYLMS", 
    "PKMPR", "SZQMN", "INQNB", "RSNVS", "ITPDA", "ESSBP", "CVRAI", "INRAJ", "MARAK", "DKRAN", 
    "USRAP", "CKRAR", "PKRAZ", "DEREG", "MARBA", "BRRBR", "ZARCB", "USRDD", "USRDG", "USRMD", 
    "USRAG", "FRRDZ", "BRREC", "ITREG", "ISREY", "ESRUS", "USRFD", "PFRFP", "PFRGI", "MMRGN", 
    "USRHI", "GRRHO", "USRIC", "BRRIO", "LVRIX", "CNRZH", "HRRJK", "USRKD", "USRKS", "AERKT", 
    "NERLT", "EGRMF", "ITRMI", "SERNB", "FRRNE", "DKRNN", "USRNO", "FRRNS", "USROA", "USROC", 
    "AUROK", "ITROM", "PWROR", "ARROS", "NZROT", "BGRDU", "RUROV", "MURRG", "ARRSA", "BSRSD", 
    "USRST", "HNRTB", "NLRTM", "SARUH", "RERUN", "FIRVN", "USRWI", "PKRWP", "PKRYK", "USSAF", 
    "YESAH", "SMSAI", "SVSAL", "USSAN", "BRSAO", "HNSAP", "USSAT", "USSAV", "USSBA", "BLSBH", 
    "FRSBK", "USSBN", "USCSL", "ZASBU", "MYSBW", "USSBY", "ROSBZ", "USSCC", "USSCE", "USSTZ", 
    "USSCK", "DESCN", "ESSCQ", "CUSCU", "USLUI", "HNSDH", "JPSDJ", "SESDL", "DOSDQ", "ESSDR", 
    "JPSOS", "PKSDT", "USSDY", "USSEA", "LYSEB", "KRSEL", "GBSND", "EGSEW", "SCVIC", "TNSFA", 
    "MFMAR", "GLSFJ", "ARSFN", "USSFO", "DKSGD", "USSGF", "VNSGN", "USSGU", "USSGY", "CNSGH", 
    "USSZH", "CNSHY", "AESHJ", "USSHR", "JPSHS", "USSHV", "CVSID", "SGSIN", "UASIP", "ZASIS", 
    "USSIT", "AUSIX", "USSJC", "MXLCB", "BASJJ", "CRSJO", "USSJT", "PRSJU", "FISJY", "KNSKB", 
    "UZSKD", "GRSKG", "MKSKP", "DKSKR", "EGSKV", "RUSKX", "PKSKZ", "ARSLA", "DKSLB", "USSLC", 
    "USSLE", "OMSLL", "MXSLP", "BGSLS", "LCCAS", "BRSLZ", "PTSMA", "USSAC", "GRSMI", "USSMX", 
    "USSNA", "ECSNC", "IELMK", "USSNS", "BGSOF", "NOSOG", "AUSOI", "VUSAN", "FISOT", "GBSOU", 
    "ESSPC", "USSPI", "MPSPN", "USSPS", "HRSPU", "CISPY", "BRSRA", "BOSRB", "MXSRL", "USZOR", 
    "BOSCS", "BRSSA", "GQSSG", "EGSSH", "COSSL", "CLSCL", "USSTL", "SESTO", "USSTP", "DESTR", 
    "USSTS", "VISTT", "INSTV", "VISTX", "IDSUB", "ITSUF", "PKSUL", "GUAGA", "US5SV", "FJSUV", 
    "USSUX", "VCKTN", "NOSVG", "FISVL", "ESSVQ", "RUYEK", "USSWF", "NASWP", "DESWR", "GBSWA", 
    "FRSXB", "IESLI", "SXPHI", "INSXR", "AUSYD", "USSYR", "GBSTO", "GBSHE", "TRSSX", "ATSZG", 
    "ZASZK", "CNSNZ", "PLSZZ", "TTSCR", "JPTAK", "MXTAM", "CNQIN", "UZTAS", "TWTPE", "GETBS", 
    "TOTBU", "AUTCA", "BSTCB", "ESTCI", "USTCL", "ZATCU", "DKTED", "AUTEM", "TRTEK", "PTTER", 
    "NZTEU", "USQTR", "METGD", "HNTGU", "BGTGV", "MXTGZ", "BRTHE", "IRTHR", "ALTIA", "SATIF", 
    "MXTIJ", "LYTIP", "AUTIS", "METIV", "USTKA", "JPTKS", "FITKU", "USTLH", "EETLL", "FRTLS", 
    "ILTLV", "FITMP", "STTMS", "AUTMW", "UZTER", "CNJNA", "MATNG", "MGTNR", "MYTOD", "USTOL", 
    "NOTOS", "NCTOU", "VGTOV", "JPTOY", "USTPA", "AUTPR", "ITTPS", "NOTRD", "USOUI", "ITTRN", 
    "AUTRO", "ITTRS", "INTRV", "KITRW", "INTRI", "NATSB", "KZAST", "ITTRV", "CNTNJ", "AUTSV", 
    "USTTN", "ARTUC", "PKTUK", "USTUL", "TNTUN", "USTUP", "USTUZ", "SATUU", "USTVC", "USTVF", 
    "USTVL", "AUTWB", "USTWF", "MYTWU", "USTXK", "CNTYU", "JPTYO", "USTYR", "USTYS", "TRTZX", 
    "PFUAH", "GLUAK", "PFUAP", "BRUBB", "JPUBJ", "THUBP", "USUCA", "RUUCT", "NLUDE", "BRUDI", 
    "UAUZH", "INUDR", "AUUEE", "PKUET", "RUUFA", "UZUGC", "AOUGO", "CZUHE", "HNUII", "USUIN", 
    "ECUIO", "FRUIP", "MHUIT", "JPUKB", "USUKI", "JPUKY", "VUULB", "ZAULD", "MNULN", "UGULU", 
    "GLUMD", "SEUME", "AUUMR", "VCUNI", "USUNK", "GBUNT", "IDUPG", "CRUPA", "MX2NB", "USUPP", 
    "BRXXX", "CNURM", "BRURG", "VEURM", "AFURZ", "ARUSH", "KRUSN", "NLUTC", "THUTH", "ZAUTN", 
    "THUTP", "ZAUTT", "RUUUD", "GAOYE", "LCVIF", "EGUVL", "FIVAA", "TRVAN", "CLVAP", "BGVAR", 
    "TRVAS", "VAVAT", "ITBRC", "SEVBY", "ITVCE", "ESVDH", "USVDZ", "USVEL", "MXVER", "ZWVFA", 
    "ESVGO", "BGVID", "ATVIE", "VGVIJ", "USVIS", "ESVIT", "USVIV", "BRVIX", "ESVLC", "USVLD", 
    "VUVLI", "ESVLL", "VEVLN", "RUVLU", "LTVIL", "INVNS", "RUVOG", "USFWB", "CUVRA", "USVRB", 
    "FIVRK", "ITVRN", "MXVSA", "SEVST", "LAVTE", "RUVVO", "SEVXO", "ZAVYD", "USVYV", "HRVZN", 
    "PLWAW", "NAWDH", "AUWEI", "ZAWEL", "AUWGA", "NZWHK", "AUWAM", "GBWCK", "DEWIB", "USWLB", 
    "NZWLG", "WFMAU", "AUWMB", "PKWNS", "AUWOL", "USWPK", "NZWRE", "USWRG", "USWRL", "PLWRO", 
    "CNNHN", "AUWUN", "CNWXS", "ZAWVB", "AUWYA", "AUWYN", "USWYS", "CXFFC", "CNXYA", "CAXXX", 
    "USXLX", "PFXMH", "CNXAM", "CAXPK", "ESJRZ", "CAXSI", "XXXXX", "USYAK", "CMYAO", "CAYAT", 
    "CAYBE", "CAYCB", "CAYDF", "CAEDM", "CAYEV", "CAFAL", "CAIQL", "CAFRE", "CAYFO", "CAYGL", 
    "CAYGW", "CAYGX", "CAYHR", "CAHAL", "CASAU", "CNYIC", "CAKML", "USYKM", "RUYKS", "CALRA", 
    "CAKWL", "CAYMM", "CAMTR", "SAYNB", "JPYOK", "CARBL", "CAOTT", "CAPME", "CAPRR", "CAQUE", 
    "CAYQD", "CAWND", "CAMNT", "CAREG", "CATHU", "CAGAN", "CAREB", "CASJB", "CAFSM", "CANVK", 
    "CAYTH", "CATOR", "CAYUD", "USYUM", "CAHAB", "CAYVB", "CAYVO", "CAFCM", "CAYVQ", "CAVAN", 
    "CAWNP", "CAYWK", "CASAK", "CAFSJ", "CAYXN", "CAPRG", "CAYXT", "CALOD", "CAYXY", "CACAL", 
    "CAYYD", "CAVIC", "CACHV", "CAGOO", "CASJF", "CAMIS", "CAYZF", "CASSP", "HRZAD", "HRZAG", 
    "ESZAZ", "AUZBO", "MXZCL", "MXZIH", "CAZKE", "MXZLO", "NEZND", "AUNWM", "NZZQN", "CHZRH", 
    "BSZSA", "CIZSS", "GRZTH", "CAZTM", "BDZYL"
};



const char * const City::m_fullNames[NUMCITY] = { "No City",
    "Abenra", "Al Arish", "Annaba", "Aalborg", "Al Ain", "Aarhus", "Altay", "Abadan", "Allentown", "Abilene", 
    "Abidjan", "Bamaga", "Albuquerque", "Aberdeen", "Abu Simbel", "Abuja", "Albury", "Albany", "Aberdeen", "Acapulco", 
    "Accra", "Lanzarote", "Altenrhein", "Alderney", "Nantucket", "Waco", "Eureka", "Atlantic City", "Adana", "Addis Ababa", 
    "Aden", "Adiyaman", "Adelaide", "Kodiak", "Alldays", "San Andres", "Abeche", "Sochi", "Alesund", "Agadir", 
    "Augsburg", "Malaga", "Augusta", "Aguascalientes", "Aggeneys", "Khamis Mushayt", "Athens", "Alghero", "Al Hoceima", "Ajaccio", 
    "Jouf", "Anjouan", "Aracaju", "Agades", "Auckland", "King Salmon", "Akrotiri", "Almaty", "Albany", "Alicante", 
    "Alta", "Algiers", "Albany", "Alexander Bay", "Waterloo", "Aleppo", "Andorra la Vella", "Walla Walla", "Alexandria", "Amarillo", 
    "Ahmedabad", "Amman", "Amsterdam", "Anniston", "Anchorage", "Ankara", "Annapolis", "Antwerp", "Saint John's", "Ancona", 
    "Aomori", "Karpathos", "Altoona", "Naples", "Apia", "Aqaba", "Ann Arbor", "Arkhangelsk", "Arusha", "Ashgabat", 
    "Aspen", "Jamestown", "Amami Oshima", "Yamoussoukro", "Asmara", "Alice Springs", "Kayseri", "Asuncion", "Aswan", "Athens", 
    "Atlanta", "Athens", "Amritsar", "Appleton", "Watertown", "Assiut", "Oranjestad", "Abu Rudeis", "Augusta", "Abu Dhabi", 
    "Aurillac", "Austin", "Asheville", "Wilkes-Barre", "The Valley", "Altus", "Akita", "Aylesbury", "Ayers Rock", "Ayr", 
    "Antalya", "Aiyura", "Ayawasi", "Amazon Bay", "Kalamazoo", "Samana", "Manama", "Baku", "Barranquilla", "Bhubaneswar", 
    "Basse-Terre", "Berberati", "Bucharest", "Bambari", "Barcelona", "Jinka", "Boca Raton", "Belmopan", "Hamilton", "Bundaberg", 
    "Hartford", "Bandung", "Baroda", "Bridgeport", "Brindisi", "Bardufoss", "Benbecula", "Belgrade", "Benton Harbor", "Belem", 
    "Benghazi", "Berlin", "Brest", "Bethel", "Beira", "Beirut", "Bradford", "Bakersfield", "Bloemfontein", "Buffalo Range", 
    "Belfast", "Bucaramanga", "Bangui", "Bridgetown", "Johnson City", "Bergen", "Bangor", "Bangassou", "Baghdad", "Bergamo", 
    "Bullhead City", "Blenheim", "Birmingham", "Bhopal", "Broken Hill", "Bahawalpur", "Birmingham", "Bastia", "Billings", "Bilbao", 
    "Biarritz", "Bismarck", "Bria", "Bemidji", "Banjul", "Bujumbura", "Beijing", "Bodrum", "Leon", "Badajoz", 
    "Kota Kinabalu", "Bangkok", "Bamako", "Beckley", "Brookings", "Barcelona", "Bluefield", "Bellingham", "Blackpool", "Billund", 
    "Bologna", "Bangalore", "Blackwater", "Blantyre", "Broome", "Bloomington", "Bloomington", "Brampton", "Nashville", "Brisbane", 
    "Bonn", "Ballina", "Bronnoysund", "Bannu", "Banja Luka", "Bora Bora", "Bordeaux", "Bogota", "Bournemouth", "Boise", 
    "Bourgas", "Mumbai", "Kralendijk", "Bodo", "Boston", "Bobo-Dioulasso", "Beaumont", "Brunswick", "Aguadilla", "San Carlos de Bariloche", 
    "Brainerd", "Bremen", "Bari", "Burlington", "Bern", "Barra", "Bristol", "Brussels", "Brasilia", "Basel", 
    "Basra", "Butte", "Baton Rouge", "Bratislava", "Bintulu", "Burlington", "Budapest", "Buenos Aires", "Buffalo", "Benguela", 
    "Bulawayo", "Burbank", "Boa Vista", "Baltimore", "Bandar Seri Begawan", "Burnie", "Borrego Springs", "Bouake", "Bayreuth", "Belize City", 
    "Barisal", "Bozeman", "Brazzaville", "Cabinda", "Columbia", "Cagliari", "Cairo", "Akron", "Campbeltown", "Guangzhou", 
    "Casablanca", "Cayenne", "Cochabamba", "Cambridge", "Canberra", "Cottbus", "Calicut", "West Island", "Concord", "Caracas", 
    "Calcutta", "Chub Cay", "Cedar City", "Cordova", "Cebu City", "Crescent City", "Ceduna", "Ciudad Obregon", "Cannes", "Cessnock", 
    "Clermont-Ferrand", "Cienfuegos", "Donegal", "Coffs Harbour", "Corfu", "Craig", "Cuiaba", "Cologne", "Zhengzhou", "Chittagong", 
    "Changchun", "Campo Grande", "Ciudad Guayana", "Chattanooga", "Christchurch", "Chaoyang", "Chicago", "Charlottesville", "Chania", "Charleston", 
    "Chico", "Cedar Rapids", "Chipata", "Canouan Island", "Coimbatore", "Calama", "Chitral", "Ciudad Juarez", "Clarksburg", "Chongqing", 
    "Conakry", "Carlsbad", "Cleveland", "Cluj-Napoca", "College Station", "Port Angeles", "Cali", "Colima", "Charlotte", "Calvi", 
    "Colombo", "Ciudad del Carmen", "Chambery", "Columbus", "Champaign", "Clermont", "Hancock", "Constanta", "Belo Horizonte", "Corrientes", 
    "Cairns", "Chiang Mai", "Cody", "Cochin", "Concord", "Porto-Novo", "Cordoba", "Colorado Springs", "Coober Pedy", "Copenhagen", 
    "Casper", "Cape Town", "Carnot", "Jacksonville", "Corpus Christi", "Charleston", "Columbus", "Casino", "Carson City", "Castaway", 
    "Catania", "Cartagena", "Cooktown", "Chengdu", "Culiacan", "Cancun", "Willemstad", "Chihuahua", "Cincinnati", "Ciudad Victoria", 
    "Carnarvon", "Coventry", "Wausau", "Curitiba", "Cardiff", "Kiritimati", "Charters Towers", "Cheyenne", "Cuyo", "Chichen-Itza", 
    "Constantine", "Cozumel", "Daytona Beach", "Dhaka", "Dallas", "Damascus", "Danville", "Dar es Salaam", "Dayton", "Dubbo", 
    "Dubuque", "Dubrovnik", "Dalby", "Washington", "Roseau", "Daydream Island", "Decatur", "New Delhi", "Denver", "Detroit", 
    "Dhahran", "Dothan", "Dili", "Jambi", "Djerba", "Jayapura", "Daloa", "Dunk Island", "Dakar", "Douala", 
    "Dalian", "Dillingham", "Duluth", "Dalaman", "Dillon's Bay", "Ad Dammam", "Dundee", "Dnipro", "Dinard", "Denizli", 
    "Dodoma", "Doha", "Melville Hall", "Dover", "Devonport", "Bali", "Derby", "Durango", "Dresden", "Darwin", 
    "Dera Ismail Khan", "Des Moines", "Dortmund", "Dublin", "Dunedin", "Dubois", "Durban", "Dusseldorf", "Dutch Harbor", "Devils Lake", 
    "Dubai", "Dysart", "Dushanbe", "Mamoudzou", "San Sebastian", "Wenatchee", "Eau Claire", "Elba", "Entebbe", "Esbjerg", 
    "Ebene", "Saint-Etienne", "Edinburgh", "Bergerac", "Vail", "Egilsstadir", "Eindhoven", "Road Town", "Elkhart", "Elko", 
    "North Eleuthera", "Ellisras", "Elmira", "El Paso", "East London", "Ely", "Nottingham", "Emerald", "El Minya", "Kenai", 
    "Nancy", "Enontekio", "Ecatepec", "Esperance", "Erzincan", "Erfurt", "Erie", "Erzurum", "Espoo", "Escanaba", 
    "Alexandria", "Brighton", "El Salvador", "Elat", "Eugene", "El Aaiun", "Evenes", "Yerevan", "Evansville", "New Bern", 
    "Jersey City", "Exeter", "Key West", "Esch-sur-Alzette", "Torshavn", "Fairbanks", "Faro", "Fargo", "Fresno", "Fort Bragg", 
    "Lubumbashi", "Kalispell", "Foster City", "Fort-de-France", "Friedrichshafen", "Fez", "Frankfort", "Fort Huachuca", "Fair Isle", "Kinshasa", 
    "Al Fujayrah", "Karlsruhe", "Kisangani", "Franklin", "Fukushima", "Flagstaff", "Fort Lauderdale", "Florianopolis", "Florence", "Florence", 
    "Farmington", "Osnabruck", "Fort Myers", "Freetown", "Funchal", "Nimes", "Pyongyang", "Flint", "Fort Dodge", "Topeka", 
    "Fortaleza", "Foula", "Freeport", "Frankfurt am Main", "Fort Riley", "Frejus", "Floro", "Bishkek", "Francistown", "Figari", 
    "Sioux Falls", "Fort Smith", "Saint-Pierre", "Fuerteventura", "Fukuoka", "Funafuti", "Futuna", "Fort Wayne", "Fayetteville", "Gadsden", 
    "Yamagata", "Guwahati", "Gaborone", "GIFT City", "Gillette", "Saint Peter Port", "George Town", "Grand Canyon", "Gandhinagar", "Guadalajara", 
    "Gdansk", "Cockburn Town", "Glendive", "Spokane", "Georgetown", "Geraldton", "Geelong", "Griffith", "Grand Forks", "Longview", 
    "Glasgow", "Governors Harbour", "Gibraltar", "Gilgit", "Jijel", "Grand Junction", "Great Keppel Island", "Glasgow", "Greenville", "Gladstone", 
    "Grenoble", "Saint Georges", "Ghent", "Gainesville", "Genoa", "Nuuk", "Goa", "Nizhny Novgorod", "Groton", "Goondiwindi", 
    "Gothenburg", "Garoua", "Gove", "Gorna Oryakhovitsa", "Araxos", "Gulfport", "Grand Rapids", "Green Bay", "Greenwich", "George", 
    "Grindsted", "Gerona", "Groningen", "Grand Rapids", "Jaen", "Graz", "King Edward Point", "Guadalcanal", "Greensboro", "Greenville", 
    "Groote Eylandt", "Great Falls", "Guttin", "Mount Cook", "Guatemala City", "Gunnison", "Hagatna", "Geneva", "Gwadar", "Gweru", 
    "Westerland", "Galway", "Guayaquil", "Goiania", "Gympie", "Gaza", "Gaziantep", "Hachijo Jima", "The Hague", "Moroni", 
    "Hannover", "Hamburg", "Hanoi", "Whitsunday Island", "Harrisburg", "Haugesund", "Havana", "Hobart", "Hyderabad", "Steamboat Springs", 
    "Hatyai", "Helsinki", "Heraklion", "Haifa", "Hammerfest", "Hangzhou", "Korhogo", "Hilton Head Island", "Hibbing", "Lake Havasu City", 
    "Hiroshima", "Honiara", "Hayman Island", "Hakodate", "Hong Kong", "Phuket", "Hickory", "Lanseria", "Ulanhot", "Helena", 
    "Hamilton", "Hamilton", "Hermosillo", "Morioka", "Hoonah", "Hinchinbrook Island", "Honolulu", "Haines", "Holguin", "Homer", 
    "Huron", "Hof", "Horta", "Houston", "White Plains", "Harbin", "Harare", "Hurghada", "Kharkov", "Harlingen", 
    "Horsens", "Huntsville", "Chita", "'s-Hertogenbosch", "Hamilton Island", "Huntington", "Terre Haute", "Huahine", "Hue", "Huatulco", 
    "Humberside", "Hervey Bay", "New Haven", "Havre", "Hwange National Park", "Hyannis", "Hyderabad", "Hydaburg", "Niagara Falls", "Ibiza", 
    "Incheon", "Wichita", "Idaho Falls", "Indore", "Kyiv", "Innisfail", "Ingham", "Iguazu", "Jacksonville", "Irkutsk", 
    "Killeen", "Wilmington", "Iliamna", "Wilmington", "Ile des Pins", "Islay", "Zilina", "Indianapolis", "Nis", "International Falls", 
    "Innsbruck", "Yaren", "Inverness", "Douglas", "Ile Ouen", "Easter Island", "Imperial", "Williamsport", "Iquitos", "Lockhart River", 
    "Birao", "Mount Isa", "Islamabad", "Ishigaki", "Williston", "Kingston", "Long Island", "Istanbul", "Ithaca", "Hilo", 
    "Alofi", "Invercargill", "Ivalo", "Bagdogra", "Chandigarh", "Belgaum", "Jammu", "Ranchi", "Jamshedpur", "Inyokern", 
    "Izmir", "Jalalabad", "Jackson Hole", "Jandakot", "Jaffna", "Jacobabad", "Jaipur", "Jacmel", "Jalapa", "Jambol", 
    "Jackson", "Jacquinot Bay", "Jauja", "Jonesboro", "Joacaba", "Julia Creek", "Jacobina", "Juiz de Fora", "Jodhpur", "Jingdezhen", 
    "Jeddah", "Jeremie", "Jefferson City", "Saint Helier", "Jamnagar", "Jagdalpur", "Jiayuguan", "Jian", "Johor Bahru", "Helsingborg", 
    "Jinghong", "Kapalua", "Shute Harbour", "Jamestown", "Djibouti", "Jijiga", "Jilin", "Jimma", "Jinja", "Jipijapa", 
    "Jiri", "Jiujiang", "Jiwani", "Juanjui", "Jinjiang", "Jonkoping", "Chios", "Janakpur", "Jakarta", "Jacksonville", 
    "Joplin", "Jalandhar", "Jales", "Jamba", "Mykonos", "Jomsom", "Jamestown", "Jiamusi", "Januaria", "Johannesburg", 
    "Jining", "Junin", "Juneau", "Naxos", "Jinzhou", "Joensuu", "Joinville", "Jolo", "Johnston Atoll", "Jos", 
    "Joao Pessoa", "Jaque", "Jorhat", "Kilimanjaro", "Jerusalem", "Jaisalmer", "Skiathos", "Jose de San Martin", "Johnstown", "Jatai", 
    "Thira", "Juba", "Juist", "Jujuy", "Juliaca", "Jumla", "Jundah", "Juticalpa", "Upernavik", "Janesville", 
    "Jwaneng", "Jackson", "Jyvaskyla", "Kajaani", "Kano", "Kuusamo", "Kabul", "Streaky Bay", "Coffman Cove", "Collinsville", 
    "Kuching", "Kahramanmaras", "Kochi", "Khuzdar", "Skardu", "Kiel", "Kemi", "Kingscote", "Kaliningrad", "Kalgoorlie", 
    "Kigali", "Kos", "Kaohsiung", "Karachi", "Kauhajoki", "Khabarovsk", "Kristianstad", "Niigata", "Kirkuk", "Kimberley", 
    "Kingston", "Kerry", "Chisinau", "Kitwe", "Kirkenes", "Kampala", "Kalmar", "Klagenfurt", "Klawock", "Kalamata", 
    "Kleinzee", "Miyazaki", "Kumamoto", "Keetmanshoop", "Komatsu", "King Island", "Kanpur", "Kununurra", "Kailua Kona", "Kirkwall", 
    "Kagoshima", "Kokkola", "Karumba", "Krakow", "Kiruna", "Karup", "Kristiansand", "Khartoum", "Karlstad", "Kassala", 
    "Kristiansund", "Karratha", "Thorne Bay", "Kathmandu", "Ketchikan", "Katherine", "Kittila", "Kuantan", "Samara", "Kushiro", 
    "Kuala Lumpur", "Kuopio", "Kavala", "Kuwait City", "Guilin", "Kowanyama", "Konya", "Kars", "Kazan", "Luanda", 
    "Lae", "Lafayette", "Lannion", "Lansing", "La Paz", "Laramie", "Las Vegas", "Lawton", "Los Angeles", "Leeds", 
    "Lubbock", "Latrobe", "Albi", "Lambarene", "Labuan", "Libreville", "Larnaca", "La Coruna", "Lake Charles", "Lindeman Island", 
    "Lourdes", "Lidkoping", "Londonderry", "Learmonth", "Lebanon", "Saint Petersburg", "Almeria", "Leipzig", "Labe", "Land's End", 
    "Leinster", "Leuven", "Lexington", "Lelystad", "Lafayette", "Lome", "Long Beach", "Liege", "Langkawi", "Lahore", 
    "Vaduz", "Lifou", "Limoges", "Lihue", "Long Island", "Lille", "Lima", "Lisala", "Lisbon", "Little Rock", 
    "Ljubljana", "Lakselv", "Lucknow", "Lulea", "Lilongwe", "Los Mochis", "Lampedusa", "Klamath Falls", "Lincoln", "Leonora", 
    "Lancaster", "Lanai City", "Linz", "London", "Lagos", "Las Palmas", "La Paz", "Liverpool", "Lappeenranta", "Laredo", 
    "Longreach", "Jacksonville", "La Rochelle", "Casa de Campo", "Lorient", "La Crosse", "Sumburgh", "Launceston", "Lismore", "Veli Losinj", 
    "Loreto", "Luderitz", "Lugano", "Lusikisiki", "Mangshi", "Lusaka", "Luxembourg", "Laverton", "Lewisburg", "Lerwick", 
    "L'viv", "Lewiston", "Lewistown", "Luxor", "Lynchburg", "Lyons", "Faisalabad", "Longyearbyen", "Lyon", "Lydd", 
    "Lazaro Cardenas", "Lucerne", "Lizard Island", "Madras", "Madrid", "Midland", "Mahon", "Majuro", "Manchester", "Manaus", 
    "Maracaibo", "Maupiti", "Mayaguez", "Mombasa", "Maryborough", "Montego Bay", "Mkambati", "Saginaw", "Maribor", "Merced", 
    "Kansas City", "Mount McKinley", "Monaco", "Macon", "Orlando", "Macapa", "Muscat", "Mason City", "Sunshine Coast", "Maceio", 
    "Manado", "Medellin", "Mandalay", "Mar del Plata", "Midway Islands", "Mendoza", "Madinah", "Mare", "Meridian", "Melbourne", 
    "Memphis", "Medan", "Mexico City", "Messina", "McAllen", "Moanda", "Muzaffarabad", "Macau", "Milford Sound", "Manguna", 
    "Maradi", "Medford", "Mfuwe", "Managua", "Mount Gambier", "Maringa", "Margate", "Montgomery", "Mogadishu", "Morgantown", 
    "Mitchell", "Marsh Harbour", "Mariehamn", "Manchester", "Miami", "Merida", "Mikkeli", "Milan", "Merimbula", "Monastir", 
    "Man", "Mohenjodaro", "Mouila", "Majunga", "Jackson", "Mytilene", "Murcia", "Makati City", "Milwaukee", "Muskegon", 
    "Kaunakakai", "Jackson", "Meekatharra", "Mackay", "Valletta", "Melbourne", "Male", "Mulhouse", "Moline", "Morelia", 
    "Miles City", "Monroe", "Monrovia", "Malatya", "Malmo", "Teesside", "Mount Magnet", "Matsumoto", "Murmansk", "Middlemount", 
    "Miyako Jima", "Brades", "Manila", "Mobile", "Modesto", "Minot", "Moranbah", "Moscow", "Mo'orea", "Katima Mulilo", 
    "Sindhri", "Montpellier", "Maputo", "Mildura", "Mardin", "Moundou", "Marquette", "Martinsburg", "Marseille", "Port Louis", 
    "Mineralnye Vody", "Monterey", "Moree", "Kent", "Muscle Shoals", "Madison", "Missoula", "Minneapolis", "Minsk", "Mus", 
    "Maastricht", "Maseru", "New Orleans", "Marathon", "Montrose", "Maitland", "Metlakatla", "Mattoon", "Manzini", "Minatitlan", 
    "Monterrey", "Maun", "Munich", "Kamuela", "Matruh", "Multan", "Mvengue", "Montevideo", "Maroua", "Martha's Vineyard", 
    "Masvingo", "Mianwali", "Moses Lake", "Mexicali", "Moruya", "Malindi", "Matsuyama", "Mysore", "Myrtle Beach", "Miri", 
    "Mzamba", "Makung", "Metz", "Mazatlan", "Mossel Bay", "Narrabri", "Nagpur", "Nakhchivan", "Nadi", "Naples", 
    "Nassau", "Natal", "Nairobi", "Nice", "Newcastle", "Newcastle", "Annecy", "Nouadhibou", "N'Djamena", "Rundu", 
    "Nevis", "Ngaoundere", "Nagoya", "Nagasaki", "Nicosia", "Niamey", "Nouakchott", "Nukunonu", "Diego Garcia", "Ndola", 
    "Nuevo Laredo", "Kingston", "Nelspruit", "Nikolaev", "Nanning", "Knock", "Mactan Island", "Noumea", "Newquay", "Narrandera", 
    "Norrkoping", "Noosa", "Nelson", "Scone", "Nakhon Si Thammarat", "Nantes", "Newcastle", "Pilanesberg", "Nuremberg", "Norwich", 
    "New York", "Orange", "Jacksonville", "Oakland", "Oaxaca", "Ouadda", "Cordoba", "Odense", "Odessa", "Kahului", 
    "Ohrid", "Kohat", "Oita", "Okinawa", "Oklahoma City", "Sapporo", "Okayama", "Mokuti Lodge", "Olbia", "Wolf Point", 
    "Olympia", "Oldenburg", "Olympic Dam", "Omaha", "Oranjemund", "Nome", "Urmia", "Mostar", "Ondangwa", "Ontario", 
    "Gold Coast", "Cooma", "Porto", "Orebro", "Norfolk", "Worcester", "Cork", "Oran", "Orpheus Island", "Osaka", 
    "Oshkosh", "Osijek", "Oslo", "Mosul", "North Bend", "Kotzebue", "Ouagadougou", "Oujda", "Oudtshoorn", "Outer Skerries", 
    "Oulu", "Zouerat", "Novosibirsk", "Oviedo", "Owensboro", "Bissau", "Oxnard", "Ouarzazate", "Panama City", "Paderborn", 
    "Paducah", "Port-au-Prince", "Paris", "Patna", "Puebla", "Thimphu", "West Palm Beach", "Paramaribo", "Paraburdoo", "Plettenberg Bay", 
    "Pasig City", "Princeton", "Ponta Delgada", "Pendleton", "Portland", "Perugia", "Pereira", "Penang", "Perth", "Peshawar", 
    "Panama City", "Paphos", "Port-aux-Francais", "Page", "Perpignan", "Greenville", "Port Harcourt", "Port Hedland", "Newport News", "Philadelphia", 
    "Phalaborwa", "Phoenix", "Peoria", "Laurel", "Pocatello", "Pierre", "Poitiers", "Pittsburgh", "Panjgur", "Parkersburg", 
    "Selebi-Phikwe", "Plattsburgh", "Pellston", "Port Lincoln", "Port Elizabeth", "Palmdale", "Palma de Mallorca", "Palermo", "Palmerston North", "Margarita", 
    "Palmas", "Adamstown", "Phnom Penh", "Palikir", "Pantelleria", "Pune", "Pointe Noire", "Pensacola", "Porto Alegre", "Port Gentil", 
    "Port Moresby", "Puerto Plata", "Pori", "Port-of-Spain", "Poughkeepsie", "Poznan", "Pago Pago", "Proserpine", "Papeete", "Presque Isle", 
    "Port Macquarie", "Prague", "Pristina", "Pretoria", "Nabulus", "Pisa", "Pasco", "Port Said", "Ponce", "Petersburg", 
    "Pasni", "Palm Springs", "Pescara", "Stanley", "Polokwane", "Portland", "Pueblo", "Pau", "Port Augusta", "Punta Cana", 
    "Punta Arenas", "Busan", "Pullman", "Pula", "Providence", "Porto Velho", "Preveza", "Puerto Vallarta", "Portland", "Puerto Escondido", 
    "Porto Santo Island", "Jeypore", "Pattaya", "Pietermaritzburg", "Zhob", "Puerto Ordaz", "Chiba", "Montenegro", "Biella", "Limassol", 
    "Mirpur", "Mbabane", "Anand", "Novi Sad", "Padua", "Sabadell", "Praia", "Rajkot", "Marrakech", "Randers", 
    "Rapid City", "Avarua", "Rawalakot", "Regensburg", "Rabat", "Rio Branco", "Richards Bay", "Redding", "Reading", "Redmond", 
    "Raleigh", "Rodez", "Recife", "Reggio Calabria", "Reykjavik", "Reus", "Rockford", "Raiatea", "Rangiroa", "Yangon", 
    "Rhinelander", "Rhodes", "Richmond", "Rio de Janeiro", "Riga", "Rizhao", "Rijeka", "Rockland", "Rock Springs", "Ras al Khaimah", 
    "Arlit", "Marsa Alam", "Rimini", "Ronneby", "Roanne", "Ronne", "Reno", "Rennes", "Roanoke", "Rochester", 
    "Rockhampton", "Rome", "Ngerulmud", "Rosario", "Rotorua", "Ruse", "Rostov", "Rodrigues", "Santa Rosa", "Rock Sound", 
    "Rochester", "Roatan", "Rotterdam", "Riyadh", "Saint-Denis", "Rovaniemi", "Rocky Mount", "Rawalpindi", "Rahim Yar Khan", "Santa Fe", 
    "Sana'a", "San Marino City", "San Salvador", "San Diego", "Sao Paulo", "San Pedro Sula", "San Antonio", "Savannah", "Santa Barbara", "Gustavia", 
    "Saint-Brieuc", "South Bend", "San Luis Obispo", "Springbok", "Sibu", "Salisbury", "Sibiu", "Prudhoe Bay", "State College", "Scottsdale", 
    "Stockton", "Saarbrucken", "Santiago de Compostela", "Santiago de Cuba", "Louisville", "Santa Rosa de Copan", "Sendai", "Sundsvall", "Santo Domingo", "Santander", 
    "Sado Shima", "Saidu Sharif", "Sidney", "Seattle", "Sebha", "Seoul", "Southend", "Siwa", "Victoria", "Sfax", 
    "Marigot", "Kangerlussuaq", "Santa Fe", "San Francisco", "Sonderborg", "Springfield", "Ho Chi Minh City", "Saint George", "Skagway", "Shanghai", 
    "Shenandoah", "Shenyang", "Sharjah", "Sheridan", "Shimonoseki", "Shreveport", "Sal", "Singapore", "Simferopol", "Sishen", 
    "Sitka", "Singleton", "San Jose", "Los Cabos", "Sarajevo", "San Jose", "San Angelo", "San Juan", "Seinajoki", "Basseterre", 
    "Samarkand", "Thessaloniki", "Skopje", "Skrydstrup", "Saint Catherine", "Saransk", "Sukkur", "Salta", "Silkeborg", "Salt Lake City", 
    "Salem", "Salalah", "San Luis Potosi", "Silistra", "Castries", "Sao Luis", "Santa Maria", "Sacramento", "Samos", "Santa Maria", 
    "Santa Ana", "Salinas", "Limerick", "Salinas", "Sofia", "Sogndal", "South Molle Island", "Santo", "Sodankyla", "Southampton", 
    "Santa Cruz de La Palma", "Springfield", "Capital Hill", "Wichita Falls", "Split", "San-Pedro", "Santa Rosa", "Santa Rosa", "Santa Rosalia", "Sarasota", 
    "Santa Cruz de la Sierra", "Salvador", "Malabo", "Sharm El Sheikh", "Santa Rosalia", "Santiago", "Saint Louis", "Stockholm", "Saint Paul", "Stuttgart", 
    "Santa Rosa", "Charlotte Amalie", "Surat", "Saint Croix", "Surabaya", "Lamezia-Terme", "Sui", "Agana", "Sun Valley", "Suva", 
    "Sioux City", "Kingstown", "Stavanger", "Savonlinna", "Sevilla", "Yekaterinburg", "Newburgh", "Swakopmund", "Schwerin", "Swansea", 
    "Strasbourg", "Sligo", "Philipsburg", "Srinagar", "Sydney", "Syracuse", "Stornoway", "Sheffield", "Samsun", "Salzburg", 
    "Skukuza", "Shenzhen", "Szczecin", "Scarborough", "Takamatsu", "Tampico", "Qingdao", "Tashkent", "Taipei", "Tbilisi", 
    "Nuku'alofa", "Tennant Creek", "Treasure Cay", "Tenerife", "Tuscaloosa", "Thaba Nchu", "Thisted", "Temora", "Tekirdag", "Terceira", 
    "Te Anau", "Telluride", "Podgorica", "Tegucigalpa", "Targovishte", "Tuxtla Gutierrez", "Teresina", "Tehran", "Tirana", "Taif", 
    "Tijuana", "Tripoli", "Thursday Island", "Tivat", "Talkeetna", "Tokushima", "Turku", "Tallahassee", "Tallinn", "Toulouse", 
    "Tel Aviv-Yafo", "Tampere", "Sao Tome", "Tamworth", "Termez", "Jinan", "Tangier", "Antananarivo", "Tioman", "Toledo", 
    "Tromso", "Touho", "Tortola", "Toyama", "Tampa", "Tom Price", "Trapani", "Trondheim", "Blountville", "Turin", 
    "Taree", "Trieste", "Thiruvananthapuram", "Tarawa", "Tiruchirappalli", "Tsumeb", "Astana", "Treviso", "Tianjin", "Townsville", 
    "Trenton", "Tucuman", "Turbat", "Tulsa", "Tunis", "Tupelo", "Tucson", "Tabuk", "Traverse City", "Thief River Falls", 
    "South Lake Tahoe", "Toowoomba", "Twin Falls", "Tawau", "Texarkana", "Taiyuan", "Tokyo", "Tyler", "Knoxville", "Trabzon", 
    "Ua Huka", "Narsarsuaq", "Ua Pou", "Uberaba", "Ube", "Ubon Ratchathani", "Utica", "Ukhta", "Uden", "Uberlandia", 
    "Uzhgorod", "Udaipur", "Queenstown", "Quetta", "Ufa", "Urgench", "Uige", "Uherske Hradiste", "Utila", "Quincy", 
    "Quito", "Quimper", "Jaluit Island", "Kobe", "Ukiah", "Kyoto", "Ulei", "Ulundi", "Ulan Bator", "Gulu", 
    "Uummannaq", "Umea", "Woomera", "Union Island", "Unalakleet", "Unst", "Ujung Pandang", "Upala", "Uruapan", "Upolu Point", 
    "Urubupunga", "Urumqi", "Uruguaiana", "Uriman", "Uruzgan", "Ushuaia", "Ulsan", "Utrecht", "Udon Thani", "Upington", 
    "Utapao", "Umtata", "Ulan-Ude", "Oyem", "Vieux Fort", "Kharga", "Vaasa", "Van", "Valparaiso", "Varna", 
    "Sivas", "Vatican City", "Brescia", "Visby", "Venice", "Valverde", "Valdez", "Vernal", "Veracruz", "Victoria Falls", 
    "Vigo", "Vidin", "Vienna", "Virgin Gorda", "Visalia", "Vitoria-Gasteiz", "Vienna", "Vitoria", "Valencia", "Valdosta", 
    "Port Vila", "Valladolid", "Valencia", "Velikiye Luki", "Vilnius", "Varanasi", "Volgograd", "Fort Walton Beach", "Varadero", "Vero Beach", 
    "Varkaus", "Verona", "Villahermosa", "Vasteras", "Vientiane", "Vladivostok", "Vaxjo", "Vryheid", "Mill Valley", "Varazdin", 
    "Warsaw", "Windhoek", "Weipa", "Welkom", "Wagga Wagga", "Whakatane", "Wickham", "Wick", "Wiesbaden", "Labouchere Bay", 
    "Wellington", "Mata Utu", "Warrnambool", "Nawabshah", "Wollongong", "Winter Park", "Whangarei", "Wrangell", "Worland", "Wroclaw", 
    "Wuhan", "Wiluna", "Wuxi", "Walvis Bay", "Whyalla", "Wyndham", "West Yellowstone", "Flying Fish Cove", "Xianyang", "Lac Brochet", 
    "Stamford", "Manihi", "Xiamen", "Pukatawagan", "Jerez de la Frontera", "South Indian Lake", "No City", "Yakutat", "Yaounde", "Attawapiskat", 
    "Uranium City", "Cambridge Bay", "Deer Lake", "Edmonton", "Inuvik", "Fort Albany", "Iqaluit", "Fredericton", "Flin Flon", "La Grande", 
    "Kuujjuarapik", "Gillam", "Chevery", "Halifax", "Saint-Augustin", "Yichang", "Kamloops", "Yakima", "Yakutsk", "Leaf Rapids", 
    "Kelowna", "Fort McMurray", "Montreal", "Yanbu", "Yokohama", "Rainbow Lake", "Ottawa", "Port-Menier", "Prince Rupert", "Quebec", 
    "The Pas", "Windsor", "Moncton", "Regina", "Thunder Bay", "Gander", "Resolute Bay", "Saint-John", "Fort Smith", "Nanisivik", 
    "Thompson", "Toronto", "Umiujaq", "Yuma", "Hall Beach", "Bonaventure", "Val-d'Or", "Kuujjuaq", "Norman Wells", "Vancouver", 
    "Winnipeg", "Wabush", "Saskatoon", "Fort-Saint-John", "Whale Cove", "Prince George", "Terrace", "London", "Whitehorse", "Calgary", 
    "Smithers", "Victoria", "Churchill", "Goose Bay", "Saint-John's", "Mississauga", "Yellowknife", "Sandspit", "Zadar", "Zagreb", 
    "Zaragoza", "Bowen", "Zacatecas", "Ixtapa", "Kashechewan", "Manzanillo", "Zinder", "Newman", "Queenstown", "Zurich", 
    "San Salvador Island", "Sassandra", "Zakynthos", "Shamattawa", "Sylhet"
};


//



