/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     TextScreen.h
 *
 * Description : Écran affichant un texte basé sur une image (en-tête)
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


// Protection
#ifndef BOXLAND_TEXTSCREEN_H
#define BOXLAND_TEXTSCREEN_H

/*
 * En-têtes
 */

// gtkmm
#include <gdkmm/pixbuf.h>

// Module actuel
#include "Point.h"
#include "Screen.h"

namespace Boxland
{

// Déclaration anticipée
class Screen;

/*
 * Classe TextScreen : écran de jeu chargé d'afficher une image en
 * transparence de l'écran appelant, de manière à présenter un texte à
 * l'utilisateur
 */
class TextScreen : public Screen
{
public:
    /*
     * Constructeur et destructeur
     */

    // Constructeur
    explicit TextScreen(Screen &scr);

    // Destructeur
    virtual ~TextScreen();


    /*
     * Initialisation et terminaison
     */

    // Sélectionne l'image à afficher
    void SetImage(Glib::RefPtr<Gdk::Pixbuf> img, const Point &center);

    // Retourne à l'écran précédent
    void Terminate() { Return(); }

protected:
    /*
     * Méthodes de gestion d'événements
     */

    // L'affiche est demandé
    virtual void OnDraw();

    // Une touche est appuyée
    virtual void OnKeyPressed(unsigned int key);

private:
    /*
     * Variables
     */

    Screen &screen;                  // Écran appelant
    Glib::RefPtr<Gdk::Pixbuf> image; // Image à afficher
    Point pos;                       // Positino de l'image à afficher
};

} // namespace Boxland 

#endif // !BOXLAND_TEXTSCREEN_H
