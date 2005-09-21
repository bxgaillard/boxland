/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Point.h
 *
 * Description : Coordonnées à deux dimensions (en-tête)
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
#ifndef BOXLAND_POINT_H
#define BOXLAND_POINT_H

namespace Boxland
{

/*
 * Classe Point
 */
class Point
{
public:
    // Coordonnées en deux dimensions
    int x, y;

    // Constructeurs
    explicit Point()               : x(0),   y(0)   {}
    explicit Point(int pxy)        : x(pxy), y(pxy) {}
    explicit Point(int px, int py) : x(px),  y(py)  {}

    // Affecter des coordonnées au point
    void SetPoint(int px, int py) { x = px, y = py; }

    // Déplacer le point
    void Move(int mx, int my) { x += mx, y += my; }

    // Opérateurs arithmétiques
    Point operator +(const Point &p) const { return Point(x + p.x, y + p.y); }
    Point operator -(const Point &p) const { return Point(x - p.x, y - p.y); }
    Point operator *(const int i)    const { return Point(x * i, y * i); }

    // Opérateurs arithmétiques et affectation
    Point &operator +=(const Point &p) { x += p.x, y += p.y; return *this; }
    Point &operator -=(const Point &p) { x -= p.x, y -= p.y; return *this; }
    Point &operator *=(const int i)    { x *= i; y *= i; return *this; }

    // Opérateurs de comparaison
    bool operator ==(const Point &p) const { return x == p.x && y == p.y; }
    bool operator !=(const Point &p) const { return !(*this == p); }
};

} // namespace Boxland

#endif // !BOXLAND_POINT_H
