/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Character.cpp
 *
 * Description : Gestion du personnage contrôlable
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

// Module actuel
#include "Board.h"
#include "Character.h"


/*
 * Implémentation
 */

namespace Boxland
{

/*
 * Vecteurs correspondant aux quatre directions
 */
const Point Character::dirs[DIR_COUNT] = {
    Point(0, 0), Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)
};

/*
 * Constructeur
 */
Character::Character(Board &brd, const Point &mpos)
    : board(brd), pos(mpos), offset(0, 0),
      moving(false), undoing(false), moveDir(DIR_NONE)
{}

/*
 * Destructeur
 */
Character::~Character()
{}

/*
 * Affiche le personnage sur le plateau de jeu
 */
void Character::Draw()
{
    // Affiche la case du dessous (sol)
    board.DrawBox(pos, false);

    // Si l'on est en cours de mouvement
    if (offset != Point(0, 0)) {
	// Affiche aussi la case de destination du mouvement (sol)
	board.DrawBox(pos + dirs[moveDir], false);

	// En cas de déplacement de boîte
	if (moveBox) {
	    Point boxPos, boxPos2, &boxPos3 = boxPos2;

	    // Suivant si c'est une annulation, l'ordre du départ et de
	    // l'arrivée n'est pas le même
	    if (!undoing) {
		boxPos = pos + dirs[moveDir];
		boxPos2 = boxPos + dirs[moveDir];
	    } else {
		boxPos = pos - dirs[moveDir];
		boxPos2 = pos;
		boxPos3 = boxPos;
	    }

	    // Si on est à la moitié du mouvement, on change le graphisme de
	    // la boîte, basé sur la case d'arrivée
	    if (moveRemain == Board::BOX_SIZE / MOVE_SPEED / 2 ||
		moveRemain == (Board::BOX_SIZE / MOVE_SPEED + 1) / 2)
		boxImage = board.IsEnd(boxPos2) ?
		    Board::IMG_ENDBOX : Board::IMG_BOX;

	    // Finalement, affiche le sol et la boîte
	    board.DrawBox(boxPos3, false);
	    board.DrawBoxImage(boxImage, boxPos, offset);
	}
    }

    // Affiche l'image du personnage
    board.DrawBoxImage(Board::IMG_CHARACTER, pos, offset);
}

/*
 * Débute un mouvement avec animation
 */
bool Character::Move(Direction dir)
{
    // Si l'on effectue déjà un mouvement, retourne immédiatement
    if (moving)
	return false;

    // Case de destination
    Point first = pos + dirs[dir];

    // Pas de mouvement par défaut
    bool doMove = false;

    // Si on reste dans le cadre de la carte et que la case est du sol (pas un
    // mur ou du vide)
    if (first.x >= 0 && first.x < (int) board.GetWidth() &&
	first.y >= 0 && first.y < (int) board.GetHeight() &&
	board.IsWalkable(first)) {
	if (board.IsBox(first)) {
	    // Une boîte se trouve devant nous

	    // Case située après la boîte dans la direction du mouvement
	    Point second = first + dirs[dir];

	    // Si cette dernière est située dans le cadre de la carte et si
	    // c'est du sol ne contenant pas de boîte
	    if (second.x >= 0 && second.x < (int) board.GetWidth() &&
		second.y >= 0 && second.y < (int) board.GetHeight() &&
		board.IsWalkable(second) && !board.IsBox(second)) {
		// Déplace la boîte
		board.RemoveBox(first);
		board.AddBox(second);

		// On effectue le mouvement
		doMove = true;
		moveBox = true;
	    }
	} else {
	    // Pas de boîte, le mouvement s'effectue
	    doMove = true;
	    moveBox = false;
	}
    }

    // Si on effectue le mouvement
    if (doMove) {
	// Initialise les variables correspondantes
	moving = true;
	moveDir = dir;
	moveVector = dirs[dir] * MOVE_SPEED;
	moveRemain = Board::BOX_SIZE / MOVE_SPEED;

	// Dans le cas d'un déplacement de boîte, détermine son image
	if (moveBox)
	    boxImage = board.IsEnd(first) ?
		Board::IMG_ENDBOX : Board::IMG_BOX;

	// Met en place le timer
	if (Timeout())
	    timer = Glib::signal_timeout()
		.connect(sigc::mem_fun(*this, &Character::Timeout), 10);
    }

    return doMove;
}

/*
 * Annule le dernier mouvement
 */
bool Character::Undo()
{
    // Directions opposées
    static const Direction opposite[DIR_COUNT] = {
	DIR_NONE, DIR_RIGHT, DIR_LEFT, DIR_DOWN, DIR_UP
    };

    // Si on n'est pas déjà en mouvement et que le mouvement est annulable
    if (!moving && moveDir != DIR_NONE) {
	// Initialise les variables correspondantes
	undoing = true;
	moving = true;
	moveDir = opposite[moveDir];
	moveVector = dirs[moveDir] * MOVE_SPEED;
	moveRemain = Board::BOX_SIZE / MOVE_SPEED;

	// Dans le cas d'un déplacement avec boîte, on la déplace sur le
	// plateau
	if (moveBox) {
	    board.RemoveBox(pos - dirs[moveDir]);
	    board.AddBox(pos);
	}

	// Met et place le timer
	if (Timeout())
	    timer = Glib::signal_timeout()
		.connect(sigc::mem_fun(*this, &Character::Timeout), 10);

	return true;
    }

    return false;
}

/*
 * Débute une pause
 */
void Character::BeginPause()
{
    // Arrête le timer si on effectue un mouvement
    if (moving)
	timer.disconnect();
}

/*
 * Termine une pause
 */
void Character::EndPause()
{
    // Relance le timer si on effectue un mouvement
    if (moving && Timeout())
	timer = Glib::signal_timeout()
	    .connect(sigc::mem_fun(*this, &Character::Timeout), 10);
}

/*
 * Méthode de timeout utilisée pour le mouvement
 */
bool Character::Timeout()
{
    // S'il y a encore des étapes à effectuer
    if (moveRemain > 0) {
	// Met à jour le décalage et affiche le tout
	moveRemain--;
	offset += moveVector;
	Draw();

	// Continue le timer
	return true;
    }

    // Si on s'est déplacé
    if (moving) {
	// On effectue le vrai déplacement sur le plateau et affichage
	pos += dirs[moveDir];
	offset.SetPoint(0, 0);
	Draw();
    }

    // On affiche la boîte si on en déplaçait une
    if (moveBox)
	board.DrawBox(!undoing ? pos + dirs[moveDir] : pos - dirs[moveDir]);

    // En cas d'annulation, désactive une seconde annulation
    if (undoing) {
	moveDir = DIR_NONE;
	undoing = false;
    }

    // On ne se déplace plus
    moving = false;

    // Vérifie si le niveau est terminé
    board.CheckRemaining();

    // Stoppe le timer
    return false;
}

} // namespace Boxland
