#include <iostream>
#include <thread>
#include <chrono>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "mem/mem.hpp"
#include "game.hpp"

mem::process_t cstrike::process;
mem::moduleinfo_t cstrike::modules::hw;
mem::moduleinfo_t cstrike::modules::client;

bool is_key_down(KeySym key)
{
    Display* display = XOpenDisplay(NULL);
    char keymap[32];
    XQueryKeymap(display, keymap);
    KeyCode kc = XKeysymToKeycode(display, key);
    bool key_pressed = !!(keymap[kc >> 3] & (1 << (kc & 7)));
    XCloseDisplay(display);
    return key_pressed;
}

int main()
{
    std::cout << "<< cstrike_bhop by rdbo >>" << std::endl;
    std::cout << "[*] initializing..." << std::endl;
    cstrike::process = mem::ex::get_process("hl_linux");
    cstrike::modules::hw = mem::ex::get_module_info(cstrike::process, "hw.so");
    cstrike::modules::client = mem::ex::get_module_info(cstrike::process, "/client.so");
    if(cstrike::modules::hw.base == (mem::voidptr_t)-1 || cstrike::modules::client.base == (mem::voidptr_t)-1)
    {
        std::cout << "[!] unable to load game modules" << std::endl;
        return -1;
    }

    mem::voidptr_t force_jump = (mem::voidptr_t)((mem::uintptr_t)cstrike::modules::client.end + cstrike::offsets::dwForceJump);
    cstrike::game::player local_player;

    std::cout << "[*] cstrike::modules::client" << std::endl;
    std::cout << "    name: " << cstrike::modules::client.name << std::endl;
    std::cout << "    base: " << cstrike::modules::client.base << std::endl;
    std::cout << "    end: " << cstrike::modules::client.end << std::endl;

    std::cout << "[*] cstrike::modules::hw" << std::endl;
    std::cout << "    name: " << cstrike::modules::hw.name << std::endl;
    std::cout << "    base: " << cstrike::modules::hw.base << std::endl;
    std::cout << "    end: " << cstrike::modules::hw.end << std::endl;

    std::cout << "[*] initialized" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    while(true)
    {
        mem::ex::read(cstrike::process, (mem::voidptr_t)((mem::uintptr_t)cstrike::modules::hw.end + cstrike::offsets::dwLocalPlayer), &local_player, sizeof(local_player));
        std::cout << "[*] waiting..." << std::endl;
        if(is_key_down(cstrike::keys::bhop) && local_player.health > 0 && local_player.on_ground == 1)
        {
            std::cout << "[*] forcing jump..." << std::endl;
            mem::ex::write(cstrike::process, force_jump, (mem::byteptr_t)new mem::dword_t(6), sizeof(mem::dword_t));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}