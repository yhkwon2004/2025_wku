#include <GL/glew.h>
#include <glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment (lib, "glew32.lib")

const char* vertex_shader = "#version 460 core\n \
in vec3 aPos; \
uniform mat4 projection; \
uniform mat4 transform; \
void main() { \
    gl_Position = projection * transform * vec4(aPos, 1.0);\
}";

const char* fragment_shader = "#version 460 core\n \
out vec4 FragColor; \
uniform vec3 aColor; \
void main() \
{\
    FragColor = vec4(aColor, 1.0);   \
}";

GLuint shader_program;
GLuint VAO, VAO2, VAO3;

GLfloat Delta = 0.0f, offset = 0.0f;
bool bullet_started = false;
GLfloat bx = 0.0f, by = -0.75f;
GLfloat xdiff = 0.02f, ydiff = 0.02f;

GLfloat block_x[8] = { -0.6, -0.2, 0.2, 0.6, -0.6, -0.2, 0.2, 0.6 };
GLfloat block_y[8] = { 0.8, 0.8, 0.8, 0.8, 0.6, 0.6, 0.6, 0.6 };
GLint block_on[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

void MyDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(Delta, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform3f(glGetUniformLocation(shader_program, "aColor"), 0.0f, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    transform = glm::translate(glm::mat4(1.0f), glm::vec3(bx, by, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform3f(glGetUniformLocation(shader_program, "aColor"), 1.0f, 0.0f, 0.0f);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    for (int i = 0; i < 8; i++) {
        if (block_on[i] == 1) {
            transform = glm::translate(glm::mat4(1.0f), glm::vec3(block_x[i], block_y[i], 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
            glUniform3f(glGetUniformLocation(shader_program, "aColor"), 0.0f, 1.0f, 0.0f);
            glBindVertexArray(VAO3);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    glFlush();
}

void MyReshape(int NewWidth, int NewHeight) {
    glViewport(0, 0, NewWidth, NewHeight);
    GLfloat aspect_ratio = (GLfloat)NewWidth / (GLfloat)NewHeight;
    glUseProgram(shader_program);
    glm::mat4 projection = glm::ortho(-1.0f * aspect_ratio, 1.0f * aspect_ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void MyTimer(int Value)
{
    if (bullet_started) {
        bx += xdiff;
        by += ydiff;
        if (bx > 1.0f || bx < -1.0f) xdiff *= -1;
        if (by > 1.0f) ydiff *= -1;
        if (by <= -0.75f) {
            if (bx < -0.2f + Delta || bx > 0.2f + Delta) {
                bullet_started = false;
                xdiff = ydiff = 0;
                printf("Game Over\n");
            }
            else {
                ydiff = 0.02f;
            }
        }
        for (int i = 0; i < 8; i++) {
            if (block_on[i] == 1) {
                if (bx >= block_x[i] - 0.1f && bx <= block_x[i] + 0.1f &&
                    by >= block_y[i] - 0.05f && by <= block_y[i] + 0.05f) {
                    block_on[i] = 0;
                    ydiff *= -1;
                }
            }
        }
    }
    else {
        bx = Delta;
        by = -0.75f;
    }

    glutTimerFunc(40, MyTimer, 1);
    glutPostRedisplay();
}

void MyKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'a': offset = -0.01f; Delta += offset; break;
    case 'd': offset = 0.01f; Delta += offset;break;

    case 'w':
        if (!bullet_started) {
            bullet_started = true;
            bx = Delta;
            by = -0.75f;
            xdiff = 0.02f;
            ydiff = 0.02f;
        }
        break;
    case 'q':
        exit(0);
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Brick Game");
    glewInit();
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutTimerFunc(40, MyTimer, 1);
    glutKeyboardFunc(MyKeyboard);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    float vertices[] = {
       -0.2f, -0.9f,  0.0f,  0.2f, -0.9f,  0.0f,  -0.2f, -0.8f,  0.0f,
       -0.2f, -0.8f,  0.0f,  0.2f, -0.9f,  0.0f,   0.2f, -0.8f,  0.0f
    };
    float vertices2[] = {
      -0.05f, -0.05f, 0.0f,  0.05f, -0.05f, 0.0f,  -0.05f, 0.05f, 0.0f,
      -0.05f, 0.05f,  0.0f,  0.05f, -0.05f, 0.0f,   0.05f, 0.05f, 0.0f
    };
    float vertices3[] = {
      -0.1f, -0.05f, 0.0f,   0.1f, -0.05f, 0.0f,  -0.1f, 0.05f, 0.0f,
      -0.1f, 0.05f,  0.0f,   0.1f, -0.05f, 0.0f,   0.1f, 0.05f, 0.0f
    };

    GLuint VBO[3];
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO2);
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(3, VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glutMainLoop();
    return 0;
}