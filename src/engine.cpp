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

#include <math.h>
#include "main.hpp"

Engine::Engine(int32_t screenWidth, int32_t screenHeight) : gameStatus(STARTUP),
	player_(NULL), map_(NULL), fovRadius_(10),
	screenWidth_(screenWidth), screenHeight_(screenHeight)
{
    TCODConsole::initRoot(screenWidth_, screenHeight_, "OurUO Tech Demo", false);
    gui_ = new Gui();
}

void Engine::Init()
{
    player_ = new Actor(40, 25, '@', "player", TCODColor::white);
    player_->destructible_ = new PlayerDestructible(30, 2, "il tuo cadavere");
    player_->attacker_=new Attacker(5);
    player_->ai_ = new PlayerAi();
    player_->container_ = new Container(26);
    actors_.push(player_);
    map_ = new Map(80, 43);
    map_->Init(true);
    gui_->Message(TCODColor::red, "Benvenuto Tester!\nPreparati a morire tra le segrete della Tech Demo di OurUO.");
    gameStatus = STARTUP;
}

Engine::~Engine()
{
	Term();
    delete gui_;
}

void Engine::Term()
{
    actors_.clearAndDelete();
    if (map_) delete map_;
    gui_->Clear();
}

void Engine::Update()
{
	if (gameStatus == STARTUP) {
		map_->ComputeFov();
	}
   	gameStatus = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey_, &mouse_);
    if (lastKey_.vk == TCODK_ESCAPE) {
    	Save();
    	Load();
    }
    player_->Update();
    if (gameStatus == NEW_TURN) {
    	for (Actor *actor : actors_) {
	        if (actor != player_) {
	            actor->Update();
	        }
	    }
	}
}

void Engine::Render()
{
	TCODConsole::root->clear();
	// draw the map
	map_->Render();
	// draw the actors
	//for (Actor **iterator=actors_.begin(); iterator != actors_.end(); iterator++) {
	//	Actor *actor=*iterator;
	for (Actor *actor : actors_) {
		if ((actor != player_) && (map_->IsInFov(actor->x_,actor->y_))) {
	        actor->Render();
	    }
	}
	player_->Render();
	// show the player's stats
	gui_->Render();
}

void Engine::SendToBack(Actor *actor)
{
	actors_.remove(actor);
	actors_.insertBefore(actor, 0);
}

Actor *Engine::GetActor(int32_t x, int32_t y) const
{
	for (Actor *actor : actors_) {
		if ((actor->x_ == x) && (actor->y_ == y) && (actor->destructible_) && (!actor->destructible_->IsDead())) {
			return actor;
		}
	}
	return NULL;
}

Actor *Engine::GetClosestMonster(int32_t x, int32_t y, float range) const
{
	Actor *closest = NULL;
	float bestDistance = 1E6f;

	for (Actor *actor : actors_) {
		if ((actor != player_) && (actor->destructible_) && (!actor->destructible_->IsDead())) {
			float distance = actor->GetDistance(x, y);
			if ((distance < bestDistance) && ((distance <= range) || (range == 0.0f))) {
				bestDistance = distance;
				closest = actor;
			}
		}
	}

	return closest;
}

bool Engine::PickATile(int32_t *x, int32_t *y, float maxRange)
{
	while (!TCODConsole::isWindowClosed()) {
		Render();
		// highlight the possible range
		for (int32_t cx = 0; cx < map_->width_; cx++) {
			for (int32_t cy = 0; cy < map_->height_; cy++) {
				if ((map_->IsInFov(cx,cy)) && ((maxRange == 0) || (player_->GetDistance(cx, cy) <= maxRange))) {
					TCODColor col = TCODConsole::root->getCharBackground(cx, cy);
					col = col * 1.2f;
					TCODConsole::root->setCharBackground(cx, cy, col);
				}
			}
		}
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey_, &mouse_);
		if ((map_->IsInFov(mouse_.cx, mouse_.cy)) && ((maxRange == 0) || (player_->GetDistance(mouse_.cx, mouse_.cy)) <= maxRange)) {
			TCODConsole::root->setCharBackground(mouse_.cx, mouse_.cy, TCODColor::white);
			if (mouse_.lbutton_pressed) {
				*x = mouse_.cx;
				*y = mouse_.cy;
				return true;
			}
		} 
		if ((mouse_.rbutton_pressed) || (lastKey_.vk != TCODK_NONE)) {
			return false;
		}
		TCODConsole::flush();
	}
	return false;
}
