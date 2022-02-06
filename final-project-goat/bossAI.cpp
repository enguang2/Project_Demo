#include "bossAI.h"

bossAI::bossAI(GameWindow* gamewindow, terrain* ter) : Object(gamewindow) {
	boss = new bossMaster(gw, ter);
	busy = false;
	currentAttack = NONE;
	innerStage = 0;
	this->ter = ter;
	for(int i=0; i<30; i++) {
		missiles.push_back(new missile(this->gw, 0, 0, ROCKET_SPEED, 0, false));
		missiles.at(i)->dead = true;
	}
	rocketDropTimer.start();
	spawnRockets = false;
}

bossAI::~bossAI() {
}

void bossAI::update() {
	boss->update();
	for(int i = missiles.size()-1; i>=0; i--) {
		 missiles.at(i)->update(ter);
	}
	for(int i = rockets_drop.size()-1; i>=0; i--) {
		 rockets_drop.at(i)->update(ter);
		 if(rockets_drop.at(i)->explosion->dead) {
			 missile* m = rockets_drop.at(i);
			 rockets_drop.erase(rockets_drop.begin() + i);
			 delete m;
			 i--;
		 }
	}

	if(rocketDropTimer.elapsedTime() > 500 && spawnRockets) {
		missile* m = new missile(this->gw, getRandInteger(300,LEVELWIDTH), 100, 0, ROCKET_SPEED*3, true);
		rockets_drop.push_back(m);
		rocketDropTimer.start();
	}

	if(currentAttack==RAYGUN){
			if(innerStage<=0) { //Move boss to position
				boss->bossNavigator->moveTo(TL);
				innerStage = 1;
			}
			else if(innerStage==1 && !boss->bossNavigator->isBusy()) { //Boss at position, go to next stage
				innerTimer.start();
				innerStage = 2;
			}
			else if(innerStage==2) {	//Move ufo to -20deg
				boss->ufo->setAngle(Lerp(0, -20, innerTimer.elapsedTime()/200.0f));
				if(innerTimer.elapsedTime()>=200.0) {
					boss->raygunStarted = true;
					boss->ufo->laser = true;
					
					innerTimer.start();
					innerStage = 3;
				}
			}
			else if(innerStage==3) {	//Move laser up to down
				boss->ufo->setAngle(Lerp(-20, 60, innerTimer.elapsedTime()/BOSS_RAYGUN_DURATION));
				if(innerTimer.elapsedTime()>=BOSS_RAYGUN_DURATION) {
					innerTimer.start();
					innerStage = 4;
				}
			}
			else if(innerStage==4) {	//Move laser back up
				boss->ufo->setAngle(Lerp(60, -20, innerTimer.elapsedTime()/BOSS_RAYGUN_DURATION));
				if(innerTimer.elapsedTime()>=BOSS_RAYGUN_DURATION) {
					boss->raygunStarted = false;
					boss->ufo->laser = false;
					innerTimer.start();
				
					boss->bossNavigator->moveTo(OUT);
					innerStage = 5;
				}
			}
			else if(innerStage==5) {	//Move ufo to default position and bugger off
				boss->ufo->setAngle(Lerp(-20, 0, innerTimer.elapsedTime()/500.0f));
				if(!boss->bossNavigator->isBusy()) innerStage = 6;
			}
			else if(innerStage==6) {	//Boss arrived outside, end attack
				launchAttack(NONE);
			}
			// break;
		}

		if(currentAttack==ROCKETS){
			if(innerStage<=0) { //Move boss to position
				boss->bossNavigator->moveTo(TL2);
				innerStage = 1;
			}
			else if(innerStage==1 && !boss->bossNavigator->isBusy()) { //Increase Tail movement
			
				boss->ufo->setdefaultPos(5.0, 2.0);
				innerTimer.start();
				//spawn rockets
				Vector2 spawnPos;
				spawnPos.set(-530, 700);
				for(int i=0; i<missiles.size()/3; i++) {missiles.at(i)->setPos(spawnPos); spawnPos.y += 80; missiles.at(i)->reset();}
				spawnPos.set(-930, 200);
				for(int i=missiles.size()/3; i<2*missiles.size()/3; i++) {missiles.at(i)->setPos(spawnPos); spawnPos.y += 80; missiles.at(i)->reset();}
				spawnPos.set(-1330, 700);
				for(int i=2*missiles.size()/3; i<missiles.size(); i++) {missiles.at(i)->setPos(spawnPos); spawnPos.y += 80; missiles.at(i)->reset();}
				innerStage = 2;
			}
			else if(innerStage==2 && innerTimer.elapsedTime()>5000.0f) {	//bugger off
				
				boss->ufo->setdefaultPos(1.0, 1.0);
				boss->bossNavigator->moveTo(OUT);
				innerStage = 5;
				innerTimer.start();
			}
			else if(innerStage==5) {	//Move ufo to default position and bugger off
				boss->ufo->setAngle(Lerp(0, -120, innerTimer.elapsedTime()/500.0f));
				if(!boss->bossNavigator->isBusy()) innerStage = 6;
			}
			else if(innerStage==6 && !boss->bossNavigator->isBusy()) {	//Boss arrived outside, end attack
				boss->ufo->setAngle(0);
				launchAttack(NONE);
			}
			// break;
		// }

		// default : {
		// 	break;
		// }

	}

}

void bossAI::launchAttack(ATTACKS a) {
	currentAttack = a;
	innerStage = 0;
}

void bossAI::draw() {
	for(int i = missiles.size()-1; i>=0; i--) {
		missiles.at(i)->draw();
	}
	for(int i = rockets_drop.size()-1; i>=0; i--) {
		rockets_drop.at(i)->draw();
	}
	boss->draw();
}
