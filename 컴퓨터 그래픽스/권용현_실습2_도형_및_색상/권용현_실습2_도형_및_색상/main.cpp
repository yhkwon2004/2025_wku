#include <GL/glew.h>
#include <glut.h>	
#include <stdio.h>
#include <math.h>

#pragma comment (lib, "glew32.lib")

const char* vertex_shader = "#version 460 core\n \
layout (location = 0) in vec3 aPos; \
void main() { \
  gl_Position = vec4(aPos, 1.0); \
}";

const char* fragment_shader = "#version 460 core\n \
out vec4 FragColor; \
uniform vec3 aColor; \
void main() { \
  FragColor = vec4(1.0, 0.7, 0.6, 1.0); \
}";

GLuint shader_program;
GLuint VAO = 0;
//20232301 �ǿ��� ���� �ߺ� ����
void MyDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 18); //���� �ﰢ�� 3 -> ������ ��* 3 ex) (�簢�� 6 , ������ 9 , ������ 18)
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Purple Hexagon");
    glewInit();
    glutDisplayFunc(MyDisplay);


    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        printf("Vertex Shader Compilation Error : %s\n", infoLog);
    }
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        printf("Fragment Shader Compilation Error  : %s\n", infoLog);
    }
    shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    GLfloat vertices[] = { // ������ ��ǥ

        //���ڸ������ ������ ��ġ
        0.0f,  0.5f,  0.0f,
        -0.25f,  0.25f,  0.0f,
        0.25f,  0.25f,  0.0f,

        // ��2 (���� �Ʒ� �ﰢ��)
        -0.25f,  0.25f,  0.0f,
        -0.5f,   0.0f,   0.0f,
        -0.25f, -0.25f,  0.0f,

        // ��3 (������ �Ʒ� �ﰢ��)
        0.25f,  0.25f,  0.0f,
        0.25f, -0.25f,  0.0f,
        0.5f,   0.0f,   0.0f,

        // ��4 (�Ʒ��� �ﰢ��)
        0.0f,  -0.5f,  0.0f,
        -0.25f, -0.25f,  0.0f,
        0.25f, -0.25f,  0.0f,
    };

    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glutMainLoop();
    return 0;
}