# TicTacToeX

Tic Tac Toe X isn't just a common game, it is a well known game but extended to five players on the same board.
It is an experiment to learn new C++ 20 features and it will keep evolving, receiving updates from the latest versions and improvements.

The NPC player calculates its advantage position using a sum calculation, simple like this, where this position will result in a sum that brings victory for him or avoid another winner.
# 1 Boards

The board design is based on the amount of lines for each player. For instance, the common board has 2 horizontal lines and 2 vertical lines, so there is one line in each direction for each player. Expanding this thinking, if you add another player you can just add two more lines.

 - Commom board (3x3 - two players)
<pre>
   Order           Matrix
 1 | 2 | 3       00| 01| 02
---|---|---     ---|---|---
 4 | 5 | 6       10| 11| 12 
---|---|---     ---|---|---
 7 | 8 | 9       20| 21| 22
</pre>

 - Three players board (4x4)
<pre>
 1 | 2 | 3 | 4 
---|---|---|---
 5 | 6 | 7 | 8 
---|---|---|---
 9 | 10| 11| 12
---|---|---|---
 13| 14| 15| 16 
 </pre>
 
 - Four players board (5x5)
 <pre>
 1 | 2 | 3 | 4 | 5
---|---|---|---|---
 6 | 7 | 8 | 9 | 10
---|---|---|---|---
 11| 12| 13| 14| 15
---|---|---|---|---
 16| 17| 18| 19| 20
---|---|---|---|---
 21| 22| 23| 24| 25
 </pre>
 
 - Five players board (6x6)
 <pre>
 1 | 2 | 3 | 4 | 5 | 6
---|---|---|---|---|---
 7 | 8 | 9 | 10| 11| 12
---|---|---|---|---|---
 13| 14| 15| 16| 17| 18
---|---|---|---|---|---
 19| 20| 21| 22| 23| 24
---|---|---|---|---|---
 25| 26| 27| 28| 29| 30
---|---|---|---|---|---
 31| 32| 33| 34| 35| 36
 </pre>
 
 ## 1.1 Board structure
 
To keep the algorithm as simple as possible, which means not overthinking treatment on the edges, a fault border was created to avoid any lack of position calculation. This fault border increases the board size by 2 sections, so a common board will be from 3x3 to 5x5. Something like this:

<pre>
   |   |   |   |   |   |
---|---|---|---|---|---|---
   |   |   |   |   |   |
---|---|---|---|---|---|---
   |   | 00| 01| 02|   | 
---|---|---|---|---|---|---
   |   | 10| 11| 12|   | 
---|---|---|---|---|---|---
   |   | 20| 21| 22|   | 
---|---|---|---|---|---|---
   |   |   |   |   |   |  
---|---|---|---|---|---|---
   |   |   |   |   |   |
</pre>
