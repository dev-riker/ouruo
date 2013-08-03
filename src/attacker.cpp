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
#include "main.hpp"

Attacker::Attacker(float power) : power_(power)
{
}

void Attacker::Attack(Actor *owner, Actor *target)
{
	if ((target->destructible_) && (!target->destructible_->IsDead())) {
		if ((power_ - target->destructible_->defense_) > 0) {
			engine.gui_->Message(owner==engine.player_ ? TCODColor::red : TCODColor::lightGrey,	"%s attacca %s per %g HP.",
				owner->name_, target->name_, (power_ - target->destructible_->defense_));
		} else {
			engine.gui_->Message(TCODColor::lightGrey, "%s attacca %s ma non ha alcun effetto!", owner->name_, target->name_);
		}
		target->destructible_->TakeDamage(target,power_);
	} else {
		engine.gui_->Message(TCODColor::lightGrey, "%s attacca %s mancandolo.", owner->name_, target->name_);
	}
}
