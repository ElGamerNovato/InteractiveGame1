#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Callback para ajustar el viewport cuando la ventana cambia de tamaño
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Inicialización de GLFW y creación de la ventana
GLFWwindow* initializeWindow(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Fallo al inicializar GLFW" << std::endl;
        return nullptr;
    }

    // Especificar la versión de OpenGL (en este caso 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Fallo al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Fallo al inicializar GLEW" << std::endl;
        return nullptr;
    }

    return window;
}

// Configuración de la vista 2D
void setup2DScene(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Función para cargar una textura desde un archivo
GLuint loadTexture(const char* filename) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Fallo al cargar la textura: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}

// Función para dibujar una nave espacial en 2D con textura
void drawTexturedSpaceShip(GLuint textureID, float x, float y, float width, float height) {
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

int main() {
    int width = 800, height = 600;

    GLFWwindow* window = initializeWindow(width, height, "Nave Espacial 2D con Textura");
    if (!window) {
        return -1;
    }

    setup2DScene(width, height);

    // Habilitar texturas 2D
    glEnable(GL_TEXTURE_2D);

    // Cargar la textura de la nave espacial
    const char* texturePath = "C:\\Proyectos\\JuegosInteractivos\\TestGL\\textures\\ship_texture.png";
    GLuint spaceShipTexture = loadTexture(texturePath);
    if (!spaceShipTexture) {
        std::cerr << "Error: no se pudo cargar la textura de la nave espacial." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Renderizado
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar nave espacial con textura
        drawTexturedSpaceShip(spaceShipTexture, 350.0f, 250.0f, 100.0f, 100.0f);

        // Intercambiar buffers y manejar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &spaceShipTexture);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}