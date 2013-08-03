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
#include <math.h>
#include "main.hpp"

#ifdef WIN32
inline double round(double x) { return floor(x + 0.5); }
#endif

// how many turns the monster chases the player
// after losing his sight
static const int32_t TRACKING_TURNS = 3;

MonsterAi::MonsterAi() : moveCount_(0)
{
}

void MonsterAi::Update(Actor *owner)
{
    if ((owner->destructible_) && (owner->destructible_->IsDead())) {
    	return;
    }
	if (engine.map_->IsInFov(owner->x_, owner->y_)) {
    	// we can see the player. move towards him
    	moveCount_ = TRACKING_TURNS;
    } else {
    	moveCount_--;
    }
   	if (moveCount_ > 0) {
   		MoveOrAttack(owner, engine.player_->x_, engine.player_->y_);
   	}
}

void MonsterAi::MoveOrAttack(Actor *owner, int32_t targetx, int32_t targety)
{
	int32_t dx = targetx - owner->x_;
	int32_t dy = targety - owner->y_;
	int32_t stepdx = (dx > 0 ? 1 : -1);
	int32_t stepdy = (dy > 0 ? 1 : -1);
	float distance = sqrtf((float) (dx * dx + dy * dy));

	if (distance >= 2) {
		dx = (int32_t) (round(dx / distance));
		dy = (int32_t) (round(dy / distance));
		if (engine.map_->CanWalk(owner->x_ + dx, owner->y_ + dy)) {
			owner->x_ += dx;
			owner->y_ += dy;
		} else if (engine.map_->CanWalk(owner->x_ + stepdx, owner->y_)) {
			owner->x_ += stepdx;
		} else if (engine.map_->CanWalk(owner->x_, owner->y_ + stepdy)) {
			owner->y_ += stepdy;
		}
	} else if (owner->attacker_) {
		owner->attacker_->Attack(owner, engine.player_);
	}
}

ConfusedMonsterAi::ConfusedMonsterAi(int nbTurns, Ai *oldAi) 
	: nbTurns_(nbTurns), oldAi_(oldAi)
{
}

void ConfusedMonsterAi::Update(Actor *owner)
{
	TCODRandom *rng = TCODRandom::getInstance();
	int32_t dx = rng->getInt(-1, 1);
	int32_t dy = rng->getInt(-1, 1);
	if ((dx != 0) || (dy != 0)) {
		int32_t destx = owner->x_ + dx;
		int32_t desty = owner->y_ + dy;
		if (engine.map_->CanWalk(destx, desty)) {
			owner->x_ = destx;
			owner->y_ = desty;
		} else {
			Actor *actor = engine.GetActor(destx, desty);
			if (actor) {
				owner->attacker_->Attack(owner, actor);
			}
		}
	}
	nbTurns_--;
	if (nbTurns_ == 0) {
		owner->ai_ = oldAi_;
		delete this;
	}
}

void PlayerAi::Update(Actor *owner)
{
    if ((owner->destructible_) && (owner->destructible_->IsDead())) {
    	return;
    }

    int32_t dx = 0, dy = 0;

    switch(engine.lastKey_.vk) {
	case TCODK_UP:
		dy = -1;
		break;
	case TCODK_DOWN:
		dy = 1;
		break;
	case TCODK_LEFT:
		dx = -1;
		break;
	case TCODK_RIGHT:
		dx = 1;
		break;
	case TCODK_CHAR:
		HandleActionKey(owner, engine.lastKey_.c);
		break;
    default:
		break;
    }
    if ((dx != 0) || (dy != 0)) {
    	engine.gameStatus = Engine::NEW_TURN;
    	if (MoveOrAttack(owner, owner->x_ + dx, owner->y_ + dy)) {
    		engine.map_->ComputeFov();
    	}
	}
}

bool PlayerAi::MoveOrAttack(Actor *owner, int32_t targetx, int32_t targety)
{
	if (engine.map_->IsWall(targetx, targety)) {
		return false;
	}
	// look for living actors to attack
	for (Actor *actor : engine.actors_) {
		if ((actor->destructible_) && (!actor->destructible_->IsDead()) && (actor->x_ == targetx) && (actor->y_ == targety)) {
			owner->attacker_->Attack(owner, actor);
			return false;
		}
	}

	// look for corpses or items
	for (Actor *actor : engine.actors_) {
		bool corpseOrItem = ((actor->destructible_ && actor->destructible_->IsDead()) || actor->pickable_);

		if ((corpseOrItem) && (actor->x_ == targetx) && (actor->y_ == targety)) {
			engine.gui_->Message(TCODColor::lightGrey, "Qui c\'e\' un(a) %s.", actor->name_);
		}
	}
	owner->x_ = targetx;
	owner->y_ = targety;

	return true;
}

void PlayerAi::HandleActionKey(Actor *owner, int32_t ascii)
{
	switch(ascii) {
	case 'd': { // drop item
		Actor *actor = ChooseFromInventory(owner);
		if (actor) {
			actor->pickable_->Drop(actor, owner);
			engine.gameStatus = Engine::NEW_TURN;
		}			
	}
	break;
	case 'g': { // pickup item
		bool found = false;

		for (Actor *actor : engine.actors_) {
			if ((actor->pickable_) && (actor->x_ == owner->x_) && (actor->y_ == owner->y_)) {
				if (actor->pickable_->Pick(actor,owner)) {
					found = true;
					engine.gui_->Message(TCODColor::lightGrey,"Raccogli un %s.", actor->name_);
					break;
				} else if (!found) {
					found = true;
					engine.gui_->Message(TCODColor::red, "Il tuo inventario e\' pieno.");
				}
			}
		}
		if (!found) {
			engine.gui_->Message(TCODColor::lightGrey, "Non c\'e\' nulla che possa essere raccolto.");
		}
		engine.gameStatus = Engine::NEW_TURN;
	}
	break;
	case 'i': { // display inventory
		Actor *actor = ChooseFromInventory(owner);
		if (actor) {
			actor->pickable_->Use(actor, owner);
			engine.gameStatus = Engine::NEW_TURN;
		}
	}
	break;
	case 'c': { // Character Sheet
		DisplayCharacterSheet(owner);
	}
	break;
	}
}

Actor *PlayerAi::ChooseFromInventory(Actor *owner)
{
	static const int32_t INVENTORY_WIDTH = 50;
	static const int32_t INVENTORY_HEIGHT = 28;
	static TCODConsole con(INVENTORY_WIDTH, INVENTORY_HEIGHT);

	// display the inventory frame
	con.setDefaultForeground(TCODColor(200, 180, 50));
	con.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Inventario");

	// display the items with their keyboard shortcut
	con.setDefaultForeground(TCODColor::white);
	int32_t shortcut='a';
	int32_t y = 1;

	for (Actor *actor : owner->container_->inventory) {
		con.print(2, y, "(%c) %s", shortcut, actor->name_);
		y++;
		shortcut++;
	}

	// blit the inventory console on the root console
	TCODConsole::blit(&con, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, TCODConsole::root, engine.screenWidth_ / 2 - INVENTORY_WIDTH / 2, engine.screenHeight_ / 2 - INVENTORY_HEIGHT / 2);
	TCODConsole::flush();

	// wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
	if (key.vk == TCODK_CHAR) {
		int actorIndex=key.c - 'a';
		if ((actorIndex >= 0) && (actorIndex < owner->container_->inventory.size())) {
			return owner->container_->inventory.get(actorIndex);
		}
	}
	return NULL;
}

void PlayerAi::DisplayCharacterSheet(Actor *owner)
{
	static const int32_t CHARACTER_SHEET_WIDTH = 50;
	static const int32_t CHARACTER_SHEET_HEIGHT = 28;
	static TCODConsole con(CHARACTER_SHEET_WIDTH, CHARACTER_SHEET_HEIGHT);

	// display the inventory frame
	con.setDefaultForeground(TCODColor(200, 180, 50));
	con.printFrame(0, 0, CHARACTER_SHEET_WIDTH, CHARACTER_SHEET_HEIGHT, true, TCOD_BKGND_DEFAULT, "Scheda del Personaggio");

	// display the items with their keyboard shortcut
	int currRow = 1;

	// Skills
	std::map <std::string, Skill *>::iterator iter;

	con.setDefaultForeground(TCODColor::darkRed);
	con.print(2, currRow, "Major Skills");
	con.setDefaultForeground(TCODColor::white);
	for (iter = owner->destructible_->mobileClass_.skills_.majorSkills_.begin(); iter != owner->destructible_->mobileClass_.skills_.majorSkills_.end(); ++iter) {
		currRow++;
		con.print(2, currRow, "%s:", iter->second->GetName());
		if (iter->second->GetCurrValue() > iter->second->GetBaseValue()) {
			con.setDefaultForeground(TCODColor::green);
		} else {
			if (iter->second->GetCurrValue() < iter->second->GetBaseValue()) {
				con.setDefaultForeground(TCODColor::red);
			}
		}
		con.print(2 + 13, currRow, "%3d", iter->second->GetCurrValue());
		con.setDefaultForeground(TCODColor::white);
	}

	currRow += 2;
	con.setDefaultForeground(TCODColor::darkRed);
	con.print(2, currRow, "Minor Skills");
	con.setDefaultForeground(TCODColor::white);

	for (iter = owner->destructible_->mobileClass_.skills_.minorSkills_.begin(); iter != owner->destructible_->mobileClass_.skills_.minorSkills_.end(); ++iter) {
		currRow++;
		con.print(2, currRow, "%s:", iter->second->GetName());
		if (iter->second->GetCurrValue() > iter->second->GetBaseValue()) {
			con.setDefaultForeground(TCODColor::green);
		} else {
			if (iter->second->GetCurrValue() < iter->second->GetBaseValue()) {
				con.setDefaultForeground(TCODColor::red);
			}
		}
		con.print(2 + 13, currRow, "%3d", iter->second->GetCurrValue());
		con.setDefaultForeground(TCODColor::white);
	}

	// Attributes
	currRow = 1;

	con.setDefaultForeground(TCODColor::darkRed);
	con.print(25, currRow, "Basic Attributes");
	con.setDefaultForeground(TCODColor::white);

	currRow += 2;
	con.setDefaultForeground(TCODColor::darkRed);
	con.print(25, currRow, "Derived Attributes");
	con.setDefaultForeground(TCODColor::white);


	// blit the inventory console on the root console
	TCODConsole::blit(&con, 0, 0, CHARACTER_SHEET_WIDTH, CHARACTER_SHEET_HEIGHT, TCODConsole::root, engine.screenWidth_ / 2 - CHARACTER_SHEET_WIDTH / 2, engine.screenHeight_ / 2 - CHARACTER_SHEET_HEIGHT / 2);
	TCODConsole::flush();

	// wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
	if (key.vk == TCODK_CHAR) {
	}
}
