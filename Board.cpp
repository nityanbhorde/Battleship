#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;



Board::Board(const Game& g)
 : m_game(g)
{
	for (int r = 0; r < m_game.rows(); r++) {
 		for (int c = 0; c < m_game.cols(); c++) {
			m_board[r][c] = '.';
		}
	}
}
Board::~Board() {

}
void Board::clear()
{
	for (int r = 0; r < m_game.rows(); r++) {
		for (int c = 0; c < m_game.cols(); c++) {
			m_board[r][c] = '.';
		}
	}
}

void Board::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
				m_board[r][c] = 'b';
            }
}

void Board::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
			if (m_board[r][c] == 'b') {
				m_board[r][c] = '.';
			}
        }
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	
	if (shipId < 0 || shipId >= m_game.nShips()) { // if invalid ship id return false
		return false;
	}
	int length = m_game.shipLength(shipId);
	char symbol = m_game.shipSymbol(shipId);
	int row = topOrLeft.r;
	int col = topOrLeft.c;
	
	if (!m_game.isValid(topOrLeft)) { // if part of ship is out of bounds return false 
		return false;
	}
	char history[5];// keep track of what we may  overwrite on board in case of errors
	if (dir == 0) {
		if (col + length-1 > m_game.cols() - 1) {
			return false;
		}
		int i = 0;
		while( i < length){
			if ((m_board[row][col + i]) == '.') { // if its anything but empty return false, otherwise assign with symbol
				history[i] = m_board[row][col + i];
				m_board[row][col + i] = symbol;
				i++;
			}
			else {
				while (i > 0) {
					i--;
					m_board[row][col + i] = history[i];
				}
				return false;
			}
		}
	}
	else { // for vertical placement
		if (row + length -1> m_game.rows() - 1) {
			return false;
		}
		int i = 0;
		while (i < length) {
			if ((m_board[row + i][col]) == '.') { // undo if space isnt empty
				history[i] = m_board[row + i][col];
				m_board[row + i][col] = symbol;
				i++;
			}
			else {
				while (i > 0) {
					i--;
					m_board[row + i][col] = history[i];
				}
				return false;
			}
		}
	}
	return true;
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	if (shipId < 0 || shipId >= m_game.nShips()) { // if invalid ship id return false
		return false;
	}
	int length = m_game.shipLength(shipId);
	char history[10];
	int row = topOrLeft.r;
	int col = topOrLeft.c;
	char symbol = m_game.shipSymbol(shipId);
	
	if (dir == 0) {
		for (int i = 0; i < length; i++) {
			if (m_board[row][col + i] != symbol) { // if we dont have our symbol, undo, otherwise replace it with the empty space
				while (i > 0) {
					i--;
					m_board[row][col + i] = history[i];
				}
				return false;
			}
			else {
				history[i] = m_board[row][col + i];
				m_board[row][col + i] = '.';
			}
		}
	}
	else {
		for (int i = 0; i < length; i++) {
			if (m_board[row + i][col] != symbol) {
				while (i > 0) {
					i--;
					m_board[row + i][col] = history[i];
				} 
				return false;
			}
			else {
				history[i] = m_board[row + i][col];
				m_board[row+i][col] = '.';
			}
		}
	}
	return true;
}

void Board::display(bool shotsOnly) const // displays either only shots or the regular board
{
	int rows = m_game.rows();
	int cols = m_game.cols();
	if (!shotsOnly) {
		cout << "  ";
		int numShips = m_game.nShips();
		
		for (int i = 0; i < cols; i++) {
			cout << i;
		}
		cout << endl;
		
		for (int r = 0; r < rows; r++) {
			cout << r << " ";
			for (int c = 0; c < cols; c++) {
				cout << m_board[r][c];
			}
			cout << endl;
		}
	}
	else {
		cout << "  ";
		int numShips = 10;
		for (int i = 0; i < numShips; i++) {
			cout << i;
		}
		cout << endl;
		for (int r = 0; r < rows; r++) {
			cout << r << " ";
			for (int c = 0; c < cols; c++) {
				if (m_board[r][c] != 'X' && m_board[r][c] != 'o') {
					cout << '.';
				}
				else {
					cout << m_board[r][c];
				}
			}
			cout << endl;
		}
	}
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	shotHit = false;
	shipDestroyed = false;
	int row = p.r;
	int col = p.c;
	if (!m_game.isValid(p)) {
		return false;
	}
	if (m_board[row][col] == 'X' || m_board[row][col] == 'o') {
		return false;
	}
	char temp_symbol = m_board[row][col];
	if (m_board[row][col] != '.' ) { // if we hit a ship mark it
		shotHit = true;
		m_board[row][col] = 'X';
	
	}else {
		shotHit = false;
		m_board[row][col] = 'o';
	}
	return true;
}

bool Board::allShipsDestroyed() const
{
	for (int r = 0; r < m_game.rows(); r++) {
		for (int c = 0; c < m_game.cols(); c++) {
			if (m_board[r][c] != '.' && m_board[r][c] != 'X' && m_board[r][c] != 'o' ) {
				return false;
			}
		}
	}
	return true;
}
