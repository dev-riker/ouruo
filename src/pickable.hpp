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

#ifndef _PICKABLE_H_
#define _PICKABLE_H_

class Pickable : public Persistent
{
public:
	virtual ~Pickable() { };
	bool Pick(Actor *owner, Actor *wearer);
	void Drop(Actor *owner, Actor *wearer);
	virtual bool Use(Actor *owner, Actor *wearer);
	static Pickable *Create(TCODZip &zip);

protected:
	enum PickableType
	{
		HEALER, LIGHTNING_BOLT, CONFUSER, FIREBALL
	};
};

class Healer : public Pickable
{
public:
	float amount_; // how many hp

	Healer(float amount);
	bool Use(Actor *owner, Actor *wearer);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);	
};

class LightningBolt : public Pickable
{
public:
	float range_;
	float damage_;
	LightningBolt(float range, float damage);
	bool Use(Actor *owner, Actor *wearer);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};

class Confuser : public Pickable
{
public:
	int nbTurns_;
	float range_;
	Confuser(int nbTurns, float range);
	bool Use(Actor *owner, Actor *wearer);	
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};

class Fireball : public LightningBolt
{
public:
	Fireball(float range, float damage);
	bool Use(Actor *owner, Actor *wearer);	
	void Save(TCODZip &zip);	
};

#endif // _PICKABLE_H_
