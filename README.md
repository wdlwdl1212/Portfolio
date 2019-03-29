compile:
g++ -std=c++11 game.c -o main
execution:
./main

Overview：
To develop the Four Connect game c11 programming language is used with console based interface. The console displays the game state after each move. There are four versions are implemented 1. MinMax vs Random (Disk Removal), 2. MinMax vs Random (Without Disk Removal), 3. MinMax vs Human and 4. Player vs Player. At the start of the game, it asks the user to select the type of the game.  The game is played between two users 1. Computer (MinMax), Computer (Random Move). It is allowed to MinMax player to remove its disk at the bottom after each 3rd turns and due to disk removal all the disks are moved to below one row to bottom. The MinMax player is depicted with character ‘O’ while Random player is depicted with character ‘X’. The game continue till it doesn’t find sequence of (four) ‘O’ or ‘X’ characters at horizontally, vertically or diagonally. The Depth of the game is defined as 2, it denotes the how the state is deep in the search, although depth may increase but the computation time increase too. 

Winners：
I have played the game several times, the ratio of winner game for MinMax player is high as compared to Random move. In conclusion, if I played this game 10 times, the 7 time MinMax player won the game while 3 times other player won the game. 
