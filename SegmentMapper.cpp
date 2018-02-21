#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    std::vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, std::vector<StreetSegment>> segMap;
    
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0; i < ml.getNumSegments(); i++){
        StreetSegment ss;
        if (!ml.getSegment(i, ss)) //return if getSegment is false, nothing was retrieved
            return;
        if (segMap.find(ss.segment.start) != nullptr && segMap.find(ss.segment.end) != nullptr){ //check if vector exists for start and end GeoCoords
            segMap.find(ss.segment.start)->push_back(ss); //add ss to vector associated with starting GeoCoord
            segMap.associate(ss.segment.start, *segMap.find(ss.segment.start)); //associate vector with starting GeoCoord
            
            //do same for ending GeoCoord
            segMap.find(ss.segment.end)->push_back(ss);
            segMap.associate(ss.segment.end, *segMap.find(ss.segment.end));
        }
        else{
            //create new vector if start/end GeoCoords not associated with one already
            vector<StreetSegment> st;
            st.push_back(ss);
            segMap.associate(ss.segment.start, st);
            vector<StreetSegment> end;
            segMap.associate(ss.segment.end, end);
            }
        //loop through attractions and associate their geocoordinates with street segment
            for (int i = 0 ; i < ss.attractions.size(); i++){
                
                if (segMap.find(ss.attractions[i].geocoordinates) != nullptr){ //check if attraction's GeoCoords are already associated with vector
                    segMap.find(ss.attractions[i].geocoordinates)->push_back(ss);
                }
                else{ //create new vector with ss and associate with geocoord of current attraction
                    vector<StreetSegment> at;
                    at.push_back(ss);
                    segMap.associate(ss.attractions[i].geocoordinates, at);
                }
            }
            
        
        }
    
    
}

std::vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    //return vector associated with GeoCoord
    return *segMap.find(gc);
}

bool operator< (const GeoCoord &first, const GeoCoord &second){
    return first.latitude < second.latitude;
}

bool operator== (const GeoCoord &first, const GeoCoord &second){
    return (first.latitude == second.latitude && first.longitude == second.longitude);
}



//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
    m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
    delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
    return m_impl->getSegments(gc);
}


