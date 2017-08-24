#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer, a truly terrible player htat does most things at random and groups his ships up
//*********************************************************************



AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer,
//*********************************************************************
bool getLineWithTwoIntegers(int& r, int& c)
{
	bool result(cin >> r >> c);
	if (!result)
		cin.clear();  // clear error state so can do more input operations
	cin.ignore(10000, '\n');
	return result;
}
bool HumanPlayer::placeShips(Board&b) { // prompts the user for where they want to place their ships
	int i = 0;
	while (i < game().nShips()) {
		cout << "Enter h or v for the direction of " << game().shipName(0) << "(length " << game().shipLength(0) << ")";
		Direction direction;
		char dir;
		cin >> dir;
		while (dir != 'h' && dir != 'v') {
			cout << endl << "Direction must be h or v." << endl;
			cout << "Enter h or v for the direction of " << game().shipName(0) << "(length " << game().shipLength(0) << ")";
			cin >> dir;
		}
		if (dir == 'h') {
			direction = HORIZONTAL;
		}
		else {
			direction = VERTICAL;
		}
		cout << endl;
		int* r;
		int * c;
		cout << "Enter row and column of leftmost cell (e.g. 3 5):";
		getLineWithTwoIntegers(*r, *c);
		cout << endl;
		Point p(*r, *c);
		while (*r < 0 || *r >= game().rows() || *c < 0 || *c >= game().rows() || !b.placeShip(p,i,direction)) {
			cout << "The ship can not be placed there" << endl;
			cout << "Enter row and column of leftmost cell (e.g. 3 5):";
			getLineWithTwoIntegers(*r, *c);
			cout << endl;
		}
		i++;
	}
	return true;
}
Point HumanPlayer::recommendAttack() { // attacks for the player
	cout << "Enter the row and column to attack (e.g, 3 5): ";
	int r;
	int c;
	getLineWithTwoIntegers(r, c);
	Point p;
	p.r = r;
	p.c = c;
	return p;
}
void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
	bool /* shotHit */, bool /* shipDestroyed */,
	int /* shipId */)
{
	// AwfulPlayer completely ignores the result of any attack
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
	// AwfulPlayer completely ignores what the opponent does
}


//*********************************************************************
//  MediocrePlayer randomly blocks half his cells and places ships where he can
//*********************************************************************
bool MediocrePlayer::placeShips(Board& b) {
	bool allShipsPlaced = false;
	for (int z = 0; z < 50; z++) {
		b.block();

		Point origin;
		origin.r = 0;
		origin.c = 0;
		int count = 0;
		int shipIdmax = game().nShips();
		while (count < shipIdmax) {
			if (b.placeShip(origin, count, HORIZONTAL)) {
				if (count == shipIdmax - 1) {
					allShipsPlaced = true;
				}
				count++;
			}
			else if (b.placeShip(origin, count, VERTICAL)) {
				if (count == shipIdmax - 1) {
					allShipsPlaced = true;
				}
				count++;
			}
			else {
				if (tryAgain(b, origin, count, HORIZONTAL)) {
					if (count == shipIdmax - 1) {
						allShipsPlaced = true;
					}
					count++;
					origin.c = 0;
					origin.r = 0;
				}
				else {
					count = shipIdmax;
				}
			}
		}
		if (allShipsPlaced) {
			b.unblock();
			return true;
		}
	}
	return false;
}
bool MediocrePlayer:: tryAgain(Board &b, Point origin, int shipId, Direction dir) { // recursively called in an attempt to fit the ships on the board
	if (origin.r >= game().rows() && origin.c >= game().cols()) {
		if (shipId < 1) {
			return false;
		}
		if (b.unplaceShip(arr_origin[shipId - 1], shipId - 1, dir)) {
			origin.r = 0;
			origin.c = 0;
			if (tryAgain(b, origin, shipId, dir) && tryAgain(b, arr_origin[shipId - 1], shipId - 1,dir)) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	origin.r += 1;
	if (!b.placeShip(origin, shipId, HORIZONTAL)) {
		tryAgain(b, origin, shipId, HORIZONTAL);
	}
	else {
		arr_origin[shipId] = origin;
		return true;
	}
	origin.r -= 1;
	origin.c += 1;
	if (!b.placeShip(origin, shipId, VERTICAL)) {
			tryAgain(b, origin, shipId, VERTICAL);
	}
	else {
		arr_origin[shipId] = origin;
		return true;
	}
	return true;
}
void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{
	
}

void MediocrePlayer::determineRadius(int &n, int &e,int  &w, int &s) { // return the valid points in a radius for mediocre guy to attack
	int breadth;
	if (target_shot.r + 4 >= game().rows()) {
		breadth = game().rows() - target_shot.r;
	}
	else {
		breadth = 4;
	}
	s = breadth;
	if (target_shot.r - 4 < 0) {
		breadth = target_shot.r;
	}
	else {
		breadth = 4;
	}
	n = breadth;
	if (target_shot.c + 4 >= game().cols()) {
		breadth = game().cols() - target_shot.r;
	}
	else {
		breadth = 4;
	}
	e = breadth;
	if (target_shot.c + 4 < 0) {
		breadth =  target_shot.r;
	}
	else {
		breadth = 4;
	}
	w = breadth;
}
void MediocrePlayer:: addWillShoot(int n, int e, int w, int s) { // adds to queue for shooting
	for (int i = 1; i <= n; i++) {
		Point temp;
		temp.r = target_shot.r + i;
		temp.c = target_shot.c;
		will_shoot[will_count] = temp;
		will_count++;
	}
	for (int i = 1; i <= s; i++) {
		Point temp;
		temp.r = target_shot.r - i;
		temp.c = target_shot.c;
		will_shoot[will_count] = temp;
		will_count++;
	}
	for (int i = 1; i <= e; i++) {
		Point temp;
		temp.r = target_shot.r;
		temp.c = target_shot.c + i ;
		will_shoot[will_count] = temp;
		will_count++;
	}
	for (int i = 1; i <= w; i++) {
		Point temp;
		temp.r = target_shot.r - i;
		temp.c = target_shot.c;
		will_shoot[will_count] = temp;
		will_count++;
	}
}
Point MediocrePlayer::recommendAttack() {
	Point test;
	if (state == 1) {
		bool isRandom = false;
		while (!isRandom) {
			isRandom = true;
			int row = randInt(game().rows());
			int col = randInt(game().cols());

			test.r = row;
			test.c = col;
			for (std::vector<Point>::iterator it = has_shot.begin(); it != has_shot.end(); ++it) { // iterate through vector, make sure we havent shot there
				if (test == *it) {
					isRandom = false;
				}
			}
		}
		return test;
	}
	else if (state == 2) {
		for (std::vector<Point>::iterator it = has_shot.begin(); it != has_shot.end(); ++it) { // iterate through vector, make sure we havent shot there
			if (will_shoot[shooting] == *it) {
				shooting++;
			} else{
				return will_shoot[shooting];
			}
		}
		state = 1;
		return recommendAttack();
	}
}
void MediocrePlayer::recordAttackResult(Point p,bool validShot, bool shotHit, bool shipDestroyed, int shipId) {
	has_shot.push_back(p);
	if (state == 1) {
		if (shotHit && !shipDestroyed) { // if the shot hit and did not destroy ship
			state = 2;
			target_shot = p;
			int n, e, s, w;
			determineRadius(n, e, w, s);
			addWillShoot(n, e, w, s);
		}
	}
	else if (state == 2) {
		if (shipDestroyed) {
			state = 1;
		}
	}
}
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

typedef AwfulPlayer GoodPlayer;

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
