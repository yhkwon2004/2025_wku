#include <GL/glew.h>
#include <glut.h>	
#include <stdio.h>
#include <math.h>
#pragma comment (lib, "glew32.lib")
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const char* vertex_shader = "#version 460 core\n \
layout (location = 0) in vec3 aPos; \
uniform mat4 projection; \
void main() { \
  gl_Position = projection * vec4(aPos, 1.0);\
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

void MyReshape(int NewWidth, int NewHeight) {
    glViewport(0, 0, NewWidth, NewHeight);
    GLfloat aspect_ratio = (GLfloat)NewWidth / (GLfloat)NewHeight;

    glUseProgram(shader_program);
    glm::mat4 projection;
    projection = glm::ortho(-1.0 * aspect_ratio, 1.0 * aspect_ratio, -1.0, 1.0, -1.0, 1.0);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]);
}



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
    glutReshapeFunc(MyReshape);


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
        // 
        //��1
        0.0f,   0.0f,  0.0f,
        0.5f,   0.0f,  0.0f,
        0.25f,  0.43f, 0.0f,

        //��2
        0.0f,   0.0f,  0.0f,
        0.25f,  0.43f, 0.0f,
        -0.25f,  0.43f, 0.0f,

        //��3
        0.0f,   0.0f,  0.0f,
        -0.25f,  0.43f, 0.0f,
        -0.5f,  0.0f,  0.0f,


        //��4
        0.0f,   0.0f,  0.0f,
        -0.5f,  0.0f,  0.0f,
        -0.25f, -0.43f, 0.0f,


        //��5
        0.0f,   0.0f,  0.0f,
        -0.25f, -0.43f, 0.0f,
        0.25f, -0.43f, 0.0f,


        //��6
        0.0f,   0.0f,  0.0f,
        0.25f, -0.43f, 0.0f,
        0.5f,   0.0f,  0.0f
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