#ifndef _GAME_UI_INCLUDE
#define _GAME_UI_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Player.h"

#define N_CHARS_TRIES	2
#define N_CHARS_SCORE	4
#define N_CHARS_TIME	4

class GameUI
{

public:
	GameUI();
	~GameUI();

	static GameUI* createGameUI();

	void init(ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPlayer(Player *p);

private:
	Texture bg_sheet, lives_sheet, numbers_sheet;
	Sprite *bg, *lives;
	vector<Sprite*> tries;
	vector<Sprite*> score;
	vector<Sprite*> time;

	Player *player;

	float timer;
};


#endif // _GAME_UI_INCLUDE
