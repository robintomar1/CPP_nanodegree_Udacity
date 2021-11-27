#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <fstream>
#include <sstream>


int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  int score_1=0,score_2=0,score_3=0;
  std::ofstream logFile_out;
  std::ifstream logfile_in;
  std::string line;
  try{
    logfile_in.open("../scores.txt");
    std::getline(logfile_in, line);
    std::istringstream linestream(line);
    linestream >> score_1 >> score_2 >> score_3;//in decreasing order
    renderer.setScores(score_1,score_2,score_3);
    logfile_in.close();
  }
  catch(...)
  {
     std::cerr<<"Error opening scores file\n";
  }
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  if(game.GetScore()>score_1)
  {
    score_3=score_2;
    score_2=score_1;
    score_1=game.GetScore();
  }
  else if(game.GetScore()>score_2)
  {
    score_3=score_2;
    score_2=game.GetScore();
  }
  else if(game.GetScore()>score_3)
  {
    score_3=game.GetScore();
  }
  try{
    logFile_out.open("../scores.txt");
    logFile_out<<score_1<<" "<<score_2<<" "<<score_3<<" ";
    logFile_out.close();
  }
  catch(...)
  {
    std::cerr<<"Error opening scores file\n";
  }

  return 0;
}