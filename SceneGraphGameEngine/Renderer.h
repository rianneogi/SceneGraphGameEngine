#pragma once

#include "Water.h"
#include <map>

enum SHADER_TYPE {
	NONE = 0, MULTI_TEXTURE = 1, SOLID_COLOR = 2, NORMAL_MAP = 4, PARALLAX_MAP = 8, SPECULAR_MAP = 16, SKYBOX = 32, WATER = 64,
	DIRECTIONAL = 128, POINT = 256, AMBIENT = 512, SHADOW_MAP = 1024
};

struct RenderObject
{
	MeshData* mMesh;
	Material* mMaterial;
	int mShader;
	glm::mat4* mModelMat;
	RenderObject() : mMesh(NULL), mMaterial(NULL), mShader(NONE), mModelMat(NULL) {};
	RenderObject(MeshData* mesh, Material* mat, int shader, glm::mat4* model ) :
		mMesh(mesh), mMaterial(mat), mShader(shader), mModelMat(model) {};
};

struct PointLight
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 attenuation;

	PointLight(glm::vec3 p, glm::vec3 c, glm::vec3 a) : pos(p), color(c), attenuation(a) {}
};

struct DirectionalLight
{
	glm::vec3 dir;
	glm::vec3 color;
	float intensity;

	DirectionalLight(glm::vec3 d, glm::vec3 c, float i) : dir(d), color(c), intensity(i) {}
};

class Renderer
{
public:
	std::vector<MeshDataTex*> mMeshes;
	std::vector<Texture2D*> mTextures;
	//std::vector<ShaderProgram*> mShaders;
	std::map<int, ShaderProgram*> mShaders;
	std::map<int, std::vector<RenderObject>> mRenderables;

	std::vector<RenderObject> mRenderObjects;

	std::vector<DirectionalLight> mDirectionalLights;
	std::vector<PointLight> mPointLights;

	/*ShaderProgram* mAmbientShader;
	ShaderProgram* mDirectionalShader;
	ShaderProgram* mPointShader;
	ShaderProgram* mAmbientShaderTerrain;
	ShaderProgram* mDirectionalShaderTerrain;
	ShaderProgram* mPointShaderTerrain;
	ShaderProgram* mSkyboxShader;
	ShaderProgram* mWaterShader;*/

	Water* mWater;
	FrameBuffer mShadowFBO;

	Camera* mCamera;
	SkyBox* mSkyBox;

	Renderer();
	Renderer(Camera* cam);
	~Renderer();

	void loadShaders();

	void addMesh(MeshDataTex* mesh);
	void addTexture(Texture2D* tex);
	void addRenderObject(MeshData * mesh, Material * tex, int shader, glm::mat4 * model);
	void addPointLight(const PointLight& l);
	void addDirectionalLight(const DirectionalLight& l);

	void renderSkybox(const glm::mat4& view, const glm::mat4& projection);
	void renderShader(int shader, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& lightView, const glm::mat4& lightProj);
	void doShadowPass(glm::mat4& lightView, glm::mat4& lightProj);
	void renderScene(const glm::mat4& view, const glm::mat4& projection);
	void render();
};

std::string getDefsFromShaderType(int shader);

