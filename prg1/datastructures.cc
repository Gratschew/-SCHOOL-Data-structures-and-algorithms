// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

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
    return id_.size();
}

void Datastructures::clear_all()
{
    // Replace this comment with your implementation
}

std::vector<PlaceID> Datastructures::all_places()
{
    // Replace this comment with your implementation
    return { placeids };
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    auto search = id_.find(id);
    if (search != id_.end()) {
        return false;
    } else {
        Place place;
        place.name = name;
        place.type = type;
        place.coord = xy;

        id_.insert(std::make_pair(id, place));
        placeids.push_back(id);
        return true;
    }
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    return { id_.at(id).name, id_.at(id).type };
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    auto search = id_.find(id);
    if (search != id_.end()) {
        return id_.at(id).coord;
    } else {
        return NO_COORD;
    }
}

bool Datastructures::add_area(AreaID id, const Name& name, std::vector<Coord> coords)
{
    // Replace this comment with your implementation
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    // Replace this comment with your implementation
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    // Replace this comment with your implementation
    return { NO_COORD };
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}

std::vector<PlaceID> Datastructures::places_alphabetically()
{
    sort(placeids.begin(), placeids.end(), [=](PlaceID a, PlaceID b) { return id_.at(a).name < id_.at(b).name; });
    return placeids;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{

    sort(placeids.begin(), placeids.end(), [=](PlaceID a, PlaceID b) {
        return my_cmp(a, b);
    });
    return placeids;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    // Replace this comment with your implementation
    return {};
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    // Replace this comment with your implementation
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    // Replace this comment with your implementation
    return { NO_AREA };
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    // Replace this comment with your implementation
    return { NO_AREA };
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    return NO_AREA;
}

bool Datastructures::my_cmp(PlaceID a, PlaceID b)
{
    bool comp;
    if (sqrt(pow(id_.at(a).coord.x, 2) + pow(id_.at(a).coord.y, 2)) == sqrt(pow(id_.at(b).coord.x, 2) + pow(id_.at(b).coord.y, 2))) {
        comp = id_.at(a).coord.y > id_.at(b).coord.y;
        return comp;
    }
    comp = sqrt(pow(id_.at(a).coord.x, 2) + pow(id_.at(a).coord.y, 2)) < sqrt(pow(id_.at(b).coord.x, 2) + pow(id_.at(b).coord.y, 2));
    return comp;
}
