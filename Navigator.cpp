#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
    StreetSegment* find(GeoCoord start, GeoCoord end) const;
    //will be stored in priority queue, nodes at each GeoCoord
    class NavNode{
    public:
        NavNode(GeoCoord gc, double distance, NavNode* prev) :
        position(gc), travelled(distance), previous(prev), visited(false){}
        
        //current pos
        GeoCoord position;
        
        //whether node has been visited
        bool visited;
        
        //visit node
        void setVisitToTrue() {visited = true;}
        
        //distance already travelled from start
        double travelled;
        
        //priority used for comparison
        //priorirty = travelled + distance left to end
        int priority;
        
        //distance to end
        double distanceToEnd;
        
        //pointer to previous Node, for retracing path
        NavNode *previous;
        
        //updates distance travelled
        void updateDistanceTravelled(GeoCoord start){
            travelled = distanceEarthMiles(start, position);
        }
        //updates distance to end, used to determine priority
        void updateDistanceToEnd(GeoCoord end){
            distanceToEnd = distanceEarthMiles(position, end);
        }
        
        //updates priority
        void changePriority(GeoCoord end){
            updateDistanceToEnd(end);
            priority = travelled + distanceToEnd;
        }
        //getter functions
        double getX(){return position.latitude;}
        double getY(){return position.longitude;}
        int getPriority(){return priority;}
        double getDistanceTravelled(){return travelled;}
        //comparison operator for NavNode
        bool operator< (const NavNode & a)
        {//don't know why compiler throws an error
            
            return priority > a.priority; //greater priority items will be at front of queue
        }
        
    };
        
    
private:
    SegmentMapper m_segments;
    AttractionMapper m_attractions;
};




    



NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    
    MapLoader ml; //create MapLoader object to load data onto
    if (!ml.load(mapFile)) //load map data
        return false;
    m_segments.init(ml); //initialize private member variables
    m_attractions.init(ml);
    return true;
}

//return direction in string form
string getDirection(const GeoSegment &line1){
    double angle = angleOfLine(line1);
    if ((angle >= 0 && angle <= 22.5) || (angle > 337.5 && angle < 360))
        return "east";
    if (angle > 22.5 && angle <= 67.5)
        return "northeast";
    if (angle > 67.5 && angle <= 112.5)
        return "north";
    if (angle > 112.5 && angle <= 157.5)
        return "northwest";
    if (angle > 157.5 && angle <= 202.5)
        return "west";
    if (angle > 202.5 && angle <= 247.5)
        return "southwest";
    if (angle > 247.5 && angle <= 292.5)
        return "south";
    if (angle > 292.5 && angle <= 337.5)
        return "southeast";
    return ""; //never returned
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    //empties array if it isn't empty
    directions.clear();
    
    
    //find out where to start from
    GeoCoord s;
    if (!m_attractions.getGeoCoord(start, s)){ //put starting GeoCoord in s
        return NAV_BAD_SOURCE; //return if it doesn't exist
    }
    //do same for end's GeoCoord, get destination GeoCoordinates
    GeoCoord e;
    if (!m_attractions.getGeoCoord(end, e)){
        return NAV_BAD_DESTINATION; //return if it doesn't exist
    }
    
    //check if attractions are on same segment, then we only need to push one NavSegment onto directions
    if (m_segments.getSegments(s)[0].segment.start.latitude == m_segments.getSegments(e)[0].segment.start.latitude && m_segments.getSegments(s)[0].segment.start.longitude == m_segments.getSegments(e)[0].segment.start.longitude)
    {
        NavSegment firstNS(getDirection(m_segments.getSegments(e)[0].segment), m_segments.getSegments(e)[0].streetName, distanceEarthMiles(s, e), m_segments.getSegments(e)[0].segment);
        directions.push_back(firstNS);
        return NAV_SUCCESS;
    }
    
    //create GeoSegment between attraction and end of Street Segment
    GeoSegment f(s, m_segments.getSegments(s)[0].segment.end);
    NavSegment firstNS(getDirection(f), m_segments.getSegments(s)[0].streetName, distanceEarthMiles(s, m_segments.getSegments(s)[0].segment.end), f);
    
    //push required proceed vector onto directions vector
    directions.push_back(firstNS);
    
    //create priority queue to hold NavNodes
    priority_queue<NavNode> q;
    
    //create NavNode representing starting segment
    NavNode* n = new NavNode(s, 0, nullptr);
    n->changePriority(e);
    //push start NavNode onto segment
    q.push(*n);
    
    GeoCoord current = s;
    NavNode* parent = n;
    while(!q.empty()){
        //
        
        //TODO: FIGURE OUT HOW TO SET LOCATION TO VISITED
        
        //if we got to the end
        if (q.top().position == e){
            vector<StreetSegment> path;
            //create stack of geocoords and use it to create path of StreetSegments
            
            //trace back path until beginning
            NavNode cur = q.top();
            
            //while not at start
            while(cur.position != s) {
                StreetSegment* corresponding = find(cur.position, cur.previous->position);
                if (corresponding != nullptr) //push back corresponding street segment if found
                    path.push_back(*corresponding);
                
                
                q.pop(); //remove from priority queue
                
                cur = *(cur.previous); //points to node before it
            }
            
             //TODO: USE HELPER FUNCTION TO PROCESS VECTOR OF STREETSEGMENTS
            return NAV_SUCCESS;
            }
        
        //check which paths can be accessed and add to queue
        for (int i = 0; i < m_segments.getSegments(current).size(); i++){
            //create node to be followed
            NavNode* child = new NavNode(m_segments.getSegments(current)[i].segment.start, distanceEarthMiles(s, current), parent);
            child->changePriority(e);
            q.push(*child);
            
            parent->setVisitToTrue(); //parent has been visited
            
        }
    
    }
    
    
    
    return NAV_NO_ROUTE;  // This compiles, but may not be correct
}


//return pointer to matching segment if there is one

StreetSegment* NavigatorImpl::find(GeoCoord start, GeoCoord end) const{
    for (int i = 0; i < m_segments.getSegments(start).size(); i++){
        if(m_segments.getSegments(start)[i].segment.start == start && m_segments.getSegments(start)[i].segment.end == end)
            return &(m_segments.getSegments(start)[i]);
    }
    return nullptr; //if there is not a matching segment
}


//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}

