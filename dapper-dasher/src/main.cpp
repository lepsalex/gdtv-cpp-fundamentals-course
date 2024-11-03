#include "raylib.h"

struct EntityData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    Color tint;
};

bool isOnGround(const EntityData &entityData, const int windowHeight) {
    return entityData.pos.y + entityData.rec.height >= windowHeight;
}

void animateEntity(EntityData &entityData, const int maxFrame, const float DT) {
    entityData.runningTime += DT;
    if (entityData.runningTime >= entityData.updateTime) {
        // reset anim timer
        entityData.runningTime = 0.0f;

        // update frame
        entityData.frame = (entityData.frame + 1) % maxFrame;

        // update texture position in rect
        entityData.rec.x = entityData.frame * entityData.rec.width;
    }
}

float moveBkgLayer(const float x, const float velocity, const Texture2D &texture, const float scale, const float DT) {
    float newX = x - (velocity * DT);

    // reset to 0 if scrolling past second texture
    if (newX <= texture.width * -scale) {
        newX = 0.0;
    }

    return newX;
}

void drawBkgLayer(const float x, const float scale, const Texture2D &texture) {
    Vector2 bg1Pos{x, 0};
    Vector2 bg2Pos{x + texture.width * scale, 0};
    DrawTextureEx(texture, bg1Pos, 0.0, 2.0, WHITE);
    DrawTextureEx(texture, bg2Pos, 0.0, 2.0, WHITE);
}

int main(void) {
    // Window
    constexpr int windowDimensions[2]{512, 380};

    // World Constants
    constexpr int GRAVITY{1'600}; // (pixels/s)/s

    // Entity Constants
    constexpr int jumpVelocity{-800}; // pixels/s

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");
    SetTargetFPS(60);

    // Load Resources
    const Texture2D scarfyTexture = LoadTexture("resources/scarfy.png");
    const Texture2D nebulaTexture = LoadTexture("resources/12_nebula_spritesheet.png");
    const Texture2D backgroundTexture = LoadTexture("resources/far-buildings.png");
    const Texture2D midgroundTexture = LoadTexture("resources/back-buildings.png");
    const Texture2D foregroundTexture = LoadTexture("resources/foreground.png");

    // Game State
    bool playerCollided{false};
    bool playerWon{false};

    // Background Layer State
    float bgX = 0;
    float mgX = 0;
    float fgX = 0;

    // Player State
    const float scarfyWidth = static_cast<float>(scarfyTexture.width) / 6;
    const float scarfyHeight = static_cast<float>(scarfyTexture.width) / 6;
    constexpr int scaryAnimMaxFrame = 6;
    float playerVelocity{0.0f};
    bool isPlayerGrounded{true};
    EntityData scarfyData{
        .rec = {
            .width = scarfyWidth,
            .height = scarfyHeight,
            .x = 0.0f,
            .y = 0.0f
        },
        .pos = {
            .x = (windowDimensions[0] / 2) - scarfyWidth,
            .y = windowDimensions[1] - scarfyHeight
        },
        .frame = 0,
        .updateTime = 1.0 / 12.0,
        .runningTime = 0.0f,
        .tint = WHITE
    };


    // Nebula State
    const float nebulaWidth = static_cast<float>(nebulaTexture.width) / 8;
    const float nebulaHeight = static_cast<float>(nebulaTexture.width) / 8;
    constexpr float nebulaVelocity{200.0f}; // pixels/s
    constexpr int nebulaAnimMaxFrame = 8;
    constexpr float nebulaFramePad = 50.0;

    const int sizeOfNebulaeColours = 3;
    const Color nebulaeColours[sizeOfNebulaeColours]{WHITE, RED, BLUE};

    const int sizeOfNebulae = 6;
    EntityData nebulae[sizeOfNebulae];
    for (int i = 0; i < sizeOfNebulae; i++) {
        nebulae[i] = {
            .rec = {
                .width = nebulaWidth,
                .height = nebulaHeight,
                .x = 0.0f,
                .y = 0.0f
            },
            .pos = {
                .x = windowDimensions[0] * (i + 1.0f),
                .y = windowDimensions[1] - nebulaHeight
            },
            .frame = 0,
            .updateTime = 1.0 / 16.0,
            .runningTime = 0.0f,
            .tint = nebulaeColours[i % sizeOfNebulaeColours]
        };
    }

    // Finish Line
    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // Game Loop
    while (!WindowShouldClose()) {
        ///////////
        // SETUP //
        ///////////

        // delta time (time since last frame)
        const float DT{GetFrameTime()};

        BeginDrawing();
        ClearBackground(RAYWHITE);

        ///////////
        // Logic //
        ///////////

        // ground check
        if (isOnGround(scarfyData, windowDimensions[1])) {
            // rect on ground
            isPlayerGrounded = true;
            playerVelocity = 0;
        } else {
            // rect in air
            isPlayerGrounded = false;
            playerVelocity += GRAVITY * DT;
        }

        // add velocity on "jump" (order matters)
        if (isPlayerGrounded && IsKeyDown(KEY_SPACE)) {
            playerVelocity += jumpVelocity;
        }

        // update scarfy position
        scarfyData.pos.y += playerVelocity * DT;

        // update nebula positions and check player collision
        for (auto &nebulaData: nebulae) {
            nebulaData.pos.x -= nebulaVelocity * DT;

            // Generate nebula rec at actual position
            Rectangle nebRec{
                nebulaData.pos.x + nebulaFramePad,
                nebulaData.pos.y + nebulaFramePad,
                nebulaData.rec.width - (nebulaFramePad * 2),
                nebulaData.rec.height - (nebulaFramePad * 2),
            };

            // generate player rect at actual position
            Rectangle playerRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height,
            };

            if (CheckCollisionRecs(playerRec, nebRec)) {
                playerCollided = true;
            }
        }

        // update finish line
        finishLine -= nebulaVelocity * DT;

        // check player win (crossed finish line)
        if (scarfyData.pos.x >= finishLine) {
            playerWon = true;
        }

        ///////////////
        // Animation //
        ///////////////

        // only update player animation if on the ground
        if (isPlayerGrounded) {
            animateEntity(scarfyData, scaryAnimMaxFrame, DT);
        }

        // animate nebulae
        for (auto &nebulaData: nebulae) {
            animateEntity(nebulaData, nebulaAnimMaxFrame, DT);
        }

        // scroll background layers
        bgX = moveBkgLayer(bgX, 20.0, backgroundTexture, 2, DT);
        mgX = moveBkgLayer(mgX, 40.0, midgroundTexture, 2, DT);
        fgX = moveBkgLayer(fgX, 80.0, foregroundTexture, 2, DT);

        ////////////
        // Render //
        ////////////

        // Draw Background Layers
        drawBkgLayer(bgX, 2, backgroundTexture);
        drawBkgLayer(mgX, 2, midgroundTexture);
        drawBkgLayer(fgX, 2, foregroundTexture);

        if (playerCollided) {
            DrawText("Game Over!", windowDimensions[0] / 3 - 20, windowDimensions[1] / 2 - 20, 40.0, RED);
        } else {
            // Draw Entities
            DrawTextureRec(scarfyTexture, scarfyData.rec, scarfyData.pos, scarfyData.tint);

            for (auto &nebulaData: nebulae) {
                DrawTextureRec(nebulaTexture, nebulaData.rec, nebulaData.pos, nebulaData.tint);
            }
        }

        if (playerWon) {
            DrawText("You Win!", windowDimensions[0] / 3 + 10, windowDimensions[1] / 2 - 20, 40.0, RED);
        }

        EndDrawing();
    }

    // Unload Resources
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);
    UnloadTexture(backgroundTexture);
    UnloadTexture(midgroundTexture);
    UnloadTexture(foregroundTexture);

    CloseWindow();

    return 0;
}
