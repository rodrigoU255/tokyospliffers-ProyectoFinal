#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include "LinearMath/btDefaultMotionState.h"

#include "Mesh.h"
#include  "Shader.h"

using namespace std;





struct BoneInfo
{
	int id;
	glm::mat4 offset;
};

GLint TextureFromEmbedded(const aiTexture* embeddedTex);


GLint TextureFromFile(const char *path, string directory);

class Model
{
public:
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model(GLchar *path, bool bIsFlippedUV = false)
	{
		this->loadModel(path,bIsFlippedUV);
	}

	// Draws the model, and thus all its meshes
	void Draw(Shader shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
		{
			this->meshes[i].Draw(shader);
		}
	}



	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;

	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount() { return m_BoneCounter; }


private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

										/*  Functions   */
										// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string path, bool bIsFlippedUV)
	{

		//const aiScene* scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
		//const aiScene* scene = aiImportFile(path.c_str(),
		//	aiProcess_Triangulate |
		//	//aiProcess_FlipUVs |
		//	aiProcess_GenSmoothNormals |
		//	aiProcess_LimitBoneWeights
		//);

		unsigned int flags = aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_LimitBoneWeights;

		if (!bIsFlippedUV)
		{
			flags |= aiProcess_FlipUVs;
		}

		const aiScene* scene = aiImportFile(path.c_str(), flags);

		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP:: " << aiGetErrorString() << endl;
			return;
		}

		// Retrieve the directory path of the filepath
		size_t lastSlash = path.find_last_of("/\\");
		this->directory = (lastSlash != std::string::npos) ? path.substr(0, lastSlash) : ".";
		//this->directory = path.substr(0, path.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);

		// IMPORTANTE: Liberar la memoria de la escena al terminar el proceso
		aiReleaseImport(scene);

		// Read file via ASSIMP
	//	Assimp::Importer importer;
	//	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	//	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	//	// Check for errors
	//	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	//	{
	//		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
	//		return;
	//	}
	//	// Retrieve the directory path of the filepath
	//	this->directory = path.substr(0, path.find_last_of('/'));

	//	// Process ASSIMP's root node recursively
	//	this->processNode(scene->mRootNode, scene);
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			this->meshes.push_back(this->processMesh(mesh, scene));
		}

		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// Data to fill
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

							  // Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// Normals
		
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}
			else
			{
				// Si el FBX no tiene normales, asignamos un valor por defecto para no romper los shaders de luz
				vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			/*vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;*/

			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			// 2. Specular maps
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data

		ExtractBoneWeightForVertices(vertices, mesh, scene);
		return Mesh(vertices, indices, textures);
	}

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName, const aiScene* scene)
	{
		vector<Texture> textures;

		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;

			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)

					break;
				}
			}

			if (!skip)
			{   // If texture hasn't been loaded already, load it
				Texture texture;
				std::string texPath = str.C_Str();

				// Las texturas embebidas en .glb/.fbx llegan con el formato "*0", "*1", etc.
				if (!texPath.empty() && texPath[0] == '*')
				{
					int texIndex = std::stoi(texPath.substr(1));
					if (texIndex >= 0 && texIndex < static_cast<int>(scene->mNumTextures))
					{
						texture.id = TextureFromEmbedded(scene->mTextures[texIndex]);
					}
				}
				else
				{
					texture.id = TextureFromFile(str.C_Str(), this->directory);
				}
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);

				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}

		return textures;
	}

	void SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.m_BoneIDs[i] < 0)
			{
				vertex.m_Weights[i] = weight;
				vertex.m_BoneIDs[i] = boneID;
				break;
			}
		}
	}

	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
	{
		for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

			if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
			{
				if (m_BoneCounter >= 100) continue; // Límite de MAX_BONES en el shader

				BoneInfo newBoneInfo;
				newBoneInfo.id = m_BoneCounter;

				aiMatrix4x4 from = mesh->mBones[boneIndex]->mOffsetMatrix;
				glm::mat4 to;
				to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
				to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
				to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
				to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
				newBoneInfo.offset = to;

				m_BoneInfoMap[boneName] = newBoneInfo;
				boneID = m_BoneCounter;
				m_BoneCounter++;
			}
			else
			{
				boneID = m_BoneInfoMap[boneName].id;
			}

			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				if (vertexId < vertices.size() && weight > 0.0f)
				{
					SetVertexBoneData(vertices[vertexId], boneID, weight);
				}
			}
		}
	}
};

GLint TextureFromFile(const char* path, string directory) {
	string filename = string(path);
	// Limpiar prefijos URI que a veces deja Assimp al leer COLLADA (.dae)
	if (filename.find("file:///") == 0) filename = filename.substr(8);
	else if (filename.find("file://") == 0) filename = filename.substr(7);

	string fullPath = (directory == "." || directory.empty()) ? filename : (directory + "/" + filename);

	int width, height, channels;
	unsigned char* image = SOIL_load_image(fullPath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	if (!image)
	{
		std::cout << "ERROR::TEXTURA:: No se pudo abrir: " << fullPath
			<< " | SOIL: " << SOIL_last_result() << std::endl;
		return 0;
	}

	std::cout << "OK::TEXTURA cargada: " << fullPath << " (" << width << "x" << height << ")" << std::endl;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}
GLint TextureFromEmbedded(const aiTexture* embeddedTex)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, channels;
	unsigned char* image = nullptr;

	if (embeddedTex->mHeight == 0)
	{
		image = SOIL_load_image_from_memory(
			reinterpret_cast<const unsigned char*>(embeddedTex->pcData),
			embeddedTex->mWidth,
			&width, &height, &channels, SOIL_LOAD_AUTO
		);
	}

	if (image)
	{
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
	}

	return textureID;
}

btRigidBody* LoadRigidCollition(const std::string& path, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{

	const aiScene* scene = aiImportFile(path.c_str(), aiProcess_Triangulate);

	if (!scene || !scene->mRootNode) {
		std::cout << "Error al cargar colision: " << aiGetErrorString() << std::endl;
		return nullptr;
	}


	btTriangleMesh* trimesh = new btTriangleMesh();


	for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
		aiMesh* mesh = scene->mMeshes[m];

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];


			if (face.mNumIndices == 3) {
				aiVector3D v0 = mesh->mVertices[face.mIndices[0]];
				aiVector3D v1 = mesh->mVertices[face.mIndices[1]];
				aiVector3D v2 = mesh->mVertices[face.mIndices[2]];

				trimesh->addTriangle(
					btVector3(v0.x, v0.y, v0.z),
					btVector3(v1.x, v1.y, v1.z),
					btVector3(v2.x, v2.y, v2.z),
					true
				);
			}
		}
	}

	aiReleaseImport(scene);

	btBvhTriangleMeshShape* terrainShape = new btBvhTriangleMeshShape(trimesh, true);

	/*terrainShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));*/


	btTransform transform;
	transform.setIdentity();
	//transform.setOrigin(btVector3(0, 0, 0)); // El origen debe coincidir con el modelo visual
	transform.setOrigin(btVector3(position.x, position.y, position.z)); // El origen debe coincidir con el modelo visual


	btQuaternion rot;
	rot.setEuler(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)); // Para en radianes
	//rot.setEuler(rotation.y, rotation.x, rotation.z); // En euler
	transform.setRotation(rot);


	terrainShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));


	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, terrainShape, btVector3(0, 0, 0));

	btRigidBody* terrainBody = new btRigidBody(rbInfo);
	// Quitar fricción si no quieres que la cápsula se atore en las pendientes
	terrainBody->setFriction(0.0f);

	return terrainBody;
}