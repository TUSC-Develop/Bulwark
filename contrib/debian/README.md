
Debian
====================
This directory contains files used to package tuscd/tusc-qt
for Debian-based Linux systems. If you compile tuscd/tusc-qt yourself, there are some useful files here.

## tusc: URI support ##


tusc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install tusc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your tuscqt binary to `/usr/bin`
and the `../../share/pixmaps/tusc128.png` to `/usr/share/pixmaps`

tusc-qt.protocol (KDE)

