#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>

class Game{
    private:
    //Variables
    //window
    sf::VideoMode* VideoMode;
    sf::RenderWindow* window; //va inizializzata nel .cpp
    sf::Event ev; //è inizializzata di default
    //Private Functions
    void initVariables();
    void initWindow();


    public:
    //Constructor Decontructor
    Game();
    virtual ~Game();
    //Functions
    void update();
    void render();


}





