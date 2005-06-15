/*
 * ---------------------------------------------------------------------------
 *
 * Boxland : un petit Boxworld
 * Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 * Fichier :     Board.cpp
 *
 * Description : Plateau de jeu gérant cases et personnage
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
#include <fstream>
#include <string>

// Bibliothèque C standard
#include <cstdio>
#include <cstring>

// gtkmm
#include <gdkmm/pixbuf.h>
#include <gdkmm/color.h>
#include <gdk/gdkkeysyms.h>

// Module actuel
#include "config.h"
#include "Character.h"
#include "Menu.h"
#include "Board.h"


/*
 * Implémentation
 */

namespace Boxland
{

/*
 * Caractères représentant le type de case dans les fichiers de niveau
 */
enum {
    CHR_GROUND       = ' ',
    CHR_ENDGROUND    = 'X',
    CHR_WALL         = '#',
    CHR_BOX          = '%',
    CHR_ENDBOX       = '*',
    CHR_CHARACTER    = '$',
    CHR_ENDCHARACTER = '&'
};

/*
 * Constructeur sans chargement de carte
 */
Board::Board(Menu &mainMenu)
    : menu(mainMenu), state(STT_PLAYING), charact(*this), textscr(*this),
      center(ORIGIN_X + (MAX_MAP_WIDTH  * BOX_SIZE) / 2,
	     ORIGIN_Y + (MAX_MAP_HEIGHT * BOX_SIZE) / 2)
{
    // Charge les images
    LoadImages();
}

/*
 * Constructeur avec chargement de carte
 */
Board::Board(Menu &mainMenu, const std::string &fname)
    : menu(mainMenu), state(STT_PLAYING), charact(*this), textscr(*this),
      center(ORIGIN_X + (MAX_MAP_WIDTH  * BOX_SIZE) / 2,
	     ORIGIN_Y + (MAX_MAP_HEIGHT * BOX_SIZE) / 2)
{
    // Charge les images et la carte
    LoadImages();
    LoadMap(fname);
}

/*
 * Desctucteur
 */
Board::~Board()
{}

/*
 * Charge les images
 */
void Board::LoadImages()
{
    // Tampon contenant le nom de fichier de l'image
    char buffer[64];

    // Nom de fichier de chaque image
    const static char *images_names[] = {
	"window",
	"ground",
	"endground",
	"wall",
	"box",
	"endbox",
	"character",
	"pause",
	"endlevel",
	"endgame"
    };

    // Charge les images une par une
    for (unsigned int i = IMG_FIRST; i < IMG_COUNT; i++) {
	sprintf(buffer, "images/%s.png", images_names[i]);
	images[i] = Gdk::Pixbuf::create_from_file(buffer);
    }
}

/*
 * Charge une carte
 */
bool Board::LoadMap(const std::string &fname)
{
    // Ouvre le fichier
    std::ifstream file;
    file.open(fname.c_str());
    if (!file.good())
	return false;

    // Initialise les variables
    filename = fname;
    memset(isBox, 0, sizeof(isBox));
    width = 0;
    height = 0;
    remain = 0;
    moves = 0;
    time = 0;

    // Initialise les variables locales de position et de caractère
    unsigned int x = 0, y = 0;
    int c;

    // Lit chaque caractère tant qu'on se trouve dans les limites de la carte
    while (y < MAX_MAP_HEIGHT && (c = file.get()) != EOF) {
	// Objet de classe Point représentant la position actuelle
	Point pos(x, y);

	switch (c) {
	case CHR_GROUND:
	    // Sol normal
	    map[y][x].Set(pos, Ground::GROUND_NORMAL, IMG_GROUND);
	    break;

	case CHR_ENDGROUND:
	    // Sol devant contenir une boîte à la fin du niveau
	    map[y][x].Set(pos, Ground::GROUND_END, IMG_ENDGROUND);
	    break;

	case CHR_WALL:
	    // Mur
	    map[y][x].Set(pos, Ground::GROUND_WALL, IMG_WALL);
	    break;

	case CHR_BOX:
	    // Sol normal contenant une boîte
	    map[y][x].Set(pos, Ground::GROUND_NORMAL, IMG_GROUND);
	    isBox[y][x] = true;
	    remain++;
	    break;

	case CHR_ENDBOX:
	    // Sol devant contenir une boîte, et en contenant une
	    map[y][x].Set(pos, Ground::GROUND_END, IMG_ENDGROUND);
	    isBox[y][x] = true;
	    break;

	case CHR_CHARACTER:
	    // Sol normal sur lequel se trouve le personnage
	    map[y][x].Set(pos, Ground::GROUND_NORMAL, IMG_GROUND);
	    charact.SetPosition(pos);
	    break;

	case CHR_ENDCHARACTER:
	    // Sol devant contenir une boîte où se trouve le personnage
	    map[y][x].Set(pos, Ground::GROUND_END, IMG_GROUND);
	    charact.SetPosition(pos);
	    break;

	case '\r':
	    // Avant '\n' sur Windows
	    continue;

	default:
	    // Par défaut, la case est vide
	    map[y][x].Clear();
	}

	if (x < MAX_MAP_WIDTH - 1) {
	    // On n'est pas encore à la fin de la ligne
	    if (c != '\n')
		// Caractère suivant
		x++;
	    else {
		// Ligne suivante

		// Calcul de la largeur
		if (x > width)
		    width = x;

		//remplit le reste par des cases vides
		while (x < MAX_MAP_WIDTH)
		    map[y][x++].Clear();

		// Passe à la ligne suivante
		x = 0;
		y++;
	    }
	} else {
	    // Calcul de la largeur
	    width = MAX_MAP_WIDTH;

	    // Passe les autres caractères de cette ligne
	    do
		c = file.get();
	    while (c != '\n' && c != EOF);

	    // Passe à la ligne suivante
	    if (c == '\n') {
		x = 0;
		y++;
	    }
	}
    }

    // Ferme le fichier
    file.close();

    // Calcul de la hauteur (-1 si on est au début de la ligne suivante)
    if (y > 0 && x == 0)
	y--;
    height = y + 1;

    // Calcul le coin supérieur gauche du plateau de jeu
    origin.SetPoint(ORIGIN_X + (MAX_MAP_WIDTH  - width ) * BOX_SIZE / 2,
		    ORIGIN_Y + (MAX_MAP_HEIGHT - height) * BOX_SIZE / 2);

    // Initialise l'état
    state = STT_PLAYING;
    return true;
}

/*
 * Affiche le sol et l'éventuelle boîte s'y trouvant si drawBoxes == true
 */
void Board::DrawBox(const Point &pos, bool drawBoxes) const
{
    // Affiche la case (sol)
    map[pos.y][pos.x].Draw(*this);

    // Affiche l'éventuelle boîte s'y trouvant
    if (drawBoxes && IsBox(pos)) {
	const Glib::RefPtr<Gdk::Pixbuf> image = map[pos.y][pos.x].IsEnd() ?
	    images[IMG_ENDBOX] : images[IMG_BOX];
	DrawImage(image, Point(origin.x + pos.x * BOX_SIZE,
			       origin.y + pos.y * BOX_SIZE));
    }
}

/*
 * Affiche les informations relatives au niveau en cours
 */
void Board::DrawInformations()
{
    // Efface les informations graphiquement
    SetColor(black);
    DrawRectangle(Point(241, 9), Point(62, 22));
    DrawRectangle(Point(441, 9), Point(53, 22));
    DrawRectangle(Point(569, 9), Point(62, 22));
    SetColor(green);

    // Affiche les informations
    char buffer[24];
    sprintf(buffer, "%u/%u", menu.GetLevelIndex() + 1, menu.GetLevelCount());
    DrawText(Point(244, 12), buffer);
    sprintf(buffer, "%d", moves);
    DrawText(Point(444, 12), buffer);
    sprintf(buffer, "%u s", time);
    DrawText(Point(572, 12), buffer);
}

/*
 * Initialisation
 */
void Board::OnInit()
{
    // Crée les couleurs
    black = Gdk::Color("black");
    green = Gdk::Color("green");
    AllocColor(black);
    AllocColor(green);
}

/*
 * Affichage
 */
void Board::OnDraw()
{
    // Affiche le fond de la fenêtre
    DrawImage(images[IMG_BACKGROUND], Point(0, 0));

    // Affiche chaque case
    for (unsigned y = 0; y < height; y++)
	for (unsigned x = 0; x < width; x++)
	    DrawBox(Point(x, y));

    // Affiche le personnage et les informations de niveau
    charact.Draw();
    DrawInformations();
}

/*
 * Appui d'une touche
 */
void Board::OnKeyPressed(unsigned int key)
{
    switch (state) {
    case STT_PLAYING:
	// On est en train de jouer

	// Direction de déplacement du personnage
	Character::Direction dir;

	switch (key) {
	case GDK_Left:
	case GDK_KP_Left:
	case GDK_KP_4:
	    // On se déplace vers la gauche
	    dir = Character::DIR_LEFT;
	    break;

	case GDK_Right:
	case GDK_KP_Right:
	case GDK_KP_6:
	    // On se déplace vers la droite
	    dir = Character::DIR_RIGHT;
	    break;

	case GDK_Up:
	case GDK_KP_Up:
	case GDK_KP_8:
	    // On se déplace vers le haut
	    dir = Character::DIR_UP;
	    break;

	case GDK_Down:
	case GDK_KP_Down:
	case GDK_KP_2:
	    // On se déplace vers le bas
	    dir = Character::DIR_DOWN;
	    break;

	case GDK_R:
	case GDK_r:
	    // Recommence le niveau
	    timer.disconnect();
	    LoadMap(filename);
	    OnDraw();
	    return;

	case GDK_BackSpace:
	case GDK_U:
	case GDK_u:
	    // Annule le dernier mouvement
	    if (charact.Undo()) {
		moves--;
		DrawInformations();
	    }
	    return;

	case GDK_space:
	case GDK_P:
	case GDK_p:
	    // Pause
	    charact.BeginPause();

	    // Affiche le texte de pause
	    timer.disconnect();
	    state = STT_PAUSED;
	    textscr.SetImage(images[IMG_PAUSE], center);
	    Switch(&textscr);
	    return;

	case GDK_Escape:
	case GDK_Q:
	case GDK_q:
	    // Retourne au menu
	    timer.disconnect();
	    Return();
	default:
	    // Par défaut, on ne fait rien
	    return;
	}

	if (charact.Move(dir)) {
	    // Si on se déplace pour la première fois, on démarre le timer
	    if (moves++ == 0)
		timer = Glib::signal_timeout()
		    .connect(sigc::mem_fun(*this, &Board::Timeout), 1000);

	    // Affiche les informations de niveau
	    DrawInformations();
	}
	break;

    case STT_PAUSED:
	// On est en pause

	// Si la bonne touche est appuyée, on sort de la pause
	if (key == GDK_space || key == GDK_P || key == GDK_p) {
	    textscr.Terminate();
	    state = STT_PLAYING;

	    // On réenclenche le timer
	    if (moves > 0)
		timer = Glib::signal_timeout()
		    .connect(sigc::mem_fun(*this, &Board::Timeout), 1000);
	    charact.EndPause();
	}
	break;

    case STT_WAITING:
	// On ne fait rien du tout en situation d'attente
	break;
    }
}

/*
 * Vérifie si le niveau est terminé
 */
void Board::CheckRemaining()
{
    // Si le niveau est terminé (plus de boîte restantes à placer)
    if (remain == 0) {
	// Désactive le timer du temps et met à jour l'état
	timer.disconnect();
	state = STT_WAITING;

	// Affiche un écran de texte
	textscr.SetImage(images[!menu.IsLastLevel() ?
				IMG_ENDLEVEL : IMG_ENDGAME], center);
	Switch(&textscr);
	timer = Glib::signal_timeout()
	    .connect(sigc::mem_fun(*this, &Board::EndTimeout), 3000);
    }
}

/*
 * Timeout utilisé pour incrémenter le temps
 */
bool Board::Timeout()
{
    if (remain != 0) {
	// Incrémente le temps et affiche les informations
	time++;
	DrawInformations();
	return true;
    }

    // Désactive le timer
    return false;
}

/*
 * Timeout utilisé pour afficher un texte un certain temps
 */
bool Board::EndTimeout()
{
    // Met fin à l'écran de test
    textscr.Terminate();

    if (menu.NextLevel())
	// Niveau suivant : on l'affiche
	OnDraw();
    else
	// Plus de niveau (fin du jeu) : on retourne au menu
	Return();

    // Désactive le timer
    return false;
}

} // namespace Boxland
