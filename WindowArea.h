/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     WindowArea.h
 *
 * Description : Zone de fenêtre utilisée pour afficher des écrans (en-tête)
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
#ifndef BOXLAND_WINDOWAREA_H
#define BOXLAND_WINDOWAREA_H

// STL
#include <vector>

// gtkmm
#include <gtkmm/drawingarea.h>
#include <gdkmm/window.h>
#include <gdkmm/gc.h>
#include <gdkmm/pixbuf.h>
#include <gdk/gdkevents.h>

// Déclaration anticipée
namespace Gdk
{
    class Color;
}


namespace Boxland
{

// Déclarations anticipées
class Point;
class Screen;

/*
 * Classe WindowArea : cette classe, utilisée comme un widget gtkmm, gère
 * l'affichage successif de différents écrans ainsi que les transitions entre
 * eux
 */
class WindowArea : public Gtk::DrawingArea
{
public:
    /*
     * Type de données
     */

    // Dimensions du contrôle graphique
    enum { WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480 };


    /*
     * Constructeur est destructeur
     */

    // Constructeur
    WindowArea(Screen *screen = NULL);

    // Destructeur
    virtual ~WindowArea();


    /*
     * Méthode de transition entre écrans
     */

    // Passe à un nouvel écran
    void Switch(Screen *screen);

    // Retourne à l'écran précédent
    void Return();


    /*
     * Gestionnaires d'événements gtkmm
     */

    // Les contrôles graphiques peuvent être initialisés
    virtual void on_realize();

    // Le système demande un redessin de la zone
    virtual bool on_expose_event(GdkEventExpose *event);

    // Une touche est appuyée
    virtual bool on_key_press_event(GdkEventKey *event);

protected:
    // La classe Screen a accès à ces méthodes
    friend class Screen;


    /*
     * Méthodes de dessin
     */

    // Efface la fenêtre avec la couleur par défaut
    void Clear() const;

    // Alloue une couleur dans la table des couleurs de l'application
    void AllocColor(Gdk::Color &color) const;

    // Utilise une couleur pour les méthodes suivantes
    void SetColor(Gdk::Color &color) const;

    // Affiche une image
    void DrawImage(const Glib::RefPtr<Gdk::Pixbuf> image, const Point &pos)
	const;

    // Dessine un rectangle plein
    void DrawRectangle(const Point &pos, const Point &dim) const;

    // Affiche du texte
    void DrawText(const Point &pos, const char *text);

private:
    /*
     * Variables
     */

    bool                  ready;   // La zone a été initialisée
    std::vector<Screen *> screens; // Pile des écrans

    Glib::RefPtr<Gdk::Window>   window; // Fenêtre dans laquelle dessiner
    Glib::RefPtr<Gdk::GC>       gc;     // Contexte graphique
    Glib::RefPtr<Gdk::Colormap> cm;     // Table des couleurs
};

} // namespace Boxland

#endif // !BOXLAND_WINDOWAREA_H
