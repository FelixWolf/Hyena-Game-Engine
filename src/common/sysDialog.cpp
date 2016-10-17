#include <SDL2/SDL.h>
namespace dialog{
    void error(const char* source, const char* message){    
        fprintf(stderr, "[%s] %s\n", source, message);
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            source,
            message,
            NULL
        );
    }
    
    void info(const char* source, const char* message){    
        fprintf(stdout, "[%s] %s\n", source, message);
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_INFORMATION,
            source,
            message,
            NULL
        );
    }
    
    void warn(const char* source, const char* message){    
        fprintf(stdout, "[%s] %s\n", source, message);
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_WARNING,
            source,
            message,
            NULL
        );
    }
}
