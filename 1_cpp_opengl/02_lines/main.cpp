// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

const char* vertex_shader_source = R"(
  #version 330 core
  layout (location = 0) in vec3 pos;
  void main () {
    gl_Position = vec4(pos, 1.0);
  }
)";

const char* fragment_shader_source = R"(
    #version 330 core
    out vec4 FragColor;
    void main(){
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

int main() {
  if (!glfwInit()) {
    std::cerr << "failed to init GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  auto window = glfwCreateWindow(800, 600, "Lines Example", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize GLAD" << std::endl;
    return -1;
  }

  int success;
  char info_log[512];

  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cerr << "failed to compile vertex shader: " << info_log << std::endl;
    return -1;
  }

  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cerr << "failed to compile fragment shader: " << info_log << std::endl;
    return -1;
  }

  auto shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
    std::cerr << "failed to link shader program: " << info_log << std::endl;
    return -1;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  float vertices[] = {
      // x, y, z
      0.0f,  0.5f,  0.0f,  // v3
      0.5f,  0.0f,  0.0f,  // v1
      0.3f,  -0.5f, 0.0f,  // v4
      -0.3f, -0.5f, 0.0f,  // v2
      -0.5f, 0.0f,  0.0f,  // v5
  };

  unsigned int vertex_array_object, vertex_buffer_object;

  // 1개의 VAO를 생성하고 그 아이디를 vertex_array_object에 저장
  glGenVertexArrays(1, &vertex_array_object);

  // 1개의 버퍼 오브젝트 생성하고 그 아이디를 vertex_buffer_object에 저장
  glGenBuffers(1, &vertex_buffer_object);

  // vertex_array_object를 vertex array로 바인딩 ?? 이게 뭘 하는거지
  glBindVertexArray(vertex_array_object);

  // vertex_buffer_object를 GL_ARRAY_BUFFER로 바인딩,
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  // 버퍼에 실제 정점 데이터를 업로드
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // vertex attribute설정: layout(location = 0)

  // 0번 attribute를 사용할거다
  glEnableVertexAttribArray(0);
  // 0번 attri, 3개, 데이터는 float, 노멀라이징x, stride는 float 3개, offset 0번
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  // 렌더 루프
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_LINE_LOOP, 0, 5);

    // 버퍼 교체 및 이벤트 폴링
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 할당한 리소스 정리
  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteProgram(shader_program);

  glfwTerminate();

  return 0;
}
