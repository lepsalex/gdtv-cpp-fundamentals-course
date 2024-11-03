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


    const int sizeOfNebulaeColours = 3;
    const Color nebulaeColours[sizeOfNebulaeColours]{WHITE, RED, GREEN};

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

        // update nebula positions
        for (auto &nebulaData: nebulae) {
            nebulaData.pos.x -= nebulaVelocity * DT;
        }

        ////////////
        // Render //
        ////////////

        // Update Animations

        // only update player animation if on the ground
        if (isPlayerGrounded) {
            animateEntity(scarfyData, scaryAnimMaxFrame, DT);
        }

        for (auto &nebulaData: nebulae) {
            animateEntity(nebulaData, nebulaAnimMaxFrame, DT);
        }

        // Draw
        DrawTextureRec(scarfyTexture, scarfyData.rec, scarfyData.pos, scarfyData.tint);

        for (auto &nebulaData: nebulae) {
            DrawTextureRec(nebulaTexture, nebulaData.rec, nebulaData.pos, nebulaData.tint);
        }

        EndDrawing();
    }

    // Unload Resources
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);

    CloseWindow();

    return 0;
}
