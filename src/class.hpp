#ifndef _CLASS_HPP_
#define _CLASS_HPP_

class MobileBaseClass
{
public:
	MobileBaseClass();
	enum class_specialization { combat, stealth, magic };

public:
	SkillTree 			skills_;
	BasicAttributes 	basicAttributes_;
	DerivedAttributes 	derivedAttributes_;

protected:
	std::vector <std::string> favoredAttributes_;
	std::vector <std::string> majorSkills_;
};

class WarriorClass : public MobileBaseClass
{
public:
	WarriorClass();
};
#endif // _CLASS_HPP_
