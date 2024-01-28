#include "./raylib/raylib.h"

#define SPEED 100

#define WIDHT  800
#define HEIGHT 600
#define LEVEL_WIDTH  10
#define LEVEL_HEIGHT 10

#define TILES 2

Texture2D[TILES + 1] tiles;

int[LEVEL_WIDTH * LEVEL_HEIGHT] world = [
  1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
  2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
  1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
  2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
  1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
  2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
  1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
  2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
  1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
  2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
];

Color player_color = {
  .r = 0xFF,
  .g = 0,
  .b = 0,
  .a = 0xFF,
};

void draw_world(Vector2* pos) {

}

void deal_with_player(Vector2* pos, float delta) {
  float sped = SPEED * delta;

  if (IsKeyDown(KEY_W)) pos->y -= sped;
  if (IsKeyDown(KEY_S)) pos->y += sped;
  if (IsKeyDown(KEY_A)) pos->x -= sped;
  if (IsKeyDown(KEY_D)) pos->x += sped;

  DrawRectangle(pos->x, pos->y, 100, 100, player_color);
}

int main() {
  InitWindow(WIDTH, HEIGHT, "Game!");
  Color bkg_color = {
    .r = 0,
    .b = 0,
    .g = 0,
    .a = 0xFF,
  };

  Vector2 pos = { .x = 0.0, .y = 0.0, };

  tiles[1] = LoadTexture("tile1.png");
  tiles[2] = LoadTexture("tile2.png");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bkg_color);
    float delta = GetFrameTime();

    draw_world(&pos);

    deal_with_player(&pos, delta);

    EndDrawing();
  }

  return 0;
}
