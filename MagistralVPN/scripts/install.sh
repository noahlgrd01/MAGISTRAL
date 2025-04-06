#!/bin/bash

# Création et configuration de l'exécution
chmod a+x uninstall.sh
chmod a+x ../MagistralVPN
sudo cp ../MagistralVPN /bin/MagistralVPN

# Création du répertoire système
sudo mkdir /etc/MagistralVPN
sudo mkdir /etc/MagistralVPN/logs
sudo chmod a+w -R /etc/MagistralVPN/logs/
echo "DEFAULT_PROFILE=???" | sudo tee /etc/MagistralVPN/vpn.conf > /dev/null
echo "2FA=NON" | sudo tee -a /etc/MagistralVPN/vpn.conf > /dev/null
echo "IP=x.x.x.x" | sudo tee /etc/MagistralVPN/2fa.conf > /dev/null
echo "PORT=??" | sudo tee -a /etc/MagistralVPN/2fa.conf > /dev/null

# Installation des packages annexes
sudo apt install wireguard &> /dev/null
sudo apt install libgtk-3-dev build-essential &> /dev/null

# Compile le client GUI
cd ../GUI/
gcc -o MagistralVPN main.c `pkg-config --cflags --libs gtk+-3.0`
cd ..

# Ouvre le fichier de paramètres
echo "[*] Ouverture du fichier de paramètres"
sleep 2
sudo nano /etc/MagistralVPN/vpn.conf