#include "raylib.h"

int main(void) {
    // window specs
    static constexpr int screen_width = 800;
    static constexpr int screen_height = 450;
    static const char *window_title = "Axe Game";

    // circle
    int circle_x = screen_width / 2 - 200;
    int circle_y = screen_height / 2;
    int circle_r = 25;

    // circle edges
    int l_circle_x = circle_x - circle_r;
    int r_circle_x = circle_x + circle_r;
    int u_circle_y = circle_y - circle_r;
    int b_circle_y = circle_y + circle_r;

    // axe
    int axe_x = 400;
    int axe_y = 0;
    int axe_length = 50;

    // axe edges
    int l_axe_x = axe_x;
    int r_axe_x = axe_x + axe_length;
    int u_axe_y = axe_y;
    int b_axe_y = axe_y + axe_length;
    int direction = 10;

    // collision
    bool collision_with_axe = false;

    InitWindow(screen_width, screen_height, window_title);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        // Game logic begins
        if (collision_with_axe) {
            DrawText("Game Over!", 400, 200, 20, RED);
        } else {
            // Update collider edges
            l_circle_x = circle_x - circle_r;
            r_circle_x = circle_x + circle_r;
            u_circle_y = circle_y - circle_r;
            b_circle_y = circle_y + circle_r;
            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_length;
            u_axe_y = axe_y;
            b_axe_y = axe_y + axe_length;

            // Detect collision with axe
            collision_with_axe = (b_axe_y >= u_circle_y
                                  && u_axe_y <= b_circle_y
                                  && l_axe_x <= r_circle_x
                                  && r_axe_x >= l_circle_x);

            DrawCircle(circle_x, circle_y, circle_r, BLUE);
            DrawRectangle(axe_x, axe_y, axe_length, axe_length, RED);

            // Move Axe
            axe_y += direction;
            if (axe_y + axe_length >= screen_height || axe_y <= 0) {
                direction *= -1;
            }

            // Input
            if (IsKeyDown(KEY_D) && circle_x + circle_r < screen_width)
                circle_x += 10;
            if (IsKeyDown(KEY_A) && circle_x - circle_r > 0)
                circle_x -= 10;
        }
        // Game logic ends

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
