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
	for (int i = 1; i < m_grades.size(); ++i)
	{
		if ((closestGrade->requiredTime > m_grades[i].requiredTime || timePassed > closestGrade->requiredTime) && timePassed <= m_grades[i].requiredTime)
			closestGrade = &m_grades[i];
	}

	if (timePassed > closestGrade->requiredTime)
		closestGrade = &m_failGrade;

	return *closestGrade;
}