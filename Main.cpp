#include <raylib.h>
#include <raymath.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float FPS = 60;
const float TIMESTEP = 1 / FPS;
const float FRICTION = 0.5;

int main(){
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Homework 3 - Tamano/Olivares - Pool Table");
    while(!WindowShouldClose()){
        BeginDrawing();
        
        ClearBackground(LIME);
        DrawRectangle(75,0,650,40,RED); // Top Bar
        DrawRectangle(0,75,40,450,RED); // Left Bar
        DrawRectangle(760,75,40,450,RED); // Right Bar
        DrawRectangle(75,560,650,40,RED); // Bottom Bar

        DrawCircleV(Vector2{37.5,37.5},37.5,BLACK); // Upper Left
        DrawCircleV(Vector2{800-37.5,37.5},37.5,BLACK); // Upper Right
        DrawCircleV(Vector2{37.5,600-37.5},37.5,BLACK); // Lower Left
        DrawCircleV(Vector2{800-37.5,600-37.5},37.5,BLACK); //Lower Right

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
