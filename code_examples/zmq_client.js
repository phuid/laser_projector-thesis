var zmq = require("zeromq"),
  subscriber = zmq.socket("sub"), // Vytvoření objektu subscriber representujícího vstupní socket. Argument "sub" značí, že program bude zprávy ze socketu přijímat
  command_sender = zmq.socket("pub"); // Vytvoření objektu command_sender representujícího výstupní socket. Argument "pub" značí, že program bude zprávy do socketu posílat

  subscriber.connect("tcp://localhost:5558"); // Připojení socketu subscriber k adrese a portu
  subscriber.subscribe(""); // Přihlášení k odběru všech zpráv

command_sender.connect("tcp://localhost:5559"); // Připojení socketu command_receiver k adrese a portu

command_receiver.on("message", (msg) => { // Registrace callback funkce, která se spustí při přijetí zprávy
  console.log("Přijatá zpráva: ", msg.toString()); // Vypsání přijaté zprávy do konzole
  publisher.send("prijal jsem zpravu: " + msg.toString()); // Odeslání zprávy do socketu publisher
});