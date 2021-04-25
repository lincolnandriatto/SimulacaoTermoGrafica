#ifndef _SCR_INTERACTOR
#define _SCR_INTERACTOR


//************** Includes ***************************
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "lgl/lgl.h"
#include <glad/glad.h>
//#include <GL/glew.h>
//#include <GL/freeglut.h>
#include <GLFW/glfw3.h>




// we need vec3 rotation
#include "glm/gtx/vector_angle.hpp"



#include <glm/glm.hpp>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
//#include <learnopengl/shader_s.h>
#include <learnopengl/camera.h>

#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include "TrackBall.h"



using namespace std;
//************** Defines de valores e funcoes ***************************





enum render_mode{RUSER , RWIRE, RPOINT, RSHADING, RSHADING_WIRE};

enum view_mode{VIEW_FREE, VIEW_TOP, VIEW_BOTTOM, VIEW_LEFT, VIEW_RIGHT, VIEW_NEAR, VIEW_FAR};

#define BTN_RENDER  100
#define BTN_USER    101
#define BTN_RESET   102
#define BTN_X_AXIS	103
#define BTN_Y_AXIS	104
#define BTN_Z_AXIS	105
#define BTN_COLOR   106 // muda a cor do BG
#define BTN_CHANGE_NORMAL 107 // muda o sinal da normal
#define BTN_CENTROIDE 108 // calcula o centroide


#define BLACK 200
#define WHITE 201
#define USER 202

#define SUB_SIZE 15
#define MAX(X, Y)\
		(X) > (Y) ? (X) : (Y)
#define MIN(X, Y)\
		(X) < (Y) ? (X) : (Y)





//*********************************** glButton *****************************************************************
class glButton
{
	public :
		glButton()	{ this->state = false; }
		void    init(auxRect<float> edge_values, float * clr_text, float * clr_btn, char * text);
		void    setState ()							 { this->state = !this->state; }
		void    setColor (float * edge, float * btn);
		void    setLabel (char * text)				 { strcpy(this->label, text); }
		void    setVisible(bool visible)             { this->visible = visible; }
		
		bool    getState ()							 { return this->state; }

		char *  getLabel ()							 { return this->label; }

		void    Draw();
        
     
        
        

	protected :
		void    Draw_Label();
		
		auxRect<float> edge;
		bool	state;
		bool    visible;
		float * text_color;
      float * btn_color;
		char  *	label;

};


/*class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if(geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if(geometryPath != nullptr)
        {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if(geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryPath != nullptr)
            glDeleteShader(geometry);

    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

*/
//*********************************** scrIterator *****************************************************************



class scrInteractor
{
	public :
     
	   
        scrInteractor(int width, int height);
		virtual ~scrInteractor(){} // Destrutor virtual para poder usar a função virtual para calcular o centróide
        void processInput();
			
        void scrInteractorMainLoop();
        bool Init(float xmin, float xmax, float ymin, float ymax, float near_z, float far_z, int *argc, char **argv);
		
        bool Init (auxRect<float> rec_extents, float near_z, float far_z,int *argc, char **argv);

		void Display();

        void setDraw(void (* func)(const Shader&));

		void Refresh_List();

		void Reshape(int width, int height);

        void MouseMotion(GLFWwindow* window,int x, int y);
		
        void Mouse(GLFWwindow* window,int button, int state, int x, int y);

		bool Keyboard(unsigned char key, int x, int y);

		void subDisplay();

		void subReshape(int width, int height);

		void subMouse(int button, int state, int x, int y);

		void subInit();
		
		float * getTranslation();
		
		void Init_Menu();

		void Calc_Rotation();
      
        void setBGColor(); // muda os valores das variaveis bg_color e bg_state de acordo com o estado atual do BG
        
        int getBGState(); // verifica o estado atual do BG
        
        float getBGColor(int rgb); // pega os valores de cada uma das componentes RGB, de acordo com a variavel int rgb
      
        void setUserBGColor(); // le valores passados pelo usuario e modifica o valor do vetor bg_color
        
        void Change_BG_Color(GLFWwindow* window); // modifica a cor do BG
      
		static void Set_Centroide(float x, float y, float z); // Estatico porque é usado na VisPrint.
		
		static bool Get_Normal_Change(); // função que devolve a normal, usada no main.

		
        void SetScreenImageFilename(std::string filename);
        std::string& GetScreenImageFilename();
        bool WriteScreenImage();
	int view_list;
	void Init_List();
    GLFWwindow* window;


    // camera
    glm::vec3 cameraInitialPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camerareferenceInitialPos=glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraInitialFront = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 cameraInitialUp = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::vec3 cameraPos;
    glm::vec3 cameraAuxPos;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::mat4 view;
    float yaw   = 0.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch =  0.0f;
    float fov=45.0f;
    bool woodBox=true;
    bool skyBox=false;
    bool wireframeMode=false;
    float deltaTime ;	// time between current frame and last frame
    float lastFrame;
    bool firstMouse;
    float lastX,lastY;
    Camera *camera;
    glm::mat4 transform;
    glm::mat4 model;
    glm::vec3 planeRotXYZ = glm::vec3(0.0f);
    glm::mat3 orientation;
    // shaders
    Shader sphereShader; // you can name your shader files however you like
    Shader ourShader; // you can name your shader files however you like

    Shader lightCubeShader;
    Shader skyboxShader;
    Shader shaderBox;
    Shader frogShader;
    Shader starShader;

     void mouse_callback(GLFWwindow* window, double xpos, double ypos);
     float * matrix;
     auxPoint<int> pt_mouse;
     auxPoint<int> pt_mouse_old;

     bool mouse_left;
     bool mouse_right;
     bool shift_pressed;
     bool user_button;

     float val_tx;
     float val_ty;
     float val_tz;

     double  far_z;
     double  near_z;
     double  aux_z;

     double  initial_tx;
     double  initial_ty;
     double  initial_tz;

     double model_view_matrix[16];
    TrackBall trackball;
    double scale;

    float * vec_translation;

	protected :
	
		/*Criar uma variavel centroide para armazenar o centroide =)*/
		// terminar de programar o calculo do centroide.
	
		static float* Centroide; // estatico pois é usado em um metodo estatico
	
        int bg_state; // estado atual da cor do BG
      
        float *bg_color; // vetor que armazena os valores RGB para o BG

		static bool Normal_Change; // variavel usada para dizer quando o sinal deve ser mudado ou não
	
		void Reset();

		void setRender_Mode();

		void setView_Mode();

		void Draw_String(char * text);

		void Draw_Axis();


	
		void Calc_Scale();

		void Calc_Translation();

		void Initial_Translation ();

		void Init_Lights();

		void setLights(bool lights_on);

		void ScreenToPoint(auxPoint<int> mouse, double point[3]);

		void ScreenToPoint_Axis(auxPoint<int> mouse, double point[3]);

		int  getObject_Id(int x, int y);

        void (*functionDraw)(const Shader&);
	
		void Display_Point();
	
		auxRect<int> rec_screen; // coordinates of screen
		auxRect<float> rec_initial;


		unsigned short int point_size;




		float * axis_matrix;
		
		

		int main_winid;
		int sub_winid;

        std::string   screenFilename;
        int           screenFilenameNumber;

		glButton * buttons;

		render_mode render;



};


 
#endif
