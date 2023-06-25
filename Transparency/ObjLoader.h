#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <vector>


	/* Very, VERY simple OBJ loader.
Here is a short list of features a real function would provide :
 - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
 - Animations & bones (includes bones weights)
 - Multiple UVs
 - All attributes should be optional, not "forced"
 - More stable. Change a line in the OBJ file and it crashes.
 - More secure. Change another line and you can inject code.
 - Loading from memory, stream, etc
*/
	bool simpleOBJLoader(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
	//Assimp obj loader
	bool assimpOBJLoader(const char* path, std::vector<unsigned short>& indices, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals);


