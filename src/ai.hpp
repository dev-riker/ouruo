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

#ifndef _AI_HPP_
#define _AI_HPP_

class Ai : public Persistent
{
public:
	virtual 		~Ai() { };
	virtual void	Update(Actor *owner) = 0;
	static Ai		*Create(TCODZip &zip);

protected:
	enum AiType
	{
		MONSTER, CONFUSED_MONSTER, PLAYER
	};
};

class MonsterAi : public Ai
{
public:
			MonsterAi();
	void	Update(Actor *owner);
	void	Load(TCODZip &zip);
	void	Save(TCODZip &zip);

protected:
	int32_t		moveCount_;

	void	MoveOrAttack(Actor *owner, int32_t targetx, int32_t targety);
};

class ConfusedMonsterAi : public Ai
{
public:
			ConfusedMonsterAi(int32_t nbTurns, Ai *oldAi);
	void	Update(Actor *owner);
	void	Load(TCODZip &zip);
	void	Save(TCODZip &zip);

protected:
	int32_t		nbTurns_;
	Ai		*oldAi_;
};

class PlayerAi : public Ai
{
public:
	void	Update(Actor *owner);
	void	Load(TCODZip &zip);
	void	Save(TCODZip &zip);
protected:
	bool	MoveOrAttack(Actor *owner, int32_t targetx, int32_t targety);
	void	HandleActionKey(Actor *owner, int32_t ascii);
	Actor	*ChooseFromInventory(Actor *owner);
	void	DisplayCharacterSheet(Actor *owner);
private:
	void	PrintAttributeOnCharacterSheet(TCODConsole &con, Attribute *currAttr, int32_t baseRow, int32_t baseColumn);
};

#endif // _AI_HPP_

