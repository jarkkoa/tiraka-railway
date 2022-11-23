// Datastructures.hh
//
// Student name: Jarkko Ahvenniemi
// Student email: jarkko.ahvenniemi@tuni.fi
// Student number: H292105

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <list>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <QDebug>

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
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
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
    // Short rationale for estimate: Unordered map size() time complexity
    unsigned int station_count();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Two consequtive linear operations
    void clear_all();

    // Estimate of performance: O(N)
    // Short rationale for estimate: The heaviest operation is a linear iteration through all map elements
    std::vector<StationID> all_stations();

    // Estimate of performance: Average: O(1) Worst-case: O(N)
    // Short rationale for estimate: Average and worst-case complexities of unordered_map find() and insert().
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: Average: O(1) Worst-case: O(N)
    // Short rationale for estimate: Average and worst-case complexity of unordered_map find().
    Name get_station_name(StationID id);

    // Estimate of performance: Average: O(1) Worst-case: O(N)
    // Short rationale for estimate: Average and worst-case complexity of unordered_map find().
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(N*log(N))
    // Short rationale for estimate: Time complexity of the heaviest operation, std::sort()
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(N*log(N))
    // Short rationale for estimate: Time complexity of the heaviest operation, std::sort()
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Time complexity of find_if()
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: Average: O(1), Worst-case: O(N)
    // Short rationale for estimate: Time complexity of unordered_map::find()
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: Average: O(log(N)), Worst-case: O(N)
    // Short rationale for estimate: Average: map::at() logarithmic, worst-case: map::insert() linear
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: Average: O(log(N)), worst-case: O(N)
    // Short rationale for estimate: Average: map::at() logarithmic, worst-case: map::erase() & map::find() linear
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: Average: O(N), worst-caseO(N^2)
    // Short rationale for estimate: Iterating through departure times and trains departing at those times, perftest seems to scale linearly
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: Average: O(1), worst-case: O(N)
    // Short rationale for estimate: Average and worst-case time complexities of unordered_map::find() and ::insert()
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Iterating through an unordered map
    std::vector<RegionID> all_regions();

    // Estimate of performance: Average O(1), worst: O(N)
    // Short rationale for estimate: unordered_map::find()
    Name get_region_name(RegionID id);

    // Estimate of performance: Average O(1), worst: O(N)
    // Short rationale for estimate: unordered_map::find()
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: Average O(1), worst: O(N)
    // Short rationale for estimate: unordered_map::find()
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: Average O(1), worst: O(N)
    // Short rationale for estimate: unordered_map::find()
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: Average O(1), worst: O(N)
    // Short rationale for estimate: Recursive function getParents() calls unordered_map::find() and vector::push_back()
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: Average: O(1), worst O(N^2)
    // Short rationale for estimate: Recursive unordered_map::find() calls
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(N*log(N))
    // Short rationale for estimate: std::sort()
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: Average O(1), worst: O(N)
    // Short rationale for estimate: unordered_map::find(), unordered_map::erase()
    bool remove_station(StationID id);

    // Estimate of performance: Average: O(N), worst-case: O(N^2)
    // Short rationale for estimate: At most N1 * N2 comparisons (std::find_first_of), perftest seems to scale linearly
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here

    struct Region
    {
        Name name;
        std::vector<Coord> vertices;
        RegionID parentRegion;
        std::list<RegionID> subregions;
    };

    struct Station
    {
        Name name;
        Coord location;
        RegionID region;
        std::map<Time, std::set<TrainID>> departures;
    };

    double euclideanDistance(Coord xy);
    double euclideanDistance2(Coord xy1, Coord xy2);

    /**
     * @brief findStation Finds the station with the given station ID.
     * @param id Station ID.
     * @return Pointer to the station, nullptr if the station is not found.
     */
    Station* findStation(StationID id); // avg: O(1), worst O(N) unordered_map::find()

    void getParents(RegionID child, std::vector<RegionID> &parents);
    void getChildren(RegionID parent, std::vector<RegionID> &children);

    std::unordered_map<StationID, Station> stations_;
    std::unordered_map<RegionID, Region> regions_;

};

#endif // DATASTRUCTURES_HH
