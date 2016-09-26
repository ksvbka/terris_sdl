#include "Block.h"
#include "Board.h"

Block* Block::instance_ = nullptr;

Block::Block(SDL_Renderer* renderer) : Entity(renderer) {
    /*Load texture*/
    SDL_Surface* img = IMG_Load("img/block.png");
    block_texture_ = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

}

Block::~Block() {
    SDL_DestroyTexture(block_texture_);
}

Block* Block::getInstance(SDL_Renderer* renderer) {
    if(instance_ == nullptr)
        instance_ = new Block(renderer);
    return instance_;
}

void Block::render(Point p) {
    SDL_Rect dstrect;
    dstrect.x = p.col * BLOCK_WIDTH  + BOARD_OFFSET;
    dstrect.y = BOARD_HEIGHT - ((p.row +1) * BLOCK_HEIGHT + BOARD_OFFSET);
    dstrect.w = BLOCK_WIDTH;
    dstrect.h = BLOCK_HEIGHT;
    SDL_RenderCopy(renderer_, block_texture_, 0, &dstrect);
}
