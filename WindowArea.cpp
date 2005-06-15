/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     WindowArea.cpp
 *
 * Description : Zone de fenêtre utilisée pour afficher des écrans
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
#include <vector>

// gtkmm
#include <gdkmm/window.h>
#include <gdkmm/gc.h>
#include <gdkmm/color.h>
#include <gdkmm/pixbuf.h>
#include <gdk/gdkevents.h>

// Module actuel
#include "Point.h"
#include "Screen.h"
#include "WindowArea.h"


/*
 * Implémentation
 */

namespace Boxland
{

/*
 * Constructeur
 */
WindowArea::WindowArea(Screen *screen)
    : ready(false)
{
    // Règle les paramètres du contrôle graphique
    set_flags(Gtk::CAN_FOCUS);
    set_events(Gdk::BUTTON_PRESS_MASK);

    // Initialise le premier écran s'il est donné
    if (screen != NULL) {
	screens.push_back(screen);
	screen->SetWindowArea(this);
    }
}

/*
 * Destructeur
 */
WindowArea::~WindowArea()
{
    // Quitte l'écran actuel
    if (screens.size() > 0)
	screens.back()->OnLeave();
}

/*
 * Passe à un nouvel écran
 */
void WindowArea::Switch(Screen *screen)
{
    // Quitte l'écran actuel s'il existe
    if (screens.size() > 0)
	screens.back()->OnLeave();

    // Si l'écran est valide
    if (screen != NULL) {
	// L'ajoute à la liste et l'initialise
	screens.push_back(screen);
	screen->SetWindowArea(this);

	// Si possible, on l'active
	if (ready) {
	    screen->OnInit();
	    screen->OnEnter();
	    screen->OnDraw();
	}
    }
}

/*
 * Retourne à l'écran précédent
 */
void WindowArea::Return()
{
    // S'il y a un écran
    if (screens.size() > 0) {
	// On le quitte
	screens.back()->OnLeave();
	screens.pop_back();

	// Et on active le précédent
	if (screens.size() > 0)
	    screens.back()->OnEnter();
	    screens.back()->OnDraw();
    }
}

/*
 * Les contrôles graphiques peuvent être initialisés
 */
void WindowArea::on_realize()
{
    // Appel de l'événement du la classe parente (nécessaire)
    Gtk::DrawingArea::on_realize();

    // Initialise les variables
    window = get_window();
    gc = Gdk::GC::create(window);
    cm = get_colormap();

    // Paramètre le widget
    set_size_request(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialise les écrans
    if (screens.size() > 0) {
	for (std::vector<Screen *>::iterator iter = screens.begin();
	     iter != screens.end(); ++iter) {
	    (*iter)->SetWindowArea(this);
	    (*iter)->OnInit();
	}

	// Affiche le dernier
	screens.back()->OnEnter();
	screens.back()->OnDraw();
    }

    // On est prêts
    ready = true;
}

/*
 * Le système demande un redessin de la zone
 */
bool WindowArea::on_expose_event(GdkEventExpose *)
{
    // Affiche le dernier écran s'il existe
    if (ready && screens.size() > 0)
	screens.back()->OnDraw();
    return true;
}

/*
 * Une touche est appuyée
 */
bool WindowArea::on_key_press_event(GdkEventKey *event)
{
    // Appelle le gestionnaire du dernier écran s'il existe
    if (ready && screens.size() > 0)
	screens.back()->OnKeyPressed(event->keyval);
    return true;
}

/*
 * Efface la fenêtre avec la couleur par défaut
 */
void WindowArea::Clear() const
{
    // Effectue l'effacement
    window->clear();
}

/*
 * Alloue une couleur dans la table des couleurs de l'application
 */
void WindowArea::AllocColor(Gdk::Color &color) const
{
    // Alloue la couleur
    cm->alloc_color(color);
}

/*
 * Utilise une couleur pour les méthodes suivantes
 */
void WindowArea::SetColor(Gdk::Color &color) const
{
    // Sélectionne cette couleur comme couleur d'avant-plan
    gc->set_foreground(color);
}

/*
 * Affiche une image
 */
void WindowArea::DrawImage(const Glib::RefPtr<Gdk::Pixbuf> image,
			   const Point &pos) const
{
    // Affiche l'image
    window->draw_pixbuf(gc, image, 0, 0, pos.x, pos.y,
			image->get_width(), image->get_height(),
			Gdk::RGB_DITHER_NORMAL, 0, 0);
}

/*
 * Dessine un rectangle plein
 */
void WindowArea::DrawRectangle(const Point &pos, const Point &dim) const
{
    // Dessine le rectangle
    window->draw_rectangle(gc, true, pos.x, pos.y, dim.x, dim.y);
}

/*
 * Affiche du texte
 */
void WindowArea::DrawText(const Point &pos, const char *text)
{
    // Affiche le texte
    window->draw_layout(gc, pos.x, pos.y, create_pango_layout(text));
}

} // namespace Boxland
