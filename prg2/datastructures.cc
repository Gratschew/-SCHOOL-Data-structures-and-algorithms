// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include "iostream"
#include <cmath>

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
    return 0;
}

void Datastructures::clear_all()
{
    // Replace this comment with your implementation
}

std::vector<PlaceID> Datastructures::all_places()
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    // Replace this comment with your implementation
    return false;
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    // Replace this comment with your implementation
    return { NO_NAME, PlaceType::NO_TYPE };
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    // Replace this comment with your implementation
    return NO_COORD;
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
    // Replace this comment with your implementation
    return {};
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    // Replace this comment with your implementation
    return {};
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

std::vector<WayID> Datastructures::all_ways()
{
    // Replace this comment with your implementation
    return wayVector;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{
    /*if (wayMap.find(id) != wayMap.end()) {
        return false;
    } else {

        auto WAY = std::make_shared<Way>();

        WAY->coord = coords.at(0);
        std::shared_ptr<Way> y { WAY };

        for (auto coor : coords) {

            std::shared_ptr<Way> WAY2 { new Way };
            WAY2->coord = coor;
            WAY2->prevNode = y;
            y->nextNode = WAY2;
            y = WAY2;
            WAY->leafNode = y;
        }
        auto x = std::make_pair(WAY, coords);
        wayMap.insert(std::make_pair(id, x));
        wayVector.push_back(id);

        return true;
    }*/
    if (wayMap2.find(id) != wayMap2.end()) {
        return false;
    } else {

        auto WAY = std::make_shared<Way2>();
        WAY->id = id;
        WAY->coords = coords;
        wayMap2.insert(std::make_pair(id, WAY));
        wayVector.push_back(id);
        auto CROSSROAD1 = std::make_shared<Crossroad>();
        auto CROSSROAD2 = std::make_shared<Crossroad>();

        // first crossroad
        if (crossroadMap.find(coords.front()) == crossroadMap.end()) {
            CROSSROAD1->coord = coords.front();
            CROSSROAD1->ways.push_back(WAY);
            crossroadMap.insert(std::make_pair(coords.front(), CROSSROAD1));
        } else {
            crossroadMap.at(coords.front())->ways.push_back(WAY);
        }

        // second crossroad
        if (crossroadMap.find(coords.back()) == crossroadMap.end()) {
            CROSSROAD2->coord = coords.back();
            CROSSROAD2->ways.push_back(WAY);
            crossroadMap.insert(std::make_pair(coords.back(), CROSSROAD2));
        } else {
            crossroadMap.at(coords.back())->ways.push_back(WAY);
        }
        return true;
    }
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{
    /*std::vector<std::pair<WayID, Coord>> waysFromCoord;

    for (auto x : wayMap) {
        if (x.second.first->coord == xy) {
            auto pair = std::make_pair(x.first, x.second.first->leafNode->coord);
            waysFromCoord.push_back(pair);
        }
        if (x.second.first->leafNode->coord == xy) {
            auto pair = std::make_pair(x.first, x.second.first->coord);
            waysFromCoord.push_back(pair);
        }
    }
    if (waysFromCoord.size() > 0) {
        return waysFromCoord;
    }
    return {};*/
    std::vector<std::pair<WayID, Coord>> waysFromCoord;
    if (crossroadMap.find(xy) == crossroadMap.end()) {
        return {};
    } else {

        for (auto way : crossroadMap.at(xy)->ways) {
            if (way->coords.back() != xy) {
                auto pair = std::make_pair(way->id, way->coords.back());
                waysFromCoord.push_back(pair);
            } else {
                auto pair = std::make_pair(way->id, way->coords.front());
                waysFromCoord.push_back(pair);
            }
        }

        return waysFromCoord;
    }
}

std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    /*if (wayMap.find(id) != wayMap.end()) {
        return wayMap.at(id).second;
    } else {
        return { NO_COORD };
    }*/
    if (wayMap2.find(id) != wayMap2.end()) {
        return wayMap2.at(id)->coords;
    } else {
        return { NO_COORD };
    }
}

void Datastructures::clear_ways()
{
    wayMap2.clear();
    wayVector.clear();
    crossroadMap.clear();
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_any(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    if (isRoutePossible(fromxy, toxy)) {
    }
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

bool Datastructures::remove_way(WayID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

std::vector<std::tuple<Coord, WayID>> Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return { { NO_COORD, NO_WAY } };
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}

bool Datastructures::isRoutePossible(Coord fromxy, Coord toxy)
{
    if (crossroadMap.find(fromxy) == crossroadMap.end()) {
        return false;
    }
    if (crossroadMap.find(toxy) == crossroadMap.end()) {
        return false;
    }
    return true;
}
