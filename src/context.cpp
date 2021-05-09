#include "context.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {

    float vertices[] = {
        0.5f, 0.5f, 0.0f,       // top right
        0.5f, -0.5f, 0.0f,      // bottom right
        -0.5f, -0.5f, 0.0f,     // bottom left
        -0.5f, 0.5f, 0.0f,      // top left
    };
        uint32_t indices[] = {  // note that we start from 0! 점의 인덱스 표현
        0, 1, 3,                // first triangle
        1, 2, 3,                // second triangle
    };

    m_vertexLayout = VertexLayout::Create();                                       //array obj -> 버퍼 obj 순서중요

    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,
        GL_STATIC_DRAW, vertices, sizeof(float) * 12);                             //용도는 "STATIC | DYNAMIC | STREAM", "DRAW | COPY | READ"의 조합
  	
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);     //점의 위치 묘사 n | size | type | normailzed | stride | offset

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
        GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);
   
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
	  
    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);        //primitive | 인덱스의 개수 | 데이터타입 |pointer/offset
} 