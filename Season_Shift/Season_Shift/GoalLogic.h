#pragma once
#include "Logic.h"

class TableOfTimes;
class Player;
class GoalLogic : public Logic
{
private:
	Ref<TableOfTimes> m_myTimes;
private:
	void writeGrade(const Ref<Player>& playerLogic);
public:
	GoalLogic();
	~GoalLogic() = default;
	void onCollision(Ref<Collider> collider) override;
	const Ref<TableOfTimes>& getTableOfTimes() const;
};

