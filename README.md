# BAN WARNING

```diff
- THIS SOFTWARE OPEN CONNECTIONS TO BLIZZARD ONLINE SERVICES AND PERSONS DOING THIS MAY BE SUBJECT FOR ACCOUNT SUSPENSION OR CLOSURE.

- DO NOT USE THIS SOFTWARE IF YOU ARE NOT READY TO FACE THE CONSEQUENCES.

- YOU HAVE BEEN WARNED!
```

# Instructions

compile, install, rename app.dist.yml to app.yml, modify app.yml, run

Refer to discord developer documentation for how to get a bot id and token

d2r web credentials token can be gotten from battle.net website by looking at the cookies while logged in

# Windows

Requires up-to-date vcpkg installation with environment variable `VCPKG_ROOT` pointing to the vcpkg root directory

- Tested on Windows 10
- Visual Studio 2022

# Ubuntu

- tested on ubuntu 22.04 LTS
- cmake 22+
- gcc 11+
- g++ 11+

## required packages

`sudo apt update && sudo apt install libfmt-dev libspdlog-dev libyaml-cpp-dev libssl-dev libboost-all-dev libopus0 libopus-dev libsodium-dev`

## install dpp

```
cd ~
wget -O dpp.deb https://dl.dpp.dev/
dpkg -i dpp.deb
rm dpp.deb
```

## compile and install

```
cd ~
git clone https://github.com/ejt1/d2r-discord-bot
cd d2r-discord-bot
mkdir build
cd build
cmake .. --install-prefix=~/d2r-discord-bot-bin
make -j8
make install
```

## run

```
cd ~/d2r-discord-bot-bin
cp app.dist.yml app.yml
nano app.yml
./d2r-discord-bot
```
