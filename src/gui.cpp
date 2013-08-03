///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//	This is OurUO Tech Demo, for more info on this project, please refer to:         //
//	                                                                                 //
// 	https://github.com/OurUO/ouruo-server                                            //
// 		                                                                             //
// 	Copyright (C) 2013  Developer Riker (https://github.com/dev-riker)               //
// 	Copyright (C) 2013  Developer Sebastien (https://github.com/devsebastien)        //
//                                                                                   //
// 	This program is free software; you can redistribute it and/or                    //
// 	modify it under the terms of the GNU General Public License                      //
// 	as published by the Free Software Foundation; either version 2                   //
// 	of the License, or (at your option) any later version.                           //
//                                                                                   //
// 	This program is distributed in the hope that it will be useful,                  //
// 	but WITHOUT ANY WARRANTY; without even the implied warranty of                   //
// 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    //
// 	GNU General Public License for more details.                                     //
//                                                                                   //
// 	You should have received a copy of the GNU General Public License                //
// 	along with this program; if not, write to the Free Software                      //
// 	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include "main.hpp"

static const int32_t PANEL_HEIGHT	= 7;
static const int32_t BAR_WIDTH		= 20;
static const int32_t MSG_X			= BAR_WIDTH + 2;
static const int32_t MSG_HEIGHT		= PANEL_HEIGHT - 1;

Gui::Gui()
{
	con_ = new TCODConsole(engine.screenWidth_, PANEL_HEIGHT);
}

Gui::~Gui()
{
	delete con_;
	Clear();
}

void Gui::Clear()
{
	log_.clearAndDelete();
}

void Gui::Render()
{
	// clear the GUI console
	con_->setDefaultBackground(TCODColor::black);
	con_->clear();

	// draw the health bar
	RenderBar(1, 1, BAR_WIDTH, "HP", engine.player_->destructible_->hp_, engine.player_->destructible_->maxHp_, TCODColor::lightRed, TCODColor::darkerRed);

	// draw the message log
	int32_t y = 1;
	float colorCoef = 0.4f;

	for (message *msg : log_) {
		con_->setDefaultForeground(msg->col * colorCoef);
		con_->print(MSG_X, y, msg->text);
		y++;
		if (colorCoef < 1.0f) {
			colorCoef += 0.3f;
		}
	}

	// mouse look
	RenderMouseLook();

	// blit the GUI console on the root console
	TCODConsole::blit(con_, 0, 0, engine.screenWidth_, PANEL_HEIGHT, TCODConsole::root, 0, engine.screenHeight_ - PANEL_HEIGHT);
}

void Gui::RenderBar(int32_t x, int32_t y, int32_t width, const char *name, float value, float maxValue, const TCODColor &barColor, const TCODColor &backColor)
{
	// fill the background
	con_->setDefaultBackground(backColor);
	con_->rect(x, y, width, 1, false, TCOD_BKGND_SET);

	int32_t barWidth = (int32_t) (value / maxValue * width);
	if (barWidth > 0) {
		// draw the bar
		con_->setDefaultBackground(barColor);
		con_->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
	}
	// print text on top of the bar
	con_->setDefaultForeground(TCODColor::white);
	con_->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name, value, maxValue);
}

Gui::message::message(const char *text, const TCODColor &col) :
	text(strdup(text)), col(col)
{
}

Gui::message::~message()
{
	free(text);
}

void Gui::RenderMouseLook()
{
	if (!engine.map_->IsInFov(engine.mouse_.cx, engine.mouse_.cy)) {
		// if mouse is out of fov, nothing to render
		return;
	}
	char buf[128] = "";
	bool first = true;

	for (Actor *actor : engine.actors_) {
		// find actors under the mouse cursor
		if ((actor->x_ == engine.mouse_.cx) && (actor->y_ == engine.mouse_.cy)) {
			if (!first) {
				strcat(buf, ", ");
			} else {
				first = false;
			}
			strcat(buf, actor->name_);
		}
	}
	// display the list of actors under the mouse cursor
	con_->setDefaultForeground(TCODColor::lightGrey);
	con_->print(1, 0, buf);
}

void Gui::Message(const TCODColor &col, const char *text, ...)
{
	// build the text
	va_list ap;
	char buf[128];
	va_start(ap, text);
	vsprintf(buf, text, ap);
	va_end(ap);

	char *lineBegin = buf;
	char *lineEnd;
	do {
		// make room for the new message
		if (log_.size() == MSG_HEIGHT) {
			message *toRemove = log_.get(0);
			log_.remove(toRemove);
			delete toRemove;
		}

		// detect end of the line
		lineEnd = strchr(lineBegin, '\n');
		if ( lineEnd ) {
			*lineEnd = '\0';
		}

		// add a new message to the log
		message *msg = new message(lineBegin, col);
		log_.push(msg);

		// go to next line
		lineBegin = lineEnd + 1;
	}
	while (lineEnd);
}

Menu::~Menu()
{
	Clear();
}

void Menu::Clear()
{
	items_.clearAndDelete();
}

void Menu::AddItem(MenuItemCode code, const char *label)
{
	MenuItem *item = new MenuItem();
	item->code = code;
	item->label = label;
	items_.push(item);
}

Menu::MenuItemCode Menu::Pick()
{
	static TCODImage img("menu_background1.png");
	int32_t selectedItem = 0;
	while(!TCODConsole::isWindowClosed()) {
		img.blit2x(TCODConsole::root, 0, 0);
		int32_t currentItem = 0;

		for (MenuItem *it : items_) {
			if (currentItem == selectedItem) {
				TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
			} else {
				TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);
			}
			TCODConsole::root->print(10, 10 + currentItem * 3, it->label);
			currentItem++;
		}
		TCODConsole::flush();

		// check key presses
		TCOD_key_t key;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
		switch (key.vk) {
		case TCODK_UP:
			selectedItem--; 
			if (selectedItem < 0) {
				selectedItem = items_.size() - 1;
			}
		break;
		case TCODK_DOWN: 
			selectedItem = (selectedItem + 1) % items_.size(); 
		break;
		case TCODK_ENTER: 
			return items_.get(selectedItem)->code;
			break;
		default:
			break;
		}
	}
	return NONE;
}
