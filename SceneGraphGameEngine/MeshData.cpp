#include "MeshData.h"

MeshData::MeshData()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

MeshData::~MeshData()
{
	clear();
}

void MeshData::clear()
{
	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VB);

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IB);
}

bool MeshData::init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
{
	clear();

	glGenBuffers(1, &VB);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	/*for (int i = Vertices.size()-1;i >=0;i--)
	{
	printf("added %f %f %f\n", Vertices[i].Pos.x, Vertices[i].Pos.y, Vertices[i].Pos.z);
	}*/

	// Generate an ID for the index buffer.
	glGenBuffers(1, &IB);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	NumIndices = Indices.size();
	printf("making mesh %d %d\n", (int)VB, (int)IB);
	return true;
}

void MeshData::render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, UV));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	//printf("4 %d, using buffers %d and %d\n", mEntries[i].NumIndices, mEntries[i].VB, mEntries[i].IB);
	//printf("Mesh ptr draw: %d\n", this);
	//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

	//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
	//	//m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
	//	//m_Textures[MaterialIndex]->bind();
	//}
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("DRAW ERROR: %d", err);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

MeshDataTexArray::MeshDataTexArray()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

MeshDataTexArray::~MeshDataTexArray()
{
	clear();
}

void MeshDataTexArray::clear()
{
	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VB);

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IB);
}

bool MeshDataTexArray::init(const std::vector<VertexTexArray>& Vertices, const std::vector<unsigned int>& Indices)
{
	clear();

	glGenBuffers(1, &VB);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexArray)*Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	/*for (int i = Vertices.size()-1;i >=0;i--)
	{
	printf("added %f %f %f\n", Vertices[i].Pos.x, Vertices[i].Pos.y, Vertices[i].Pos.z);
	}*/

	// Generate an ID for the index buffer.
	glGenBuffers(1, &IB);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	NumIndices = Indices.size();
	return true;
}

void MeshDataTexArray::render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexArray), (const GLvoid*)offsetof(VertexTexArray, Pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexArray), (const GLvoid*)offsetof(VertexTexArray, UV));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexArray), (const GLvoid*)offsetof(VertexTexArray, Normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	//printf("4 %d, using buffers %d and %d\n", mEntries[i].NumIndices, mEntries[i].VB, mEntries[i].IB);
	//printf("Mesh ptr draw: %d\n", this);
	//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

	//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
	//	//m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
	//	//m_Textures[MaterialIndex]->bind();
	//}
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	//debugOpengl("mesh entry 2d array draw");
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);
}

MeshDataSolidColor::MeshDataSolidColor()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

MeshDataSolidColor::~MeshDataSolidColor()
{
	clear();
}

bool MeshDataSolidColor::init(const std::vector<VertexSolidColor>& Vertices, const std::vector<unsigned int>& Indices)
{
	clear();

	glGenBuffers(1, &VB);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexSolidColor)*Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	/*for (int i = Vertices.size()-1;i >=0;i--)
	{
	printf("added %f %f %f\n", Vertices[i].Pos.x, Vertices[i].Pos.y, Vertices[i].Pos.z);
	}*/

	// Generate an ID for the index buffer.
	glGenBuffers(1, &IB);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	NumIndices = Indices.size();
	return true;
}

void MeshDataSolidColor::render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSolidColor), (const GLvoid*)offsetof(VertexSolidColor, Pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSolidColor), (const GLvoid*)offsetof(VertexSolidColor, Color));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSolidColor), (const GLvoid*)offsetof(VertexSolidColor, Normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	//printf("4 %d, using buffers %d and %d\n", mEntries[i].NumIndices, mEntries[i].VB, mEntries[i].IB);
	//printf("Mesh ptr draw: %d\n", this);
	//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

	//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
	//	//m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
	//	//m_Textures[MaterialIndex]->bind();
	//}
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	//debugOpengl("mesh entry 2d array draw");
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);
}

void MeshDataSolidColor::clear()
{
	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VB);

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IB);
}
