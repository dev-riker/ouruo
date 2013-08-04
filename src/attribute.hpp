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
	virtual			~Attribute();

	void			AddBaseValueBonus(int32_t bonus);
	void			SetBaseValue(int32_t newValue);
	int32_t			GetBaseValue(void) const;

	int32_t			GetCurrValue(void) const;

	const char*		GetName(void) const;
	void			SetName(std::string name);

	bool			Restore(void);
	bool			Damage(int32_t value);
	bool			Fortify(int32_t value, int32_t rounds);
	bool			Drain(int32_t value, int32_t rounds);
	bool			Absorb(int32_t value, int32_t rounds);


	virtual void	Recompute(void);
	void			RoundTick(void);

protected:
	enum effect_type { damage, fortify, drain, absorb };
	struct active_effect_t
	{
		effect_type	effect;
		int32_t		value;
		int32_t		remainingRounds;
	};

	std::vector <active_effect_t> effects_;

	int32_t		baseValue_;
	int32_t		currentValue_;
	std::string name_;

protected:
	bool		UpdateEffect(effect_type effect, int32_t value, int32_t rounds);
	bool		IsEffectFinished(active_effect_t effect);
};

class Agility : public Attribute {
public:
				Agility();
};

class Endurance : public Attribute {
public:
				Endurance();
};

class Intelligence : public Attribute {
public:
				Intelligence();
};

class Luck : public Attribute {
public:
				Luck();
};

class Personality : public Attribute {
public:
				Personality();
};

class Speed : public Attribute {
public:
				Speed();
};

class Strength : public Attribute {
public:
				Strength();
};

class Willpower : public Attribute {
public:
				Willpower();
};

// Forward declaration
class DerivedAttributes;

class BasicAttributes {
public:
				BasicAttributes();
				~BasicAttributes();

	void		SetDerivedAttributes(DerivedAttributes *newDerivedAttributes);
	void		Recompute();

	std::unique_ptr <Agility>		agility_;
	std::unique_ptr <Endurance>		endurance_;
	std::unique_ptr <Intelligence>	intelligence_;
	std::unique_ptr <Luck>			luck_;
	std::unique_ptr <Personality>	personality_;
	std::unique_ptr <Speed>			speed_;
	std::unique_ptr <Strength>		strength_;
	std::unique_ptr <Willpower>		willpower_;

protected:
	DerivedAttributes *derivedAttributes_;
};

class Health : public Attribute {
public:
				Health();
};

class Magicka : public Attribute {
public:
				Magicka();
};

class Fatigue : public Attribute {
public:
				Fatigue();
};

class Encumbrance : public Attribute {
public:
				Encumbrance();
};

class DerivedAttributes {
public:
				DerivedAttributes();
				~DerivedAttributes();

	std::unique_ptr <Health> 		health_;
	std::unique_ptr <Magicka> 		magicka_;
	std::unique_ptr <Fatigue> 		fatigue_;
	std::unique_ptr <Encumbrance> 	encumbrance_;
};

class Aggression : public Attribute {
public:
				Aggression();
};

class Confidence : public Attribute {
public:
				Confidence();
};

class Disposition : public Attribute {
public:
				Disposition();
};

class EnergyLevel : public Attribute {
public:
				EnergyLevel();
};

class Responsibility : public Attribute {
public:
				Responsibility();
};

class NPCAttributes {
public:
				NPCAttributes();
				~NPCAttributes();

	std::unique_ptr <Aggression> 		aggression_;
	std::unique_ptr <Confidence> 		confidence_;
	std::unique_ptr <Disposition> 		disposition_;
	std::unique_ptr <EnergyLevel> 		energyLevel_;
	std::unique_ptr <Responsibility> 	responsibility_;
};

#endif // _ATTRIBUTE_H_
