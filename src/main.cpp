#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "common/sysDialog.cpp"
#include "common/logging.cpp"
#include "shaders/compiler.cpp"

#include "formats/hif.cpp"

#define windowTitle "Untitled window"

//TODO: Make these load from a config
#define window_width 1024
#define window_height 576
#define render_fov 110.f

#define LogLevel logging::warn

int main(){
    if(!glfwInit()){
        dialog::error("GLFW Error", "Failed to initialize GLFW!");
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    GLFWwindow* window;
    
    window = glfwCreateWindow(
        window_width,
        window_height,
        "Title here",
        NULL, NULL
    );
    glfwSetWindowPos(
        window,
        (mode->width / 2) - (window_width / 2),
        (mode->height / 2) - (window_height / 2)
    );
    
    if(window == NULL){
        dialog::error(
            "GL Error",
            "Failed to open GLFW window. "
            "If you have an Intel GPU, they are not 3.3 compatible. "
            "Try the 2.1 version of the tutorials."
        );
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if(glewInit() != GLEW_OK) {
        dialog::error("GLEW Error", "Failed to initialize GLEW!");
        glfwTerminate();
        return -1;
    }
    
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(g_vertex_buffer_data),
		g_vertex_buffer_data,
		GL_STATIC_DRAW
	);
	
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    GLuint prog = loadShader("Test",
        "#version 330 core\n"
        "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
        "layout(location = 1) in vec3 vertexColor;\n"
        "out vec3 fragmentColor;\n"
        "uniform mat4 MVP;\n"
        "void main(){\n"
        "    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n"
        "    fragmentColor = vertexColor;\n"
        "}",
        "#version 330 core\n"
        "in vec3 fragmentColor;\n"
        "out vec3 color;\n"
        "void main(){\n"
        "    color = fragmentColor;\n"
        "}"
    );
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(prog, "MVP");

    glm::mat4 Projection = glm::perspective(
        glm::radians(render_fov),
        float(window_width) / float(window_height),
        0.1f, //Closest render
        100.0f //Farthest render
    );
    
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(0, 0, 0), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 MVP = Projection * View * Model;
    
    hif image;
    image.readFile("./colours_grid.hif");
    
    double lastTime = glfwGetTime();
    int frame = 0;
    int nbFrames = 0;
    do{
        //FPS
        double currentTime = glfwGetTime();
        nbFrames++;
        frame++;
        if(currentTime - lastTime >= 1.0){
            char formattedTitle[256];
            sprintf(
                formattedTitle,
                "%s - %i FPS",
                windowTitle,
                int(nbFrames)
            );
            glfwSetWindowTitle(window, formattedTitle);
            nbFrames = 0;
            lastTime += 1.0;
        }
        
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(sin(double(frame)/100)*75, cos(double(frame)/100)*75, -32), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		glm::mat4 MVP = Projection * View * Model;
		
        //Prepare stage
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(prog);
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute 0.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void*)0  // array buffer offset
        );
        
        // 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
        
        //Draw
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        //Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while(!glfwWindowShouldClose(window));
    
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(prog);
	glDeleteVertexArrays(1, &VertexArrayID);
	
	glfwTerminate();
	
	return 0;
}
