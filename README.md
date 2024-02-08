# Bubble-Trouble
Bubble Trouble is a Bubble shooter game made in C++ as the final project of a course CS101 - Computer Programming and Utilization. It uses [simplecpp](https://www.cse.iitb.ac.in/~ranade/simplecpp/) library for graphics. 

## Instructions
* Click on Canvas using mouse to start the game
* Click again on canvas to begin level 1.
* After each level, click on screen to start next level.
* Keys - `a` : left, `d`: right, `w`: shoot bullet, `q`: exit game in middle of a level
* To exit game, either if your game is over or if you have won, you have to click on canvas using mouse and then you will exit.
## Features Implemented
* Bubble’s parabolic path with bouncing off the ground.
* 3 levels of game.
* Collision between bubble-bullet and bubble-shooter.
* As you finish all bubbles in a level, you advance to next level.
* A score, health and time Counter. Whenever a bullet hits a bubble, score increases by 1 and that bullet disappears. Whenever bubble hits the shooter, health decreases by 1. Each level has a time counter.
* If health becomes 0, then game is over and the health counter indicates you by changing its color.
* If time reaches its upper limit, then game is over and the time counter indicates you by changing its color.
* If you clear all 3 levels, you win the game!
* **Level 1** : simplest level with 2 small bubbles and they directly disappear when bullet hits them.
* **Level 2** : more and larger bubbles with slightly more speed. Bubbles split into smaller bubbles once when hit by bullet and on second hit they disappear.
* **Level 3** : even more larger bubbles that split into smaller bubbles on first hit and then smaller ones split into even more smaller bubbles on second hit. On third hit, they finally disappear. As bubbles become smaller, their speed increases.


### Demonstration Video link: 
https://drive.google.com/drive/folders/1394jDdUaBQ1LxHTez6ewrXV1oziZ7zgU?usp=sharing

This link contains two videos. First video includes general game in which user loses due to health going to zero with all 3 levels shown. In second video, user loses in level 2 due to time factor. My system is not so efficient, so videos might show some lagging, hope you won’t mind it.
