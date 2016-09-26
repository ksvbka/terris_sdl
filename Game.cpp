#include "Game.h"
#include <stdexcept>
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <algorithm>

Game::Game() {
    window_     = nullptr;
    renderer_   = nullptr;
    quit_       = false;
    font_       = nullptr;
    scoreTexture_ = nullptr;
    score_ = 0;
    moved_ = false;

}

Game::~Game() {
    clean();
}

void Game::init() {
    /* Init SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0 )
        throw std::runtime_error("Can not init SDL!");

    //Set texture filtering to linear
    if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        throw std::runtime_error("Warning: Linear texture filtering not enabled!");

    /* Init Window*/
    window_ = SDL_CreateWindow("SDL Terris",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN
                              );
    if (!window_)
        throw std::runtime_error("Can not create window");

    /* Create render for window*/
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_)
        throw std::runtime_error("Can not create renderer!");
    SDL_SetRenderDrawColor( renderer_, 0, 0, 0, 0xFF );

    /* Initialize PNG loading*/
    int imgFlags = IMG_INIT_PNG;
    if (!IMG_Init(imgFlags) & imgFlags)
        throw std::runtime_error("SDL_image could not initialize!");

    /* Initialize SDL_ttf */
    if ( TTF_Init() == -1 )
        throw std::runtime_error("SDL_ttf could not initialize!");

    font_ = TTF_OpenFont( "img/JOYSTIX.TTF", 25 );
    if ( font_ == nullptr )
        throw std::runtime_error("Failed to load JOYSTIX font");

    scoreTexture_ = new Texture(renderer_);
}

void Game::clean() {

    delete scoreTexture_;

    // Clean renderer and window
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);


    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}


void Game::display(Piece * piece, Point pos) {
    for (auto block : piece->getBody())
        Block::getInstance(renderer_)->render(block + pos);
    SDL_RenderPresent( renderer_ );
}

void Game::newGame() {
    board_->clear();
    score_ = 0;
}

void Game::getNewPiece() {
    /*Get new piece and reset curr_piece_pos_*/

    if (current_piece_)
        delete current_piece_;
    current_piece_ = next_piece_;
    srand(time(0));
    next_piece_ = new Piece(rand());

    /* Start point */
    curr_piece_pos_ = Point(BOARD_ROW_ORG, BOARD_COL_ORG / 2 );

    down_time_ = DOWN_TIME;
    moved_ = false;
}

void Game::handleEvent() {
    while ( SDL_PollEvent( &event_ ) != 0 ) {
        //User requests quit
        if ( event_.type == SDL_QUIT ) {
            quit_ = true;
            return;
        }

        if ( event_.type == SDL_KEYDOWN ) {

            /* Update state or position of piece shape*/
            switch (event_.key.keysym.sym) {
            case SDLK_LEFT:
                curr_piece_pos_.col = std::max(curr_piece_pos_.col - 1, 0);
                //curr_piece_pos_.col--;
                moved_ = true;
                break;
            case SDLK_RIGHT:
                //curr_piece_pos_.col++;
                curr_piece_pos_.col = std::min(curr_piece_pos_.col + 1, board_->getColumn() - current_piece_->getWidth() - 1);
                moved_ = true;
                break;
            case SDLK_UP:
                current_piece_->nextRotation();
                moved_ = true;
                break;
            case SDLK_DOWN:
                down_time_ = FALLING_TIME;
                break;
            }
        }

    }
}


extern std::vector<PieceShape> gPicesLib;

void Game::run() {

    /*Create Item*/
    board_ = new Board(renderer_, BOARD_ROW, BOARD_COL);
    srand(time(0));
    current_piece_ = new Piece(rand());
    next_piece_    = new Piece(rand());

    /* Start point */
    curr_piece_pos_ = Point(BOARD_ROW_ORG, BOARD_COL_ORG / 2 );

    down_time_ = DOWN_TIME;

    /*Main loop*/
    Uint32 frame_time_start = SDL_GetTicks();

    getNewPiece();

    int status;
    Point last_piece_pos_;

    while (!quit_) {

        if (board_->getMaxHeight() >= board_->getRow() - BOARD_INVISIBLE_SIZE)
            newGame();

        /* Handle input command and set new position*/
        handleEvent();

        if (curr_piece_pos_ != last_piece_pos_) {

            status = board_->place(current_piece_, curr_piece_pos_);

            if (status == Board::PLACE_OK) {
                render();
                board_->undo();
            }
            bool failed = (status >= Board::PLACE_OUT_BOUNDS);
            if (failed) {
                std::cout << "Place failed" << std::endl;
                board_->undo();
                board_->place(current_piece_, last_piece_pos_);
            }

            if (failed && !moved_) {
                std::cout << "Place failed and not moved_" << std::endl;
                board_->commit();
                score_ += board_->clearRow();
                SDL_RenderClear( renderer_ );
                getNewPiece();
            }
        }

        // score_ += board_->clearRow();
        SDL_Color textColor = { 0xff, 0xff, 0xff };
        scoreTexture_->loadFromRenderedText(font_, std::to_string(score_), textColor);

        /* Let piece go down*/
        Uint32 frame_time_count = SDL_GetTicks();
        if (frame_time_count - frame_time_start > down_time_) {
            last_piece_pos_ = curr_piece_pos_;
            --curr_piece_pos_.row;
            frame_time_start = SDL_GetTicks();
            moved_ = false;
        }

    }

    delete board_;
    delete current_piece_;
    delete next_piece_;
}


void Game::render() {
    //Clear screen

    // SDL_RenderClear( renderer_ );

    //Render item's texture to screen
    board_->render();

    display(next_piece_, Point(NEXT_PIECE_POS_ROW, NEXT_PIECE_POS_COL));

    scoreTexture_->render(SCORE_POS_ROW, SCORE_POS_COL);

    //Update screen
    SDL_RenderPresent( renderer_ );
}
