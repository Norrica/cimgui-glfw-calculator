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
#include <stdbool.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <input.h>
#include <graph.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <float.h>
#include <yard.h>
int WIN_WIDTH;
int WIN_HEIGHT;
static void error_callback(int e, const char *d) {
  printf("Error %d: %s\n",
         e,
         d);
}

//static float Saw(void*j,int i) { return (i & 1) ? 1.0f : -1.0f; }

/* Platform */
static GLFWwindow *win;
struct ImGuiContext *ctx;
struct ImGuiIO *io;

void gui_init() {
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

int Filter(ImGuiInputTextCallbackData *data) {

  if (data->EventChar < 256
      && strchr("sincostansqrtlng+-*/^%x1234567890().",
                (char) (data->EventChar))) {
    return 0;
  }
  return 1;
}

float max(const float *arr, int n) {
  float max = DBL_MIN;
  for (int i = 0; i < n; ++i) {
    if (arr[i] > max)max = arr[i];
  }
  return max;
}
float min(const float *arr, int n) {
  float min = arr[0];
  for (int i = 0; i < n; ++i) {
    if (arr[i] < min)min = arr[i];
  }
  return min;
}
static char error_message[256];
static int show_calc = 0;
int ce = 1;
static int left_x = -(2000000 / 2);
static int right_x = 2000000 / 2;
static float graph_points_x;
static float graph_points_y[2000000] = {};

static float getter(void *j, int i) {
  return graph_points_y[i];
}

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))       // Size of a static C-style array. Don't use on pointers!

void gui_update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();

  igSetNextWindowPos((ImVec2) {0, 0}, ImGuiCond_Always, (ImVec2) {0, 0});
  igSetNextWindowSize((struct ImVec2) {WIN_WIDTH / 2, WIN_HEIGHT / 2},
                      ImGuiCond_Appearing);

  static char buf[256];
  //error_message[0]=0;
  if (igBegin("Calculator", NULL, 0)) {
    if (igIsKeyPressed('=', 0) && (igGetIO()->KeyShift))
      strcat(buf, "+");
    for (int i = 48; i < 58; ++i) {
      if (!igGetIO()->KeyShift) {
        if (igIsKeyPressed(i, 1)) {
          char c = (char) i;
          strncat(buf, &c, 1);
        }
        if (igIsKeyPressed(i + 320 - 48, 0)) {
          char c = (char) i;
          strncat(buf, &c, 1);
        }
      } else if (igIsKeyPressed(i, 0)) {
        if (i == '5') {
          strcat(buf, "%");
        }
        if (i == '6') {
          strcat(buf, "^");
        }
        if (i == '8') {
          strcat(buf, "*");
        }
        if (i == '9') {
          strcat(buf, "(");
        }
        if (i == '0') {
          strcat(buf, ")");
        }
      }
    }
    char ops3[] = "SINCOTAQRLGX";
    for (int i = 0; i < strlen(ops3); ++i) {
      if (igIsKeyPressed(ops3[i], 0)) {
        char s = ops3[i] + 32;
        strncat(buf, &s, 1);
      }
    }
    if (igIsKeyPressed(259, 0)) {
      buf[strlen(buf) - 1] = 0;
    }
    //float textsize = igCalcTextSize(buf, NULL, false, 0).x;
    //igSetCursorPosX(igGetContentRegionAvail().x / 2 - textsize / 2);
    igInputText("", buf, 255,
                ImGuiInputTextFlags_AutoSelectAll
                    | ImGuiInputTextFlags_CharsNoBlank
                    | ImGuiInputTextFlags_CallbackCharFilter, &Filter, NULL);
    igSameLine(0, 5);
    if (error_message[0] != 0)
      igTextColored((ImVec4) {1, 0, 0, 1}, "%s", error_message);
    int c_count = 3;
    float but_size_w = 80;
    float but_size_h = 80;
    igColumns(c_count, "test", 1);
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
    if (igButton("C", size) || igIsKeyPressed(261, 0)) {
      buf[0] = '\0';
      show_calc = 0;
      error_message[0] = 0;
    }
    igSameLine(0, spacing);
    if (igButton(".", size)) {
      strcat(buf, ".");
    }
    igNextColumn();
    char *ops = "()+-*/^%=";
    for (int i = 0; i < 8; ++i) {
      if ((i % 3) != 0)
        igSameLine(0, spacing);
      char l[2];
      snprintf(l, 2, "%c", ops[i]);
      if (igButton(l, size)) {
        strcat(buf, l);
      }
    }
    igSameLine(0, spacing);

    if ((igButton("=", size)
        || (igIsKeyPressed('=', 0) && !(igGetIO()->KeyShift)))
        || igIsKeyPressed(336, 0)
        || igIsKeyPressed(335, 0)
        || igIsKeyPressed(257, 0)
        ) {
      if (buf[0] != '\0') {
        if (contains_x(buf)) {
          show_calc = 1;

          char *ss = truncate_trig(buf);  //  contains 'x'
          char *sss = str_replace(ss, "x", "%lf");
          for (int x_count = left_x, i = 0; x_count < right_x;
               x_count += 1, i++) {
            char *str = format_x(sss, x_count);
            graph_points_y[i] = yard(str, error_message);
            free(str);
            if (*error_message != '\0') {
              break;
            }
          }
          free(sss);
          free(ss);
          show_calc = *error_message == '\0';
        } else {
          sprintf(buf, "%lf", calc_res(buf, error_message));
        }

      }
    }

    igNextColumn();
    char *ops2[] =
        {"sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "log", "ln", "x"};

    for (int i = 0; i < 9; ++i) {
      if ((i % 3) != 0)
        igSameLine(0, spacing);
      char l[5];
      snprintf(l, 5, "%s", ops2[i]);
      if (igButton(l, size)) {
        strcat(buf, l);
      }
    }
    igNextColumn();

  }

  igEnd();
  igSetNextWindowPos((ImVec2) {WIN_WIDTH / 2, 0},
                     ImGuiCond_Appearing,
                     (ImVec2) {0, 0}
  );
  igSetNextWindowSize((struct ImVec2) {
                          WIN_WIDTH / 2, WIN_HEIGHT / 2},
                      ImGuiCond_Appearing);
//show_calc = 1;
  if (show_calc) {
    float n = min(graph_points_y, right_x - left_x);
    float x = max(graph_points_y, right_x - left_x);
    igBegin("Graph", NULL, 0);
    igLabelText("min", "%f", n);
    igSameLine(0, 5);
    igLabelText("max", "%f", x);
    static int display_count = 200000;
    display_count = right_x - left_x;
    igSliderInt("Left", &left_x, -1000000, 0, "%d");
    igDragInt("##Left", &left_x, 1.f, -1000000, 0, "%d");
    if (igIsItemDeactivatedAfterEdit()) {
      char *ss = truncate_trig(buf);  //  contains 'x'
      char *sss = str_replace(ss, "x", "%lf");
      for (int i = left_x, j = 0; i < right_x; i++) {
        char *str = format_x(sss, i);
        graph_points_y[j++] = yard(str, error_message);
        free(str);
      }
      free(ss);
      free(sss);
    }
    igSliderInt("Right", &right_x, 0, 1000000, "%d");

    igDragInt("##Right", &right_x, 1.f, 0, 1000000, "%d");
    if (igIsItemDeactivatedAfterEdit()) {
      char *ss = truncate_trig(buf);  //  contains 'x'
      char *sss = str_replace(ss, "x", "%lf");
      for (int i = left_x, j = 0; i < right_x; i++) {
        char *str = format_x(sss, i);
        graph_points_y[j++] = yard(str, error_message);
        free(str);
      }
      free(ss);
      free(sss);
    }
    display_count = right_x - left_x;
    ImVec2 size = igGetContentRegionAvail();
    igPlotLinesFnPtr("Lines",
                     &getter,
                     NULL,
                     display_count,
                     0,
                     NULL,
                     MAXFLOAT,
                     MAXFLOAT,
                     size
    );

    igSeparator();
    igEnd();
  }
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
