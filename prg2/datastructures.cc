// Datastructures.cc
//
// Student name: Jarkko Ahvenniemi
// Student email: jarkko.ahvenniemi@tuni.fi
// Student number: H292105

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

/**
 * @brief Datastructures::Datastructures Datastructures constructor
 */
Datastructures::Datastructures()
{
    // Write any initialization you need here
}

/**
 * @brief Datastructures::~Datastructures Datastructures destructor
 */
Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

/**
 * @brief Datastructures::station_count Returns the total station count
 * @return Total station count
 */
unsigned int Datastructures::station_count()
{
    return stations_.size();
}

/**
 * @brief Datastructures::clear_all Clears the data structures
 */
void Datastructures::clear_all()
{    
    stations_.clear();
    regions_.clear();
}

/**
 * @brief Datastructures::all_stations Creates a list of every existing station
 * @return A vector containing the stations
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> allStations;
    allStations.reserve(stations_.size());

    for (auto it = stations_.begin(); it != stations_.end(); ++it)
    {
        allStations.push_back(it->first);
    }

    return allStations;
}

/**
 * @brief Datastructures::add_station Creates a new station
 * @param id Station ID
 * @param name Station name
 * @param xy Station coordinates
 * @return Was the addition successful?
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    if (stations_.find(id) != stations_.end())
    {
        return false;
    }

    Station newStation;
    newStation.name = name;
    newStation.location = xy;
    newStation.region = NO_REGION;

    stations_.insert(std::make_pair(id, newStation));

    return true;
}

/**
 * @brief Datastructures::get_station_name Gets the name of a given station
 * @param id Station ID
 * @return Station name
 */
Name Datastructures::get_station_name(StationID id)
{
    auto stationIt = stations_.find(id);

    if (stationIt != stations_.end())
    {
        return stationIt->second.name;
    }

    return NO_NAME;
}

/**
 * @brief Datastructures::get_station_coordinates Gets the coordinates of a given station
 * @param id Station ID
 * @return Station coordinates
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    auto stationIt = stations_.find(id);

    if (stationIt != stations_.end())
    {
        return stationIt->second.location;
    }

    return NO_COORD;
}

/**
 * @brief Datastructures::stations_alphabetically Sorts the existing stations by name
 * @return Sorted vector containing the stations
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<std::pair<StationID, Name>> stations;
    std::vector<StationID> idOnly;

    stations.reserve(stations_.size());
    idOnly.reserve(stations_.size());

    for (auto it = stations_.begin(); it != stations_.end(); ++it)
    {
        stations.push_back(std::make_pair(it->first, it->second.name));
    }

    std::sort(stations.begin(), stations.end(),
              [](auto a, auto b) {return a.second < b.second;});

    for (const auto &pair : stations)
    {
        idOnly.push_back(pair.first);
    }

    return idOnly;
}

/**
 * @brief Datastructures::stations_distance_increasing Sorts the existing stations by their coordinates
 * @return Sorted vector containing the stations
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{

    std::vector<std::pair<StationID, Coord>> stations;
    std::vector<StationID> idOnly;

    stations.reserve(stations_.size());
    idOnly.reserve(stations_.size());

    for (auto it = stations_.begin(); it != stations_.end(); ++it)
    {
        stations.push_back(std::make_pair(it->first, it->second.location));
    }

    std::sort(stations.begin(), stations.end(),
              [this](auto a, auto b) {return euclideanDistance(a.second) < euclideanDistance(b.second);});

    for (const auto &pair : stations)
    {
        idOnly.push_back(pair.first);
    }

    return idOnly;
}

/**
 * @brief Datastructures::find_station_with_coord Searches the coordinates for a station
 * @param xy Search coordinates
 * @return Search result. If not found, NO_STATION is returned.
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto stationIt = std::find_if(stations_.begin(), stations_.end(),
                                  [xy](std::pair<StationID, Station> pair)
                                  {
                                      return pair.second.location == xy;
                                  });

    if (stationIt != stations_.end())
    {
        return stationIt->first; // StationID
    }

    return NO_STATION;
}

/**
 * @brief Datastructures::change_station_coord Changes the coordinates of a given station
 * @param id Station ID
 * @param newcoord New coordinates
 * @return Was the change successful?
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto stationIt = stations_.find(id);

    if (stationIt == stations_.end())
    {
        return false;
    }

    stationIt->second.location = newcoord;
    return true;
}

/**
 * @brief Datastructures::add_departure Creates a new departure
 * @param stationid Station ID
 * @param trainid ID of the departing train
 * @param time Time of departure
 * @return Was the addition successful?
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{   
    Station* station = findStation(stationid);

    if (station == nullptr)
    {
        return false;
    }

    // Add a departure time if it doesn't exist
    std::set<TrainID> departingTrains;
    station->departures.insert(std::make_pair(time, departingTrains));

    // .second = bool: insert happened
    return station->departures.at(time).insert(trainid).second;
}

/**
 * @brief Datastructures::remove_departure Removes a departure
 * @param stationid Departure Station ID
 * @param trainid ID of the departing train
 * @param time Time of departure
 * @return Was the removal successful?
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    Station* station = findStation(stationid);

    if (station == nullptr)
    {
        return false; // Station not found
    }

    auto departureIt = station->departures.find(time);

    if (departureIt == station->departures.end())
    {
        return false; // Departure time not found
    }

    auto trainIt = station->departures.at(time).find(trainid);

    if (trainIt == station->departures.at(time).end())
    {
        return false; // Train not found
    }

    station->departures.at(time).erase(trainid);
    return true; // Removal successful
}

/**
 * @brief Datastructures::station_departures_after Finds all departures at a given station during or after a given time
 * @param stationid Station ID
 * @param time Departure time
 * @return A vector containing the departures
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    Station* station = findStation(stationid);
    std::vector<std::pair<Time, TrainID>> departures;

    if (station == nullptr)
    {
        departures.push_back(std::make_pair(NO_TIME, NO_TRAIN));
        return departures;
    }

    // Find the position of the first eligible time
    auto depIt = station->departures.begin();
    while (depIt != station->departures.end())
    {
        if (depIt->first >= time)
        {
            break;
        }
        depIt++;
    }

    // Return all departures later than 'time'
    for (auto it = depIt; it != station->departures.end(); ++it)
    {
        for (const auto &train : it->second)
        {
            departures.push_back(std::make_pair(it->first, train));
        }
    }

    return departures;
}

/**
 * @brief Datastructures::add_region Creates a new region
 * @param id Region ID
 * @param name Region name
 * @param coords Coordinates of the region vertices
 * @return Was the addition successful?
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    auto regionIt = regions_.find(id);
    if (regionIt != regions_.end()) // If found, don't do anything
    {
        return false;
    }

    Region newRegion;

    newRegion.name = name;
    newRegion.vertices = coords;
    newRegion.parentRegion = NO_REGION;

    regions_.insert(std::make_pair(id, newRegion));
    return true;
}

/**
 * @brief Datastructures::all_regions Compiles a vector of all existing regions
 * @return A vector containing every existing region
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> allRegions;

    for (const auto &region : regions_)
    {
        allRegions.push_back(region.first);
    }

    return allRegions;
}

/**
 * @brief Datastructures::get_region_name Gets the name of a given region
 * @param id Region ID
 * @return Name of the region
 */
Name Datastructures::get_region_name(RegionID id)
{
    auto regionIt = regions_.find(id);
    if (regionIt == regions_.end())
    {
        return NO_NAME;
    }
    else
    {
        return regionIt->second.name;
    }
}

/**
 * @brief Datastructures::get_region_coords Gets the vertex coordinates of a given region.
 * @param id Region ID
 * @return Vector containing the vertices
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    std::vector<Coord> vertices;
    auto regionIt = regions_.find(id);
    if (regionIt == regions_.end())
    {
        vertices.push_back(NO_COORD);
        return vertices;
    }
    else
    {
        return regionIt->second.vertices;
    }
}

/**
 * @brief Datastructures::add_subregion_to_region Adds one region as the subregion of another region
 * @param id Subregion ID
 * @param parentid Parent region ID
 * @return Was the addition successful?
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto regionIt = regions_.find(id);
    auto parentRegionIt = regions_.find(parentid);

    if (regionIt == regions_.end() || parentRegionIt == regions_.end()
            || regionIt->second.parentRegion != NO_REGION)
    {
        return false;
    }

    regionIt->second.parentRegion = parentid;
    parentRegionIt->second.subregions.push_back(id);
    return true;
}

/**
 * @brief Datastructures::add_station_to_region Adds the given station to the given region.
 * @param id Station ID
 * @param parentid Region ID
 * @return Was the addition successful?
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto stationIt = stations_.find(id);
    auto parentRegionIt = regions_.find(parentid);

    if (stationIt == stations_.end() || parentRegionIt == regions_.end()
            || stationIt->second.region != NO_REGION)
    {
        return false;
    }

    stationIt->second.region = parentid;
    return true;
}

/**
 * @brief Datastructures::station_in_regions Finds every region that the given station belongs to, directly or indirectly
 * @param id Station ID
 * @return A vector containing the parent regions of the station
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> parents;
    Station* station = findStation(id);

    if (station == nullptr)
    {
        parents.push_back(NO_REGION);
        return parents;
    }

    parents.push_back(station->region);
    getParents(station->region, parents);

    return parents;
}

/**
 * @brief Datastructures::all_subregions_of_region Finds every subregion belonging to the given region ID
 * @param id Region ID
 * @return Vector containing every subregion
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::vector<RegionID> subregions;

    auto regionIt = regions_.find(id);
    if (regionIt == regions_.end() || regionIt->second.vertices.empty())
    {
        subregions.push_back(NO_REGION);
        return subregions;
    }

    getChildren(id, subregions); // Saves children directly in the 'subregions' vector
    return subregions;
}

/**
 * @brief Datastructures::stations_closest_to Finds three stations closest to the given coordinates.
 * @param xy Coordinates
 * @return Vector containing the nearest stations
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<StationID> closeStations;
    auto stationIt = stations_.begin();
    closeStations.reserve(3);

    // For small station counts, simpy sort them by the distance
    if (stations_.size() <= 3)
    {
        std::vector<std::pair<StationID, Coord>> allStations;

        for (const auto &station : stations_)
        {
            allStations.push_back(std::make_pair(station.first, station.second.location));
        }

        std::sort(allStations.begin(), allStations.end(),
                  [this, xy](auto a, auto b)
                  {
                      return euclideanDistance2(a.second, xy) < euclideanDistance2(b.second, xy);
                  });

        for (const auto &station : allStations)
        {
            closeStations.push_back(station.first);
        }
        return closeStations;
    }


    /* For larger amount of stations, iterate through the list and keep track of
     * the top 3 closest stations */

    double current;

    // Pick initial values
    closeStations.push_back(NO_STATION);
    closeStations.push_back(NO_STATION);
    closeStations.push_back(NO_STATION);

    // Keep track of the distances to reduce the need for recalculation
    double first, second, third;
    first = second = third = __DBL_MAX__;

    while (stationIt != stations_.end())
    {
        current = euclideanDistance2(stationIt->second.location, xy);

        if (current <= third)
        {
            third = current;
            closeStations.at(2) = stationIt->first;
        }

        if (current <= second)
        {
            third = second;
            second = current;

            closeStations.at(2) = closeStations.at(1);
            closeStations.at(1) = stationIt->first;
        }

        if (current <= first)
        {
            second = first;
            first = current;

            closeStations.at(1) = closeStations.at(0);
            closeStations.at(0) = stationIt->first;
        }

        stationIt++;
    }

    return closeStations;
}

/**
 * @brief Datastructures::remove_station Removes a single station with the given ID
 * @param id Station ID
 * @return Boolean value which states whether the removal was successful.
 */
bool Datastructures::remove_station(StationID id)
{
    auto stationIt = stations_.find(id);

    if (stationIt == stations_.end())
    {
        return false;
    }

    stations_.erase(stationIt);
    return true;
}

/**
 * @brief Datastructures::common_parent_of_regions Finds the nearest common parent region of two regions
 * @param id1 Region 1 ID
 * @param id2 Region 2 ID
 * @return ID of the parent region. Returns NO_REGION if neither a common parent or any of the argument regions are not found.
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    if (regions_.find(id1) == regions_.end() ||
            regions_.find(id2) == regions_.end())
    {
        return NO_REGION;
    }

    std::vector<RegionID> region1Parents, region2Parents, commonParents;

    // Get the parents of both regions
    getParents(id1, region1Parents);
    getParents(id2, region2Parents);

    qDebug() << region1Parents << region2Parents;

    // Find the first common element
    auto firstCommonIt = std::find_first_of(region1Parents.begin(),
                                            region1Parents.end(),
                                            region2Parents.begin(),
                                            region2Parents.end());

    if (firstCommonIt == region1Parents.end())
    {
        // No common element found
        return NO_REGION;
    }

    return *firstCommonIt;
}

/**
 * @brief Datastructures::euclideanDistance Calculates the "normalized" euclidean distance of a single coordinate point.
 * @param xy Coordinates
 * @return Distance
 */
double Datastructures::euclideanDistance(Coord xy)
{
    return std::sqrt(xy.x*xy.x + xy.y*xy.y);
}

/**
 * @brief Datastructures::euclideanDistance2 Calculates the euclidean distance between 2 coordinates
 * @param xy1 Coordinate 1
 * @param xy2 Coordinate 2
 * @return Distance between the coordinates
 */
double Datastructures::euclideanDistance2(Coord xy1, Coord xy2)
{
    return std::sqrt((xy1.x-xy2.x)*(xy1.x-xy2.x) + (xy1.y-xy2.y)*(xy1.y-xy2.y));
}

/**
 * @brief Datastructures::findStation Finds the station with the given station ID.
 * @param id Station ID.
 * @return Pointer to the station, nullptr if the station is not found.
 */
Datastructures::Station* Datastructures::findStation(StationID id)
{
    auto stationIt = stations_.find(id);

    if (stationIt == stations_.end())
    {
        return nullptr;
    }

    return &stationIt->second;
}

/**
 * @brief Datastructures::getParents Finds every parent of 'child' and adds them to the 'parents' vector
 * @param child Child region
 * @param parents Vector containing every parent region of 'child'
 */
void Datastructures::getParents(RegionID child, std::vector<RegionID> &parents)
{
    auto childIt = regions_.find(child);
    if (childIt == regions_.end() || childIt->second.parentRegion == NO_REGION)
    {
        return;
    }

    RegionID parent = childIt->second.parentRegion;

    parents.push_back(parent);
    getParents(parent, parents);
}

/**
 * @brief Datastructures::getChildren Finds every subregion belonging to 'parent'
 * @param parent The parent region
 * @param children Vector containing every subregion
 */
void Datastructures::getChildren(RegionID parent, std::vector<RegionID> &children)
{
    auto parentIt = regions_.find(parent);
    if (parentIt == regions_.end() || parentIt->second.subregions.empty())
    {
        return;
    }

    for (const auto &childRegion : parentIt->second.subregions)
    {
        children.push_back(childRegion);
        getChildren(childRegion, children);
    }
}

bool Datastructures::add_train(TrainID /*trainid*/, std::vector<std::pair<StationID, Time> > /*stationtimes*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_train()");
}

std::vector<StationID> Datastructures::next_stations_from(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("next_stations_from()");
}

std::vector<StationID> Datastructures::train_stations_from(StationID /*stationid*/, TrainID /*trainid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("train_stations_from()");
}

void Datastructures::clear_trains()
{
    // Replace the line below with your implementation
    throw NotImplemented("clear_trains()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_any()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_least_stations()");
}

std::vector<StationID> Datastructures::route_with_cycle(StationID /*fromid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_with_cycle()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_shortest_distance()");
}

std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID /*fromid*/, StationID /*toid*/, Time /*starttime*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_earliest_arrival()");
}

