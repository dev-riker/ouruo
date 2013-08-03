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

#include <iostream>
#include "main.hpp"

static const int32_t ROOM_MAX_SIZE = 12;
static const int32_t ROOM_MIN_SIZE = 6;
static const int32_t MAX_ROOM_MONSTERS = 3;
static const int32_t MAX_ROOM_ITEMS = 2;

class BspListener : public ITCODBspCallback
{
private:
    Map &map_; // a map to dig
    int32_t roomNum_; // room number
    int32_t lastx_;
    int32_t lasty_; // center of the last room

public:
    BspListener(Map &map) : map_(map), roomNum_(0), lastx_(0), lasty_(0) {}
    bool visitNode(TCODBsp *node, void *userData)
	{
    	if (node->isLeaf()) {
    		int x, y, w, h;
			// dig a room
			bool withActors = (bool) userData;
			w = map_.rng_->getInt(ROOM_MIN_SIZE, node->w - 2);
			h = map_.rng_->getInt(ROOM_MIN_SIZE, node->h - 2);
			x = map_.rng_->getInt(node->x + 1, node->x + node->w - w - 1);
			y = map_.rng_->getInt(node->y + 1, node->y + node->h - h - 1);
			map_.CreateRoom(roomNum_ == 0, x, y, x + w - 1, y + h - 1, withActors);
			if ( roomNum_ != 0 ) {
			    // dig a corridor from last room
			    map_.Dig(lastx_, lasty_, x + w / 2, lasty_);
			    map_.Dig(x + w / 2, lasty_, x + w / 2, y + h / 2);
			}
            lastx_ = x + w / 2;
            lasty_ = y + h / 2;
            roomNum_++;
        }
        return true;
    }
};

Map::Map(int32_t width, int32_t height)
	: width_(width), height_(height)
{
	seed_ = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
	map_ = nullptr;
	tiles_ = nullptr;
	rng_ = nullptr;
}

void Map::Init(bool withActors)
{
	rng_ = new TCODRandom(seed_, TCOD_RNG_CMWC);
    tiles_ = new Tile[width_ * height_];
    map_=new TCODMap(width_, height_);
    TCODBsp bsp(0, 0, width_, height_);
    bsp.splitRecursive(rng_, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, (void *) withActors);
}

Map::~Map()
{
    delete [] tiles_;
    delete map_;
}

void Map::Dig(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    if (x2 < x1) {
    	int32_t tmp = x2;
        x2 = x1;
        x1 = tmp;
    }
    if (y2 < y1) {
    	int32_t tmp = y2;
        y2 = y1;
        y1 = tmp;
    }

    for (int32_t tilex = x1; tilex <= x2; tilex++) {
        for (int32_t tiley = y1; tiley <= y2; tiley++) {
            map_->setProperties(tilex, tiley, true, true);
        }
    }
}

void Map::AddMonster(int32_t x, int32_t y)
{
	TCODRandom *rng = TCODRandom::getInstance();
    if (rng->getInt(0, 100) < 80) {
		Actor *orc = NULL;
		try {
			// Crea un Orco
			orc = new Actor(x, y, 'o', "orco", TCODColor::desaturatedGreen);
			orc->destructible_ = new MonsterDestructible(10, 0, "orco morto");
			orc->attacker_ = new Attacker(3);
			orc->ai_ = new MonsterAi();
		}
		catch (const std::bad_alloc &ba) {
			std::cerr<<ba.what()<<std::endl;

			if (orc != NULL) {
				if (orc->destructible_ != NULL) {
					delete orc->destructible_;
				}
				if (orc->attacker_ != NULL) {
					delete orc->attacker_;
				}
				if (orc->ai_ != NULL) {
					delete orc->ai_;
				}
			}
		}
        engine.actors_.push(orc);
    } else {
        // Crea un Troll
		Actor *troll = NULL;

		try {
			troll = new Actor(x, y, 'T', "troll", TCODColor::darkerGreen);
			troll->destructible_ = new MonsterDestructible(16, 1, "carcassa di troll");
			troll->attacker_ = new Attacker(4);
			troll->ai_ = new MonsterAi();
		}
		catch (const std::bad_alloc &ba) {
			std::cerr<<ba.what()<<std::endl;
			if (troll != NULL) {
				if (troll->destructible_ != NULL) {
					delete troll->destructible_;
				}
				if (troll->attacker_ != NULL) {
					delete troll->attacker_;
				}
				if (troll->ai_ != NULL) {
					delete troll->ai_;
				}
			}
		}
        engine.actors_.push(troll);
    }
}

void Map::AddItem(int32_t x, int32_t y)
{
	TCODRandom *rng = TCODRandom::getInstance();
	int dice = rng->getInt(0, 100);

	if (dice < 70)
	{
		// create a health potion
		Actor *healthPotion = new Actor(x, y, '!', "pozione di guarigione", TCODColor::violet);
		healthPotion->blocks_ = false;
		healthPotion->pickable_ = new Healer(4);
		engine.actors_.push(healthPotion);
	} else if (dice < (70 + 10)) {
		// create a scroll of lightning bolt 
		Actor *scrollOfLightningBolt = new Actor(x, y, '#', "pergamena di fulmine",	TCODColor::lightYellow);
		scrollOfLightningBolt->blocks_ = false;
		scrollOfLightningBolt->pickable_ = new LightningBolt(5, 20);
		engine.actors_.push(scrollOfLightningBolt);
	} else if (dice < (70 + 10 + 10)) {
		// create a scroll of fireball
		Actor *scrollOfFireball = new Actor(x, y, '#', "pergamena di palla di fuoco", TCODColor::lightYellow);
		scrollOfFireball->blocks_ = false;
		scrollOfFireball->pickable_ = new Fireball(3, 12);
		engine.actors_.push(scrollOfFireball);
	} else {
		// create a scroll of confusion
		Actor *scrollOfConfusion = new Actor(x, y, '#', "pergamena di confusione", TCODColor::lightYellow);
		scrollOfConfusion->blocks_ = false;
		scrollOfConfusion->pickable_ = new Confuser(10, 8);
		engine.actors_.push(scrollOfConfusion);
	}
}

void Map::CreateRoom(bool first, int32_t x1, int32_t y1, int32_t x2, int32_t y2, bool withActors)
{
    Dig (x1, y1, x2, y2);
    if (!withActors) {
    	return;
    }  
    if (first) {
        // put the player in the first room
        engine.player_->x_ = (x1 + x2) / 2;
        engine.player_->y_ = (y1 + y2) / 2;
    } else {
		TCODRandom *rng = TCODRandom::getInstance();
		// add monsters
		int32_t nbMonsters=rng->getInt(0, MAX_ROOM_MONSTERS);
		while (nbMonsters > 0) {
			int32_t x = rng->getInt(x1, x2);
			int32_t y = rng->getInt(y1, y2);
    		if (CanWalk(x,y)) {
				AddMonster(x, y);
			}
		    nbMonsters--;
		}
		// add items
		int32_t nbItems = rng->getInt(0, MAX_ROOM_ITEMS);
		while (nbItems > 0) {
			int32_t x = rng->getInt(x1, x2);
			int32_t y = rng->getInt(y1, y2);
    		if (CanWalk(x, y)) {
				AddItem(x, y);
			}
		    nbItems--;
		}
    }
}

bool Map::IsWall(int32_t x, int32_t y) const
{
    return !map_->isWalkable(x, y);
}

bool Map::CanWalk(int32_t x, int32_t y) const
{
    if (IsWall(x, y)) {
        // this is a wall
        return false;
    }
    for (Actor **iterator = engine.actors_.begin(); iterator != engine.actors_.end(); iterator++) {
        Actor *actor = *iterator;
        if ((actor->blocks_) && (actor->x_ == x) && (actor->y_ == y)) {
            // there is a blocking actor here. cannot walk
            return false;
        }
    }
    return true;
}
 
bool Map::IsExplored(int32_t x, int32_t y) const
{
    return tiles_[x + y * width_].explored_;
}

bool Map::IsInFov(int32_t x, int32_t y) const
{
	if ((x < 0) || (x >= width_) || (y < 0) || (y >= height_)) {
		return false;
	}
    if (map_->isInFov(x,y)) {
        tiles_[x + y * width_].explored_ = true;
        return true;
    }

    return false;
}
 
void Map::ComputeFov()
{
    map_->computeFov(engine.player_->x_, engine.player_->y_, engine.fovRadius_);
}

void Map::Render() const
{
    static const TCODColor darkWall(0, 0, 100);
    static const TCODColor darkGround(50, 50, 150);
	static const TCODColor lightWall(130, 110, 50);
	static const TCODColor lightGround(200, 180, 50);

	for (int32_t x = 0; x < width_; x++) {
	    for (int32_t y = 0; y < height_; y++) {
	        if (IsInFov(x,y)) {
	            TCODConsole::root->setCharBackground(x, y, IsWall(x, y) ? lightWall : lightGround);
	        } else if (IsExplored(x, y)) {
	            TCODConsole::root->setCharBackground(x, y, IsWall(x, y) ? darkWall : darkGround);
	        }
   	    }
	}
}
