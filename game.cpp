#include "game.hpp"
void Game::initVariables(){
    this->window = nullptr //la window è allocata dinamicamente cosi la possiamo chiudere quando vogliamo

}

void Game::initWindow(){
    this->VideoMode->getDesktopMode;
    this->window= new sf::RenderWindow(this->VideoMode, "SFML works!")
}
void Game::Game(){
    this->initVariables();
    this->initWindow();
}

void Game::~Game(){
    delete this->window; //fondamentale ricordarsi di cancellare altrmenti grNDE PERDITA DI MEMORIA
}
//Accessors
//Functions
