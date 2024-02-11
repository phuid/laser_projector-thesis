//  Clone server Model One

#include "zmq.hpp"
#include "zmq_addon.hpp"
#include <iostream>
#include <string>

int main(void)
{
    zmq::context_t ctx(1); // Třída context_t je používána k vytvoření všech soketů v programu.

    zmq::socket_t publisher(ctx, zmq::socket_type::pub); // Vytvoření třídy socket_t reprezentující výstupní socket. Druhý argument určuje, zda do socketu můžeme zapisovat (publish) nebo z něj číst (subscribe).
    publisher.bind("tcp://*:5556"); // Funkcí bind je socketu přiřazena adresa. V tomto případě socket pracuje s protokolem tcp na portu 5556.
    
    zmq::socket_t command_receiver(ctx, zmq::socket_type::sub); // Vytvoření třídy reprezentující vstupní socket.
    command_receiver.bind("tcp://*:5557"); // Vstupnímu socketu je přiřazena adresa.
    command_receiver.set(zmq::sockopt::subscribe, ""); // Jestliže je socket typu sub, je třeba nastavit, na která témata bude reagovat. V tomto případě je nastaveno, že bude reagovat na všechna témata.

    while (true)
    {
        zmq::message_t received; // Vytvoření třídy message_t, která reprezentuje zprávu. Při príjmání zpráv je třeba mít předem vytvořenou třídu message_t, do které se zpráva uloží.
        if (command_receiver.recv(received, zmq::recv_flags::none)) // Příjem zprávy. Příjem je blokující, dokud není zpráva přijata. Při použití argumentu zmq::recv_flags::dontwait by byl příjem neblokující. Funkce vrací true, pokud byla zpráva úspěšně přijata, jinak vrací false.
        {
        std::cout << "přijato: \"" << received.to_string() << "\"" << std::endl;

        std::string string_to_be_sent = "potvrzuji, přijal jsem zprávu \"" + received.to_string() + "\"";

        zmq::message_t to_be_sent(string_to_be_sent.c_str(), string_to_be_sent.length() + 1); //Vytvoření zprávy, která bude odeslána.
        publisher.send(to_be_sent, zmq::send_flags::none); // Odeslání zprávy.
        }
        else {
            std::cout << "Nebyla úspěšně přijata žádná zpráva."
        }

    }
    return 0;
}