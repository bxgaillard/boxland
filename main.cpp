/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     main.cpp
 *
 * Description : Fonction principale
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

// gtkmm
#include <gtkmm/main.h>
#include <gtkmm/window.h>

// Module actuel
#include "Menu.h"
#include "WindowArea.h"

/*
 * Fonction principale
 */
extern "C" int main(int argc, char **argv)
{
    // Crée la fenêtre et l'initialise
    Gtk::Main kit(argc, argv);
    Gtk::Window win;
    win.set_title("Boxland");
    win.set_resizable(false);

    // Crée le premier écran et la zone dans la fenêtre
    Boxland::Menu menu;
    Boxland::WindowArea area(&menu);
    win.add(area);

    // Affiche la fenêtre et entre dans la boucle principale
    area.show();
    Gtk::Main::run(win);

    // Quitte sans erreur
    return 0;
}
