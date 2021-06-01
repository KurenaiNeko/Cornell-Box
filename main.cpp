#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

SDL_Window* window;

GLuint LoadShader(GLenum type, const GLchar* vs_code) {
    //Shader erzeugen (Vertex Shader)
    GLuint vertexShader = glCreateShader(type);
    if (vertexShader == 0)
    {
        return 0;
    }
    //Shader-Source zuweisen und kompilieren
    glShaderSource(vertexShader, 1, &vs_code, NULL);
    glCompileShader(vertexShader);

    //Fehlerbehandlung für Shader
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " <<
            infoLog << std::endl;
    }
    return vertexShader;
}
GLuint shaderProgram_;
GLuint VBO_;
GLuint VAO_;

bool Init() {
    //Vertex-Shader-Code
    const GLchar* vs_code =
        "#version 410 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";
    //Fragment-Shader-Code
    const GLchar* fs_code =
        "#version 410 core\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "color = vec3(1.0, 0.0, 0.0);\n"
        "}\n\0";


    GLuint vertexShader;
    GLuint fragmentShader;
    vertexShader = LoadShader(GL_VERTEX_SHADER, vs_code);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fs_code);
    if (vertexShader == 0 || fragmentShader == 0)
    {
        return false;
    }
    //Shader-Programm erzeugen
    GLuint shaderProgram = glCreateProgram();
    //Shader zuweisen und Programm linken
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindAttribLocation(shaderProgram, 0, "vPosition");
    glLinkProgram(shaderProgram);

    //Fehlerbehandlung für Shader-Programme
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << infoLog << std::endl;
        glDeleteProgram(shaderProgram);
        return false;
    }
    shaderProgram_ = shaderProgram;

    //Dreieck Koordinaten
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,     // Left  
        0.0f, -1.0f, 0.0f,      // Right 
        -0.5f, 0.0f, 0.0f,       // Top  

        1.0f, 1.0f, 0.0f,       // Left  
        0.0f, 1.0f, 0.0f,       // Right 
        0.5f, 0.0f, 0.0f,        // Top   
    };



    //VAO,VBO erstellen
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    //VBO binden
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //VBO füllen
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
        GL_STATIC_DRAW);
    //VAO binden
    glBindVertexArray(VAO);
    //Vertex Daten einem Array-Index zuordnen  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    VBO_ = VBO;
    VAO_ = VAO;
    return true;
}

void Draw() {
    //Hintergrundfarbe festlegen
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    //Fenster leeren
    glClear(GL_COLOR_BUFFER_BIT);
    //Shader-Programm für Draw-Call verwenden
    glUseProgram(shaderProgram_);
    //VAO aktivieren
    glBindVertexArray(VAO_);
    //Dreieck zeichnen
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //Front- und Back-Buffer tauschen
    SDL_GL_SwapWindow(window);
}



int main(int argc, char* argv[])
{
    //SDL initialisieren
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL could not initialize!SDL Error :" <<
            std::string(SDL_GetError());

        return EXIT_FAILURE;
    }
    //OpenGL 4.1 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //Fenster erstellen
    window = SDL_CreateWindow("Triangle",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL);
    //OpenGL-Kontext erstellen
    SDL_GLContext context = SDL_GL_CreateContext(window);
    //Event erstellen
    SDL_Event windowEvent;
    //GLEW initialisieren
    glewExperimental = GL_TRUE;

    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        std::cout << "Failed to initialize GLEW: " <<
            glewGetErrorString(glewError) << std::endl;

        return EXIT_FAILURE;
    }

    if (!Init())
    {
        return 0;
    }

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
        }
        Draw();

    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}