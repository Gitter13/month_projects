#include <iostream>
#include <raylib.h>
#include <vector>
#include <bitset>
using namespace std;

const int width = 800;
const int height = width;
const float sirka_pole = width/8;


class Panacek{
public:
    Vector2 pozice;
    Texture2D textura;
    char barva;
    Panacek(Vector2 position, char ibarva, Texture2D itextura){
        pozice = position;
        barva = ibarva;
        textura = itextura;
    }
};

bitset <64> bily_bit;
bitset<64> cerny_bit;

vector <Panacek> panacci;

void vykresli_panacky(){
    for (int p = 0; p<panacci.size();p++){
        Texture2D tex = panacci[p].textura;
        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};  // celý obrázek
        Rectangle dst = {panacci[p].pozice.x, panacci[p].pozice.y, (float)tex.width, (float)tex.height}; // cílová pozice
        Vector2 origin = {(float)tex.width/2, (float)tex.height/2}; // střed textury
        DrawTexturePro(tex, src,dst,origin,0,WHITE);
    }
}
void vykresli_hraci_plochu(){
    for (int i=0; i<width; i+= sirka_pole){
        int j;
        if ((int(i/sirka_pole))%2 ==0) j=0; else j = sirka_pole;
        for (j;j<width; j+= 2*sirka_pole){
            DrawRectangle(j,i,sirka_pole,sirka_pole,BROWN);
        }
    }
}

void nacti_textury(){
    //
}
int main(){
    InitWindow(width,height, "sachy");
    for (int s=0; s <16;s++){
        bily_bit.set(s);
        
    }
    for (int z=0; z <16;z++){
        cerny_bit.set(63-z);
    }
    nacti_textury();
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        vykresli_hraci_plochu();
        vykresli_panacky();
        float dt = GetFrameTime();
        DrawCircle(0,0,3,BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}