/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Board.h
 *
 * Description : Plateau de jeu gérant cases et personnage (en-tête)
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
#ifndef BOXLAND_BOARD_H
#define BOXLAND_BOARD_H

/*
 * En-têtes
 */

// STL
#include <string>
#include <vector>

// gtkmm
#include <gdkmm/pixbuf.h>
#include <gdkmm/color.h>

// Module actuel
#include "Screen.h"
#include "Point.h"
#include "Ground.h"
#include "Character.h"
#include "TextScreen.h"


namespace Boxland
{

// Déclaration anticipée
class Menu;

/*
 * Classe Board : contient le plateau de jeu, gère l'affichage des cases et du
 * personnage et intercepte les évéments de clavier pour contrôler le
 * personnage
 */
class Board : public Screen
{
public:
    /*
     * Types de données
     */

    // Différentes constantes en rapport avec le plateau de jeu
    enum { BOX_SIZE = 32, MAX_MAP_WIDTH = 19, MAX_MAP_HEIGHT = 13,
	   ORIGIN_X = 16, ORIGIN_Y = 48 };

    // État du jeu
    enum State { STT_PLAYING, STT_PAUSED, STT_WAITING };

    // Numéro d'image chargée
    enum BoxImage {
	IMG_FIRST = 0,      // Début du tableau
	IMG_BACKGROUND = 0, // Image de fond
	IMG_GROUND,         // Sol
	IMG_ENDGROUND,      // Sol de destination de boîte
	IMG_WALL,           // Mur
	IMG_BOX,            // Boîte
	IMG_ENDBOX,         // Boîte arrivée à destination
	IMG_CHARACTER,      // Personnage
	IMG_PAUSE,          // Texte de pause
	IMG_ENDLEVEL,       // Texte de fin de niveau
	IMG_ENDGAME,        // Texte de fin de jeu
	IMG_COUNT           // Fin du tableau
    };


    /*
     * Constructeurs et destructeur
     */

    // Constructeur sans chargement de carte
    explicit Board(Menu &mainMenu);

    // Constructeur avec chargement de carte
    explicit Board(Menu &mainMenu, const std::string &fname);

    // Destructeur
    virtual ~Board();


    /*
     * Accesseurs
     */

    // Obtient le nombre de mouvements
    unsigned int GetMoveCount() const { return moves;  }

    // Obtient le temps passé
    unsigned int GetTime()      const { return time;   }

    // Obtient les coordonnées d'origine du plateau
    const Point &GetOrigin()    const { return origin; }

    // Obtient la largeur du plateau
    unsigned int GetWidth()     const { return width;  }

    // Obtient la hauteur du plateau
    unsigned int GetHeight()    const { return height; }


    /*
     * Méthodes d'obtention d'informations sur une case
     */

    // Obtient je type de sol
    Ground::GroundType GetBox(const Point &pos) const
	{ return map[pos.y][pos.x].GetType(); }

    // Permet de savoir si le personnage peut se trouver sur cette case
    bool IsWalkable(const Point &pos) const
	{ return map[pos.y][pos.x].IsWalkable(); }

    // Permet de savoir si cette case doit contenir une boîte en fin de niveau
    bool IsEnd(const Point &pos) const { return map[pos.y][pos.x].IsEnd(); }

    // Permet de savoir si cette case comporte une boîte
    bool IsBox(const Point &pos) const { return isBox[pos.y][pos.x]; }

    // Supprime la boîte se trouvant à cette position
    void RemoveBox(const Point &pos)
	{ isBox[pos.y][pos.x] = false; if (IsEnd(pos)) remain++; }

    // Ajoute une boîte à cette position
    void AddBox   (const Point &pos)
	{ isBox[pos.y][pos.x] = true;  if (IsEnd(pos)) remain--; }


    /*
     * Méthodes d'affichage des cases
     */

    // Affiche le sol et l'éventuelle boîte s'y trouvant si drawBoxes == true
    void DrawBox(const Point &pos, bool drawBoxes = true) const;

    // Affiche une image à l'emplacement d'une case (numéro d'image)
    void DrawBoxImage(unsigned int image, const Point &pos,
		      const Point &offset = Point(0, 0)) const
	{ DrawBoxImage(images[image], pos, offset); }

    // Affiche une image à l'emplacement d'une case (image elle-même)
    void DrawBoxImage(const Glib::RefPtr<Gdk::Pixbuf> image, const Point &pos,
		      const Point &offset = Point(0, 0)) const
	{ DrawImage(image, origin + pos * BOX_SIZE + offset); }


    /*
     * Méthodes de gestion de la carte
     */

    // Charge la carte
    bool LoadMap(const std::string &fname);

    // Vérifie si le niveau est terminé
    void CheckRemaining();

protected:
    /*
     * Gestionnaires d'événements
     */

    // Initialisatino (rattachement à une zone de fenêtre
    virtual void OnInit();

    // Affichage
    virtual void OnDraw();

    // Appui sur une touche
    virtual void OnKeyPressed(unsigned int key);

private:
    /*
     * Variables
     */

    Menu &menu;           // Référence vers l'objet de la classe Menu
    std::string filename; // Nom du fichier contenant la carte chargée

    Ground map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH]; // Sols de la carte
    bool isBox[MAX_MAP_HEIGHT][MAX_MAP_WIDTH]; // Boîtes sur la carte
    unsigned int width, height;                // Dimensions de la carte
    Point origin;                              // Point supérieur gauche

    unsigned int remain;    // Nombre de boîtes restant à placer
    unsigned int time;      // Temps écoulé
    sigc::connection timer; // Timer pour le temps écoulé

    State        state;   // État du jeu
    unsigned int moves;   // Nombre de mouvements
    Character    charact; // Personnage contrôlable par l'utilisateur

    Gdk::Color black, green;                     // Couleurs
    Glib::RefPtr<Gdk::Pixbuf> images[IMG_COUNT]; // Images chargées

    TextScreen textscr; // Écran d'affichage du texte
    const Point center; // Point central du plateau de jeu


    /*
     * Méthodes
     */

    // Charge les images utilisées par cette classe
    void LoadImages();

    // Affiche les informations de niveau, temps et mouvements
    void DrawInformations();


    /*
     * Gestionnaires de timer
     */

    // Timeout utilisé pour incrémenter le temps
    bool Timeout();

    // Timeout utilisé pour afficher un texte un certain temps
    bool EndTimeout();
};

} // namespace Boxland

#endif // !BOXLAND_BOARD_H
