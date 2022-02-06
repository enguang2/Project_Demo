#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "gamemaster.h"
#include "assets/terrain.h"
#include "assets/player.h"
#include "assets/projectile.h"
#include "assets/animation.h"
#include "assets/background.h"
#include "assets/BOSS/boss_AI.h"
//#include "phsicalObj.h"

#include "gamemaster.h"

#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"


using namespace std;

//trivial testcase for verification
TEST_CASE("testcase_check") {
    REQUIRE(true);
}

TEST_CASE("starter_new_window") {
        GameWindow* win = new GameWindow();
	win->init();
	REQUIRE(win);
}


TEST_CASE("starter_background") {
	GameWindow* win = new GameWindow();
	win->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	background* bg = new background(win);
	REQUIRE(bg);
}

TEST_CASE("starter_prep_obj") {
    	GameWindow* win = new GameWindow();
	win->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	background* bg = new background(win);
	terrain* o = new terrain(win);

	player* p = new player(win);

	boss_AI* b = new boss_AI(win, o);
	REQUIRE(bg);
	REQUIRE(o);
	REQUIRE(p);
	REQUIRE(b);
}

TEST_CASE("starter_animation_obj") {
        GameWindow* win = new GameWindow();
	win->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	background* bg = new background(win);
	terrain* o = new terrain(win);

	player* p = new player(win);

	boss_AI* b = new boss_AI(win, o);
	animation* onDeath = new animation(win);
	REQUIRE(onDeath);
}


TEST_CASE("starter_terrain") {
    	GameWindow* win = new GameWindow();
	win->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	//background* bg = new background(win);
	terrain* o = new terrain(win);
	REQUIRE(o);

}


TEST_CASE("starter_prep_player") {
    	GameWindow* win = new GameWindow();
	win->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	
	player* p = new player(win);


	REQUIRE(p);

}

TEST_CASE("starter_prep_BOSS") {
        GameWindow* win = new GameWindow();
	win->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	background* bg = new background(win);
	terrain* o = new terrain(win);

	player* p = new player(win);

	boss_AI* b = new boss_AI(win, o);
	//animation* onDeath = new animation(win);
	REQUIRE(b);
}


//deprecated testcases
/*
TEST_CASE("Phisical_Obj") {
        GameWindow* win = new GameWindow();
	win->init();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	background* bg = new background(win);
	terrain* o = new terrain(win);

	player* p = new player(win);

	boss_AI* b = new boss_AI(win, o);
	//animation* onDeath = new animation(win);
	float x = 100.0;
	float y = 120.0;
	PhsicalObj* obj = new PhsicalObj(x, y);
	REQUIRE(obj);
	REQUIRE(b);
}
*/

