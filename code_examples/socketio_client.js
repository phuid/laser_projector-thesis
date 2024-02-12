// klient
// knihovny jsou přiádny v html kódu

class terminal {
  constructor(container_id) { // díky tomuto kontruktoru je jednoduché k více terminálům knihovny xterm.js přiřadit jejich html elementy a nastavit jejich vlastnosti
    this.container = document.getElementById(container_id);
    this.term = new Terminal({
      cursorBlink: true,
      allowTransparency: true,
      drawBoldTextInBrightColors: true,
      fontSize:
        1 * parseFloat(getComputedStyle(document.documentElement).fontSize),
      theme: {
        background: "rgba(0, 0, 0, 0)",
      },
    });
    this.fit = new FitAddon.FitAddon();
    this.term.loadAddon(this.fit);
    this.term.open(this.container);
    this.fit.fit();
  }
}

var terminals = { // výše popsaný kontruktor je využit k vytvoření 3 xterm terminálů
  ssh: new terminal("ssh-terminal-container"),
  lasershow: new terminal("lasershow-terminal-container"),
  wifiman: new terminal("wifiman-terminal-container"),
};

// Browser -> Backend
terminals.ssh.term.onData(function (ev) { // Pokud do ssh terminálu uživatel něco zadá, klient to pošle serveru pro zpracování v ssh
  socket.emit("sshdata", ev.toString());
});

// Browser -> Backend
var lasershow_line = ""
terminals.lasershow.term.onData(function (ev) { // Pokud uživatel něco zadá do terminálů naležícím komunikaci s backendovými programy, tento text je ukládán do řetězce lasershow_line a je odeslán až, když uživatel stiskne enter
  string = ev.toString();
  lasershow_line += string;
  terminals.lasershow.term.write(ev);
  if (string.match(/\n|\r/gi) != null) { // hledání klávesy enter pomocí regexu (regulárního výrazu)
    lasershow_line = lasershow_line.replace(/\n|\r/gi, "")
    socket.emit("LASERSHOWdata", lasershow_line);
    terminals.lasershow.term.write("\r> ");
    lasershow_line = "";
  }
});
//to stejné platí i pro wifi_manager


// Backend -> Browser
socket.on("sshdata", function (data) { // Při přijetí dat pocházejících z ssh jsou vypsány do náležitého terminálu
  terminals.ssh.term.write(data);
});
// Backend -> Browser
socket.on("LASERSHOWmsg", function (data) { // Při přijetí dat pocházejících od programu lasershow jsou vypsány do náležitého terminálu
  console.log(data);
  terminals.lasershow.term.write(data.replace(/\n/g, "\n\r"));
});
// Backend -> Browser
socket.on("WIFIMANmsg", function (data) { // Při přijetí dat pocházejících od programu wifi_manager jsou vypsány do náležitého terminálu
  terminals.wifiman.term.write(data.replace(/\n/g, "\n\r"));
});