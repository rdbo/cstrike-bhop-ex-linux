#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "mem/mem.hpp"

namespace cstrike
{
    extern mem::process_t process;

    namespace keys
    {
        const KeySym bhop = XK_space;
    }

    namespace offsets
    {
        //client.so (anon map):
        const mem::uintptr_t dwForceAttack = 0x25978;
        const mem::uintptr_t dwForceJump = 0x25988;

        //hw.so (anon map):
        const mem::uintptr_t dwLocalPlayer = 0x8f8280;

        //Player:
        const mem::uintptr_t dwHealth = 0x90;
        const mem::uintptr_t bOnGround = 0x54;

    }

    namespace modules
    {
        extern mem::moduleinfo_t hw;
        extern mem::moduleinfo_t client;
    }

    namespace game
    {
        class player
        {
            public:
            union
            {
                CREATE_UNION_MEMBER(mem::bool_t,  on_ground, cstrike::offsets::bOnGround);
                CREATE_UNION_MEMBER(mem::dword_t, health,    cstrike::offsets::dwHealth);
            };
        };

    }

}