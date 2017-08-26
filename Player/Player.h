#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include <string>
#include <vector>
#include <queue>
#include "../globals.h"
class Point;
class Board;
class Game;

class Player
{
  public:
    Player(std::string nm, const Game& g)
     : m_name(nm), m_game(g)
    {}

    virtual ~Player() {}

    std::string name() const { return m_name; }
    const Game& game() const { return m_game; }

    virtual bool isHuman() const { return false; }

    virtual bool placeShips(Board& b) = 0;
    virtual Point recommendAttack() = 0;
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                        bool shipDestroyed, int shipId) = 0;
    virtual void recordAttackByOpponent(Point p) = 0;
      // We prevent any kind of Player object from being copied or assigned
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

  private:
    std::string m_name;
    const Game& m_game;
};

class AwfulPlayer : public Player
{
public:
	AwfulPlayer(std::string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
private:
	Point m_lastCellAttacked;
};

class HumanPlayer : public Player
{
public:
	HumanPlayer(std::string nm, const Game& g)
		:Player(nm, g) {};
	virtual bool isHuman() const  { return true; }
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
};
class MediocrePlayer : public Player
{
public:
	MediocrePlayer(std::string nm, const Game& g)
		:Player(nm, g) {
		state = 1;;
	};
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
	bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
	void determineRadius(int &n, int &e, int &w, int &s);
	bool attemptPlace(Board &b, Point origin, int shipId);
	void addWillShoot(int n, int e, int w, int s);
private:
	std::queue <Point> will_shoot;
	int shooting =0;
	std::vector <Point> has_shot;
	Point target_shot;
	Point arr_origin[100];
	int state;
};
Player* createPlayer(std::string type, std::string nm, const Game& g);

#endif // PLAYER_INCLUDED
