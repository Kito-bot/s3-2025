/*
// Image loader functions
// Author: Hafid HOUSNI
*/

#include "loader.h"

// Clean ressources used by SDL
/*void clean_resources(SDL_cond *w, SDL_Renderer *r, SDL_Texture *t)
{
	if(t != NULL)
		SDL_DestroyTexture(t);

    if(r != NULL)
        SDL_DestroyRenderer(r);
    
    if(w != NULL)
        SDL_DestroyWindow(w);

    SDL_Quit();
}*/

// Initialize SDL
void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Could not initialize SDL : %s\n", SDL_GetError());
        clean_resources(NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }
}

// Load an image
SDL_Surface *load_img(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
    {
        SDL_Log("Cannot load %s: %s\n", path, IMG_GetError());
        clean_resources(NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    return img;
}

// Load an image and display it
int loader(char *file)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    SDL_Surface *image = load_img(file);

    SDL_Rect rect = {0, 0, image->w, image->h};

    init_sdl();

    window = SDL_CreateWindow("Display Image", 
       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        image->w, image->h, SDL_WINDOW_RESIZABLE);
    if(window == NULL)
    {
        SDL_Log("ERROR > %s\n", SDL_GetError());
        clean_resources(NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL)
    {
        SDL_Log("ERROR > %s\n", SDL_GetError());
        clean_resources(window, NULL, NULL);
        exit(EXIT_FAILURE);
    }   

    if(image == NULL)
    {
        SDL_Log("ERROR > %s\n", SDL_GetError());
        clean_resources(window, renderer, NULL);
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    if(texture == NULL)
    {
        SDL_Log("ERROR > %s\n", SDL_GetError());
        clean_resources(window, renderer, NULL);
        exit(EXIT_FAILURE);
    }

    if(SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h) != 0)
    {
        SDL_Log("ERROR > %s\n", SDL_GetError());
        clean_resources(window, renderer, texture);
        exit(EXIT_FAILURE);
    }

    if(SDL_RenderCopy(renderer, texture, NULL, &rect) != 0)
    {
        SDL_Log("ERROR > %s\n", SDL_GetError());
        clean_resources(window, renderer, texture);
        exit(EXIT_FAILURE); 
    }

    SDL_RenderPresent(renderer);

    int keep = 1;
    SDL_Event event;
    while(keep)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                keep = 0;
                break;
        }
    }

    clean_resources(window, renderer, texture);

    return EXIT_SUCCESS;
}