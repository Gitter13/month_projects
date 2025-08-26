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
int prave_hraje_index = -1;
vector <bool> rosada_mozna_bila = {true, true};
vector <bool> rosada_mozna_cerna = {true, true}; 

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
class sipka{
public:
    Vector2 pozice_odkud;
    Vector2 pozice_kam;
    sipka(Vector2 odkud, Vector2 kam){
        pozice_kam=kam;
        pozice_odkud = odkud;
    }
    void draw(){
        if (pozice_kam.x != pozice_odkud.x || pozice_kam.y != pozice_odkud.y){
            Vector2 odkud_real = {pozice_odkud.x*sirka_pole+sirka_pole/2, pozice_odkud.y*sirka_pole+sirka_pole/2};
            Vector2 kam_real = {pozice_kam.x*sirka_pole+sirka_pole/2, pozice_kam.y*sirka_pole+sirka_pole/2};
            DrawLineEx(odkud_real,kam_real, 6,GRAY);
            DrawCircle(kam_real.x,kam_real.y, 10,GRAY);
        }
    }
};
bitset <64> bily_bit;
bitset<64> cerny_bit;

sipka ukazatel = sipka({0,0},{0,0});
vector <Panacek> panacci;

void vykresli_panacky(){
    for (int p = 0; p<panacci.size();p++){
        Texture2D tex = panacci[p].textura;
        float akt_height = sirka_pole-20;
        float akt_width = tex.width/(tex.height/(sirka_pole-20));
        int pridavek_peska = 0;
        if (panacci[p].typ == "pesek") akt_height -=30, akt_width = tex.width/(tex.height/(sirka_pole-50)), pridavek_peska=15;
        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};  // celý obrázek
        Rectangle dst = {panacci[p].pozice.x*sirka_pole+sirka_pole/2, panacci[p].pozice.y*sirka_pole+sirka_pole/2+pridavek_peska, akt_width, akt_height}; // cílová pozice
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
            if (panacci[p].typ == "kral") konec_hry = true; else panacci[p].pozice = {-1,0};//šoupnem ho mimo hrací plochu
            break;
        }
    }
}
int najdi (Vector2 jaka_pozice){
    for (int p = 0; p<panacci.size();p++){
        if (panacci[p].pozice.y == jaka_pozice.y && panacci[p].pozice.x == jaka_pozice.x){
            return p;
        }
    }
}

bool rosada_probiha = false;
Vector2 vez_byla, vez_bude, kral_byl, kral_bude;
string rosada_typ;
bitset <64> cerny_mezibit, bily_mezibit;
bool kontrola(int index_panacka, Vector2 policko_pos){
    bool policko_je_mozne = false;
    string typ_panacka = panacci[index_panacka].typ;
    int index_bit = panacci[index_panacka].pozice.x + panacci[index_panacka].pozice.y*8;
    int index_bit_pole = policko_pos.x + policko_pos.y*8;
    Vector2 panacek_pos = panacci[index_panacka].pozice;
    char barva_panacka = panacci[index_panacka].barva_char;
    if (policko_pos.x == panacek_pos.x && policko_pos.y == panacek_pos.y) return policko_je_mozne;
    if (barva_panacka == 'b' && bily_mezibit.test(index_bit_pole)){
        // kod::::rosada::<::<
        if (typ_panacka=="kral"){
            if (policko_pos.x==0 && policko_pos.y==0 && rosada_mozna_bila[0]){ //bila rosada ta vlevo (kratsi)
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++){
                    if (cerny_mezibit.test(panacek_pos.x-i) || bily_mezibit.test(panacek_pos.x-i)) return policko_je_mozne;
                }
                policko_je_mozne = true;
                rosada_probiha = true;

                rosada_typ = "bila_vlevo";

            }
            else if(policko_pos.y==0 && policko_pos.x==7 && rosada_mozna_bila[1]){ //bila vpravo (delsi)
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++){
                    if (cerny_mezibit.test(panacek_pos.x+i) || bily_mezibit.test(panacek_pos.x+i)) return policko_je_mozne;
                }
                policko_je_mozne = true;
                rosada_probiha = true;

                rosada_typ = "bila_vpravo";

            } else return policko_je_mozne;
        } else return policko_je_mozne;
    }
    if (barva_panacka == 'c' && cerny_mezibit.test(index_bit_pole)){
        // kodik:::<<#:::rosada
        if (typ_panacka=="kral"){
            if (policko_pos.x==0 && policko_pos.y==7 && rosada_mozna_cerna[0]){ //cerna vlevo (delsi)
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++){
                    if (cerny_mezibit.test(index_bit-i) || bily_mezibit.test(index_bit-i)) return policko_je_mozne; 
                }
                policko_je_mozne = true;
                rosada_probiha = true;
                
                rosada_typ = "cerna_vlevo";

            }
            else if(policko_pos.y==7 && policko_pos.x==7 && rosada_mozna_cerna[1]){ //cerna vpravo (kratsi)
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++){
                    if (cerny_mezibit.test(index_bit+i) || bily_mezibit.test(index_bit+i)) return policko_je_mozne;
                }
                policko_je_mozne = true;
                rosada_probiha = true;

                rosada_typ = "cerna_vpravo";

            } else return policko_je_mozne;
        } else return policko_je_mozne;
    }
    if (typ_panacka == "vez" || typ_panacka == "kralovna"){
            if (panacek_pos.x == policko_pos.x){
                for (int i=1; i< abs(policko_pos.y-panacek_pos.y);i++){
                    if (policko_pos.y >panacek_pos.y){
                        if(cerny_mezibit.test(index_bit+8*i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit+8*i)) return policko_je_mozne;
                    } else {
                        if(cerny_mezibit.test(index_bit-8*i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit-8*i)) return policko_je_mozne;
                    }
                }
                policko_je_mozne = true;
                if (typ_panacka=="vez"){
                    if (panacek_pos.x ==0){
                        if (barva_panacka=='b') rosada_mozna_bila[0]=false;
                        if (barva_panacka=='c') rosada_mozna_cerna[0]=false;
                    } else if (panacek_pos.x ==7){
                        if (barva_panacka=='b') rosada_mozna_bila[1]=false;
                        if (barva_panacka=='c') rosada_mozna_cerna[1]=false;
                    }
                }
            }
            if (panacek_pos.y == policko_pos.y){
                for (int i=1; i< abs(policko_pos.x-panacek_pos.x);i++){
                    if (policko_pos.x >panacek_pos.x){
                        if(cerny_mezibit.test(index_bit+i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit+i)) return policko_je_mozne;
                    } else {
                        if(cerny_mezibit.test(index_bit-i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit-i)) return policko_je_mozne;
                    }
                }
                policko_je_mozne = true;
                if (typ_panacka=="vez"){
                    if (panacek_pos.x ==0){
                        if (barva_panacka=='b') rosada_mozna_bila[0]=false;
                        if (barva_panacka=='c') rosada_mozna_cerna[0]=false;
                    } else if (panacek_pos.x ==7){
                        if (barva_panacka=='b') rosada_mozna_bila[1]=false;
                        if (barva_panacka=='c') rosada_mozna_cerna[1]=false;
                    }
                }
            }
            
        }
    if (typ_panacka == "strelec" || typ_panacka == "kralovna"){
            if (panacek_pos.x-policko_pos.x == panacek_pos.y-policko_pos.y){ //doleva nahoru nebo doprava dolu
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++)
                    if (panacek_pos.x > policko_pos.x){ //doleva nahoru
                        if(cerny_mezibit.test(index_bit-i-8*i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit-i-8*i)) return policko_je_mozne;
                    } else{
                        if(cerny_mezibit.test(index_bit+i+8*i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit+i+8*i)) return policko_je_mozne;
                    }
                policko_je_mozne = true;
            }
            if (panacek_pos.x-policko_pos.x == policko_pos.y-panacek_pos.y){ //doprava nahoru nebo doleva dolu
                for (int i=1; i<abs(panacek_pos.x-policko_pos.x);i++)
                    if (panacek_pos.x > policko_pos.x){ //doleva dolu
                        if(cerny_mezibit.test(index_bit-i+8*i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit-i+8*i)) return policko_je_mozne;
                    } else{
                        if(cerny_mezibit.test(index_bit+i-8*i)) return policko_je_mozne;
                        if(bily_mezibit.test(index_bit+i-8*i)) return policko_je_mozne;
                    }
                policko_je_mozne = true;
            }
        }
    if (typ_panacka == "kun"){
            if (abs(policko_pos.x-panacek_pos.x) ==1 && abs(policko_pos.y-panacek_pos.y) ==2) policko_je_mozne = true;
            if (abs(policko_pos.x-panacek_pos.x) ==2 && abs(policko_pos.y-panacek_pos.y) ==1) policko_je_mozne = true;
        }
    if (typ_panacka == "kral"){
        if (abs(policko_pos.x-panacek_pos.x) <= 1 && abs(policko_pos.y-panacek_pos.y) <=1){
            policko_je_mozne = true;
        }
    }
    if (typ_panacka == "pesek"){
        if ((policko_pos.y-1 == panacek_pos.y && barva_panacka=='b')||(barva_panacka == 'c' && policko_pos.y+1 == panacek_pos.y)){
            if (policko_pos.x == panacek_pos.x){
                if (barva_panacka == 'b' && !cerny_mezibit.test(index_bit+8)) policko_je_mozne = true;
                if (barva_panacka == 'c' && !bily_mezibit.test(index_bit-8)) policko_je_mozne = true;
            }
            else if (policko_pos.x-panacek_pos.x == -1){ // jde doleva
                if (barva_panacka == 'b' && cerny_mezibit.test(index_bit-1+8)) policko_je_mozne = true;
                if (barva_panacka == 'c' && bily_mezibit.test(index_bit-1-8)) policko_je_mozne = true;
            }
            else if (policko_pos.x-panacek_pos.x == 1){ // jde napravo
                if (barva_panacka == 'b' && cerny_mezibit.test(index_bit+1+8)) policko_je_mozne = true;
                if (barva_panacka == 'c' && bily_mezibit.test(index_bit+1-8)) policko_je_mozne = true;
            }
        } else if (policko_pos.x == panacek_pos.x&&((policko_pos.y-2 == panacek_pos.y && barva_panacka=='b' && panacek_pos.y ==1 && !cerny_mezibit.test(index_bit+16))||(barva_panacka == 'c' && policko_pos.y+2 == panacek_pos.y && panacek_pos.y==6 && !bily_mezibit.test(index_bit-16)))) policko_je_mozne = true;
    }
    return policko_je_mozne;
}

bool kontrola_šachu(Vector2 pozice_krale, char barva_krale){
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int p = 0; p<panacci.size();p++){
        if (panacci[p].pozice.x >=0 && panacci[p].barva_char != barva_krale){
            if (kontrola(p, pozice_krale)) return true;
        }
    }
    return false;
}

void kontrola_mysi(){
    if (!proces_hrani) prave_hraje_index = -1;
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
        }
        if (!proces_hrani && kontrola(prave_hraje_index,policko_s_mysi)){
            if (!rosada_probiha){
                if (kdo_je_na_rade=='b') bily_mezibit.reset(panacci[prave_hraje_index].pozice.x + panacci[prave_hraje_index].pozice.y*8);
                if (kdo_je_na_rade=='c') cerny_mezibit.reset(panacci[prave_hraje_index].pozice.x + panacci[prave_hraje_index].pozice.y*8);
                int pozice_bit = policko_s_mysi.x + policko_s_mysi.y*8;
                bool vyhodime = false;
                if (kdo_je_na_rade=='b'){
                    bily_mezibit.set(pozice_bit);
                    if (cerny_mezibit.test(pozice_bit)) cerny_mezibit.reset(pozice_bit), vyhodime = true;
                } else if (kdo_je_na_rade=='c'){
                    cerny_mezibit.set(pozice_bit);
                    if (bily_mezibit.test(pozice_bit)) bily_mezibit.reset(pozice_bit), vyhodime =true;
                }
                if (panacci[prave_hraje_index].typ != "kral" || !kontrola_šachu(policko_s_mysi, panacci[prave_hraje_index].barva_char)){
                    if (panacci[prave_hraje_index].typ == "kral"){
                        if (panacci[prave_hraje_index].barva_char=='b') rosada_mozna_bila = {false,false};
                        if (panacci[prave_hraje_index].barva_char == 'c') rosada_mozna_cerna = {false,false};
                    }
                    ukazatel.pozice_odkud = panacci[prave_hraje_index].pozice; ukazatel.pozice_kam = policko_s_mysi;
                    panacci[prave_hraje_index].pozice = policko_s_mysi;
                    cerny_bit = cerny_mezibit; bily_bit = bily_mezibit;
                    if (vyhodime) vyhodit(panacci[prave_hraje_index].pozice);
                    if (panacci[prave_hraje_index].barva_char=='b') kdo_je_na_rade='c';
                    if (panacci[prave_hraje_index].barva_char == 'c') kdo_je_na_rade='b';
                }else cout<<"NoOo sebevrazda"<<endl;
            } else{
                rosada_probiha = false;
                if (rosada_typ == "bila_vlevo") vez_byla = {0,0}, vez_bude = {2,0}, kral_byl = {3,0}, kral_bude = {1,0};
                if (rosada_typ == "bila_vpravo") vez_byla = {7,0}, vez_bude = {4,0}, kral_byl = {3,0}, kral_bude = {5,0};
                if (rosada_typ == "cerna_vlevo") vez_byla = {0,7}, vez_bude = {3,7}, kral_byl = {4,7}, kral_bude = {2,7};
                if (rosada_typ == "cerna_vpravo") vez_byla = {7,7}, vez_bude = {5,7}, kral_byl = {4,7}, kral_bude = {6,7};
                if (!kontrola_šachu(panacci[prave_hraje_index].pozice, panacci[prave_hraje_index].barva_char)){
                    if (kdo_je_na_rade == 'c'){
                        //vezicka
                        cerny_mezibit.reset(vez_byla.x+vez_byla.y*8);
                        cerny_mezibit.set(vez_bude.x+vez_bude.y*8);
                        //kralicek
                        cerny_mezibit.reset(kral_byl.y*8+kral_byl.x);
                        cerny_mezibit.set(kral_bude.y*8+kral_bude.x);
                    } else if (kdo_je_na_rade == 'b'){
                        //vezicka
                        bily_mezibit.reset(vez_byla.x+vez_byla.y*8);
                        bily_mezibit.set(vez_bude.x+vez_bude.y*8);
                        //kralicek
                        bily_mezibit.reset(kral_byl.y*8+kral_byl.x);
                        bily_mezibit.set(kral_bude.y*8+kral_bude.x);
                    }
                    if(!kontrola_šachu(kral_bude, panacci[prave_hraje_index].barva_char)){
                        if (kdo_je_na_rade=='c') rosada_mozna_cerna = {false,false}, kdo_je_na_rade='b';
                        else if (kdo_je_na_rade=='b') rosada_mozna_bila = {false, false}, kdo_je_na_rade ='c';
                        cerny_bit = cerny_mezibit; bily_bit = bily_mezibit;
                        panacci[najdi(vez_byla)].pozice = vez_bude;
                        panacci[prave_hraje_index].pozice = kral_bude;
                        ukazatel.pozice_odkud = vez_bude; ukazatel.pozice_kam = kral_bude;
                    }
                }
            }
            bily_mezibit = bily_bit; cerny_mezibit = cerny_bit;
        }
    }
}


Texture2D vez, kun, strelec, kral, kralovna, pesek, sach;
void nacti_textury(){
    vez = LoadTexture("vez.png");
    kun = LoadTexture("kun.png");
    strelec = LoadTexture("strelec.png");
    kral = LoadTexture("kral.png");
    kralovna = LoadTexture("kralovna.png");
    pesek = LoadTexture("pesek.png");
    sach = LoadTexture("sach.png");
}

void vykresli_sach(){
    for (int p=0; p<panacci.size();p++){
        if (panacci[p].typ == "kral" && kontrola_šachu(panacci[p].pozice, panacci[p].barva_char)) DrawTextureEx(sach, {panacci[p].pozice.x*sirka_pole, panacci[p].pozice.y*sirka_pole},0, sirka_pole/sach.height, WHITE);
    }
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
    bily_mezibit = bily_bit; cerny_mezibit = cerny_bit;
    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);
        vykresli_hraci_plochu(); // to po tomhle kódu je vykreslí růžového políčka
        if (prave_hraje_index !=-1) DrawRectangleV({panacci[prave_hraje_index].pozice.x*sirka_pole, panacci[prave_hraje_index].pozice.y*sirka_pole}, {sirka_pole,sirka_pole},PURPLE);
        vykresli_panacky();
        vykresli_sach();
        ukazatel.draw();
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