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
	skills_.UpdateSkillTree();
}
