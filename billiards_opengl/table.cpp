#include "billiards.h"

/** initialization **/
Table::Table() {
	table_init();
}

Table::~Table() {}

void Table::table_init() {
	//wall of top
	box_propaty[0].width = _FLOOR_WIDTH;
	box_propaty[0].height = _WALL_SIZE;
	box_propaty[0].depth = _WALL_SIZE * (3.0/2.0);
	box_propaty[0].center = const_cast<GLdouble*>(_WALL_TOP_POS);
	box_propaty[0].color = const_cast<GLfloat*>(_BROWN);
	//wall of bottom
	box_propaty[1].width = _FLOOR_WIDTH;
	box_propaty[1].height = _WALL_SIZE;
	box_propaty[1].depth = _WALL_SIZE * (3.0 / 2.0);
	box_propaty[1].center = const_cast<GLdouble*>(_WALL_BOTTOM_POS);
	box_propaty[1].color = const_cast<GLfloat*>(_BROWN);
	//wall of left
	box_propaty[2].width = _WALL_SIZE;
	box_propaty[2].height = _FLOOR_HEIGHT;
	box_propaty[2].depth = _WALL_SIZE * (3.0 / 2.0);
	box_propaty[2].center = const_cast<GLdouble*>(_WALL_LEFT_POS);
	box_propaty[2].color = const_cast<GLfloat*>(_BROWN);
	//wall of right
	box_propaty[3].width = _WALL_SIZE;
	box_propaty[3].height = _FLOOR_HEIGHT;
	box_propaty[3].depth = _WALL_SIZE * (3.0 / 2.0);
	box_propaty[3].center = const_cast<GLdouble*>(_WALL_RIGHT_POS);
	box_propaty[3].color = const_cast<GLfloat*>(_BROWN);

	//delete floor
	box_propaty[4].width = _FLOOR_WIDTH + _WALL_SIZE * 2;
	box_propaty[4].height = _FLOOR_HEIGHT + _WALL_SIZE * 2;
	box_propaty[4].depth = _WALL_SIZE / 2;
	box_propaty[4].center = const_cast<GLdouble*>(_BOTTOM_FLOOR_POS);
	box_propaty[4].color = const_cast<GLfloat*>(_BLACK);

	//set box
	for (size_t i = 0; i < 5; i++) {
		Box box(box_propaty[i].width, box_propaty[i].height, box_propaty[i].depth, box_propaty[i].center, box_propaty[i].color);
		table_parts[i] = box;
	}
	
	//set trapezoid vertexes
	for (size_t i = 0; i < 8; i++) {
		trapezoid_propaty[0].vertex[i] = const_cast<GLdouble*>(_TRAPEZOID_TOP_VERTEX[i]);
		trapezoid_propaty[1].vertex[i] = const_cast<GLdouble*>(_TRAPEZOID_BOTTOM_VERTEX[i]);
		trapezoid_propaty[2].vertex[i] = const_cast<GLdouble*>(_TRAPEZOID_TL_VERTEX[i]);
		trapezoid_propaty[3].vertex[i] = const_cast<GLdouble*>(_TRAPEZOID_BL_VERTEX[i]);
		trapezoid_propaty[4].vertex[i] = const_cast<GLdouble*>(_TRAPEZOID_TR_VERTEX[i]);
		trapezoid_propaty[5].vertex[i] = const_cast<GLdouble*>(_TRAPEZOID_BR_VERTEX[i]);
		trapezoid_propaty[6].vertex[i] = const_cast<GLdouble*>(_CORNER_TL_VERTEX[i]);
		trapezoid_propaty[7].vertex[i] = const_cast<GLdouble*>(_CORNER_BL_VERTEX[i]);
		trapezoid_propaty[8].vertex[i] = const_cast<GLdouble*>(_CORNER_TR_VERTEX[i]);
		trapezoid_propaty[9].vertex[i] = const_cast<GLdouble*>(_CORNER_BR_VERTEX[i]);
		trapezoid_propaty[10].vertex[i] = const_cast<GLdouble*>(_FLOOR_CENTER_VERTEX[i]);
		trapezoid_propaty[11].vertex[i] = const_cast<GLdouble*>(_FLOOR_TL_VERTEX[i]);
		trapezoid_propaty[12].vertex[i] = const_cast<GLdouble*>(_FLOOR_BL_VERTEX[i]);
		trapezoid_propaty[13].vertex[i] = const_cast<GLdouble*>(_FLOOR_TR_VERTEX[i]);
		trapezoid_propaty[14].vertex[i] = const_cast<GLdouble*>(_FLOOR_BR_VERTEX[i]);
	}
	//set trapezoid to box
	for (size_t i = 0; i < array_length(trapezoid_propaty); i++) {
		if (i < 6) trapezoid_propaty[i].color = const_cast<GLfloat*>(_LIGHT_BROWN);
		else if (i < 10) trapezoid_propaty[i].color = const_cast<GLfloat*>(_BROWN);
		else trapezoid_propaty[i].color = const_cast<GLfloat*>(_GREEN);
		Box box(trapezoid_propaty[i].vertex, trapezoid_propaty[i].color);
		table_parts[5 + i] = box;
	}
}
/** so far initialization **/

void Table::draw() {
	for (size_t i = 0; i < array_length(table_parts); i++) table_parts[i].draw();
}

