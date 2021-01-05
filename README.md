# Project 17-TankAttack :memo: :point_down:	

TankAttack je 2D multiplayer (komunikacija 2 igrača preko mreže) igrica koja simulira bitku između tenkova koji se nalaze u lavirintu. Cilj igrice je eliminisati protivnika pre nego što on eliminiše tebe. Inicijalno ispaljuju tenkovsko đule, a tokom bitke postoji mogućnost skupljanja supermoćnoh oružja koja olakšavaju eliminaciju protivnika. Igrica će imati različite teksture i modove kao što je simulacija bitke između dva igrača koji igraju na istom računaru. Postoje još neke ideje koje ćemo pokušati da uklopimo tokom razvoja projekta. 

## Table of contents :sunglasses:	
* [General info](#general-info)
* [Screenshots](#screenshots-dart)
* [Technologies](#technologies-wrench)
* [Setup](#setup-pencil2)
* [Features](#features)
* [Status](#status)
* [Inspiration](#inspiration)
* [Developers](#developers-punch-muscle)

## Screenshots :dart:
Snimke svih modova igre mozete pogledati na sledecem youtube linku:
* [tank_attack](https://www.youtube.com/playlist?list=PLzcUic-tf2LSq8aBPbCxmtnR_erb_Fu7O)

![Example screenshot](./screenshots/tank_attack_final_gif.gif)

## Technologies :wrench:
*
*
*

## Setup :pencil2:
Prvo je potrebno klonirati repozitorijum i pozicionirati se u njega:
```
$ git clone htttp-adress --ovde ce da bude pravi http
$ cd 17-tankattack/
```

Primer prevodjenja i pokretanja programa koriscenjem qmake-a i Makefile:
``` 
$ mkdir ../build_tank_attack
$ qmake -makefile -o ../build_tank_attack/Makefile "CONFIG+=tank_attack" tank_attack.pro
$ cd ../build_tank_attack
$ make 
$ ./tank_attack
```
Aplikaciju mozete pokrenuti takodje preko QtCreator-a, kako ga instalirati:
```
sudo apt-get install build-essential
sudo apt-get install qtcreator
sudo apt-get install qt5-default
```
Da biste imali zvuk tokom igrice potrebno je da instalirate neke dodatne biblioteke:
```
$ sudo apt-get update
$ sudo apt-get install qtmultimedia5-dev libqt5multimediawidgets5 libqt5multimedia5-plugins libqt5multimedia5
```


## Developers :punch: :muscle:

- [Nikola Mićić, 170/2017](https://gitlab.com/nikolamicic) :boy:
- [Luka Miletić, 91/2017](https://gitlab.com/lukamileticc) :man:
- [Nikola Lazarević, 267/2017](https://gitlab.com/_nikolalazarevic) :person_with_blond_hair:
- [Slobodan Jovanović, 186/2017](https://gitlab.com/Sloba98) :man_with_gua_pi_mao:
- [Mihailo Trišović, 474/2018](https://gitlab.com/mihailotrisovic) :baby:
