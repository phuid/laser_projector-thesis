/* části, kde knihovna http odesílá, nebo příjmá soubory jsou přeskočeny */

var zmq = require("zeromq"),
  lasershow_sender = zmq.socket("pub"),
  wifiman_sender = zmq.socket("pub"),
  lasershow_receiver = zmq.socket("sub"),
  wifiman_receiver = zmq.socket("sub"); // definice zeromq socketů; Ta už byla několikkrát podrobně popsána u programů wifi_manager a UI

var io = require("socket.io")(server); // inicializace knihovny socket.io
var SSHClient = require("ssh2").Client; // inicializace knihovny ssh2

//připojení zeromq socketů
lasershow_sender.connect("tcp://localhost:5557");
lasershow_receiver.connect("tcp://localhost:5556");
lasershow_receiver.subscribe("");

wifiman_sender.connect("tcp://localhost:5559");
wifiman_receiver.connect("tcp://localhost:5558");
wifiman_receiver.subscribe("");

/* definice zpracování HTTP requestů knihovnou http */

io.on("connection", function (socket) { // Definice funkce volané při připojení klienta
  var conn = new SSHClient(); // Spuštění ssh terminálu, do něho server vypisuje systémové příkazy
  conn
    .on("ready", function () { // Definice funkce spuštěné při spuštění ssh terminálu
      conn.shell(function (err, stream) { // Připojení k rozhraní terminálu, při připojení se zavolá námi definovaná funkce, která dostane argumenty err a stream. Argument err je použit k detekci errorů, argument stream je využit pro samotnou komunikaci s terminálem
        if (err) // Ověření, že se terminál otevřel bez problémů. Pokud je objekt err evaluován jako true, něco se pokazilo.
          return socket.emit(
            "sshdata",
            "\r\n*** SSH SHELL ERROR: " + err.message + " ***\r\n"
          );
        socket.on("sshdata", function (data) { // Jestliže socket.io příjme zprávu týkající se tématu "sshdata", vypíše ji do terminálu
          stream.write(data);
        });
        stream
          .on("data", function (d) { // Definice funkce, která je zavolána, jestliže terminál chce vypsat nějáké data. V tom případě je server pošle klientovi
            socket.emit("sshdata", d.toString("binary"));
          })
          .on("close", function () { // Definice funkce zavolané v případě, že je stream terminálu uzavřen
            socket.emit("sshdata", "**ssh stream closed**");
          });
      });
    })
    .on("close", function () { // Definice funkce zavolané v případě, že je terminál uzavřen
      socket.emit("sshdata", "\r\n*** SSH CONNECTION CLOSED ***\r\n");
    })
    .on("error", function (err) { // Definice funkce zavolané v případě, že v terminálu nastane chyba
      socket.emit(
        "sshdata",
        "\r\n*** SSH CONNECTION ERROR: " + err.message + " ***\r\n"
      );
    });
    conn.connect({ // Samotné připojení k ssh terminálu, využívá rsa klíče místo hesel
      host: config.sshHost,
      username: config.sshUsername,
      privateKey: fs.readFileSync(config.sshKeyPath),
    });

    socket.on("LASERSHOWdata", function (data) { // Když od klienta přijde zpráva úrčená pro program lasershow, server ji přepošle do příslišného zeromq socketu
      lasershow_sender.send(data.toString());
    });
    socket.on("WIFIMANdata", function (data) { // Když od klienta přijde zpráva úrčená pro program wifi_manager, server ji přepošle do příslišného zeromq socketu
      wifiman_sender.send(data.toString());
    });
});

lasershow_receiver.on("message", (msg) => { // Když zeromq socketem příjde zpráva od programu lasershow, server ji přepošle klientovi s označním, od kterého programu zpráva přichází
  io.sockets.emit("LASERSHOWmsg", msg.toString() + "\n\r");
});
wifiman_receiver.on("message", (msg) => { // Když zeromq socketem příjde zpráva od programu wifi_manager, server ji přepošle klientovi s označním, od kterého programu zpráva přichází
  io.sockets.emit("WIFIMANmsg", msg.toString() + "\n\r");
});

/* spuštění http serveru */
