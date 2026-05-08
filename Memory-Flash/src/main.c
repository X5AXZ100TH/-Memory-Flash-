// Here's a call for the libraries / استدعاء المكتبات 
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
// Define the game State / تعريف حلات اللعبة
typedef enum {STATE_MENU,STATE_PLAYING,STATE_GAMEOVER,STATE_WIN} gameState;
// the game / اللعبة
int main(){
    //window Settings / النافذه
    const int X = 800;
    const int Y = 600;
    InitWindow(X,Y,"/Memory flash/");
    SetTargetFPS(60);
    // seed for the random generator / (!!@#$%#$)بذرة العشوائيه
    srand(time(NULL));
    // Game variables / متغيرات اللعبة
    gameState currentState = STATE_MENU;
    char correctLetter = ' ';
    char rong[6];
    char options[10];
    float displayTimer = 2.5f;
    float cdwon = 2.0f;
    float musprs = 0.1f;
    bool isShowingLetter = true;
    int level = 1;
    int numOptions = 2;
    while (!WindowShouldClose()) {
        // logic Section / قسم المنطق و المعالجه
        switch (currentState) {
            case STATE_MENU:
            // main menu / الواجهه
                if (IsKeyPressed(KEY_ENTER)){
                    level = 1;
                    numOptions = 2;
                    displayTimer = 2.5f;
                    correctLetter = 'A' + (rand() % 26);
                    options[0] = correctLetter;
                    options[1] =  'A' + (rand() % 26);
                    rong[0] = options[1];
                    while (options[1] == options[0]) {
                        options[1] = 'A' + (rand() % 26);
                    }
                    isShowingLetter = true;
                    currentState = STATE_PLAYING;
                }
                break;
            case STATE_PLAYING:
                if (isShowingLetter){
                    // update Memorization time / تحديث وقت الحفظ
                    displayTimer -= GetFrameTime();
                    if(displayTimer <= 0){ 
                        isShowingLetter = false;
                        cdwon = 2.0f;// resat decision timer / ضبط مؤقت القرار 
                        GetKeyPressed();// clear keyboard buffer / تنظيف ضغطات الازرار
                    }}
                else {
                    // Decision timer logic (level 10 - level 15) / منطق مؤقت القرار
                    if(level >= 10){
                        cdwon -= GetFrameTime();
                        if(cdwon <= 0){
                            currentState = STATE_GAMEOVER;
                        }
                    }
                    int key = GetKeyPressed();
                    if (key > 0){
                        // Check the key / تفحص الازرار المضغوطه
                        if (key >= KEY_ONE && key < KEY_ONE + numOptions){
                            int choiceIndex = key - KEY_ONE;
                            if (options[choiceIndex] == correctLetter){
                                // Success logic / منطق الفوز
                                level++;
                                if (level > 15){currentState = STATE_WIN;}
                                displayTimer = 2.5f - (level * musprs);
                                if (displayTimer < 0.5f){displayTimer = 0.5f;}
                                if (level > 5){
                                    numOptions = 2 + (level - 5);
                                }
                                if (numOptions > 6){
                                    numOptions = 6;
                                }
                                correctLetter = 'A' + (rand() % 26);
                                options[0] = correctLetter;
                                int rongCount = 0;
                                for(int i = 1; i < numOptions; i++){
                                    options[i] = 'A' + (rand() % 26);
                                for(int j = 0; j < i; j++){
                                    if(options[i] == options[j]){
                                        i--;
                                        break;}}}
                                for(int i = 0,r = 0; i<numOptions;i++){
                                    if(options[i] != correctLetter){
                                        rong[r] = options[i];
                                        r++;
                                    }
                                }
                                int randIdx = rand() % numOptions;
                                char temp = options[0];
                                options[0]= options[randIdx];
                                options[randIdx] = temp;
                                isShowingLetter = true;
                            }
                            else{
                                // loss logic / منطق الخساره 
                                bool foundIR = false;
                                for(int i =0; i< numOptions-1;i++){
                                    if(options[choiceIndex] == rong[i]){
                                        foundIR = true;
                                        break;
                                    }
                                }
                                if (foundIR){
                                    currentState = STATE_GAMEOVER;
                                }
                            }      
                    }}}
                break;
            case STATE_GAMEOVER:
            // game over / انتهت اللعبة
                if (IsKeyPressed(KEY_R)){
                    level = 1;
                    currentState = STATE_MENU;}
                break;
            case STATE_WIN:
            // win the game / فز اللعبة
                    if (IsKeyPressed(KEY_R)){
                        level = 1;
                        currentState = STATE_MENU;
                    }
                break;
        }
        // Drawing Section(you will discover if on your own) / قسم الرسم (رح تكتشفه لوحدك)
        BeginDrawing();
            ClearBackground(BLACK);
            switch (currentState) {
                case STATE_MENU:
                    DrawText("(Memory-Flash)", 250,100,40, DARKGREEN);
                    DrawText("_____________",250,130,40,DARKGREEN);
                    DrawText("Memorize the Letter.../then pick the correct one!",150,230,20,ORANGE);
                    DrawText("press (ENTER) to start....",180,380,20,RAYWHITE);
                    DrawText("press (ESC) to Exit",220,430,15,GRAY);
                    break;
                case STATE_PLAYING:
                    DrawText(TextFormat("level : %d",level),20,20,20,BLUE);
                    if (isShowingLetter){
                        DrawCircle(400,300,100,DARKGRAY);
                        float start = 0.0f;
                        float end = (displayTimer / (2.5f - (level * musprs))) * 360.0f;
                        DrawCircleSector((Vector2){400, 300},100,start,end,36,GREEN);
                        DrawText(TextFormat("%c", correctLetter),370,250,100,BROWN);
                        DrawText(TextFormat("%.1fs", displayTimer),370,420,20,RAYWHITE);
                    }
                    else{
                        if(level >= 10){
                            DrawText(TextFormat("(%.1f)",cdwon),580,50,25,RED);
                            DrawText("FASTER!?",240,120,20,ORANGE);
                    }
                        DrawText("choose the correct letter:",240,150,25,RAYWHITE);
                        for (int i = 0; i < numOptions; i++){
                            DrawText(TextFormat("%d) %c", i + 1 , options[i]),350,220 + (i * 45),40 , DARKGREEN);
                        }
                    }
                    break;
                case STATE_GAMEOVER:
                    DrawText("============\nGAME OVER\n============",250,200,50,RED);
                    DrawText("press (R) to Return into Menu",260,400,20,DARKGREEN);
                    DrawText("or press (ESC) to exit",260,450,15,DARKGRAY);
                    break;
                case STATE_WIN:
                    DrawText("GOOD NEWS, YOU WIN!?",140,200,40,GOLD);
                    DrawText("prees (R) to play Again",250,350,20,DARKGREEN);
                    DrawText("or press (ESC) to exit",265,390,20,DARKGRAY);
                    break;
    }
    EndDrawing();
}
    CloseWindow();
    return 0;
}// the end / النهايه 