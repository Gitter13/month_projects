#include <iostream>
#include <raylib.h>

const int width = 800;
const int height = width;
const float sirka_pole = width/8;
void vykresli_hraci_plochu(){
    for (int i=0; i<width; i+= sirka_pole){
        int j;
        if ((int(i/sirka_pole))%2 ==0) j=0; else j = sirka_pole;
        for (j;j<width; j+= 2*sirka_pole){
            DrawRectangle(j,i,sirka_pole,sirka_pole,BROWN);
        }
    }
}

int main(){
    InitWindow(width,height, "sachy");
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        vykresli_hraci_plochu();
        float dt = GetFrameTime();
        DrawCircle(0,0,3,BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}