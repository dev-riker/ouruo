#ifndef _SKILL_HPP_
#define _SKILL_HPP_

class Skill : public Attribute
{
public:
				Skill();

	bool		IsMajor();
	void		SetMajor(bool newValue);

	/*
	void		SetValue(int newValue);
	int			GetValue(void);
	*/

protected:
	bool		major_;
};

class BladeSkill : public Skill
{
public:
	BladeSkill();
};

class BluntSkill : public Skill
{
public:
	BluntSkill();
};

class HandToHandSkill : public Skill
{
public:
	HandToHandSkill();
};

class ArmorerSkill : public Skill
{
public:
	ArmorerSkill();
};

class BlockSkill : public Skill
{
public:
	BlockSkill();
};

class HeavyArmorSkill : public Skill
{
public:
	HeavyArmorSkill();
};

class AthleticsSkill : public Skill
{
public:
	AthleticsSkill();
};

class AcrobaticsSkill : public Skill
{
public:
	AcrobaticsSkill();
};

class LightArmorSkill : public Skill
{
public:
	LightArmorSkill();
};

class SecuritySkill : public Skill
{
public:
	SecuritySkill();
};

class SneakSkill : public Skill
{
public:
	SneakSkill();
};

class MarksmanSkill : public Skill
{
public:
	MarksmanSkill();
};

class MercantileSkill : public Skill
{
public:
	MercantileSkill();
};

class SpeechCraftSkill : public Skill
{
public:
	SpeechCraftSkill();
};

class IllusionSkill : public Skill
{
public:
	IllusionSkill();
};

class AlchemySkill : public Skill
{
public:
	AlchemySkill();
};

class ConjurationSkill : public Skill
{
public:
	ConjurationSkill();
};

class MysticismSkill : public Skill
{
public:
	MysticismSkill();
};

class AlterationSkill : public Skill
{
public:
	AlterationSkill();
};

class DestructionSkill : public Skill
{
public:
	DestructionSkill();
};

class RestorationSkill : public Skill
{
public:
	RestorationSkill();
};

class SkillTree
{
public:
			SkillTree();
	void	UpdateSkillTree();

protected:
	bool	AddMajorSkill(Skill *newMajorSkill);
	bool	AddMinorSkill(Skill *newMinorSkill);

public:
	std::map <std::string, Skill *>		majorSkills_;
	std::map <std::string, Skill *>		minorSkills_;

	std::unique_ptr <AcrobaticsSkill>	acrobatics_;
	std::unique_ptr <AlchemySkill>		alchemy_;
	std::unique_ptr <AlterationSkill>	alteration_;
	std::unique_ptr <ArmorerSkill>		armorer_;
	std::unique_ptr <AthleticsSkill>	athletics_;
	std::unique_ptr <BladeSkill>		blade_;
	std::unique_ptr <BlockSkill>		block_;
	std::unique_ptr <BluntSkill>		blunt_;
	std::unique_ptr <ConjurationSkill>	conjuration_;
	std::unique_ptr <DestructionSkill>	destruction_;
	std::unique_ptr <HandToHandSkill>	handToHand_;
	std::unique_ptr <HeavyArmorSkill>	heavyArmor_;
	std::unique_ptr <IllusionSkill>		illusion_;
	std::unique_ptr <LightArmorSkill>	lightArmor_;
	std::unique_ptr <MarksmanSkill>		marksman_;
	std::unique_ptr <MercantileSkill>	mercantile_;
	std::unique_ptr <MysticismSkill>	mysticism_;
	std::unique_ptr <RestorationSkill>	restoration_;
	std::unique_ptr <SecuritySkill>		security_;
	std::unique_ptr <SneakSkill>		sneak_;
	std::unique_ptr <SpeechCraftSkill>	speechCraft_;
};
#endif // _SKILL_HPP_
