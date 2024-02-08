#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include <sstream>

/* Simulation Vars */
const double STEP_TIME = 0.02;

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME))
        {
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(int level)
{
    // create bubbles in the game according to each level
    vector<Bubble> bubbles;
    if(level == 1)
    {
       bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX*2, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(255,105,180)));
       bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX*2, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(255,105,180)));
    }
    else if(level == 2)
    {
       bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, -BUBBLE_DEFAULT_VX*2, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(105,40,245)));
       bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX*2, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(105,40,245)));
       bubbles.push_back(Bubble(WINDOW_X*3.0/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX*2, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(105,40,245)));
    }
    else if(level == 3)
    {
       bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, -BUBBLE_DEFAULT_VX*3, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(180,20,150)));
       bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX*3, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(180,20,150)));
       bubbles.push_back(Bubble(WINDOW_X*3.0/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX*3, BUBBLE_DEFAULT_VY, BUBBLE_AY, COLOR(180,20,150)));
    }
    return bubbles;
}

vector<Bubble> create_sub_bubbles(double x_, double y_, double r_, int level)
{
    //Create smaller bubbles in higher levels
    vector<Bubble> sub_bubbles;
    if(level == 2)
    {
    sub_bubbles.push_back(Bubble(x_, y_, r_/2.0, -BUBBLE_DEFAULT_VX*3, 0, BUBBLE_AY*4, COLOR(105,40,245)));
    sub_bubbles.push_back(Bubble(x_, y_, r_/2.0, BUBBLE_DEFAULT_VX*3, 0, BUBBLE_AY*4, COLOR(105,40,245)));
    }
    else if(level == 3)
    {
    sub_bubbles.push_back(Bubble(x_, y_, r_/2.0, -BUBBLE_DEFAULT_VX*4, 0, BUBBLE_AY*5, COLOR(180,20,150)));
    sub_bubbles.push_back(Bubble(x_, y_, r_/2.0, BUBBLE_DEFAULT_VX*4, 0, BUBBLE_AY*5, COLOR(180,20,150)));
    }
    return sub_bubbles;
}

//update collision of bullet and bubble
void bubble_hit_bullet(vector<Bubble> &bubbles, vector<Bullet> &bullets, int &game_level, string &msg_score, int &scoreCount){
    for(unsigned int i = 0; i < bullets.size(); i++)
    {
        for(unsigned int j = 0; j < bubbles.size(); j++)
        {
           if(bubbles[j].hit_bullet(STEP_TIME,bullets[i]))
           {
               if(bubbles[j].get_radius() == BUBBLE_DEFAULT_RADIUS) //erase if radius is default radius
               {
                  bullets.erase(bullets.begin() + i);
                  bubbles.erase(bubbles.begin() + j);
                  scoreCount++;
                  stringstream score_; // update score count
                  score_ << scoreCount;
                  score_ >> msg_score;
               }
               else  // if bubbles are larger than default
               {
                  double bubble_x = bubbles[j].get_center_x();
                  double bubble_y = bubbles[j].get_center_y();
                  double bubble_r = bubbles[j].get_radius();
                  vector<Bubble> sub_bubbles = create_sub_bubbles(bubble_x,bubble_y,bubble_r,game_level);
                  bullets.erase(bullets.begin() + i);
                  bubbles.erase(bubbles.begin() + j);
                  bubbles.push_back(sub_bubbles[0]);  // split into smaller bubbles
                  bubbles.push_back(sub_bubbles[1]);
                  scoreCount++;    // update score count
                  stringstream score_;
                  score_ << scoreCount;
                  score_ >> msg_score;
               }
           }
        }
    }

    if(bubbles.size() == 0) game_level++; // level up, when all bubbles cleared of previous level are cleared

}

// collision of bubble and shooter
void bubble_hit_shooter(vector<Bubble> &bubbles, Shooter shooter, string &msg_health,int &healthCount)
{
    for(unsigned int i = 0; i < bubbles.size(); i++)
    {
        if(bubbles[i].hit_shooter(STEP_TIME,shooter) && healthCount > 0)
        {
           healthCount--; // reduce health
           stringstream health_;
           health_ << healthCount;
           health_ >> msg_health;
        }
    }
}

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    // Messages for user
    Text Message(WINDOW_X/2.0, PLAY_Y_HEIGHT/2.0, "START GAME");
    Message.setColor(COLOR(255,45,75));
    getClick();
    Message.setMessage("Level 1!");
    getClick();
    Message.hide();

    // string for cmd from user
    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // time counter
    string timeCount_str = "0";
    int timeCount = 0;
    double pseudo_time = 0;
    Text timeCounter(LEFT_MARGIN, TOP_MARGIN, "Time: " + timeCount_str + "/50");

    // health counter
    string msg_health = "3";
    int healthCount = 3;
    Text healthCounter(WINDOW_X-50, TOP_MARGIN, "Health: " + msg_health + "/3");

    // score counter
    string msg_score("0");
    int scoreCount = 0;
    Text scoreCounter(WINDOW_X-50, BOTTOM_MARGIN, "Score: " + msg_score);

    // variable to keep track of game level
    int game_level = 1;
    Text level(WINDOW_X/2.0, BOTTOM_MARGIN, "Level: 1/3");

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles(game_level);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    bool game_over = false;

    // Main game loop for level 1
    while (game_level == 1 && !game_over)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // Update Time Counter
        pseudo_time += 0.1;
        timeCount = pseudo_time;
        stringstream time_;
        time_ << timeCount;
        time_ >> timeCount_str;
        timeCounter.setMessage("Time: " + timeCount_str + "/50");

        // Collision between bullet and bubble
        bubble_hit_bullet(bubbles,bullets,game_level,msg_score,scoreCount);

        // Collision between bubble and shooter
        bubble_hit_shooter(bubbles,shooter,msg_health,healthCount);

        // Update Health Counter
        healthCounter.setMessage("Health: " + msg_health + "/3");

        // Update Score Counter
        scoreCounter.setMessage("Score: " + msg_score);

        if(healthCount == 0)  // game_over if health is zero
        {
           healthCounter.setColor(COLOR(255,0,0));
           healthCounter.imprint();
           game_over = true;
        }

        if(timeCount == 50)  // game_over due to lack of time
        {
           timeCounter.setColor(COLOR(255,0,0));
           timeCounter.imprint();
           game_over = true;
        }
        wait(STEP_TIME);
    }

    if(game_over)  // return after getclick if game_over
    {
       Text Game_over(WINDOW_X/2.0, PLAY_Y_HEIGHT/2.0,"GAME OVER");
       getClick();
       return 0;
    }

    // message for level 2
    Text level_2(WINDOW_X/2.0, PLAY_Y_HEIGHT/2.0, "Level 2!");
    level_2.setColor(COLOR(20,40,245));
    getClick();
    bubbles = create_bubbles(game_level);
    timeCount = 0;  // reset time to zero
    pseudo_time = 0;
    healthCount = 3;  // reset health count
    stringstream health_2;
    health_2 << healthCount;
    health_2 >> msg_health;
    healthCounter.setMessage("Health: " + msg_health + "/3");
    level_2.hide();
    level.setMessage("Level: 2/3");

    // main game loop for level 2
    while (game_level == 2 && !game_over)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // Update Time Counter
        pseudo_time += 0.1;
        timeCount = pseudo_time;
        stringstream time_;
        time_ << timeCount;
        time_ >> timeCount_str;
        timeCounter.setMessage("Time: " + timeCount_str + "/50");

        // Collision between bullet and bubble
        bubble_hit_bullet(bubbles,bullets,game_level,msg_score,scoreCount);

        // Collision between bubble and shooter
        bubble_hit_shooter(bubbles,shooter,msg_health,healthCount);

        // Update Health Counter
        healthCounter.setMessage("Health: " + msg_health + "/3");

        // Update Score Counter
        scoreCounter.setMessage("Score: " + msg_score);

        if(healthCount == 0)  // game_over if health is zero
        {
           healthCounter.setColor(COLOR(255,0,0));
           healthCounter.imprint();
           game_over = true;
        }

        if(timeCount == 50)  // game_over due to time lack
        {
           timeCounter.setColor(COLOR(255,0,0));
           timeCounter.imprint();
           game_over = true;
        }
        wait(STEP_TIME);
    }

    if(game_over)  // return after getclick if game_over
    {
       Text Game_over(WINDOW_X/2.0, PLAY_Y_HEIGHT/2.0,"GAME OVER");
       getClick();
       return 0;
    }

    // message for level 3
    Text level_3(WINDOW_X/2.0, PLAY_Y_HEIGHT/2.0, "Level 3!");
    level_3.setColor(COLOR(150,45,45));
    getClick();
    bubbles = create_bubbles(game_level);
    timeCount = 0;  // reset time counter
    pseudo_time = 0;
    healthCount = 3;  // reset health count
    stringstream health_3;
    health_3 << healthCount;
    health_3 >> msg_health;
    healthCounter.setMessage("Health: " + msg_health + "/3");
    level_3.hide();
    level.setMessage("Level: 3/3");

    // main game loop for level 3
    while (game_level == 3 && !game_over)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // Update Time Counter
        pseudo_time += 0.1;
        timeCount = pseudo_time;
        stringstream time_;
        time_ << timeCount;
        time_ >> timeCount_str;
        timeCounter.setMessage("Time: " + timeCount_str + "/50");

        // Collision between bullet and bubble
        bubble_hit_bullet(bubbles,bullets,game_level,msg_score,scoreCount);

        // Collision between bubble and shooter
        bubble_hit_shooter(bubbles,shooter,msg_health,healthCount);

        // Update Health Counter
        healthCounter.setMessage("Health: " + msg_health + "/3");

        // Update Score Counter
        scoreCounter.setMessage("Score: " + msg_score);

        if(healthCount == 0)  // game_over if health is zero
        {
           healthCounter.setColor(COLOR(255,0,0));
           healthCounter.imprint();
           game_over = true;
        }

        if(timeCount == 50)  // game_over due to lack of time
        {
           timeCounter.setColor(COLOR(255,0,0));
           timeCounter.imprint();
           game_over = true;
        }
        wait(STEP_TIME);
    }

    if(game_over) // return after getclick if game_over
    {
       Text Game_over(WINDOW_X/2.0, PLAY_Y_HEIGHT/2.0,"GAME OVER");
       getClick();
       return 0;
    }

    // If your game is till on, congrats you have won!
    Text you_won(WINDOW_X/2.0, PLAY_Y_HEIGHT/2.0,"YOU WON!");
    you_won.setColor(COLOR(100,200,100));
    scoreCounter.setColor(COLOR(100,200,100));
    getClick();

    return 0;
}
