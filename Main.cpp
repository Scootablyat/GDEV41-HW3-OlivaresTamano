#include <raylib.h>
#include <raymath.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int numberOfCircles = 3;
const float FPS = 60;
const float TIMESTEP = 1 / FPS; // Sets the timestep to 1 / FPS. But timestep can be any very small value.
const float FRICTION = 0.5;


struct Ball {
    Vector2 position;
    float radius;
    Color color;

    float mass;
    float inverse_mass; // A variable for 1 / mass. Used in the calculation for acceleration = sum of forces / mass
    Vector2 acceleration;
    Vector2 velocity;
};

int toggleElasticity(int e){
    if(e == 0){
        return 1;
    }
    return 0;
}

float getDistance(Ball b1, Ball b2){
    Vector2 dist = Vector2Subtract(b1.position, b2.position);
    return std::abs(Vector2Length(dist));
}

bool isCirclesColliding(Ball b1, Ball b2){
    float sumOfRadii = b1.radius + b2.radius; 
    float distance = getDistance(b1, b2);
    if(distance <= sumOfRadii){
        std::cout << "colliding" << std::endl;
        return true;
    }
    return false;
}

int main() {
    Ball ball;
    ball.position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    ball.radius = 30.0f;
    ball.color = WHITE;
    ball.mass = 1.0f;
    ball.inverse_mass = 1 / ball.mass;
    ball.acceleration = Vector2Zero();
    ball.velocity = Vector2Zero();

    Ball ball2;
    ball2.position = {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4};
    ball2.radius = 30.0f;
    ball2.color = BLACK;
    ball2.mass = 1.0f;
    ball2.inverse_mass = 1 / ball.mass;
    ball2.acceleration = Vector2Zero();
    ball2.velocity = Vector2Zero();

    Ball ball3;
    ball3.position = {WINDOW_WIDTH- 200, WINDOW_HEIGHT- 200};
    ball3.radius = 30.0f;
    ball3.color = YELLOW;
    ball3.mass = 1.0f;
    ball3.inverse_mass = 1 / ball.mass;
    ball3.acceleration = Vector2Zero();
    ball3.velocity = Vector2Zero();

    Ball ballArray[numberOfCircles] = {ball, ball2, ball3};

    int elasticityCoefficient = 0;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tamano - Exercise 4");

    SetTargetFPS(FPS);

    float accumulator = 0;

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        Vector2 forces = Vector2Zero();
        if(IsKeyPressed(KEY_SPACE)){
            elasticityCoefficient = toggleElasticity(elasticityCoefficient);
            std::cout << "elasticity: " << elasticityCoefficient << std::endl;
        }

        if(IsKeyDown(KEY_W)) {
            //std::cout << ball.acceleration.x << " " << ball.acceleration.y << std::endl;
            forces = Vector2Add(forces, {0, -100});
        }
        if(IsKeyDown(KEY_A)) {
            //std::cout << ball.acceleration.x << " " << ball.acceleration.y << std::endl;
            forces = Vector2Add(forces, {-100, 0});
        }
        if(IsKeyDown(KEY_S)) {
            //std::cout << ball.acceleration.x << " " << ball.acceleration.y << std::endl;
            forces = Vector2Add(forces, {0, 100});
        }
        if(IsKeyDown(KEY_D)) {
            //std::cout << ball.acceleration.x << " " << ball.acceleration.y << std::endl;
            forces = Vector2Add(forces, {100, 0});
        }

        ballArray[0].acceleration = Vector2Scale(forces, ball.inverse_mass);
        
        // Physics
        accumulator += delta_time;
        while(accumulator >= TIMESTEP) {
            for(int i = 0; i < numberOfCircles; i++){
                // std::cout << "i, k: " << i << " " << k << std::endl;
                ballArray[i].velocity = Vector2Add(ballArray[i].velocity, Vector2Scale(ballArray[i].acceleration, TIMESTEP));
                ballArray[i].velocity = Vector2Subtract(ballArray[i].velocity, Vector2Scale(ballArray[i].velocity, FRICTION * ballArray[i].inverse_mass * TIMESTEP));
                ballArray[i].position = Vector2Add(ballArray[i].position, Vector2Scale(ballArray[i].velocity, TIMESTEP));
                
                if(ballArray[i].position.x + ballArray[i].radius >= WINDOW_WIDTH || ballArray[i].position.x - ballArray[i].radius <= 0) {
                    ballArray[i].velocity.x *= -1;std::cout << ballArray[i].velocity.x << std::endl;
                }
                if(ballArray[i].position.y + ballArray[i].radius >= WINDOW_HEIGHT || ballArray[i].position.y - ballArray[i].radius <= 0) {   
                    ballArray[i].velocity.y *= -1;std::cout << ballArray[i].velocity.y << std::endl;
                }
                for(int k = 0; k < numberOfCircles; k++){
                    if(k == i){
                        k++;
                    }
                    if(k >= numberOfCircles || i >= numberOfCircles){
                        break;
                    }
                    if(isCirclesColliding(ballArray[i], ballArray[k])){
                        Vector2 n = Vector2Subtract(ballArray[i].position, ballArray[k].position); // currently swapped (as per sir's comment)
                        float j = -( ((1+elasticityCoefficient)*Vector2DotProduct(Vector2Subtract(ballArray[i].velocity, ballArray[k].velocity/**/), n)) 
                                    / (Vector2DotProduct(n,n)*(1/ballArray[i].mass)+(1/ballArray[k].mass)) );
                        Vector2 newVelocity = Vector2Add(ballArray[i].velocity, Vector2Scale(n, (j/ballArray[i].mass) ));
                        ballArray[i].velocity = newVelocity;
                        Vector2 newVelocity2 = Vector2Subtract(ballArray[k].velocity, Vector2Scale(n, (j/ballArray[k].mass) ));
                        ballArray[k].velocity = newVelocity2;
                    }
                }
            }
            accumulator -= TIMESTEP;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircleV(ballArray[0].position, ballArray[0].radius, ballArray[0].color);
        DrawCircleV(ballArray[1].position, ballArray[1].radius, ballArray[1].color);
        DrawCircleV(ballArray[2].position, ballArray[2].radius, ballArray[2].color);

        // pool table border
        ClearBackground(LIME);
        DrawRectangle(75,0,650,40,RED); // Top Bar
        DrawRectangle(0,75,40,450,RED); // Left Bar
        DrawRectangle(760,75,40,450,RED); // Right Bar
        DrawRectangle(75,560,650,40,RED); // Bottom Bar
        

        // pool table corners
        DrawCircleV(Vector2{37.5,37.5},37.5,BLACK); // Upper Left
        DrawCircleV(Vector2{800-37.5,37.5},37.5,BLACK); // Upper Right
        DrawCircleV(Vector2{37.5,600-37.5},37.5,BLACK); // Lower Left
        DrawCircleV(Vector2{800-37.5,600-37.5},37.5,BLACK); //Lower Right


        EndDrawing();
    }
    CloseWindow();
    return 0;
}