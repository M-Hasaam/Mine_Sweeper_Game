#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;
using namespace sf;

Font font;

// Constants for game configuration

const int no_of_boxX = 9;
const int no_of_boxY = 9;

const int box_space = 100;
const int box_size = 90;
const int no_of_bomb = 10;

int flag_count = 0;

const int resolutionX = no_of_boxX * box_space;
const int resolutionY = no_of_boxY * box_space;

bool Game_over = false;

class BOX
{
public:
    RectangleShape R;

    Text text;

    int value;
    string st_value;

    bool bomb;
    bool opened;
    bool marked;
    bool blasted;

    int x_i, x_f, y_i, y_f; // initial , final poisitoin f box

    BOX()
    {
        bomb = false;
        opened = false;
        marked = false;
        blasted = false;

        value = 0;
        st_value = "";
    }

    void set_value(int v, int x, int y)
    {
        value = v;
        st_value = to_string(v);

        text.setFont(font); // Set the font
        // text.setString(to_string(v));             // Set the text string
        text.setCharacterSize(70);                // Set the character size (in pixels)
        text.setFillColor(Color::Black);          // Set the text color
        text.setStyle(Text::Bold | Text::Italic); // Set text style (optional)
        text.setPosition(32 + x * box_space, y * box_space);

        if (bomb)
            st_value = "X";

        if (v == 0)
            st_value = "";
    }

    void pressed()
    {
        if (!marked)
        {

            opened = true;

            R.setFillColor(Color::White);

            text.setString(st_value);

            if (bomb)
            {
                Game_over = true;
                R.setFillColor(Color::Red);
                blasted = true;
            }
        }
    }

    void all_blast()
    {
        opened = true;

        R.setFillColor(Color::Red);

        text.setString(st_value);

        blasted = true;
    }

    void mark_unmark()
    {
        if (!opened)
        {
            marked = (marked ? false : true);

            if (marked)
            {
                R.setFillColor(Color::Blue);
                flag_count++;
            }
            else if (!marked)
            {
                R.setFillColor(Color::Green);
                flag_count--;
            }
        }
    }
};

void GAME_LOOP(RenderWindow &window, BOX boxes[no_of_boxX][no_of_boxY], RectangleShape groundRectangle);
void First_GAME_LOOP(RenderWindow &window, BOX boxes[no_of_boxX][no_of_boxY], RectangleShape groundRectangle, Vector2f &skip);

void Track_Mouse_R(BOX boxes[no_of_boxX][no_of_boxY], int x, int y);
void Track_Mouse_R(BOX boxes[no_of_boxX][no_of_boxY], int x, int y, Vector2f &skip);
void Track_Mouse_L(BOX boxes[no_of_boxX][no_of_boxY], int x, int y);

void add_bomb(BOX boxes[no_of_boxX][no_of_boxY], Vector2f skip);

void give_color_and_place_to_the_boxes(BOX boxes[no_of_boxX][no_of_boxY]);

void open_boxes_at_3x3_of_box(BOX boxes[no_of_boxX][no_of_boxY], int i, int j);
void open_boxes_by_own_if_empty(BOX boxes[no_of_boxX][no_of_boxY]);
void open_boxex_near_skip(BOX boxes[no_of_boxX][no_of_boxY], Vector2f skip);

void help(BOX boxes[no_of_boxX][no_of_boxY]);

int main()
{
    srand(time(0));

    // Initialize the game window
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Mine Sweeper", Style::Close | Style::Titlebar); // Style refers to type of window
    window.setPosition(Vector2i(500, 200));

    if (!font.loadFromFile("Fonts/arial.ttf"))
    {
        cout << "Error: Could not load font!" << endl;
        return -1;
    }

    // Ground rectangle
    RectangleShape groundRectangle(Vector2f(resolutionX, resolutionY));
    groundRectangle.setPosition(0, 0);
    groundRectangle.setFillColor(Color(0, 200, 0));

    BOX boxes[no_of_boxX][no_of_boxY];

    give_color_and_place_to_the_boxes(boxes);

    Vector2f skip(-1, -1);

    First_GAME_LOOP(window, boxes, groundRectangle, skip);

    add_bomb(boxes, skip);

    help(boxes);

    boxes[int(skip.x)][int(skip.y)].pressed();

    open_boxex_near_skip(boxes, skip);

    // Main game loop
    GAME_LOOP(window, boxes, groundRectangle);

    return 0;
}

void GAME_LOOP(RenderWindow &window, BOX boxes[no_of_boxX][no_of_boxY], RectangleShape groundRectangle)
{
    // int ones = 0;
    unsigned long long loop_counter = 0;

    while (window.isOpen())
    {
        Event e; // Class For inputting Keyborad , Mouse , joystick operations
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }

            if (e.type == Event::MouseButtonPressed)
            {
                // Identify the mouse button
                if (e.mouseButton.button == Mouse::Left)
                {
                    cout << "Left mouse button clicked at position: ("
                         << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                    Track_Mouse_R(boxes, e.mouseButton.x, e.mouseButton.y);
                }
                else if (e.mouseButton.button == Mouse::Right)
                {
                    cout << "Right mouse button clicked at position: ("
                         << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                    Track_Mouse_L(boxes, e.mouseButton.x, e.mouseButton.y);
                }
            }
        }

        // Clear the window
        window.clear();

        // Draw ground
        window.draw(groundRectangle);

        for (int i = 0; i < no_of_boxX; i++)
        {
            for (int j = 0; j < no_of_boxY; j++)
            {
                window.draw(boxes[i][j].R);
                window.draw(boxes[i][j].text);
            }
        }

        if (Game_over)
        {
            loop_counter++;
            if (loop_counter / 150 > 0)
            {
                loop_counter = 0;

                cout << " GAME OVER \n";

                int ones = 0;

                for (int i = 0; i < no_of_boxX; i++)
                {
                    for (int j = 0; j < no_of_boxY; j++)
                    {

                        if (boxes[i][j].bomb && !boxes[i][j].blasted)
                        {
                            boxes[i][j].all_blast();
                            ones++;
                            break;
                        }
                    }
                    if (ones == 1)
                        break;
                }
            }
        }

        open_boxes_by_own_if_empty(boxes);

        // Display the updated window
        window.display();
    }
}

void First_GAME_LOOP(RenderWindow &window, BOX boxes[no_of_boxX][no_of_boxY], RectangleShape groundRectangle, Vector2f &skip)
{
    while (window.isOpen() && skip == Vector2f(-1, -1))
    {
        Event e; // Class For inputting Keyborad , Mouse , joystick operations
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }

            if (e.type == Event::MouseButtonPressed)
            {
                // Identify the mouse button
                if (e.mouseButton.button == Mouse::Left)
                {
                    cout << "Left mouse button clicked at position: ("
                         << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                    Track_Mouse_R(boxes, e.mouseButton.x, e.mouseButton.y, skip);
                }
                else if (e.mouseButton.button == Mouse::Right)
                {
                    cout << "Right mouse button clicked at position: ("
                         << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;
                }
            }
        }

        // Clear the window
        window.clear();

        // Draw ground
        window.draw(groundRectangle);

        for (int i = 0; i < no_of_boxX; i++)
        {
            for (int j = 0; j < no_of_boxY; j++)
            {
                window.draw(boxes[i][j].R);
                window.draw(boxes[i][j].text);
            }
        }

        // Display the updated window
        window.display();
    }
}

void add_bomb(BOX boxes[no_of_boxX][no_of_boxY], Vector2f skip)
{
    for (int i = 0; i < no_of_bomb; i++) // giving bomb
    {
        int r_X;
        int r_Y;
        do
        {
            r_X = rand() % (no_of_boxX);
            r_Y = rand() % (no_of_boxY);

        } while (boxes[r_X][r_Y].bomb || (skip == Vector2f(r_X, r_Y)));

        boxes[r_X][r_Y].bomb = true;
    }

    for (int x = 0; x < no_of_boxX; x++) // giving value
    {
        for (int y = 0; y < no_of_boxY; y++)
        {
            int count = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {

                    if (0 <= x + i && x + i < no_of_boxX && 0 <= y + j && y + j < no_of_boxY)
                    {
                        if (boxes[x + i][y + j].bomb == true)
                        {
                            count++;
                        }
                    }
                }
            }
            boxes[x][y].set_value(count, x, y);
        }
    }
}

void Track_Mouse_R(BOX boxes[no_of_boxX][no_of_boxY], int x, int y)
{

    for (int i = 0; i < no_of_boxX; i++)
    {
        for (int j = 0; j < no_of_boxY; j++)
        {
            if (boxes[i][j].x_i < x && x < boxes[i][j].x_f && boxes[i][j].y_i < y && y < boxes[i][j].y_f)
            {

                cout << " BOX[" << i + 1 << "][" << j + 1 << "] with value " << boxes[i][j].value << endl;

                boxes[i][j].pressed();

                if (!boxes[i][j].bomb && !boxes[i][j].marked)
                {
                    open_boxes_at_3x3_of_box(boxes, i, j);
                }
            }
        }
    }
}

void Track_Mouse_R(BOX boxes[no_of_boxX][no_of_boxY], int x, int y, Vector2f &skip)
{

    for (int i = 0; i < no_of_boxX; i++)
    {
        for (int j = 0; j < no_of_boxY; j++)
        {
            if (boxes[i][j].x_i < x && x < boxes[i][j].x_f && boxes[i][j].y_i < y && y < boxes[i][j].y_f)
            {

                cout << " BOX[" << i + 1 << "][" << j + 1 << "] with value " << boxes[i][j].value << endl;

                skip = Vector2f(i, j);

                cout << "SKIPED:" << skip.x + 1 << "," << skip.y + 1 << endl;

                boxes[i][j].pressed();
            }
        }
    }
}

void Track_Mouse_L(BOX boxes[no_of_boxX][no_of_boxY], int x, int y)
{

    for (int i = 0; i < no_of_boxX; i++)
    {
        for (int j = 0; j < no_of_boxY; j++)
        {
            if (boxes[i][j].x_i < x && x < boxes[i][j].x_f && boxes[i][j].y_i < y && y < boxes[i][j].y_f)
            {

                cout << " BOX[" << i + 1 << "][" << j + 1 << "] with value " << boxes[i][j].value << endl;

                boxes[i][j].mark_unmark();
            }
        }
    }
}

void open_boxes_at_3x3_of_box(BOX boxes[no_of_boxX][no_of_boxY], int i, int j)
{

    int count = 0;

    for (int a = -1; a <= 1; a++)
    {
        for (int b = -1; b <= 1; b++)
        {
            if (0 <= i + a && i + a < no_of_boxX && 0 <= j + b && j + b < no_of_boxY)
            {
                if (boxes[i + a][j + b].bomb == false)
                {
                    count++;
                }
            }
            else
            {
                count++;
            }
        }
    }

    if (5 < count)
    {
        for (int a = -1; a <= 1; a++)
        {
            for (int b = -1; b <= 1; b++)
            {
                if (0 <= i + a && i + a < no_of_boxX && 0 <= j + b && j + b < no_of_boxY)
                    if (boxes[i + a][j + b].bomb == false)
                    {
                        boxes[i + a][j + b].pressed();
                    }
            }
        }
    }
}

void give_color_and_place_to_the_boxes(BOX boxes[no_of_boxX][no_of_boxY])
{
    for (int i = 0; i < no_of_boxX; i++)
    {
        for (int j = 0; j < no_of_boxY; j++)
        {
            boxes[i][j].R.setSize(Vector2f(box_size, box_size));
            boxes[i][j].R.setPosition(i * box_space + 5, j * box_space + 5);
            boxes[i][j].R.setFillColor(Color::Green);

            boxes[i][j].x_i = i * box_space + 5;
            boxes[i][j].x_f = i * box_space + 5 + box_size;

            boxes[i][j].y_i = j * box_space + 5;
            boxes[i][j].y_f = j * box_space + 5 + box_size;
        }
    }
}

void help(BOX boxes[no_of_boxX][no_of_boxY])
{
    for (int x = 0; x < no_of_boxX; x++)
    {
        for (int y = 0; y < no_of_boxY; y++)
        {
            cout << " ";
            cout << boxes[y][x].value;
            cout << (boxes[y][x].bomb ? "x" : "_");
        }
        cout << endl;
    }
}

void open_boxes_by_own_if_empty(BOX boxes[no_of_boxX][no_of_boxY])
{
    for (int i = 0; i < no_of_boxX; i++)
    {
        for (int j = 0; j < no_of_boxY; j++)
        {
            if (boxes[i][j].value == 0 && boxes[i][j].opened)
            {
                for (int a = -1; a <= 1; a++)
                {
                    for (int b = -1; b <= 1; b++)
                    {
                        if (0 <= i + a && i + a < no_of_boxX && 0 <= j + b && j + b < no_of_boxY)
                        {
                            if (!boxes[i + a][j + b].opened)
                            {
                                boxes[i + a][j + b].pressed();
                            }
                        }
                    }
                }
            }
        }
    }
}

void open_boxex_near_skip(BOX boxes[no_of_boxX][no_of_boxY], Vector2f skip)
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (0 <= skip.x + i && skip.x + j < no_of_boxX && 0 <= skip.y + j && skip.y + j < no_of_boxY)
            {
                if (!boxes[int(skip.x) + i][int(skip.y) + j].bomb)
                {
                    boxes[int(skip.x) + i][int(skip.y) + j].pressed();
                }
            }
        }
    }
}