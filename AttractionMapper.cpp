#include "provided.h"
#include "MyMap.h"
#include <cctype>
#include<string>
using namespace std;

class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(std::string attraction, GeoCoord& gc) const;
private:
    MyMap<std::string, GeoCoord> *attractionMap;
};

//non member helper function to convert string to lowercase
void convertStringToLower(std::string& convert){
    for (int i = 0; i < convert.size(); i++){
        convert[i] = tolower(convert[i]);
    }
}
AttractionMapperImpl::AttractionMapperImpl()
{
    MyMap<string, GeoCoord> *attractionMap = new MyMap<string, GeoCoord>();
}

AttractionMapperImpl::~AttractionMapperImpl()
{
    delete attractionMap;
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0; i < ml.getNumSegments(); i++){
        StreetSegment ss;
        if (!ml.getSegment(i, ss))
            return;
        
        //store attraction names as lowercase, so they can be found with getGeoCoord if uppercase name is passed in
        for (Attraction att : ss.attractions){
            convertStringToLower(att.name);
            attractionMap->associate(att.name, att.geocoordinates);
        }
    }
}



bool AttractionMapperImpl::getGeoCoord(std::string attraction, GeoCoord& gc) const
{
    convertStringToLower(attraction);
    if (attractionMap->find(attraction) == nullptr){ //function should be case insensitive so attraction names stored as lowercase
        return false;
    }
    gc = *(attractionMap->find(attraction));
    return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
    m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
    delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(std::string attraction, GeoCoord& gc) const
{
    return m_impl->getGeoCoord(attraction, gc);
}


