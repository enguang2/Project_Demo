#ifndef bossAI_H
#define bossAI_H

#include "bossMaster.h"
#include "../../random.h"
#include "../missile.h"
#include <vector>

enum ATTACKS {RAYGUN, ROCKETS, NONE};

class bossAI : Object {
	private:
		vector<missile*> missiles;
		vector<missile*> rockets_drop;
		Timer innerTimer;
		Timer rocketDropTimer;
		bool busy;
		ATTACKS currentAttack;
		int innerStage;
		bossMaster* boss;
		terrain* ter;

		void laser();

	public:
		bool spawnRockets;
		bool isBusy();
		void update();
		void draw();
		void launchAttack(ATTACKS a);
		bossAI(GameWindow* gamewindow, terrain* ter);
		~bossAI();
};



#endif // bossAI_H
