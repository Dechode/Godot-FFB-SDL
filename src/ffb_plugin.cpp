//#define SDL_MAIN_HANDLED
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_haptic.h>
//#include <SDL2/SDL_joystick.h>

#include "ffb_plugin.h"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;

void FFBPlugin::_bind_methods(){
	ClassDB::bind_method(D_METHOD("_init"), &FFBPlugin::_init);
    ClassDB::bind_method(D_METHOD("init_ffb"), &FFBPlugin::init_ffb);
    ClassDB::bind_method(D_METHOD("init_constant_force_effect"), &FFBPlugin::init_constant_force_effect);
    ClassDB::bind_method(D_METHOD("update_constant_force_effect"), &FFBPlugin::update_constant_ffb_effect);
    ClassDB::bind_method(D_METHOD("play_constant_force_effect"), &FFBPlugin::play_constant_ffb_effect);
    ClassDB::bind_method(D_METHOD("destroy_ffb_effect"), &FFBPlugin::destroy_ffb_effect);
    ClassDB::bind_method(D_METHOD("close_ffb_device"), &FFBPlugin::close_ffb_device);

}

FFBPlugin::FFBPlugin(){
    // FFBPlugin::init_ffb(0);
}

FFBPlugin::~FFBPlugin(){
    FFBPlugin::destroy_ffb_effect(0);
    SDL_HapticClose(haptic);
}

void FFBPlugin::_init(){
}

int FFBPlugin::init_ffb(int p_device){
//  Initialize the joystick subsystem and haptic system
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	SDL_Init(SDL_INIT_HAPTIC);
	if (SDL_NumJoysticks() <= 0) {
		UtilityFunctions::print("No joysticks found!");
        //return -1;
	}

    joy = SDL_JoystickOpen(p_device);
    if (joy == NULL) {
		UtilityFunctions::print("Cant open joy");
		//return -1;
        
    }

    if (SDL_JoystickIsHaptic(joy) == SDL_FALSE) {
        UtilityFunctions::print("Joy isnt haptic");
        //return -1;
    }

    //haptic = SDL_HapticOpen(p_device);
    haptic = SDL_HapticOpenFromJoystick(joy);
    if (haptic == NULL){
        UtilityFunctions::print("Cant open haptic from joy, trying from device\n");
		haptic = SDL_HapticOpen(p_device);
		if (haptic == NULL){
        UtilityFunctions::print("Cant open device, most likely joystick isn't haptic\n");
        force_feedback = false;
        //return -1;
		} else {force_feedback = true;}
	} else {force_feedback = true;}


	if (!force_feedback)  {
		UtilityFunctions::print("No force feedback found");
		return -1;
	}

//      See if it can do constant force
    if ((SDL_HapticQuery(haptic) & SDL_HAPTIC_CONSTANT)==0) {
        SDL_HapticClose(haptic); // No Constant effect
        UtilityFunctions::print("No constant force effect capabilities found!\n");
        constant_force = false;
        return -1;
    }
    constant_force = true;
    return 0;
}

int FFBPlugin::init_constant_force_effect(){
	if (constant_force == false) {
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

    //Godot::print("Effect_id = " + String::num_int64(effect_id));
    return effect_id;
}

int FFBPlugin::update_constant_ffb_effect(float force, int length, int effect_id){
    if (!force_feedback || !constant_force || effect_id == -1){
		return -1;
	}

//  clamp ffb force between -1 and 1
    if (force>1.0) force = 1.0;
    else if (force<-1.0) force = -1.0;

//  Use sdl infinity if length == 0
    if (length == 0) {
        length = SDL_HAPTIC_INFINITY;
    }

    effect.constant.level = (short) (force * 32767.0);
    effect.constant.length = length;

    if (SDL_HapticUpdateEffect(haptic, effect_id, &effect) != 0) {
        return -1;
    }
    return 0;
}

int FFBPlugin::play_constant_ffb_effect(int effect_id, int iterations){
//~ int FFBPlugin::play_constant_ffb_effect(int effect_id, int iterations){
    if (!force_feedback || !constant_force || effect_id == -1){
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

void FFBPlugin::destroy_ffb_effect(int effect_id){
    SDL_HapticDestroyEffect(haptic ,effect_id);
}

void FFBPlugin::close_ffb_device(){
    SDL_JoystickClose(joy);
    SDL_HapticClose(haptic);
}

