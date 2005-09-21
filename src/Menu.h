/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Menu.h
 *
 * Description : Écran de menu (en-tête)
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
#ifndef BOXLAND_MENU_H
#define BOXLAND_MENU_H

/*
 * En-têtes
 */

// gtkmm
#include <gdkmm/pixbuf.h>
#include <gdkmm/color.h>

// Module actuel
#include "Screen.h"
#include "Board.h"


namespace Boxland
{

// Déclaration anticipée
class Board;

/*
 * Classe Menu : affiche un menu à l'utilisateur lui permettant de
 * sélectionner un niveau ainsi que de voir les touches utilisées
 */
class Menu : public Screen
{
public:
    /*
     * Constructeur et destructeur
     */

    // Constructeur
    Menu();

    // Destructeur
    virtual ~Menu();


    /*
     * Méthodes de gestion des niveaux
     */

    // Obtient le nombre de nivaeux (inline)
    unsigned int GetLevelCount() const { return levels.size(); }

    // Obtient le numéro du niveau actuel (commençant à 0) (inline)
    unsigned int GetLevelIndex() const { return level; }

    // Indique si le niveau actuel est le dernier (inline)
    bool IsLastLevel() const { return level == levels.size() - 1; }

    // Passe au niveau suivant
    bool NextLevel();
        
protected:
    /*
     * Gestionnaires d'événements
     */

    // Initialisatino (attachement à la zone de fenêtre
    virtual void OnInit();

    // Affichage
    virtual void OnDraw();

    // Appui d'une touche
    virtual void OnKeyPressed(unsigned int key);

private:
    /*
     * Variables
     */

    std::vector<std::string> levels;        // Liste de niveaux
    unsigned                 level;         // Niveau courant
    unsigned int            *moves, *times; // Scores des niveaux

    Board                     board;        // Plateau de jeu
    Glib::RefPtr<Gdk::Pixbuf> window;       // Image de fond
    Gdk::Color                black, green; // Couleurs


    /*
     * Méthodes
     */

    // Charge la liste des nivaeux
    bool LoadLevels();

    // Affiche les informations relatives au niveau sélectionné
    void DrawInformations();
};

} // namespace Boxland 

#endif /* !BOXLAND_MENU_H */
