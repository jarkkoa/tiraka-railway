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

/*!
 * \brief Datastructures::station_count counts the amount of staions.
 * \return a count of stations.
 */


unsigned int Datastructures::station_count()
{
    return _stations.size();
}

/*!
 * \brief Datastructures::clear_all removes all the data stored in the application.
 */

void Datastructures::clear_all()
{
    _stations.clear();
    _stationNames.clear();
    _stationCoordinates.clear();
    _regions.clear();
    _regionNames.clear();
}

/*!
 * \brief Datastructures::all_stations gets all the stations.
 * \return vector of all the available station ids.
 */

std::vector<StationID> Datastructures::all_stations()
{
    std::vector<std::string> stationIDs;
    for (auto const& element : _stations) {
      stationIDs.push_back(element.first);
    }
    return stationIDs;
}

/*!
 * \brief Datastructures::add_station adds station to the application
 * \param id of the station
 * \param name of the station
 * \param xy coordinate for the station
 * \return boolean if the creation was succesfull.
 */

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    if (_stations.find(id) != _stations.end()) {
        return false;
    } else {
        std::pair<Name, StationID> stationNameID(name, id);
        std::vector<std::pair<Time, TrainID>> departures;
        RegionID regionid = 0;

        _stationNames.insert(stationNameID);
        _stationCoordinates.insert({xy, id});

        _stations.insert({id, Station {id, name, xy, regionid, departures}});
        return true;
    }
}

/*!
 * \brief Datastructures::get_station_name gets the name of one station
 * \param id of the station
 * \return name of the station
 */

Name Datastructures::get_station_name(StationID id)
{
    auto it = _stations.find(id);
    if (it == _stations.end()) {
        return NO_NAME;
    } else {
        Name name = it->second.name;
        return name;
    }
}

/*!
 * \brief Datastructures::get_station_coordinates gets the coordinates of one station
 * \param id of the station
 * \return coordinates of the station
 */

Coord Datastructures::get_station_coordinates(StationID id)
{
    auto it = _stations.find(id);
    if (it == _stations.end()) {
        return NO_COORD;
    } else {
        Coord coordinates = it->second.location;
        return coordinates;
    }
}

/*!
 * \brief Datastructures::stations_alphabetically gives all stations in the application alphabetically ordered
 * \return vector of station ids
 */

std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> stations;
    for (auto itr : _stationNames) {
        stations.push_back(itr.second);
    }
    return stations;
}

/*!
 * \brief Datastructures::stations_distance_increasing gives all the stations in the application ordered by their distance from origo
 * \return vector of station ids.
 */

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> stations;
    for (auto itr : _stationCoordinates) {
        stations.push_back(itr.second);
    }
    return stations;
}

/*!
 * \brief Datastructures::find_station_with_coord finds a station with given coordinate.
 * \param xy coordinate of the station
 * \return id of the station.
 */

StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto itr = _stationCoordinates.find(xy);
    if (itr == _stationCoordinates.end()) {
        return NO_STATION;
    }
    StationID station = itr->second;
    return station;
}

/*!
 * \brief Datastructures::change_station_coord changes a coordinate of given station.
 * \param id of the station
 * \param newcoord is the new coordinate of the station
 * \return boolean if the change was succesfull.
 */

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto itr = _stations.find(id);
    if (itr == _stations.end()) {
        return false;
    }

    Coord oldcoord = itr->second.location;

    itr->second.location = newcoord;

    _stationCoordinates.erase(_stationCoordinates.find(oldcoord)->first);
    _stationCoordinates.insert({newcoord, id});

    return true;
}

/*!
 * \brief Datastructures::add_departure add one train departure to one station
 * \param stationid station id where the departure belongs
 * \param trainid which train the departure concerns
 * \param time which time the departure is
 * \return boolean if the adding was successfull
 */

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto itr = _stations.find(stationid);
    std::pair<Time, TrainID> newDeparture = std::make_pair(time, trainid);
    if (itr == _stations.end() || find(itr->second.departures.begin(), itr->second.departures.end(), newDeparture) != itr->second.departures.end() ) {
        return false;
    }

    itr->second.departures.push_back(std::make_pair(time, trainid));
    return true;
}

/*!
 * \brief Datastructures::remove_departure remove one departure
 * \param stationid identifier of the station
 * \param trainid identifier of the train
 * \param time the time when the departure is scheduled
 * \return boolean if the removal was successfull or not.
 */

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto itr = _stations.find(stationid);
    std::pair<Time, TrainID> departure = std::make_pair(time, trainid);

    if (itr == _stations.end() || find(itr->second.departures.begin(), itr->second.departures.end(), departure) == itr->second.departures.end() ) {
        return false;
    }

    Station& station = itr->second;

    for (auto i = station.departures.begin(); i != station.departures.end(); ++i)
    {
        if(i->first == time && i->second == trainid) {
            station.departures.erase(i);
            return true;
        }
    }

    return false;
}

/*!
 * \brief Datastructures::station_departures_after get all departures from certain station after certain time.
 * \param stationid identifier of the station.
 * \param time the time when the departure is scheduled.
 * \return vector of all the time-train pairs who fulfill the requirement.
 */

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    auto itr = _stations.find(stationid);
    std::vector<std::pair<Time, TrainID>> departures;

    if (itr == _stations.end()) {
        departures.push_back({NO_TIME, NO_TRAIN});
        return departures;
    }

    std::vector<std::pair<Time, TrainID>> allDepartures = _stations.find(stationid)->second.departures;

    for (auto i = allDepartures.begin(); i != allDepartures.end(); ++i)
    {
        if(i->first >= time) {
            departures.push_back(*i);
        }
    }

    return departures;
}

/*!
 * \brief Datastructures::add_region add region to the application.
 * \param id identifier of the region
 * \param name of the region
 * \param coords vector of coordinates for the region (identifies the borders of the region).
 * \return boolean if the addition was succesfull or not.
 */

bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    auto itr = _regions.find(id);
    if (itr != _regions.end()) {
        return false;
    }

    std::pair<Name, RegionID> regionNameID(name, id);
    std::vector<StationID> stations;
    std::vector<RegionID> subregions;
    Region* parentregion = nullptr;

    _regions.insert({id, Region {id, name, coords, stations, subregions, parentregion}});
    _regionNames.insert(regionNameID);

    return true;
}

/*!
 * \brief Datastructures::all_regions gives all the regions stored in the application.
 * \return vector of all the region ids.
 */

std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> regionIDs;
    for(auto it = _regions.begin(); it != _regions.end(); ++it) {
        regionIDs.push_back(it->first);
    }
    return regionIDs;
}

/*!
 * \brief Datastructures::get_region_name gives a name of certain region.
 * \param id identifier of the region
 * \return name of the region
 */

Name Datastructures::get_region_name(RegionID id)
{
    auto itr = _regions.find(id);
    if (itr == _regions.end()) {
        return NO_NAME;
    }

    Name name = itr->second.name;
    return name;
}

/*!
 * \brief Datastructures::get_region_coords gets coordinates of certain region
 * \param id identifier of the region
 * \return vector of coordinates
 */

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    std::vector<Coord> coords;

    auto itr = _regions.find(id);
    if (itr == _regions.end()) {
        std::vector<Coord> coords{NO_COORD};
        return coords;
    }
    return itr->second.coords;
}

/*!
 * \brief Datastructures::add_subregion_to_region add one region to a subregion for another
 * \param id identifier of the subregion
 * \param parentid identifier of the parent region
 * \return boolean if the addition was succesfull
 */

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto regionItr = _regions.find(id);
    auto parentRegionItr = _regions.find(parentid);

    if (regionItr == _regions.end() || parentRegionItr == _regions.end() || regionItr->second.parentregion != nullptr) {
        return false;
    }

    regionItr->second.parentregion = &parentRegionItr->second;
    parentRegionItr->second.subregions.push_back(id);
    return true;
}

/*!
 * \brief Datastructures::add_station_to_region add information that certain station belongs to certain region
 * \param id identifier of the station
 * \param parentid identifier of the region
 * \return boolean if the addiotion was successfull
 */

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto regionItr = _regions.find(parentid);
    auto stationItr = _stations.find(id);
    if (regionItr == _regions.end() || stationItr == _stations.end() || stationItr->second.region != 0) {
        return false;
    }

    stationItr->second.region = parentid;
    regionItr->second.stations.push_back(id);

    return true;
}

/*!
 * \brief Datastructures::station_in_regions get all the regions where certain station belongs to
 * \param id identifier of the station
 * \return vector of all the region ids
 */

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    auto itr = _stations.find(id);
    std::vector<RegionID> regions;

    if (itr == _stations.end()) {
        regions.push_back(NO_REGION);
        return regions;
    }

    RegionID regionid = _stations.find(id)->second.region;

    if (regionid == 0) {
        return regions;
    } else {
        regions.push_back(regionid);
        Region* regionptr = _regions.find(regionid)->second.parentregion;
        while (regionptr != nullptr) {
            regions.push_back(regionptr->id);
            regionptr = regionptr->parentregion;
        }
        return regions;
    }
}

/*!
 * \brief Datastructures::get_subregions subregions of certain region (used as recursive function)
 * \param id identifier of the region
 * \return vector of the subregion ids
 */

std::vector<RegionID> Datastructures::get_subregions(RegionID id)
{
    auto itr = _regions.find(id);
    std::vector<RegionID> regions;

    if (itr->second.subregions.size() == 0) {
        return regions;
    }

    std::vector<RegionID> subregions = _regions.find(id)->second.subregions;

    regions.insert(regions.end(), subregions.begin(), subregions.end());
    for (auto const& regionid : itr->second.subregions) {
        std::vector<RegionID> subregions = get_subregions(regionid);
        regions.insert(regions.end(), subregions.begin(), subregions.end());
    }
    return regions;
}

/*!
 * \brief Datastructures::all_subregions_of_region get all the subregions of certain region
 * \param id identifier of the region
 * \return vector of all the subregion ids
 */

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    auto itr = _regions.find(id);
    std::vector<RegionID> regions;

    if (itr == _regions.end()) {
        regions.push_back(NO_REGION);
        return regions;
    }

    std::vector<RegionID> subregions = get_subregions(id);

    regions.insert(regions.end(), subregions.begin(), subregions.end());

    return regions;
}

/*!
 * \brief Datastructures::stations_closest_to get station closest to certain coordinate
 * \param xy coordinate where the distance is measured
 * \return vector of station ids in ascending order based on the distance to the coordinate
 */

std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<StationID> stations;
    std::vector<std::pair<float, TrainID>> distances;

    if(_stationCoordinates.size() == 0) {
        return stations;
    }

    int vectorSize = 0;

    for(auto it = _stationCoordinates.begin(); it != _stationCoordinates.end(); ++it) {
        float distance = sqrt(pow(xy.x - it->first.x, 2) + pow(xy.y - it->first.y, 2));
        distances.push_back(make_pair(distance, it->second));
        ++vectorSize;
    }

    std::sort(distances.begin(), distances.end(),
              [] (const auto &x, const auto &y) { return x.first < y.first; });

    std::transform(distances.begin(), distances.end(), std::back_inserter(stations), [](auto const& pair) -> auto const& { return pair.second; });

    if (vectorSize > 3) {
        stations.resize(3);
    }

    return stations;
}

/*!
 * \brief Datastructures::remove_station remove certain station from the application
 * \param id identifier of the station
 * \return boolean indicating if the removal was successfull
 */

bool Datastructures::remove_station(StationID id)
{
    auto itr = _stations.find(id);
    if (itr == _stations.end()) {
        return false;
    }

    RegionID regionid = itr->second.region;

    if (regionid != 0) {
        std::vector<StationID>& regionstations = _regions.find(regionid)->second.stations;
        for (auto i = regionstations.begin(); i != regionstations.end(); ++i) {
            if (*i == id) {
                regionstations.erase(i);
                break;
            }
        }
    }

    for (auto i = _stationCoordinates.begin(); i != _stationCoordinates.end(); ++i) {
        if (i->second == id) {
            _stationCoordinates.erase(i);
            break;
        }
    }

    for (auto i = _stationNames.begin(); i != _stationNames.end(); ++i) {
        if (i->second == id) {
            _stationNames.erase(i);
            break;
        }
    }

    _stations.erase(itr);
    return true;
}

/*!
 * \brief Datastructures::common_parent_of_regions finds the closest parent between to regions
 * \param id1 identifier of the first region
 * \param id2 identifier of the second region
 * \return id of the common parent region
 */

RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    auto itr1 = _regions.find(id1);
    auto itr2 = _regions.find(id2);
    if (itr1 == _regions.end() || itr2 == _regions.end()) {
        return NO_REGION;
    }

    Region* regionptr1 = itr1->second.parentregion;
    Region* regionptr2 = itr2->second.parentregion;

    while (regionptr1 != nullptr) {
        while (regionptr2 != nullptr) {
            if (regionptr1 == regionptr2) {
                return regionptr1->id;
            }
            regionptr2 = regionptr2->parentregion;
        }
        regionptr1 = regionptr1->parentregion;
        regionptr2 = itr2->second.parentregion;
    }

    return NO_REGION;
}
