// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

// 버텍스 셰이더 소스 코드
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main(){
        gl_Position = vec4(aPos, 1.0);
    }
)";

// 프래그먼트 셰이더 소스 코드
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main(){
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

int main() {
  // GLFW 초기화 및 윈도우 생성
  if (!glfwInit()) {
    std::cerr << "GLFW 초기화 실패" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "Triangle Example", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "GLFW 윈도우 생성 실패" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // GLAD 초기화: OpenGL 함수 포인터 로딩
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "GLAD 초기화 실패" << std::endl;
    return -1;
  }

  // 셰이더 컴파일 및 프로그램 생성
  // 버텍스 셰이더
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  // 컴파일 에러 체크
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cerr << "버텍스 셰이더 컴파일 에러:\n" << infoLog << std::endl;
  }

  // 프래그먼트 셰이더
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cerr << "프래그먼트 셰이더 컴파일 에러:\n" << infoLog << std::endl;
  }

  // 셰이더 프로그램 링크
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "셰이더 프로그램 링크 에러:\n" << infoLog << std::endl;
  }
  // 더 이상 필요 없는 개별 셰이더 삭제
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // 삼각형 버텍스 데이터
  float vertices[] = {
      // x, y, z 좌표
      -0.5f, -0.5f, 0.0f,  // 왼쪽 아래
      0.5f,  -0.5f, 0.0f,  // 오른쪽 아래
      0.0f,  0.5f,  0.0f   // 위
  };

  // VAO와 VBO 생성 및 설정
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // VAO 바인딩 (이후의 VBO 및 vertex attribute 설정이 VAO에 저장됨)
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // vertex attribute 설정: layout(location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // 바인딩 해제 (필수는 아니지만 클린업을 위해)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // 렌더 루프
  while (!glfwWindowShouldClose(window)) {
    // 입력 처리 (ESC 키로 종료)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // 화면 클리어
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 삼각형 그리기
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 버퍼 교체 및 이벤트 폴링
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 할당한 리소스 정리
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
