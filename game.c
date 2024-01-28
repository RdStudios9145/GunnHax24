#include "./raylib/raylib.h"

#define SPEED 100

#define WIDTH  800
#define HEIGHT 600
#define LEVEL_WIDTH  10
#define LEVEL_HEIGHT 10
#define TILE_SIZE 25
#define TEXTBOX_HEIGHT 100
#define TEXTBOX_PADDING 20
#define FONTSIZE 20

#define TILES 2
#define TEXT 1

Texture2D tiles[TILES + 1];

int world[LEVEL_WIDTH * LEVEL_HEIGHT] = {
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
};

Color player_color = {
  .r = 0xFF,
  .g = 0,
  .b = 0,
  .a = 0xFF,
};

char* text[TEXT + 1] = {
  "You Should Never See This",
  "Test"
};

void draw_world(Vector2* pos) {
  for (int i = 0; i < LEVEL_WIDTH * LEVEL_HEIGHT; i++) {
    int tile = world[i];

    if (tile == 0) continue;

    Texture2D tex = tiles[tile];

    DrawTexture(tex, (int)(i % LEVEL_WIDTH) * TILE_SIZE, (int)(i / LEVEL_WIDTH) * TILE_SIZE, WHITE);
  }
}

void deal_with_player(Vector2* pos, float delta) {
  float sped = SPEED * delta;

  if (IsKeyDown(KEY_W)) pos->y -= sped;
  if (IsKeyDown(KEY_S)) pos->y += sped;
  if (IsKeyDown(KEY_A)) pos->x -= sped;
  if (IsKeyDown(KEY_D)) pos->x += sped;

  DrawRectangle(pos->x, pos->y, 100, 100, player_color);
}

void gui(int talking) {
  if (talking == 0) return;

  DrawRectangle(0, HEIGHT - TEXTBOX_HEIGHT, WIDTH, TEXTBOX_HEIGHT, BLACK);
  DrawText(text[talking], TEXTBOX_PADDING, HEIGHT - TEXTBOX_HEIGHT + TEXTBOX_PADDING, FONTSIZE, WHITE);
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

  int talking = 1;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bkg_color);
    float delta = GetFrameTime();

    draw_world(&pos);

    deal_with_player(&pos, delta);

    gui(talking);

    EndDrawing();
  }

  return 0;
}
