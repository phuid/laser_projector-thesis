var zmq = require("zeromq"),
  publisher = zmq.socket("pub"), // Vytvoření objektu publisher representujícího výstupní socket. Argument "pub" značí, že program bude zprávy do socketu posílat
  command_receiver = zmq.socket("sub"); // Vytvoření socketu command_receiver representujícího vstupní socket. Argument "sub" značí, že program bude zprávy ze socketu přijímat

publisher.bindSync("tcp://*:5558"); // Přiřazení adresy a portu socketu publisher

command_receiver.bindSync("tcp://*:5559"); // Přiřazení adresy a portu socketu command_receiver
command_receiver.subscribe(""); // Přihlášení k odběru všech zpráv

command_receiver.on("message", (msg) => { // Při přijetí zprávy
  console.log("Přijatá zpráva: ", msg.toString()); // Vypsání přijaté zprávy do konzole
  publisher.send("prijal jsem zpravu: " + msg.toString()); // Odeslání zprávy do socketu publisher
});