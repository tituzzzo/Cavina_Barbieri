#include "game.hpp"
int main()
{
    /*
    //crea un oggetto window (che sia renderizzabile a video), sf::mode è una classe costituita da due interi(la usiamo per passargli la grandezza della finestra)
    //sf::Style::<<titoloStile>>
    //Default apre una finestra con titolo, pulsanti per chiudere etc. inolyre la finestra è modificabile in grandezza
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!"); //il secondo parametro è una stringa (il titolo), come terzo paramtero possiamo passrgli lo stile della finestra
    //l'oggetto window è sensibile verso gli eventi
    //creiamo una classe eventiper catturarli
    sf::Event ev;
    */
   Game game();
    while (window.isOpen())
        //finchè la finestra recepisce degli eventi
        //noi catturiamo gli eventi e li mettiamo in ev con window.pollEvent()
        while (window.pollEvent(ev))
        {
            //usiamo uno switch per gestire gli eventi
            //.type mi ritorna il tipo di evento e le eventuali info che ne comportano
            switch (ev.type){
                case sf::Event::Closed: //evento(bottone di chiusura finestra)
                window.close(); //metodo che chiude la finestra
                break;
                case sf::Event::KeyPressed:
                if (ev.key.code==sf::Keyboard::Escape){
                    //do soomething
                }

            }
           //Update

           //Render
           window.clear(); //clear old frame, si può specificare con quale colore tramite sf::color::<Colore> oppure con (R,G,B,transaprency)

           window.display(); //
        }

    return 0;
}