/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     TextScreen.cpp
 *
 * Description : Écran affichant un texte basé sur une image
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
#include <gdkmm/pixbuf.h>

// Module actuel
#include "Screen.h"
#include "Point.h"
#include "TextScreen.h"


/*
 * Implémentation
 */

namespace Boxland
{

/*
 * Constructeur
 */
TextScreen::TextScreen(Screen &scr)
    : screen(scr)
{}

/*
 * Destructeur
 */
TextScreen::~TextScreen()
{}

/*
 * Sélectionne l'image à affiche
 */
void TextScreen::SetImage(Glib::RefPtr<Gdk::Pixbuf> img, const Point &center)
{
    // Stocke la référence à l'image et calcule sa positino
    image = img;
    pos = center - Point(img->get_width() / 2, img->get_height() / 2);
}

/*
 * Évenement déclenché en cas de demande d'affichage
 */
void TextScreen::OnDraw()
{
    // Affiche le contenu de l'écran précédent
    screen.Draw();

    // Et affiche l'image par-dessus
    DrawImage(image, pos);
}

/*
 * Évenement déclenché en cas d'appui de touche
 */
void TextScreen::OnKeyPressed(unsigned int key)
{
    // Appelle la méthode de l'écran précédent
    screen.PressKey(key);
}

} // namespace Boxland 
