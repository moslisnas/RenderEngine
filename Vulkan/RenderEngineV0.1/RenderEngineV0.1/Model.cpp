#include "Model.h"
#ifndef TINYOBJLOADER_IMPLEMENTATION
	#define TINYOBJLOADER_IMPLEMENTATION
	#include <tiny_obj_loader.h>
	#include <unordered_map>
#endif

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>Model</c> class.
/// </summary>
Model::Model(){
	this->numVertices = 0;
	this->numIndices = 0;
}
/// <summary>
/// Destructor of <c>Model</c> class.
/// </summary>
Model::~Model(){
}
#pragma endregion

#pragma region Loading methods
/// <summary>
/// Loads a rectangle as model mesh.
/// </summary>
void Model::loadRectangle(){
	this->numVertices = 4;
	this->numIndices = 6;
	vertices.resize(numVertices);
	indices.resize(numIndices);

	//Coordinates data.
	Vertex auxVertex = {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
	vertices[0] = auxVertex;
	auxVertex = {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
	vertices[1] = auxVertex;
	auxVertex = {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}};
	vertices[2] = auxVertex;
	auxVertex = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
	vertices[3] = auxVertex;

	//Indices data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;
}
/// <summary>
/// Loads a rectangle as model mesh.
/// </summary>
void Model::loadRectangle2(){
	this->numVertices = 4;
	this->numIndices = 6;
	vertices.resize(numVertices);
	indices.resize(numIndices);

	//Coordinates data.
	Vertex auxVertex = {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
	vertices[0] = auxVertex;
	auxVertex = {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
	vertices[1] = auxVertex;
	auxVertex = {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}};
	vertices[2] = auxVertex;
	auxVertex = {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
	vertices[3] = auxVertex;

	//Indices data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;
}
/// <summary>
/// Loads an archive as model mesh.
/// <param name="file">Path of the model file.</param>
/// </summary>
void Model::loadFileModel(char * file){
	//Model data parameters.
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	//Load model data through the file.
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file))
		throw std::runtime_error(err);

	//Data to avoid repeat vertices.
	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
	for(const auto& shape : shapes){
		for(const auto& index : shape.mesh.indices){
			//Reading vertices data.
			Vertex vertex = {};
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color = {1.0f, 1.0f, 1.0f};
			//Pass data to our struct data.
			if(uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
/// <summary>
/// Loads an archive as model mesh.
/// <param name="file">Path of the model file.</param>
/// </summary>
void Model::loadFileModel2(char * file){
	//Model data parameters.
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	//Load model data through the file.
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file))
		throw std::runtime_error(err);

	//Data to avoid repeat vertices.
	std::unordered_map<Vertex, uint32_t> uniqueVertices ={};
	for(const auto& shape : shapes){
		for(const auto& index : shape.mesh.indices){
			//Reading vertices data.
			Vertex vertex ={};
			vertex.pos ={
				attrib.vertices[3 * index.vertex_index + 0] + 1.5,
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord ={
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color ={1.0f, 1.0f, 1.0f};
			//Pass data to our struct data.
			if(uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

/// <summary>
/// Loads an archive as model mesh.
/// <param name="file">Path of the model file.</param>
/// </summary>
void Model::loadFileModel3(char * file){
	//Model data parameters.
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	//Load model data through the file.
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file))
		throw std::runtime_error(err);

	//Data to avoid repeat vertices.
	std::unordered_map<Vertex, uint32_t> uniqueVertices ={};
	for(const auto& shape : shapes){
		for(const auto& index : shape.mesh.indices){
			//Reading vertices data.
			Vertex vertex ={};
			vertex.pos ={
				attrib.vertices[3 * index.vertex_index + 0] - 1.5,
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord ={
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color ={1.0f, 1.0f, 1.0f};
			//Pass data to our struct data.
			if(uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
#pragma endregion