bool button(static char* text, vector* pos, int padding, int fontsize) {
    int width = MeasureText(text, fontSize);
    DrawRectangleLines(pos->x, pos->y, size->x + 2 * padding + width, size->y + 2 * padding + fontsize, WHITE);
    DrawText(text, pos->x + padding, pos->y + padding, fontSize, WHITE);
    Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, (Rectangle){ pos->x, pos->y, size->x + 2 * padding + width, size->y + 2 * padding + fontsize});
}
