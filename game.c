#include "./raylib/raylib.h"
#include "stdio.h"

#define SPEED 100

#define WIDTH  800
#define HEIGHT 600
#define LEVEL_WIDTH  16
#define LEVEL_HEIGHT 16
#define TILE_SIZE 50
#define TEXTBOX_HEIGHT 200
#define TEXTBOX_PADDING 20
#define FONTSIZE 15
#define SPACING 5

#define TILES 2
#define NPCS 8

static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);   // Draw text using font inside rectangle limits
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);    // Draw text using font inside rectangle limits with support for text selection

Texture2D tiles[TILES + 1];
Texture2D sprites[NPCS];
Texture2D player;

Sound walking;
float timeSincePlayed = 0;

int world[LEVEL_WIDTH * LEVEL_HEIGHT] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 2, 2, 2, 0, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 2, 2, 2, 0, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0, 0, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
  0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
  0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 2, 2, 2, 2, 2, 0, 2, 2, 1, 1, 1, 1, 1, 1, 0,
  0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 1, 1, 0,
  0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 0, 2, 2, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 0, 2, 2, 0, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 0, 2, 2, 0, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int npc_locations[NPCS * 2] = {
  0, 0, 2, 1, 4, 2, 6, 3, 8, 4, 10, 5, 2, 6, 4, 7,
};

Color player_color = {
  .r = 0xFF,
  .g = 0,
  .b = 0,
  .a = 0xFF,
};

char* text[NPCS] = {
  // The Colonel / Player's dialogue
  "Colonel (You) Diary Entry:\n\
  The guests arrived at around 8:30 pm and the Cook served us dinner. All of the guests were there for dinner at the same time, and we finished at around 10 pm. Me, the Poet, the Merchant, and the Old Man went back to the living room, the Butler gave us some drinks from upstairs and cleaned some rooms, and I don't know where everyone else went after that.",
  // The Merchant's dialogue
  "Merchant:\n\
  Dinner started at 8:30 pm soon after I arrived. The Cook served us dinner and we finished at 10 pm. I went back to the living room with the Host and some of the other guests. After an hour or so, the Host went upstairs and came back with some drinks while the Butler went to clean some rooms. Soon before the Butler discovered the body, the Professor excused himself to use the bathroom.", 
  // The Butler's dialogue
  "Butler:\n\
  I was cleaning and setting up the living room before the guests arrived at 8:30 pm when we had dinner. After dinner, I was walking and chatting with the Professor. He excused himself and went upstairs to the bathroom, and I went upstairs as well to clean some rooms up there. I opened a closet, and I saw the son, slumped over and with a gash in his chest. I ran downstairs to tell everyone that the son was in a closet with a stab wound.",
  // The Old Man's dialogue
  "Old Man:\n\
  I got here at around 8:30 pm around the same time as the other guests. We soon had dinner and finished at around 10 pm. I went to the living room with the Host, Merchant, and Poet. The Host got a knife for the snacks and went upstairs. After some time, he returned with some drinks. After some more talking, he asked me to return the knife from earlier to the kitchen, which I did. At around 11 pm, the Professor excused himself to go to the bathroom upstairs, and the Butler also went upstairs to clean some rooms. A few minutes later, the Butler ran downstairs, shaken, telling everyone that the Professor's son was stabbed in a closet.",
  // The Professor's dialogue
  "Professor:\n\
  Oh my son, my son! Whyyyyy? He's only 7! We were the first guests here. The Host showed him the playroom upstairs, where he spent the whole night. I was downstairs during and after dinner, mostly talking with the Butler near the stairs. I went upstairs to the bathroom, and when I came down I was told my son was stabbed! I should have never come here with my son! Why would the colonel invite me to his party anyway? He hates me!",
  // The Cook's dialogue
  "Cook:\n\
  I started preparing the meal two hours before the guests arrived at 8:30. I served courses for dinner for an hour and a half and cleaned dishes throughout dinner and after until 10:30 pm. I served some snacks for the guests afterward, and the Host asked for a knife for the snacks. The Old Man returned it to me at around 11 pm as I was cleaning dishes from the snacks.",
  // The Poet's dialogue
  "Poet:\n\
  We all ate dinner at around 8:30 pm soon after we all arrived. We finished at around 10 pm and I went to the living room with some of the other guests and the Host. The Cook served us snacks and the Host got a knife to cut them into slices for us. He went upstairs to get some drinks for us. After some time, the Old Man returned the knife to the kitchen. After some more chatting, the Professor excused himself to go to the bathroom upstairs and the Butler also went to clean some rooms upstairs. After a few minutes, he ran downstairs and shouted that the Professor's son was stabbed in a closet.",
  "*Ded LOL*",
};

float abs(float a) {
  if (a > 0) return a;
  return -1 * a;
}

void draw_world(Vector2* pos) {
  for (int i = 0; i < LEVEL_WIDTH * LEVEL_HEIGHT; i++) {
    int tile = world[i];

    if (tile == 0) continue;

    Texture2D tex = tiles[tile];

    DrawTexture(tex,
                (int)(i % LEVEL_WIDTH) * TILE_SIZE - pos->x - 25 + WIDTH / 2,
                (int)(i / LEVEL_WIDTH) * TILE_SIZE - pos->y + HEIGHT / 2,
                WHITE);
  }
}

void draw_npcs(Vector2* pos) {
  for (int i = 0; i < NPCS; i++) {
    Texture npc = sprites[i];
    int x = npc_locations[2 * i];
    int y = npc_locations[2 * i + 1];

    if (i == 0) {
      DrawTexture(npc,
                  x * TILE_SIZE - pos->x - 25 + WIDTH / 2,
                  y * TILE_SIZE - pos->y + HEIGHT / 2,
                  WHITE);
      continue;
    }

    DrawTexture(npc,
                x * TILE_SIZE - pos->x - 50 + WIDTH / 2,
                y * TILE_SIZE - pos->y - 50 + HEIGHT / 2,
                WHITE);
  }
}

void deal_with_player(Vector2* pos, float delta) {
  float sped = SPEED * delta;

  int w = IsKeyDown(KEY_W);
  int s = IsKeyDown(KEY_S);
  int a = IsKeyDown(KEY_A);
  int d = IsKeyDown(KEY_D);

  if ((w || s || a || d) && !IsSoundPlaying(walking) && timeSincePlayed >= .25) {
    PlaySound(walking);
    timeSincePlayed = 0;
  }

  if (w) pos->y -= sped;
  if (s) pos->y += sped;
  if (a) pos->x -= sped;
  if (d) pos->x += sped;

  if (pos->x <= 0) pos->x = 0;
  if (pos->x >= (LEVEL_WIDTH - 1) * TILE_SIZE) pos->x = (LEVEL_WIDTH - 1) * TILE_SIZE;
  if (pos->y <= 0) pos->y = 0;
  if (pos->y >= (LEVEL_HEIGHT - 1) * TILE_SIZE) pos->y = (LEVEL_HEIGHT - 1) * TILE_SIZE;

  // DrawRectangle(WIDTH / 2 - 50, HEIGHT / 2 - 50, 100, 100, player_color);
  DrawTexture(player, WIDTH / 2 - 50, HEIGHT / 2 - 50, WHITE);
}

void gui(int talking) {
  char* click_text = "[Left Click] to interact";
  DrawText(click_text, WIDTH / 2 - MeasureText(click_text, 20) / 2, 0, 20, WHITE);

  if (talking < 0) return;

  DrawRectangle(0, HEIGHT - TEXTBOX_HEIGHT, WIDTH, TEXTBOX_HEIGHT, BLACK);
  // DrawText(text[talking], TEXTBOX_PADDING, HEIGHT - TEXTBOX_HEIGHT + TEXTBOX_PADDING, FONTSIZE, WHITE);
  DrawTextBoxed(GetFontDefault(), text[talking], (Rectangle){ TEXTBOX_PADDING, HEIGHT - TEXTBOX_HEIGHT + TEXTBOX_PADDING, WIDTH - 2 * TEXTBOX_PADDING, TEXTBOX_HEIGHT - 2 * TEXTBOX_PADDING }, FONTSIZE, SPACING, 1, WHITE);
}

void interact(int* talking, Vector2* pos) {
  if (*talking != -1) {
    *talking = -1;
    return;
  }

  for (int i = 0; i < NPCS; i++) {
    int x = npc_locations[2 * i];
    int y = npc_locations[2 * i + 1];

    //printf("%f, %f, %d, %d, %d\n", pos->x, pos->y, x, y, i);

    if (abs(x - pos->x / TILE_SIZE) <= 1 && abs(y - pos->y / TILE_SIZE) <= 1) {
      *talking = i;
      return;
    }
  }
}

int main() {
  InitWindow(WIDTH, HEIGHT, "Game!");
  InitAudioDevice();

  walking = LoadSound("Waulcking_Sownde_Iffeckt.wav");

  Image icon = LoadImage("NM-Icon.png");
  SetWindowIcon(icon);

  Color bkg_color = {
    .r = 0,
    .b = 0,
    .g = 0,
    .a = 0xFF,
  };

  Vector2 pos = { .x = 0.0, .y = 0.0, };

  Image tile1 = LoadImage("tile1.png");
  ImageResizeNN(&tile1, TILE_SIZE, TILE_SIZE);
  Image tile2 = LoadImage("tile2.png");
  ImageResizeNN(&tile2, TILE_SIZE, TILE_SIZE);

  tiles[1] = LoadTextureFromImage(tile1);
  tiles[2] = LoadTextureFromImage(tile2);

  Image book = LoadImage("diary.png");
  ImageResize(&book, TILE_SIZE, TILE_SIZE);

  sprites[0] = LoadTextureFromImage(book);
  sprites[2] = LoadTexture("butler.png");
  sprites[5] = LoadTexture("Chef.png");
  sprites[4] = LoadTexture("professor.png");
  sprites[1] = LoadTexture("merchant.png");
  sprites[3] = LoadTexture("old_man.png");
  sprites[6] = LoadTexture("Poet.png");
  sprites[7] = LoadTexture("Son.png");

  player = LoadTexture("Colonel.png");

  int talking = -1;

  while (!WindowShouldClose()) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) interact(&talking, &pos);
    BeginDrawing();
    ClearBackground(bkg_color);
    float delta = GetFrameTime();

    timeSincePlayed += delta;

    draw_world(&pos);
    draw_npcs(&pos);

    deal_with_player(&pos, delta);

    gui(talking);

    EndDrawing();
  }

  CloseAudioDevice();
  CloseWindow();
  return 0;
}

static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
{
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0;          // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int state = wordWrap? MEASURE_STATE : DRAW_STATE;

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width*scaleFactor : font.glyphs[index].advanceX*scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container).
        // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
        // and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1)? i : endLine;
                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n') state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize*scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec((Rectangle){ rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth, (float)font.baseSize*scaleFactor }, selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font, codepoint, (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY }, fontSize, isGlyphSelected? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                state = !state;
            }
        }

        if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
    }
}
