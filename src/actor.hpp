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

#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

class Actor : public Persistent
{
public:
    int32_t			x_;				// x position on map
	int32_t			y_;				// y position on map
    int32_t			ch_;			// ascii code
    TCODColor		col_;			// color
	const char *	name_;			// the actor's name
	bool			blocks_;		// can we walk on this actor?
	Attacker * 		attacker_;		// something that deals damages
	Destructible * 	destructible_; // something that can be damaged
	Ai *			ai_;			// something self-updating
	Pickable *		pickable_;		// something that can be picked and used
	Container *		container_;	// something that can contain actors
	 
					Actor(int32_t x, int32_t y, int32_t ch, const char *name, const TCODColor &col);
	virtual			~Actor();
	void			Update();
    void			Render() const;
    float			GetDistance(int32_t cx, int32_t cy) const;
	void			Load(TCODZip &zip);
	void			Save(TCODZip &zip);    
};

#endif // _ACTOR_HPP_
