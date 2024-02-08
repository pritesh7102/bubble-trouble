#include <simplecpp>

/* Bubble Vars */
const int BUBBLE_START_X = 250;
const int BUBBLE_START_Y = 50;
const int BUBBLE_DEFAULT_RADIUS = 10;
const int BUBBLE_RADIUS_THRESHOLD = 10;
const int BUBBLE_DEFAULT_VX = 100;
const int BUBBLE_DEFAULT_VY = 100;
const int BUBBLE_AY = 10;

class Bubble
{
private:
    Circle circle;  // the circle representing the bubble
    double vx, vy;  // velocity in x and y direction
    double ay;      // acceleraton in y direction
    Color color;    // color of the bubble

public:
    Bubble(double cx, double cy, double r, double vx_=BUBBLE_DEFAULT_VX, double vy_=BUBBLE_DEFAULT_VY, double ay_=BUBBLE_AY, Color color_=COLOR(0, 0, 255))
    {
        // Bubble constructor
        color = color_;
        circle = Circle(cx, cy, r);
        circle.setColor(color);
        circle.setFill(true);

        vx = vx_;
        vy = vy_;
        ay = ay_;
    }

    void nextStep(double t)
    {
        // move the bubble
        double new_x = circle.getX() + vx*t;
        double new_y = circle.getY() + vy*t;

        if ((vx < 0 && new_x < (0 + circle.getRadius())) // bounce along X direction at left border
            ||
            (vx > 0 && new_x > (WINDOW_X - circle.getRadius()))) // bounce along X direction at right border
        {

            vx = -vx;
            new_x = circle.getX() + vx*t;

        }
        if ((vy < 0 && new_y < (0 + circle.getRadius())) // bounce slong Y direction at top of canvas
             ||
             (vy > 0 && new_y > (PLAY_Y_HEIGHT - circle.getRadius()))) // bounce along Y direction at bottom margin of play
        {

           vy = -vy;
           new_y = circle.getY() + vy*t;

        }
        circle.moveTo(new_x, new_y);

        if(vy >= 0) // update vy to ensure parabolic path
        {
           vy += ay*t;
        }
        else vy = vy - ay*t;
    }

    // Check the collision b/w bubble & bullet
    bool hit_bullet(double t,Bullet bullet)
    {
        double new_x_bubble = circle.getX() + vx*t;
        double new_y_bubble = circle.getY() + vy*t;
        double bullet_x1_top = bullet.get_center_x() - bullet.get_width()/2;
        double bullet_y_top = bullet.get_center_y() - bullet.get_height()/2;
        double bullet_x2_top = bullet.get_center_x() + bullet.get_width()/2;
        double distance1 = sqrt(pow(bullet_x1_top - new_x_bubble,2) + pow(bullet_y_top - new_y_bubble,2));
        double distance2 = sqrt(pow(bullet_x2_top - new_x_bubble,2) + pow(bullet_y_top - new_y_bubble,2));

        if((distance1 <= circle.getRadius()) || (distance2 <= circle.getRadius()))
           return true;
        else
           return false;
    }

    //Check collision b/w bubble & shooter
    bool hit_shooter(double t, Shooter shooter)
    {
        double shooter_x = shooter.get_head_center_x();
        double head_center_y = shooter.get_head_center_y();
        double head_radius = shooter.get_head_radius();
        double new_x_bubble = circle.getX() + vx*t;
        double new_y_bubble = circle.getY() + vy*t;
        double body_left_x = shooter_x - shooter.get_body_width()/2.0;;
        double body_right_x = shooter_x + shooter.get_body_width()/2.0;;
        double body_top_y = head_radius + head_center_y;
        double dist_head_bubble = sqrt(pow(new_x_bubble - shooter_x,2) + pow(new_y_bubble-head_center_y,2));

        if(dist_head_bubble <= head_radius + circle.getRadius()) // collision from shooter's head
        {
           vx = -vx;
           vy = -vy;
           return true;
        }
        else if(body_top_y <= new_y_bubble) // collision from shooter's body
        {
           if(((new_x_bubble <= shooter_x) && ((body_left_x-new_x_bubble) <= circle.getRadius()))
              ||
              ((new_x_bubble >= shooter_x) && ((new_x_bubble-body_right_x) <= circle.getRadius())))
           {
              vx = -vx;
              vy = -vy;
              return true;
            }
           else return false;
        }
        else return false;
    }

    double get_radius()
    {
        // return the radius of the bubble
        return circle.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bubble
        return circle.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bubble
        return circle.getY();
    }

};
