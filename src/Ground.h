/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Ground.h
 *
 * Description : Case du plateau de jeu (en-tête)
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
#ifndef BOXLAND_GROUND_H
#define BOXLAND_GROUND_H

namespace Boxland
{

// Déclaratino anticipée
class Board;

/*
 * Classe Ground : stocke les informations d'une case du plateau et permet de
 * l'afficher.
 */
class Ground
{
public:
    /*
     * Type de données
     */

    // Type de case
    enum GroundType { GROUND_NONE, GROUND_NORMAL, GROUND_END, GROUND_WALL };


    /*
     * Constructeurs (inline)
     */

    // Constructeur par défaut
    explicit Ground() : type(GROUND_NONE) {}

    // Constructeur avec sélection initial de type de case
    explicit Ground(const Point &bpos, GroundType gtype, unsigned int img)
	: pos(bpos), type(gtype), image(img) {}


    /*
     * Méthodes d'initialisation de la case (inline)
     */

    // Définit les caractéristiques de la case
    void Set(const Point &bpos, GroundType gtype, unsigned int img)
	{ pos = bpos; type = gtype; image = img; }

    // Définiti la case comme étant vide
    void Clear() { type = GROUND_NONE; }


    /*
     * Méthodes d'obtention d'informations sur la case (inline)
     */

    // Obtient le type de terrain
    GroundType GetType() const { return type; }

    // Permet de savoir s'il s'agit d'une case où le personnage peut se placer
    bool IsWalkable() const
	{ return type == GROUND_NORMAL || type == GROUND_END; }

    // Permet de savoir si la case doit contenir une boîte en fin de niveau
    bool IsEnd() const { return type == GROUND_END; }


    /*
     * Méthodes d'affichage
     */

    void Draw(const Board &board) const;

private:
    /*
     * Données relatives à la case
     */

    Point pos;          // Position de la case dans le plateau
    GroundType type;    // Type de case (sol)
    unsigned int image; // Image représentant ce type de case
};

} // namespace Boxland

#endif // !BOXLAND_GROUND_H
