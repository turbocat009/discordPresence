/*
 * SDL2 Tile Animator
 *
 * This program loads a PNG spritesheet, divides it into tiles based on
 * a specified tile size, and animates through the tiles.
 *
 * Dependencies: You must have the SDL2 and SDL2_image libraries installed.
 *
 * Compilation (example on Linux/macOS):
 * g++ main.cpp -o animator -lSDL2 -lSDL2_image
 *
 * Usage:
 * 1. Place your spritesheet (e.g., "my_sprites.png") in the same directory.
 * 2. Update the CONFIGURATION constants below to match your file and tile size.
 * 3. Compile and run.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>

// --- CONFIGURATION ---
// Set these values to match your spritesheet

// Path to your PNG spritesheet
const std::string PNG_FILE_PATH = "MainMenu.png";

// The width of a single tile/frame in your spritesheet
const int TILE_WIDTH = 512;

// The height of a single tile/frame in your spritesheet
const int TILE_HEIGHT = 512;

// Desired animation speed in milliseconds per frame
const int FRAME_DURATION_MS = 100;

// --- END CONFIGURATION ---

// Screen dimensions (can be anything you like)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Global variables
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gSpriteSheetTexture = NULL;

// A vector to hold all the clipping rectangles for each tile
std::vector<SDL_Rect> gSpriteClips;

// Function to initialize SDL and create window
bool init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("SDL2 Tile Animator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create a VSync-enabled renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
    {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize PNG loading (part of SDL_image)
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

// Function to load media (our spritesheet)
bool loadMedia()
{
    // Load sprite sheet texture
    gSpriteSheetTexture = IMG_LoadTexture(gRenderer, PNG_FILE_PATH.c_str());
    if (gSpriteSheetTexture == NULL)
    {
        std::cerr << "Failed to load texture image " << PNG_FILE_PATH << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Get the dimensions of the loaded texture
    int sheetWidth, sheetHeight;
    SDL_QueryTexture(gSpriteSheetTexture, NULL, NULL, &sheetWidth, &sheetHeight);

    // Check if tile dimensions are valid
    if (TILE_WIDTH <= 0 || TILE_HEIGHT <= 0)
    {
        std::cerr << "Error: TILE_WIDTH and TILE_HEIGHT must be greater than 0." << std::endl;
        return false;
    }
    if (sheetWidth < TILE_WIDTH || sheetHeight < TILE_HEIGHT)
    {
        std::cerr << "Error: Spritesheet dimensions (" << sheetWidth << "x" << sheetHeight
                  << ") are smaller than tile dimensions (" << TILE_WIDTH << "x" << TILE_HEIGHT << ")." << std::endl;
        return false;
    }

    // Calculate number of columns and rows
    int cols = sheetWidth / TILE_WIDTH;
    int rows = sheetHeight / TILE_HEIGHT;

    std::cout << "Loaded spritesheet: " << sheetWidth << "x" << sheetHeight << std::endl;
    std::cout << "Tile size: " << TILE_WIDTH << "x" << TILE_HEIGHT << std::endl;
    std::cout << "Found " << rows << " rows and " << cols << " columns." << std::endl;
    std::cout << "Total frames: " << (rows * cols) << std::endl;

    // Go through the spritesheet and create a clip rectangle for each tile
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            SDL_Rect clipRect;
            clipRect.x = x * TILE_WIDTH;
            clipRect.y = y * TILE_HEIGHT;
            clipRect.w = TILE_WIDTH;
            clipRect.h = TILE_HEIGHT;
            gSpriteClips.push_back(clipRect);
        }
    }

    // Check if any clips were actually created
    if (gSpriteClips.empty())
    {
        std::cerr << "Error: No sprite clips were generated. Check TILE_WIDTH and TILE_HEIGHT." << std::endl;
        return false;
    }

    return true;
}

// Function to clean up resources
void close()
{
    // Free loaded image
    SDL_DestroyTexture(gSpriteSheetTexture);
    gSpriteSheetTexture = NULL;

    // Destroy window and renderer
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *args[])
{
    // Start up SDL and create window
    if (!init())
    {
        std::cerr << "Failed to initialize!" << std::endl;
        close();
        return 1;
    }

    // Load media
    if (!loadMedia())
    {
        std::cerr << "Failed to load media!" << std::endl;
        close();
        return 1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Animation variables
    int currentFrame = 0;
    Uint32 lastFrameTime = SDL_GetTicks();

    // The destination rectangle on the screen
    // This will center the tile and scale it up 2x
    int renderWidth = TILE_WIDTH * 2;
    int renderHeight = TILE_HEIGHT * 2;
    SDL_Rect destRect;
    destRect.x = (SCREEN_WIDTH - renderWidth) / 2;
    destRect.y = (SCREEN_HEIGHT - renderHeight) / 2;
    destRect.w = renderWidth;
    destRect.h = renderHeight;

    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // --- Animation Logic ---
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + FRAME_DURATION_MS)
        {
            // Move to the next frame
            currentFrame++;
            // Wrap around to the beginning if we've reached the end
            if (currentFrame >= gSpriteClips.size())
            {
                currentFrame = 0;
            }
            // Update the time of the last frame change
            lastFrameTime = currentTime;
        }

        // --- Rendering ---

        // Clear screen (set to a light gray)
        SDL_SetRenderDrawColor(gRenderer, 0xE0, 0xE0, 0xE0, 0xFF);
        SDL_RenderClear(gRenderer);

        // Get the clip rectangle for the current frame
        SDL_Rect *currentClip = &gSpriteClips[currentFrame];

        // Render the current tile to the screen
        SDL_RenderCopy(gRenderer, gSpriteSheetTexture, currentClip, &destRect);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    // Free resources and close SDL
    close();

    return 0;
}
