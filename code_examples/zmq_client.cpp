//  Clone client Model One

#include "zmq.hpp"
#include "zmq_addon.hpp"
#include <iostream>
#include <chrono>

int main(void)
{
    zmq::context_t ctx(1); //Třída context_t je používána k vytvoření všech soketů v programu.

    zmq::socket_t subscriber(ctx, zmq::socket_type::sub); // Vytvoření třídy socket_t reprezentující výstupní socket. Druhý argument určuje, zda do socketu můžeme zapisovat (publish) nebo z něj číst (subscribe).
    subscriber.connect("tcp://localhost:5556"); // Funkcí connect se socket připojí k socketu, který už jiný program zaregistroval na danou adresu.
    subscriber.set(zmq::sockopt::subscribe, "");

    zmq::socket_t command_sender(ctx, zmq::socket_type::pub);
    command_sender.connect("tcp://localhost:5557");

    std::cout << "start done" << std::endl;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        zmq::message_t received;
        while(subscriber.recv(received, zmq::recv_flags::dontwait)) {
            std::cout << "received: \"" << received.to_string() << "\"" << std::endl;
        }

        // read user input
        std::string u_in;
        getline(std::cin, u_in);

        zmq::message_t msg(u_in.c_str(), u_in.length());
        command_sender.send(msg, zmq::send_flags::none);
    }
    return 0;
}