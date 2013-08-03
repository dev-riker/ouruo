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

#include "main.hpp"

void Map::Load(TCODZip &zip)
{
	seed_=zip.getInt();
    Init(false);
	for (int32_t i = 0; i < width_ * height_; i++)
	{
		tiles_[i].explored_ = zip.getInt();
	}
}

void Map::Save(TCODZip &zip)
{
	zip.putInt(seed_);
	for (int32_t i = 0; i < width_ * height_; i++)
	{
		zip.putInt(tiles_[i].explored_);
	}
}

void Actor::Load(TCODZip &zip)
{
	x_ = zip.getInt();
	y_ = zip.getInt();
	ch_ = zip.getInt();
	col_ = zip.getColor();
	name_ = strdup(zip.getString());
	blocks_ = zip.getInt();
	bool hasAttacker = zip.getInt();
	bool hasDestructible = zip.getInt();
	bool hasAi = zip.getInt();
	bool hasPickable = zip.getInt();
	bool hasContainer = zip.getInt();
	if (hasAttacker == true) {
		attacker_ = new Attacker(0.0f);
		attacker_->Load(zip);
	}
	if (hasDestructible == true) {
		destructible_ = Destructible::Create(zip);
	}
	if (hasAi == true) {
		ai_ = Ai::Create(zip);
	}
	if (hasPickable == true) {
		pickable_ = Pickable::Create(zip);
	}
	if (hasContainer == true) {
		container_ = new Container(0);
		container_->Load(zip);
	}
}

void Actor::Save(TCODZip &zip)
{
	zip.putInt(x_);
	zip.putInt(y_);
	zip.putInt(ch_);
	zip.putColor(&col_);
	zip.putString(name_);
	zip.putInt(blocks_);
	zip.putInt(attacker_ != NULL);
	zip.putInt(destructible_ != NULL);
	zip.putInt(ai_ != NULL);
	zip.putInt(pickable_ != NULL);
	zip.putInt(container_ != NULL);
	if (attacker_ != NULL) attacker_->Save(zip);
	if (destructible_ != NULL) destructible_->Save(zip);
	if (ai_ != NULL) ai_->Save(zip);
	if (pickable_ != NULL) pickable_->Save(zip);
	if (container_ != NULL) container_->Save(zip);
}

void Container::Load(TCODZip &zip)
{
	size_ = zip.getInt();
	int32_t nbActors = zip.getInt();
	while (nbActors > 0) {
		Actor *actor = new Actor(0, 0, 0, NULL, TCODColor::white);
		actor->Load(zip);
		inventory.push(actor);
		nbActors--;
	}
}

void Container::Save(TCODZip &zip)
{
	zip.putInt(size_);
	zip.putInt(inventory.size());
	for (Actor **it = inventory.begin(); it != inventory.end(); it++) {
		(*it)->Save(zip);
	}
}

void Destructible::Load(TCODZip &zip)
{
	maxHp_ = zip.getFloat();
	hp_ = zip.getFloat();
	defense_ = zip.getFloat();
	corpseName_ = strdup(zip.getString());
}

void Destructible::Save(TCODZip &zip)
{
	zip.putFloat(maxHp_);
	zip.putFloat(hp_);
	zip.putFloat(defense_);
	zip.putString(corpseName_);
}

void PlayerDestructible::Save(TCODZip &zip)
{
	zip.putInt(PLAYER);
	Destructible::Save(zip);
}

void MonsterDestructible::Save(TCODZip &zip)
{
	zip.putInt(MONSTER);
	Destructible::Save(zip);
}

Destructible *Destructible::Create(TCODZip &zip)
{
	DestructibleType type = (DestructibleType) zip.getInt();
	Destructible *destructible = NULL;
	switch(type) {
	case MONSTER:
		destructible = new MonsterDestructible(0, 0, NULL);
		break;
	case PLAYER:
		destructible = new PlayerDestructible(0, 0, NULL);
		break;
	}
	if (destructible != NULL) {
		destructible->Load(zip);
	}

	return destructible;
}

void Attacker::Load(TCODZip &zip)
{
	power_ = zip.getFloat();
}

void Attacker::Save(TCODZip &zip)
{
	zip.putFloat(power_);
}

void MonsterAi::Load(TCODZip &zip)
{
	moveCount_ = zip.getInt();
}

void MonsterAi::Save(TCODZip &zip)
{
	zip.putInt(MONSTER);
	zip.putInt(moveCount_);
}

void ConfusedMonsterAi::Load(TCODZip &zip)
{
	nbTurns_ = zip.getInt();
	oldAi_ = Ai::Create(zip);
}

void ConfusedMonsterAi::Save(TCODZip &zip)
{
	zip.putInt(CONFUSED_MONSTER);
	zip.putInt(nbTurns_);
	oldAi_->Save(zip);
}

void PlayerAi::Load(TCODZip &zip)
{
}

void PlayerAi::Save(TCODZip &zip)
{
	zip.putInt(PLAYER);
}

Ai *Ai::Create(TCODZip &zip)
{
	AiType type = (AiType) zip.getInt();
	Ai *ai = NULL;

	switch(type) {
	case PLAYER:
		ai = new PlayerAi();
		break;
	case MONSTER:
		ai = new MonsterAi();
		break;
	case CONFUSED_MONSTER:
		ai = new ConfusedMonsterAi(0, NULL);
		break;
	}

	if (ai != NULL) {
		ai->Load(zip);
	}

	return ai;
}

void Healer::Load(TCODZip &zip)
{
	amount_=zip.getFloat();
}

void Healer::Save(TCODZip &zip)
{
	zip.putInt(HEALER);
	zip.putFloat(amount_);
}

void LightningBolt::Load(TCODZip &zip)
{
	range_ = zip.getFloat();
	damage_ = zip.getFloat();
}

void LightningBolt::Save(TCODZip &zip)
{
	zip.putInt(LIGHTNING_BOLT);
	zip.putFloat(range_);
	zip.putFloat(damage_);
}

void Confuser::Load(TCODZip &zip)
{
	nbTurns_ = zip.getInt();
	range_ = zip.getFloat();
}

void Confuser::Save(TCODZip &zip)
{
	zip.putInt(CONFUSER);
	zip.putInt(nbTurns_);
	zip.putFloat(range_);
}

void Fireball::Save(TCODZip &zip)
{
	zip.putInt(FIREBALL);
	zip.putFloat(range_);
	zip.putFloat(damage_);	
}

Pickable *Pickable::Create(TCODZip &zip)
{
	PickableType type = (PickableType) zip.getInt();
	Pickable *pickable = NULL;

	switch(type) {
	case HEALER:
		pickable = new Healer(0);
		break;
	case LIGHTNING_BOLT:
		pickable = new LightningBolt(0, 0);
		break;
	case CONFUSER:
		pickable = new Confuser(0, 0);
		break;
	case FIREBALL:
		pickable = new Fireball(0, 0);
		break;
	}

	if (pickable != NULL) {
		pickable->Load(zip);
	}

	return pickable;
}

void Gui::Load(TCODZip &zip)
{
	int nbMessages = zip.getInt();

	while (nbMessages > 0) {
		const char *text = zip.getString();
		TCODColor col = zip.getColor();
		Message(col, text);
		nbMessages--;
	}
}

void Gui::Save(TCODZip &zip)
{
	zip.putInt(log_.size());

	for (message **it = log_.begin(); it != log_.end(); it++) {
		zip.putString((*it)->text);
		zip.putColor(&(*it)->col);
	}
}

void Engine::Load()
{
	engine.gui_->menu_.Clear();
	engine.gui_->menu_.AddItem(Menu::NEW_GAME, "New game");
	if (TCODSystem::fileExists("game.sav")) {
		engine.gui_->menu_.AddItem(Menu::CONTINUE, "Continue");
	}
	engine.gui_->menu_.AddItem(Menu::EXIT, "Exit");

	Menu::MenuItemCode menuItem = engine.gui_->menu_.Pick();
	if ((menuItem == Menu::EXIT) || (menuItem == Menu::NONE )) {
		// Exit or window closed
		exit(0);
	} else if (menuItem == Menu::NEW_GAME) {
		// New game
		engine.Term();
		engine.Init();
	} else {
		TCODZip zip;
		// continue a saved game
		engine.Term();
		zip.loadFromFile("game.sav");
		// load the map
		int32_t width = zip.getInt();
		int32_t height = zip.getInt();
		map_ = new Map(width, height);
		map_->Load(zip);
		// then the player
		player_ = new Actor(0, 0, 0, NULL, TCODColor::white);
		actors_.push(player_);
		player_->Load(zip);
		// then all other actors
		int32_t nbActors = zip.getInt();
		while (nbActors > 0) {
			Actor *actor = new Actor(0, 0, 0, NULL, TCODColor::white);
			actor->Load(zip);
			actors_.push(actor);
			nbActors--;
		}
		// finally the message log
		gui_->Load(zip);
		// to force FOV recomputation
		gameStatus = STARTUP;
	}
}

void Engine::Save()
{
	if (player_->destructible_->IsDead()) {
		TCODSystem::deleteFile("game.sav");
	} else {
		TCODZip zip;
		// save the map
		zip.putInt(map_->width_);
		zip.putInt(map_->height_);
		map_->Save(zip);
		// save the player
		player_->Save(zip);
		// then all the other actors
		zip.putInt(actors_.size()-1);
		for (Actor **it = actors_.begin(); it != actors_.end(); it++) {
			if (*it != player_) {
				(*it)->Save(zip);
			}
		}
		// finally the message log
		gui_->Save(zip);
		zip.saveToFile("game.sav");
	}
}
