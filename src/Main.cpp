// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include <iostream>
#include <SDL2/SDL.h>

int main(int argv, char** args)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr<<"Error initializing SDL!\n";
    return 1;
  }

  SDL_Quit();
  std::cout<<"Program ended successfully\n";
  return 0;
}