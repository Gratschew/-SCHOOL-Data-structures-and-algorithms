// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <functional>
#include <limits>
#include <string>
#include <tuple>
#include <unordered_map>
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
AreaID const NO_PARENT = -1;

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

struct Place {
    Name name;
    PlaceType type;
    Coord coord;
};

struct Area {
    Name name;
    std::vector<Coord> coords;
    std::vector<AreaID> subAreas;
    bool isASubArea = false;
    AreaID parent = NO_PARENT;
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

// This is the class you are supposed to implement

class Datastructures {
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Time complexity for .size() is a constant
    int place_count();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Time complexity for .clear() is linear for u_o map and vectors
    void clear_all();

    // Estimate of performance: O(1)
    // Short rationale for estimate: only returns a vector
    std::vector<PlaceID> all_places();

    // Estimate of performance: O(1)
    // Short rationale for estimate: push_back() for vector and insert for u_o map is a constant
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: uses u_o map's find(), which complexity is a constant
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: uses u_o map's find(), which complexity is a constant
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(N(log(N))
    // Short rationale for estimate: std::sort has a time complexity of O(N(log(N))
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: O(N(log(N))
    // Short rationale for estimate: std::sort has a time complexity of O(N(log(N))
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Loops through a vector and push_backs some elements to another vector
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Loops through a vector and push_backs some elements to another vector
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: O(1)
    // Short rationale for estimate: u_o map's find()'s complexity is constant on average
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: O(1)
    // Short rationale for estimate: u_o map's find()'s complexity is constant on average
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1)
    // Short rationale for estimate: push_back() for vector, insert and find for u_o map are constants
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find and at for u_o map are constants on average
    Name get_area_name(AreaID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find and at for u_o map are constants on average
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: only returns a vector
    std::vector<AreaID> all_areas();

    // Estimate of performance: O(1)
    // Short rationale for estimate: find and at for u_o map are constants on average
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Constant operations inside a loop
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance: O(N)
    // Short rationale for estimate: I'm not actually sure since it calls a recursive function that is recursing inside a for loop (recSubAreas),
    // could be also O(N^2)
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance: O(N(log(N))
    // Short rationale for estimate: Has a for loop with constant operations inside and std::sort successively.
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Erase from a map, clearing of two vectors and for loop with vector push_backs
    bool remove_place(PlaceID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: calls a linear function subarea_in_areas twice, then has a loop.
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

    // OWN HELPER FUNCTIONS

    // Estimate of performance: O(1)
    // Short rationale for estimate: compares
    bool coordCompare(PlaceID a, PlaceID b);

    // Estimate of performance: O(N)
    // Short rationale for estimate: recursing inside a loop
    void recSubAreas(AreaID id, std::vector<AreaID>& vec);

    // Estimate of performance: O(1)
    // Short rationale for estimate: compares
    bool twoPointDistance(PlaceID id1, PlaceID id2, Coord compared);

private:
    // Datastructure for the places
    std::unordered_map<PlaceID, Place> placeMap;
    std::vector<PlaceID> placeidsName_;
    std::vector<PlaceID> placeidsCoord_;

    // Boolean values if the placeids vectors are sorted or not
    bool sortedCoord_ = false;
    bool sortedAlpha_ = false;

    // Datastructure for the areas
    std::unordered_map<AreaID, Area> areaMap;
    std::vector<AreaID> areaids;
};

#endif // DATASTRUCTURES_HH
