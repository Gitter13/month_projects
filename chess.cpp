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
    string typ;
    Panacek(Vector2 position, string ityp, Texture2D itextura){
        pozice = position;
        typ = ityp;
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

Texture2D vez;
Texture2D kun;
Texture2D strelec;
Texture2D kral;
Texture2D kralovna;
Texture2D pesek;
void nacti_textury(){
    vez = LoadTexture("vez.png");
    kun = LoadTexture("kun.png");
    strelec = LoadTexture("strelec.png");
    kral = LoadTexture("kral.png");
    kralovna = LoadTexture("kralovna.png");
    pesek = LoadTexture("pesek.png");
}
int main(){
    InitWindow(width,height, "sachy");
    for (int s=0; s <16;s++){
        bily_bit.set(s);
        Vector2 pos = {sirka_pole/2+sirka_pole*(s%8), sirka_pole/2+int(s/8)*sirka_pole};
        Texture2D akt_tex;
        string akt_typ;
        if (s == 0 || s == 7) akt_tex=vez, akt_typ = "vez";
        if (s == 1 || s == 6) akt_tex=kun, akt_typ = "kun";
        if (s == 2 || s == 5) akt_tex=strelec, akt_typ = "strelec";
        if (s == 4) akt_tex=kralovna, akt_typ = "kralovna";
        if (s == 3) akt_tex=kral, akt_typ = "kral";
        if (s > 7) akt_tex=pesek, akt_typ = "pesek";
        panacci.push_back(Panacek(pos,akt_typ,akt_tex));
    }
    for (int z=0; z <16;z++){
        cerny_bit.set(63-z);
        Vector2 pos = {sirka_pole/2+sirka_pole*(z%8), height-(sirka_pole/2+int(z/8)*sirka_pole)};
        Texture2D akt_tex;
        string akt_typ;
        if (z == 0 || z == 7) akt_tex=vez, akt_typ = "vez";
        if (z == 1 || z == 6) akt_tex=kun, akt_typ = "kun";
        if (z == 2 || z == 5) akt_tex=strelec, akt_typ = "strelec";
        if (z == 3) akt_tex=kralovna, akt_typ = "kralovna";
        if (z == 4) akt_tex=kral, akt_typ = "kral";
        if (z > 7) akt_tex=pesek, akt_typ = "pesek";
        panacci.push_back(Panacek(pos,akt_typ,akt_tex));
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