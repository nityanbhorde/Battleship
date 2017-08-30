"# Battleship" 

This is an implementation of the classic game Battleship that allows you to play computer oppenents of varying difficulty in the terminal.



--Classes--
1. Board: This class maintains the board data structure. During a game,
there will be an instance of the Board class for each player.

2. A Player abstract base class: This base class specifies the interface for all players. 

3. An AwfulPlayer class: attacks in a pattern and groups its ships up.

4. A HumanPlayer class: 
 Prompts the user to decide where to place ships, and where to attack. 

5. A MediocrePlayer : Uses the Hunt/ Target algorithm that randomly attacks until it hits a ship. Upon hitting a ship, "target" mode is activated and the player will shoot around that target point.

6. A GoodPlayer that uses density functions to calculate the probability of a ship being at a certain point.

7. A Game class: This class is used to run a complete game between two players.
