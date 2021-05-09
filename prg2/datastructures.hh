// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER = 0,
    FIREPIT,
    SHELTER,
    PARKING,
    PEAK,
    BAY,
    AREA,
    NO_TYPE };

// Type for a coordinate (x, y)
struct Coord {
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1 == c2); } // Not strictly necessary

struct CoordHash {
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) {
        return true;
    } else if (c2.y < c1.y) {
        return false;
    } else {
        return c1.x < c2.x;
    }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = { NO_VALUE, NO_VALUE };

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;

struct Way {
    WayID id;
    std::vector<Coord> coords;
    Distance length;
};

struct Crossroad {
    Coord coord;
    std::vector<std::shared_ptr<Way>> ways;
    std::string color = "white";
    Distance distFromPrev = 0;
    std::shared_ptr<Crossroad> prevCrossroad = nullptr; // for bfs and dijsktra
    std::vector<std::shared_ptr<Crossroad>> prevCrossroads; // for dfsCycle
    WayID wayUsed;
};
// This is the class you are supposed to implement

class Datastructures {
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    int place_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> all_places();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_area_name(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_areas();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_place(PlaceID id);

    // Estimate of performance:
    // Short rationale for estimate:
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

    // Phase 2 operations

    // Estimate of performance: O(1)
    // Short rationale for estimate: Returns a vector
    std::vector<WayID> all_ways();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Only pushback for a vector and insert and find for unordered_map used.
    bool add_way(WayID id, std::vector<Coord> coords);

    // Estimate of performance: O(N)
    // Short rationale for estimate: For loop to go through a vector
    std::vector<std::pair<WayID, Coord>> ways_from(Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Only find used for an unoredered_map.
    std::vector<Coord> get_way_coords(WayID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Linear in size, clear functions called for vector and unordered_maps.
    void clear_ways();

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: Depth first search used, where V is vertices and E is edges.
    std::vector<std::tuple<Coord, WayID, Distance>> route_any(Coord fromxy, Coord toxy);

    // Non-compulsory operations

    // Estimate of performance: O(N)
    // Short rationale for estimate: Linear in size, loops through unordered_maps and vectors.
    bool remove_way(WayID id);

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: Breadth first search used, where V is vertices and E is edges.
    std::vector<std::tuple<Coord, WayID, Distance>> route_least_crossroads(Coord fromxy, Coord toxy);

    // Estimate of performance: O(V+E)
    // Short rationale for estimate: Depth first search used, where V is vertices and E is edges.
    std::vector<std::tuple<Coord, WayID>> route_with_cycle(Coord fromxy);

    // Estimate of performance: O((V+E)lgV)
    // Short rationale for estimate: Dijkstra's algorithm implemented with a priority queue goes O(V) for while loop
    // and O(E) for for-loop at max.
    std::vector<std::tuple<Coord, WayID, Distance>> route_shortest_distance(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_ways();

private:
    // P H A S E 2  O P E R A T I O N S
    // helper function to calculate waylength
    Distance calcWayLength(std::vector<Coord> coords);

    // returns a truth value if coords are in the data
    bool isRoutePossible(Coord fromxy, Coord toxy);

    // depth first search using stack
    std::stack<std::shared_ptr<Crossroad>> dfs(Coord fromxy, Coord toxy);

    // depth first search to detect a cycle using stack
    std::stack<std::shared_ptr<Crossroad>> dfsCycle(Coord fromxy);

    // breadth first search
    bool bfs(Coord fromxy, Coord toxy);

    // dijsktra's algorithm implementation
    bool dijkstra(Coord fromxy, Coord toxy);

    // helper function for dijsktra's algorithm
    void relax(std::shared_ptr<Crossroad> u, std::shared_ptr<Crossroad> v, WayID way);

    // clears the visits in crossroads
    void clearVisits();

    // all ways in a map
    std::vector<WayID> wayVector_;

    // all ways in a map as WayID as key and ptr to the way as value
    std::unordered_map<WayID, std::shared_ptr<Way>> wayMap_;

    // all crossroads as coords as keys and pointer to crossroad as value, coordhash given
    // so Coords can be used as keys.
    std::unordered_map<Coord, std::shared_ptr<Crossroad>, CoordHash> crossroadMap_;
};

#endif // DATASTRUCTURES_HH
