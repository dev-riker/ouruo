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

#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

#include <string>
#include <vector>

#define EFFECT_INFINITE_ROUNDS		(-100000)

class Attribute
{
public:
				Attribute(std::string name);
				Attribute(char name[]);
				Attribute();

	void		SetBaseValue(int32_t newValue);
	int32_t		GetBaseValue(void) const;

//	void		SetCurrValue(int newValue);
	int32_t		GetCurrValue(void) const;

	const char	*GetName(void) const;
	void		SetName(std::string name);

	bool		Restore(void);
	bool		Damage(int32_t value);
	bool		Fortify(int32_t value, int32_t rounds);
	bool		Drain(int32_t value, int32_t rounds);
	bool		Absorb(int32_t value, int32_t rounds);


	void		Recompute(void);
	void		RoundTick(void);

protected:
	enum effect_type { damage, fortify, drain, absorb };
	struct active_effect
	{
		effect_type	effect;
		int32_t		value;
		int32_t		remainingRounds;
	};

	std::vector <active_effect> effects_;

	int32_t		baseValue_;
	int32_t		currentValue_;
	std::string name_;

protected:
	bool		UpdateEffect(effect_type effect, int32_t value, int32_t rounds);
	bool		IsEffectFinished(active_effect effect);
};

#endif // _ATTRIBUTE_H_
