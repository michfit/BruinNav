//
//  support.cpp
//  Project 4
//
//  Created by Michelle Su on 3/15/17.
//  Copyright © 2017 Michelle Su. All rights reserved.
//
#include "provided.h"
#include "support.h"
bool operator< (const GeoCoord &first, const GeoCoord &second){
    return first.latitude < second.latitude;
}

bool operator== (const GeoCoord &first, const GeoCoord &second){
    return (first.latitude == second.latitude && first.longitude == second.longitude);
}

bool operator!= (const GeoCoord &first, const GeoCoord &second){
    return (first.latitude != second.latitude || first.longitude != second.longitude);

}


