// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

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

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::station_count()
{
    return stations_.size();
}

void Datastructures::clear_all()
{    
    stations_.clear();

    // TODO clear regions
}

std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> allStations;

    for (auto it = stations_.begin(); it != stations_.end(); ++it)
    {
        allStations.push_back(it->first);
    }

    return allStations;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    if (stations_.find(id) != stations_.end())
    {
        return false;
    }

    Station newStation;
    newStation.name = name;
    newStation.location = xy;

    stations_.insert(std::pair(id, newStation));

    return true;
}

Name Datastructures::get_station_name(StationID id)
{
    auto stationIt = stations_.find(id);

    if (stationIt != stations_.end())
    {
        return stationIt->second.name;
    }

    return NO_NAME;
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    auto stationIt = stations_.find(id);

    if (stationIt != stations_.end())
    {
        return stationIt->second.location;
    }

    return NO_COORD;
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<std::pair<StationID, Name>> stations;
    std::vector<StationID> idOnly;

    for (auto it = stations_.begin(); it != stations_.end(); ++it)
    {
        stations.push_back(std::pair(it->first, it->second.name));
    }

    std::sort(stations.begin(), stations.end(),
              [](auto a, auto b) {return a.second > b.second;});

    for (const auto &pair : stations)
    {
        idOnly.push_back(pair.first);
    }

    return idOnly;
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{

    std::vector<std::pair<StationID, Coord>> stations;
    std::vector<StationID> idOnly;

    for (auto it = stations_.begin(); it != stations_.end(); ++it)
    {
        stations.push_back(std::pair(it->first, it->second.location));
    }

    std::sort(stations.begin(), stations.end(),
              [this](auto a, auto b) {return euclideanDistance(a.second) > euclideanDistance(b.second);});

    for (const auto &pair : stations)
    {
        idOnly.push_back(pair.first);
    }

    return idOnly;
}

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

bool Datastructures::add_departure(StationID /*stationid*/, TrainID /*trainid*/, Time /*time*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_departure()");
}

bool Datastructures::remove_departure(StationID /*stationid*/, TrainID /*trainid*/, Time /*time*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_departure()");
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID /*stationid*/, Time /*time*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("station_departures_after()");
}

bool Datastructures::add_region(RegionID /*id*/, const Name &/*name*/, std::vector<Coord> /*coords*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_region()");
}

std::vector<RegionID> Datastructures::all_regions()
{
    // Replace the line below with your implementation
    throw NotImplemented("all_regions()");
}

Name Datastructures::get_region_name(RegionID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("get_region_name()");
}

std::vector<Coord> Datastructures::get_region_coords(RegionID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("get_region_coords()");
}

bool Datastructures::add_subregion_to_region(RegionID /*id*/, RegionID /*parentid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_subregion_to_region()");
}

bool Datastructures::add_station_to_region(StationID /*id*/, RegionID /*parentid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_station_to_region()");
}

std::vector<RegionID> Datastructures::station_in_regions(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("station_in_regions()");
}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_station()");
}

RegionID Datastructures::common_parent_of_regions(RegionID /*id1*/, RegionID /*id2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("common_parent_of_regions()");
}

double Datastructures::euclideanDistance(Coord xy)
{
    return std::sqrt(xy.x*xy.x + xy.y*xy.y);
}
