// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include "iostream"
#include <cmath>
#include <limits>
#include <queue>
#include <stack>

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
    return wayVector;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{

    if (wayMap.find(id) != wayMap.end()) {
        return false;
    } else {

        auto WAY = std::make_shared<Way>();
        WAY->id = id;
        WAY->coords = coords;
        WAY->length = calcWayLength(coords);
        wayMap.insert(std::make_pair(id, WAY));
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
    if (wayMap.find(id) != wayMap.end()) {
        return wayMap.at(id)->coords;
    } else {
        return { NO_COORD };
    }
}

void Datastructures::clear_ways()
{
    wayMap.clear();
    wayVector.clear();
    crossroadMap.clear();
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_any(Coord fromxy, Coord toxy)
{
    if (isRoutePossible(fromxy, toxy)) {

        auto stack = dfs(fromxy, toxy);
        std::vector<std::tuple<Coord, WayID, Distance>> route;
        Distance distance = 0;
        WayID wayID = "null";

        auto tempStack = stack;
        if (stack.size() != 0) {
            std::stack<std::shared_ptr<Crossroad>> tempStack;
            while (stack.size() > 0) {
                tempStack.push(stack.top());
                stack.pop();
            }
            stack = tempStack;
            while (stack.size() > 0) {
                auto values = std::make_tuple(stack.top()->coord, wayID, distance);
                route.push_back(values);
                distance += stack.top()->distFromPrev;
                stack.pop();
            }
            clearVisits();
            return route;
        }
    }
    clearVisits();
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

bool Datastructures::remove_way(WayID id)
{
    if (wayMap.find(id) != wayMap.end()) {
        for (auto it = begin(wayVector); it != end(wayVector);) {
            if (*it == id) {
                it = wayVector.erase(it);
                break;
            }

            else
                ++it;
        }

        auto beginCrossroad = crossroadMap.at(wayMap.at(id)->coords.front());
        auto endCrossroad = crossroadMap.at(wayMap.at(id)->coords.back());

        auto it = beginCrossroad->ways.begin();
        while (it != beginCrossroad->ways.end()) {
            // specify condition for removing element;
            if (*it == wayMap.at(id)) {
                // erase() invalidates the iterator, use returned iterator
                it = beginCrossroad->ways.erase(it);
                break;
            }
            // Notice that iterator is incremented only on the else part
            else {
                ++it;
            }
        }
        if (beginCrossroad->ways.size() == 0) {
            crossroadMap.erase(crossroadMap.at(wayMap.at(id)->coords.front())->coord);
        }
        it = endCrossroad->ways.begin();
        while (it != endCrossroad->ways.end()) {
            // specify condition for removing element;
            if (*it == wayMap.at(id)) {
                // erase() invalidates the iterator, use returned iterator
                it = endCrossroad->ways.erase(it);
                break;
            }
            // Notice that iterator is incremented only on the else part
            else {
                ++it;
            }
        }
        if (endCrossroad->ways.size() == 0) {
            crossroadMap.erase(crossroadMap.at(wayMap.at(id)->coords.back())->coord);
        }

        wayMap.erase(id);
        return true;
    }
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    std::vector<std::tuple<Coord, WayID, Distance>> route;
    if (isRoutePossible(fromxy, toxy)) {
        if (bfs(fromxy, toxy)) {
            auto crossRoad = crossroadMap.at(toxy);

            auto crossRoadInFront = crossRoad;
            auto values = std::make_tuple(crossRoad->coord, NO_WAY, crossRoad->distFromPrev);
            route.push_back(values);
            crossRoad = crossRoad->prevCrossroad;
            while (true) {

                auto values = std::make_tuple(crossRoad->coord, crossRoadInFront->wayUsed, crossRoad->distFromPrev);
                route.push_back(values);
                if (crossRoad->coord == fromxy) {
                    break;
                }
                crossRoad = crossRoad->prevCrossroad;
                crossRoadInFront = crossRoadInFront->prevCrossroad;
            }

            clearVisits();
            std::reverse(route.begin(), route.end());
            return route;
        }
    }
    clearVisits();
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

std::vector<std::tuple<Coord, WayID>> Datastructures::route_with_cycle(Coord fromxy)
{
    if (crossroadMap.find(fromxy) != crossroadMap.end()) {
        std::vector<std::tuple<Coord, WayID>> route;
        auto stack = dfsCycle(fromxy);
        auto tempStack = stack;
        if (stack.size() != 0) {
            std::stack<std::shared_ptr<Crossroad>> tempStack;
            while (stack.size() > 0) {
                tempStack.push(stack.top());
                stack.pop();
            }
            stack = tempStack;

            while (stack.size() > 0) {
                auto values = std::make_tuple(stack.top()->coord, stack.top()->wayUsed);

                stack.pop();

                route.push_back(values);
            }

            clearVisits();
            return route;
        }
    }
    clearVisits();
    return { { NO_COORD, NO_WAY } };
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    std::vector<std::tuple<Coord, WayID, Distance>> route;
    if (isRoutePossible(fromxy, toxy)) {
        if (dijsktra(fromxy, toxy)) {
            auto crossRoad = crossroadMap.at(toxy);

            auto crossRoadInFront = crossRoad;
            auto values = std::make_tuple(crossRoad->coord, NO_WAY, crossRoad->distFromPrev);
            route.push_back(values);
            crossRoad = crossRoad->prevCrossroad;
            while (true) {

                auto values = std::make_tuple(crossRoad->coord, crossRoadInFront->wayUsed, crossRoad->distFromPrev);
                route.push_back(values);
                if (crossRoad->coord == fromxy) {
                    break;
                }
                crossRoad = crossRoad->prevCrossroad;
                crossRoadInFront = crossRoadInFront->prevCrossroad;
            }

            clearVisits();
            std::reverse(route.begin(), route.end());
            return route;
        }
    }

    clearVisits();
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

void Datastructures::clearVisits()
{
    for (auto x : crossroadMap) {
        x.second->color = "white";
        x.second->distFromPrev = 0;
        x.second->prevCrossroad = nullptr;
        x.second->wayUsed = NO_WAY;
    }
}

std::stack<std::shared_ptr<Crossroad>> Datastructures::dfs(Coord fromxy, Coord toxy)
{
    std::stack<std::shared_ptr<Crossroad>> s;
    s.push(crossroadMap.at(fromxy));
    while (!s.empty()) {
        auto last = s.top();

        s.pop();
        if (last->color == "white") {
            if (last->coord == toxy) {
                s.push(last);
                return s;
            }

            last->color = "gray";
            s.push(last);
            auto possibleCoords = ways_from(last->coord);
            bool broke = 0;
            for (auto v : possibleCoords) {

                if (crossroadMap.at(v.second)->color == "white") {
                    last->distFromPrev = wayMap.at(v.first)->length;
                    s.push(crossroadMap.at(v.second));
                    broke = 1;
                    break;
                }
            }
            if (!broke) {

                s.top()->color = "black";
                s.pop();
                if (!s.empty()) {
                    s.top()->color = "white";
                }
            }

        } else {
            last->color = "black";
        }
    }
    return s;
}

Distance Datastructures::calcWayLength(std::vector<Coord> coords)
{

    Distance distance = 0;
    for (unsigned long long i = 0; i < coords.size() - 1; i++) {
        distance += floor(sqrt(pow((coords[i].x - coords[i + 1].x), 2) + pow((coords[i].y - coords[i + 1].y), 2)));
    }
    return distance;
}

bool Datastructures::bfs(Coord fromxy, Coord toxy)
{
    std::queue<std::shared_ptr<Crossroad>> d;
    crossroadMap.at(fromxy)->color = "gray";
    d.push(crossroadMap.at(fromxy));

    while (!d.empty()) {
        auto u = d.front();
        d.pop();
        if (u->coord == toxy) {

            return true;
        }
        auto possibleCoords = ways_from(u->coord);
        for (auto v : possibleCoords) {
            if (crossroadMap.at(v.second)->color == "white") {
                crossroadMap.at(v.second)->color = "gray";
                crossroadMap.at(v.second)->prevCrossroad = u;
                crossroadMap.at(v.second)->distFromPrev = u->distFromPrev + wayMap.at(v.first)->length;
                crossroadMap.at(v.second)->wayUsed = v.first;
                d.push(crossroadMap.at(v.second));
            }
        }
        u->color = "black";
    }

    return false;
}

std::stack<std::shared_ptr<Crossroad>> Datastructures::dfsCycle(Coord fromxy)
{
    std::stack<std::shared_ptr<Crossroad>> s;
    s.push(crossroadMap.at(fromxy));
    while (!s.empty()) {

        auto last = s.top();
        s.pop();
        if (last->color == "white") {

            last->color = "gray";
            s.push(last);
            auto possibleCoords = ways_from(last->coord);
            bool broke = 0;

            for (auto v : possibleCoords) {
                if (crossroadMap.at(v.second)->color == "white") {
                    last->distFromPrev = wayMap.at(v.first)->length;

                    s.push(crossroadMap.at(v.second));
                    s.top()->prevCrossroad = last;
                    last->wayUsed = v.first;
                    s.top()->prevCrossroads.push_back(last);
                    broke = 1;
                    break;
                }
                if (std::find(last->prevCrossroads.begin(), last->prevCrossroads.end(), crossroadMap.at(v.second)) == last->prevCrossroads.end()) {
                    s.push(crossroadMap.at(v.second));
                    last->wayUsed = v.first;
                    return s;
                }
            }
            if (!broke) {
                s.top()->color = "black";

                s.pop();
                if (!s.empty()) {
                    if (s.top() != crossroadMap.at(fromxy)) {
                        s.top()->color = "white";
                        s.top()->prevCrossroads.push_back(last);
                    }
                }
            }

        } else {
            last->color = "black";
            return s;
        }
    }
    return s;
}

bool Datastructures::dijsktra(Coord fromxy, Coord toxy)
{

    auto cmp = [](std::shared_ptr<Crossroad> left, std::shared_ptr<Crossroad> right) {
        return (left->distFromPrev > right->distFromPrev);
    };
    std::priority_queue<std::shared_ptr<Crossroad>, std::vector<std::shared_ptr<Crossroad>>, decltype(cmp)> pq(cmp);

    pq.push(crossroadMap.at(fromxy));
    pq.top()->color = "gray";

    for (auto x : crossroadMap) {
        x.second->distFromPrev = std::numeric_limits<int>::max();
    }

    pq.top()->distFromPrev = 0;
    while (!pq.empty()) {

        auto u = pq.top();
        pq.pop();
        auto possibleCoords = ways_from(u->coord);
        for (auto v : possibleCoords) {
            if (crossroadMap.at(v.second)->color == "white") {
                crossroadMap.at(v.second)->color = "gray";

                pq.push(crossroadMap.at(v.second));
            }
            relax(u, crossroadMap.at(v.second), v.first);
        }

        u->color = "black";
    }
    if (crossroadMap.at(toxy)->prevCrossroad != nullptr) {
        return true;
    }
    return false;
}

void Datastructures::relax(std::shared_ptr<Crossroad> u, std::shared_ptr<Crossroad> v, WayID way)
{
    if (v->distFromPrev > u->distFromPrev + wayMap.at(way)->length) {
        v->distFromPrev = u->distFromPrev + wayMap.at(way)->length;
        v->prevCrossroad = u;
        v->wayUsed = way;
    }
}
