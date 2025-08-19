#include <iostream>
#include <raylib.h>
#include <vector>
#include <bitset>
using namespace std;

const int width = 800;
const int height = width;
const float sirka_pole = width/8;
bool proces_hrani = false;
bool konec_hry = false;
char kdo_je_na_rade = 'b';
int prave_hraje_index;

class Panacek{
public:
    Color barva;
    Vector2 pozice;
    Texture2D textura;
    char barva_char;
    string typ;
    Panacek(Vector2 position, string ityp, Texture2D itextura, Color ibarva, char ibarva_char){
        pozice = position;
        typ = ityp;
        textura = itextura;
        barva = ibarva;
        barva_char = ibarva_char;
    }
};

bitset <64> bily_bit;
bitset<64> cerny_bit;

vector <Panacek> panacci;

void vykresli_panacky(){
    for (int p = 0; p<panacci.size();p++){
        Texture2D tex = panacci[p].textura;
        float akt_height = sirka_pole-20;
        float akt_width = tex.width/(tex.height/(sirka_pole-20));
        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};  // celý obrázek
        Rectangle dst = {panacci[p].pozice.x*sirka_pole+sirka_pole/2, panacci[p].pozice.y*sirka_pole+sirka_pole/2, akt_width, akt_height}; // cílová pozice
        Vector2 origin = {akt_width/2, akt_height/2}; // střed textury
        DrawTexturePro(tex, src,dst,origin,0,panacci[p].barva);
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

void vyhodit(Vector2 z_jake_pozice){
    for (int p = 0; p<panacci.size();p++){
        if (panacci[p].pozice.y == z_jake_pozice.y && panacci[p].pozice.x == z_jake_pozice.x && kdo_je_na_rade != panacci[p].barva_char){
            if (panacci[p].typ == "kral") konec_hry = true; else panacci.erase(panacci.begin()+p);
            break;
        }
    }
}

bool kontrola(int index_panacka, Vector2 policko_pos){
    bool policko_je_mozne = false;
    string typ_panacka = panacci[index_panacka].typ;
    int index_bit = panacci[index_panacka].pozice.x + panacci[index_panacka].pozice.y*8;
    int index_bit_pole = policko_pos.x + policko_pos.y*8;
    Vector2 panacek_pos = panacci[index_panacka].pozice;
    char barva_panacka = panacci[index_panacka].barva_char;
    if (policko_pos.x == panacek_pos.x && policko_pos.y == panacek_pos.y) return policko_je_mozne;
    if (barva_panacka == 'b' && bily_bit.test(index_bit_pole)) return policko_je_mozne;
    if (barva_panacka == 'c' && cerny_bit.test(index_bit_pole)) return policko_je_mozne;
    if (typ_panacka == "vez" || typ_panacka == "kralovna"){
            if (panacek_pos.x == policko_pos.x){
                for (int i=1; i< abs(policko_pos.y-panacek_pos.y);i++){
                    if (policko_pos.y >panacek_pos.y){
                        if(cerny_bit.test(index_bit+8*i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit+8*i)) return policko_je_mozne;
                    } else {
                        if(cerny_bit.test(index_bit-8*i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit-8*i)) return policko_je_mozne;
                    }
                }
                policko_je_mozne = true;
            }
            if (panacek_pos.y == policko_pos.y){
                for (int i=1; i< abs(policko_pos.x-panacek_pos.x);i++){
                    if (policko_pos.x >panacek_pos.x){
                        if(cerny_bit.test(index_bit+i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit+i)) return policko_je_mozne;
                    } else {
                        if(cerny_bit.test(index_bit-i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit-i)) return policko_je_mozne;
                    }
                }
                policko_je_mozne = true;
            }
            
        }
    if (typ_panacka == "strelec" || typ_panacka == "kralovna"){
            if (panacek_pos.x-policko_pos.x == panacek_pos.y-policko_pos.y){ //doleva nahoru nebo doprava dolu
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++)
                    if (panacek_pos.x > policko_pos.x){ //doleva nahoru
                        if(cerny_bit.test(index_bit-i-8*i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit-i-8*i)) return policko_je_mozne;
                    } else{
                        if(cerny_bit.test(index_bit+i+8*i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit+i+8*i)) return policko_je_mozne;
                    }
                policko_je_mozne = true;
            }
            if (panacek_pos.x-policko_pos.x == policko_pos.y-panacek_pos.y){ //doprava nahoru nebo doleva dolu
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++)
                    if (panacek_pos.x > policko_pos.x){ //doleva dolu
                        if(cerny_bit.test(index_bit-i+8*i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit-i+8*i)) return policko_je_mozne;
                    } else{
                        if(cerny_bit.test(index_bit+i-8*i)) return policko_je_mozne;
                        if(bily_bit.test(index_bit+i-8*i)) return policko_je_mozne;
                    }
                policko_je_mozne = true;
            }
        }
    if (typ_panacka == "kun"){
            if (abs(policko_pos.x-panacek_pos.x) ==1 && abs(policko_pos.y-panacek_pos.y) ==2) policko_je_mozne = true;
            if (abs(policko_pos.x-panacek_pos.x) ==2 && abs(policko_pos.y-panacek_pos.y) ==1) policko_je_mozne = true;
        }
    if (typ_panacka == "kral"){
        if (abs(policko_pos.x-panacek_pos.x) <= 1 && abs(policko_pos.y-panacek_pos.y) <=1) policko_je_mozne = true;
    }
    if (typ_panacka == "pesek"){
        if ((policko_pos.y-1 == panacek_pos.y && barva_panacka=='b')||(barva_panacka == 'c' && policko_pos.y+1 == panacek_pos.y)){
            if (policko_pos.x == panacek_pos.x) policko_je_mozne = true;
            else if (policko_pos.x-panacek_pos.x == -1){ // jde doleva
                if (barva_panacka == 'b' && cerny_bit.test(index_bit-1+8)) policko_je_mozne = true;
                if (barva_panacka == 'c' && bily_bit.test(index_bit-1-8)) policko_je_mozne = true;
            }
            else if (policko_pos.x-panacek_pos.x == 1){ // jde napravo
                if (barva_panacka == 'b' && cerny_bit.test(index_bit+1+8)) policko_je_mozne = true;
                if (barva_panacka == 'c' && bily_bit.test(index_bit+1-8)) policko_je_mozne = true;
            }
        } else if ((policko_pos.y-2 == panacek_pos.y && barva_panacka=='b' && panacek_pos.y ==1)||(barva_panacka == 'c' && policko_pos.y+2 == panacek_pos.y && panacek_pos.y==6)) policko_je_mozne = true;
    }
    return policko_je_mozne;
}
void kontrola_mysi(){
    if (!proces_hrani && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        float mousex = GetMouseX(), mousey = GetMouseY();
        for (int p = 0; p<panacci.size();p++){
            float rozdil_x = abs((panacci[p].pozice.x*sirka_pole +sirka_pole/2)-mousex);
            float rozdil_y = abs((panacci[p].pozice.y*sirka_pole +sirka_pole/2)-mousey);
            if (rozdil_x<sirka_pole/2&&rozdil_y<sirka_pole/2&&kdo_je_na_rade==panacci[p].barva_char){
                proces_hrani = true;
                prave_hraje_index = p;
                break;
            }
        }
    } else if (proces_hrani){
        Vector2 policko_s_mysi;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            float mouse_x = GetMouseX(), mouse_y = GetMouseY();
            policko_s_mysi = {float(int(mouse_x/sirka_pole)), float(int(mouse_y/sirka_pole))};
            proces_hrani=false;
            cout << "Policko s mysi souradnice: " << int(mouse_x/sirka_pole) << " | " << int(mouse_y/sirka_pole) << endl;
        }
        if (!proces_hrani && kontrola(prave_hraje_index,policko_s_mysi)){
            if (kdo_je_na_rade=='b') bily_bit.reset(panacci[prave_hraje_index].pozice.x + panacci[prave_hraje_index].pozice.y*8);
            if (kdo_je_na_rade=='c') cerny_bit.reset(panacci[prave_hraje_index].pozice.x + panacci[prave_hraje_index].pozice.y*8);
            panacci[prave_hraje_index].pozice = {policko_s_mysi.x, policko_s_mysi.y};
            cout << "Policko kam se odebral: " << panacci[prave_hraje_index].pozice.x << " | " << panacci[prave_hraje_index].pozice.y << endl;
            int pozice_bit = panacci[prave_hraje_index].pozice.x + panacci[prave_hraje_index].pozice.y*8;
            if (kdo_je_na_rade=='b'){
                bily_bit.set(pozice_bit);
                if (cerny_bit.test(pozice_bit)) cerny_bit.reset(pozice_bit), vyhodit(panacci[prave_hraje_index].pozice);
                kdo_je_na_rade='c';
            } else if (kdo_je_na_rade=='c'){
                cerny_bit.set(pozice_bit);
                if (bily_bit.test(pozice_bit)) bily_bit.reset(pozice_bit), vyhodit(panacci[prave_hraje_index].pozice);
                kdo_je_na_rade='b';
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)){
            proces_hrani=false;
        }
    }
}

Texture2D vez, kun, strelec, kral, kralovna, pesek;
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
    nacti_textury();
    for (int s=0; s <16;s++){
        bily_bit.set(s);
        Vector2 pos = {s%8,int(s/8)};
        Texture2D akt_tex;
        string akt_typ;
        if (s == 0 || s == 7) akt_tex=vez, akt_typ = "vez";
        if (s == 1 || s == 6) akt_tex=kun, akt_typ = "kun";
        if (s == 2 || s == 5) akt_tex=strelec, akt_typ = "strelec";
        if (s == 4) akt_tex=kralovna, akt_typ = "kralovna";
        if (s == 3) akt_tex=kral, akt_typ = "kral";
        if (s > 7) akt_tex=pesek, akt_typ = "pesek";
        panacci.push_back(Panacek(pos,akt_typ,akt_tex, WHITE,'b'));
        panacci.push_back(Panacek(pos,akt_typ,akt_tex, WHITE,'b'));
    }
    for (int z=0; z <16;z++){
        cerny_bit.set(63-z);
        Vector2 pos = {z%8, height/sirka_pole-int(z/8)-1};
        Texture2D akt_tex;
        string akt_typ;
        if (z == 0 || z == 7) akt_tex=vez, akt_typ = "vez";
        if (z == 1 || z == 6) akt_tex=kun, akt_typ = "kun";
        if (z == 2 || z == 5) akt_tex=strelec, akt_typ = "strelec";
        if (z == 3) akt_tex=kralovna, akt_typ = "kralovna";
        if (z == 4) akt_tex=kral, akt_typ = "kral";
        if (z > 7) akt_tex=pesek, akt_typ = "pesek";
        panacci.push_back(Panacek(pos,akt_typ,akt_tex,BLACK, 'c'));
    }
    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);
        vykresli_hraci_plochu();
        vykresli_panacky();
        if (!konec_hry){
            kontrola_mysi();
            if (kdo_je_na_rade=='c') DrawText("cerní",10,10,20,BLACK);
            else DrawText("bílí",10,10,20,BLACK);
        } else{
            DrawText("GAME OVER",20,20,70,RED);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}