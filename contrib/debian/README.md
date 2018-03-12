
Debian
====================
This directory contains files used to package ultimad/ultima-qt
for Debian-based Linux systems. If you compile ultimad/ultima-qt yourself, there are some useful files here.

## ultima: URI support ##


ultima-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install ultima-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your ultima-qt binary to `/usr/bin`
and the `../../share/pixmaps/ultima128.png` to `/usr/share/pixmaps`

ultima-qt.protocol (KDE)

