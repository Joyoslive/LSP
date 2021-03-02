#include "pch.h"
#include "GoalLogic.h"
#include "Collider.h"
#include "GameObject.h"
#include "TableOfTimes.h"
#include "Player.h"


GoalLogic::GoalLogic()
{
	m_myTimes = std::make_shared<TableOfTimes>();
}

const Ref<TableOfTimes>& GoalLogic::getTableOfTimes() const
{
	return m_myTimes;
}

char msgbuf[1000];
void GoalLogic::writeGrade(const Ref<Player>& playerLogic)
{
	long double time = playerLogic->goalTimerGetTime();
	Grade grade = m_myTimes->getGrade(time);
	//Temporary
	sprintf_s(msgbuf, "\n------------------------------------\nYour Grade was %s and Your Time was %f\n------------------------------------\n", grade.grade.c_str(), time);
	OutputDebugStringA(msgbuf);
}

void GoalLogic::onCollision(Ref<Collider> collider)
{
	if (collider->getGameObject()->getName() == "player")
	{
		writeGrade(collider->getGameObject()->getComponentType<Player>(Component::ComponentEnum::LOGIC));
	}
}