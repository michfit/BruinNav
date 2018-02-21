#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>//test
#include <vector>

using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> segments;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    cerr << "before " << endl;
    ifstream mapData(mapFile);
    if (!mapData){
        cerr << "Can't open map data!" << endl;
        return false;
    }
    cerr << "passed" << endl;
    string data;
    while (getline(mapData, data)){
        //store name of street segment
        StreetSegment ss;
        ss.streetName = data;
        
        //get starting coordinates, store as geocoord
        string startLat;
        getline(mapData, startLat, ',');
        mapData.ignore(10000, ' '); //ignore space after comma
        string startLong;
        getline(mapData, startLong, ' ');

        GeoCoord start(startLat, startLong);
        
        //get ending coords, store as geocoord
        string endLat, endLong;
        getline(mapData, endLat, ',');
        getline(mapData, endLong);
        GeoCoord end(endLat, endLong);
        
        GeoSegment gs(start, end);
        ss.segment = gs;
        //store in current segment
        
        string numAttractions;
        getline(mapData, numAttractions);
        
        //convert string to char ptr to convert to int
        const char *c = numAttractions.c_str();
        int nAttractions = atoi(c);
        
        //iterate through each attraction on segment
        for (int i = 0; i < nAttractions; i++){
            Attraction att;
            string name;
            
            //get attraction name
            getline(mapData, name, '|');
            string attLat, attLong;
            getline(mapData, attLat, ',');
            mapData.ignore(10000, ' '); //ignore space after comma
            getline(mapData, attLong);
            GeoCoord attG(attLat, attLong);
            Attraction attr;
            attr.name = name;
            attr.geocoordinates = attG;
            
            ss.attractions.push_back(att);
        }
        segments.push_back(ss); //push each segment into segments
    }
    return true;
}


size_t MapLoaderImpl::getNumSegments() const
{
	return segments.size(); 
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum >= segments.size()){
        return false;
    }
    seg = segments[segNum];
    return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}

int main(){
    cerr << "before ml" << endl;
    MapLoader ml;
    cerr << "ml created" << endl;
    ml.load("/Users/michellesu/Desktop/cs32/Projects/Project 4/Project 4/mapdata.txt");
    
    StreetSegment seg1;
    ml.getSegment(0, seg1);
    assert(seg1.streetName == "10th Helena Drive");
    assert(seg1.segment.start.latitudeText == "34.0547000");
    assert(seg1.segment.start.longitudeText == "-118.4794734");
    assert(seg1.segment.end.latitudeText == "34.0544590");
    assert(seg1.segment.end.longitudeText == "-118.4801137");
    assert(seg1.attractions.size() == 0);
    ml.getSegment(ml.getNumSegments() - 1, seg1);
    assert(seg1.streetName == "access road for water tank");
    assert(seg1.segment.start.latitudeText == "34.0853742");
    assert(seg1.segment.start.longitudeText == "-118.4955413");
    assert(seg1.segment.end.latitudeText == "34.0852898");
    assert(seg1.segment.end.longitudeText == "-118.4954341");
    assert(seg1.attractions.size() == 0);
    cerr << " all tests passed" << endl;
}

