#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Texture {
  public:
    Texture(SDL_Renderer* renderer)
        : texture_{nullptr}, renderer_{renderer}, width_{}, height_{}
    {}

    ~Texture() { free();}

    bool loadFromFile(std::string path);

    void free();
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void setAlpha( Uint8 alpha );

    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    int width() { return width_;}

    int height() { return height_;}

    //Creates image from font string
    bool loadFromRenderedText(TTF_Font* font, std::string textureText, SDL_Color textColor );

  private:
    SDL_Texture*  texture_;
    SDL_Renderer* renderer_;
    int     width_;
    int     height_;
};

#endif
