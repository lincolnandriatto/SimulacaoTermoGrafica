#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <list>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <learnopengl/filesystem.h>
//#include <glm/gtc/type_ptr.hpp>

//#include <GL/glew.h>
//#include <GL/freeglut.h>
#include <time.h>
//#include <functional>
#include "GL_Interactor.h"
#include "Gizmo.h"
#include <learnopengl/model.h>

//#include "CommandComponent.hpp"
//#include "MyCommands.hpp"

#include "ofVertexStarIteratorSurfaceVertex.h"
#include "of.h"
#include "ofOffPointsReader.h"
#include "Handler.hpp"
#include "Point.hpp"
#include "printof.hpp"


//Define a malha a ser usada.
typedef of::ofDefaultSurface TTraits;
typedef of::ofDefaultSurface TTraitsSSMesh;
typedef of::ofDefaultSurface::sVertex TofVertex;
typedef of::ofMesh<TTraits> TMesh;
typedef of::ofMesh<TTraitsSSMesh> TMeshSSM;
typedef of::ofPlane<TTraits> TPlane;
typedef of::ofOffReader<TTraits> TReader;
typedef of::ofOffWriter<TTraits> TWriter;
TMesh *malha;
TMesh *malhaFrog;
TReader Reader;
TWriter Writer;
Handler<TMesh> meshHandler;
Handler<TMesh> meshHandlerFrog;

typedef PrintOf<TTraits> TPrintOf;

TPrintOf *Print;
//TPrintOf *PrintObst;


//typedef MyCommands<TPrintOf> TMyCommands;
//typedef CommandComponent TAllCommands;

of::ofVtkWriter<TTraits> writer;
//TAllCommands *allCommands;

//Define o tamanho da tela.
const unsigned int SCR_WIDTH = 1280 ;
const unsigned int SCR_HEIGHT = 870;

#define SIZE_FACTOR 0.4f


//Shader sphereShader("sphere.vs", "sphere.fs"); // you can name your shader files however you like

scrInteractor *Interactor = new scrInteractor(SCR_WIDTH, SCR_HEIGHT);



//sphereShader.init("sphere.vs", "sphere.fs");

//Shader sphereShader("sphere.vs", "sphere.fs"); // you can name your shader files however you like
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);
void initImGUI();
void renderScene(glm::vec3& pos);
void renderBoxScene();
void renderSphere();
void renderGizmo();
void renderBoxCube();
void renderSkybox();
void renderWoodbox();
void renderGUI();
void renderOurShader();
void renderFrog();
void initStarRender();
void renderStarVertex(TMesh *m,int vertex,int r);


void renderBoxScene(const Shader &shader);
glm::mat3 rotateAroundAxis(const glm::vec3& axis, float angle);

// to hold planes' corners for drawing 4 dots
clock_t start_insert;
clock_t end_insert;
clock_t start_print;
clock_t end_print;


unsigned int VBO, frogVBO,VAO=0,frogVAO=0;
unsigned int  EBO,frogEBO,cubeVBO,lightCubeVAO;
unsigned int starVBO, starEBO,starVAO=0;
unsigned int cubeVAO=0;
unsigned int sphereVAO = 0;
unsigned int indexCount;
// skybox VAO
unsigned int skyboxVAO=0, skyboxVBO;
unsigned int cubemapTexture;
unsigned int woodTexture;
ImTextureID my_tex_id;
unsigned int depthCubemap;
// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeBoxVAO = 0;
unsigned int cubeBoxVBO = 0;
int starVertexShader;
int starFragmentShader;
int starShaderProgram;
bool starok=false,saveok=false;
int startok =0;
bool cicleok=false;

Gizmo gizmo;
vector<std::string> faces
{
    FileSystem::getPath("resources/textures/skybox/Maskonaive/right.jpg"),
    FileSystem::getPath("resources/textures/skybox/Maskonaive/left.jpg"),
    FileSystem::getPath("resources/textures/skybox/Maskonaive/top.jpg"),
    FileSystem::getPath("resources/textures/skybox/Maskonaive/bottom.jpg"),
    FileSystem::getPath("resources/textures/skybox/Maskonaive/front.jpg"),
    FileSystem::getPath("resources/textures/skybox/Maskonaive/back.jpg")
};
using namespace std;
using namespace of;

int positions[] ={531, 1188, 180, 1876, 383, 1327, 68, 1728, 528, 1175, 167, 1863,376, 1320, 4203, 3840, 4011};
float temperatures[]={0.0667 ,   0.1328 ,   0.0915 ,   0.1250 ,   0.0882 ,   0.0744 ,   0.0728 ,   0.0833 ,   0.0882 ,   0.0881 ,   0.1272 ,   0.0661 ,   0.0714 ,   0.0597 ,   0.1352 ,   0.0894 ,   0.0548 , 0.0200, 0.9905};
//{32676, 31843, 32426, 31137, 36695, 38173, 39039, 38065, 36578};
//{64, 210, 302, 427, 575, 615, 862, 863};
//{3266, 31843, 32426, 31137, 36695, 38173, 39039, 38065, 36578};
//{531, 1188, 180, 1876, 383, 1327, 68, 1728, 528, 1175, 167, 1863,376, 1320, 4203, 3840, 4011};
int np=17;
int nstar=5;
//{1999, 146, 1440, 639, 1988, 241, 1443, 688, 2031, 186, 1492, 691, 2020, 249, 1475, 680, 2023}; // caminho 2
//{62, 1320, 376, 1863, 167, 1175, 528, 1728, 68, 1327, 383, 1876, 180, 1188, 531, 1731}; //caminho1
int pi=0;

/*Shader ourShader("3.3.shader.vs", "3.3.shader.fs"); // you can name your shader files however you like

Shader lightCubeShader("2.1.light_cube.vs", "2.1.light_cube.fs");
Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");
Shader shaderBox("3.2.1.point_shadows.vs", "3.2.1.point_shadows.fs");*/

float alphaOurShader=1.0;

//##################################################################//

void RenderScene(const Shader &shader){

    //Print->Vertices(malha,blue,3);
    Print->FacesColoredFace(malha);



    glfwSwapBuffers(Interactor->window);
}

void HandleKeyboard(unsigned char key, int x, int y){



    double coords[3];
    char *xs[10];
    //allCommands->Keyboard(key);

    switch (key) {
        case '\033':
        case 'e':
            exit(1);
        break;
        case 'v':
            coords[0]=x;
            coords[1]=-y;
            coords[2]=0.0;
            malha->addVertex(coords);
        break;
        case 's':


        break;

        case 'd':


        break;


    }


    Interactor->Refresh_List();
//	glutPostRedisplay();


}
// lighting
glm::vec3 lightPos(3.0f, 1.0f, 3.0f);
glm::vec3 lightPos1(-3.0f, 1.0f, 3.0f);



using namespace std;
// create transformations

glm::mat4 projection;

glm::mat4 model;
glm::vec3 center;
glm::vec3 centerFrog;


void timerEvent(int value) {
    int i;
    if(Interactor->woodBox)
        renderWoodbox();


    if(Interactor->skyBox)
        renderSkybox();


    if (!Interactor->wireframeMode)
      {
        Interactor->ourShader.use();
        Interactor->ourShader.setBool("Wiremode",false);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        //glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glColorMask(true, true, true, true);
        // Enable blending

        renderOurShader();
       glEnable(GL_DEPTH_TEST);

       glEnable(GL_CULL_FACE);
       glEnable(GL_DEPTH_TEST);

        }
    else
    {
        /*glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Enable blending

        renderOurShader();
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);*/
       /* glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        glColor3f( 0.0f, 0.0f, 0.0f );


       // glColorMask(true, true, true, false);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Interactor->ourShader.use();
       //Interactor->ourShader.setVec4("objectColor", lred.R, lred.G, lred.B,0.225);
        alphaOurShader=0.4;
        renderOurShader();
       glDisable(GL_BLEND);
        */
        //glColorMask(true, true, true, true);
        Interactor->ourShader.use();
        Interactor->ourShader.setBool("Wiremode",true);
        Interactor->ourShader.setVec3("objectColor",glm::vec3(1.0,1.0,1.0));
       glDisable(GL_POLYGON_OFFSET_FILL);

        // Enable blending
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //renderInnerShader();
        alphaOurShader=1.0;
        glEnable(GL_LINE_WIDTH);
        glLineWidth(2);
        renderOurShader();
         glDisable(GL_LINE_WIDTH);



        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);


    }
    renderScene(lightPos);
    renderScene(lightPos1);
    if(starok)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_LINE_WIDTH);
        glLineWidth(4);
        renderStarVertex(malha,positions[pi],nstar);
        glDisable(GL_LINE_WIDTH);
    }
    //renderSphere();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    renderFrog();
    gizmo.updateViewMatrix(Interactor->view);
    renderGizmo();
    renderGUI();
    if((startok==0)&&(pi==np-1)&&(cicleok))
    {

        for(i=0;i<80;i++)
           Interactor->WriteScreenImage();
        pi=0;
        cicleok=false;
    }
    else
    if(startok==1)
    {

        starok=!starok;
        std::cout <<"starview = " << starok << std::endl;
        for( i=0;i<10;i++)
           Interactor->WriteScreenImage();
        startok++;
    }
    else
    if(startok==2)
    {
        starok=!starok;
        std::cout <<"starview = " << starok << std::endl;
        for(i=0;i<10;i++)
           Interactor->WriteScreenImage();
        pi++;
        if(pi<np-1)
            startok=1;
        else
        {
            startok=0;
            starok=false;

        }

    }
}


int main(int argc, char **argv)
{



  TReader reader;

  Interactor->setDraw(RenderScene);
	meshHandler.Set(new TMesh());
      //char *file = "../sem_parede_12M.ply";

   char *file_frog = "../siameseCat.ply";
    char *file = "../terreno_modelado.ply";
    float fc=0.08f;
    malha = new TMesh();
    malhaFrog = new TMesh();
    reader.readPly(malha,file);

    reader.readPly(malhaFrog,file_frog);

    meshHandlerFrog.Set(malhaFrog);
    meshHandler.Set(malha);


    //Print = new TPrintOf(meshHandler);



	double a,x1,x2,y1,y2,z1,z2;

	of::ofVerticesIterator<TTraits> iv(&meshHandler);

	iv.initialize();
	x1 = x2 = iv->getCoord(0);
	y1 = y2 = iv->getCoord(1);
	z1 = z2 = iv->getCoord(2);

	for(iv.initialize(); iv.notFinish(); ++iv){
		if(iv->getCoord(0) < x1) x1 = a = iv->getCoord(0);
		if(iv->getCoord(0) > x2) x2 = a = iv->getCoord(0);
		if(iv->getCoord(1) < y1) y1 = a = iv->getCoord(1);
		if(iv->getCoord(1) > y2) y2 = a = iv->getCoord(1);
		if(iv->getCoord(2) < z1) z1 = a = iv->getCoord(2);
		if(iv->getCoord(2) > z2) z2 = a = iv->getCoord(2);
	}

    float maxdim;
    maxdim = fabs(x2 - x1);
    if(maxdim < fabs(y2 - y1)) maxdim = fabs(y2 - y1);
    if(maxdim < fabs(z2 - z1)) maxdim = fabs(z2 - z1);

    maxdim *= 1.2;


     center = glm::vec3((x1+x2)/2.0, (y1+y2)/2.0, (z1+z2)/2.0 );
    for(iv.initialize(); iv.notFinish(); ++iv){
        iv->setCoord(0,((iv->getCoord(0)-center[0])*SIZE_FACTOR)*fc);
        iv->setCoord(1,((iv->getCoord(1)-center[1])*SIZE_FACTOR)*fc);
        iv->setCoord(2,((iv->getCoord(2)-center[2])*SIZE_FACTOR)*fc);
    }


    of::ofVerticesIterator<TTraits> ivf(&meshHandlerFrog);

    ivf.initialize();
    x1 = x2 = ivf->getCoord(0);
    y1 = y2 = ivf->getCoord(1);
    z1 = z2 = ivf->getCoord(2);

    for(ivf.initialize(); ivf.notFinish(); ++ivf){
        if(ivf->getCoord(0) < x1) x1 = a = ivf->getCoord(0);
        if(ivf->getCoord(0) > x2) x2 = a = ivf->getCoord(0);
        if(ivf->getCoord(1) < y1) y1 = a = ivf->getCoord(1);
        if(ivf->getCoord(1) > y2) y2 = a = ivf->getCoord(1);
        if(ivf->getCoord(2) < z1) z1 = a = ivf->getCoord(2);
        if(ivf->getCoord(2) > z2) z2 = a = ivf->getCoord(2);
    }



     centerFrog = glm::vec3((x1+x2)/2.0, (y1+y2)/2.0, (z1+z2)/2.0 );

     for(ivf.initialize(); ivf.notFinish(); ++ivf){
         ivf->setCoord(0,((ivf->getCoord(0)-centerFrog[0])*0.005f));//0.01f frog
         ivf->setCoord(1,((ivf->getCoord(1)-centerFrog[1])*0.005f));
         ivf->setCoord(2,((ivf->getCoord(2)-centerFrog[2])*0.005f));
     }




    Interactor->Init(center[0]-maxdim, center[0]+maxdim,
                                    center[1]-maxdim, center[1]+maxdim,
                                    center[2]-10*maxdim, center[2]+10*maxdim,&argc,argv);

    glfwSetCursorPosCallback(Interactor->window, cursor_pos_callback);
    glfwSetMouseButtonCallback(Interactor->window, mouse_callback);
    glfwSetKeyCallback(Interactor->window, key_callback);
   // AddKeyboard(HandleKeyboard);

    //allCommands->Help(std::cout);
	std::cout<< std::endl<< "Press \"?\" key for help"<<std::endl<<std::endl;
	double t;



/***********************************************************************/
    /* EXEMPLO DE PERCORRIMENTO DOS VIZINHOS DE UM VERTICE*/
/*
 *  Vai percorrer todos vizinhos do vertice 0
 * /
 *
 */

    //std::pair<int, int>  pc;
    of::ofVertexStarIteratorSurfaceVertex<TTraits> isv(malha);
       for(isv.initialize(63); isv.notFinish(); ++isv){

        std::cout << "viz = " << &isv << std::endl;

       }


       of::ofVertexStarIteratorSurface<TTraits> is(malha);
       for(is.initialize(63); is.notFinish(); ++is){

        std::cout << "cel = " << &is << std::endl;

       }


 /*******************************************************************/



    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


   Interactor->ourShader.init("3.3.shader.vs", "3.3.shader.fs");
   Interactor->ourShader.use();
   Interactor->ourShader.setBool("Wiremode",false);
   Interactor->starShader.init("starShader.vert","starShader.frag");
   Interactor->frogShader.init("frogShader.vert", "frogShader.frag");
   Interactor->lightCubeShader.init("2.1.light_cube.vs", "2.1.light_cube.fs");
   Interactor->skyboxShader.init("6.1.skybox.vs", "6.1.skybox.fs");
   Interactor->shaderBox.init("3.2.1.point_shadows.vs", "3.2.1.point_shadows.fs");

   Interactor->sphereShader.init("sphere.vs", "sphere.fs");
   Interactor->sphereShader.use();
   Interactor->sphereShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
   Interactor->sphereShader.setFloat("ao", 1.0f);
   Interactor->sphereShader.setVec3("lightPosition",lightPos);
   Interactor->sphereShader.setVec3("lightPosition1",lightPos1);

   // shader box configuration
   // --------------------
   Interactor->shaderBox.use();
   Interactor->shaderBox.setInt("diffuseTexture", 0);
   Interactor->shaderBox.setInt("depthMap", 1);




   Interactor->skyboxShader.use();
   Interactor->skyboxShader.setInt("skybox", 0);

Interactor->view =Interactor->camera->GetViewMatrix();
   // build up gizmo
   gizmo.build();
   gizmo.updateViewMatrix(Interactor->view);
   initImGUI();
my_tex_id = (void*) loadTexture(FileSystem::getPath("resources/textures/texture.jpg").c_str());
while (!glfwWindowShouldClose(Interactor->window))
{
        float currentFrame = glfwGetTime();
        Interactor->deltaTime = currentFrame - Interactor->lastFrame;
        Interactor->lastFrame = currentFrame;

        // input
        // -----
        //Interactor->processInput();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        timerEvent(1);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // ------------------------------------------------------------------------------

        glfwSwapBuffers(Interactor->window);
        glfwPollEvents();


}

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void renderFrog()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if(frogVAO==0)
        Print->MakeMeshVBO(malhaFrog,frogVBO,frogVAO,frogEBO);
         //Print->MakeTriangleVBO(VBO,VAO,EBO);

    // activate shader
    Interactor->frogShader.use();
    Interactor->frogShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    Interactor->frogShader.setVec3("viewPos", Interactor->camera->Position);
    // pass projection matrix to shader (note that in this case it could change every frame)
    projection = glm::perspective(glm::radians(Interactor->fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    Interactor->frogShader.setMat4("projection", projection);
    //view = Interactor->camera->GetViewMatrix();//glm::lookAt(Interactor->cameraPos, Interactor->cameraPos + Interactor->cameraFront, Interactor->cameraUp);
    //if((!Interactor->woodBox)&&(!Interactor->skyBox))
      //  view = Interactor->camera->GetViewMatrix();

    Interactor->frogShader.setMat4("view", Interactor->view);
    // world transformation
     model = glm::mat4(1.0f);

    //model = glm::translate(model,centerFrog);
     //model = glm::scale(model, glm::vec3(0.02f)); // move bottom-right
     model = glm::translate(model, glm::vec3(malha->getVertex(positions[pi])->getCoord(0),malha->getVertex(positions[pi])->getCoord(1)+0.02,malha->getVertex(positions[pi])->getCoord(2)));
     Interactor->orientation = rotateAroundAxis(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(Interactor->planeRotXYZ.x)) *
         rotateAroundAxis(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(Interactor->planeRotXYZ.y)) *
         rotateAroundAxis(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(Interactor->planeRotXYZ.z));
     model = model*glm::mat4(Interactor->orientation);
     //model=glm::rotate(model, glm::radians(-42.0f), glm::vec3(0.0, 0.0, 1.0));
     //model=glm::rotate(model, glm::radians(-100.0f), glm::vec3(1.0, 0.0, 0.0));
      //model = glm::translate(model,centerFrog);

    //

     //
    //
     //model = glm::translate(model,centerFrog);

      //model = glm::translate(model,-centerFrog);
     //lightPos=orientation*lightPos;
     //lightPos1=orientation*lightPos1;
     Interactor->frogShader.setVec3("lightPos", lightPos);
     Interactor->frogShader.setVec3("lightPos1", lightPos1);

    //model = glm::scale(model, glm::vec3(0.5f)); // move bottom-right

    //model=glm::rotate(model, glm::radians(42.0f), glm::vec3(0.0, 0.0, 1.0));

    //model=glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    //model=glm::rotate(model, glm::radians(150.0f), glm::vec3(0.0, 0.0, 1.0));


    Interactor->frogShader.setMat4("model", model);
    //Interactor->frogShader.setMat4("transform",Interactor->transform);
    // render the triangle
    glBindVertexArray(frogVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3*malhaFrog->getNumberOfCells(), GL_UNSIGNED_INT, 0);
     glBindVertexArray(0); // no need to unbind it every time
     glDisable(GL_DEPTH_TEST);
     glDisable(GL_CULL_FACE);
};

void renderOurShader()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if(VAO==0)
        Print->MakeMeshVBO(malha,VBO,VAO,EBO);
         //Print->MakeTriangleVBO(VBO,VAO,EBO);

    // activate shader
    Interactor->ourShader.use();
    Interactor->ourShader.setVec3("objectColor", 0.7f, 0.7f, 0.7f);

    Interactor->ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    Interactor->ourShader.setFloat("alpha",alphaOurShader);
    Interactor->ourShader.setVec3("viewPos", Interactor->camera->Position);
    // pass projection matrix to shader (note that in this case it could change every frame)
    projection = glm::perspective(glm::radians(Interactor->fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    Interactor->ourShader.setMat4("projection", projection);

    Interactor->ourShader.setMat4("view", Interactor->view);
    // world transformation
     model = glm::mat4(1.0f);
     //Interactor->orientation = rotateAroundAxis(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(Interactor->planeRotXYZ.x)) *
       //  rotateAroundAxis(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(Interactor->planeRotXYZ.y)) *
         //rotateAroundAxis(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(Interactor->planeRotXYZ.z));
     //model = glm::mat4(Interactor->orientation);
     Interactor->ourShader.setVec3("lightPos", lightPos);
     Interactor->ourShader.setVec3("lightPos1", lightPos1);
     //model = glm::scale(model, glm::vec3(0.05f)); // move bottom-right
     //model=glm::rotate(model, glm::radians(-42.0f), glm::vec3(0.0, 0.0, 1.0));
    Interactor->ourShader.setMat4("model", model);
    Interactor->ourShader.setMat4("transform",Interactor->transform);
    // render the triangle
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3*malha->getNumberOfCells(), GL_UNSIGNED_INT, 0);
     glBindVertexArray(0); // no need to unbind it every time
     glDisable(GL_CULL_FACE);
     glDisable(GL_DEPTH_TEST);

};

void renderSkybox()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    Interactor->skyboxShader.use();
    //view = Interactor->camera->GetViewMatrix();
    // remove translation from the view matrix
    Interactor->skyboxShader.setMat4("view", Interactor->view);
    projection = glm::perspective(glm::radians(Interactor->camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    Interactor->skyboxShader.setMat4("projection", projection);
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f)); // a smaller cube
    //model =glm::rotate(model, glm::radians(-42.0f), glm::vec3(0.0, 0.0, 1.0));
    Interactor->skyboxShader.setMat4("model",model);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    renderBoxScene();
    glDepthFunc(GL_LESS); // set depth function back to default
};

void renderBoxScene()
{
    if(skyboxVAO==0)
    {
        // load textures
        // -------------
        woodTexture = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str());

        // configure depth map FBO
        // -----------------------
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        // create depth cubemap texture
         depthCubemap;
        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        float skyboxVertices[] = {
            // positions
            -10.0f,  10.0f, -10.0f,
            -10.0f, -10.0f, -10.0f,
             10.0f, -10.0f, -10.0f,
             10.0f, -10.0f, -10.0f,
             10.0f,  10.0f, -10.0f,
            -10.0f,  10.0f, -10.0f,

            -10.0f, -10.0f,  10.0f,
            -10.0f, -10.0f, -10.0f,
            -10.0f,  10.0f, -10.0f,
            -10.0f,  10.0f, -10.0f,
            -10.0f,  10.0f,  10.0f,
            -10.0f, -10.0f,  10.0f,

             10.0f, -10.0f, -10.0f,
             10.0f, -10.0f,  10.0f,
             10.0f,  10.0f,  10.0f,
             10.0f,  10.0f,  10.0f,
             10.0f,  10.0f, -10.0f,
             10.0f, -10.0f, -10.0f,

            -10.0f, -10.0f,  10.0f,
            -10.0f,  10.0f,  10.0f,
             10.0f,  10.0f,  10.0f,
             10.0f,  10.0f,  10.0f,
             10.0f, -10.0f,  10.0f,
            -10.0f, -10.0f,  10.0f,

            -10.0f,  10.0f, -10.0f,
             10.0f,  10.0f, -10.0f,
             10.0f,  10.0f,  10.0f,
             10.0f,  10.0f,  10.0f,
            -10.0f,  10.0f,  10.0f,
            -10.0f,  10.0f, -10.0f,

            -10.0f, -10.0f, -10.0f,
            -10.0f, -10.0f,  10.0f,
             10.0f, -10.0f, -10.0f,
             10.0f, -10.0f, -10.0f,
            -10.0f, -10.0f,  10.0f,
             10.0f, -10.0f,  10.0f
        };
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        cubemapTexture = loadCubemap(faces);
    }
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

}


void renderWoodbox()
{
    // 0. create depth cubemap transformation matrices
    // -----------------------------------------------
    float near_plane = 1.0f;
    float far_plane  = 25.0f;


    //glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Interactor->shaderBox.use();
    projection = glm::perspective(glm::radians(Interactor->camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //view = Interactor->camera->GetViewMatrix();
    Interactor->shaderBox.setMat4("projection", projection);
    Interactor->shaderBox.setMat4("view", Interactor->view);
    // set lighting uniforms
    Interactor->shaderBox.setVec3("lightPos", lightPos);
    Interactor->shaderBox.setVec3("viewPos", Interactor->camera->Position);
    Interactor->shaderBox.setInt("shadows", false); // enable/disable shadows by pressing 'SPACE'
    //Interactor->shaderBox.setMat4("model", model);
    Interactor->shaderBox.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    renderBoxScene(Interactor->shaderBox);
};


void renderScene(glm::vec3 &pos)
{
    // also draw the lamp object
    Interactor->lightCubeShader.use();
    Interactor->lightCubeShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    Interactor->lightCubeShader.setMat4("projection", projection);
    Interactor->lightCubeShader.setMat4("view", Interactor->view);
    model = glm::mat4(1.0f);
    model = glm::mat4(Interactor->orientation);
    model = glm::translate(model, pos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    Interactor->lightCubeShader.setMat4("model", model);

    if(cubeVAO==0)
    {
    //light cube

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };


        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)

        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        // note that we update the lamp's position attribute's stride to reflect the updated buffer data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0); // no need to unbind it every time


}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
glm::vec3 polarVector(float p, float y)
{
// this form is already normalized
return glm::vec3(std::cos(y) * std::cos(p),
std::sin(p),
std::sin(y) * std::cos(p));
};

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Interactor->firstMouse)
    {
        Interactor->lastX = xpos;
        Interactor->lastY = ypos;
        Interactor->firstMouse = false;
    }

     //   std::cout<< "xpos =" << xpos << " ypos =" << ypos <<std::endl;


    float xoffset = xpos - Interactor->lastX;
    float yoffset = Interactor->lastY - ypos; // reversed since y-coordinates go from bottom to top

    Interactor->lastX = xpos;
    Interactor->lastY = ypos;


    if( Interactor->mouse_left && !ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemHovered())
    {
        //Interactor->MouseMotion(Interactor->window,Interactor->lastX,Interactor->lastY);
        Interactor->camera->ProcessMouseMovement(xoffset, yoffset);
       // Interactor->model=glm::mat4(1.0f);
        //Interactor->model=glm::mat4(Interactor->matrix[0],Interactor->matrix[1],Interactor->matrix[2],Interactor->matrix[3],Interactor->matrix[4],Interactor->matrix[5],Interactor->matrix[6],Interactor->matrix[7],Interactor->matrix[8],Interactor->matrix[9],Interactor->matrix[10],Interactor->matrix[11],Interactor->matrix[12],Interactor->matrix[13],Interactor->matrix[14],Interactor->matrix[15]);
        //Interactor->transform = glm::rotate(Interactor->transform, Interactor->trackball.getAngPhi(), glm::vec3(Interactor->trackball.getQuat(0), Interactor->trackball.getQuat(1), Interactor->trackball.getQuat(2)));
            //printf("Cam Initial Pos: (%.02f, %.02f, %.02f)\n", Interactor->cameraInitialPos.x, Interactor->cameraInitialPos.y, Interactor->cameraInitialPos.z);
            //printf("Cam Pos: (%.02f, %.02f, %.02f)\n", Interactor->camera->Position.x, Interactor->camera->Position.y, Interactor->camera->Position.z)
            Interactor->cameraPos= glm::rotate(Interactor->cameraInitialPos, -glm::radians(Interactor->camera->Yaw+90), glm::vec3(0.0f, 1.0f, 0.0f));
            //printf("Cam Pos: (%.02f, %.02f, %.02f)\n", Interactor->camera->Position.x, Interactor->camera->Position.y, Interactor->camera->Position.z);
            glm::vec3 right = glm::normalize(glm::cross(Interactor->camera->Front, glm::vec3(0.0f, 1.0f, 0.0f)));
            Interactor->camera->Position =glm::rotate(Interactor->cameraPos, glm::radians(Interactor->camera->Pitch), right);
            //printf("Cam Pos: (%.02f, %.02f, %.02f)\n", Interactor->camera->Position.x, Interactor->camera->Position.y, Interactor->camera->Position.z);

        Interactor->ourShader.use();
         Interactor->view = Interactor->camera->GetViewMatrix();//glm::lookAt(Interactor->cameraPos, Interactor->cameraPos+Interactor->cameraFront, Interactor->cameraUp);
        Interactor->ourShader.setMat4("view",Interactor->view);
        Interactor->shaderBox.use();
        Interactor->shaderBox.setMat4("view",Interactor->view);
        Interactor->skyboxShader.use();
        Interactor->skyboxShader.setMat4("view",Interactor->view);

        Interactor->starShader.use();
        Interactor->starShader.setMat4("view",Interactor->view);

        gizmo.updateViewMatrix(Interactor->view);


    }
    if(Interactor->mouse_right && !ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemHovered())
    {
        Interactor->fov -= (float)yoffset*0.5;
            if (Interactor->fov < 1.0f)
                Interactor->fov = 1.0f;
            if (Interactor->fov > 45.0f)
                Interactor->fov = 45.0f;
            Interactor->ourShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(Interactor->fov), 1.0f, 0.1f, 100.0f);
            Interactor->ourShader.setMat4("projection", projection);
    }
   // Interactor->camera->Position=Interactor->cameraPos;
}


void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action==GLFW_PRESS) {

        Interactor->pt_mouse_old = Interactor->pt_mouse;
        Interactor->pt_mouse.setValues(Interactor->lastX, Interactor->lastY);

        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            Interactor->mouse_right = true;

        if (button == GLFW_MOUSE_BUTTON_LEFT)
            Interactor->mouse_left = true;



    }
    else if (action==GLFW_RELEASE)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            Interactor->mouse_left = false;
            //Interactor->cameraInitialPos=Interactor->cameraPos;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            Interactor->mouse_right = false;
    }
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Interactor->camera->ProcessMouseScroll(yoffset);
}


// renders (and builds at first invocation) a sphere
// -------------------------------------------------
void renderSphere()
{
    Interactor->sphereShader.use();
    Interactor->sphereShader.setFloat("roughness",0.5);
    Interactor->sphereShader.setVec3("albedo", 0.7f, 0.0f, 0.0f);
    Interactor->sphereShader.setMat4("projection", projection);

    Interactor->sphereShader.setMat4("view", Interactor->view);
    Interactor->sphereShader.setVec3("camPos", Interactor->camera->Position);
    Interactor->sphereShader.setVec3("lightColor",glm::vec3(300.0f, 300.0f, 300.0f));
    model = glm::mat4(1.0f);
    model = glm::mat4(Interactor->orientation);
    model = glm::translate(model, glm::vec3(malha->getVertex(48236)->getCoord(0),malha->getVertex(48236)->getCoord(1),malha->getVertex(48236)->getCoord(2)));
    //model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.1f*SIZE_FACTOR));
    //model =glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0, 1.0, 0.0));
    Interactor->sphereShader.setMat4("model",model);
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 8;
        const unsigned int Y_SEGMENTS = 8;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
     glBindVertexArray(0); // no need to unbind it every time
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


// renders the 3D scene
// --------------------
void renderBoxScene(const Shader &shader)
{
    // room cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,center);
    model = glm::scale(model, glm::vec3(20.0f));
   // model =glm::rotate(model, glm::radians(-42.0f), glm::vec3(0.0, 0.0, 1.0));
    shader.setMat4("model", model);
    glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
    renderBoxCube();
    glEnable(GL_CULL_FACE);
}


void renderBoxCube()
{
    // initialize (if necessary)
    if (cubeBoxVAO == 0)
    {
        // load textures
        // -------------
        woodTexture = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str());

        // configure depth map FBO
        // -----------------------
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        // create depth cubemap texture
         depthCubemap;
        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeBoxVAO);
        glGenBuffers(1, &cubeBoxVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeBoxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeBoxVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeBoxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

/// create a rotational matrix to rotate around the input `axis` for `angle`.
/// it creates a new mat3.
/// `axis` needs to be normalized
glm::mat3 rotateAroundAxis(const glm::vec3& axis, float angle)
{
    float nx_squared = axis.x * axis.x;
    float ny_squared = axis.y * axis.y;
    float nz_squared = axis.z * axis.z;
    float nx_ny = axis.x * axis.y;
    float nx_nz = axis.x * axis.z;
    float ny_nz = axis.y * axis.z;
    float costheta = std::cos(angle);
    float sintheta = std::sin(angle);
    float one_costheta = 1.0f - costheta;

    return glm::mat3(
            nx_squared*one_costheta + costheta,     nx_ny*one_costheta + sintheta*axis.z,   nx_nz*one_costheta - sintheta*axis.y,
            nx_ny*one_costheta - sintheta*axis.z,   ny_squared*one_costheta + costheta,     ny_nz*one_costheta + sintheta*axis.x,
            nx_nz*one_costheta + sintheta*axis.y,   ny_nz*one_costheta - sintheta*axis.x,   nz_squared*one_costheta + costheta);
}

void renderGizmo()
{
    gizmo.draw();
}

void renderGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
int tmpHeight;
#define IMGUI_WINDOW_WIDTH 500
#define IMGUI_WINDOW_HEIGHT 280
#define IMGUI_WINDOW_MARGIN 0

if((startok==0)&&(!cicleok))
        tmpHeight=280;
else
    tmpHeight=200;
    ImGui::SetNextWindowSize(ImVec2(IMGUI_WINDOW_WIDTH, tmpHeight));
    ImGui::SetNextWindowSizeConstraints(ImVec2(IMGUI_WINDOW_WIDTH, tmpHeight), ImVec2(IMGUI_WINDOW_WIDTH,tmpHeight));
    ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH - IMGUI_WINDOW_WIDTH - IMGUI_WINDOW_MARGIN, IMGUI_WINDOW_MARGIN));
    ImGui::Begin("Informations");
        //static bool mm = true;
        //ImGui::ShowDemoWindow(&mm);
    //ImGuiIO& io = ImGui::GetIO();
    float minTemp=21.9;
    float maxTemp=26.0;
    float error = 0.01*(maxTemp-minTemp);
    float target = (1.0-0.67)*(maxTemp-minTemp)+minTemp;
    float current;
    float my_tex_w = 450;
    float my_tex_h = 30;
    //ImGui::GetFontSize();
    //ImGui::T
    ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "Temperature (in degrres)");
    ImGui::Text("Min = %.1f", minTemp); ImGui::SameLine(320); ImGui::Text("Max = %.1f", maxTemp);
    //ImVec2 pos = ImGui::GetCursorScreenPos();
       ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
       if((startok==0)&&(!cicleok))
       {
       ImGui::Separator();
        static const float f32_zero = 0.f, f32_360 = 360.0f, f32_neg_180 = -180.0f, f32_pos_180 = 180.f, f32_neg_90 = -90.0f, f32_pos_90 = 90.0f;

        // plane transformation
        ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "Plane Rot (in degrees)");
        ImGui::SetNextItemWidth(100);
        if (ImGui::DragScalar("Z", ImGuiDataType_Float, &Interactor->planeRotXYZ.z, 1.0f, &f32_zero, &f32_360, "%.2f", 1.0f))
        {
            // bound value entered from user
            if (Interactor->planeRotXYZ.z < f32_zero)
                Interactor->planeRotXYZ.z = f32_zero;
            else if (Interactor->planeRotXYZ.z > f32_360)
                Interactor->planeRotXYZ.z = f32_360;
        }
        ImGui::SetNextItemWidth(100);
        ImGui::SameLine(140);
        if (ImGui::DragScalar("Y", ImGuiDataType_Float, &Interactor->planeRotXYZ.y, 1.0f, &f32_neg_180, &f32_pos_180, "%.2f", 1.0f))
        {
            if (Interactor->planeRotXYZ.y < f32_neg_180)
                Interactor->planeRotXYZ.y = f32_neg_180;
            else if (Interactor->planeRotXYZ.y > f32_pos_180)
                Interactor->planeRotXYZ.y = f32_pos_180;
        }
        ImGui::SetNextItemWidth(100);
        ImGui::SameLine(270);
        if (ImGui::DragScalar("X", ImGuiDataType_Float, &Interactor->planeRotXYZ.x, 1.0f, &f32_neg_90, &f32_pos_90, "%.2f", 1.0f))
        {
            if (Interactor->planeRotXYZ.x < f32_neg_90)
                Interactor->planeRotXYZ.x = f32_neg_90;
            else if (Interactor->planeRotXYZ.x > f32_pos_90)
                Interactor->planeRotXYZ.x = f32_pos_90;
        }
       }

        ImGui::Separator();

        //ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Misc");

        // wireframe mode
        // TODO: migrate to use shader to draw wireframe instead of using fixed-function ...
        //ImGui::Checkbox("Wireframe mode", &Interactor->wireframeMode);
        //ImGui::Text("Cam Pos: (%.02f, %.02f, %.02f)", Interactor->camera->Position.x, Interactor->camera->Position.y, Interactor->camera->Position.z);
        //ImGui::Text("Cam Front: (%.02f, %.02f, %.02f)", Interactor->camera->Front.x, Interactor->camera->Front.y, Interactor->camera->Front.z);
        //ImGui::Text("Cam Up: (%.02f, %.02f, %.02f)", Interactor->camera->Up.x, Interactor->camera->Up.y, Interactor->camera->Up.z);
        //ImGui::Separator();
        ImGui::Text("Target Temp: (%.02f)  +- (%0.3f)", target,error);
        //ImGui::SameLine(120);
        current = minTemp+ (1.0-temperatures[pi])*(maxTemp-minTemp);
        if(fabs(current-target)>error)
            ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f),"Current Temp: (%.03f)", current);
            else
            ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f),"Current Temp: (%.03f)", current);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void initImGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(Interactor->window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;

        case GLFW_KEY_R:
            Interactor->cameraPos =Interactor->cameraInitialPos = Interactor->camerareferenceInitialPos;
            //camerareferenceInitialPos=cameraInitialPos;
            //cameraFront= cameraInitialFront;
            Interactor->planeRotXYZ = glm::vec3(0.0f);
            Interactor->camera->Position=Interactor->cameraPos;
            Interactor->camera->WorldUp=Interactor->cameraInitialUp;
            Interactor->camera->Yaw=-90.0f;
            Interactor->camera->Pitch=0.0f;
            //camera->Front=cameraInitialFront;
            Interactor->camera->updateCameraVectors();
            Interactor->view= Interactor->camera->GetViewMatrix();
            break;

        case GLFW_KEY_1:
                Interactor->woodBox=true;
                Interactor->skyBox=false;
            break;

        case GLFW_KEY_2:
                Interactor->woodBox=false;
                Interactor->skyBox=true;
            break;

        case GLFW_KEY_0:
                Interactor->woodBox=false;
                Interactor->skyBox=false;
            break;
        case GLFW_KEY_S:
                  startok=1;
                  cicleok=true;
            break;
        case GLFW_KEY_T:
                  starok=!starok;
            break;
        case GLFW_KEY_I:
            Interactor->WriteScreenImage();
            break;
        case GLFW_KEY_N:
            pi=(pi+1)%np;
            break;
        case GLFW_KEY_A:
            Interactor->camera->ProcessKeyboard(RIGHT, Interactor->deltaTime);
            Interactor->view = Interactor->camera->GetViewMatrix();
            break;
        case GLFW_KEY_W:
            Interactor->wireframeMode = !Interactor->wireframeMode;
            break;
        case GLFW_KEY_UP:
            float cameraSpeed = 2.5 * Interactor->deltaTime;
            Interactor->fov += (float)cameraSpeed*0.5;
            if (Interactor->fov > 45.0f)
                Interactor->fov = 45.0f;
            Interactor->ourShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(Interactor->fov), 1.0f, 0.1f, 100.0f);
            Interactor->ourShader.setMat4("projection", projection);
            break;


        }
    }
}



void renderStarVertex(TMesh *m,int vertex,int r)
{
    int i,j,k,nc,auxcell,matecell,level=1;
    bool t=false;
    of::ofVertexStarIteratorSurface<TTraits> is(m);
    of::ofVertexStarIteratorSurfaceVertex<TTraits> ivs(m);
    float x,y,z;
    std::vector<int> cells;
    std::vector<int> vtxs;
    for(is.initialize(vertex); is.notFinish(); ++is){

       // std::cout << "cel = " << &is << std::endl;
        t=false;
        for(i=0;i<cells.size();i++)
            if(&is==cells[i])
                t=true;
        if(!t)
            cells.push_back(&is);

    }
    for(ivs.initialize(vertex); ivs.notFinish(); ++ivs){

       // std::cout << "vtx = " << &ivs << std::endl;
        t=false;
        for(i=0;i<vtxs.size();i++)
            if(&ivs==vtxs[i])
            {
                t=true;
                break;
            }
        if(!t)
            vtxs.push_back(&ivs);

    }
    nc=vtxs.size();
    while(level<r)
    {
        for(i=0;i<nc;i++)
        {
            auxcell=vtxs[i];
            for(is.initialize(auxcell); is.notFinish(); ++is){
                t=false;

                for(j=0;j<cells.size();j++)
                    if(&is==cells[j])
                    {
                        t=true;
                        break;
                    }
                if(!t)
                    cells.push_back(&is);
        }
            for(ivs.initialize(auxcell); ivs.notFinish(); ++ivs){

                //std::cout << "vtx = " << &ivs << std::endl;
                t=false;
                for(j=0;j<vtxs.size();j++)
                    if(&ivs==vtxs[j])
                    {
                        t=true;
                        break;
                    }
                if(!t)
                    vtxs.push_back(&ivs);

            }

        }
        level++;
        nc=vtxs.size();
    }
    nc=cells.size();
/*[62, 1320, 376, 1863, 167, 1175, 528, 1728, 68, 1327, 383, 1876, 180, 1188, 531, 1731]*/
    float *v=(float*) malloc(18*nc*sizeof(float));
    unsigned int *indices= (unsigned int*) malloc(3*nc*sizeof(unsigned int));
    for (i=0;i<nc;i++)
    {
        v[18*i]=m->getVertex(m->getCell(cells[i])->getVertexId(0))->getCoord(0);
        x=v[18*i];
        v[18*i+1]=m->getVertex(m->getCell(cells[i])->getVertexId(0))->getCoord(1);
        y=v[18*i+1];
        v[18*i+2]=m->getVertex(m->getCell(cells[i])->getVertexId(0))->getCoord(2);
        z=v[18*i+2];
        v[18*i+3]=m->getVertex(m->getCell(cells[i])->getVertexId(0))->getNormal(0);
        v[18*i+4]=m->getVertex(m->getCell(cells[i])->getVertexId(0))->getNormal(1);
        v[18*i+5]=m->getVertex(m->getCell(cells[i])->getVertexId(0))->getNormal(2);
        v[18*i+6]=m->getVertex(m->getCell(cells[i])->getVertexId(1))->getCoord(0);
        x=v[18*i+6];
        v[18*i+7]=m->getVertex(m->getCell(cells[i])->getVertexId(1))->getCoord(1);
        y=v[18*i+7];
        v[18*i+8]=m->getVertex(m->getCell(cells[i])->getVertexId(1))->getCoord(2);
        z=v[18*i+8];
        v[18*i+9]=m->getVertex(m->getCell(cells[i])->getVertexId(1))->getNormal(0);
        v[18*i+10]=m->getVertex(m->getCell(cells[i])->getVertexId(1))->getNormal(1);
        v[18*i+11]=m->getVertex(m->getCell(cells[i])->getVertexId(1))->getNormal(2);
        v[18*i+12]=m->getVertex(m->getCell(cells[i])->getVertexId(2))->getCoord(0);
        x=v[18*i+12];
        v[18*i+13]=m->getVertex(m->getCell(cells[i])->getVertexId(2))->getCoord(1);
        y=v[18*i+13];
        v[18*i+14]=m->getVertex(m->getCell(cells[i])->getVertexId(2))->getCoord(2);
        z=v[18*i+14];
        v[18*i+15]=m->getVertex(m->getCell(cells[i])->getVertexId(2))->getNormal(0);
        v[18*i+16]=m->getVertex(m->getCell(cells[i])->getVertexId(2))->getNormal(1);
        v[18*i+17]=m->getVertex(m->getCell(cells[i])->getVertexId(2))->getNormal(2);
        indices[3*i]=3*i;
        indices[3*i+1]=3*i+1;
        indices[3*i+2]=3*i+2;
    }

    if(starVAO==0)
    {
        glGenVertexArrays(1, &starVAO);
        glGenBuffers(1, &starVBO);
        glGenBuffers(1, &starEBO);
    }
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(starVAO);

        glBindBuffer(GL_ARRAY_BUFFER, starVBO);
        glBufferData(GL_ARRAY_BUFFER, 18*nc*sizeof(float), v, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*nc*sizeof(unsigned int), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    // activate shader
    Interactor->starShader.use();
    Interactor->starShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    Interactor->starShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    Interactor->starShader.setVec3("viewPos", Interactor->camera->Position);
    // pass projection matrix to shader (note that in this case it could change every frame)
    projection = glm::perspective(glm::radians(Interactor->fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    Interactor->starShader.setMat4("projection", projection);

    Interactor->starShader.setMat4("view", Interactor->view);
    // world transformation
     model = glm::mat4(1.0f);

     Interactor->starShader.setVec3("lightPos", lightPos);
     Interactor->starShader.setVec3("lightPos1", lightPos1);
     //model = glm::translate(model,glm::vec3(-2.0,-1.0,3.5));
     //model = glm::scale(model, glm::vec3(0.05f)); // move bottom-right
     //model=glm::rotate(model, glm::radians(-42.0f), glm::vec3(0.0, 0.0, 1.0));
    Interactor->starShader.setMat4("model", model);


    // draw our first triangle
    //glUseProgram(starShaderProgram);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(starVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 3*nc, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    delete v;
    delete indices;
}
