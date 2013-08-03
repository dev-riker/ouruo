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

Actor::Actor(int32_t x, int32_t y, int32_t ch, const char *name, const TCODColor &col) :
    x_(x), y_(y), ch_(ch), col_(col), name_(name),
    blocks_(true), attacker_(nullptr), destructible_(nullptr), ai_(nullptr),
    pickable_(nullptr), container_(nullptr)
{
}
 
Actor::~Actor()
{
	if (attacker_) delete attacker_;
	if (destructible_) delete destructible_;
	if (ai_) delete ai_;
	if (pickable_) delete pickable_;
	if (container_) delete container_;
}

void Actor::Render() const
{
    TCODConsole::root->setChar(x_, y_, ch_);
    TCODConsole::root->setCharForeground(x_, y_, col_);
}

void Actor::Update()
{
	if (ai_) ai_->Update(this);
}

float Actor::GetDistance(int32_t cx, int32_t cy) const
{
	int32_t dx = x_ - cx;
	int32_t dy = y_ - cy;
	return sqrtf((float) (dx * dx + dy * dy));
}
