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
#include <functional>
#include <algorithm>
#include "main.hpp"

Attribute::Attribute(std::string name)
{
	name_ = name;
	effects_.clear();
	baseValue_ = 0;
	currentValue_ = 0;
}

Attribute::Attribute(char name[])
{
	name_ = name;
	effects_.clear();
	baseValue_ = 0;
	currentValue_ = 0;
}

Attribute::Attribute()
{
	name_ = "NO_NAME";
	effects_.clear();
	baseValue_ = 0;
	currentValue_ = 0;
}

void Attribute::SetBaseValue(int newValue)
{
	baseValue_ = newValue;
}

int32_t Attribute::GetBaseValue() const
{
	return baseValue_;
}

// void Attribute::SetCurrValue(int newValue)
// {
// 	currentValue_ = newValue;
// }

int32_t Attribute::GetCurrValue() const
{
	return currentValue_;
}

const char *Attribute::GetName() const
{
	return name_.c_str();
}

void Attribute::SetName(std::string name)
{
	name_ = name;
}
	
bool Attribute::Restore(void)
{
	// L'effetto di restore può essere applicato solamente se un attributo è stato danneggiato
	// in precedenza con un effetto di Damage.
	bool damaged = false;

	for (uint32_t i = 0; i < effects_.size(); i++) {
		if (effects_[i].effect == damage) {
			damaged = true;
			// Elimina l'effetto dalla lista. Posso uscire dal loop ed evitare
			// eventuali ricomputazioni della dimensione massima perché sono
			// sicuro che l'effetto in questione sia presente al massimo 1 volta
			// all'interno degli effetti della lista perché effetti dello stesso
			// tipo NON sono cumulabili. Nel caso in cui si modificasse questa policy
			// sarebbe necessario modificare la funzione in una simile a Attribute::RoundTick.
			effects_.erase(effects_.begin() + i);
			break;
		}
	}

	return damaged;
}

bool Attribute::Damage(int32_t value)
{
	return UpdateEffect(damage, -value, EFFECT_INFINITE_ROUNDS);
}

bool Attribute::Fortify(int32_t value, int32_t rounds)
{
	return UpdateEffect(fortify, +value, rounds);
}

bool Attribute::Drain(int32_t value, int32_t rounds)
{
	return UpdateEffect(drain, -value, rounds);
}

bool Attribute::Absorb(int32_t value, int32_t rounds)
{
	return UpdateEffect(absorb, -value, rounds);
}

void Attribute::Recompute(void)
{
	currentValue_ = baseValue_;

	for (uint32_t i = 0; i < effects_.size(); i++) {
		currentValue_ += effects_[i].value;
	}
	if (currentValue_ < 0) {
		currentValue_ = 0;
	}
}

bool Attribute::IsEffectFinished(active_effect effect)
{
	bool ret = false;

	if ((effect.remainingRounds > EFFECT_INFINITE_ROUNDS) && (effect.remainingRounds <= 0)) {
		ret = true;
	}

	return ret;
}

void Attribute::RoundTick()
{
	// Scorre la lista degli effetti attivi decrementando il numero di round rimanenti
	for (uint32_t i = 0; i < effects_.size(); i++) {
		if (effects_[i].remainingRounds != EFFECT_INFINITE_ROUNDS) {
			effects_[i].remainingRounds--;
		}
	}

	// Elimina gli effetti che hanno raggiunto la loro durata prefissata (effetti con remainingRounds <= 0 E > EFFECT_INFINITE_ROUNDS)
	std::vector <active_effect>::iterator newEnd = std::remove_if(effects_.begin(), effects_.end(), std::bind1st(std::mem_fun(&Attribute::IsEffectFinished), this));
	effects_.erase(newEnd, effects_.end());
}

bool Attribute::UpdateEffect(effect_type effect, int32_t value, int32_t rounds)
{
	// Cerca un eventuale altro attributo già attivo e nel caso lo
	// modifica aggiornandone il valore e ricomputando il numero di round attivi.
	bool updated = false;
	for (uint32_t i = 0; i < effects_.size(); i++) {
		if (effects_[i].effect == effect) {
			updated = true;
			if (effects_[i].value < value) {
				effects_[i].value = value;
			}
			if (effects_[i].remainingRounds < rounds) {
				effects_[i].remainingRounds = rounds;
			}
		}
	}

	if (updated == false) {
		active_effect tmpEffect;
		tmpEffect.effect = effect;
		tmpEffect.remainingRounds = rounds;
		tmpEffect.value = value;

		effects_.push_back(tmpEffect);
	}

	return true;
}
