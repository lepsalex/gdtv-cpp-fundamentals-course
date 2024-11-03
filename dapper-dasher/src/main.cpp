#include "raylib.h"

int main(void) {
    // window
    static constexpr int window_width{800};
    static constexpr int window_height{450};

    InitWindow(window_width, window_height, "Dapper Dasher");
    SetTargetFPS(60);

    static constexpr int gravity{1}; // (pixels/frame)/frame

    // rect dimensions
    const int width{50};
    const int height{80};

    // rect config
    int velocity{0};
    constexpr int jump_velocity{-22};

    // rect state
    int pos_y{window_height - height};
    bool grounded{true};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ground check
        if (pos_y + height >= window_height) {
            // rect on ground
            grounded = true;
            velocity = 0;
        } else {
            // rect in air
            grounded = false;
            velocity += gravity;
        }

        // add velocity on "jump" (order matters)
        if (grounded && IsKeyDown(KEY_SPACE)) {
            velocity += jump_velocity;
        }

        // update rect position
        pos_y += velocity;

        DrawRectangle(window_width / 2, pos_y, width, height, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
