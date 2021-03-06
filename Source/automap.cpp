//HEADER_GOES_HERE

#include "../types.h"

#include <iostream>
#include <map>
#include <vector>

DEVILUTION_BEGIN_NAMESPACE

// BUGFIX: only the first 256 elements are ever read
WORD automaptype[512];
static int MapX;
static int MapY;
BOOL automapflag;    // idb
char AmShiftTab[32]; // [31]?
unsigned char automapview[DMAXX][DMAXY];
int AutoMapScale;   // idb
int AutoMapXOfs;    // weak
int AutoMapYOfs;    // weak
int AutoMapPosBits; // weak
int AutoMapXPos;    // weak
int AutoMapYPos;    // weak
int AMPlayerX;      // weak
int AMPlayerY;      // weak

// color used to draw the player's arrow
#define COLOR_PLAYER (PAL8_ORANGE + 1)
// color for bright map lines (doors, stairs etc.)
#define COLOR_BRIGHT PAL8_YELLOW
// color for dim map lines/dots
#define COLOR_DIM (PAL16_YELLOW + 8)

#define MAPFLAG_TYPE 0x000F
// these are in the second byte
#define MAPFLAG_VERTDOOR 0x01
#define MAPFLAG_HORZDOOR 0x02
#define MAPFLAG_VERTARCH 0x04
#define MAPFLAG_HORZARCH 0x08
#define MAPFLAG_VERTGRATE 0x10
#define MAPFLAG_HORZGRATE 0x20
#define MAPFLAG_SQUARE 0x40
#define MAPFLAG_STAIRS 0x80

using namespace std;

std::map<std::string, bool> BoolConfig;
std::map<std::string, int> IntConfig; //config variables

void __cdecl InitAutomapOnce()
{
	automapflag = FALSE;
	AutoMapScale = 50;
	AutoMapPosBits = 32;
	AutoMapXPos = 16;
	AutoMapYPos = 8;
	AMPlayerX = 4;
	AMPlayerY = 2;
	AMPlayerY = AutoMapPosBits >> 4;
}

// old ver
/*
void HighlightItemsNameOnMap()
{
	class drawingQueue
	{
	public:
		int ItemID;
		int Row;
		int Col;
		int x;
		int y;
		int new_x = -1;
		int new_y = -1;
		int width;
		int height;
		int magicLevel;
		std::string text;
		drawingQueue(int x2, int y2, int width2, int height2, int Row2, int Col2, int ItemID2, int q2, std::string text2) { x = x2; y = y2; Row = Row2; Col = Col2; ItemID = ItemID2; width = width2; height = height2; magicLevel = q2; text = text2; }
	};
	char textOnGround[256];

	int ScreenHeight = 480;
	int Screen_TopEnd = 160;
	int Screen_LeftBorder = 64;
	int ScreenWidth = 640;


	std::vector<drawingQueue> q;

	//SDL_ShowMessageBox("null",0);
	/*
	if (numitems < 127)
	{
		ii = itemavail[0];
		GetSuperItemSpace(x, y, itemavail[0]);
		itemactive[numitems] = ii;
		itemavail[0] = itemavail[-numitems + 126];
		*//*
	for (int i = 0; i < numitems; i++) {
		//ItemStruct& item = ItemsOnGround[MapItemsFreeIndexes[i + 1]];
		ItemStruct& item_local = item[itemactive[i]];
		int row = item_local._ix - plr[myplr].WorldX;
		int col = item_local._iy - plr[myplr].WorldY;
		// items on ground name highlighting (Qndel)
		if (item_local._itype == ITYPE_GOLD) {
			sprintf(textOnGround, "%i gold", item_local._ivalue);
		}
		else {
			sprintf(textOnGround, "%s", item_local._iIdentified ? item_local._iIName : item_local._iName);
		}
		int x2 = 32 * (row - col);// +(200 * ScrollInfo._sxoff / 100 >> 1) + AutoMapXOfs;
		int y2 = 16 * (row + col);// +(200 * ScrollInfo._syoff / 100 >> 1) + AutoMapYOfs - 16;
		int centerXOffset = GetTextWidth(textOnGround) / 2; // offset to attempt to center the name above the item
		int x = x2;// -96 - centerXOffset;
		int y = y2;
		int x3 = x;// +95;
		int y3 = y - 1;
		//if( x > -Screen_LeftBorder * 2 && x + centerXOffset < ScreenWidth + Screen_LeftBorder && y > -8 && y < ScreenHeight ){
		//if (x > -Screen_LeftBorder * 2 && x3 + centerXOffset < ScreenWidth + Screen_LeftBorder && y3 > 13 && y3 + 13 < ScreenHeight + Screen_TopEnd) {
		if(true){
			// add to drawing queue
			//DrawLevelInfoText( x, y, textOnGround, By( item.MagicLevel, C_0_White, C_1_Blue, C_3_Gold, C_4_Orange) );
			std::string t2(textOnGround);

			q.push_back(drawingQueue(x, y, centerXOffset * 2, 13, item_local._ix, item_local._iy, itemactive[i], item_local._iMagical, t2));
		}
	}
	const int borderX = 5;
	bool highlightItem = false;
	for (unsigned int i = 0; i < q.size(); ++i) {
		if (q[i].new_x == -1 && q[i].new_y == -1) {
			q[i].new_x = q[i].x; q[i].new_y = q[i].y;
		}
		std::map<int, bool> backtrace;
		while (1) {

			bool canShow = true;

			for (unsigned int j = 0; j < i; ++j) {
				if (abs(q[j].new_y - q[i].new_y) < q[i].height + 2) {
					if (q[j].new_x >= q[i].new_x && q[j].new_x - q[i].new_x < q[i].width + borderX) {
						canShow = false;
						int newpos = q[j].new_x - q[i].width - borderX;
						if (backtrace.find(newpos) == backtrace.end()) {
							q[i].new_x = newpos;
							backtrace[newpos] = true;
						}
						else {
							newpos = q[j].new_x + q[j].width + borderX;
							q[i].new_x = newpos;
							backtrace[newpos] = true;
						}
					}
					else if (q[j].new_x < q[i].new_x && q[i].new_x - q[j].new_x < q[j].width + borderX) {
						canShow = false;
						int newpos = q[j].new_x + q[j].width + borderX;;
						if (backtrace.find(newpos) == backtrace.end()) {
							q[i].new_x = newpos;
							backtrace[newpos] = true;
						}
						else {
							newpos = q[j].new_x - q[i].width - borderX;
							q[i].new_x = newpos;
							backtrace[newpos] = true;
						}
					}
				}

			}

			if (canShow) { break; }
		}
	}


	for (unsigned int i = 0; i < q.size(); ++i) {
		drawingQueue t = q[i];
		if (t.new_x == -1 && t.new_y == -1) {
			t.new_x = t.x; t.new_y = t.y;
		}
		int x3 = t.new_x + 95;
		int y3 = t.new_y - 1;
		int bgcolor = 0;
		if(true){
		//if (t.new_x > -Screen_LeftBorder * 2 && x3 + t.width / 2 < ScreenWidth + Screen_LeftBorder && y3 > 13 && y3 + 13 < ScreenHeight + Screen_TopEnd) {

			/*
			int bgcolor = 0;
			int highlightY = t.new_y - 175;
			int highlightX = t.new_x + 30;
			if (CursorX >= highlightX && CursorX <= highlightX + t.width + 1 && CursorY >= highlightY && CursorY <= highlightY + t.height) {
				bgcolor = 134;
				HighlightedItem.ItemID = t.ItemID;
				HighlightedItem.Row = t.Row;
				HighlightedItem.Col = t.Col;
				highlightItem = true;
			}
			*/

/*
			//DrawTransparentBackground(x3, y3, t.width + 1, t.height, 0, 0, bgcolor, bgcolor);
			char color = COL_WHITE;
			//DrawCustomText(t.new_x, t.new_y, 0, &t.text[0u], color);
			int sx = t.new_x + 320 - t.width / 2;
			int sy = t.new_y + 180;

			int sx2 = t.new_x + 383 - t.width / 2;
			int sy2 = t.new_y + 342;


			if (sx < 0 || sx > 640 || sy < 0 || sy > 480) {
				continue;
			}

			if (sx2 < 0 || sx2 > 640 || sy2 < 0 || sy2 > 480) {
				continue;
			}
			DrawTransparentBackground(sx2,sy2, t.width + 1, t.height, 0, 0, bgcolor, bgcolor);
			PrintGameStr(sx,sy,&t.text[0u], color);
			//ADD_PlrStringXY(t.new_x, t.new_y, GetTextWidth(&t.text[0]), &t.text[0u], color);
		}
	}
	/*
	if (highlightItem == false) {
		HighlightedItem.ItemID = -1;
	}
	*/
/*
#ifdef PREVHIGHLIGHT
	char textOnGround[256];
	for (int i = 0; i < CountItemsOnMap; i++) {
		Item& item = ItemsOnGround[MapItemsFreeIndexes[i + 1]];
		int row = item.MapRow - PlayerRowPos;
		int col = item.MapCol - PlayerColPos;

		// items on ground name highlighting (Qndel)
		if (item.ItemCode == IC_11_GOLD) {
			sprintf(textOnGround, "%i gold", item.QualityLevel);
		}
		else {
			sprintf(textOnGround, "%s", item.Identified ? item.FullMagicalItemName : item.Name);
		}
		int x2 = 32 * (row - col) + (200 * (PlayerMovedX + PlayerShiftY) / 100 >> 1) + Xofs;
		int y2 = 16 * (row + col) + (200 * (PlayerMovedY + PlayerShiftX) / 100 >> 1) + Yofs - 16;
		int centerXOffset = GetTextWidth(textOnGround) / 2; // offset to attempt to center the name above the item
															// don't think that red square is needs (there is item outline and blue square already)
															//AutomapDrawOneItem( x2, y2 + item.CelWidth / 8, 155 ); // drawing a red square on the item
		int x = x2 - 64 - centerXOffset;
		int y = y2 - 156;
		if (x > -Screen_LeftBorder * 2 && x + centerXOffset < ScreenWidth + Screen_LeftBorder && y > -8 && y < ScreenHeight) {
			DrawLevelInfoText(x, y, textOnGround, By(item.MagicLevel, C_0_White, C_1_Blue, C_3_Gold, C_4_Orange));
		}
	}
#endif
}
*/

void __cdecl InitAutomap()
{
	unsigned char b1, b2;
	unsigned int dwTiles;
	int x, y;
	unsigned char *pAFile, *pTmp;
	int i, j;
	int d;

	j = 50;

	for (i = 0; i < 31; i++) {
		d = (j << 6) / 100;
		AmShiftTab[i] = 2 * (320 / d) + 1;
		if (320 % d)
			AmShiftTab[i]++;
		if (320 % d >= (j << 5) / 100)
			AmShiftTab[i]++;
		j += 5;
	}

	memset(automaptype, 0, sizeof(automaptype));

	switch (leveltype) {
	case DTYPE_CATHEDRAL:
		pAFile = LoadFileInMem("Levels\\L1Data\\L1.AMP", (int *)&dwTiles);
		dwTiles >>= 1;
		break;
	case DTYPE_CATACOMBS:
		pAFile = LoadFileInMem("Levels\\L2Data\\L2.AMP", (int *)&dwTiles);
		dwTiles >>= 1;
		break;
	case DTYPE_CAVES:
		pAFile = LoadFileInMem("Levels\\L3Data\\L3.AMP", (int *)&dwTiles);
		dwTiles >>= 1;
		break;
	case DTYPE_HELL:
		pAFile = LoadFileInMem("Levels\\L4Data\\L4.AMP", (int *)&dwTiles);
		dwTiles >>= 1;
		break;
	default:
		return;
	}

	pTmp = pAFile;

	for (i = 1; i <= dwTiles; i++) {
		b1 = *pTmp++;
		b2 = *pTmp++;
		automaptype[i] = b1 + (b2 << 8);
	}

	mem_free_dbg(pAFile);
	memset(automapview, 0, sizeof(automapview));

	for (y = 0; y < MAXDUNY; y++) {
		for (x = 0; x < MAXDUNX; x++)
			dFlags[x][y] &= ~DFLAG_EXPLORED;
	}
}

void __cdecl StartAutomap()
{
	AutoMapXOfs = 0;
	AutoMapYOfs = 0;
	automapflag = TRUE;
}

void __cdecl AutomapUp()
{
	--AutoMapXOfs;
	--AutoMapYOfs;
}

void __cdecl AutomapDown()
{
	++AutoMapXOfs;
	++AutoMapYOfs;
}

void __cdecl AutomapLeft()
{
	--AutoMapXOfs;
	++AutoMapYOfs;
}

void __cdecl AutomapRight()
{
	++AutoMapXOfs;
	--AutoMapYOfs;
}

void __cdecl AutomapZoomIn()
{
	if (AutoMapScale < 200) {
		AutoMapScale += 5;
		AutoMapPosBits = (AutoMapScale << 6) / 100;
		AutoMapXPos = AutoMapPosBits >> 1;
		AutoMapYPos = AutoMapXPos >> 1;
		AMPlayerX = AutoMapYPos >> 1;
		AMPlayerY = AMPlayerX >> 1;
	}
}

void __cdecl AutomapZoomOut()
{
	if (AutoMapScale > 50) {
		AutoMapScale -= 5;
		AutoMapPosBits = (AutoMapScale << 6) / 100;
		AutoMapXPos = AutoMapPosBits >> 1;
		AutoMapYPos = AutoMapXPos >> 1;
		AMPlayerX = AutoMapYPos >> 1;
		AMPlayerY = AMPlayerX >> 1;
	}
}

void __cdecl DrawAutomap()
{
	int cells;
	int sx, sy;
	int i, j;
	int mapx, mapy;

	if (leveltype == DTYPE_TOWN) {
		DrawAutomapGame();
		return;
	}

	gpBufEnd = (unsigned char *)&gpBuffer[(352 + 160) * 768];

	MapX = (ViewX - 16) >> 1;
	while (MapX + AutoMapXOfs < 0)
		AutoMapXOfs++;
	while (MapX + AutoMapXOfs >= DMAXX)
		AutoMapXOfs--;
	MapX += AutoMapXOfs;

	MapY = (ViewY - 16) >> 1;
	while (MapY + AutoMapYOfs < 0)
		AutoMapYOfs++;
	while (MapY + AutoMapYOfs >= DMAXY)
		AutoMapYOfs--;
	MapY += AutoMapYOfs;

	cells = AmShiftTab[(AutoMapScale - 50) / 5];
	if (ScrollInfo._sxoff + ScrollInfo._syoff)
		cells++;
	mapx = MapX - cells;
	mapy = MapY - 1;

	if (cells & 1) {
		sx = 384 - AutoMapPosBits * ((cells - 1) >> 1);
		sy = 336 - AutoMapXPos * ((cells + 1) >> 1);
	} else {
		sx = 384 - AutoMapPosBits * (cells >> 1) + AutoMapXPos;
		sy = 336 - AutoMapXPos * (cells >> 1) - AutoMapYPos;
	}
	if (ViewX & 1) {
		sx -= AutoMapYPos;
		sy -= AMPlayerX;
	}
	if (ViewY & 1) {
		sx += AutoMapYPos;
		sy -= AMPlayerX;
	}

	sx += AutoMapScale * ScrollInfo._sxoff / 100 >> 1;
	sy += AutoMapScale * ScrollInfo._syoff / 100 >> 1;
	if (invflag || sbookflag) {
		sx -= 160;
	}
	if (chrflag || questlog) {
		sx += 160;
	}

	for (i = 0; i <= cells + 1; i++) {
		int x = sx;
		int y;

		for (j = 0; j < cells; j++) {
			WORD maptype = GetAutomapType(mapx + j, mapy - j, TRUE);
			if (maptype)
				DrawAutomapType(x, sy, maptype);
			x += AutoMapPosBits;
		}
		mapy++;
		x = sx - AutoMapXPos;
		y = sy + AutoMapYPos;
		for (j = 0; j <= cells; j++) {
			WORD maptype = GetAutomapType(mapx + j, mapy - j, TRUE);
			if (maptype)
				DrawAutomapType(x, y, maptype);
			x += AutoMapPosBits;
		}
		mapx++;
		sy += AutoMapXPos;
	}
	DrawAutomapPlr();
	DrawAutomapGame();
}
// 4B8968: using guessed type int sbookflag;
// 69BD04: using guessed type int questlog;
// 69CF0C: using guessed type int gpBufEnd;

void __fastcall DrawAutomapType(int sx, int sy, WORD automap_type)
{
	BOOL do_vert;
	BOOL do_horz;
	BOOL do_cave_horz;
	BOOL do_cave_vert;
	int x1, y1, x2, y2;

	BYTE flags = automap_type >> 8;

	if (flags & MAPFLAG_SQUARE) {
		ENG_set_pixel(sx, sy, COLOR_DIM);
		ENG_set_pixel(sx - AMPlayerX, sy - AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx - AMPlayerX, sy + AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx + AMPlayerX, sy - AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx + AMPlayerX, sy + AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx - AutoMapYPos, sy, COLOR_DIM);
		ENG_set_pixel(sx + AutoMapYPos, sy, COLOR_DIM);
		ENG_set_pixel(sx, sy - AMPlayerX, COLOR_DIM);
		ENG_set_pixel(sx, sy + AMPlayerX, COLOR_DIM);
		ENG_set_pixel(sx + AMPlayerX - AutoMapXPos, sy + AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx - AMPlayerX + AutoMapXPos, sy + AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx - AutoMapYPos, sy + AMPlayerX, COLOR_DIM);
		ENG_set_pixel(sx + AutoMapYPos, sy + AMPlayerX, COLOR_DIM);
		ENG_set_pixel(sx - AMPlayerX, sy + AutoMapYPos - AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx + AMPlayerX, sy + AutoMapYPos - AMPlayerY, COLOR_DIM);
		ENG_set_pixel(sx, sy + AutoMapYPos, COLOR_DIM);
	}

	if (flags & MAPFLAG_STAIRS) {
		DrawLine(sx - AMPlayerX, sy - AMPlayerX - AMPlayerY, sx + AMPlayerX + AutoMapYPos, sy + AMPlayerY, COLOR_BRIGHT);
		DrawLine(sx - AutoMapYPos, sy - AMPlayerX, sx + AutoMapYPos, sy + AMPlayerX, COLOR_BRIGHT);
		DrawLine(sx - AutoMapYPos - AMPlayerX, sy - AMPlayerY, sx + AMPlayerX, sy + AMPlayerX + AMPlayerY, COLOR_BRIGHT);
		DrawLine(sx - AutoMapXPos, sy, sx, sy + AutoMapYPos, COLOR_BRIGHT);
	}

	do_vert = FALSE;
	do_horz = FALSE;
	do_cave_horz = FALSE;
	do_cave_vert = FALSE;
	switch (automap_type & MAPFLAG_TYPE) {
	case 1: // stand-alone column or other unpassable object
		x1 = sx - AutoMapYPos;
		y1 = sy - AutoMapYPos;
		x2 = x1 + AutoMapXPos;
		y2 = sy - AMPlayerX;
		DrawLine(sx, y1, x1, y2, COLOR_DIM);
		DrawLine(sx, y1, x2, y2, COLOR_DIM);
		DrawLine(sx, sy, x1, y2, COLOR_DIM);
		DrawLine(sx, sy, x2, y2, COLOR_DIM);
		return;
	case 2:
	case 5:
		do_vert = TRUE;
		break;
	case 4:
		do_vert = TRUE;
		do_horz = TRUE;
		break;
	case 3:
	case 6:
		do_horz = TRUE;
		break;
	case 8:
		do_vert = TRUE;
		do_cave_horz = TRUE;
		break;
	case 9:
		do_horz = TRUE;
		do_cave_vert = TRUE;
		break;
	case 10:
		do_cave_horz = TRUE;
		break;
	case 11:
		do_cave_vert = TRUE;
		break;
	case 12:
		do_cave_horz = TRUE;
		do_cave_vert = TRUE;
		break;
	}

	if (do_vert) {                      // right-facing obstacle
		if (flags & MAPFLAG_VERTDOOR) { // two wall segments with a door in the middle
			x1 = sx - AutoMapXPos;
			x2 = sx - AutoMapYPos;
			y1 = sy - AutoMapYPos;
			y2 = sy - AMPlayerX;

			DrawLine(sx, y1, sx - AMPlayerX, y1 + AMPlayerY, COLOR_DIM);
			DrawLine(x1, sy, x1 + AMPlayerX, sy - AMPlayerY, COLOR_DIM);
			DrawLine(x2, y1, x1, y2, COLOR_BRIGHT);
			DrawLine(x2, y1, sx, y2, COLOR_BRIGHT);
			DrawLine(x2, sy, x1, y2, COLOR_BRIGHT);
			DrawLine(x2, sy, sx, y2, COLOR_BRIGHT);
		}
		if (flags & MAPFLAG_VERTGRATE) { // right-facing half-wall
			DrawLine(sx - AutoMapYPos, sy - AMPlayerX, sx - AutoMapXPos, sy, COLOR_DIM);
			flags |= MAPFLAG_VERTARCH;
		}
		if (flags & MAPFLAG_VERTARCH) { // window or passable column
			x1 = sx - AutoMapYPos;
			y1 = sy - AutoMapYPos;
			x2 = x1 + AutoMapXPos;
			y2 = sy - AMPlayerX;

			DrawLine(sx, y1, x1, y2, COLOR_DIM);
			DrawLine(sx, y1, x2, y2, COLOR_DIM);
			DrawLine(sx, sy, x1, y2, COLOR_DIM);
			DrawLine(sx, sy, x2, y2, COLOR_DIM);
		}
		if (!(flags & (MAPFLAG_VERTDOOR | MAPFLAG_VERTGRATE | MAPFLAG_VERTARCH)))
			DrawLine(sx, sy - AutoMapYPos, sx - AutoMapXPos, sy, COLOR_DIM);
	}

	if (do_horz) { // left-facing obstacle
		if (flags & MAPFLAG_HORZDOOR) {
			x1 = sx + AutoMapYPos;
			x2 = sx + AutoMapXPos;
			y1 = sy - AutoMapYPos;
			y2 = sy - AMPlayerX;

			DrawLine(sx, y1, sx + AMPlayerX, y1 + AMPlayerY, COLOR_DIM);
			DrawLine(x2, sy, x2 - AMPlayerX, sy - AMPlayerY, COLOR_DIM);
			DrawLine(x1, y1, sx, y2, COLOR_BRIGHT);
			DrawLine(x1, y1, x2, y2, COLOR_BRIGHT);
			DrawLine(x1, sy, sx, y2, COLOR_BRIGHT);
			DrawLine(x1, sy, x2, y2, COLOR_BRIGHT);
		}
		if (flags & MAPFLAG_HORZGRATE) {
			DrawLine(sx + AutoMapYPos, sy - AMPlayerX, sx + AutoMapXPos, sy, COLOR_DIM);
			flags |= MAPFLAG_HORZARCH;
		}
		if (flags & MAPFLAG_HORZARCH) {
			x1 = sx - AutoMapYPos;
			y1 = sy - AutoMapYPos;
			x2 = x1 + AutoMapXPos;
			y2 = sy - AMPlayerX;

			DrawLine(sx, y1, x1, y2, COLOR_DIM);
			DrawLine(sx, y1, x2, y2, COLOR_DIM);
			DrawLine(sx, sy, x1, y2, COLOR_DIM);
			DrawLine(sx, sy, x2, y2, COLOR_DIM);
		}
		if (!(flags & (MAPFLAG_HORZDOOR | MAPFLAG_HORZGRATE | MAPFLAG_HORZARCH)))
			DrawLine(sx, sy - AutoMapYPos, sx + AutoMapXPos, sy, COLOR_DIM);
	}

	// for caves the horz/vert flags are switched
	if (do_cave_horz) {
		if (flags & MAPFLAG_VERTDOOR) {
			x1 = sx - AutoMapXPos;
			x2 = sx - AutoMapYPos;
			y1 = sy + AutoMapYPos;
			y2 = sy + AMPlayerX;

			DrawLine(sx, y1, sx - AMPlayerX, y1 - AMPlayerY, COLOR_DIM);
			DrawLine(x1, sy, x1 + AMPlayerX, sy + AMPlayerY, COLOR_DIM);
			DrawLine(x2, y1, x1, y2, COLOR_BRIGHT);
			DrawLine(x2, y1, sx, y2, COLOR_BRIGHT);
			DrawLine(x2, sy, x1, y2, COLOR_BRIGHT);
			DrawLine(x2, sy, sx, y2, COLOR_BRIGHT);
		} else
			DrawLine(sx, sy + AutoMapYPos, sx - AutoMapXPos, sy, COLOR_DIM);
	}

	if (do_cave_vert) {
		if (flags & MAPFLAG_HORZDOOR) {
			x1 = sx + AutoMapYPos;
			x2 = sx + AutoMapXPos;
			y1 = sy + AutoMapYPos;
			y2 = sy + AMPlayerX;

			DrawLine(sx, y1, sx + AMPlayerX, y1 - AMPlayerY, COLOR_DIM);
			DrawLine(x2, sy, x2 - AMPlayerX, sy + AMPlayerY, COLOR_DIM);
			DrawLine(x1, y1, sx, y2, COLOR_BRIGHT);
			DrawLine(x1, y1, x2, y2, COLOR_BRIGHT);
			DrawLine(x1, sy, sx, y2, COLOR_BRIGHT);
			DrawLine(x1, sy, x2, y2, COLOR_BRIGHT);
		} else
			DrawLine(sx, sy + AutoMapYPos, sx + AutoMapXPos, sy, COLOR_DIM);
	}
}

void __cdecl DrawAutomapPlr()
{
	int px, py;
	int x, y;

	if (plr[myplr]._pmode == PM_WALK3) {
		x = plr[myplr]._px;
		y = plr[myplr]._py;
		if (plr[myplr]._pdir == DIR_W)
			x++;
		else
			y++;
	} else {
		x = plr[myplr].WorldX;
		y = plr[myplr].WorldY;
	}
	px = x - 2 * AutoMapXOfs - ViewX;
	py = y - 2 * AutoMapYOfs - ViewY;

	x = (plr[myplr]._pxoff * AutoMapScale / 100 >> 1) + (ScrollInfo._sxoff * AutoMapScale / 100 >> 1) + (px - py) * AutoMapYPos + 384;
	y = (plr[myplr]._pyoff * AutoMapScale / 100 >> 1) + (ScrollInfo._syoff * AutoMapScale / 100 >> 1) + (px + py) * AMPlayerX + 336;

	if (invflag || sbookflag)
		x -= 160;
	if (chrflag || questlog)
		x += 160;
	y -= AMPlayerX;

	switch (plr[myplr]._pdir) {
	case DIR_N:
		DrawLine(x, y, x, y - AutoMapYPos, COLOR_PLAYER);
		DrawLine(x, y - AutoMapYPos, x - AMPlayerY, y - AMPlayerX, COLOR_PLAYER);
		DrawLine(x, y - AutoMapYPos, x + AMPlayerY, y - AMPlayerX, COLOR_PLAYER);
		break;
	case DIR_NE:
		DrawLine(x, y, x + AutoMapYPos, y - AMPlayerX, COLOR_PLAYER);
		DrawLine(x + AutoMapYPos, y - AMPlayerX, x + AMPlayerX, y - AMPlayerX, COLOR_PLAYER);
		DrawLine(x + AutoMapYPos, y - AMPlayerX, x + AMPlayerX + AMPlayerY, y, COLOR_PLAYER);
		break;
	case DIR_E:
		DrawLine(x, y, x + AutoMapYPos, y, COLOR_PLAYER);
		DrawLine(x + AutoMapYPos, y, x + AMPlayerX, y - AMPlayerY, COLOR_PLAYER);
		DrawLine(x + AutoMapYPos, y, x + AMPlayerX, y + AMPlayerY, COLOR_PLAYER);
		break;
	case DIR_SE:
		DrawLine(x, y, x + AutoMapYPos, y + AMPlayerX, COLOR_PLAYER);
		DrawLine(x + AutoMapYPos, y + AMPlayerX, x + AMPlayerX + AMPlayerY, y, COLOR_PLAYER);
		DrawLine(x + AutoMapYPos, y + AMPlayerX, x + AMPlayerX, y + AMPlayerX, COLOR_PLAYER);
		break;
	case DIR_S:
		DrawLine(x, y, x, y + AutoMapYPos, COLOR_PLAYER);
		DrawLine(x, y + AutoMapYPos, x + AMPlayerY, y + AMPlayerX, COLOR_PLAYER);
		DrawLine(x, y + AutoMapYPos, x - AMPlayerY, y + AMPlayerX, COLOR_PLAYER);
		break;
	case DIR_SW:
		DrawLine(x, y, x - AutoMapYPos, y + AMPlayerX, COLOR_PLAYER);
		DrawLine(x - AutoMapYPos, y + AMPlayerX, x - AMPlayerY - AMPlayerX, y, COLOR_PLAYER);
		DrawLine(x - AutoMapYPos, y + AMPlayerX, x - AMPlayerX, y + AMPlayerX, COLOR_PLAYER);
		break;
	case DIR_W:
		DrawLine(x, y, x - AutoMapYPos, y, COLOR_PLAYER);
		DrawLine(x - AutoMapYPos, y, x - AMPlayerX, y - AMPlayerY, COLOR_PLAYER);
		DrawLine(x - AutoMapYPos, y, x - AMPlayerX, y + AMPlayerY, COLOR_PLAYER);
		break;
	case DIR_NW:
		DrawLine(x, y, x - AutoMapYPos, y - AMPlayerX, COLOR_PLAYER);
		DrawLine(x - AutoMapYPos, y - AMPlayerX, x - AMPlayerX, y - AMPlayerX, COLOR_PLAYER);
		DrawLine(x - AutoMapYPos, y - AMPlayerX, x - AMPlayerY - AMPlayerX, y, COLOR_PLAYER);
	}
}

WORD __fastcall GetAutomapType(int x, int y, BOOL view)
{
	if (view) {
		if (x == -1 && y >= 0 && y < DMAXY && automapview[0][y])
			return ~GetAutomapType(0, y, FALSE) & (MAPFLAG_SQUARE << 8);
		if (y == -1) {
			if (x < 0)
				return 0;
			if (x < DMAXX && automapview[x][0])
				return ~GetAutomapType(x, 0, FALSE) & (MAPFLAG_SQUARE << 8);
		}
	}

	if (x >= 0 && x < DMAXX && y >= 0 && y < DMAXY) {
		if (automapview[x][y] || !view) {
			WORD type = automaptype[(BYTE)dungeon[x][y]];
			if (type == 7 && GetAutomapType(x - 1, y, FALSE) & (MAPFLAG_HORZARCH << 8)
			    && GetAutomapType(x, y - 1, FALSE) & (MAPFLAG_VERTARCH << 8)) {
				type = 1;
			}
			return type;
		}
	}
	return 0;
}

void __cdecl DrawAutomapGame()
{
	char desc[256];
	int nextline = 20;

	if (gbMaxPlayers > 1) {
		strcat(strcpy(desc, "game: "), szPlayerName);
		PrintGameStr(8, 20, desc, COL_GOLD);
		nextline = 35;
		if (szPlayerDescript[0]) {
			strcat(strcpy(desc, "password: "), szPlayerDescript);
			PrintGameStr(8, 35, desc, COL_GOLD);
			nextline = 50;
		}
	}
	if (setlevel)
		PrintGameStr(8, nextline, quest_level_names[(BYTE)setlvlnum], COL_GOLD);
	else if (currlevel) {
		sprintf(desc, "Level: %i", currlevel);
		PrintGameStr(8, nextline, desc, COL_GOLD);
	}
}

void __fastcall SetAutomapView(int x, int y)
{
	WORD maptype, solid;
	int xx, yy;

	xx = (x - 16) >> 1;
	yy = (y - 16) >> 1;

	if (xx < 0 || xx >= DMAXX || yy < 0 || yy >= DMAXY) {
		return;
	}

	automapview[xx][yy] = 1;

	maptype = GetAutomapType(xx, yy, FALSE);
	solid = maptype & 0x4000;

	switch (maptype & 0xF) {
	case 2:
		if (solid) {
			if (GetAutomapType(xx, yy + 1, FALSE) == 0x4007)
				automapview[xx][yy + 1] = 1;
		} else if (GetAutomapType(xx - 1, yy, FALSE) & 0x4000) {
			automapview[xx - 1][yy] = 1;
		}
		return;
	case 3:
		if (solid) {
			if (GetAutomapType(xx + 1, yy, FALSE) == 0x4007)
				automapview[xx + 1][yy] = 1;
		} else if (GetAutomapType(xx, yy - 1, FALSE) & 0x4000) {
			automapview[xx][yy - 1] = 1;
		}
		return;
	case 4:
		if (solid) {
			if (GetAutomapType(xx, yy + 1, FALSE) == 0x4007)
				automapview[xx][yy + 1] = 1;
			if (GetAutomapType(xx + 1, yy, FALSE) == 0x4007)
				automapview[xx + 1][yy] = 1;
		} else {
			if (GetAutomapType(xx - 1, yy, FALSE) & 0x4000)
				automapview[xx - 1][yy] = 1;
			if (GetAutomapType(xx, yy - 1, FALSE) & 0x4000)
				automapview[xx][yy - 1] = 1;
			if (GetAutomapType(xx - 1, yy - 1, FALSE) & 0x4000)
				automapview[xx - 1][yy - 1] = 1;
		}
		return;
	case 5:
		if (solid) {
			if (GetAutomapType(xx, yy - 1, FALSE) & 0x4000)
				automapview[xx][yy - 1] = 1;
			if (GetAutomapType(xx, yy + 1, FALSE) == 0x4007)
				automapview[xx][yy + 1] = 1;
		} else if (GetAutomapType(xx - 1, yy, FALSE) & 0x4000) {
			automapview[xx - 1][yy] = 1;
		}
		return;
	case 6:
		if (solid) {
			if (GetAutomapType(xx - 1, yy, FALSE) & 0x4000)
				automapview[xx - 1][yy] = 1;
			if (GetAutomapType(xx + 1, yy, FALSE) == 0x4007)
				automapview[xx + 1][yy] = 1;
		} else if (GetAutomapType(xx, yy - 1, FALSE) & 0x4000) {
			automapview[xx][yy - 1] = 1;
		}
		return;
	}
}

void __cdecl AutomapZoomReset()
{
	AutoMapXOfs = 0;
	AutoMapYOfs = 0;
	AutoMapPosBits = (AutoMapScale << 6) / 100;
	AutoMapXPos = AutoMapPosBits >> 1;
	AutoMapYPos = AutoMapXPos >> 1;
	AMPlayerX = AutoMapYPos >> 1;
	AMPlayerY = AMPlayerX >> 1;
}

DEVILUTION_END_NAMESPACE
