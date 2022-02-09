
#ifndef FFB_PLUGIN_H
#define FFB_PLUGIN_H

#include "Godot.hpp"
#include "Node.hpp"
//#include "String.hpp"
//#include "Input.hpp"
//#include "Array.hpp"
//#include "File.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_haptic.h"
//#include "SDL.h"
//#include "SDL_haptic.h"


//~ #include <linux/input.h>
//~ #include <sys/ioctl.h>
//~ #include <fcntl.h>
//~ #include <unistd.h>

namespace godot{
class ffb_plugin : public Node
{
    GODOT_CLASS(ffb_plugin, Node)

    int autocenter       = 0;
    bool force_feedback  = false;


   // Ref <File> file;

	SDL_Haptic *haptic;
//	SDL_HapticEffect effect;

    public:
        static void _register_methods();
        ffb_plugin();
        ~ffb_plugin();
        void _init();
        int init_ffb(int p_device);
        int init_constant_force_effect();
        int update_constant_ffb_effect(float force);
        int play_constant_ffb_effect(int effect_id);
        int remove_ffb_effect();
};
}
#endif // FFB_PLUGIN_H
