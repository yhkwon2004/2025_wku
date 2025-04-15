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
    gl_Position = projection * transform * vec4(aPos, 1.0); \
}";

const char* fragment_shader = "#version 460 core\n \
out vec4 FragColor; \
uniform vec3 aColor; \
void main() { \
    FragColor = vec4(aColor, 1.0); \
}";

GLuint shader_program;
GLuint VAO;

GLfloat theta = 0.0;    // 사각형의 회전 각도를  위한 변수
GLfloat scale = 1.0;     // 사각형의 확대 축소 배율을 나타내는 변수
GLfloat diff = 0.1;       // 사각형의 확대 축소 배율의 변화 값을 나타내는 변수

void DrawCube(glm::mat4 transform, glm::vec3 color) {
    glUseProgram(shader_program);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform3fv(glGetUniformLocation(shader_program, "aColor"), 1, glm::value_ptr(color));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);


    glm::mat4 red = glm::mat4(1.0f);
    red = glm::rotate(red, glm::radians(theta), glm::vec3(0, 0, 1));
    red = glm::translate(red, glm::vec3(0.6f, 0.0f, 0.0f));
    DrawCube(red, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 blue = glm::mat4(1.0f);
    blue = glm::scale(blue, glm::vec3(scale, scale, 1.0f));
    DrawCube(blue, glm::vec3(0.0f, 0.0f, 1.0f));

 
    glm::mat4 green = red;
    green = glm::translate(green, glm::vec3(0.3f, 0.0f, 0.0f));
    green = glm::rotate(green, glm::radians(theta * 10), glm::vec3(0, 0, 1));
    green = glm::scale(green, glm::vec3(0.3f, 0.3f, 1.0f));
    DrawCube(green, glm::vec3(0.0f, 1.0f, 0.0f));

    glFlush();
}

void Timer(int value) {
    theta += 2.0f;
    if (theta >= 360.0f) theta = 0.0f;

    scale += diff;
    if (scale > 2.0f) diff = -0.1f;
    else if (scale < 0.5f) diff = 0.1f;

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);
}

void MyReshape(int NewWidth, int NewHeight) {
    glViewport(0, 0, NewWidth, NewHeight);
    GLfloat aspect_ratio = (GLfloat)NewWidth / (GLfloat)NewHeight;

    glUseProgram(shader_program);
    glm::mat4 projection = glm::ortho(-1.0f * aspect_ratio, 1.0f * aspect_ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Transform with Orbit + Scale + Spin");
    glewInit();

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutTimerFunc(0, Timer, 0);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        printf("Vertex Shader Error: %s\n", infoLog);
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        printf("Fragment Shader Error: %s\n", infoLog);
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    GLfloat vertices[] = {
        -0.15f, -0.15f,  0.0f,
         0.15f, -0.15f,  0.0f,
        -0.15f,  0.15f,  0.0f,
        -0.15f,  0.15f,  0.0f,
         0.15f, -0.15f,  0.0f,
         0.15f,  0.15f,  0.0f,
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