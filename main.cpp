#include <stdio.h>
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <string>
#include <iostream>
#define CODE(...) #__VA_ARGS__

SDL_Window* mainwindow;
using namespace std;

bool pollEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
            //Programm wird beendet
        case SDL_QUIT:
            return false;
        default:
            break;
        }
    }
    return true;
}

extern int main(int argc, char* argv[]) {
    //SDL initialisieren
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    //OpenGL-Kontext erstellen (Version 4.1, Core Profile)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //Fenster erstellen
    mainwindow = SDL_CreateWindow(
        "OpenGl Test", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        640, 
        480, 
        SDL_WINDOW_OPENGL 
    );
    //Hintergrunfarbe festlegen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //Fenster leeren
    glClear(GL_COLOR_BUFFER_BIT);
    //Front- und Back-Buffer tauschen
    SDL_GL_SwapWindow(mainwindow);

    //Nach Events fragen
    while (true) {
        //Nach Events fragen
        if (!pollEvent()) break;
    }

    //Speicher leeren
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
    return 0;
}