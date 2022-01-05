
.\" String variables used in this document
.ds cs \fIclent-server\fP
.ds s \fIserver\fP
.ds c \fIclient\fP
.ds FAM DejavuSerif

.B1
.PSPIC -C "uni.eps"


.TL
Univerzitet u Novom Sadu
Fakultet Tehničkih Nauka
Departman za Računarstvo i automatiku

.AU
Dušan Stojković RA 140/2019

.AB 
.ps 14
.ce
Dokumentacija iz Osnova Računarskih mreža: Sistem poštanskih sandučića
.AE

.B2

.bp 1

.NH 1
Opis zadatka

.LP
Realizovati primer namenskog sistema poštanskih sandučića pomoću TCP protokola. Primer se sastoji iz serverske i
klijentske strane. Klijent preko tastature može da zadaje sledeće komande: 

.IP \(bu 5 
Login - Prijava korisnika.
.IP \(bu 5 
Logout - Odjava korisnika.
.IP \(bu 5 
Send - Slanje poruke u poštanski sandučić
.IP \(bu 5 
Check - Provera da li postoji poruka za prijavljenog u poštanskom sandučiću.
.IP \(bu 5 
Receive - Prijem poruke iz poštanskog sandučića.

.LP 
Po prihvatanju zahteva za prijem poruke i nakon slanja iste, serverska strana tu poruku uklanja.

.NH 1
Realizacija u C++ programskom jeziku

.LP
Osnovna ideja koja leži iza odluke da relaizujemo sam projekat u C++ programskom jeziku je mogućnost upotrebe:
naprednih struktura podataka, biblioteka za konkurentno programiranje i realizacija projekta u objektno orijentisanom
stilu.

.LP
Projekat je poželjno podeliti u više faza izrade kako bi realizacija bila što jasnija. Na taj način znamo tačno
gde smo stigli i možemo diskutovati promene u fazama koje nisu implementirane. Svakoj fazi ću dodeliti ime tako da
se redni broj faze može videti na osnovu poglavlja. 

.LP 
Smatram za korisnim da se dalje dokumentovanje, u smislu same implenetacije, vodi na \fBgithub\fP jer bi ovde bilo suvišno,
ovaj dokument služi da se u kratke crte definiše svaki deo projekta i funkcionalnosti.

.NH 2
Prevođenje jednostavne \*[cs] arhitekture u C++

.LP
Za ovaj deo procesa već raspolažemo C kodom sa labaratorijskih vežbi. Namena tog koda je da uz pomoć TCP protokola
pošalje sa klijenta na server poruku koja će se na konzoli servera ispisati. Prvi korak je uspešno kompajlirati
izvorni kod sa \fBg++\fP komandom. 

.LP
Pošto će se kasnije dodavati novi fajlovi u projekat, poželjno je kreirati \fBMAKEFILE\fP koji će kompajlirati ceo projekat \*[cs] arhitekture.

.LP
Poželjno će biti da se naprave zasebne klase koje u sebi sadrže metode \*[cs] za uspostavljanje i prekid veze TCP
protokola.

.NH 2
Konkurentno programiranje \*[cs]  

.LP
Prvi korak ove faze je dodavanje podrške za konkurentno opsluživanje više klijenta na serveru.
Ovde ja važno jasno definisati koje funkcije treba
opsluživati konkurentno. Dijagram koji za proizvoljnog klijenta uspostavlja vezu sa serverom je sledeći:

.PS 
box "init client-server" width 2
arrow down  
box "call server listen loop" width 2
arrow down
box "new client request" width 2
arrow down
box "init thread eval loop" width 2 
arrow down
box "wait for LOGIN from client" width 2
.PE

.QP
.ce
Blok dijagram petlje \*[s] funkcionalnosti u ovoj fazi

.LP
Jednostavna realizacija logovanja korisnika se može postići uz pomoć \fI.txt\fP fajla za to će biti dovoljno
definisati funkcije čitanja svih korisnika \fBRegistrovanih\fP na serveru (imati u vidu da je jedno od proširenja
mogućnost registracije) i spakovati u listu sve korisnike pri inicijalizaciji \*[s] strane.

.LP
Petlja koja čita dolazne poruke i interpretira ih mora biti definisana delom u jednoj funkciji, ona čita dolazne
poruke i čita im tip na osnovu tipa poziva se dalje neka od funkcija koja je deo specifikacije. Ova funkcija interpretira dolazne poruke na osnovu rečnika koji sadrži
u sebi sve moguće tipove poruka i funkcije koje su asocirane sa tim porukama. Sam sadržaj poruke se dalje interpetira
po specifikaciji pozivom očekivane funkcije.

.LP
Konstruisati objekat tipa klijent na serverskoj strani nakon poziva LOGIN komande, do tada mogu se smatrati klijenti povezani na server kao GUEST. Nakon LOGIN komande logično da poziv LOGOUT komande vraća status klijenta sa ulogovan na GUEST.

.NH 2
Realizacija prijema i slanja poruka

.LP
Kada je temelj veze između klijenta i servera uspostavljen, možemo se posvetiti implementaciji Send,
Check i Receive funkcija. Server strana mora posedovati listu svih klijenata koji sadrže listu poruka koje su primljene od drugih
klijenta.
Sama metoda za primanje poruka na \*[s] strani smešta poruke u tu listu. Kada klijent1 pokrene Check funkciju, otvara se
kanal preko koga se sve poruke poslate sa drugih klijnta na server prebrajaju i formatira se kratak opis tih poruka
koji se prosleđuje do klijenta1. 

.LP
Receive funkcija generalno može da se odradi na dva načina:

.IP \(bu 5 
Klijent1 traži od servera poruke po njegovom izboru tako što specifikuje neke parametre u RECIEVE funkciji.
Primer je primi poruke samo od klijenta2.

.IP \(bu 5 
Klijent1 ne specifikuje ništa u RECIEVE funkciji tako da se proslede sve poruke namenjene na serveru do
klijenta1.

.LP
Naravno, poruke koje se proslede sa servera se moraju i obrisati po specifikaciji.

.LP
Dodaću da treba proširiti skup poziva originalne specifikacije jer se naredbe moraju podeliti na \*[s] side i
\*[c] side.

.NH 2
Dodatak A: Enkripcija

.LP
Ideja je sledeća: dodati clasu \fBEncrypt\fP koja bi svaku poruku koja se šalje na server enkriptovala nekom hash
funkcijom (SHA256). Ovo je posebno komplikovano kada se sadržaj poruke enkriptuje ali zato je sasvim izvodljivo da se
modifikuje LOGIN i LOGOUT tako što se šifre korisnika enkriptuju i dekriptuju na neki jednostavan način.

.NH 2
Dodatak B: Konkurentan \*[c]

.LP
Proširiti klijenta tako da postoje dve niti jedna za ispis na konzoli, a duga za upis na konzoli, tako da se resorsi
ne bi blokirali. Ovo naravno po specifikaciji zadatka nema smisla, jer blokiranje neće da se desi nikad, ali ako se
RECIEVE i SEND automatizuju dobili bismo pravi chat klijent nešto što jako želim napraviti. 

.NH 2
Dodatak C: Formatirati poruke u XML zaglavljima

.LP
Ideja je jednostavna, serializovati poruke u xml tako što ćemo koristiti stringstream metode (vrlo verovatno
postoje funkcije koje rade ovo za nas). Ovo ima dosta smisla raditi jer se time naš program može mnogo lakše skalirati.
Ideja je da se sva bitna polja neke klase \fBMessage\fP formatiraju u string koji liči na xml fajl pa takav string
proslediti dalje.
