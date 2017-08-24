#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;


void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

bool Game::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}
int Game :: lookUp(char key) const {
	for (int i = 0; i < counter; i++) {
		if (key == ship_arr[i]->m_symbol) {
			return i;
		}
	}
}
Point Game::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}


Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
	counter = 0;
	m_rows = nRows;
	m_cols = nCols;
}

Game::~Game()
{
	for (int i = 0; i < counter; i++) {
		delete ship_arr[counter];
   }
}

int Game::rows() const
{
	return m_rows;
}

int Game::cols() const
{
	return m_cols;
}


bool Game::addShip(int length, char symbol, string name) // attempts to add a new type of ship to the game and put in our array
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
	Ship * temp_ship = new Ship(name,symbol,length);
	ship_arr[counter] = temp_ship;
	counter++;
	return true;
}

int Game::nShips() const
{
	return counter;
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
	return ship_arr[shipId]->m_length;
}

char Game::shipSymbol(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return ship_arr[shipId]->m_symbol;
}
string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
	return ship_arr[shipId]->m_name;
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause) // attempts to run a game between two players
{
	if (p1 == nullptr || p2 == nullptr || nShips() == 0)
		return nullptr;
	Board b1(*this);
	Board b2(*this);
	if (!(p1->placeShips(b1)) || !(p2->placeShips(b2))) {
		return nullptr;
	}
	while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed()) {// while there is at least one ship on each board, run the game!
		bool shotHit, shipDestroyed;
		int shipId;
		if (p1->isHuman()) {
			b2.display(true);
			Point attack = p1->recommendAttack();
			bool valid;
			valid = b2.attack(attack, shotHit, shipDestroyed, shipId); // player ones turn 
			p1->recordAttackResult(attack, valid, shotHit, shipDestroyed, shipId);
			b2.display(true);
		}
		else {
			b2.display(false);
			Point attack = p1->recommendAttack();
			bool valid;
			valid = b2.attack(attack, shotHit, shipDestroyed, shipId);
			p1->recordAttackResult(attack, valid, shotHit, shipDestroyed, shipId);
			b2.display(false);
		}
		if (p2->isHuman()) {
			bool shotHit1, shipDestroyed1;
			int shipId1; // p2 turn 
			b1.display(true);
			Point attack1 = p2->recommendAttack();
			bool valid1;
			valid1 = b1.attack(attack1, shotHit1, shipDestroyed1, shipId1);
			p2->recordAttackResult(attack1, valid1, shotHit1, shipDestroyed1, shipId1);
			b1.display(true);
		}
		else {
			bool shotHit1, shipDestroyed1;
			int shipId1; // p2 turn 
			b1.display(false);
			Point attack1 = p2->recommendAttack();
			bool valid1;
			valid1 = b1.attack(attack1, shotHit1, shipDestroyed1, shipId1);
			p2->recordAttackResult(attack1, valid1, shotHit1, shipDestroyed1, shipId1);
			b1.display(false);
		}
	}
	if (b1.allShipsDestroyed()) {
		if (p1->isHuman()) {
			b2.display(false);
		}
		return p2;
	}
	if (b2.allShipsDestroyed()) {
		if(p2->isHuman())
			b1.display(false);
		return p1;
	}
	
}