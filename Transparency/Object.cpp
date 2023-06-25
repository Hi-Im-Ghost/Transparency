#include "Object.h"


Object::Object(const std::string& path)
{
    bool success = loadModel(path);
    if (success) {
        initBuffers();
        modelMatrix = glm::mat4(1.0f);
    }
}

bool Object::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model: " << importer.GetErrorString() << std::endl;
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0]; // W tym przyk³adzie zak³adamy, ¿e jest tylko jeden mesh w scenie

    processMesh(mesh);

    return true;
}

void Object::processMesh(const aiMesh* mesh) {
    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    indices.reserve(3 * mesh->mNumFaces);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
}

void Object::initBuffers()
{
    //VAO 
    glGenVertexArrays(1, &VAO);
    //Funkcja do wiazania obiektu tablicy wierzcholkow
    glBindVertexArray(VAO);
    //VBO
    glGenBuffers(1, &VBO);
    //Funkcja do wiazania bufora z typem 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Funkcja do przydzielenia pamieci dla bufora
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    //Funkcja do okreslenia jak opengl ma interpretowac dane wierzcholkow
    //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    //tex
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}



void Object::setShaderUniforms(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& objectColor, const glm::vec3& lightColor, const glm::vec3& lightPos, const glm::vec3& viewPos)
{
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), lightColor.r, lightColor.g, lightColor.b);
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
}

void Object::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Object::setModelMatrix(const glm::mat4& model)
{
    modelMatrix = model;
}

void Object::translate(const glm::vec3& translation)
{
    modelMatrix = glm::translate(modelMatrix, translation);
}

void Object::rotate(float angle, const glm::vec3& axis)
{
    modelMatrix = glm::rotate(modelMatrix, angle, axis);
}

void Object::scale(const glm::vec3& scale)
{
    modelMatrix = glm::scale(modelMatrix, scale);
}