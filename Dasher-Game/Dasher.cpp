#include "raylib.h"
#include <bits/stdc++.h>
struct Animdata
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float update_time;
    float running_time;
};

bool isonground(Animdata data, int windowheight)
{
    return data.pos.y >= windowheight - data.rec.height;
}

Animdata updateanimdata(Animdata data, float dt, int maxframe)
{
    data.running_time += dt;
    if (data.running_time >= data.update_time)
    {
        data.running_time = 0;
        data.rec.x = data.rec.width * data.frame;
        data.frame++;
        if (data.frame > 5)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    int windowdimension[2];
    windowdimension[0] = 512;
    windowdimension[1] = 380;
    InitWindow(windowdimension[0], windowdimension[1], "Rishit's window");

    //Character of game settings
    Texture2D character = LoadTexture("textures/scarfy.png");
    Animdata characterdata;
    characterdata.rec.width = character.width / 6.0;
    characterdata.rec.height = character.height;
    characterdata.rec.x = 0;
    characterdata.rec.y = 0;
    characterdata.pos.x = windowdimension[0] / 2 - characterdata.rec.width / 2;
    characterdata.pos.y = windowdimension[1] - characterdata.rec.height;
    characterdata.frame = 0;
    characterdata.update_time = 1.0 / 12.0;
    characterdata.running_time = 0;
    //Nebula
    Texture2D Nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    int nebvel = -400;

    const int size_nebulae = 20;
    //animdata for nebula
    Animdata nebulae[size_nebulae]{};
    int k = 100;
    for (int i = 0; i < size_nebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = Nebula.width / 8;
        nebulae[i].rec.height = Nebula.height / 8;
        nebulae[i].pos.y = windowdimension[1] - Nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].running_time = 0.0;
        nebulae[i].update_time = 0.0;
        nebulae[i].pos.x = windowdimension[0] + (i * 700) + k;
        k += 20;
    }

    float finishline = nebulae[size_nebulae - 1].pos.x;
    //gravity
    const int gravity = 2000; //in pixel/s^2
    const int jumpvel = -800; //pixel/s

    int velocity = 0;

    bool isair = false;

    Texture2D background = LoadTexture("textures/far-buildings.png");

    float bgmovex;

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgmovex;
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgmovex;

    bool collision = false;
    SetTargetFPS(60);//1/60

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        const float dt = GetFrameTime(); //to get time between frames
        bgmovex -= 30 * dt;
        mgmovex -= 60 * dt;
        fgmovex -= 120 * dt;

        if (bgmovex <= -background.width * 2)
        {
            bgmovex = 0;
        }
        if (mgmovex <= -midground.width * 2)
        {
            mgmovex = 0;
        }
        if (fgmovex <= -foreground.width * 2)
        {
            fgmovex = 0;
        }
        //draw background
        Vector2 bg1pos{bgmovex, 0.0};
        DrawTextureEx(background, bg1pos, 0.0, 2.0, WHITE);
        Vector2 bg2pos{bgmovex + background.width * 2, 0.0};
        DrawTextureEx(background, bg2pos, 0.0, 2.0, WHITE);

        //draw midground
        Vector2 mg1pos{mgmovex, 0.0};
        DrawTextureEx(midground, mg1pos, 0.0, 2.0, WHITE);
        Vector2 mg2pos{mgmovex + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2pos, 0.0, 2.0, WHITE);

        //draw foreground
        Vector2 fg1pos{fgmovex, 0.0};
        DrawTextureEx(foreground, fg1pos, 0.0, 2.0, WHITE);
        Vector2 fg2pos{fgmovex + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2pos, 0.0, 2.0, WHITE);

        if (isonground(characterdata, windowdimension[1]))
        {
            velocity = 0;
            isair = false;
        }
        else
        {
            velocity += gravity * dt;
            isair = true;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            if (!isair)
                velocity += jumpvel;
        }

        if(IsKeyDown(KEY_LEFT)&&characterdata.pos.x>=0&&!isair)
        {
            characterdata.pos.x-=10;
        }
        if(IsKeyDown(KEY_RIGHT)&&characterdata.pos.x<=windowdimension[0]&&!isair)
        {
            characterdata.pos.x+=10;
        }
        //update nebula position
        for (int i = 0; i < size_nebulae; i++)
        {
            nebulae[i].pos.x += nebvel * dt;
        }

        finishline += nebvel * dt; //update finishline

        //update position character
        characterdata.pos.y += velocity * dt;

        //updating character animation frame
        if (!isair)
        {
            characterdata = updateanimdata(characterdata, dt, 5);
        }
        //update nebula animation frame
        for (int i = 0; i < size_nebulae; i++)
        {
            nebulae[i] = updateanimdata(nebulae[i], dt, 7);
        }

        //check collision
        for (Animdata nebula : nebulae)
        {
            float pad = 50;
            Rectangle nebrec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad};
            Rectangle charrec{characterdata.pos.x,
                              characterdata.pos.y,
                              characterdata.rec.width,
                              characterdata.rec.height};
            if(CheckCollisionRecs(nebrec,charrec))
            {
                collision=true;
            }
        }

        if (collision)
        {
           DrawText("You lose!",windowdimension[0]/2,windowdimension[1]/2,40,WHITE);
        }
        else if(characterdata.pos.x>=finishline)
        {
            DrawText("You Win!",windowdimension[0]/2,windowdimension[1]/2,40,WHITE);
        }
        else
        {
            //nebulae
            for (int i = 0; i < size_nebulae; i++)
            {
                DrawTextureRec(Nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            //character
            DrawTextureRec(character, characterdata.rec, characterdata.pos, WHITE);
        }
        EndDrawing();
    }
    UnloadTexture(character);
    UnloadTexture(Nebula);
    UnloadTexture(background);
    UnloadTexture(foreground);
    UnloadTexture(midground);
    CloseWindow();
    return 0;
}