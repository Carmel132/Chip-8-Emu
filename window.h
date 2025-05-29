#pragma once
#include <SDL3/SDL.h>
#include "resource/keyboard.h"
#include <cstdlib>

constexpr int SCR_SCL = 35;
constexpr int WIDTH = 64 * SCR_SCL;
constexpr int HEIGHT = 32 * SCR_SCL;

int CPU_HZ = 800;
#define MS_PER_CPU_CYCLE 1000/CPU_HZ

constexpr int SCREEN_FPS = 60;
constexpr int MS_PER_TIMER_CYLE = 1000 / SCREEN_FPS;

void store_key_input(Memory* mem, SDL_Keycode key) {

}

struct Window {
    SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
    
    Memory mem{};

    void start() {
        std::vector<uint8_t> instr = read_bin("programs/breakout.ch8");
        load_program_bytes(&mem, instr);
    }

    void frame() {
        // Fetch
        uint16_t instruction = get_instr_at_PC(&mem);
        //std::cout << std::hex << instruction << "\n";
        mem.program_counter += 2;
        // Decode
        interpret_instruction(instruction, & mem);
        
    }

    void render_square(SDL_Renderer* renderer, int x, int y) {
        float f_x = (float)x, f_y = (float)y;
        
        SDL_FRect r{f_x, f_y, SCR_SCL, SCR_SCL};
        SDL_RenderFillRect(renderer, &r);
    }

    void render_screen() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 32; j++) {
                bool p = mem.graphic[j][i];
                if (p) {
                    render_square(renderer, i * SCR_SCL , j * SCR_SCL);
                }
            }
        }
    }

    void init() {
        if (!SDL_Init(SDL_INIT_VIDEO)){
            std::cout << "BRUH" << SDL_GetError();
			return;
        }
	
	
        win = SDL_CreateWindow("Chip 8 Emulator", WIDTH, HEIGHT, 0);
        renderer = SDL_CreateRenderer(win, NULL);
    }

    void run() {
        bool quit = false;
        double cpu_acc{}, timer_acc{};
        uint64_t last_time = SDL_GetPerformanceCounter();

        while (!quit) {
            Uint64 start = SDL_GetPerformanceCounter();
            double change_in_time = (start - last_time) * 1000.0 / SDL_GetPerformanceFrequency();
            last_time = start;

            cpu_acc += change_in_time;
            timer_acc += change_in_time;

            while (cpu_acc >= MS_PER_CPU_CYCLE) {
                frame();
                cpu_acc -= MS_PER_CPU_CYCLE;
            }

            if (timer_acc >= MS_PER_TIMER_CYLE) {
                tick(&mem);
                if (mem.reloadGraphics) {
                    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)) {
                    std::cout << SDL_GetError() << "\n lmfao";
                    std::cout << "line";
                    }
                    SDL_RenderClear(renderer);
                    render_screen();    
                    SDL_RenderPresent(renderer);
                    mem.reloadGraphics = false;
                }
                
                
                timer_acc -= MS_PER_TIMER_CYLE;
            }

            SDL_Event e;
            if ( SDL_PollEvent(&e) ) {
                if (e.type == SDL_EVENT_QUIT){
                    quit = true;
                }
                else if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) {
                    quit = true;
                }
                else if (e.type == SDL_EVENT_KEY_DOWN) {
                    uint8_t key = key_lookup(e.key.key);
                    if (key != 0xFF) {mem.keyboard[key] = 1;}
                }
                else if (e.type == SDL_EVENT_KEY_UP) {
                    uint8_t key = key_lookup(e.key.key);
                    if (key != 0xFF) {mem.keyboard[key] = 0;}
                }
                else if (e.type == SDL_EVENT_MOUSE_WHEEL) {
                    CPU_HZ += e.wheel.integer_y;
                    std::cout << CPU_HZ << "\n";
                }
            }
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        
    }
};