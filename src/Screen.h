/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Screen.h
 *
 * Description : Écran de jeu (en-tête)
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


// Protectino
#ifndef BOXLAND_SCREEN_H
#define BOXLAND_SCREEN_H

/*
 * En-têtes
 */

// gtkmm
#include <gdkmm/pixbuf.h>

// Module actuel
#include "WindowArea.h"

// Déclaration anticipée
namespace Gdk
{
    class Color;
}


namespace Boxland
{

// Déclaration anticipée
class Point;

/*
 * Classe Screen : classe virtuelle utilisée pour implémenter les différents
 * écrans au long du jeu afin de faciliter les opérations courantes
 */
class Screen
{
public:
    /*
     * Constructeur et destructeur
     */

    // Constructeur par défaut
    explicit Screen() : area(NULL) {}

    // Destructeur (classe virtuelle)
    virtual ~Screen() = 0;


    /*
     * Méthodes de déclenchement d'événements (inline)
     */

    // Déclenche l'affichage de l'écran
    void Draw() { OnDraw(); }

    // Fait réagir l'écran à un appui de touche
    void PressKey(unsigned int key) { OnKeyPressed(key); }

protected:
    /*
     * Méthodes de transitino entre les écrans (inline)
     */

    // Donne la main à un autre écran
    void Switch(Screen *screen) const { if (area) area->Switch(screen); }

    // Retourne à l'écran précédent
    void Return() const { if (area) area->Return(); }


    /*
     * Méthodes de dessin courantes
     */

    // Efface l'écran (le remplit avec la couleur par défaut)
    void Clear() const { if (area) area->Clear(); }

    // Alloue une couleur dans la table des couleurs de l'application
    void AllocColor(Gdk::Color &color) const
	{ if (area) area->AllocColor(color); }

    // Sélectione une couleur pour les opérations suivantes
    void SetColor(Gdk::Color &color) const
	{ if (area) area->SetColor(color); }

    // Affiche une image
    void DrawImage(Glib::RefPtr<Gdk::Pixbuf> image, const Point &pos) const
	{ if (area) area->DrawImage(image, pos); }

    // Dessine un rectangle plein
    void DrawRectangle(const Point &pos, const Point &dim) const
	{ if (area) area->DrawRectangle(pos, dim); }

    // Affiche du texte
    void DrawText(const Point &pos, const char *text) const
	{ if (area) area->DrawText(pos, text); }


    /*
     * Méthodes virtuelles utilisées pour les événements
     */

    // L'écran a été attaché à une zone de fenêtre (WindowArea)
    virtual void OnInit();

    // L'écran a été sélectionné pour l'affichage
    virtual void OnEnter();

    // L'écran est quitté pour passer la pain à un autre
    virtual void OnLeave();

    // L'écran doit être réaffiché
    virtual void OnDraw();

    // Une touche a été appuyée
    virtual void OnKeyPressed(unsigned int key);

private:
    // La classe WindowArea est autorisée à accéder à ces fonctions
    friend class WindowArea;


    /*
     * Rattachement à ue zone de fenêtre
     */

    // Zone de fenêtre associée
    WindowArea *area;

    // Méthode appelée lorsque l'écran est rattaché à une zone de fenêtre
    void SetWindowArea(WindowArea *warea) { area = warea; }
};

} // namespace Boxland

#endif /* !BOXLAND_SCREEN_H */
