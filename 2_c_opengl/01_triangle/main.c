#include <stdio.h>
#include <gl/glew.h>
#include <gl/freeglut.h>

typedef struct {    // Structure to represent a 3D coordinate
    float x, y, z;
} Vec3f;

enum { TRIANGLE, N_VBOs };
GLuint VBO[N_VBOs];         // Vertex Buffer Object

static const char* pVS =    // Vertex shader source code
"#version 330                                               \n"
"layout (location = 0) in vec3 Position;                    \n"
"                                                           \n"
"void main()                                                \n"
"{                                                          \n"
"    gl_Position = vec4(Position*0.1, 1.0);                 \n"
"}";

static const char* pFS =    // Fragment shader source code
"#version 330                                               \n"
"out vec4 FragColor;                                        \n"
"                                                           \n"
"void main()                                                \n"
"{                                                          \n"
"    FragColor = vec4(1.0, 0.0, 0.0, 1.0);                  \n"
"}";

void AddShader(GLuint shaderProg, const char* pShaderSrc, GLint ShaderType)
{   // Create shader
    GLuint shader = glCreateShader(ShaderType);
    if (!shader) {
        fprintf(stderr, "Error - Shader creation (%d)\n", ShaderType);
        exit(0);
    }
    // Compile shader
    const GLchar* src[1] = { pShaderSrc };
    const GLint len[1] = { strlen(pShaderSrc) };
    glShaderSource(shader, 1, src, len);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {       // Compilation error occurred
        GLchar infoLog[256];
        glGetShaderInfoLog(shader, 256, NULL, infoLog);
        fprintf(stderr, "Error - Shader compilation (%d): %s\n", ShaderType, infoLog);
        exit(1);
    }
    // Attach the compiled shader to the shader program
    glAttachShader(shaderProg, shader);
}

void SetUpShaders()
{   // Create shader program object
    GLuint shaderProg = glCreateProgram();
    if (!shaderProg) {
        fprintf(stderr, "Error - Shader program creation\n");
        exit(1);
    }

    // Load vertex and fragment shaders
    AddShader(shaderProg, pVS, GL_VERTEX_SHADER);
    AddShader(shaderProg, pFS, GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar errLog[256];

    glLinkProgram(shaderProg);      // Link shader program
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProg, sizeof(errLog), NULL, errLog);
        fprintf(stderr, "Error - Shader program linking: %s\n", errLog);
        exit(1);
    }

    glValidateProgram(shaderProg);  // Validate the program object
    glGetProgramiv(shaderProg, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProg, sizeof(errLog), NULL, errLog);
        fprintf(stderr, "Invalid shader program: %s\n", errLog);
        exit(1);
    }
    glUseProgram(shaderProg);       // Set the current shader program object
}

void RenderCB()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the screen to white

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[TRIANGLE]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glFinish();
}

void InitVBOs()
{
    Vec3f Vertices[3] = {       // Triangle vertex coordinates
        {-5.0f, -5.0f, 0.0f},
        {5.0f, -5.0f, 0.0f},
        {0.0f, 5.0f, 0.0f} };
    // Create a vertex buffer and pass the triangle vertex coordinates
    glGenBuffers(N_VBOs, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[TRIANGLE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL Sample");

    GLenum s = glewInit();
    if (s != GLEW_OK) {
        fprintf(stderr, "Error - %s\n", glewGetErrorString(s));
        return 1;
    }

    printf("GL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    SetUpShaders();
    InitVBOs();

    glutDisplayFunc(RenderCB);
    glutMainLoop();

    return 0;
}
