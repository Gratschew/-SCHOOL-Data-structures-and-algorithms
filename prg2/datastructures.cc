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

std::vector<WayID> Datastructures::all_ways()
{
    return wayVector_;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{

    if (wayMap_.find(id) != wayMap_.end()) {
        return false;
    } else {

        auto way = std::make_shared<Way>();
        way->id = id;
        way->coords = coords;
        way->length = calcWayLength(coords);
        wayMap_.insert(std::make_pair(id, way));
        wayVector_.push_back(id);
        auto crossroadFirst = std::make_shared<Crossroad>();
        auto crossroadSecond = std::make_shared<Crossroad>();

        // first crossroad
        if (crossroadMap_.find(coords.front()) == crossroadMap_.end()) {
            crossroadFirst->coord = coords.front();
            crossroadFirst->ways.push_back(way);
            crossroadMap_.insert(std::make_pair(coords.front(), crossroadFirst));
        } else {
            crossroadMap_.at(coords.front())->ways.push_back(way);
        }

        // second crossroad
        if (crossroadMap_.find(coords.back()) == crossroadMap_.end()) {
            crossroadSecond->coord = coords.back();
            crossroadSecond->ways.push_back(way);
            crossroadMap_.insert(std::make_pair(coords.back(), crossroadSecond));
        } else {
            crossroadMap_.at(coords.back())->ways.push_back(way);
        }
        return true;
    }
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{
    std::vector<std::pair<WayID, Coord>> waysFromCoord;
    if (crossroadMap_.find(xy) == crossroadMap_.end()) {
        return {};
    } else {

        for (auto way : crossroadMap_.at(xy)->ways) {
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
    if (wayMap_.find(id) != wayMap_.end()) {
        return wayMap_.at(id)->coords;
    } else {
        return { NO_COORD };
    }
}

void Datastructures::clear_ways()
{
    wayMap_.clear();
    wayVector_.clear();
    crossroadMap_.clear();
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_any(Coord fromxy, Coord toxy)
{
    // if coords exist in the map
    if (isRoutePossible(fromxy, toxy)) {

        auto stack = dfs(fromxy, toxy);
        std::vector<std::tuple<Coord, WayID, Distance>> route;

        // distance starts from 0, wayID doesn't matter here.
        Distance distance = 0;
        WayID wayID = "null";

        // flip the stack from bottom up
        auto tempStack = stack;
        if (stack.size() != 0) {
            std::stack<std::shared_ptr<Crossroad>> tempStack;

            while (stack.size() > 0) {
                tempStack.push(stack.top());
                stack.pop();
            }

            // push flipped stack's values in to a vector.
            stack = tempStack;
            while (stack.size() > 0) {
                auto values = std::make_tuple(stack.top()->coord, wayID, distance);
                route.push_back(values);
                distance += stack.top()->distFromPrev;
                stack.pop();
            }

            // clear visit history from crossroads, return the route
            clearVisits();

            return route;
        }
    }
    // clear visit history from crossroads, return no route
    clearVisits();
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

bool Datastructures::remove_way(WayID id)
{
    // if id in wayMap_, erase it.
    if (wayMap_.find(id) != wayMap_.end()) {
        for (auto it = begin(wayVector_); it != end(wayVector_);) {
            if (*it == id) {
                it = wayVector_.erase(it);
                break;
            }

            else
                ++it;
        }

        // one way has two crossroads
        auto beginCrossroad = crossroadMap_.at(wayMap_.at(id)->coords.front());
        auto endCrossroad = crossroadMap_.at(wayMap_.at(id)->coords.back());

        // remove the way from the crossroad
        auto it = beginCrossroad->ways.begin();
        while (it != beginCrossroad->ways.end()) {

            if (*it == wayMap_.at(id)) {
                // erase() invalidates the iterator, use returned iterator
                it = beginCrossroad->ways.erase(it);
                break;
            } else {
                ++it;
            }
        }

        // remove the crossroad in the beginning of the way if no other ways left
        if (beginCrossroad->ways.size() == 0) {
            crossroadMap_.erase(crossroadMap_.at(wayMap_.at(id)->coords.front())->coord);
        }

        // remove the way from the crossroad
        it = endCrossroad->ways.begin();
        while (it != endCrossroad->ways.end()) {
            if (*it == wayMap_.at(id)) {
                // erase() invalidates the iterator, use returned iterator
                it = endCrossroad->ways.erase(it);
                break;
            } else {
                ++it;
            }
        }

        // remove the crossroad in the end of the way if no other ways left
        if (endCrossroad->ways.size() == 0) {
            crossroadMap_.erase(crossroadMap_.at(wayMap_.at(id)->coords.back())->coord);
        }

        wayMap_.erase(id);
        return true;
    }
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    // check if coords exist in the map
    if (isRoutePossible(fromxy, toxy)) {

        // if breadth first search was successful
        if (bfs(fromxy, toxy)) {
            std::vector<std::tuple<Coord, WayID, Distance>> route;

            // pointer to one crossroad and another pointer to one infront (the original crossRoad will be set to prev later)
            auto crossRoad = crossroadMap_.at(toxy);
            auto crossRoadInFront = crossRoad;

            // last crossroad has no way, vector will be reversed later
            auto values = std::make_tuple(crossRoad->coord, NO_WAY, crossRoad->distFromPrev);
            route.push_back(values);

            // original pointer set to previous crossroad from the search
            crossRoad = crossRoad->prevCrossroad;
            while (true) {

                auto values = std::make_tuple(crossRoad->coord, crossRoadInFront->wayUsed, crossRoad->distFromPrev);
                route.push_back(values);

                // when starting point reached
                if (crossRoad->coord == fromxy) {
                    break;
                }
                crossRoad = crossRoad->prevCrossroad;
                crossRoadInFront = crossRoadInFront->prevCrossroad;
            }

            // clear visits from crossroads
            clearVisits();
            std::reverse(route.begin(), route.end());
            return route;
        }
    }

    // clear visits from crossroads
    clearVisits();
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

std::vector<std::tuple<Coord, WayID>> Datastructures::route_with_cycle(Coord fromxy)
{

    if (crossroadMap_.find(fromxy) != crossroadMap_.end()) {
        std::vector<std::tuple<Coord, WayID>> route;

        // stack from dfsCycle
        auto stack = dfsCycle(fromxy);

        // temp stack to flip the original
        auto tempStack = stack;
        if (stack.size() != 0) {
            std::stack<std::shared_ptr<Crossroad>> tempStack;
            while (stack.size() > 0) {
                tempStack.push(stack.top());
                stack.pop();
            }

            // tempStack put to original
            stack = tempStack;

            while (stack.size() > 0) {

                // values from stack added to values, pushed to vector and top element of stack popped.
                auto values = std::make_tuple(stack.top()->coord, stack.top()->wayUsed);
                route.push_back(values);
                stack.pop();
            }
            // clear visits from crossroads
            clearVisits();
        }
        return route;
    }

    // clear visits from crossroads
    clearVisits();
    return { { NO_COORD, NO_WAY } };
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // if coords found from map
    if (isRoutePossible(fromxy, toxy)) {

        // if dijsktra was successful
        if (dijkstra(fromxy, toxy)) {

            std::vector<std::tuple<Coord, WayID, Distance>> route;

            // pointer to one crossroad and another pointer to one infront (the original crossRoad will be set to prev later)
            auto crossRoad = crossroadMap_.at(toxy);
            auto crossRoadInFront = crossRoad;

            // last crossroad has no way, vector will be reversed later
            auto values = std::make_tuple(crossRoad->coord, NO_WAY, crossRoad->distFromPrev);
            route.push_back(values);

            // original pointer set to previous crossroad from the search
            crossRoad = crossRoad->prevCrossroad;
            while (true) {

                auto values = std::make_tuple(crossRoad->coord, crossRoadInFront->wayUsed, crossRoad->distFromPrev);
                route.push_back(values);

                // when starting point reached
                if (crossRoad->coord == fromxy) {
                    break;
                }
                crossRoad = crossRoad->prevCrossroad;
                crossRoadInFront = crossRoadInFront->prevCrossroad;
            }

            // clear visits from crossroads
            clearVisits();
            std::reverse(route.begin(), route.end());
            return route;
        }
    }

    // clear visits from crossroads
    clearVisits();
    return { { NO_COORD, NO_WAY, NO_DISTANCE } };
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}

// P H A S E 1
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

// P H A S E 2

Distance Datastructures::calcWayLength(std::vector<Coord> coords)
{

    Distance distance = 0;
    // distance calculated from adding up all the points of a way by flooring the distance to a positive integer between two points
    // and summing em all up
    for (unsigned long long i = 0; i < coords.size() - 1; i++) {
        distance += floor(sqrt(pow((coords[i].x - coords[i + 1].x), 2) + pow((coords[i].y - coords[i + 1].y), 2)));
    }
    return distance;
}

bool Datastructures::isRoutePossible(Coord fromxy, Coord toxy)
{
    // if starting crossroad not found
    if (crossroadMap_.find(fromxy) == crossroadMap_.end()) {
        return false;
    }

    // if target crossroad not found
    if (crossroadMap_.find(toxy) == crossroadMap_.end()) {
        return false;
    }

    // found
    return true;
}

std::stack<std::shared_ptr<Crossroad>> Datastructures::dfs(Coord fromxy, Coord toxy)
{
    std::stack<std::shared_ptr<Crossroad>> s;
    s.push(crossroadMap_.at(fromxy));
    while (!s.empty()) {
        auto last = s.top();
        s.pop();

        // not handled before
        if (last->color == "white") {

            // if target found
            if (last->coord == toxy) {
                s.push(last);
                return s;
            }

            // being handled
            last->color = "gray";
            s.push(last);

            // all last's direct neighbors
            auto possibleCoords = ways_from(last->coord);

            // broke set to false
            bool broke = 0;
            for (auto v : possibleCoords) {

                // if node is white, set the information and break the loop, setting broke to true
                if (crossroadMap_.at(v.second)->color == "white") {
                    last->distFromPrev = wayMap_.at(v.first)->length;
                    s.push(crossroadMap_.at(v.second));
                    broke = 1;
                    break;
                }
            }

            // if no neighbors were unvisited, set crossroad to black
            if (!broke) {
                s.top()->color = "black";
                s.pop();

                // if stack isn't empty, set crossroad to white for backtracking purposes
                if (!s.empty()) {
                    s.top()->color = "white";
                }
            }

            // node handled
        } else {
            last->color = "black";
        }
    }
    return s;
}

std::stack<std::shared_ptr<Crossroad>> Datastructures::dfsCycle(Coord fromxy)
{
    std::stack<std::shared_ptr<Crossroad>> s;
    s.push(crossroadMap_.at(fromxy));
    while (!s.empty()) {

        auto last = s.top();
        s.pop();

        // crosshoad has not yet been handled or a backtrack is taking place
        if (last->color == "white") {

            // crossroad is being handled
            last->color = "gray";

            s.push(last);

            // all last's direct neighbors
            auto possibleCoords = ways_from(last->coord);

            // broke set to false
            bool broke = 0;

            for (auto v : possibleCoords) {

                // if node is white, set the information and break the loop, setting broke to true
                if (crossroadMap_.at(v.second)->color == "white") {
                    last->distFromPrev = wayMap_.at(v.first)->length;

                    s.push(crossroadMap_.at(v.second));
                    //s.top()->prevCrossroad = last;
                    last->wayUsed = v.first;
                    s.top()->prevCrossroads.push_back(last);
                    broke = 1;
                    break;
                }

                // if neighbor is gray (has been handled before) and not found from last's neighbors, a cycle has been detected
                if (std::find(last->prevCrossroads.begin(), last->prevCrossroads.end(), crossroadMap_.at(v.second)) == last->prevCrossroads.end()) {

                    // push cycle crossroad to stack and the way information and return the stack
                    s.push(crossroadMap_.at(v.second));
                    last->wayUsed = v.first;
                    return s;
                }
            }

            // if all neighbors were gray or black, crossroad is set to black. No more possible routes.
            if (!broke) {
                s.top()->color = "black";
                s.pop();

                // if stack is not yet empty, back track back to the crossroad with still unhandled neighbors.
                if (!s.empty()) {
                    if (s.top() != crossroadMap_.at(fromxy)) {
                        s.top()->color = "white";
                        s.top()->prevCrossroads.push_back(last);
                    }
                }
            }
            // crossroad handled, crossroad is set to black. No more possible routes.
        } else {
            last->color = "black";
            return s;
        }
    }
    return s;
}

bool Datastructures::bfs(Coord fromxy, Coord toxy)
{
    // queue created and start crossroad's color set to gray and pushed to the d.
    std::queue<std::shared_ptr<Crossroad>> d;
    crossroadMap_.at(fromxy)->color = "gray";
    d.push(crossroadMap_.at(fromxy));

    while (!d.empty()) {
        auto u = d.front();
        d.pop();

        // if target reached
        if (u->coord == toxy) {
            return true;
        }

        // all u's direct neighbors
        auto possibleCoords = ways_from(u->coord);
        for (auto v : possibleCoords) {

            // if crossroad has not been handled
            if (crossroadMap_.at(v.second)->color == "white") {
                crossroadMap_.at(v.second)->color = "gray";
                crossroadMap_.at(v.second)->prevCrossroad = u;
                crossroadMap_.at(v.second)->distFromPrev = u->distFromPrev + wayMap_.at(v.first)->length;
                crossroadMap_.at(v.second)->wayUsed = v.first;
                d.push(crossroadMap_.at(v.second));
            }
        }

        // crossroad completely handled
        u->color = "black";
    }

    return false;
}

bool Datastructures::dijkstra(Coord fromxy, Coord toxy)
{
    // compare function for priority_queue to set the elements so the smallest one is on the top
    // when it comes to distance from previous crossroad
    auto cmp = [](std::shared_ptr<Crossroad> left, std::shared_ptr<Crossroad> right) {
        return (left->distFromPrev > right->distFromPrev);
    };

    // priority_queue pq with the compare function
    std::priority_queue<std::shared_ptr<Crossroad>, std::vector<std::shared_ptr<Crossroad>>, decltype(cmp)> pq(cmp);

    // starting crossroad pushed to pq and its color set to gray
    pq.push(crossroadMap_.at(fromxy));
    pq.top()->color = "gray";

    // each crossroad's distance from previous crossroad set to "infinity"
    for (auto crossroad : crossroadMap_) {
        crossroad.second->distFromPrev = std::numeric_limits<int>::max();
    }
    // starting crossroads distance from it self is 0.
    pq.top()->distFromPrev = 0;

    while (!pq.empty()) {
        auto u = pq.top();
        pq.pop();

        // all u's direct neighbors
        auto possibleCoords = ways_from(u->coord);
        for (auto v : possibleCoords) {

            // if crossroad has not been handled
            if (crossroadMap_.at(v.second)->color == "white") {
                crossroadMap_.at(v.second)->color = "gray";
                pq.push(crossroadMap_.at(v.second));
            }

            // see if there is possibly a shorter route to v
            relax(u, crossroadMap_.at(v.second), v.first);
        }

        // crossroad handled, set to black
        u->color = "black";
    }

    // if the target was reached, return true
    if (crossroadMap_.at(toxy)->prevCrossroad != nullptr) {
        return true;
    }

    // the target was not reached
    return false;
}

void Datastructures::relax(std::shared_ptr<Crossroad> u, std::shared_ptr<Crossroad> v, WayID way)
{
    // if there is a possible shorter way to v, set it.
    if (v->distFromPrev > u->distFromPrev + wayMap_.at(way)->length) {
        v->distFromPrev = u->distFromPrev + wayMap_.at(way)->length;
        v->prevCrossroad = u;
        v->wayUsed = way;
    }
}

void Datastructures::clearVisits()
{
    // clears all the visit information from crossroads
    for (auto x : crossroadMap_) {
        x.second->color = "white";
        x.second->distFromPrev = 0;
        x.second->prevCrossroad = nullptr;
        x.second->wayUsed = NO_WAY;
    }
}
