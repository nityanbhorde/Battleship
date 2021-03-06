
#include "Game/Game.h"
#include "Player/Player.h"
#include "Board/Board.h"
#include <iostream>
#include <string>

using namespace std;


bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

int main()
{
    const int NTRIALS = 10;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A good player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a mediocre and an good player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(3, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("good", "Good Guy Gary", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        int goodWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(5, 5);
            addStandardShips(g);
            Player* p1 = createPlayer("good", "Good guy Gary", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
			Player* winner;
			if (g.play(p1, p2, false) == p1) {
				winner = p1;
			}
			else {
				winner = p2;
			}
            if (winner == p1)
                goodWins++;
            delete p1;
            delete p2;
        }
        cout << "The good player won " << goodWins << " out of "
             << NTRIALS << " games." << endl;
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }
}