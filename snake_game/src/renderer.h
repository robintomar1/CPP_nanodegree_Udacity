#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  int RenderStartMenu(std::string str);
  int RenderHighScores();
  void UpdateWindowTitle(int score, int fps);
  void MakeCircle(SDL_Renderer * renderer, int centreX, int centreY, int radius);
  void setScores(int one,int two,int three)
  {
    score_1=one;
    score_2=two;
    score_3=three;
  }

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  TTF_Font *sdl_font;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  int score_1;
  int score_2;
  int score_3;
};

#endif