"# Battleship" 

This is an implementation of the classic game Battleship that allows you to play computer oppenents of varying difficulty in the terminal.



--Classes--
1. Board: This class maintains the board data structure. During a game, there will be an instance of the Board class for each player.

2. An AwfulPlayer:  attacks in a pattern and groups its ships up.

3. A HumanPlayer class:  Prompts the user to decide where to place ships, and where to attack. 

4. A MediocrePlayer : Uses the Hunt/ Target algorithm that randomly attacks until it hits a ship. Upon hitting a ship, "target" mode is activated and the player will shoot around that target point.

5. A GoodPlayer that uses density algorithms to calculate the probability of a ship being at a certain point.  The algorithm keeps track of where it has shot, and attempts to place all of the enemies' ships at the coordinates.  Using this information it will determine the place that a ship will most likely be, and target it.

6. A Game class: This class is used to run a complete game between two players.
