#include "zmq.hpp"
#include "zmq_addon.hpp"
#include <iostream>
#include <chrono>

int main(void)
{
    zmq::context_t ctx(1); // Třída context_t je potřebná k vytvoření všech soketů v programu.

    zmq::socket_t subscriber(ctx, zmq::socket_type::sub); // Vytvoření třídy socket_t reprezentující výstupní socket. Druhý argument určuje, zda do socketu můžeme zapisovat (publish) nebo z něj číst (subscribe).
    subscriber.connect("tcp://localhost:5556"); // Funkcí connect se socket připojí k socketu, který jiný program zaregistroval / zaregistruje na danou adresu.
    subscriber.set(zmq::sockopt::subscribe, ""); // Nastavení socketu pro příjmání všech zpráv.

    zmq::socket_t command_sender(ctx, zmq::socket_type::pub);
    command_sender.connect("tcp://localhost:5557");

    while (true)
    {
        zmq::message_t received; // Vytvoření třídy message_t, do které můžeme zapsat přijatou zprávu.
        while (subscriber.recv(received, zmq::recv_flags::dontwait)) // Metodou recv se pokusíme přijmout zprávu ze socketu. Pokud je v socketu zpráva, uloží se do proměnné received a cyklus pokračuje. Díky argumentu zmq::recv_flags::dontwait je metoda neblokující. Tímto způsobem přečteme všechny zprávy, které jsou v socketu.
        {
            std::cout << "received: \"" << received.to_string() << "\"" << std::endl;
        }

        std::string user_input;
        getline(std::cin, user_input); // Přečtení vstupu od uživatele.

        zmq::message_t msg(user_input.c_str(), user_input.length()); // Vytvoření zprávy, kterou chceme odeslat.
        command_sender.send(msg, zmq::send_flags::none); // Odeslání zprávy.
    }
    return 0;
}