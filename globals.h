#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

#include <random>

const int MAXROWS = 10;
const int MAXCOLS = 10;

enum Direction {
    HORIZONTAL, VERTICAL
};

class Point
{
  public:
    Point() : r(0), c(0) {}
	Point(int rr, int cc) : r(rr), c(cc) {}
	bool operator ==( const Point &b) const{
		if (r == b.r && c == b.c) {
			return true;
		}
		return false;
	}
    int r;
    int c;
};
namespace std // hash function for unordered map
{
	template <>
	struct hash<Point>
	{
		size_t operator()(const Point& k) const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return ((hash<int>()(k.r) ^ (hash<int>()(k.c) << 1)) >> 1);
		}
	};
}

  // Return a uniformly distributed random int from 0 to limit-1
inline int randInt(int limit)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    if (limit < 1)
        limit = 1;
    std::uniform_int_distribution<> distro(0, limit-1);
    return distro(generator);
}

#endif // GLOBALS_INCLUDED
