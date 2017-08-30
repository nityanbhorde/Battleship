#include "Player.h"
#include "../Board/Board.h"
#include "../Game/Game.h"
#include "../globals.h"
#include <iostream>
#include <string>

using namespace std;



//*********************************************************************
//  AwfulPlayer, a truly terrible player that groups his ships in the top left, and attacks row by row
//*********************************************************************


AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL,false))
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


//*********************************************************************
//  HumanPlayer, relies on human input
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
		cout << "Enter h or v for the direction of " << game().shipName(i) << "(length " << game().shipLength(i) << ")";
		Direction direction;
		char dir;
		cin >> dir;
		while (dir != 'h' && dir != 'v') {
			cout << endl << "Direction must be h or v." << endl;
			cout << "Enter h or v for the direction of " << game().shipName(i) << "(length " << game().shipLength(i) << ")";
			cin >> dir;
		}
		if (dir == 'h') {
			direction = HORIZONTAL;
		}
		else {
			direction = VERTICAL;
		}
		cout << endl;
		int r;
		int  c;
		r = 0;
		c = 0;
		cout << "Enter row and column of leftmost cell (e.g. 3 5):";
		getLineWithTwoIntegers(r, c);
		cout << endl;
		Point p(r, c);
		while (r < 0 || r >= game().rows() || c < 0 || c >= game().rows() || !b.placeShip(p,i,direction,false)) {
			cout << "The ship can not be placed there" << endl;
			b.display(false);
			cout << "Enter row and column of leftmost cell (e.g. 3 5):";
			getLineWithTwoIntegers(r, c);
			p.r = r;
			p.c = c;
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
	
}



//*********************************************************************
//  MediocrePlayer randomly blocks half his cells and places ships where he can, then attacks based on if hes recently hit a ship
//*********************************************************************
bool MediocrePlayer::placeShips(Board& b) { // place ships on cells not blocked using recursive function
	bool allShipsPlaced = false;
	for (int i = 0; i < 50; i++) {
		b.block();
		Point origin;
		int current = 0;
		int shipIdmax = game().nShips();
		while (current < shipIdmax) {
			if (attemptPlace(b, origin, current)) {
				if (current == shipIdmax - 1) {
					allShipsPlaced = true;
				}
				current++;
			}
			else {
				current = shipIdmax;
			}
		}
		if (allShipsPlaced) {
			b.unblock();
			return true;
		}
		b.unblock();
	}
	return false;
}

bool MediocrePlayer::attemptPlace(Board &b, Point origin, int shipId) { // recursively called in an attempt to fit the ships on the board
	if (origin.r < game().rows()) {
		if (b.placeShip(origin, shipId, HORIZONTAL,false)) {
			arr_origin[shipId] = origin;
			return true;
		}
		else {
			origin.r += 1;
			if (attemptPlace(b, origin, shipId)) {
				return true;
			}
			else {
				origin.r -= 1;
			}
		}
	}
	 if (origin.c < game().cols()) {
		if (b.placeShip(origin, shipId, VERTICAL,false)) {
			arr_origin[shipId] = origin;
			return true;
		}
		else {
			origin.c += 1;
			if (attemptPlace(b, origin, shipId)) {
				return true;
			}
			else {
				origin.c -= 1;
			}
		}
	}
	if(origin.r == 0 && origin.c == 0) {
		if (shipId < 1) {
			return false;
		}
		if (b.unplaceShip(arr_origin[shipId - 1], shipId - 1, HORIZONTAL)) {
			origin.r = 0;
			origin.c = 0;
			if (attemptPlace(b, origin, shipId)) {
				return attemptPlace(b, arr_origin[shipId - 1], shipId - 1);
			}
			else {
				return false;
			}
		}
		else if (b.unplaceShip(arr_origin[shipId - 1], shipId - 1, VERTICAL)) {
			origin.r = 0;
			origin.c = 0;
			if (attemptPlace(b, origin, shipId)) {
				return attemptPlace(b, arr_origin[shipId - 1], shipId - 1);
			}
			else {
				return false;
			}
		}
	}
	return false;
}


void MediocrePlayer::determineRadius(int &n, int &e,int  &w, int &s) { // pushes points around the point that was recently hit onto the attack queue
	int breadth;
	if (target_shot.r + 4 > game().rows()-1) {
		breadth = game().rows()-1 - target_shot.r;
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
	if (target_shot.r + 4 > game().cols()-1) {
		breadth = game().cols()-1 - target_shot.c;
	}
	else {
		breadth = 4;
	}
	e = breadth;
	if (target_shot.c - 4 < 0) {
		breadth =  target_shot.c;
	}
	else {
		breadth = 4;
	}
	w = breadth;
}
void MediocrePlayer:: addWillShoot(int n, int e, int w, int s) { // adds to queue for shooting
	for (int i = 1; i <= n; i++) {
		Point temp;
		temp.r = target_shot.r - i;
		temp.c = target_shot.c;
		will_shoot.push(temp);
	}
	for (int i = 1; i <= s; i++) {
		Point temp;
		temp.r = target_shot.r + i;
		temp.c = target_shot.c;
		will_shoot.push(temp);
	}
	for (int i = 1; i <= e; i++) {
		Point temp;
		temp.r = target_shot.r;
		temp.c = target_shot.c + i ;
		will_shoot.push(temp);
	}
	for (int i = 1; i <= w; i++) {
		Point temp;
		temp.r = target_shot.r - i;
		temp.c = target_shot.c;
		will_shoot.push(temp);
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
			if (!will_shoot.empty()) {
				if (will_shoot.front() == *it) { // if it is just pop it off
					will_shoot.pop();
				}
				else {
					Point p = will_shoot.front();
					will_shoot.pop();
					return p;
				}
			}
		}
		state = 1;
		return recommendAttack();
	}
}
void MediocrePlayer::recordAttackResult(Point p,bool validShot, bool shotHit, bool shipDestroyed, int shipId) {
	has_shot.push_back(p);
	if(state == 1) {
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

//*********************************************************************
//  GoodPlayer
//*********************************************************************

GoodPlayer::GoodPlayer(std::string nm, const Game& g)
	:Player(nm, g) , enemy(game())
{
	first = true;
	 nShips = game().nShips();
	 g_rows = game().rows();
	 g_cols = game().cols();
	 for (int i = 0; i < nShips; i++) {
		 alive[i] = true;
	 }
	 for (int i = 0; i < nShips; i++) {
		 lengths[i] = game().shipLength(i);
	 }
}
Point GoodPlayer::attackRand() {
	Point test;
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
void GoodPlayer::calculateDensity(Point p,int shipId,int length) { //calculates the problablity that there is a ship on each point 
	if (p.r >= g_rows || p.c >= g_cols) {
		return;

	}
		if (enemy.placeShip(p, shipId, HORIZONTAL, true)) {
			density[p] += 1;
			int i = 0;
			while (i < length - 1) {
				Point temp(p.r, p.c + i+1);
				density[temp] += 1;
				i++;
			}
		}
		if (enemy.placeShip(p, shipId, VERTICAL, true)) {
			density[p] += 1;
			int i = 0;
			while (i < length - 1) {
				Point temp(p.r + i + 1, p.c);
				density[temp] += 1;
				i++;
			}
		}
		if (enemy.get(p) == '.') {
			enemy.write(p);
		}
		Point q(p.r + 1, p.c);
		calculateDensity(q, shipId, length);
		
		Point l(p.r, p.c + 1);
		calculateDensity(l, shipId, length);
		
}
Point GoodPlayer::recommendAttack() {
	Point p;
	if (first) { // if first turn attack middle of the board
		p.r = game().rows() / 2;
		p.c = game().cols() / 2;
		first = false;
		return p;
	}
	for (int i = 0; i < nShips; i++) {
		if (alive[i]) {
			p.r = 0;
			p.c = 0;
			int length = lengths[i];
			calculateDensity(p, i, length);
		}
	}
	int very_dense = 0;
	Point ret;
	for (unordered_map<Point, int>::iterator it = density.begin(); it != density.end(); it++) { // loop through find biggest density point return that
		if (it->second > very_dense) {
			very_dense = it->second;
			ret = it->first;
		}
	}
	if (very_dense == 0) {
		return attackRand();
	}
	return ret;
}
void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {
	has_shot.push_back(p);
	enemy.record(p);
	if (shipDestroyed) {
		alive[shipId] = false;
	}
	density.clear();
	
	if (shotHit && !shipDestroyed) {
		lengths[shipId] -= 1;
		
		if (p.r > 0) {
			Point n(p.r - 1, p.c);
			if(enemy.get(n) != 'o')
				density[n] += 4;
		}
		if (p.c > 0) {
			Point w(p.r, p.c-1);
			if (enemy.get(w) != 'o')
				density[w] += 4;
		}
		if (p.r < g_rows-1) {
			Point n(p.r +1, p.c);
			if (enemy.get(n) != 'o')
				density[n] += 4;
		}
		if (p.c < g_cols-1) {
			Point n(p.r, p.c+1);
			if (enemy.get(n) != 'o')
				density[n] += 4;
		} 
	} 
	enemy.clear();
}
bool GoodPlayer::placeShips(Board& b) {
	for (int i = 0; i < nShips; i++) {
		bool success = false;
		while (!success) {
			int row = randInt(g_rows);
			int col = randInt(g_cols);
			Point place(row, col);
			if (!b.placeShip(place, i, HORIZONTAL, false)) {
				if (!b.placeShip(place, i, VERTICAL, false)) {
					;
				}
				else
					success = true;
			}
			else
				success = true;
		}
	}
	return true;
}
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
