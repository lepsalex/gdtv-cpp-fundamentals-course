#include "raylib.h"

int main(void) {
    // Window
    constexpr int window_width{800};
    constexpr int window_height{450};

    InitWindow(window_width, window_height, "Dapper Dasher");
    SetTargetFPS(60);

    // Load Resources
    const Texture2D scarfy = LoadTexture("resources/scarfy.png");
    Rectangle scarfy_rec{
        .width = static_cast<float>(scarfy.width) / 6,
        .height = static_cast<float>(scarfy.height),
        .x = 0.0f,
        .y = 0.0f
    };

    // World Constants
    constexpr int GRAVITY{1'600}; // (pixels/s)/s

    // Player Properties
    constexpr int jump_velocity{-800}; // pixels/s
    constexpr float anim_update_time{1.0 / 12.0}; // 12-frames/s

    // Player State
    Vector2 scarfy_pos{
        .x = (window_width / 2) - (scarfy_rec.width / 2),
        .y = (window_height - scarfy_rec.height)
    };
    float velocity{0.0f};
    bool grounded{true};
    float anim_running_time{0.0};
    int anim_frame{};

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
        if (scarfy_pos.y + scarfy_rec.height >= window_height) {
            // rect on ground
            grounded = true;
            velocity = 0;
        } else {
            // rect in air
            grounded = false;
            velocity += GRAVITY * DT;
        }

        // add velocity on "jump" (order matters)
        if (grounded && IsKeyDown(KEY_SPACE)) {
            velocity += jump_velocity;
        }

        // update scarfy position
        scarfy_pos.y += velocity * DT;

        ////////////
        // Render //
        ////////////

        // Update Animation
        anim_running_time += DT;
        if (anim_running_time >= anim_update_time) {
            // reset anim timer
            anim_running_time = 0.0f;

            // update frame
            anim_frame = (anim_frame + 1) % 6;

            // update texture position in rect
            scarfy_rec.x = anim_frame * scarfy_rec.width;
        }

        // Draw
        DrawTextureRec(scarfy, scarfy_rec, scarfy_pos, WHITE);

        EndDrawing();
    }

    // Unload Resources
    UnloadTexture(scarfy);

    CloseWindow();

    return 0;
}
