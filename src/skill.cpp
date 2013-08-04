#include "main.hpp"

#define SKILL_STARTING_VALUE			(5)

Skill::Skill() : Attribute(), major_(false)
{
}

bool Skill::IsMajor()
{
	return major_;
}

void Skill::SetMajor(bool newValue)
{
	major_ = newValue;
}

BladeSkill::BladeSkill()
{
	name_ = "Blade";
	baseValue_ = SKILL_STARTING_VALUE;
}

BluntSkill::BluntSkill()
{
	name_ = "Blunt";
	baseValue_ = SKILL_STARTING_VALUE;
}

HandToHandSkill::HandToHandSkill()
{
	name_ = "Hand to Hand";
	baseValue_ = SKILL_STARTING_VALUE;
}

ArmorerSkill::ArmorerSkill()
{
	name_ = "Armorer";
	baseValue_ = SKILL_STARTING_VALUE;
}

BlockSkill::BlockSkill()
{
	name_ = "Block";
	baseValue_ = SKILL_STARTING_VALUE;
}

HeavyArmorSkill::HeavyArmorSkill()
{
	name_ = "Heavy Armor";
	baseValue_ = SKILL_STARTING_VALUE;
}

AthleticsSkill::AthleticsSkill()
{
	name_ = "Athletics";
	baseValue_ = SKILL_STARTING_VALUE;
}

AcrobaticsSkill::AcrobaticsSkill()
{
	name_ = "Acrobatics";
	baseValue_ = SKILL_STARTING_VALUE;
}

LightArmorSkill::LightArmorSkill()
{
	name_ = "Light Armor";
	baseValue_ = SKILL_STARTING_VALUE;
}

SecuritySkill::SecuritySkill()
{
	name_ = "Security";
	baseValue_ = SKILL_STARTING_VALUE;
}

SneakSkill::SneakSkill()
{
	name_ = "Sneak";
	baseValue_ = SKILL_STARTING_VALUE;
}

MarksmanSkill::MarksmanSkill()
{
	name_ = "Marksman";
	baseValue_ = SKILL_STARTING_VALUE;
}

MercantileSkill::MercantileSkill()
{
	name_ = "Mercantile";
	baseValue_ = SKILL_STARTING_VALUE;
}

SpeechCraftSkill::SpeechCraftSkill()
{
	name_ = "Speechcraft";
	baseValue_ = SKILL_STARTING_VALUE;
}

IllusionSkill::IllusionSkill()
{
	name_ = "Illusion";
	baseValue_ = SKILL_STARTING_VALUE;
}

AlchemySkill::AlchemySkill()
{
	name_ = "Alchemy";
	baseValue_ = SKILL_STARTING_VALUE;
}

ConjurationSkill::ConjurationSkill()
{
	name_ = "Conjuration";
	baseValue_ = SKILL_STARTING_VALUE;
}

MysticismSkill::MysticismSkill()
{
	name_ = "Mysticism";
	baseValue_ = SKILL_STARTING_VALUE;
}

AlterationSkill::AlterationSkill()
{
	name_ = "Alteration";
	baseValue_ = SKILL_STARTING_VALUE;
}

DestructionSkill::DestructionSkill()
{
	name_ = "Destruction";
	baseValue_ = SKILL_STARTING_VALUE;
}

RestorationSkill::RestorationSkill()
{
	name_ = "Restoration";
	baseValue_ = SKILL_STARTING_VALUE;
}

SkillTree::SkillTree()
{
	majorSkills_.clear();
	minorSkills_.clear();

	// Crea tutte le skill disponibili
	blade_ =		std::unique_ptr<BladeSkill>			(new BladeSkill());
	blunt_ =		std::unique_ptr<BluntSkill>			(new BluntSkill());
	handToHand_ =	std::unique_ptr<HandToHandSkill>	(new HandToHandSkill());
	armorer_ =		std::unique_ptr<ArmorerSkill>		(new ArmorerSkill());
	block_ =		std::unique_ptr<BlockSkill>			(new BlockSkill());
	heavyArmor_ =	std::unique_ptr<HeavyArmorSkill>	(new HeavyArmorSkill());
	athletics_ =	std::unique_ptr<AthleticsSkill>		(new AthleticsSkill());
	acrobatics_ =	std::unique_ptr<AcrobaticsSkill>	(new AcrobaticsSkill());
	lightArmor_ =	std::unique_ptr<LightArmorSkill>	(new LightArmorSkill());
	security_ =		std::unique_ptr<SecuritySkill>		(new SecuritySkill());
	sneak_ =		std::unique_ptr<SneakSkill>			(new SneakSkill());
	marksman_ =		std::unique_ptr<MarksmanSkill>		(new MarksmanSkill());
	mercantile_ =	std::unique_ptr<MercantileSkill>	(new MercantileSkill());
	speechCraft_ =	std::unique_ptr<SpeechCraftSkill>	(new SpeechCraftSkill());
	illusion_ =		std::unique_ptr<IllusionSkill>		(new IllusionSkill());
	alchemy_ =		std::unique_ptr<AlchemySkill>		(new AlchemySkill());
	conjuration_ =	std::unique_ptr<ConjurationSkill>	(new ConjurationSkill());
	mysticism_ =	std::unique_ptr<MysticismSkill>		(new MysticismSkill());
	alteration_ =	std::unique_ptr<AlterationSkill>	(new AlterationSkill());
	destruction_ =	std::unique_ptr<DestructionSkill>	(new DestructionSkill());
	restoration_ =	std::unique_ptr<RestorationSkill>	(new RestorationSkill());
}

bool SkillTree::AddMajorSkill(Skill *newMajorSkill)
{
	bool ret = false;

	// Controlla che non si sia gi� inserita una skill minore o maggiore dello stesso tipo nella lista
	if ((majorSkills_.count(newMajorSkill->GetName()) == 0) && (minorSkills_.count(newMajorSkill->GetName()) == 0))
	{
		majorSkills_[newMajorSkill->GetName()] = newMajorSkill;
		ret = true;
	}

	return ret;
}

bool SkillTree::AddMinorSkill(Skill *newMinorSkill)
{
	bool ret = false;

	// Controlla che non si sia gi� inserita una skill minore o maggiore dello stesso tipo nella lista
	if ((majorSkills_.count(newMinorSkill->GetName()) == 0) && (minorSkills_.count(newMinorSkill->GetName()) == 0))
	{
		minorSkills_[newMinorSkill->GetName()] = newMinorSkill;
		ret = true;
	}

	return ret;
}

void SkillTree::UpdateSkillTree()
{
	if (acrobatics_->IsMajor() == true)	{ AddMajorSkill(acrobatics_.get());	}
	else { AddMinorSkill(acrobatics_.get()); }
	acrobatics_->Recompute();

	if (alchemy_->IsMajor() == true)	{ AddMajorSkill(alchemy_.get());	}
	else { AddMinorSkill(alchemy_.get()); }
	alchemy_->Recompute();
	
	if (alteration_->IsMajor() == true)	{ AddMajorSkill(alteration_.get());	}
	else { AddMinorSkill(alteration_.get()); }
	alteration_->Recompute();
	
	if (armorer_->IsMajor() == true)	{ AddMajorSkill(armorer_.get());	}
	else { AddMinorSkill(armorer_.get()); }
	armorer_->Recompute();
	
	if (athletics_->IsMajor() == true)	{ AddMajorSkill(athletics_.get());	}
	else { AddMinorSkill(athletics_.get()); }
	athletics_->Recompute();
	
	if (blade_->IsMajor() == true)	{ AddMajorSkill(blade_.get());	}
	else { AddMinorSkill(blade_.get()); }
	blade_->Recompute();
	
	if (block_->IsMajor() == true)	{ AddMajorSkill(block_.get());	}
	else { AddMinorSkill(block_.get()); }
	block_->Recompute();
	
	if (blunt_->IsMajor() == true)	{ AddMajorSkill(blunt_.get());	}
	else { AddMinorSkill(blunt_.get()); }
	blunt_->Recompute();
	
	if (conjuration_->IsMajor() == true)	{ AddMajorSkill(conjuration_.get());	}
	else { AddMinorSkill(conjuration_.get()); }
	conjuration_->Recompute();
	
	if (destruction_->IsMajor() == true)	{ AddMajorSkill(destruction_.get());	}
	else { AddMinorSkill(destruction_.get()); }
	destruction_->Recompute();
	
	if (handToHand_->IsMajor() == true)	{ AddMajorSkill(handToHand_.get());	}
	else { AddMinorSkill(handToHand_.get()); }
	handToHand_->Recompute();
	
	if (heavyArmor_->IsMajor() == true)	{ AddMajorSkill(heavyArmor_.get());	}
	else { AddMinorSkill(heavyArmor_.get()); }
	heavyArmor_->Recompute();
	
	if (illusion_->IsMajor() == true)	{ AddMajorSkill(illusion_.get());	}
	else { AddMinorSkill(illusion_.get()); }
	illusion_->Recompute();
	
	if (lightArmor_->IsMajor() == true)	{ AddMajorSkill(lightArmor_.get());	}
	else { AddMinorSkill(lightArmor_.get()); }
	lightArmor_->Recompute();
	
	if (marksman_->IsMajor() == true)	{ AddMajorSkill(marksman_.get());	}
	else { AddMinorSkill(marksman_.get()); }
	marksman_->Recompute();
	
	if (mercantile_->IsMajor() == true)	{ AddMajorSkill(mercantile_.get());	}
	else { AddMinorSkill(mercantile_.get()); }
	mercantile_->Recompute();
	
	if (mysticism_->IsMajor() == true)	{ AddMajorSkill(mysticism_.get());	}
	else { AddMinorSkill(mysticism_.get()); }
	mysticism_->Recompute();
	
	if (restoration_->IsMajor() == true)	{ AddMajorSkill(restoration_.get());	}
	else { AddMinorSkill(restoration_.get()); }
	restoration_->Recompute();
	
	if (security_->IsMajor() == true)	{ AddMajorSkill(security_.get());	}
	else { AddMinorSkill(security_.get()); }
	security_->Recompute();
	
	if (sneak_->IsMajor() == true)	{ AddMajorSkill(sneak_.get());	}
	else { AddMinorSkill(sneak_.get()); }
	sneak_->Recompute();
	
	if (speechCraft_->IsMajor() == true)	{ AddMajorSkill(speechCraft_.get());	}
	else { AddMinorSkill(speechCraft_.get()); }
	speechCraft_->Recompute();
}
