#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_set>
#include <set>
#include <iostream>
#include <iterator>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


struct Region
{
    RegionID region_id;
    Name region_name;
    std::vector< Coord > region_coord;
    std::unordered_set< StationID > stations = {};
    Region* parent_region = nullptr;
    std::unordered_set< Region* > subregions = {};
};

struct Station
{
    StationID station_id;
    Name station_name;
    Coord coordinates;
    Region* region = nullptr;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};



// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if ( (c1.x*c1.x + c1.y*c1.y) < (c2.x*c2.x + c2.y*c2.y) ) { return true; }
    else if ( (c1.x*c1.x + c1.y*c1.y) > (c2.x*c2.x + c2.y*c2.y) ) { return false; }
    else { return c1.y < c2.y; }
    /*if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }*/
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: method complexity constant
    unsigned int station_count();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Clear constant for map, linear for vector
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop goes through all values,
    //                               push_back amortized constant
    std::vector<StationID> all_stations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: adding to map complexity log n, uo map amoritised constant
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case O(n), amortised complexity constant
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case O(n), amortised complexity constant
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Stored in alphabetically ordered map, 
    //                               which is gone through with a loop
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Stored in map ordered by coordinate, 
    //                               which is gone through with a loop
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Stored in map ordered by coordinate. 
    //                               Find complexity log(n)
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: erasing from map logarithmic
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Inserting to map is logarithmic
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding and erasing from uo map amortized constant
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop going through station departures
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Unordered map find worst case linear
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop goes through all regions
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Unordered map find worst case linear
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Unordered map find worst case linear
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Unordered map find worst case linear
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Unordered map find worst case linear
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop may go through all values
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop going through all stations in subregions
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop going through all stations.
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Unordered map find worst case linear
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop in the range of regions parents
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here
    std::unordered_map< StationID, Station > stations_;
    std::map< Coord, Station* > station_coords_;
    std::map< Name, Station* > station_names_;

    std::unordered_map< StationID, std::set< std::pair< Time, TrainID >>> departures_;

    std::unordered_map< RegionID, Region > regions_;

    bool station_exists( StationID id );
    
    bool in_departures( StationID id );

    bool region_exists( RegionID id );

    double calculate_distance( Coord c1, Coord c2 );
};

#endif // DATASTRUCTURES_HH
