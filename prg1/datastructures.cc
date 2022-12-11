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
 * @brief station_exists Checks if station exists in datastructure.
 * 
 * @param id Station to be checked.
 * @return true if station exists.
 * @return false if station doesn't exist
 */
bool Datastructures::station_exists( StationID id ) 
{
    return stations_.count( id );
}


/**
 * @brief region_exists Checks if region exists in datastructure.
 * 
 * @param id Region to be checked.
 * @return true if region exists.
 * @return false if region doesn't exist.
 */
bool Datastructures::region_exists( RegionID id )
{
    return regions_.count( id );
}


/**
 * @brief in_departures Checks if station exists in departure structure.
 * 
 * @param id Station to be checked.
 * @return true if station exists.
 * @return false if station doesn't exist.
 */
bool Datastructures::in_departures( StationID id )
{
    return departures_.count( id );
}

/**
 * @brief calculate_distance Calculates distance between two coordinates.
 * @param c1 Coordinate 1.
 * @param c2 Coordinate 2.
 * @return Distance between coordinates.
 */
double Datastructures::calculate_distance( Coord c1, Coord c2 )
{
    return sqrt( ( c1.x - c2.x ) * ( c1.x - c2.x ) + 
                 ( c1.y - c2.y ) * ( c1.y - c2.y ) );
}


/**
 * @brief Construct a new Datastructures:: Datastructures object.
 * 
 */
Datastructures::Datastructures()
{

}

/**
 * @brief Destroy the Datastructures:: Datastructures object.
 * 
 */
Datastructures::~Datastructures()
{

}

/**
 * @brief station_count Counts the amount of stations in the datastructure.
 * 
 * @return unsigned int Amount of stations.
 */
unsigned int Datastructures::station_count()
{
    return stations_.size();
}

/**
 * @brief clear_all Clears all datastructures.
 * 
 */
void Datastructures::clear_all()
{
    // Clear station structures
    station_coords_.clear();
    station_names_.clear();
    stations_.clear();

    // Clear departure structure
    departures_.clear();

    // Clear region structure
    regions_.clear();
}

/**
 * @brief all_stations Used for returning all stations in the structure.
 * 
 * @return std::vector<StationID> All stations.
 */
std::vector<StationID> Datastructures::all_stations()
{

    std::vector< StationID > stations_v;
    stations_v.reserve(station_count());

    // Go through stations and add to vector
    for ( const auto& pair : stations_ ) {
        stations_v.push_back(pair.first);
    }
    return stations_v;
}

/**
 * @brief add_station Adds given station to the datastructure.
 * 
 * @param id Station id.
 * @param name Station name.
 * @param xy Station coordinates.
 * @return true if added.
 * @return false if already in structure.
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    auto it = stations_.find( id );
    if ( it != stations_.end() ) {
        return false;
    }
    stations_[id] = Station( {id, name, xy} );
    station_coords_[xy] = ( &stations_[id] );
    station_names_[name] = ( &stations_[id] );

    return true;
}

/**
 * @brief get_station_name Get station name based on id.
 * 
 * @param id Station id .
 * @return Name Station's name.
 */
Name Datastructures::get_station_name(StationID id)
{
    auto it = stations_.find( id );
    if ( it == stations_.end() ) {
        return NO_NAME;
    }

    return it->second.station_name;
}

/**
 * @brief get_station_coordinates Get station coordinates based on id.
 * 
 * @param id Station id.
 * @return Coord Station coordinates.
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    auto it = stations_.find( id );
    if ( it == stations_.end() ) {
        return NO_COORD;
    }

    return it->second.coordinates;
}

/**
 * @brief stations_alphabetically Get all stations sorted by alphabetically by
 * name.
 * 
 * @return std::vector<StationID> Stations in an alphabetical order.
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> alph;
    alph.reserve(station_count());

    if ( stations_.empty() ) {
        return {};
    }
    for ( const auto & elem : station_names_ ) {
        alph.push_back( elem.second->station_id );
    }

    return alph;
}

/**
 * @brief stations_distance_increasing Get all stations sorted by distance 
 * to (0,0).
 * 
 * @return std::vector<StationID> Stations in order by distance.
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> dist;
    dist.reserve(station_count());

    for ( const auto & elem : station_coords_ ) {
        dist.push_back( elem.second->station_id );
    }
    return dist;
}

/**
 * @brief find_station_with_coord Looks for station with given coordinate. 
 * 
 * @param xy Coordinate to find.
 * @return StationID found station.
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    if ( station_coords_.count(xy) == 0 ) {
        return NO_STATION;
    }
    return station_coords_.at(xy)->station_id;
}

/**
 * @brief change_station_coord Change coordinate of given station.
 * 
 * @param id Station's id.
 * @param newcoord New coordinate for the station.
 * @return true if coordinate was changed.
 * @return false if failed.
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto it = stations_.find( id );
    if ( it == stations_.end() ) {
            return false;
    }
    Coord oldcoord = stations_.at( id ).coordinates;

    stations_.at( id ).coordinates = newcoord;

    station_coords_[newcoord] = &stations_[id];

    station_coords_.erase(oldcoord);
    
    return true;
}

/**
 * @brief add_departure Adds departure from the given station.
 * 
 * @param stationid Station's id.
 * @param trainid Train's id.
 * @param time Departure time.
 * @return true if add was succesfull.
 * @return false if failed.
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    if ( !station_exists( stationid ) ) {
        return false;
    } 
    auto it = departures_[stationid].insert( {time, trainid} );

    if ( !it.second ) {
        return false;
    }

    return true;

}

/**
 * @brief remove_departure Removes a departure from datastructure.
 * 
 * @param stationid Station's id.
 * @param trainid Train's id.
 * @param time Departure time.
 * @return true if remove succesfull.
 * @return false if failed.
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    if ( !in_departures( stationid ) ) {
        return false;
    }

    auto erased = departures_.at( stationid ).erase( { time, trainid } );
    if ( erased == 0 ) {
        return false;
    }
    return true;
}

/**
 * @brief station_departures_after Gives all departures from station after
 * given time.
 * 
 * @param stationid Station's id.
 * @param time Time after which look for departures.
 * @return std::vector<std::pair<Time, TrainID>> 
 * pair where first is departure time and second is trains id.
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    if ( !station_exists( stationid ) ) {
        return  { {NO_TIME, NO_TRAIN} };
    }

    std::vector<std::pair<Time, TrainID>> depart;

    for ( auto dep : departures_[stationid] ) {
        if ( dep.first >= time ) {
            depart.push_back(dep);
        }
    }

    return depart;
}

/**
 * @brief add_region Add region to datastructure.
 * 
 * @param id Region's id.
 * @param name Region's name.
 * @param coords Regions coordinates as vector.
 * @return true if add was succesfull.
 * @return false if region already in datastructure.
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    if ( region_exists( id ) ) {
        return false;
    }
    regions_[id] = Region( {id, name, coords} );
    return true;    
}

/**
 * @brief all_regions Gives all regions in the datastructure.
 * 
 * @return std::vector<RegionID> Regions in vector.
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector< RegionID > reg;

    for ( auto region : regions_ ) {
        reg.push_back(region.first);
    }

    return reg;
}

/**
 * @brief get_region_name Get region's name by id.
 * 
 * @param id Region's id.
 * @return Name Region's name.
 */
Name Datastructures::get_region_name(RegionID id)
{
    auto it = regions_.find(id);
    if ( it == regions_.end() ) {
        return NO_NAME;
    }
    return it->second.region_name;
}

/**
 * @brief get_region_coords Get region's coordinates by id.
 * 
 * @param id Region's id.
 * @return std::vector<Coord> Region's coordinates in vector.
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    auto it = regions_.find(id);
    if ( it == regions_.end() ) {
        return {NO_COORD};
    }
    return it->second.region_coord;
}

/**
 * @brief add_subregion_to_region Adds given region as parent regions subregion.
 * 
 * @param id Id of region to be added as subregion.
 * @param parentid Parent region's id.
 * @return true If subregion added succesfully.
 * @return false If station doesn't exist or already has a parent region.
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto idit = regions_.find(id);
        if ( idit == regions_.end() ) {
            return false;
        }
    auto parentit = regions_.find(parentid);
        if ( parentit == regions_.end() ) {
            return false;
        }
    if ( idit->second.parent_region ) {
        return false;
    }

    parentit->second.subregions.insert(&regions_.at(id));
    idit->second.parent_region = &regions_.at(parentid);

    return true;
}

/**
 * @brief add_station_to_region Add given station to given region.
 * 
 * @param id Station's id.
 * @param parentid Region's id.
 * @return true If station succesfully added to region
 * @return false if station or region doesn't exist or station already in region.
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto statit = stations_.find( id );
    if ( statit == stations_.end() ) {
        return false;
    }
    if ( statit->second.region ) {
        return false;
    }
    auto regit = regions_.find( parentid );
    if ( regit == regions_.end() ) {
        return false;
    }

    statit->second.region = &regions_.at( parentid );
    regit->second.stations.insert(id);

    return true;
}

/**
 * @brief station_in_regions Used to get all the regions that station is in.
 * 
 * @param id Station's id.
 * @return std::vector<RegionID> Vector of regions.
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector< RegionID > reg = {};

    auto it = stations_.find( id );
    if ( it == stations_.end() ) {
        return {NO_REGION};
    }
    if ( !it->second.region) {
        return {};
    }
    Region* parent = it->second.region;
    reg.push_back( parent->region_id );

    for ( int i = 0; i < regions_.size(); i++ ) {
        if ( !parent->parent_region ) {
            break;
        }
        parent = parent->parent_region;
        reg.push_back( parent->region_id );
    }

    return reg;
}

/**
 * @brief all_subregions_of_region Used to get all the subregions a region has.
 * 
 * @param id Region's id.
 * @return std::vector<RegionID> Vector of all subregion ids
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    auto it = regions_.find( id );
    if ( it == regions_.end() ) {
        return {NO_REGION};
    }
    if (it->second.subregions.empty()) {
        return {};
    }

    std::vector< RegionID > subregs;

    for ( auto sub : it->second.subregions ) {

        subregs.push_back(sub->region_id);
        auto subreg = it->second;

        auto regions = all_subregions_of_region(sub->region_id);

        if (!regions.empty()) {
            subregs.insert( subregs.end(), regions.begin(), regions.end() );
        }
    }

    return subregs;

}

/**
 * @brief stations_closest_to Finds 3 stations closest to given coordinate.
 * 
 * @param xy Given coordinate.
 * @return std::vector<StationID> Vector of 3 closest stations.
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    if ( station_count() <= 3 ) {
        return all_stations();
    }

    // Comparison for distances
    auto compare = [] ( std::pair<int, Station*> pair1, 
                        std::pair<int, Station*> pair2 ) {
        if ( pair1.first < pair2.first ) {
            return true;
        }
        else if ( pair1.first > pair2.first ) {
            return false;
        }
        else {
            return pair1.second->coordinates.y < pair2.second->coordinates.y;
        }
    };

    // Pairs for storing the closest stations
    std::pair<int, Station*> 
        first={NO_DISTANCE, nullptr}
        , second={NO_DISTANCE, nullptr}
        , third={NO_DISTANCE, nullptr};

    // Go through stations and check if closer than existing stations
    for ( auto station : station_coords_ ) {
        std::pair< int, Station* > new_pair =
            {calculate_distance(station.first, xy), station.second};
        if (!first.second) {
            first = new_pair;
        }
        else if (compare(new_pair, first)) {
            third = second;
            second = first;
            first = new_pair;
        }
        else if (!second.second) {
            second = new_pair;
        }
        else if (compare(new_pair, second)) {
            third = second;
            second = new_pair;
        }
        else if (!third.second) {
            third = new_pair;
        }
        else if (compare(new_pair, third)) {
            third = new_pair;
        }
    }
    // Add stations to a vector
    std::vector<StationID> stations = std::vector<StationID>({
        first.second->station_id,
        second.second->station_id,
        third.second->station_id
            });
    return stations;

}

/**
 * @brief remove_station Removes station from all datastructures.
 * 
 * @param id ID of station to be removed.
 * @return true If station succesfully removed.
 * @return false If station doesn't exist.
 */
bool Datastructures::remove_station(StationID id)
{
    // Check if exists
    auto stationit = stations_.find( id );
    if ( stationit == stations_.end() ) {
        return false;
    }

    // If station in any region, remove from there
    if ( stationit->second.region ) {
        regions_.at( stationit->second.region->region_id ).stations.erase( id );
    }

    // Remove from departures
    departures_.erase( id );

    // Remove from station maps
    station_coords_.erase(stations_.at(id).coordinates);
    station_names_.erase(stations_.at(id).station_name);
    stations_.erase(id);

    return true;

}

/**
 * @brief common_parent_of_regions Find the first common parent region of 
 * regions.
 * 
 * @param id1 ID of region 1.
 * @param id2 ID of region 2.
 * @return RegionID The first common parent region's ID.
 * @return NO_REGION If no common parents.
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    std::unordered_set< RegionID > parents = {};

    // Check if parent 1 exists
    auto reg1 = regions_.find( id1 );
    if ( reg1 == regions_.end()) {
        return NO_REGION;
    }
    if ( !reg1->second.parent_region ) {
        return NO_REGION;
    }
    auto parent1 = reg1->second.parent_region;

    // Check if parent 2 exists
    auto reg2 = regions_.find( id2 );
    if ( reg2 == regions_.end()) {
        return NO_REGION;
    }
    if ( !reg2->second.parent_region) {
        return NO_REGION;
    }
    auto parent2 = reg2->second.parent_region;

    // Add all region1's parents to container
    while ( parent1 ) {
        parents.insert(parent1->region_id);
        parent1 = parent1->parent_region;
    }
    // Check if any of region2's parents are already in the container
    while (parent2) {
        if ( parents.count(parent2->region_id) == 1) {
            return parent2->region_id;
        }
        parent2 = parent2->parent_region;
    }

    return NO_REGION;
}
