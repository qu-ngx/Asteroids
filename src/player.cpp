#include "player.hpp"

void Player::draw_player(Player *player)
{
    Vector2 p1 = player->position;
    Vector2 left = {-10, 10};
    Vector2 right = {10, 10};

    left = Vector2Rotate(left, player->rotation);
    right = Vector2Rotate(right, player->rotation);

    DrawLineV(p1, Vector2Add(left, p1), RED);
    DrawLineV(p1, Vector2Add(right, p1), RED);
    DrawLineV(Vector2Add(left, p1), Vector2Add(right, p1), RED);
}