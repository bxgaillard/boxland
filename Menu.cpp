/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Menu.cpp
 *
 * Description : Écran de menu
 *
 * ---------------------------------------------------------------------------
 *
 * Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le
 * modifier conformément aux dispositions de la Licence Publique Générale GNU,
 * telle que publiée par la Free Software Foundation ; version 2 de la
 * licence, ou encore (à votre convenance) toute version ultérieure.
 *
 * Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE
 * GARANTIE ; sans même la garantie implicite de COMMERCIALISATION ou
 * D'ADAPTATION À UN OBJET PARTICULIER. Pour plus de détail, voir la Licence
 * Publique Générale GNU.
 *
 * Vous devez avoir reçu un exemplaire de la Licence Publique Générale GNU en
 * même temps que ce programme ; si ce n'est pas le cas, écrivez à la Free
 * Software Foundation Inc., 675 Mass Ave, Cambridge, MA 02139, États-Unis.
 *
 * ---------------------------------------------------------------------------
 */


/*
 * En-têtes
 */

// STL
#include <iostream>
#include <vector>

// Bibliothèque C standard
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// gdkmm
#include <gtkmm/main.h>
#include <gdkmm/pixbuf.h>
#include <gdkmm/color.h>
#include <gdk/gdkkeysyms.h>

// Module courant
#include "config.h"
#include "Board.h"
#include "Menu.h"


/*
 * Implémentation
 */

namespace Boxland
{

/*
 * Constructeur
 */
Menu::Menu()
    : moves(NULL), times(NULL), board(*this),
      window(Gdk::Pixbuf::create_from_file(IMAGE_DIR "/menu.png"))
{
    // Charge la liste des niveaux
    LoadLevels();
}

/*
 * Destructeur
 */
Menu::~Menu()
{
    // Détruit les tableaux s'ils existent
    if (moves != NULL)
	delete[] moves;
    if (times != NULL)
	delete[] times;
}

/*
 * Passe au niveau suivant
 */
bool Menu::NextLevel()
{
    // Enregistre les records
    unsigned int move = board.GetMoveCount();
    if (moves[level] == 0 || move < moves[level])
	moves[level] = move;
    unsigned int time = board.GetTime();
    if (times[level] == 0 || time < times[level])
	times[level] = time;

    // Passe au niveau suivant s'il en reste
    if (level < levels.size() - 1) {
	board.LoadMap(levels[++level]);
	return true;
    }

    return false;
}

/*
 * Initialisation (rattachement à une zone de fenêtre)
 */
void Menu::OnInit()
{
    // Création des couleurs
    black = Gdk::Color("black");
    green = Gdk::Color("green");
    AllocColor(black);
    AllocColor(green);
}

/*
 * Affichage
 */
void Menu::OnDraw()
{
    // Affiche l'image de fond et les informations de niveau
    DrawImage(window, Point(0, 0));
    DrawInformations();
}

/*
 * Événement produit lorsqu'une touche est appuyée
 */
void Menu::OnKeyPressed(unsigned int key)
{
    switch (key) {
    case GDK_Down:
    case GDK_Right:
    case GDK_KP_Down:
    case GDK_KP_Right:
    case GDK_KP_2:
    case GDK_KP_6:
    case GDK_KP_Add:
	// Niveau suivant
        if (level < levels.size() - 1) {
	    level++;
	    DrawInformations();
	}
	break;
    
    case GDK_Up:
    case GDK_Left:
    case GDK_KP_Up:
    case GDK_KP_Left:
    case GDK_KP_8:
    case GDK_KP_4:
    case GDK_KP_Subtract:
	// Niveau précédent
        if (level > 0) {
	    level--;
	    DrawInformations();
	}
	break;
	
    case GDK_Return:
    case GDK_space:
    case GDK_KP_Enter:
	// Démarrage du niveau sélectionné
	if (levels.size() > 0) {
	    board.LoadMap(levels[level]);
	    Switch(&board);
	}
	break;

    case GDK_Escape:
    case GDK_q:
    case GDK_Q:
	// Quitte le programme
	Gtk::Main::quit();
    }
}

/*
 * Charge la liste de niveaux
 */
bool Menu::LoadLevels()
{
    // Efface la liste et les tableaux actuels
    level = 0;
    levels.clear();
    if (moves != NULL)
	delete[] moves;
    if (times != NULL)
	delete[] times;

    // Ouvre le répertoire
    DIR *levelDirectory;
    if ((levelDirectory = opendir(LEVEL_DIR)) == NULL)
	return false;

    // Lit les entrées de répertoire
    while (struct dirent *ent = readdir(levelDirectory)) {
	std::string name = std::string(LEVEL_DIR) + '/' + ent->d_name;
	struct stat statbuf;

	// Ajoute le fichier si c'est un fichier normal
	if ((ent->d_name[0] != '.')
	    && stat(name.c_str(), &statbuf) == 0
	    && S_ISREG(statbuf.st_mode))
	    levels.push_back(name);
    }

    // Ferme le répertoire
    closedir(levelDirectory);

    // S'il y a des niveaux, initialise les scores
    if (levels.size() != 0) {
	moves = new unsigned int[levels.size()];
	times = new unsigned int[levels.size()];
	memset(moves, 0, levels.size() * sizeof(int));
	memset(times, 0, levels.size() * sizeof(int));
    }

    return true;
}

/*
 * Affiche les informatinos relatives au niveau sélectionné
 */
void Menu::DrawInformations()
{
    char buffer[32]; // Tampon temporaire pour le formatage des données

    // Efface les informations grahiquement
    SetColor(black);
    DrawRectangle(Point(520, 104), Point(98, 26));
    DrawRectangle(Point(520, 250), Point(98, 26));
    DrawRectangle(Point(520, 315), Point(98, 26));
    SetColor(green);

    // Niveau sélectionné
    if (levels.size() > 0)
	sprintf(buffer, "%d/%d", level + 1, levels.size());
    else
	strcpy(buffer, "-");
    DrawText(Point(524, 108), buffer);

    // Meilleur temps
    if (levels.size() > 0 && moves[level] > 0)
	sprintf(buffer, "%d s", times[level]);
    else
	strcpy(buffer, "-");
    DrawText(Point(524, 254), buffer);

    // Meilleur nombre de déplacements
    if (levels.size() > 0 && moves[level] > 0)
	sprintf(buffer, "%d", moves[level]);
    else
	strcpy(buffer, "-");
    DrawText(Point(524, 319), buffer);
}

} // namespace Boxland
