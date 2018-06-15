#include "stdafx.h"
#include "FileIO.h"
void initTest()
{
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glCullFace(GL_BACK);
   Board b;
   FileIO::loadBoard("./Diplom.xml", b);
   std::string str = "ENG,NTH,YOR,WAL";
   std::vector<std::string> p = FileIO::splitString(',', str);
   Territory* naples = b.getTPtr("NAP");
   Territory* rome = b.getTPtr("ROM");
   Territory* venice = b.getTPtr("VEN");
   Territory* apulia = b.getTPtr("APU");
   
   FCON_DATA tricky = dtc_f(b.getTPtr("BUL"), b.getTPtr("GRE"), 1, -1);
   FCON_DATA tricky2 = dtc_f(b.getTPtr("GRE"), b.getTPtr("BUL"), -1, 2);
   std::string input = "";
   using namespace std;
   while (input != "exit")
   {
      cin >> input;
      
   }
}
int main(int argc, char** argv)
{
   // Initialize glfw
   
   if (!glfwInit())
   {
      fprintf(stderr, "Failed to init GLFW\n");
      return -1;
   }
   initTest();
   glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

   GLFWwindow* window;
   window = glfwCreateWindow(1024, 768, "Diplomacy Alpha", NULL, NULL);
   if (window == NULL)
   {
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glewExperimental = true;
   if (glewInit() != GLEW_OK)
   {
      fprintf(stderr, "Error: GLEW init fail\n");
      return -1;
   }
   do {
      // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();

   } // Check if the ESC key was pressed or the window was closed
   while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
      glfwWindowShouldClose(window) == 0);
   return 0;
}