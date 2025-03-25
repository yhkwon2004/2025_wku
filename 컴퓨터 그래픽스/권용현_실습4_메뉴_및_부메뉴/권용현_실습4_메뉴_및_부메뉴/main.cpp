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
uniform vec4 setColor; \
void main() { \
  FragColor = setColor; \
}";

GLuint shader_program;
GLuint VAO_triangle, VAO_rect, VAO_pentagon;
GLint object = 1;
GLint color = 1;
GLboolean IsSmall = true;

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);


    glm::vec4 currentColor;
    switch (color) {
    case 1: currentColor = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f); break; // 보라
    case 2: currentColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); break; // 빨강
    case 3: currentColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); break; // 파랑
    }
    glUniform4fv(glGetUniformLocation(shader_program, "setColor"), 1, glm::value_ptr(currentColor));

    glm::mat4 transform = glm::mat4(1.0f);
    if (IsSmall) {
        transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
    }
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

    if (object == 1) {
        glBindVertexArray(VAO_triangle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    else if (object == 2) {
        glBindVertexArray(VAO_rect);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else if (object == 3) {
        glBindVertexArray(VAO_pentagon);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
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
// 메인 메뉴
void MyMainMenu(int entryID) {
    if (entryID >= 1 && entryID <= 3)
        object = entryID;
    else if (entryID == 4)
        exit(0);
    glutPostRedisplay();
}
// 색상
void MyColorSubMenu(int entryID) {
    color = entryID;
    glutPostRedisplay();
}

// 크기
void MySizeSubMenu(int entryID) {
    IsSmall = (entryID == 1);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL 도형 메뉴 예제");
    glewInit();
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);

    GLint sizeMenuID = glutCreateMenu(MySizeSubMenu);
    glutAddMenuEntry("Small", 1);
    glutAddMenuEntry("Large", 2);

    GLint colorMenuID = glutCreateMenu(MyColorSubMenu);
    glutAddMenuEntry("Purple", 1);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Blue", 3);

    GLint mainMenuID = glutCreateMenu(MyMainMenu);
    glutAddMenuEntry("Triangle", 1);
    glutAddMenuEntry("Rectangle", 2);
    glutAddMenuEntry("Pentagon", 3);
    glutAddSubMenu("Change Size", sizeMenuID);
    glutAddSubMenu("Change Color", colorMenuID);
    glutAddMenuEntry("Exit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    // 삼각형
    GLfloat triangle[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    GLuint VBO1;
    glGenVertexArrays(1, &VAO_triangle);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 사각형
    GLfloat rectangle[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };
    GLuint VBO2;
    glGenVertexArrays(1, &VAO_rect);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO_rect);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 오각형
    GLfloat pentagon[] = {
         0.0f,  0.5f, 0.0f,
         0.47f,  0.15f, 0.0f,
         0.29f, -0.4f, 0.0f,
        -0.29f, -0.4f, 0.0f,
        -0.47f,  0.15f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    GLuint VBO3;
    glGenVertexArrays(1, &VAO_pentagon);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO_pentagon);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentagon), pentagon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glutMainLoop();
    return 0;
}
