/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Character.h
 *
 * Description : Gestion du personnage contrôlable (en-tête)
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
#ifndef BOXLAND_CHARACTER_H
#define BOXLAND_CHARACTER_H

/*
 * En-têtes
 */

// Module actuel
#include "Point.h"


namespace Boxland
{

// Déclaration anticipée
class Board;

/*
 * Classe Character : gère le mouvement et l'affichage du personnage
 */
class Character
{
public:
    /*
     * Type de données
     */

    // Énumération des quatre direction
    enum Direction {
	DIR_NONE = 0, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_COUNT
    };


    /*
     * Constructeur et destructeur
     */

    // Constructeur
    explicit Character(Board &brd, const Point &pos = Point(0, 0));

    // Destructeur
    virtual ~Character();


    /*
     * Méthodes relatives à la position
     */

    // Donne la position courante
    const Point &GetPosition() const { return pos; }

    // Fixe la position du personnage
    void SetPosition(const Point &mpos) { pos = mpos; moveDir = DIR_NONE; }


    /*
     * Actions
     */

    // Dessine le personnage
    void Draw();

    // Débute un mouvement avec animation
    bool Move(Direction dir);

    // Annule le dernier mouvement
    bool Undo();


    /*
     * Gestion de la pause
     */

    // Débute la pause
    void BeginPause();

    // Termine la pause
    void EndPause();

private:
    /*
     * Type de données
     */

    // Vitesse de déplacements des mobiles, en pixels par 10 ms
    enum { MOVE_SPEED = 4 };


    /*
     * Variables
     */

    // Les vecteurs pour les quatre directions
    static const Point dirs[DIR_COUNT];

    Board &board;      // Référence vers le plateau de jeu
    Point pos, offset; // Position et décalage sur le plateau

    bool moving, undoing; // État du déplacement et de l'annulation
    Direction moveDir;    // Direction du mouvement
    Point moveVector;     // Vecteur pour le mouvement (direction * vitesse)

    sigc::connection timer;  // Timer utilisé pour le mouvement
    unsigned int moveRemain; // Nombre d'étapes de mouvement restantes

    bool moveBox;          // Indique si une boîte est déplacée en même temps
    unsigned int boxImage; // Image correspondante à la boîte


    /*
     * Méthodes
     */

    // Méthode de timeout utilisée pour le mouvement
    bool Timeout();
};

} // namespace Boxland 

#endif // !BOXLAND_CHARACTER_H
