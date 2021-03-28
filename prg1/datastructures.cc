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
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    // Replace this comment with your implementation
    return placeMap.size();
}

void Datastructures::clear_all()
{
    placeMap.clear();
    placeidsName_.clear();
    placeidsCoord_.clear();

    areaMap.clear();
    areaids.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    // Replace this comment with your implementation
    return { placeidsName_ };
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    auto search = placeMap.find(id);
    if (search != placeMap.end()) {
        return false;
    } else {
        Place place;
        place.name = name;
        place.type = type;
        place.coord = xy;

        placeMap.insert(std::make_pair(id, place));
        placeidsName_.push_back(id);
        placeidsCoord_.push_back(id);
        sortedCoord_ = false;
        sortedAlpha_ = false;
        return true;
    }
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    auto search = placeMap.find(id);
    if (search != placeMap.end()) {
        return { placeMap.at(id).name, placeMap.at(id).type };
    }
    return { NO_NAME, PlaceType::NO_TYPE };
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    auto search = placeMap.find(id);
    if (search != placeMap.end()) {
        return placeMap.at(id).coord;
    } else {
        return NO_COORD;
    }
}

bool Datastructures::add_area(AreaID id, const Name& name, std::vector<Coord> coords)
{
    auto search = areaMap.find(id);
    if (search != areaMap.end()) {
        return false;
    } else {
        Area area;
        area.name = name;
        area.coords = coords;
        areaMap.insert(std::make_pair(id, area));
        areaids.push_back(id);

        return true;
    }
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    auto search = areaMap.find(id);
    if (search != areaMap.end()) {
        return areaMap.at(id).name;
    } else {
        return NO_NAME;
    }
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    auto search = areaMap.find(id);
    if (search != areaMap.end()) {
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
        sort(placeidsCoord_.begin(), placeidsCoord_.end(), [=](PlaceID a, PlaceID b) { return coordCompare(a, b); });
        sortedCoord_ = true;
    }
    return placeidsCoord_;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    std::vector<PlaceID> places;
    for (auto& it : placeMap) {
        if (it.second.name == name) {
            places.push_back(it.first);
        }
    }
    return places;
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    std::vector<PlaceID> places;
    for (auto& it : placeMap) {
        if (it.second.type == type) {
            places.push_back(it.first);
        }
    }
    return places;
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{

    auto search = placeMap.find(id);
    if (search != placeMap.end()) {
        placeMap.at(id).name = newname;
        sortedAlpha_ = false;
        return true;
    } else {
        return false;
    }
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    auto search = placeMap.find(id);
    if (search != placeMap.end()) {
        placeMap.at(id).coord = newcoord;
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
    auto searchid = areaMap.find(id);
    auto searchParentid = areaMap.find(parentid);
    if (searchid != areaMap.end()) {
        if (searchParentid != areaMap.end()) {
            if (areaMap.at(id).isASubArea == false) {
                areaMap.at(parentid).subAreas.push_back(id);
                areaMap.at(id).parent = parentid;
                areaMap.at(id).isASubArea = true;
                //areaMap.at(id).hasParent = true;
                return true;
            }
        }
    }
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    AreaID currentid = id;
    std::vector<AreaID> parentMap;
    auto searchid = areaMap.find(id);
    if (searchid != areaMap.end()) {
        while (true) {
            if (areaMap.at(currentid).parent != NO_PARENT) {
                currentid = areaMap.at(currentid).parent;
                parentMap.push_back(currentid);
            } else {
                break;
            }
        }
        return parentMap;
    }
    return { NO_AREA };
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::vector<PlaceID> places;
    if (type != PlaceType::NO_TYPE) {
        for (auto place : placeidsCoord_) {
            if (placeMap.at(place).type == type) {
                places.push_back(place);
            }
        }
    } else {
        places = placeidsCoord_;
    }
    sort(places.begin(), places.end(), [=](PlaceID a, PlaceID b) { return euclideanDistance(a, b, xy); });
    if (places.size() > 3) {
        places.resize(3);
    }

    return places;
}

bool Datastructures::remove_place(PlaceID id)
{
    auto search = placeMap.find(id);
    if (search != placeMap.end()) {
        placeMap.erase(id);
        placeidsCoord_.clear();
        placeidsName_.clear();
        for (auto& it : placeMap) {

            placeidsCoord_.push_back(it.first);
            placeidsName_.push_back(it.first);
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
    auto search = areaMap.find(id);
    if (search != areaMap.end()) {
        recSubAreas(id, subAreaVec);
        return subAreaVec;
    }

    return { NO_AREA };
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    auto search = areaMap.find(id1);
    auto search2 = areaMap.find(id2);
    if (search != areaMap.end() || search2 != areaMap.end()) {
        auto parents1 = subarea_in_areas(id1);
        if (parents1.size() > 0) {
            auto parents2 = subarea_in_areas(id2);
            if (parents2.size() > 0) {
                std::unordered_map<AreaID, int> parents1Map;
                for (auto x : parents1) {
                    parents1Map[x];
                }
                for (auto x : parents2) {
                    auto search = parents1Map.find(x);
                    if (search != parents1Map.end()) {
                        return x;
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
    if (sqrt(pow(placeMap.at(a).coord.x, 2) + pow(placeMap.at(a).coord.y, 2)) == sqrt(pow(placeMap.at(b).coord.x, 2) + pow(placeMap.at(b).coord.y, 2))) {
        comp = placeMap.at(a).coord.y > placeMap.at(b).coord.y;
        return comp;
    }
    comp = sqrt(pow(placeMap.at(a).coord.x, 2) + pow(placeMap.at(a).coord.y, 2)) < sqrt(pow(placeMap.at(b).coord.x, 2) + pow(placeMap.at(b).coord.y, 2));
    return comp;
}

void Datastructures::recSubAreas(AreaID id, std::vector<AreaID>& subAreaVec)
{
    for (auto area : areaMap.at(id).subAreas) {
        subAreaVec.push_back(area);
        recSubAreas(area, subAreaVec);
    }
}

bool Datastructures::euclideanDistance(PlaceID id1, PlaceID id2, Coord compared)
{
    if (sqrt(pow(compared.x - placeMap.at(id1).coord.x, 2) + pow(compared.y - placeMap.at(id1).coord.y, 2))
        == sqrt(pow(compared.x - placeMap.at(id2).coord.x, 2) + pow(compared.y - placeMap.at(id2).coord.y, 2))) {
        if (placeMap.at(id1).coord.y < placeMap.at(id2).coord.y) {
            return true;
        }
    }
    if (sqrt(pow(compared.x - placeMap.at(id1).coord.x, 2) + pow(compared.y - placeMap.at(id1).coord.y, 2))
        < sqrt(pow(compared.x - placeMap.at(id2).coord.x, 2) + pow(compared.y - placeMap.at(id2).coord.y, 2))) {
        return true;
    }

    return false;
}
