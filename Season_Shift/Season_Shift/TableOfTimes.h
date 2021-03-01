#pragma once

struct Grade
{
	double requiredTime;
	std::string grade;
};

class TableOfTimes
{
private:
	std::vector<Grade> m_grades;
	Grade m_failGrade;
public:
	TableOfTimes();
	~TableOfTimes();

	void addGrade(const std::string& grade, const float& requiredTime);
	const Grade& getGrade(const float& timePassed);
};

