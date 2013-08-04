#include "main.hpp"

MobileBaseClass::MobileBaseClass()
{
}

WarriorClass::WarriorClass()
{
	skills_.armorer_->SetMajor(true);
	skills_.athletics_->SetMajor(true);
	skills_.blade_->SetMajor(true);
	skills_.block_->SetMajor(true);
	skills_.blunt_->SetMajor(true);
	skills_.handToHand_->SetMajor(true);
	skills_.heavyArmor_->SetMajor(true);

	skills_.blade_->AddBaseValueBonus(5);
	skills_.blunt_->AddBaseValueBonus(5);
	skills_.handToHand_->AddBaseValueBonus(5);
	skills_.heavyArmor_->AddBaseValueBonus(10);
	skills_.mercantile_->AddBaseValueBonus(10);
	skills_.speechCraft_->AddBaseValueBonus(10);
	skills_.UpdateSkillTree();

	basicAttributes_.SetDerivedAttributes(&derivedAttributes_);
	basicAttributes_.strength_->SetBaseValue(40);
	basicAttributes_.intelligence_->SetBaseValue(40);
	basicAttributes_.willpower_->SetBaseValue(30);
	basicAttributes_.agility_->SetBaseValue(30);
	basicAttributes_.speed_->SetBaseValue(40);
	basicAttributes_.endurance_->SetBaseValue(40);
	basicAttributes_.personality_->SetBaseValue(50);
	basicAttributes_.luck_->SetBaseValue(50);
	basicAttributes_.Recompute();

}
