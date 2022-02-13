
#ifndef FFB_PLUGIN_H
#define FFB_PLUGIN_H

#include "Godot.hpp"
#include "Node.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_haptic.h"


namespace godot{
class ffb_plugin : public Node
{
    GODOT_CLASS(ffb_plugin, Node)

    int autocenter          = 0;
    bool force_feedback     = false;

    public:
        static void _register_methods();
        ffb_plugin();
        ~ffb_plugin();
        void _init();
        int init_ffb(int p_device);
        int init_constant_force_effect();
//        int update_constant_ffb_effect(float force, int effect_id);
        int update_constant_ffb_effect(float force, int length, int effect_id);
        int play_constant_ffb_effect(int effect_id, Uint32 iterations);
        //~ int play_constant_ffb_effect(int effect_id, int iterations);
        void destroy_ffb_effect(int effect_id);
        void close_ffb_device();
};
}
#endif // FFB_PLUGIN_H
