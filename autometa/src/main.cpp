#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>

#include <string>

#include "Water.h"
#include "Lava.h"
#include "Oil.h"
#include "UnLava.h"
#include "FlowingWater.h"
#include "FlowingLava.h"
#include "FlowingOil.h"
#include "lava/Lava.h"
#include "lava/FlowingLava.h"
#include "water/Water.h"
#include "water/FlowingWater.h"

std::string GetStringFromFile(const std::string& filepath)
{
    std::ifstream stream(filepath);

    std::string line;
    std::string contents;
    while (getline(stream, line))
    {
        contents = contents + line + "\n";
    }
    //std::cout << contents << "\n";

    return contents;
}


static GLuint compileShader(GLuint type, const std::string& src)
{
    GLuint id = glCreateShader(type);
    const char* shaderSrc = src.c_str();
    glShaderSource(id, 1, &shaderSrc, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)_malloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);

        std::cout << "ERROR IN SHADER " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        std::cout << message << "\n";

        glDeleteShader(id);
        return 0;

    }

    return id;
}




int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 1024, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    glewInit();




    float quad[12] = {
        -1.0 ,-1.0,
        1.0 ,-1.0,
        1.0, 1.0,

        1.0 ,1.0,
        -1.0 ,1.0,
        -1.0, -1.0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    GLuint buff;
    glGenBuffers(1, &buff);

    glBindBuffer(GL_ARRAY_BUFFER, buff);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);



    GLint program = glCreateProgram();
    GLint vertexShader = compileShader(GL_VERTEX_SHADER, GetStringFromFile("assets/shader/basic.vsh") );
    GLint fragmentShader = compileShader(GL_FRAGMENT_SHADER, GetStringFromFile("assets/shader/basic.fsh") );

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2 * SIZE, 2 * SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glActiveTexture(GL_TEXTURE0);

    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "textureId"), 0);

    double delta = 0.0;
    double time = 0.0;

    const double tps = 1.0/20;

    //int updates = 0;

    double lastTime = glfwGetTime();

    Water waterTexture;
    FlowingWater flowingWaterTexture;
    Lava lavaTexture;
    FlowingLava flowingLavaTexture;
    LavaGenerator lavaGen;
    FlowingLavaGenerator flowingLavaGen;
    WaterGenerator waterGen;
    FlowingWaterGenerator flowingWaterGen;

    glBindTexture(GL_TEXTURE_2D, 0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        time = glfwGetTime();
        delta += time - lastTime;
        lastTime = time;
        

        //std::cout << delta << "\n";
        while (delta > tps)
        {
            //updates++;
            delta -= tps;
            //update texture
            //waterTexture.Tick();
            //flowingWaterTexture.Tick();
            //lavaTexture.Tick();
            //flowingLavaTexture.Tick();
            lavaGen.onTick();
            flowingLavaGen.onTick();
            waterGen.onTick();
            flowingWaterGen.onTick();
            


            //send data

            glBindTexture(GL_TEXTURE_2D, textureId);

            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, waterGen.xSize, waterGen.ySize, GL_RGBA, GL_UNSIGNED_BYTE, waterGen.img.data() );
            glTexSubImage2D(GL_TEXTURE_2D, 0, SIZE, 0, flowingWaterGen.xSize, flowingWaterGen.ySize, GL_RGBA, GL_UNSIGNED_BYTE, flowingWaterGen.img.data());
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, SIZE, lavaGen.xSize, lavaGen.ySize, GL_RGBA, GL_UNSIGNED_BYTE, lavaGen.img.data());
            //glTexSubImage2D(GL_TEXTURE_2D, 0, SIZE, SIZE, SIZE, SIZE, GL_RGBA, GL_UNSIGNED_BYTE, flowingLavaTexture.GetData().data());
            glTexSubImage2D(GL_TEXTURE_2D, 0, SIZE, SIZE, flowingLavaGen.xSize, flowingLavaGen.ySize, GL_RGBA, GL_UNSIGNED_BYTE, flowingLavaGen.img.data());

            glBindTexture(GL_TEXTURE_2D, 0);

        }
        //updates = 0;

        /* Render here */


        glBindTexture(GL_TEXTURE_2D, textureId);

        glClear(GL_COLOR_BUFFER_BIT);



        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteTextures(1, &textureId);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}