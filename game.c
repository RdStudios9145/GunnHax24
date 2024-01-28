#include "./raylib/raylib.h"

#define SPEED 100

#define WIDTH  800
#define HEIGHT 600
#define LEVEL_WIDTH  10
#define LEVEL_HEIGHT 10
#define TILE_SIZE 25
#define TEXTBOX_HEIGHT 200
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
  // The Colonel / Player's dialogue
  "The guests arrived at around 8:30 pm and the Cook served us dinner. All of the guests were there for dinner at the same time, and we finished at around 10 pm. Me, the Poet, the Merchant, and the Old Man went back to the living room, the Butler gave us some drinks from upstairs and cleaned some rooms, and I don't know where everyone else went after that.",
  // The Merchant's dialogue
  "Dinner started at 8:30 pm soon after I arrived. The Cook served us dinner and we finished at 10 pm. I went back to the living room with the Host and some of the other guests. After an hour or so, the Host went upstairs and came back with some drinks while the Butler went to clean some rooms. Soon before the Butler discovered the body, the Professor excused himself to use the bathroom.", 
  // The Butler's dialogue
  "I was cleaning and setting up the living room before the guests arrived at 8:30 pm when we had dinner. After dinner, I was walking and chatting with the Professor. He excused himself and went upstairs to the bathroom, and I went upstairs as well to clean some rooms up there. I opened a closet, and I saw the son, slumped over and with a gash in his chest. I ran downstairs to tell everyone that the son was in a closet with a stab wound."
  // The Old Man's dialogue
  "I got here at around 8:30 pm around the same time as the other guests. We soon had dinner and finished at around 10 pm. I went to the living room with the Host, Merchant, and Poet. The Host got a knife for the snacks and went upstairs. After some time, he returned with some drinks. After some more talking, he asked me to return the knife from earlier to the kitchen, which I did. At around 11 pm, the Professor excused himself to go to the bathroom upstairs, and the Butler also went upstairs to clean some rooms. A few minutes later, the Butler ran downstairs, shaken, telling everyone that the Professor's son was stabbed in a closet."
  // The Professor's dialogue
  "Oh my son, my son! Whyyyyy? He's only 7! We were the first guests here. The Host showed him the playroom upstairs, where he spent the whole night. I was downstairs during and after dinner, mostly talking with the Butler near the stairs. I went upstairs to the bathroom, and when I came down I was told my son was stabbed! I should have never come here with my son! Why would the colonel invite me to his party anyway? He hates me!"
  // The Cook's dialogue
  "I started preparing the meal two hours before the guests arrived at 8:30. I served courses for dinner for an hour and a half and cleaned dishes throughout dinner and after until 10:30 pm. I served some snacks for the guests afterward, and the Host asked for a knife for the snacks. The Old Man returned it to me at around 11 pm as I was cleaning dishes from the snacks.",
  // The Poet's dialogue
  "We all ate dinner at around 8:30 pm soon after we all arrived. We finished at around 10 pm and I went to the living room with some of the other guests and the Host. The Cook served us snacks and the Host got a knife to cut them into slices for us. He went upstairs to get some drinks for us. After some time, the Old Man returned the knife to the kitchen. After some more chatting, the Professor excused himself to go to the bathroom upstairs and the Butler also went to clean some rooms upstairs. After a few minutes, he ran downstairs and shouted that the Professor's son was stabbed in a closet."
  
};

void draw_world(Vector2* pos) {
  for (int i = 0; i < LEVEL_WIDTH * LEVEL_HEIGHT; i++) {
    int tile = world[i];

    if (tile == 0) continue;

    Texture2D tex = tiles[tile];

    DrawTexture(tex,
                (int)(i % LEVEL_WIDTH) * TILE_SIZE - pos->x,
                (int)(i / LEVEL_WIDTH) * TILE_SIZE - pos->y,
                WHITE);
  }
}

void deal_with_player(Vector2* pos, float delta) {
  float sped = SPEED * delta;

  if (IsKeyDown(KEY_W)) pos->y -= sped;
  if (IsKeyDown(KEY_S)) pos->y += sped;
  if (IsKeyDown(KEY_A)) pos->x -= sped;
  if (IsKeyDown(KEY_D)) pos->x += sped;

  DrawRectangle(WIDTH / 2 - 50, HEIGHT / 2 - 50, 100, 100, player_color);
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
