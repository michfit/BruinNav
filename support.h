//
//  support.hpp
//  Project 4
//
//  Created by Michelle Su on 3/15/17.
//  Copyright © 2017 Michelle Su. All rights reserved.
//

#ifndef support_hpp
#define support_hpp

#include <stdio.h>

//comparison operators for GeoCoords, which were used in MyMap templated class and in NavigatorImpl
bool operator< (const GeoCoord &first, const GeoCoord &second);
bool operator== (const GeoCoord &first, const GeoCoord &second);
bool operator!= (const GeoCoord &first, const GeoCoord &second);

#endif /* support_hpp */
