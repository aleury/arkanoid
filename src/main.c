#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>

#define BG_COLOR \
    (Color) { 21, 21, 21, 255 }

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20

#define BALL_WIDTH 20
#define BALL_HEIGHT 20

int main() {
    // Initialization
    // ---------------------------------------------------------------
    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "Arkanoid");
    SetTargetFPS(60);

    // Setup and initialize our game variables
    // ---------------------------------------------------------------
    float paddle_speed = 400;
    Rectangle paddle = {(float)screen_width / 2, (float)screen_height - 40, PADDLE_WIDTH, PADDLE_HEIGHT};

    float ball_speed = 400;
    Vector2 ball_dir = {0, 1};
    Rectangle ball = {(float)screen_width / 2, (float)screen_height / 2, BALL_WIDTH, BALL_HEIGHT};

    size_t brick_rows = 6;
    size_t brick_cols = 8;
    size_t num_bricks = brick_rows * brick_cols;
    Rectangle bricks[num_bricks];
    bool bricks_alive[num_bricks];
    Color brick_colors[6] = {BLUE, YELLOW, ORANGE, RED, GREEN, PURPLE};
    for (size_t y = 0; y < brick_rows; y++) {
        for (size_t x = 0; x < brick_cols; x++) {
            size_t i = (y * brick_cols) + x;
            bricks[i].x = x * 100;
            bricks[i].y = y * 20;
            bricks[i].width = 100;
            bricks[i].height = 20;
            bricks_alive[i] = true;
        }
    }

    // Main game loop
    // ---------------------------------------------------------------
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_LEFT)) {
            paddle.x -= paddle_speed * dt;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            paddle.x += paddle_speed * dt;
        }
        paddle.x = Clamp(paddle.x, 0, screen_width - PADDLE_WIDTH);

        // Update the ball
        ball.x += ball_speed * ball_dir.x * dt;
        ball.y += ball_speed * ball_dir.y * dt;

        // Check collision with the walls
        if (ball.x <= 0 || ball.x >= screen_width - ball.width) {
            ball_dir.x *= -1;
        }
        if (ball.y <= 0) {
            ball_dir.y *= -1;
        }
        if (ball.y >= screen_height) {
            ball = (Rectangle){(float)screen_width / 2, (float)screen_height / 2, BALL_WIDTH, BALL_HEIGHT};
            ball_dir = (Vector2){0, 1};
        }

        // Check collision with the paddle
        if (CheckCollisionRecs(paddle, ball)) {
            Vector2 paddle_center = {paddle.x + paddle.width / 2, paddle.y + paddle.height / 2};
            Vector2 ball_center = {ball.x + ball.width / 2, ball.y + ball.height / 2};
            ball_dir = Vector2Normalize(Vector2Subtract(ball_center, paddle_center));
        }

        // Check collision with the bricks
        for (size_t i = 0; i < num_bricks; i++) {
            if (bricks_alive[i] && CheckCollisionRecs(bricks[i], ball)) {
                bricks_alive[i] = false;
                Vector2 brick_center = {bricks[i].x + bricks[i].width / 2, bricks[i].y + bricks[i].height / 2};
                Vector2 ball_center = {ball.x + ball.width / 2, ball.y + ball.height / 2};
                ball_dir = Vector2Normalize(Vector2Subtract(ball_center, brick_center));
            }
        }

        BeginDrawing();
        {
            ClearBackground(BG_COLOR);
            DrawRectangleRec(paddle, WHITE);
            DrawRectangleRec(ball, RED);
            for (size_t i = 0; i < num_bricks; i++) {
                if (bricks_alive[i]) {
                    DrawRectangleRec(bricks[i], brick_colors[i % 6]);
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
