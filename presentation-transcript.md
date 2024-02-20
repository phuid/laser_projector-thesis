### Uvod (1)
Vazena poroto, milí posluchači, rád bych vám představil moji středoškolskou odbornou činnost, s názvem RGB Laserovy projektor, ktera spada do oboru 10 -- elektrotechnika, elektronika a telekomunikace a byla finančne podpořena jihomoravským krajem.

#### Laser scanning (2)
Laserove projektory funguji díky technologii laser scanningu, která spočívá v rychlém pohybu laserového paprsku.

Tato technologie se využívá například v...

#### Dálkové promítání (3)
dálkovém promítání,
#### Laser shows (4)
laser shows pro publikum,
#### HUD (5)
v head up displejích,
#### 3D skenování (6)
při 3d skenování prostor a zemského povrchu,
#### Skenování čárových kódů (7)
nebo skenování čárových kódů.
#### SLA 3D tisk (8)
dále se používá v SLA 3D tiskárnách,
#### Laserové gravírování (9)
nebo v laserových gravírovačkách.

### Cíle (10)
Bohužel ale neexistuje žádná uživatelsky přívětivá open-source platforma, díky které by se zájemci o laser scanning mohli s technologií seznámit.

Proto jsem si vytyčil následující cíle:

- Navrhnout a sestavit laserový projektor, který si mohou sestavit zájemci o laser scanning
- Naprogramovat k tomuto projektoru sofwarove.

  - který by měl umožňovat promítání
  - měl by mít přívětivé uživatelské prostředí\
  a
  - mělo by být jednoduché ho nainstalovat.

- A nakonec software i plány projektoru nahrát na platformu github, která slouží pro sdílení open-source kódu.

### Princip laserové projekce (11)

Laserová projekce je založena na principu persistance of vision, který spočívá v tom, že se světelný bod pohybuje rychleji, než lidské oko tento pohyb zaznamená.\
To znamená, že lidské oko vidí křivku, po které se bod pohyboval i když tam žádná čára není.

Tento efekt je možné pozorovat například, když roztočíme prskavku.
V tu chvíli je vidět kružnice, kterou opíše hořící bod.

### Promítání (12)
V laserovém promítání je tímto světelným bodem samozřejmě laserový paprsek. V mém projektoru tento paprsek vytváří RGB laserový modul, který pomocí polopropustných zrcátek skládá tři paprsky (červený, modrý a zelený) do jednoho libovolně barevného.

Pohyb paprsku poté ovládají dva galvanometry se zrcátky, které jsou uspořádané tak, aby každý ovládal jednu osu pohybu.

### Řídící jednotka (13)
Jako řídící jednotku jsem si vybral jednodeskový počítač Raspberry Pi.
V jeho odhaleném 40pinovém konektoru je 27 GPIO pinů, na které můžu připojit svoje periferie.\
Raspberry Pi také disponuje možností WiFi připojení a
Běží na něm operační systém Linux, díky kterému je možné projektor ovládat i připojením monitoru a klávesnice.

### LCD a enkodér (14)
Projektor se dá ovládat i přes zabudovaný displej a rotační enkodér.

### DPS -- HAT (15)
Všechny komponenty projektoru jsou spojené deskou plošných spojů, kterou jsem nadesignoval jako takzvaný HAT.
To znamená, že sama drží na 40 pinovém konektoru Raspberry Pi a nezabírá o moc víc místa, než samotné Raspberry Pi.

Na této desce jsou kromě konektorů periferií, které jsou přímo napojeny na 40pinový konektor RPi i tři důležité obvody a sice:\
Generátor signálu pro galvanometry,\
zdroj symetrického napětí pro galvanometry a\
obvod správy baterie.

### Generátor signálu pro galvanometry (16)
Z těchto obvodů stojí za to vysvětlit generátor signálu pro galvanometry.

Galvanometry příjmají dvě diferenciální analogové napětí v rozsahu -10 až +10 V,\
kdy Každé z těchto napětí udává jednu souřadnici paprsku, X a Y.

V obvodu D/A převodník na základě příkazů z Raspberry Pi generuje libovolné napětí v rozsahu 0 -- 5 V\
a tento signál následně operační zesilovače transformují do požadovaného napěťového rozsahu.

### Struktura softwaru (17)
Softwarová výbava se skládá celkem z pěti programů,\
tří frontendových (to jsou LCD, web_ui a discord_bot)\
a dvou backendových (to jsou lasershow a wifi_manager)

Front-endové programy interagují s uživatelem
a komunikují s každým back-endovým programem pomocí dvou socketů--\
--(vstupního, kterým zasílají příkazy od uživatele
  a výstupního, kterým příjmají informace k zobrazení uživateli).

### Instalační skript (18)
Kromě toho je součástí softwarové výbavy i instalační skript, který zajišťuje jednoduchou instalaci knihoven a závislostí mých programů
a k tomu nastaví Raspberry Pi tak, aby se moje programy spustily při každém zapnutí.

Díky němu je možné můj software nainstalovat na jakékoliv raspberry pi těmito třemi příkazy

### Program lasershow – promítání (19)
program lasershow je backendový program, který se stará o promítání.\
Byl inspirován open-source projektem rpi-lasershow.\
Oproti němu má ale navíc například funkce promítání barevných projekcí, samozřejmě komunikaci s frontendovými programy,\
ale také možnost nastavování různých vlastností projekce, které vám snad předvedu při ukázce.

### Program wifi_manager (20)
Díky programu wifi manager je možné si ve frontendových programech vybírat mezi třemi stavy wifi připojení\
to jsou:
- vypnutá wifi
- zapnutá wifi, kdy se raspberry pi snaží připojit na známé sítě v okolí\
a
- zapnutá wifi, kdy raspberry pi vysílá vlastní wifi síť, ke které se dá připojit

Dále front-endovým programům posílá aktuální informace o wifi připojení, aby je zobrazily uživateli.

### Uživatelské prostředí (21)
Přes front-endové programy může uživatel jednoduše posílat back-endovým programům příkazy nebo nahrát soubor k promítnutí.\
Uživatelské prostředí se skládá ze tří částí:
- displeje a enkodéru přímo na zařízení
- webového rozhraní, ke kterému se dá připojit z lokální sítě\
a
- bota na chattovací aplikaci discord, se kterým může uživatel interagovat odkudkoliv na světě

### Výsledky (22)
Když to shrnu,\
v této práci jsem navrhnul a sestavil vlastní laserový projektor.\
Naprogramoval jsem k němu jednoduché uživatelské prostředí\
a\
celý projekt jsem nahrál na platformu github.com, určenou ke sdílení open-source kódu

V práci by se dalo pokračovat zprovozněním napájení z baterií, díky kterému by bylo možné si projektor vzít,,, a předvést ho kdekoliv.\
Dále by se do webového rozhraní dala přidat funkce živého kreslení.\
Do ní by bylo možné implementovat i kameru, díky které by se dalo kreslit online do obrazu reality.

### Poděkování (23)
Na závěr bych rád poděkoval:
mému externímu konzultantovi Tomáši Rohlínkovi za věcné rady a připomínky k projektu,\
mé interní konzultantce magistře Kateřině Vídenkové za pomoc s prací,\
a JCMM a jihomoravskému kraji za finanční podpoření práce.\

a nyní bych vám rád funkce mého projektoru ukázal.





a především vám za pozornost (po ukázce???)
