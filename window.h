#pragma once
#include <SDL3/SDL.h>
#include <cstdlib>

constexpr int SCR_SCL = 10;
constexpr int WIDTH = 64 * SCR_SCL;
constexpr int HEIGHT = 32 * SCR_SCL;

constexpr int SCREEN_FPS = 60;
constexpr int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
static uint64_t next_time;
struct Window {
    SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
    
    Memory mem{};


    void start() {
        std::vector<uint8_t> instr = read_bin("programs/space_invaders.ch8");
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


    uint64_t time_left() {
        uint64_t now = SDL_GetTicks();
        if(next_time <= now) {return 0;}
        return next_time - now;
    }
    void init() {
        if (!SDL_Init(SDL_INIT_VIDEO))
            {std::cout << "BRUH" << SDL_GetError();
			return;}
	
	
        win = SDL_CreateWindow("Chip 8 Emulator", WIDTH, HEIGHT, 0);
        renderer = SDL_CreateRenderer(win, "Chip 8 Emulator");
    }

    void run() {
        bool quit = false;
        next_time = SDL_GetTicks() + SCREEN_TICKS_PER_FRAME;

        while (!quit) {

            uint64_t now = SDL_GetTicks();

            SDL_Event e;
            if ( SDL_PollEvent(&e) ) {
                if (e.type == SDL_EVENT_QUIT){
                    quit = true;
                }
                else if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) {
                    quit = true;
                }
            }

            op();
            
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);

            SDL_Delay(time_left());
            next_time += SCREEN_TICKS_PER_FRAME;
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        
    }
};