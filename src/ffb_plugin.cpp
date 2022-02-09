
#include "ffb_plugin.h"


using namespace godot;

void ffb_plugin::_register_methods(){
    godot::register_method("_init", &ffb_plugin::_init);
    godot::register_method("init_ffb", &ffb_plugin::init_ffb);
    godot::register_method("init_constant_force_effect", &ffb_plugin::init_constant_force_effect);
    godot::register_method("update_constant_ffb_effect", &ffb_plugin::update_constant_ffb_effect);
    godot::register_method("play_constant_ffb_effect", &ffb_plugin::play_constant_ffb_effect);
    godot::register_method("destroy_ffb_effect", &ffb_plugin::destroy_ffb_effect);

    godot::register_property("force_feedback", &ffb_plugin::force_feedback, false);
//    godot::register_property("has_constant_force", &ffb_plugin::has_constant_force, false);
    godot::register_property("autocenter", &ffb_plugin::autocenter, 0);
    godot::register_property("cf_id", &ffb_plugin::cf_id, -1);

}

SDL_Haptic *haptic;
SDL_HapticEffect effect;

ffb_plugin::ffb_plugin(){
}


ffb_plugin::~ffb_plugin(){
    ffb_plugin::destroy_ffb_effect(0);
    SDL_HapticClose(haptic);
}


void ffb_plugin::_init(){
    ffb_plugin::init_ffb(0);
}

bool has_constant_force = false;


int ffb_plugin::init_ffb(int p_device){
	SDL_Joystick *joy;
//  Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	SDL_Init(SDL_INIT_HAPTIC);
//  Check for joystick
	if (SDL_NumJoysticks() > 0) {
//      Open joystick
		joy = SDL_JoystickOpen(p_device);
//		SDL_Haptic *haptic;
//		SDL_HapticEffect effect;
		haptic = SDL_HapticOpen(p_device);
		if (haptic == NULL){
			Godot::print("Cant open device, most likely joystick isn't haptic\n");
			force_feedback = false;
			return -1; // Most likely joystick isn't haptic

		} else {
			force_feedback = true;
		}

//      See if it can do constant force
		if ((SDL_HapticQuery(haptic) & SDL_HAPTIC_CONSTANT)==0) {
			SDL_HapticClose(haptic); // No Constant effect
			Godot::print("No constant force effect\n");
			has_constant_force = false;
			return -1;

		} else {
			has_constant_force = true;
			return 0;
		}

	} else {
        return -1;
	}
}


int ffb_plugin::init_constant_force_effect(){
	if (has_constant_force == false) {
		return -1;
	}

    int effect_id = -1;
    SDL_memset( &effect, 0, sizeof(SDL_HapticEffect) ); // 0 is safe default
    effect.type = SDL_HAPTIC_CONSTANT;
    effect.constant.direction.type = SDL_HAPTIC_CARTESIAN; // Cartesian coordinates
    effect.constant.direction.dir[0] = 1;
    effect.constant.direction.dir[1] = 0;
    effect.constant.level = 0;
    effect.constant.length = 0; //4294967295
    effect.constant.attack_length = 0;
    effect.constant.fade_length = 0;

    // Upload the effect
    effect_id = SDL_HapticNewEffect(haptic, &effect );
    if (effect_id < 0) {
        Godot::print("Error uploading effect ", SDL_GetError());
    }
//    cf_id = SDL_HapticNewEffect(haptic, &effect );
    // Test the effect
//    SDL_HapticRunEffect(haptic, effect_id, 1 );
    SDL_HapticRunEffect(haptic, effect_id, SDL_HAPTIC_INFINITY );
    return 0;
}


int ffb_plugin::update_constant_ffb_effect(float force, int effect_id){
    if (!force_feedback){
		return -1;

	} else if (!has_constant_force){
		return -1;
    }

//    SDL_HapticEffect effect;

//  clamp ffb force between -1 and 1
    if (force>1.0) force=1.0;
    else if (force<-1.0) force=-1.0;

    effect.constant.level = (short) (force * 32767.0);
    effect.constant.length = 4294967295;

    if (SDL_HapticUpdateEffect(haptic, effect_id, &effect) != 0) {
        return -1;
    }
    return 0;
}


//int ffb_plugin::play_constant_ffb_effect(int effect_id, Uint32 iterations){
int ffb_plugin::play_constant_ffb_effect(int effect_id, int iterations){
    if (!force_feedback){
		return -1;

	} else if (!has_constant_force){
		return -1;
    }

    if (iterations == 0){
        iterations = SDL_HAPTIC_INFINITY;
    }

    if (SDL_HapticRunEffect(haptic, effect_id, iterations) != 0){
        return -1;
    }
	return 0;
}


void ffb_plugin::destroy_ffb_effect(int effect_id){
//  Delete effect
    SDL_HapticDestroyEffect(haptic , effect_id);
}


void ffb_plugin::close_ffb_device(){
    SDL_HapticClose(haptic);
}
