/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Ground.cpp
 *
 * Description : Case du plateau de jeu
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
#include "Board.h"
#include "Ground.h"


/*
 * Implémentation
 */

namespace Boxland
{

/*
 * Dessine la case
 */
void Ground::Draw(const Board &board) const
{
    // Dessine uniquement si la case n'est pas vide
    if (type != GROUND_NONE)
	board.DrawBoxImage(image, pos);
}

} // namespace Boxland
