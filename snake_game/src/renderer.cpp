#include "renderer.h"
#include <iostream>
#include <string>
#include <sstream>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize font
  if (TTF_Init() < 0) {
    std::cerr << "SDL Font could not initialize.\n";
    std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
  }

  // Load font
  sdl_font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf", 50);
  if(nullptr == sdl_font) {
    std::cerr << "SDL Font could not loaded.\n";
    std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  TTF_CloseFont(sdl_font);//close sdlfont
  TTF_Quit();
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::MakeCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Set Grid color to white
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0x00);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  MakeCircle(sdl_renderer, block.x+(block.w/2), block.y+(block.h/2), block.h/2);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    MakeCircle(sdl_renderer, block.x+(block.w/2), block.y+(block.h/2), block.h/2);

  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  MakeCircle(sdl_renderer, block.x+(block.w/2), block.y+(block.h/2), block.h/2);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

int Renderer::RenderHighScores()
{
  Uint32 time;
  int x,y;
  const int number_of_menu_items = 4;
  const char *text_labels[number_of_menu_items];
  std::stringstream stream_one;
  std::stringstream stream_two;
  std::stringstream stream_three; 
  stream_one<<"1.   "<<score_1;
  stream_two<<"2.   "<<score_2;
  stream_three<<"3.   "<<score_3;
  const std::string &tmp_1 = stream_one.str();
  const std::string &tmp_2 = stream_two.str();
  const std::string &tmp_3 = stream_three.str();

  const char *s_1 = tmp_1.c_str();
  const char *s_2 = tmp_2.c_str();
  const char *s_3 = tmp_3.c_str();

  text_labels[0] = "HIGH SCORES";
  text_labels[1] = s_1;
  text_labels[2] = s_2;
  text_labels[3] = s_3;
  SDL_Surface *surface_menus[number_of_menu_items];
  SDL_Texture *texture_menus[number_of_menu_items];
  bool selected[number_of_menu_items] = {0, 0 , 0 ,0};
  SDL_Color White = {255, 255, 255, 0};
  SDL_Color Red = {255, 0, 0, 0};
  SDL_Surface *screen = SDL_GetWindowSurface(sdl_window);
  surface_menus[0] = TTF_RenderText_Solid(sdl_font, text_labels[0], Red);
  surface_menus[1] = TTF_RenderText_Solid(sdl_font, text_labels[1], White);
  surface_menus[2] = TTF_RenderText_Solid(sdl_font, text_labels[2], White);
  surface_menus[3] = TTF_RenderText_Solid(sdl_font, text_labels[3], White);

  texture_menus[0] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[0]);
  texture_menus[1] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[1]);
  texture_menus[2] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[2]);
  texture_menus[3] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[3]);

  SDL_Rect pos[number_of_menu_items];
  pos[0].x = screen->w/2 - surface_menus[0]->w/2;
  pos[0].y = screen->h - 500;// (screen->h)/4; // surface_menus[0]->h;
  pos[0].w = surface_menus[0]->w;
  pos[0].h = surface_menus[0]->h;

  pos[1].x = screen->w/2 - surface_menus[0]->w/2;
  pos[1].y = screen->h -400;
  pos[1].w = surface_menus[1]->w;
  pos[1].h = surface_menus[1]->h;

  pos[2].x = screen->w/2- surface_menus[0]->w/2;
  pos[2].y = screen->h - 300;
  pos[2].w = surface_menus[2]->w;
  pos[2].h = surface_menus[2]->h;

  pos[3].x = screen->w/2- surface_menus[0]->w/2;
  pos[3].y = screen->h -200;
  pos[3].w = surface_menus[3]->w;
  pos[3].h = surface_menus[3]->h;
  
  SDL_Event event;
  while(1)
  {
    time = SDL_GetTicks();
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          for (int i = 0; i < number_of_menu_items; i++) {
            SDL_FreeSurface(surface_menus[i]);
            SDL_DestroyTexture(texture_menus[i]);
          }
          return 0;
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_ESCAPE)
          {
            for (int i = 0; i < number_of_menu_items; i++) {
              SDL_FreeSurface(surface_menus[i]);
              SDL_DestroyTexture(texture_menus[i]);
            }
            return 1;
          }
      }
    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);

    // Render menu items
    for (int i = 0; i < number_of_menu_items; i++) {
      SDL_RenderCopy(sdl_renderer, texture_menus[i], NULL, &pos[i]);
    }

    // Update Screen
    SDL_RenderPresent(sdl_renderer);

    if(1000/30 > (SDL_GetTicks()-time))
      SDL_Delay(1000/30 - (SDL_GetTicks()-time));
  }
  }
}




int Renderer::RenderStartMenu(std::string str) {
  Uint32 time;
  int x, y;
  const int number_of_menu_items = 3;
  const char *text_labels[number_of_menu_items];
  text_labels[0] = str.c_str();
  text_labels[1] = "HIGH SCORES";
  text_labels[2] = "EXIT";
  SDL_Surface *surface_menus[number_of_menu_items];
  SDL_Texture *texture_menus[number_of_menu_items];
  bool selected[number_of_menu_items] = {0, 0,0};
  SDL_Color White = {255, 255, 255, 0};
  SDL_Color Green = {0, 255, 0, 0};
  SDL_Color Red = {255, 0, 0, 0};
  SDL_Color Blue = {0, 0, 255, 0};
  SDL_Surface *screen = SDL_GetWindowSurface(sdl_window);
  surface_menus[0] = TTF_RenderText_Solid(sdl_font, text_labels[0], Red);
  surface_menus[1] = TTF_RenderText_Solid(sdl_font, text_labels[1], Green);
  surface_menus[2] = TTF_RenderText_Solid(sdl_font, text_labels[2], Blue);
  texture_menus[0] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[0]);
  texture_menus[1] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[1]);
  texture_menus[2] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[2]);
  SDL_Rect pos[number_of_menu_items];
  pos[0].x = screen->w/2 - surface_menus[0]->w/2;
  pos[0].y = screen->h/3 - surface_menus[0]->h;
  pos[0].w = surface_menus[0]->w;
  pos[0].h = surface_menus[0]->h;
  pos[1].x = screen->w/2 - surface_menus[1]->w/2;
  pos[1].y = screen->h/3 + surface_menus[1]->h;
  pos[1].w = surface_menus[1]->w;
  pos[1].h = surface_menus[1]->h;
  pos[2].x = screen->w/2 - surface_menus[2]->w/2;
  pos[2].y = screen->h/2 + surface_menus[2]->h;
  pos[2].w = surface_menus[2]->w;
  pos[2].h = surface_menus[2]->h;
  SDL_Event event;
  while(1)
  {
    time = SDL_GetTicks();
    while(SDL_PollEvent(&event))
    {
	 switch(event.type)
      {
		case SDL_QUIT:
          for (int i = 0; i < number_of_menu_items; i++) {
            SDL_FreeSurface(surface_menus[i]);
            SDL_DestroyTexture(texture_menus[i]);
          }
          return 1;
        case SDL_MOUSEMOTION:
          x = event.motion.x;
          y = event.motion.y;
          for(int i = 0; i < number_of_menu_items; i += 1) {
            if(x>=pos[i].x && x<=pos[i].x+pos[i].w && y>=pos[i].y && y<=pos[i].y+pos[i].h)
            {
              if(!selected[i])
              {
                selected[i] = 1;
                SDL_FreeSurface(surface_menus[i]);
                SDL_DestroyTexture(texture_menus[i]);
                surface_menus[i] = TTF_RenderText_Solid(sdl_font, text_labels[i], White);
                texture_menus[i] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[i]);
              }
            }
            else
            {
              if(selected[i])
              {
                selected[i] = 0;
                SDL_FreeSurface(surface_menus[i]);
                SDL_DestroyTexture(texture_menus[i]);
				if(i==0)
				{
                	surface_menus[i] = TTF_RenderText_Solid(sdl_font, text_labels[i], Red);
                }
				else if(i==1)
				{
					surface_menus[i] = TTF_RenderText_Solid(sdl_font, text_labels[i], Green);
				}
				else if(i==2)
				{
					surface_menus[i] = TTF_RenderText_Solid(sdl_font, text_labels[i], Blue);
				}
				
				texture_menus[i] = SDL_CreateTextureFromSurface(sdl_renderer, surface_menus[i]);
              }
            }
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          x = event.button.x;
          y = event.button.y;
          for(int i = 0; i < number_of_menu_items; i += 1) {
            if(x>=pos[i].x && x<=pos[i].x+pos[i].w && y>=pos[i].y && y<=pos[i].y+pos[i].h)
            {
              for (int i = 0; i < number_of_menu_items; i++) {
                SDL_FreeSurface(surface_menus[i]);
                SDL_DestroyTexture(texture_menus[i]);
              }
              return i;
            }
          }
          break;
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_ESCAPE)
          {
            for (int i = 0; i < number_of_menu_items; i++) {
              SDL_FreeSurface(surface_menus[i]);
              SDL_DestroyTexture(texture_menus[i]);
            }
            return 0;
          }
      }
    }
	// Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);

    // Render menu items
    for (int i = 0; i < number_of_menu_items; i++) {
      SDL_RenderCopy(sdl_renderer, texture_menus[i], NULL, &pos[i]);
    }

    // Update Screen
    SDL_RenderPresent(sdl_renderer);

    if(1000/30 > (SDL_GetTicks()-time))
      SDL_Delay(1000/30 - (SDL_GetTicks()-time));
  }
}
  
void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
