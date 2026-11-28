#include "MicroBit.h"

MicroBit uBit;            // The MicroBit object

const int MAX_LENGTH = 4; // The maximum length of the snake
int sleep_period = 400;   // The initial sleep time
const int MIN_LENGTH = 1; // The minimum length of the snake

// Create a structure for the coordinates of each pixel of the snake
struct pixel
{
    int x, y;
};

// Create a snake that consists of MAX_LENGTH + 1 pixels.
// The actual (visible) length of the snake is MAX_LENGTH,
// i.e., from [0] to [MAX_LENGTH-1]. The last position
// (i.e., [MAX_LENGTH]) holds the previous position of the tail,
// which is erased when the snake moves.
struct pixel snake[MAX_LENGTH + 1];

// Initialise the coordinates of each pixel of the snake,
// so that the head of the snake is in position (new_head.x, new_head.y)
// and the remaining co-ordinates are set to -1.
void initialize_snake(struct pixel new_head)
{
    int i;
    snake[0] = new_head;

    for (i = 1; i < MIN_LENGTH + 1; i++)
    {
        snake[i].x = -1;
        snake[i].y = -1;
    }
}

// Move the snake. Its head (element [0]) moves to the
// new coordinates (new_head.x, new_head.y)
// while pixel [i] of the snake moves to the co-ordinates that
// pixel [i-1] of the snake used to occupy.
void move_snake(struct pixel new_head)
{
    int i;

    for (i = MAX_LENGTH; i > 0; i--)
        snake[i] = snake[i - 1];

    snake[0] = new_head;
}

// Turn on pixel [0] of the snake (the head) and turn off pixel [MAX_LENGTH] of the snake,
// where the tail (the last pixel of the snake) used to be in the previous step.
void display_snake()
{
    // Move the head forward
    if (snake[0].x >=0 && snake[0].y >= 0)
        uBit.display.image.setPixelValue(snake[0].x, snake[0].y, 15);

    // Erase the tail
    if (snake[MAX_LENGTH].x >= 0 && snake[MAX_LENGTH].y >= 0 )
        uBit.display.image.setPixelValue(snake[MAX_LENGTH].x, snake[MAX_LENGTH].y, 0);
}

int main()
{   
    // Initialise the coordinates of the snake head
    struct pixel snake_head = {0, 0};

    // Initialise the micro:bit
    uBit.init();
    // Set display mode to greyscale
    uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
    // Reveal the head of the snake at the top-left corner of the 5x5 display
    initialize_snake(snake_head);

    while (1)
    {
        // Print the snake
        display_snake();
        // Wait for 100 ms
        uBit.sleep(sleep_period);

        // Consider the edge conditions, i.e., (4,0), (4,4), (0,4), (0,0)
        // and update the x and y co-ordinates accordingly,
        // so that the head of the snake changes direction when it reaches 
        // a corner of the 5x5 display.
        if (snake_head.x < 4 && snake_head.y == 0) // The head moves from the left to the right of the top row
            snake_head.x++;
        else
        {
            if (snake_head.x == 4 && snake_head.y < 4) // The head moves from the top to the bottom of the right-most column
                snake_head.y++;
            else
            {
                if (snake_head.x > 0 && snake_head.y == 4) // The head moves from the right to the left of the bottom row
                    snake_head.x--;
                else
                {
                    if (snake_head.x == 0 && snake_head.y > 0) // The head moves from the bottom to the top of the left-most column
                        snake_head.y--;
                }
            }
        }

        // Calculate the position of each body part of the snake,
        // now that the position of its head is known.
        move_snake(snake_head);
    }
}

