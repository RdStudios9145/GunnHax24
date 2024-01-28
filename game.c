#include "./raylib/raylib.h"

#define SPEED 100

Color player_color = {
  .r = 0xFF,
  .g = 0,
  .b = 0,
  .a = 0xFF,
};

void deal_with_player(Vector2* pos, float delta) {
  float sped = SPEED * delta;

  if (IsKeyDown(KEY_W)) pos->y -= sped;
  if (IsKeyDown(KEY_S)) pos->y += sped;
  if (IsKeyDown(KEY_A)) pos->x -= sped;
  if (IsKeyDown(KEY_D)) pos->x += sped;

  DrawRectangle(pos->x, pos->y, 100, 100, player_color);
}

int main() {
  InitWindow(800, 600, "Game!");
  Color bkg_color = {
    .r = 0,
    .b = 0,
    .g = 0,
    .a = 0xFF,
  };

  Vector2 pos = { .x = 0.0, .y = 0.0, };

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bkg_color);
    float delta = GetFrameTime();

    deal_with_player(&pos, delta);

    EndDrawing();
  }

  return 0;
}
