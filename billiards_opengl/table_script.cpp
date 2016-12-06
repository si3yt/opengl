#include "billiards_header.h"

/* èâä˙âª */
Table::Table() {
	table_init();
}

Table::~Table() {}

void Table::table_init() {
	//top
	box_propaty[0].width = _FLOOR_WIDTH;
	box_propaty[0].height = _WALL_SIZE;
	box_propaty[0].depth = _WALL_SIZE;
	box_propaty[0].center = const_cast<GLdouble*>(_WALL_TOP_POS);
	box_propaty[0].color = const_cast<GLfloat*>(_RED);
	//bottom
	box_propaty[1].width = _FLOOR_WIDTH;
	box_propaty[1].height = _WALL_SIZE;
	box_propaty[1].depth = _WALL_SIZE;
	box_propaty[1].center = const_cast<GLdouble*>(_WALL_BOTTOM_POS);
	box_propaty[1].color = const_cast<GLfloat*>(_RED);
	//left
	box_propaty[2].width = _WALL_SIZE;
	box_propaty[2].height = _FLOOR_HEIGHT + _WALL_SIZE * 2;
	box_propaty[2].depth = _WALL_SIZE;
	box_propaty[2].center = const_cast<GLdouble*>(_WALL_LEFT_POS);
	box_propaty[2].color = const_cast<GLfloat*>(_RED);
	//right
	box_propaty[3].width = _WALL_SIZE;
	box_propaty[3].height = _FLOOR_HEIGHT + _WALL_SIZE * 2;
	box_propaty[3].depth = _WALL_SIZE;
	box_propaty[3].center = const_cast<GLdouble*>(_WALL_RIGHT_POS);
	box_propaty[3].color = const_cast<GLfloat*>(_RED);

	//bottom_floor
	box_propaty[4].width = _FLOOR_WIDTH;
	box_propaty[4].height = _FLOOR_HEIGHT;
	box_propaty[4].depth = _WALL_SIZE / 2;
	box_propaty[4].center = const_cast<GLdouble*>(_BOTTOM_FLOOR_POS);
	box_propaty[4].color = const_cast<GLfloat*>(_BLACK);

	for (size_t i = 0; i < array_length(table_parts); i++) {
		Box box(box_propaty[i].width, box_propaty[i].height, box_propaty[i].depth, box_propaty[i].center, box_propaty[i].color);
		table_parts[i] = box;
	}
}
/* Ç±Ç±Ç‹Ç≈èâä˙âª */

void Table::draw() {
	glDisable(GL_LIGHTING);
	for (size_t i = 0; i < array_length(table_parts); i++) {
		table_parts[i].draw();
	}
	table_floor.draw();
	glEnable(GL_LIGHTING);
	
}

