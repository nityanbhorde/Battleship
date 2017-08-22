#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <string>
#include <vector>
#include <cassert>

class Point;
class Player;



class Game
{
  public:
    Game(int nRows, int nCols);
    ~Game();
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
	int lookUp(char key) const {
		return 1;
	}
    Point randomPoint() const;
    bool addShip(int length, char symbol, std::string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    std::string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, bool shouldPause = true);
      // We prevent a Game object from being copied or assigned
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

  private:
	  class Ship {
	  public:
		  Ship(std::string name, char symbol, int id, int length)
			  : m_name(name), m_symbol(symbol), m_id(id), m_length(length) {}
	  private:
		  std::string m_name;
		  char m_symbol;
		  int m_id;
		  int m_length;
	  };
	int m_rows;
	int m_cols;
	std::vector<Ship> ship_vec;
};

#endif // GAME_INCLUDED
