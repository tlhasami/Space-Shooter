#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

// use vector for eneies using types

Texture fire_T, jet_T, rock_T, fire2_T, enemy_T, enemy_2_T, enemy_3_T;
Texture Td1, Td2;

Texture B1, B2, B3;
Sprite Bg1, Bg2, Bg3;

Texture start_T, pause_T, exit_T;
Sprite start_S, pause_S, exit_S, S_pause;

Music M1;

SoundBuffer Sb1, Sb2;
Sound Sd1, Sd2, Sd3;

Font font;

const int resolution_X = 960;
const int resolution_Y = 580;

int score_count = 0;
bool game_paused = false;
bool again = false;

bool jet_destroyed = false, jet_explosion_ended = false;

int level = 0;

class Enemy;

class Bullet
{
public:
    Sprite S;
    static Texture T;
    Vector2f Pos, Vel;

    float maxVel = 100;

    int type = 1;

    float length = T.getSize().x;
    float y_axis = T.getSize().y;
    float x_axis = y_axis / 2.0f;

    Bullet(Vector2f jet_pos = Vector2f(0, 0));

    Bullet(const Sprite &enemySprite, const Vector2f &enemy_vel) : type(2)
    {

        if (fire2_T.getSize().x == 0)
        {
            cerr << "Error: Global texture not loaded before creating a Bullet object!" << endl;
            exit(0);
        }

        // Set the bullet sprite texture
        S.setTexture(fire2_T);

        // Set the origin of the bullet sprite to its center
        S.setOrigin(fire2_T.getSize().x / 2.0f, fire2_T.getSize().y / 2.0f);

        // Get the enemy jet's center using its global bounds
        FloatRect enemyBounds = enemySprite.getGlobalBounds();
        Vector2f enemyCenter = Vector2f(
            enemyBounds.left + enemyBounds.width / 2.0f,
            enemyBounds.top + enemyBounds.height / 2.0f);

        // Get the enemy's rotation (angle in degrees)
        float enemy_angle = enemySprite.getRotation();

        // Calculate the angle in radians for positioning and velocity
        float angleRad = (enemy_angle - 90) * M_PI / 180.0f; // Convert angle to radians, adjust for alignment

        // Offset the bullet's starting position based on the jet's center and angle
        Pos = enemyCenter + Vector2f(cos(angleRad) * enemyBounds.height / 2.0f, sin(angleRad) * enemyBounds.height / 2.0f);

        // Set the bullet's velocity, adding the enemy's velocity
        float bulletSpeed = 200.0f; // Define the bullet's speed
        Vel = Vector2f(cos(angleRad) * bulletSpeed, sin(angleRad) * bulletSpeed) + enemy_vel;

        // Set the position and rotation of the bullet sprite
        S.setPosition(Pos);
        S.setRotation(enemy_angle);
    }

    Bullet(const Sprite enemySprite, const Vector2f enemy_vel, int n) : type(2)
    {

        if (fire2_T.getSize().x == 0)
        {
            cerr << "Error: Global texture not loaded before creating a Bullet object!" << endl;
            exit(0);
        }

        // Set the bullet sprite texture
        S.setTexture(fire2_T);

        // Set the origin of the bullet sprite to its center
        S.setOrigin(fire2_T.getSize().x / 2.0f, fire2_T.getSize().y / 2.0f);

        // Get the enemy jet's center using its global bounds
        FloatRect enemyBounds = enemySprite.getGlobalBounds();
        Vector2f enemyCenter = Vector2f(
            enemyBounds.left + enemyBounds.width / 2.0f,
            enemyBounds.top + enemyBounds.height / 2.0f);

        // Get the enemy's rotation (angle in degrees)
        float enemy_angle = enemySprite.getRotation();

        // Calculate the angle in radians for positioning and velocity
        float angleRad = (enemy_angle - 90 + (120 * n)) * M_PI / 180.0f; // Convert angle to radians, adjust for alignment

        // Offset the bullet's starting position based on the jet's center and angle
        Pos = enemyCenter + Vector2f(cos(angleRad) * enemyBounds.height / 2.0f, sin(angleRad) * enemyBounds.height / 2.0f);

        // Set the bullet's velocity, adding the enemy's velocity
        float bulletSpeed = 200.0f; // Define the bullet's speed
        Vel = Vector2f(cos(angleRad) * bulletSpeed, sin(angleRad) * bulletSpeed) + enemy_vel;

        // Set the position and rotation of the bullet sprite
        S.setPosition(Pos);
        S.setRotation(enemy_angle + (120 * n));
    }

    void update(float deltaTime);
};
Texture Bullet::T;

class Jet
{
public:
    Sprite S;
    static Texture T;

    Vector2f Pos = Vector2f(resolution_X / 2.0, resolution_Y / 2.0), Vel = Vector2f(0, 0);

    bool destroyed = false;

    Jet();

    void update(float deltaTime);
};
Texture Jet::T;

class Rock
{
public:
    Sprite S;
    static Texture T;
    Vector2f Pos, Vel = Vector2f(0, 100);

    float animationTimer = 0, Texture_Index = 0;

    float length = T.getSize().x;
    float axis = T.getSize().y;

    Rock(int x);

    void update(float deltaTime);
};
Texture Rock::T;

class Rock_Explode
{
public:
    Sprite S;
    static Texture T;
    Vector2f Pos;

    bool ended = false;
    // int index;
    Rock R = Rock(1);

    float animationTimer = 0, Texture_Index = 0;

    float length = T.getSize().x;
    float axis = T.getSize().y;

    Rock_Explode(Vector2f pos);

    void update(float deltaTime);
};
Texture Rock_Explode::T;

class Jet_Explode
{
public:
    Sprite S;
    static Texture T;

    Sound So;
    static SoundBuffer SB;

    Vector2f Pos;

    bool ended = false; /// pppp
    // Jet J;              // This is a Jet object, but you may not need it if it's only for calculations

    float length = T.getSize().x;
    float axis = T.getSize().y;

    float animationTimer = 0, Texture_Index = 0;

    int type = 0;

    Jet_Explode(Jet J);
    Jet_Explode(Enemy E);

    void update(float deltaTime);
};
Texture Jet_Explode::T;
SoundBuffer Jet_Explode::SB;

class Enemy
{
public:
    static Texture T1;
    static Texture T2;

    Sprite S;
    Vector2f Pos, Vel;

    float maxVel = 50;
    float rotationSpeed = 1.0f;

    bool destroyed = false;

    float R_speed = 400;

    float animationTimer = 0, Texture_angle = 0;

    int type = 0;

    // vector<Bullet> B;

    Enemy(int ty = 1)
    {
        // S.setTexture(T2); // Resolving ambiguity for the Bullet texture
        if (ty == 1)
        {

            S.setTexture(T1);
            S.setScale(0.3f, 0.3f);

            // srand(time(0));
            Pos = Vector2f((rand() % (resolution_X - 100)) + 50, -100);

            // S.setOrigin(S.getGlobalBounds().width/2.0,S.getGlobalBounds().height/2.0);

            S.setPosition(Pos);
        }
        else if (ty == 2)
        {
            S.setTexture(enemy_2_T);
            S.setScale(0.4, 0.4);

            // S.setOrigin(S.getGlobalBounds().width/2.0,S.getGlobalBounds().height/2.0);
            S.setOrigin(enemy_2_T.getSize().y / 2.0f, enemy_2_T.getSize().y / 2.0f);

            // srand(time(0));

            Pos = Vector2f((rand() % (resolution_X - 100)) + 50, -100);
            Vel = Vector2f(0, 50);

            S.setPosition(Pos);
        }
        else if (ty == 3)
        {
            S.setTexture(enemy_3_T);
            S.setScale(0.7, 0.7);

            // S.setOrigin(S.getGlobalBounds().width/2.0,S.getGlobalBounds().height/2.0);
            S.setOrigin(enemy_3_T.getSize().y / 2.0f, enemy_3_T.getSize().y / 2.0f);

            // srand(time(0));

            // Pos = Vector2f((rand() % (resolution_X - 100)) + 50, 100);
            // Pos = Vector2f(resolution_X + 200, -200);

            // do
            // {

            //     Vel = Vector2f(rand() % 200 - 100, rand() % 200 - 100);
            // } while ((Vel.x <= 0 && Vel.y <= 0) || (Vel.x >= 0 && Vel.y <= 0));

            // float FVel = sqrt(Vel.x * Vel.x + Vel.y * Vel.y);
            // float V_X = Vel.x/Vel.y*FVel,V_Y = Vel.y/Vel.x*FVel;

            // Vel=Vector2f(V_X,V_Y);

            float angle = rand() % 180 + 180;
            // angle=90;
            Vel = Vector2f(100 * cos(angle * M_PI / 180), -100 * sin(angle * M_PI / 180));
            // float angle = atan2(Vel.y, Vel.x) * 180 / M_PI + 90;
            // FVel = sqrt(Vel.x * Vel.x + Vel.y * Vel.y);
            float diagnal = sqrt(resolution_X * resolution_X + resolution_Y * resolution_Y);

            Pos = Vector2f(resolution_X / 2, resolution_Y / 2 + 100) - ((diagnal / 2 + 100) * Vector2f(Vel.x / 100, Vel.y / 100));
            S.setPosition(Pos);
            cout << angle << endl;
            S.setRotation(360 - angle + 90);
        }

        this->type = ty;
    }

    void setTarget(Sprite S_target, float deltaTime)
    {
        FloatRect enemyBounds = S.getGlobalBounds();
        Vector2f center = Vector2f(
            enemyBounds.left + enemyBounds.width / 2.0f,
            enemyBounds.top + enemyBounds.height / 2.0f);

        Vector2f center_T = S_target.getPosition() + Vector2f(S_target.getGlobalBounds().width / 2.0, S_target.getGlobalBounds().height / 2.0);

        Vector2f direction = center_T - center;

        float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (type == 1)
        {

            if (magnitude > 10)
            {
                Vel = Vector2f((direction.x / magnitude) * maxVel, (direction.y / magnitude) * maxVel);

                // Calculate the target angle
                float targetAngle = atan2(direction.y, direction.x) * 180 / M_PI + 90; // Convert to degrees

                // Get the current rotation
                float currentAngle = S.getRotation();

                // Interpolate between the current and target rotation
                if (fabs(targetAngle - currentAngle) > 180)
                {
                    // Handle wrap-around for angles (e.g., from 0 to 360 degrees)
                    if (targetAngle > currentAngle)
                        currentAngle += 360;
                    else
                        targetAngle += 360;
                }
                float newAngle = currentAngle + (targetAngle - currentAngle) * rotationSpeed * deltaTime;

                S.setRotation(newAngle);
            }
            else
            {
                // If the target position matches the center, set velocity to zero
                Vel = Vector2f(0, 0);
            }
        }
        else if (type == 2)
        {
            if (direction.x > 0)
            {
                Vel.x = 30;
            }
            else if (direction.x < 0)
            {
                Vel.x = -30;
            }
        }
    }

    void update(float deltaTime)
    {
        if (type == 1 || type == 3)
        {
            Pos += Vel * deltaTime;
            S.setPosition(Pos);
        }
        else if (type == 2)
        {
            Pos += Vel * deltaTime;
            S.setPosition(Pos);

            Texture_angle += R_speed * deltaTime;

            if (Texture_angle > 360)
                Texture_angle -= 360;

            S.setRotation(Texture_angle);
        }
    }
};
Texture Enemy ::T1;
Texture Enemy ::T2;

void load_All_Images_Textures_Music();

void Jet_Movement(vector<Bullet> &Bullets, Jet &jet, float &deltaTime);
// void fire(vector<Bullet> &Bullets, Jet &jet, float &bullet_fired, float &deltaTime);

void Rock_jet_colision(Jet &jet, vector<Rock> &Rocks, vector<Rock_Explode> &Rocks_Destoryed, vector<Jet_Explode> &Explosion);
void Bullet_colision(Jet &jet, vector<Enemy> &enemy, vector<Bullet> &Bullets, vector<Rock> &Rocks, vector<Rock_Explode> &Rocks_Destoryed, vector<Jet_Explode> &Explosion);

void Game_loop(RenderWindow &window);
void Initial_desplay(RenderWindow &window);
void chooce_levels(RenderWindow &window);
void game_over_display(RenderWindow &window);

int main()
{
    srand(time(0));

    load_All_Images_Textures_Music();

    RenderWindow window(VideoMode(resolution_X, resolution_Y), "SHOOTER", Style ::Titlebar | Style ::Close | Style::Resize);

    Initial_desplay(window);

flag:
    again = false;
    chooce_levels(window);

    Game_loop(window);

    // score_count = 100;

    game_over_display(window);

    if (again)
        goto flag;
    // Score(windows);
}

Bullet::Bullet(Vector2f jet_pos) : /*animationTimer(0), Texture_Index(0),*/ Vel(0, -200)
{
    if (T.getSize().x == 0)
    {
        cerr << "Error: Global texture not loaded before creating a Bullet object!" << endl;
        exit(0);
    }

    S.setTexture(T);

    Pos = jet_pos + Vector2f(2, -10);
    S.setPosition(Pos);
    // S.setColor(Color::Magenta);

    S.setTextureRect(IntRect(0, 0, x_axis, y_axis));
}
void Bullet::update(float deltaTime)
{
    Pos += Vel * deltaTime;
    S.setPosition(Pos);

    // const float frameDuration = 0.1f;

    // animationTimer += deltaTime;

    // if (animationTimer > frameDuration)
    // {
    //     animationTimer = 0;

    //     Texture_Index += x_axis;

    //     S.setTextureRect(IntRect(Texture_Index, 0, x_axis, y_axis));

    //     if (length - Texture_Index < 2 * x_axis)
    //     {
    //         Texture_Index = 0;
    //     }
    // }
}

Jet::Jet() //: Vel(0, 0), Pos(resolution_X / 2.0, resolution_Y / 2.0), destroyed(false)
{
    if (T.getSize().x == 0)
    {
        cerr << "Error: Global texture not loaded before creating a Jet object!" << endl;
        exit(0);
    }

    S.setTexture(T);
    S.setPosition(Pos);

    S.setTextureRect(IntRect(39, 85, 45, 45));
}
void Jet::update(float deltaTime)
{
    float touch = 40;
    if (Pos.x < 0)
    {
        Pos.x = 0;   // Correct position to be within bounds
        Vel.x *= -1; // Reverse velocity
    }
    else if (Pos.x > resolution_X - touch)
    {
        Pos.x = resolution_X - touch; // Correct position to be within bounds
        Vel.x *= -1;                  // Reverse velocity
    }

    if (Pos.y < 0)
    {
        Pos.y = 0;   // Correct position to be within bounds
        Vel.y *= -1; // Reverse velocity
    }
    else if (Pos.y > resolution_Y - touch)
    {
        Pos.y = resolution_Y - touch; // Correct position to be within bounds
        Vel.y *= -1;                  // Reverse velocity
    }

    // Update position if the object is not destroyed
    if (!destroyed)
    {
        Pos += Vel * deltaTime;
        S.setPosition(Pos);
    }
}

Rock::Rock(int x) : Pos(x, -100)
{
    if (T.getSize().x == 0)
    {
        cerr << "Error: Global texture not loaded before creating a Flower object!" << endl;
        exit(0);
    }

    S.setTexture(T);
    S.setPosition(Pos);
    // S.setRotation(r);

    S.setTextureRect(IntRect(0, 0, axis, axis));
}
void Rock::update(float deltaTime)
{

    Pos += Vel * deltaTime;
    S.setPosition(Pos);

    const float frameDuration = 0.05f;

    animationTimer += deltaTime;

    if (animationTimer > frameDuration)
    {
        animationTimer = 0;

        Texture_Index += axis;

        S.setTextureRect(IntRect(Texture_Index, 0, axis, axis));

        if (length - Texture_Index < 2 * axis)
        {
            Texture_Index = 0;
        }
    }
}

Rock_Explode::Rock_Explode(Vector2f pos) //: animationTimer(0), Texture_Index(0), ended(false) //, Pos(pos+Vector2f(3,3))
{
    if (T.getSize().x == 0)
    {
        cerr << "Error: Global texture not loaded before creating a Rock explosion object!" << endl;
        exit(0);
    }

    S.setTexture(T);
    S.setScale(0.4f, 0.4f);
    S.setTextureRect(IntRect(0, 0, axis, axis));

    Pos = pos + Vector2f(((R.S.getGlobalBounds().width - S.getGlobalBounds().width) / 2.0), ((R.S.getGlobalBounds().height - S.getGlobalBounds().height) / 2.0));

    S.setPosition(Pos);
}
void Rock_Explode::update(float deltaTime)
{

    const float frameDuration = 0.025;

    animationTimer += deltaTime;

    if (animationTimer > frameDuration)
    {
        animationTimer = 0;

        Texture_Index += axis;

        S.setTextureRect(IntRect(Texture_Index, 0, axis, axis));

        if (length - Texture_Index < 2 * axis)
        {
            Texture_Index = 0;
            ended = true;
        }
    }
}

Jet_Explode::Jet_Explode(Jet J) //: animationTimer(0), Texture_Index(0), ended(false) //,Pos(pos + Vector2f(-15, -15))
{
    type = 1;
    // if (T.getSize().x == 0)
    // {
    //     cerr << "Error: Global texture not loaded before creating a Flower object!" << endl;
    //     exit(0);
    // }

    // S.setTexture(T);
    // S.setScale(0.6f, 0.6f);
    // S.setTextureRect(IntRect(0, 0, 256, 256));

    // jet_destroyed = true;
    // // ROCK 12288
    // // ROCK 256

    // Pos = pos + Vector2f(((J.S.getGlobalBounds().width - S.getGlobalBounds().width) / 2.0), ((J.S.getGlobalBounds().height - S.getGlobalBounds().height) / 2.0));

    // S.setPosition(Pos);
    if (T.getSize().x == 0)
    {
        cerr << "Error: Global texture not loaded before creating a Jet_Destroyed object!" << endl;
        exit(0);
    }

    // Set texture and scale for the destroyed jet sprite
    S.setTexture(T);
    S.setScale(0.6f, 0.6f);
    S.setTextureRect(IntRect(0, 0, 256, 256)); // Example size for texture rect, adjust as needed

    So.setBuffer(SB);
    So.setVolume(100);
    So.setPlayingOffset(sf::seconds(0.75));
    // So.play();
    // Adjust the position based on the enemy sprite's center and size
    FloatRect jetBounds = J.S.getGlobalBounds(); // Get the global bounds of the enemy
    Vector2f jetCenter = Vector2f(
        jetBounds.left + jetBounds.width / 2.0f, // Calculate center X
        jetBounds.top + jetBounds.height / 2.0f  // Calculate center Y
    );

    // Adjust the position to center the destroyed jet at the enemy's center position
    Pos = jetCenter - Vector2f(S.getGlobalBounds().width / 2.0f, S.getGlobalBounds().height / 2.0f);

    // Set the final position for the destroyed jet sprite
    S.setPosition(Pos);
}

Jet_Explode::Jet_Explode(Enemy E)
{
    type = 2;

    if (T.getSize().x == 0)
    {
        cerr << "Error: Global texture not loaded before creating a Jet_Destroyed object!" << endl;
        exit(0);
    }

    // Set texture and scale for the destroyed jet sprite
    S.setTexture(T);
    S.setScale(0.6f, 0.6f);
    S.setTextureRect(IntRect(0, 0, 256, 256)); // Example size for texture rect, adjust as needed

    So.setBuffer(SB);
    So.setVolume(50);
    So.setPlayingOffset(sf::seconds(0.75));
    So.play();

    // Adjust the position based on the enemy sprite's center and size
    FloatRect enemyBounds = E.S.getGlobalBounds(); // Get the global bounds of the enemy
    Vector2f enemyCenter = Vector2f(
        enemyBounds.left + enemyBounds.width / 2.0f, // Calculate center X
        enemyBounds.top + enemyBounds.height / 2.0f  // Calculate center Y
    );

    // Adjust the position to center the destroyed jet at the enemy's center position
    Pos = enemyCenter - Vector2f(S.getGlobalBounds().width / 2.0f, S.getGlobalBounds().height / 2.0f);

    // Set the final position for the destroyed jet sprite
    S.setPosition(Pos);
}

void Jet_Explode::update(float deltaTime)
{

    const float frameDuration = 0.125f;

    animationTimer += deltaTime;

    if (animationTimer > frameDuration)
    {
        animationTimer = 0;

        Texture_Index += axis;

        S.setTextureRect(IntRect(Texture_Index, 0, axis, axis));

        // So.play();
        if (length - Texture_Index < 2 * axis)
        {
            Texture_Index = 0;
            ended = true; // ppppp
            if (type == 1)
            {
                jet_explosion_ended = true;
            }
        }
    }
}

void Game_loop(RenderWindow &window)
{
    // srand(time(0));
    score_count = 0;
    Jet jet;
    vector<Enemy> enemy;
    enemy.emplace_back(3);
    // enemy.emplace_back(2);

    // Enemy enemy_2(2);

    vector<Bullet> Bullets;
    vector<Rock> Rocks;
    vector<Rock_Explode> Rocks_Destoryed;

    vector<Jet_Explode> Explosion;
    // Jet_Destroyed jet_Destroyed(Vector2f(100, 100));
    // Jet_Destroyed enemy_Destroyed(Vector2f(100, 100));

    Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(20);
    score_text.setFillColor(Color::White);
    // score_text.scale(0.1f,0.1f);
    score_text.setPosition(20, 30);

    int ones = 0;

    Clock clock;
    float bullet_fired = 0, bullet_fired_2 = 0, bullet_fired_3 = 0;
    float rock_fired = 0, enemy_produced = 0;

    while (window.isOpen())
    {
        // r = rand()%360;
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }
        }
        float deltaTime = clock.restart().asSeconds();

        if (!game_paused)
        {
            window.draw(Bg1);

            if (e.type == Event::MouseButtonPressed)
            {
                // Identify the mouse button
                if (e.mouseButton.button == Mouse::Left)
                {
                    cout << "Left mouse button clicked at position: ("
                         << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                    FloatRect range = S_pause.getGlobalBounds();
                    if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                    {
                        if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                        {
                            game_paused = true;
                            // return;
                        }
                    }
                }
            }

            {
                const float durationTime = 3;
                enemy_produced += deltaTime;

                if (enemy.size() < level && enemy_produced > durationTime)
                {
                    enemy.emplace_back(rand() % 3 + 1);
                    // enemy.emplace_back(3);
                    enemy_produced = 0;
                }
            }

            {
                const int rock_in_Space = level * 5;
                const float durationTime = 0.5;
                rock_fired += deltaTime;

                if (Rocks.size() < rock_in_Space && rock_fired > durationTime)
                {
                    rock_fired = 0;

                    Rock newRock(0);
                    bool isValid;

                    do
                    {
                        isValid = true;
                        newRock = Rock(rand() % (resolution_X - 50)); // Generate a new rock at random x-position

                        for (const auto &it : Rocks)
                        {
                            // Calculate distance between newRock and existing rocks
                            double distance = sqrt(pow((newRock.Pos.x - it.Pos.x), 2) + pow((newRock.Pos.y - it.Pos.y), 2));
                            if (distance <= 50)
                            {
                                isValid = false; // If too close to another rock, mark as invalid
                                break;
                            }
                        }
                    } while (!isValid);

                    // If valid, add the new rock to the vector
                    Rocks.push_back(newRock);
                }
            }

            if (!jet_destroyed)
            {
                const float bullet_reload = 1.0f;
                bullet_fired_2 += deltaTime;

                if (bullet_fired_2 > bullet_reload)
                {
                    for (auto &en : enemy)
                    {
                        if (en.type == 1 || en.type == 3)
                        {

                            Bullets.emplace_back(en.S, en.Vel);
                        }
                        else if (en.type == 2 && en.Pos.y < (resolution_Y + 200))
                        {
                            en.update(deltaTime);
                            window.draw(en.S);

                            Bullets.emplace_back(en.S, en.Vel, 0);
                            Bullets.emplace_back(en.S, en.Vel, 1);
                            Bullets.emplace_back(en.S, en.Vel, 2);
                        }
                        bullet_fired_2 = 0;
                    }
                }
            }

            Jet_Movement(Bullets, jet, deltaTime);

            // Handle shooting while moving
            if (!jet_destroyed)
            {
                const float bullet_reload = 0.5f;
                bullet_fired += deltaTime;

                bool fireBullet = Keyboard::isKeyPressed(Keyboard::Space) ||
                                  (Joystick::isConnected(0) && Joystick::isButtonPressed(0, 5)); // RB is usually button index 5

                if (fireBullet && bullet_fired > bullet_reload)
                {
                    Bullets.emplace_back(jet.Pos);
                    bullet_fired = 0;
                }
            }

            for (auto en = enemy.begin(); en != enemy.end();)
            {
                float b = 300;
                if (-b > en->Pos.x || en->Pos.x > resolution_X + b || -b > en->Pos.y || en->Pos.y > resolution_Y + b)
                {
                    en = enemy.erase(en);
                }
                else
                {
                    en++;
                }
            }

            Rock_jet_colision(jet, Rocks, Rocks_Destoryed, Explosion);
            Bullet_colision(jet, enemy, Bullets, Rocks, Rocks_Destoryed, Explosion);

            for (auto it = Bullets.begin(); it != Bullets.end();)
            {
                if (it->Pos.x < -50 || resolution_X + 50 < it->Pos.x || it->Pos.y < -50 || resolution_X + 50 < it->Pos.y)
                {
                    it = Bullets.erase(it);
                }
                else
                {
                    it++;
                }
            }

            for (auto it = Rocks.begin(); it != Rocks.end();)
            {
                if (it->Pos.y > resolution_Y)
                {
                    it = Rocks.erase(it);
                }
                else
                {
                    it++;
                }
            }

            for (auto it = Rocks_Destoryed.begin(); it != Rocks_Destoryed.end();)
            {
                if (it->ended)
                {
                    it = Rocks_Destoryed.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // if (!jet.destroyed)
            for (auto &bullet : Bullets)
            {
                bullet.update(deltaTime);
                window.draw(bullet.S);
            }

            for (auto &rock : Rocks)
            {
                rock.update(deltaTime);
                window.draw(rock.S);
            }
            for (auto &x_rock : Rocks_Destoryed)
            {
                x_rock.update(deltaTime);
                window.draw(x_rock.S);
            }
            // if (jet.destroyed ) // pppp
            // {
            //     .update(deltaTime);
            //     window.draw(jet_Destroyed.S);
            // }
            ////////////////////////
            for (auto &ex : Explosion)
            {
                // if (enemy.destroyed && !ex.ended)
                // {

                ex.update(deltaTime);
                window.draw(ex.S);
                // }
            }
            for (auto ex = Explosion.begin(); ex != Explosion.end();)
            {
                if (ex->ended && ex->type == 2)
                {
                    ex = Explosion.erase(ex);
                }
                else
                {
                    ex++;
                }
            }

            jet.update(deltaTime);
            window.draw(jet.S);

            // if (!enemy.destroyed)
            for (auto &en : enemy)
            {
                en.setTarget(jet.S, deltaTime);
                en.update(deltaTime);
                window.draw(en.S);
            }

            window.draw(S_pause);

            // if (enemy.destroyed && enemy_2.Pos.y < (resolution_Y + 200))
            // {
            //     enemy_2.update(deltaTime);
            //     window.draw(enemy_2.S);

            // }

            score_text.setString(to_string(score_count));
            window.draw(score_text);

            // if (Keyboard::isKeyPressed(Keyboard::X))
            // {
            //     cout << " Bullets : " << Bullets.size() << endl;
            //     cout << " Rocks : " << Rocks.size() << endl;
            //     cout << " Roks destroyed : " << Rocks_Destoryed.size() << endl;
            //     cout << " Enemy : " << enemy.size() << endl;
            //     cout << " Explosion : " << Explosion.size() << endl;
            //     cout << " Level :" << level << endl;
            // }

            ones = 0;
        }
        else if (game_paused)
        {
            window.draw(start_S);
            window.draw(exit_S);

            if (e.type == Event::MouseButtonPressed)
            {
                // Identify the mouse button
                if (e.mouseButton.button == Mouse::Left)
                {
                    // cout << "Left mouse button clicked at position: ("
                    //  << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                    FloatRect range = start_S.getGlobalBounds();
                    if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                    {
                        if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                        {
                            // return;
                            game_paused = false;
                        }
                    }

                    range = exit_S.getGlobalBounds();
                    if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                    {
                        if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                        {
                            exit(0);
                        }
                    }
                }
            }
        }

        if (jet_explosion_ended)
        {
            jet_explosion_ended = false;
            jet_destroyed = false;
            return;
        }

        if (ones == 0)
            window.display();

        if (!game_paused)
        {
            window.clear(Color::Black);
            ones = 1;
        }
        // window.draw(groundRectangle);
    }
}

void Initial_desplay(RenderWindow &window)
{

    // if (!pause_T.loadFromFile("images/pause.png"))
    // {
    //     cout << "Error: Could not Backgroud 3 texture!" << endl;
    //     exit(-1);
    // }
    // Sprite pause_S;
    // pause_S.setTexture(pause_T);
    // pause_S.setScale(scale, scale);
    // pause_S.setPosition((resolution_X - pause_S.getGlobalBounds().width) / 2.0, ((resolution_Y - pause_S.getGlobalBounds().height) / 2.0) + distance);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }
        }

        window.draw(Bg2);

        // if (game_paused)
        //     window.draw(pause_S);
        // else
        window.draw(start_S);

        window.draw(exit_S);

        if (e.type == Event::MouseButtonPressed)
        {
            // Identify the mouse button
            if (e.mouseButton.button == Mouse::Left)
            {
                //  << "Left mouse button clicked at position: ("
                //      << ecout.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                FloatRect range = start_S.getGlobalBounds();
                if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                {
                    if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                    {
                        return;
                    }
                }

                range = exit_S.getGlobalBounds();
                if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                {
                    if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                    {
                        exit(0);
                    }
                }
            }
        }

        window.display();
        window.clear(Color::Black);
    }
}

void chooce_levels(RenderWindow &window)
{
    Texture level_T;

    float scale_x = 1.5f, scale_y = 0.75f;

    int no_of_levels = 5;

    string st_level[no_of_levels] = {"Easy", "  Medium", "Hard", " Insane", "    Imposible"};

    if (!level_T.loadFromFile("images/level.png"))
    {
        cout << "Error: Could not Backgroud 3 texture!" << endl;
        exit(-1);
    }

    Sprite levels_S[no_of_levels];
    for (int i = 0; i < no_of_levels; i++)
    {
        levels_S[i].setTexture(level_T);
        levels_S[i].setScale(scale_x, scale_y);

        float Pos_x = (resolution_X - levels_S[i].getGlobalBounds().width) / 2.0;
        float Pos_y = ((resolution_Y - levels_S[i].getGlobalBounds().height) / 2.0) + ((i + 1) - (no_of_levels + 1) / 2.0f) * 100;

        levels_S[i].setPosition(Pos_x, Pos_y);
    }

    Text text[no_of_levels];
    for (int i = 0; i < no_of_levels; i++)
    {
        text[i].setFont(font);
        text[i].setCharacterSize(60);
        text[i].setFillColor(Color::Black);
        // text[i].setStyle(Text::Bold);
        text[i].scale(0.8f, 0.8f);
        text[i].setString(st_level[i]);

        FloatRect textBounds = text[i].getLocalBounds();
        float text_x = levels_S[i].getPosition().x +
                       (levels_S[i].getGlobalBounds().width - textBounds.width) / 2.0 -
                       textBounds.left;
        float text_y = levels_S[i].getPosition().y +
                       (levels_S[i].getGlobalBounds().height - textBounds.height) / 2.0 -
                       textBounds.top;

        text[i].setPosition(text_x, text_y);
    }

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }
        }

        window.draw(Bg1);

        for (int i = 0; i < no_of_levels; i++)
        {
            window.draw(levels_S[i]);
            window.draw(text[i]);
        }

        if (e.type == Event::MouseButtonPressed)
        {
            // Identify the mouse button
            if (e.mouseButton.button == Mouse::Left)
            {
                // cout << "Left mouse button clicked at position: ("
                //      << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                for (int i = 0; i < no_of_levels; i++)
                {
                    FloatRect range = levels_S[i].getGlobalBounds();

                    if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                    {
                        if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                        {
                            // cout << i << endl;
                            level = i + 1;
                            return;
                        }
                    }
                }
            }
        }

        window.display();
        window.clear(Color::Black);
    }
}

void game_over_display(RenderWindow &window)
{
    Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(100);
    score_text.setFillColor(Color::Black);
    score_text.scale(0.6f, 0.6f);

    score_text.setString("Your Score : " + to_string(score_count));

    {
        FloatRect textBounds = score_text.getGlobalBounds();

        float text_x = (resolution_X - textBounds.width) / 2.0f;
        float text_y = (resolution_Y - textBounds.height) / 2.0f - 100;

        score_text.setPosition(text_x, text_y);
    }

    Text gameOver_text;
    gameOver_text.setFont(font);
    gameOver_text.setCharacterSize(200);
    gameOver_text.setFillColor(Color::Black);
    gameOver_text.scale(0.6f, 0.6f);

    gameOver_text.setString("GAME OVER");

    {
        FloatRect textBounds = gameOver_text.getGlobalBounds();

        float text_x = (resolution_X - textBounds.width) / 2.0f;
        float text_y = (resolution_Y - textBounds.height) / 2.0f - 200;

        gameOver_text.setPosition(text_x, text_y);
    }

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }
        }

        window.draw(Bg2);

        // if (game_paused)
        //     window.draw(pause_S);
        // else
        // window.draw(start_S);
        window.draw(gameOver_text);
        window.draw(score_text);

        window.draw(start_S);
        window.draw(exit_S);

        if (e.type == Event::MouseButtonPressed)
        {
            // Identify the mouse button
            if (e.mouseButton.button == Mouse::Left)
            {
                // cout << "Left mouse button clicked at position: ("
                //      << e.mouseButton.x << ", " << e.mouseButton.y << ")" << endl;

                FloatRect range = start_S.getGlobalBounds();
                if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                {
                    if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                    {
                        again = true;
                        return;
                    }
                }

                range = exit_S.getGlobalBounds();
                if (range.left < e.mouseButton.x && e.mouseButton.x < (range.left + range.width))
                {
                    if (range.top < e.mouseButton.y && e.mouseButton.y < (range.top + range.height))
                    {

                        exit(0);
                    }
                }
            }
        }

        window.display();
        window.clear(Color::Black);
    }
}

void Rock_jet_colision(Jet &jet, vector<Rock> &Rocks, vector<Rock_Explode> &Rocks_Destoryed, vector<Jet_Explode> &Explosion)
{

    for (auto rock = Rocks.begin(); rock != Rocks.end();)
    {
        bool rock_destroyed = false; // Track if the bee was killed

        // Get bee's global bounds (centered position based on sprite size)
        FloatRect rockBounds = rock->S.getGlobalBounds();
        float rockCenterX = rockBounds.left + rockBounds.width / 2.0f;
        float rockCenterY = rockBounds.top + rockBounds.height / 2.0f;

        FloatRect jetBounds = jet.S.getGlobalBounds();
        float jetCenterX = jetBounds.left + jetBounds.width / 2.0f;
        float jetCenterY = jetBounds.top + jetBounds.height / 2.0f;

        // Calculate the distance between the bee and the bullet
        float dx = jetCenterX - rockCenterX;
        float dy = jetCenterY - rockCenterY;
        float distance = sqrt(dx * dx + dy * dy);

        // Define a collision threshold (adjust based on bee and bullet size)
        float collisionThreshold = 40.0f;

        if (distance < collisionThreshold)
        {
            // cout << "Jet Destroyed\n";

            // jet_Destoryed = Jet_Destroyed(jet.Pos);
            Explosion.emplace_back(jet);

            jet.Pos = Vector2f(resolution_X / 2.0, resolution_Y + 200);
            jet.S.setPosition(jet.Pos);

            jet.destroyed = true;
            jet_destroyed = true;

            Sd2.play();

            // Remove the bee and update the iterator
            rock = Rocks.erase(rock);
            rock_destroyed = true; // Mark the bee as killed
            break;                 // Exit the bullet loop since the bee is gone
        }

        // Increment the bee iterator only if it was not killed
        if (!rock_destroyed)
        {
            ++rock;
        }
    }
}

void Bullet_colision(Jet &jet, vector<Enemy> &enemy, vector<Bullet> &Bullets, vector<Rock> &Rocks, vector<Rock_Explode> &Rocks_Destoryed, vector<Jet_Explode> &Explosion)
{
    // Bullet + Rock
    for (auto rock = Rocks.begin(); rock != Rocks.end();)
    {
        bool rock_destroyed = false; // Track if the bee was killed

        for (auto it = Bullets.begin(); it != Bullets.end();)
        {
            // Get bee's global bounds (centered position based on sprite size)
            FloatRect rockBounds = rock->S.getGlobalBounds();
            float rockCenterX = rockBounds.left + rockBounds.width / 2.0f;
            float rockCenterY = rockBounds.top + rockBounds.height / 2.0f;

            FloatRect bulletBounds = it->S.getGlobalBounds();
            float bulletCenterX = bulletBounds.left + bulletBounds.width / 2.0f;
            float bulletCenterY = bulletBounds.top + bulletBounds.height / 2.0f;

            // Calculate the distance between the bee and the bullet
            float dx = bulletCenterX - rockCenterX;
            float dy = bulletCenterY - rockCenterY;
            float distance = sqrt(dx * dx + dy * dy);

            // Define a collision threshold (adjust based on bee and bullet size)
            float collisionThreshold = 40.0f;

            if (distance < collisionThreshold)
            {
                // cout << "Rock Destroyed\n";

                if (it->type == 1)
                    score_count += 10;

                it = Bullets.erase(it);

                Rocks_Destoryed.emplace_back(rock->Pos);

                // Md1.play();
                Sd1.play();

                // Remove the bee and update the iterator
                rock = Rocks.erase(rock);
                rock_destroyed = true; // Mark the bee as killed
                break;                 // Exit the bullet loop since the bee is gone
            }
            else
            {
                ++it; // Move to the next bullet
            }
        }

        // Increment the bee iterator only if it was not killed
        if (!rock_destroyed)
        {
            ++rock;
        }
    }

    // Red_Bullet + Jet
    for (auto it = Bullets.begin(); it != Bullets.end();)
    {
        // Get bee's global bounds (centered position based on sprite size)
        FloatRect jetBounds = jet.S.getGlobalBounds();
        float jetCenterX = jetBounds.left + jetBounds.width / 2.0f;
        float jetCenterY = jetBounds.top + jetBounds.height / 2.0f;

        FloatRect bulletBounds = it->S.getGlobalBounds();
        float bulletCenterX = bulletBounds.left + bulletBounds.width / 2.0f;
        float bulletCenterY = bulletBounds.top + bulletBounds.height / 2.0f;

        // Calculate the distance between the bee and the bullet
        float dx = bulletCenterX - jetCenterX;
        float dy = bulletCenterY - jetCenterY;
        float distance = sqrt(dx * dx + dy * dy);

        // Define a collision threshold (adjust based on bee and bullet size)
        float collisionThreshold = 40.0f;

        if (distance < collisionThreshold && it->type == 2)
        {
            // cout << "Jet Destroyed\n";
            // score_count += 10;

            it = Bullets.erase(it);

            // jet_Destroyed = Jet_Destroyed(jet.Pos);
            Explosion.emplace_back(jet);

            jet.Pos = Vector2f(resolution_X / 2.0, resolution_Y + 200);
            jet.S.setPosition(jet.Pos);

            jet.destroyed = true;
            jet_destroyed = true;

            Sd2.play();
            break; // Exit the bullet loop since the bee is gone
        }
        else
        {
            ++it; // Move to the next bullet
        }
    }

    // // Blue_Bullet + Enemy
    // for (auto en = enemy.begin(); en != enemy.end();)
    // {
    //     bool enemy_is_destroyed = false;
    //     for (auto it = Bullets.begin(); it != Bullets.end();)
    //     {
    //         // Get bee's global bounds (centered position based on sprite size)
    //         FloatRect enemyBounds = en->S.getGlobalBounds();
    //         float enemyCenterX = enemyBounds.left + enemyBounds.width / 2.0f;
    //         float enemyCenterY = enemyBounds.top + enemyBounds.height / 2.0f;

    //         FloatRect bulletBounds = it->S.getGlobalBounds();
    //         float bulletCenterX = bulletBounds.left + bulletBounds.width / 2.0f;
    //         float bulletCenterY = bulletBounds.top + bulletBounds.height / 2.0f;

    //         // Calculate the distance between the bee and the bullet
    //         float dx = bulletCenterX - enemyCenterX;
    //         float dy = bulletCenterY - enemyCenterY;
    //         float distance = sqrt(dx * dx + dy * dy);

    //         // Define a collision threshold (adjust based on bee and bullet size)
    //         float collisionThreshold = 40.0f;

    //         if (distance < collisionThreshold && it->type == 1)
    //         {
    //             cout << "enemy Destroyed\n";
    //             score_count += 100;

    //             it = Bullets.erase(it);

    //             Explosion.emplace_back(en->Pos, *en);

    //             // enemy.Pos = Vector2f(-200, -200);
    //             // enemy.S.setPosition(enemy.Pos);
    //             en = enemy.erase(en);

    //             // enemy.destroyed = true;

    //             Sd3.play();
    //             break; // Exit the bullet loop since the bee is gone
    //         }
    //         else
    //         {
    //             ++it; // Move to the next bullet
    //         }
    //     }

    //     if (!enemy_is_destroyed)
    //     {
    //         en++;
    //     }
    // }

    for (auto en = enemy.begin(); en != enemy.end();)
    {
        bool enemyDestroyed = false;

        for (auto it = Bullets.begin(); it != Bullets.end();)
        {
            // Calculate centers of enemy and bullet
            FloatRect enemyBounds = en->S.getGlobalBounds();
            float enemyCenterX = enemyBounds.left + enemyBounds.width / 2.0f;
            float enemyCenterY = enemyBounds.top + enemyBounds.height / 2.0f;

            FloatRect bulletBounds = it->S.getGlobalBounds();
            float bulletCenterX = bulletBounds.left + bulletBounds.width / 2.0f;
            float bulletCenterY = bulletBounds.top + bulletBounds.height / 2.0f;

            float dx = bulletCenterX - enemyCenterX;
            float dy = bulletCenterY - enemyCenterY;
            float distance = sqrt(dx * dx + dy * dy);

            // Collision detection
            float collisionThreshold = 40.0f;

            if (distance < collisionThreshold && it->type == 1)
            {
                // std::cout << "Enemy Destroyed\n";
                score_count += 100;

                // Erase bullet and enemy
                it = Bullets.erase(it);
                Explosion.emplace_back(*en);
                en = enemy.erase(en);

                // Sd3.play();

                enemyDestroyed = true;
                break; // Exit bullet loop
            }
            else
            {
                ++it; // Check next bullet
            }
        }

        // Increment enemy iterator if not destroyed
        if (!enemyDestroyed)
        {
            ++en;
        }
    }

    // Enemy + Jet

    for (auto en = enemy.begin(); en != enemy.end();)
    {

        FloatRect enemyBounds = en->S.getGlobalBounds();
        float enemyCenterX = enemyBounds.left + enemyBounds.width / 2.0f;
        float enemyCenterY = enemyBounds.top + enemyBounds.height / 2.0f;

        FloatRect jetBounds = jet.S.getGlobalBounds();
        float jetCenterX = jetBounds.left + jetBounds.width / 2.0f;
        float jetCenterY = jetBounds.top + jetBounds.height / 2.0f;

        // Calculate the distance between the bee and the bullet
        float dx = jetCenterX - enemyCenterX;
        float dy = jetCenterY - enemyCenterY;
        float distance = sqrt(dx * dx + dy * dy);

        float collisionThreshold = 40.0f;

        if (distance < collisionThreshold)
        {
            // cout << "enemy Destroyed\n";
            score_count += 100;

            // enemy_Destroyed = Jet_Destroyed(enemy.Pos, enemy);
            Explosion.emplace_back(*en);

            // en->Pos = Vector2f(-200, -200);
            // enemy.S.setPosition(enemy.Pos);

            en = enemy.erase(en);

            // enemy.destroyed = true;

            // Sd3.play();

            // cout << "Jet Destroyed\n";
            // score_count += 10;

            // jet_Destroyed = Jet_Destroyed(jet.Pos);
            Explosion.emplace_back(jet);

            jet.Pos = Vector2f(resolution_X / 2.0, resolution_Y + 200);
            jet.S.setPosition(jet.Pos);

            jet.destroyed = true;
            jet_destroyed = true;

            Sd2.play();
        }
        else
        {
            en++;
        }
    }
}

void Jet_Movement(vector<Bullet> &Bullets, Jet &jet, float &deltaTime)
{
    float V = 200;
    float boostMultiplier = 1.5f;

    bool joystickConnected = Joystick::isConnected(0);
    float joystickX = 0, joystickY = 0;

    if (joystickConnected)
    {
        joystickX = Joystick::getAxisPosition(0, Joystick::X) / 100.0f; // Normalize between -1 and 1
        joystickY = Joystick::getAxisPosition(0, Joystick::Y) / 100.0f;

        // Apply a dead zone to avoid accidental slow movements
        if (fabs(joystickX) < 0.2f)
            joystickX = 0;
        if (fabs(joystickY) < 0.2f)
            joystickY = 0;

        // Boost speed if the A button (index 0) is pressed
        if (Joystick::isButtonPressed(0, 0))
        {
            V *= boostMultiplier;
        }
    }

    if ((Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::S)) ||
        (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::A)))
    {
        jet.S.setTextureRect(IntRect(39, 85, 45, 45));
        jet.Vel = Vector2f(0, 0);
    }
    else if (Keyboard::isKeyPressed(Keyboard::W) || joystickY < 0)
    {
        if (Keyboard::isKeyPressed(Keyboard::D) || joystickX > 0)
        {
            jet.S.setTextureRect(IntRect(80, 40, 40, 45));
            jet.Vel = Vector2f(V, -V);
        }
        else if (Keyboard::isKeyPressed(Keyboard::A) || joystickX < 0)
        {
            jet.S.setTextureRect(IntRect(0, 40, 38, 45));
            jet.Vel = Vector2f(-V, -V);
        }
        else
        {
            jet.S.setTextureRect(IntRect(38, 40, 40, 45));
            jet.Vel = Vector2f(0, -V);
        }
    }
    else if (Keyboard::isKeyPressed(Keyboard::S) || joystickY > 0)
    {
        if (Keyboard::isKeyPressed(Keyboard::D) || joystickX > 0)
        {
            jet.S.setTextureRect(IntRect(80, 0, 40, 40));
            jet.Vel = Vector2f(V, V);
        }
        else if (Keyboard::isKeyPressed(Keyboard::A) || joystickX < 0)
        {
            jet.S.setTextureRect(IntRect(0, 0, 38, 40));
            jet.Vel = Vector2f(-V, V);
        }
        else
        {
            jet.S.setTextureRect(IntRect(38, 0, 40, 40));
            jet.Vel = Vector2f(0, V);
        }
    }
    else if (Keyboard::isKeyPressed(Keyboard::D) || joystickX > 0)
    {
        jet.S.setTextureRect(IntRect(80, 84, 38, 45));
        jet.Vel = Vector2f(V, 0);
    }
    else if (Keyboard::isKeyPressed(Keyboard::A) || joystickX < 0)
    {
        jet.S.setTextureRect(IntRect(0, 84, 38, 45));
        jet.Vel = Vector2f(-V, 0);
    }
    else
    {
        jet.S.setTextureRect(IntRect(39, 85, 45, 45));
        jet.Vel = Vector2f(0, 0);
    }
}

void load_All_Images_Textures_Music()
{
    // Objects
    if (!fire_T.loadFromFile("images/fire.png"))
    {
        cout << "Error: Could not load bullet texture!" << endl;
        exit(-1);
    }
    if (!jet_T.loadFromFile("images/Jet_image.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }
    if (!rock_T.loadFromFile("images/rock.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }
    if (!fire2_T.loadFromFile("images/fire2s.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }
    if (!enemy_T.loadFromFile("images/enemy4.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }
    if (!enemy_2_T.loadFromFile("images/enemy3s2.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }
    if (!enemy_3_T.loadFromFile("images/Senemy1.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }

    // destruction Images
    if (!Td1.loadFromFile("images/type_B.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }
    if (!Td2.loadFromFile("images/type_C.png"))
    {
        cout << "Error: Could not load player texture!" << endl;
        exit(-1);
    }

    // static varibale of classes
    Bullet::T = fire_T;
    Jet::T = jet_T;
    Rock::T = rock_T;
    Enemy::T1 = enemy_T;
    Enemy::T2 = enemy_2_T;
    // Enemy::T2 = fire2_T;
    Rock_Explode::T = Td1;
    Jet_Explode::T = Td2;

    // Backgornd
    if (!B1.loadFromFile("images/background_1.jpg"))
    {
        cout << "Error: Could not Backgroud 1 texture!" << endl;
        exit(-1);
    }
    Bg1.setTexture(B1);
    Bg1.setScale((float)resolution_X / B1.getSize().x, (float)resolution_Y / B1.getSize().y);

    if (!B2.loadFromFile("images/background_2.jpg"))
    {
        cout << "Error: Could not Backgroud 2 texture!" << endl;
        exit(-1);
    }
    Bg2.setTexture(B2);
    Bg2.setScale((float)resolution_X / B2.getSize().x, (float)resolution_Y / B2.getSize().y);

    if (!B3.loadFromFile("images/background_3.jpg"))
    {
        cout << "Error: Could not Backgroud 3 texture!" << endl;
        exit(-1);
    }
    Bg3.setTexture(B3);
    Bg3.setScale((float)resolution_X / B3.getSize().x, (float)resolution_Y / B3.getSize().y);

    // Butuns

    float scale = 0.4f;

    if (!start_T.loadFromFile("images/start.png"))
    {
        cout << "Error: Could not start texture!" << endl;
        exit(-1);
    }
    start_S.setTexture(start_T);
    start_S.setScale(scale, scale);
    start_S.setPosition((resolution_X - start_S.getGlobalBounds().width) / 2.0, ((resolution_Y - start_S.getGlobalBounds().height) / 2.0));

    if (!pause_T.loadFromFile("images/pause.png"))
    {
        cout << "Error: Could not pause texture!" << endl;
        exit(-1);
    }
    pause_S.setTexture(pause_T);
    pause_S.setScale(scale, scale);
    pause_S.setPosition((resolution_X - pause_S.getGlobalBounds().width) / 2.0, ((resolution_Y - pause_S.getGlobalBounds().height) / 2.0));
    S_pause.setTexture(pause_T);
    S_pause.setScale(0.1f, 0.1f);
    S_pause.setPosition(10, 10);

    if (!exit_T.loadFromFile("images/exit.png"))
    {
        cout << "Error: Could not Backgroud 3 texture!" << endl;
        exit(-1);
    }
    exit_S.setTexture(exit_T);
    exit_S.setScale(scale / 1.25, scale / 1.25);
    exit_S.setColor(Color::Blue);
    exit_S.setPosition((resolution_X - exit_S.getGlobalBounds().width) / 2.0, ((resolution_Y - exit_S.getGlobalBounds().height) / 2.0) + 100);

    // Background music
    if (!M1.openFromFile("Music/risk.ogg"))
    {
        std::cout << "Error loading music file!" << std::endl;
        exit(-1);
    }
    M1.setLoop(true);
    M1.setVolume(10);
    M1.play();

    // Explosion Sound effect
    if (!Sb1.loadFromFile("Sound Effects/rock-destroy-6409.ogg"))
    {
        std::cerr << "Error: Could not load explosion.ogg!" << std::endl;
        exit(-1);
    }
    Sd1.setBuffer(Sb1);
    Sd1.setVolume(10);

    if (!Sb2.loadFromFile("Sound Effects/the-sound-of-bombing-248404.ogg"))
    {
        std::cerr << "Error: Could not load explosion.ogg!" << std::endl;
        exit(-1);
    }
    Jet_Explode::SB = Sb2;

    Sd2.setBuffer(Sb2);
    Sd2.setVolume(100);
    Sd2.setPlayingOffset(sf::seconds(0.75));
    // cout<<Sb2.getDuration().asSeconds()<<endl;
    Sd3.setBuffer(Sb2);
    Sd3.setVolume(15);
    Sd3.setPlayingOffset(sf::seconds(0.75));

    if (!font.loadFromFile("Fonts/Jersey.ttf")) // Replace "arial.ttf" with the path to your font file
    {
        cout << "Error loading font file!" << endl;
        exit(-1);
    }
}
