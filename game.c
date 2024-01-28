#include "./raylib/raylib.h"

int main() {
  InitWindow(800, 600, "Game!");
  Color bkg_color = {
    .r = 0,
    .b = 0,
    .g = 0,
    .a = 0xFF,
  };

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bkg_color);
    EndDrawing();
  }

  return 0;
}
