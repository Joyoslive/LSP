#include "pch.h"
#include "TableOfTimes.h"

TableOfTimes::TableOfTimes()
{
	m_failGrade.grade = "Fail";
}

TableOfTimes::~TableOfTimes()
{

}

void TableOfTimes::addGrade(const std::string& grade, const float& requiredTime)
{
	Grade newGrade;
	newGrade.grade = grade;
	newGrade.requiredTime = requiredTime;
	m_grades.push_back(newGrade);
}

const Grade& TableOfTimes::getGrade(const float& timePassed)
{
	if (m_grades.size() == 0)
		assert(false);

	Grade* closestGrade = &m_grades[0];
	for (Grade& grade : m_grades)
	{
		if ((closestGrade->requiredTime > grade.requiredTime || timePassed > closestGrade->requiredTime) && timePassed <= grade.requiredTime)
			closestGrade = &grade;
	}

	if (timePassed > closestGrade->requiredTime)
		closestGrade = &m_failGrade;

	return *closestGrade;
}