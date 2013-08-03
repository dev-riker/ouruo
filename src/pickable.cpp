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

bool Pickable::Pick(Actor *owner, Actor *wearer)
{
	if ((wearer->container_) && (wearer->container_->Add(owner)))
	{
		engine.actors_.remove(owner);
		return true;
	}
	return false;
}

void Pickable::Drop(Actor *owner, Actor *wearer)
{
	if (wearer->container_)
	{
		wearer->container_->Remove(owner);
		engine.actors_.push(owner);
		owner->x_=wearer->x_;
		owner->y_=wearer->y_;
		engine.gui_->Message(TCODColor::lightGrey, "%s drops a %s.", wearer->name_, owner->name_);
	}
}

bool Pickable::Use(Actor *owner, Actor *wearer)
{
	if (wearer->container_)
	{
		wearer->container_->Remove(owner);
		delete owner;
		return true;
	}
	return false;
}

Healer::Healer(float amount) : amount_(amount)
{
}

bool Healer::Use(Actor *owner, Actor *wearer)
{
	if ( wearer->destructible_ )
	{
		float amountHealed = wearer->destructible_->Heal(amount_);
		if ( amountHealed > 0 )
		{
			return Pickable::Use(owner, wearer);
		}
	}
	return false;
}

LightningBolt::LightningBolt(float range, float damage) : range_(range), damage_(damage)
{
}

bool LightningBolt::Use(Actor *owner, Actor *wearer)
{
	Actor *closestMonster = engine.GetClosestMonster(wearer->x_, wearer->y_, range_);
	if (! closestMonster )
	{
		engine.gui_->Message(TCODColor::lightGrey,"No enemy is close enough to strike.");
		return false;
	}
	// hit closest monster for <damage> hit points
	engine.gui_->Message(TCODColor::lightBlue,
		"A lighting bolt strikes the %s with a loud thunder!\nThe damage is %g hit points.",
		closestMonster->name_, damage_);
	closestMonster->destructible_->TakeDamage(closestMonster,damage_);
	return Pickable::Use(owner,wearer);
}

Confuser::Confuser(int nbTurns, float range) : nbTurns_(nbTurns), range_(range)
{
}

bool Confuser::Use(Actor *owner, Actor *wearer)
{
	engine.gui_->Message(TCODColor::cyan, "Left-click an enemy to confuse it,\nor right-click to cancel.");
	int x, y;
	if (!engine.PickATile(&x,&y,range_))
	{
		return false;
	}

	Actor *actor = engine.GetActor(x,y);
	if (!actor)
	{
		return false;
	}
	// confuse the monster for <nbTurns> turns
	Ai *confusedAi = new ConfusedMonsterAi(nbTurns_, actor->ai_);
	actor->ai_ = confusedAi;
	engine.gui_->Message(TCODColor::lightGreen,"The eyes of the %s look vacant,\nas he starts to stumble around!", actor->name_);
	return Pickable::Use(owner,wearer);
}

Fireball::Fireball(float range, float damage) : LightningBolt(range, damage)
{
}

bool Fireball::Use(Actor *owner, Actor *wearer)
{
	engine.gui_->Message(TCODColor::cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
	int x, y;
	if (!engine.PickATile(&x,&y))
	{
		return false;
	}
	// burn everything in <range> (including player)
	engine.gui_->Message(TCODColor::orange,"The fireball explodes, burning everything within %g tiles!", range_);
	for (Actor **iterator = engine.actors_.begin(); iterator != engine.actors_.end(); iterator++)
	{
		Actor *actor = *iterator;
		if (actor->destructible_ && !actor->destructible_->IsDead() && actor->GetDistance(x,y) <= range_)
		{
			engine.gui_->Message(TCODColor::orange, "The %s gets burned for %g hit points.", actor->name_, damage_);
			actor->destructible_->TakeDamage(actor, damage_);
		}
	}
	return Pickable::Use(owner, wearer);
}
