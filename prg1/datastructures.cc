// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <QDebug>
#include <algorithm>
#include <iostream>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end - start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range - 1)(rand_engine);

    return static_cast<Type>(start + num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
}

Datastructures::~Datastructures()
{
}

int Datastructures::place_count()
{
    return placeMap.size();
}

void Datastructures::clear_all()
{
    // clear unordered_map of places and both vectors
    placeMap.clear();
    placeidsName_.clear();
    placeidsCoord_.clear();

    // clear unordered_map of areas and a vector
    areaMap.clear();
    areaids.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    // return vector with all place ids, order of id's doesn't matter
    return placeidsName_;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    if (placeMap.find(id) != placeMap.end()) {
        return false;
    } else {

        // create place
        Place place;
        place.name = name;
        place.type = type;
        place.coord = xy;

        // insert created place to placeMap with id as key and place as value
        // also push the id to two different vectors
        placeMap.insert(std::make_pair(id, place));
        placeidsName_.push_back(id);
        placeidsCoord_.push_back(id);

        // both vectors placeidsName_ and placeidsCoord_ are now not sorted
        sortedCoord_ = false;
        sortedAlpha_ = false;
        return true;
    }
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    if (placeMap.find(id) != placeMap.end()) {
        return { placeMap.at(id).name, placeMap.at(id).type };
    }
    return { NO_NAME, PlaceType::NO_TYPE };
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    if (placeMap.find(id) != placeMap.end()) {
        return placeMap.at(id).coord;
    } else {
        return NO_COORD;
    }
}

bool Datastructures::add_area(AreaID id, const Name& name, std::vector<Coord> coords)
{
    if (areaMap.find(id) != areaMap.end()) {
        return false;
    } else {
        // create area
        Area area;
        area.name = name;
        area.coords = coords;

        // insert area to areaMap with id as key and area as value
        // push id in to a vector
        areaMap.insert(std::make_pair(id, area));
        areaids.push_back(id);
        return true;
    }
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    if (areaMap.find(id) != areaMap.end()) {
        return areaMap.at(id).name;
    } else {
        return NO_NAME;
    }
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    if (areaMap.find(id) != areaMap.end()) {
        return areaMap.at(id).coords;
    } else {
        return { NO_COORD };
    }
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}

std::vector<PlaceID> Datastructures::places_alphabetically()
{
    if (!sortedAlpha_) {

        sort(placeidsName_.begin(), placeidsName_.end(), [=](PlaceID a, PlaceID b) { return placeMap.at(a).name < placeMap.at(b).name; });
        sortedAlpha_ = true;
    }
    return placeidsName_;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{

    if (!sortedCoord_) {

        // sort using implemented coordCompare function
        sort(placeidsCoord_.begin(), placeidsCoord_.end(), [=](PlaceID a, PlaceID b) { return coordCompare(a, b); });
        sortedCoord_ = true;
    }
    return placeidsCoord_;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    std::vector<PlaceID> places;
    for (auto& place : placeMap) {
        if (place.second.name == name) {
            places.push_back(place.first);
        }
    }
    return places;
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    std::vector<PlaceID> places;
    for (auto& place : placeMap) {
        if (place.second.type == type) {
            places.push_back(place.first);
        }
    }
    return places;
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{

    if (placeMap.find(id) != placeMap.end()) {
        placeMap.at(id).name = newname;

        // vector no more sorted by alphabetically
        sortedAlpha_ = false;
        return true;
    } else {
        return false;
    }
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    if (placeMap.find(id) != placeMap.end()) {
        placeMap.at(id).coord = newcoord;

        // vector sorted by coord not is sorted anymore
        sortedCoord_ = false;
        return true;
    } else {
        return false;
    }
}

std::vector<AreaID> Datastructures::all_areas()
{
    return areaids;
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{

    if (areaMap.find(id) != areaMap.end() && areaMap.find(parentid) != areaMap.end()) {
        if (areaMap.at(id).isASubArea == false) {

            // add area as parents subarea
            areaMap.at(parentid).subAreas.push_back(id);

            // add parent as subareas parent
            areaMap.at(id).parent = parentid;
            areaMap.at(id).isASubArea = true;
            return true;
        }
    }
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    AreaID currentid = id;
    std::vector<AreaID> parentAreas;

    if (areaMap.find(id) != areaMap.end()) {

        // while currentid has a parent
        while (areaMap.at(currentid).parent != NO_PARENT) {

            // set area's parent as currentid
            currentid = areaMap.at(currentid).parent;
            parentAreas.push_back(currentid);
        }

        return parentAreas;
    }
    return { NO_AREA };
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::vector<PlaceID> places;

    //if PlaceType has a special type, only those of that type is taken into consideration
    if (type != PlaceType::NO_TYPE) {
        for (auto place : placeidsCoord_) {
            if (placeMap.at(place).type == type) {
                places.push_back(place);
            }
        }
        // if PlaceType::NO_TYPE, all types are taken into consideration
    } else {
        places = placeidsCoord_;
    }

    // sort using implemented function euclideanDistance
    sort(places.begin(), places.end(), [=](PlaceID a, PlaceID b) { return twoPointDistance(a, b, xy); });

    // only maximum of three closest places must be returned
    if (places.size() > 3) {
        places.resize(3);
    }

    return places;
}

bool Datastructures::remove_place(PlaceID id)
{
    if (placeMap.find(id) != placeMap.end()) {

        // remove place from placeMap and empty two vectors containing placeids
        placeMap.erase(id);
        placeidsCoord_.clear();
        placeidsName_.clear();

        // refill vectors with placeids from placeMap
        for (auto& place : placeMap) {

            placeidsCoord_.push_back(place.first);
            placeidsName_.push_back(place.first);

            // vectors no more sorted
            sortedCoord_ = false;
            sortedAlpha_ = false;
        }
        return true;
    }

    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    std::vector<AreaID> subAreaVec;

    if (areaMap.find(id) != areaMap.end()) {

        // recursive function to fill subAreaVec with all subareas
        recSubAreas(id, subAreaVec);

        return subAreaVec;
    }

    return { NO_AREA };
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{

    if (areaMap.find(id1) != areaMap.end() && areaMap.find(id2) != areaMap.end()) {

        // get all parents for id1 and id2
        auto parentsID1 = subarea_in_areas(id1);
        if (parentsID1.size() > 0) {
            auto parentsID2 = subarea_in_areas(id2);
            if (parentsID2.size() > 0) {

                // put parent's areaID's from parentsID1 to unordered_map parentsID1Map as keys
                std::unordered_map<AreaID, int> parentsID1Map;
                for (auto parent : parentsID1) {
                    parentsID1Map[parent];
                }

                // loop through vector of id2's parents, try to find each parent from parentsID1Map
                // when possibly first one found, return it
                for (auto parent : parentsID2) {
                    if (parentsID1Map.find(parent) != parentsID1Map.end()) {
                        return parent;
                    }
                }
            }
        }
    }
    return NO_AREA;
}

bool Datastructures::coordCompare(PlaceID a, PlaceID b)
{
    bool comp;
    // if place's have equal distance from the origin
    if (sqrt(pow(placeMap.at(a).coord.x, 2) + pow(placeMap.at(a).coord.y, 2)) == sqrt(pow(placeMap.at(b).coord.x, 2) + pow(placeMap.at(b).coord.y, 2))) {

        // the one with smaller y coordinate comes first.
        comp = placeMap.at(a).coord.y > placeMap.at(b).coord.y;
        return comp;
    }
    // the closer one from origin comes first
    comp = sqrt(pow(placeMap.at(a).coord.x, 2) + pow(placeMap.at(a).coord.y, 2)) < sqrt(pow(placeMap.at(b).coord.x, 2) + pow(placeMap.at(b).coord.y, 2));
    return comp;
}

void Datastructures::recSubAreas(AreaID id, std::vector<AreaID>& subAreaVec)
{
    // pushes all sub areas to vector recursively
    for (auto area : areaMap.at(id).subAreas) {
        subAreaVec.push_back(area);
        recSubAreas(area, subAreaVec);
    }
}

bool Datastructures::twoPointDistance(PlaceID id1, PlaceID id2, Coord compared)
{
    // if id1 and id2 are from equal distance of compared, the one with smaller y-coordinate comes first
    if (sqrt(pow(compared.x - placeMap.at(id1).coord.x, 2) + pow(compared.y - placeMap.at(id1).coord.y, 2))
        == sqrt(pow(compared.x - placeMap.at(id2).coord.x, 2) + pow(compared.y - placeMap.at(id2).coord.y, 2))) {

        if (placeMap.at(id1).coord.y < placeMap.at(id2).coord.y) {
            return true;
        }
    }

    // the closer one comes first
    if (sqrt(pow(compared.x - placeMap.at(id1).coord.x, 2) + pow(compared.y - placeMap.at(id1).coord.y, 2))
        < sqrt(pow(compared.x - placeMap.at(id2).coord.x, 2) + pow(compared.y - placeMap.at(id2).coord.y, 2))) {
        return true;
    }

    return false;
}
