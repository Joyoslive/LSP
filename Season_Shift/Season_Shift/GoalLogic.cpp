#include "pch.h"
#include "GoalLogic.h"
#include "Collider.h"
#include "GameObject.h"
#include "TableOfTimes.h"
#include "Player.h"
#include "ResultMenu.h"


GoalLogic::GoalLogic()
{
	m_myTimes = std::make_shared<TableOfTimes>();
}

const Ref<TableOfTimes>& GoalLogic::getTableOfTimes() const
{
	return m_myTimes;
}

//char msgbuf[1000];
void GoalLogic::writeGrade(const Ref<Player>& playerLogic)
{
	long double time = playerLogic->goalTimerGetTime();
	Grade grade = m_myTimes->getGrade(static_cast<float>(time));
	//Temporary (hook up with text later)
	//sprintf_s(msgbuf, "\n------------------------------------\nYour Grade was %s and Your Time was %f\n------------------------------------\n", grade.grade.c_str(), time);
	//OutputDebugStringA(msgbuf);

	m_gameObject->getScene()->getResultMenu()->playerReachedGoal(time, grade.grade);
}

void GoalLogic::onCollision(Ref<Collider> collider)
{
	if (collider->getGameObject()->getName() == "player")
	{
		writeGrade(collider->getGameObject()->getComponentType<Player>(Component::ComponentEnum::LOGIC));
	}
}