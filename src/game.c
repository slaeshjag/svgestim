#include "svgestim.h"
#include "game.h"
#include "map.h"
#include "bullet.h"

void game_init() {
	shape_init("models.stz");
	model.player=shapesprite_new("player walk", 2);
	model.gun=shape_load("gun");
	model.bullet=shape_load("bullet");
	model.grenade=shape_load("grenade");
	model.enemy[0]=shape_load("enemy");
	model.enemy[1]=shape_load("enemy_gunner");
	model.explosion=shape_load("explosion");
	
	sound.explosion=d_sound_streamed_load("explosion.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_STEREO);
	sound.jump=d_sound_streamed_load("jump.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_STEREO);
	sound.shoot=d_sound_streamed_load("shoot.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_STEREO);
	sound.enemy_kill=d_sound_streamed_load("enemy_kill.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_STEREO);
	sound.powerup=d_sound_streamed_load("powerup.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_STEREO);
	sound.powerup_small=d_sound_streamed_load("powerup_small.ogg", DARNIT_AUDIO_PRELOAD, DARNIT_AUDIO_STEREO);
	
	grenade_explosion=shape_copy_copy(model.explosion);
}

void game_handle(DARNIT_KEYS *keys, DARNIT_MOUSE *mouse) {
	if(keys->start||keys->select)
		gamestate(GAMESTATE_PAUSE);
	map_loop();
	player_loop(keys);
}

void game_render() {
	map_render();
	player_render();
	if(gamestate_current()==GAMESTATE_GAME)
		particle_emitter_loop();
}
