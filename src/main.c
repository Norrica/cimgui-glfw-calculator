#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <cimgui.h>
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//#define WINDOW_WIDTH 1200
//#define WINDOW_HEIGHT 800
int WIN_WIDTH;
int WIN_HEIGHT;
static void error_callback(int e, const char *d) {
  printf("Error %d: %s\n",
         e,
         d);
}

/* Platform */
static GLFWwindow *win;
struct ImGuiContext *ctx;
struct ImGuiIO *io;

void gui_init() {
  // IMGUI_CHECKVERSION();
  ctx = igCreateContext(NULL);
  io = igGetIO();

  const char *glsl_version = "#version 330 core";
  ImGui_ImplGlfw_InitForOpenGL(win, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Setup style
  igStyleColorsDark(NULL);
}

void gui_terminate() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(ctx);
}

void gui_render() {
  igRender();
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

int input() {
  return 1;
}
static int Filter(ImGuiInputTextCallbackData *data) {
  if (data->EventChar < 256
      && strchr("sincostansqrtlng+-*/^%x", (char) data->EventChar))
    return 0;
  return 1;
}
void gui_update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();

  static int calc_open;
  igSetNextWindowPos((ImVec2) {0, 0}, ImGuiCond_Appearing, (ImVec2) {0, 0});
  igSetNextWindowSize((struct ImVec2) {WIN_WIDTH / 4, WIN_HEIGHT / 4},
                      ImGuiCond_Appearing);
  igBegin("Calculator", &calc_open, 0);
  static char buf[256];
  igInputText("", buf, 255,
              ImGuiInputTextFlags_AutoSelectAll |
                  ImGuiInputTextFlags_CharsNoBlank, &Filter, NULL);

  int c_count = 2;
  float but_size_w = 80;//igGetWindowWidth() * ((float) c_count / 10);
  float but_size_h = 80;//igGetWindowHeight() * (((float) c_count - 3) / 10);
  igColumns(c_count, "test", 1);
  //for (int i = 0; i < c_count; i++)
    //igSetColumnWidth(i, but_size_w*c_count);
  ImVec2 size = {but_size_w, but_size_h};
  float spacing = 5;

    igSeparator();
  for (int i = 0; i < 10; ++i) {
    if ((i % 3) != 0)
      igSameLine(0, spacing);
    char l[2];
    snprintf(l, 2, "%d", i);
    if (igButton(l, size)) {
      strcat(buf, l);
    }
  }
  igSameLine(0, spacing);
  if (igButton("C", size)) {
    buf[0] = '\0';
  }
  igSameLine(0, spacing);
  if (igButton("=", size)) {
    //TODO check for x
    // TODO invoke calc
  }
  igNextColumn();
  char *ops = "()+-*/^%";
  for (int i = 0; i < 8; ++i) {
    if ((i % 3) != 0)
      igSameLine(0, spacing);
    char l[2];
    snprintf(l, 2, "%c", ops[i]);
    if (igButton(l, size)) {
      strcat(buf, l);
    }
  }
  igNextColumn();
  igEnd();

  // // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway.
  // // Here we just want to make the demo initial state a bit more friendly!
  // igSetNextWindowPos((struct ImVec2){0,0}, ImGuiCond_FirstUseEver,(struct ImVec2){0,0} );
  igShowDemoWindow(NULL);
}

int main(int argc, char **argv) {

  /* GLFW */
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    fprintf(stdout, "[GFLW] failed to init!\n");
    exit(1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  WIN_WIDTH = mode->width;
  WIN_HEIGHT = mode->height;
  win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "CIMGUI", NULL, NULL);
  glfwMakeContextCurrent(win);
  bool err = gl3wInit();
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

  gui_init();

  // glfwSetWindowSizeCallback(win, onResize);
  int width, height;
  glfwGetWindowSize(win, &width, &height);

  /* OpenGL */
  glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

  while (!glfwWindowShouldClose(win)) {
    /* Input */
    glfwPollEvents();

    gui_update();

    /* Draw */
    glfwGetWindowSize(win, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    gui_render();

    glfwSwapBuffers(win);
  }

  gui_terminate();
  glfwTerminate();

}
