#include "glslshadermanager.h"

#include <iostream>
#include <memory>

#include "utils.h"

GlslShaderManager::GlslShaderManager()
{
	//_shaders = std::vector<GlslShader>(uint(Shaders::Count));
}

GlslShaderManager::~GlslShaderManager()
{
	deleteShaders();
}

std::shared_ptr<GlslShaderManager> GlslShaderManager::instance()
{
	static std::shared_ptr<GlslShaderManager> instance = std::make_shared<GlslShaderManager>(GlslShaderManager());
	return instance;
}

//GlslShaderManager& GlslShaderManager::instance()
//{
//	static GlslShaderManager *instance = new GlslShaderManager();
//	return *instance;
//}

void GlslShaderManager::initializeShaders()
{
	_shaders = std::vector<GlslShader>(uint(Shaders::Count));
	std::cout << "Initializeing default shaders..." << std::endl;
	initTextureToScreenShader();
	initPassThroughShader();
	initShadowsAndDiffuseShader();
	initLinealDeepthShader();
	initTranslucecyhader();
	initMainRenderShader();
	initSSSSHori();
	initSSSSVert();
	initSeparableSSSSHori();
	initSeparableSSSSVert();
	initToneMap();
	initSSSSGaussianHori();
	initSSSSGaussianVert();
	initImageBlurHori();
	initImageBlurVert();
}

void GlslShaderManager::deleteShaders()
{
	std::cout << "Deleteing default shaders..." << std::endl;
	for (auto &shader : _shaders) {
		shader.deleteShaderProgram();
	}
	_shaders.clear();
}

void GlslShaderManager::reloadShaders()
{
	std::cout << "Reloading default shaders..." << std::endl;
	deleteShaders();
	initializeShaders();
}

std::shared_ptr<GlslShader> GlslShaderManager::getShader(Shaders shader)
{
	assert(uint(shader) < _shaders.size());
	return std::make_shared<GlslShader>(_shaders[shader]);
}

void GlslShaderManager::initTextureToScreenShader()
{

	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/screen_space_quad.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::TEXTURE_TO_SCREEN] = shader;
	std::cout << "\tTexture to screen shader initialized" << std::endl;
}

void GlslShaderManager::initPassThroughShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/pass_through_shader.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/pass_through_shader.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addAttribute("vNormal");
	shader.addUniform("MVP");
	shader.addUniform("color");
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::PASS_THROUGH] = shader;
	std::cout << "\tPass Through shader initialized" << std::endl;
}

void GlslShaderManager::initShadowsAndDiffuseShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/diffuse_and_shadows.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/diffuse_and_shadows.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addAttribute("vNormal");
	shader.addUniform("MVP");
	shader.addUniform("MV");
	//shader.addUniform("M");
	shader.addUniform("N");
	shader.addUniform("S");
	shader.addUniform("eye_light_position");
	shader.addUniform("shadow_map");
	glUniform1i(shader("shadow_map"), 0);
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::SHADOWS_AND_DIFFUSE] = shader;
	std::cout << "\tShadows and diffuse shader initialized" << std::endl;
}

//void GlslShaderManager::initThicknessShader()
//{
//	GlslShader shader;
//	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/thickness_shader.vert");
//	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/thickness_shader.frag");
//	shader.createAndLinkProgram();
//	shader.use();
//	shader.addAttribute("vVertex");
//	shader.addUniform("MVP");
//	shader.addUniform("MV");
//	shader.addUniform("z_far");
//	shader.unUse();
//	checkCritOpenGLError();
//
//	_shaders[Shaders::THICKNESS_SHADER] = shader;
//	std::cout << "\tthickness shader initialized" << std::endl;
//}

void GlslShaderManager::initLinealDeepthShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/linear_depth_shader.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/linear_depth_shader.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("M");
	shader.addUniform("V");
	shader.addUniform("P");
	shader.addUniform("z_far");
	shader.addUniform("light_position");
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::LINEAL_DEEPTH] = shader;
	std::cout << "\tLineal deepth shader initialized" << std::endl;
}

void GlslShaderManager::initTranslucecyhader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/translucency_shader.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/translucency_shader.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addAttribute("vNormal");
	shader.addUniform("MVP");
	shader.addUniform("MV");
	shader.addUniform("M");
	shader.addUniform("N");
	shader.addUniform("V_L");
	shader.addUniform("BP_L");
	shader.addUniform("light_position");
	shader.addUniform("z_far");
	shader.addUniform("shadow_map");
	glUniform1i(shader("shadow_map"), 0);
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::TRANSLUCENCY_SHADER] = shader;
	std::cout << "\tTransmitance shader initialized" << std::endl;
}

void GlslShaderManager::initMainRenderShader()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/mainRender_pass.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/mainRender_pass.frag");
	shader.createAndLinkProgram();
	shader.use();

	shader.addAttribute("vVertex");
	shader.addAttribute("vNormal");
	shader.addAttribute("vColor");

	shader.addUniform("curr_WorldViewProjM");
	//shader.addUniform("prev_WorldViewProjM");
	shader.addUniform("worldInverseTransposeM");
	shader.addUniform("worldM");
	shader.addUniform("viewM");
	shader.addUniform("m_camera_pos");
	shader.addUniform("z_far");
	shader.addUniform("m_ambientcomp");
	shader.addUniform("spec_int");
	shader.addUniform("light_position");
	shader.addUniform("shadow_map");
	glUniform1i(shader("shadow_map"), 0);
	shader.addUniform("lightViewProjBiasM");

	shader.addUniform("lightViewM");
	shader.addUniform("lightProjBiasM");
	shader.addUniform("light_far_plane");
	shader.addUniform("lightLinearShadowMap");
	glUniform1i(shader("lightLinearShadowMap"), 1);
	shader.addUniform("sssEnabled"); 
	shader.addUniform("sssWidth");
	shader.addUniform("translucency");

	shader.addUniform("texture_enabled"); 
	shader.addUniform("diffuse_color_texture");
	glUniform1i(shader("diffuse_color_texture"), 2);

	shader.addUniform("ao_texture"); 
	glUniform1i(shader("ao_texture"), 3);

	shader.addUniform("normal_texture"); 
	glUniform1i(shader("normal_texture"), 4);

	shader.addUniform("viewInverseTransposeM"); 
	shader.addUniform("z_near");
	shader.addUniform("roughness");
	
	shader.addUniform("diffuse_env");
	glUniform1i(shader("diffuse_env"), 5);

	shader.addUniform("bumpiness");


	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::MAIN_RENDER_SHADER] = shader;
	std::cout << "\tMain render shader initialized" << std::endl;
	
}

void GlslShaderManager::initSSSSHori()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/Ssss_blur_horiz.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");

	shader.addUniform("pixel_size");
	shader.addUniform("gaussian");
	shader.addUniform("correction");
	shader.addUniform("sssWidth");
	shader.addUniform("cam_fovy");
	shader.addUniform("color_texture"); 
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("lineal_depth_texture");
	glUniform1i(shader("lineal_depth_texture"), 1);
	shader.addUniform("pinpong_texture");
	glUniform1i(shader("pinpong_texture"), 2); 
	shader.addUniform("cross_bilateral_factor");
	glUniform1i(shader("cross_bilateral_factor"), 3);
	shader.addUniform("curvature_texture");
	glUniform1i(shader("curvature_texture"), 4);
	shader.addUniform("albedo_texture");
	glUniform1i(shader("albedo_texture"), 5);

	shader.unUse();
	checkCritOpenGLError();


	_shaders[Shaders::SSSS_HORIZONTAL_BLUR] = shader;
	std::cout << "\tSSSS horizontal blur shader initialized" << std::endl;
}

void GlslShaderManager::initSSSSVert()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/Ssss_blur_vert.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");

	shader.addUniform("pixel_size");
	shader.addUniform("gaussian");
	shader.addUniform("correction");
	shader.addUniform("sssWidth");
	shader.addUniform("cam_fovy");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("lineal_depth_texture");
	glUniform1i(shader("lineal_depth_texture"), 1);
	shader.addUniform("cross_bilateral_factor");
	glUniform1i(shader("cross_bilateral_factor"), 3); 
	shader.addUniform("curvature_texture");
	glUniform1i(shader("curvature_texture"), 4);
	shader.addUniform("albedo_texture");
	glUniform1i(shader("albedo_texture"), 5);

	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::SSSS_VERTICAL_BLUR] = shader;
	std::cout << "\tSSSS vertical blur shader initialized" << std::endl;
}

void GlslShaderManager::initSeparableSSSSHori()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/separableSsss_blur_horiz.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");

	shader.addUniform("sssWidth");
	shader.addUniform("cam_fovy");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("lineal_depth_texture");
	glUniform1i(shader("lineal_depth_texture"), 1);
	shader.addUniform("cross_bilateral_factor");
	glUniform1i(shader("cross_bilateral_factor"), 2);
	shader.addUniform("curvature_texture");
	glUniform1i(shader("curvature_texture"), 4);
	shader.addUniform("albedo_texture");
	glUniform1i(shader("albedo_texture"), 5);

	shader.addUniform("ssss_n_samples"); 
	shader.addUniform("kernel");
	shader.addUniform("kernel_range");

	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::SEPARABLE_SSSS_HORIZONTAL_BLUR] = shader;
	std::cout << "\tseparable SSSS horizontal blur shader initialized" << std::endl;
}

void GlslShaderManager::initSeparableSSSSVert()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/separableSsss_blur_vert.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");

	shader.addUniform("sssWidth");
	shader.addUniform("cam_fovy");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("lineal_depth_texture");
	glUniform1i(shader("lineal_depth_texture"), 1);
	shader.addUniform("cross_bilateral_factor");
	glUniform1i(shader("cross_bilateral_factor"), 2);
	shader.addUniform("curvature_texture");
	glUniform1i(shader("curvature_texture"), 4);
	shader.addUniform("albedo_texture");
	glUniform1i(shader("albedo_texture"), 5);

	shader.addUniform("ssss_n_samples");
	shader.addUniform("kernel");
	shader.addUniform("kernel_range");

	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::SEPARABLE_SSSS_VERTICAL_BLUR] = shader;
	std::cout << "\tseparable SSSS vertical blur shader initialized" << std::endl;
}


void GlslShaderManager::initToneMap()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/tone_map.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	
	shader.addUniform("method");
	shader.addUniform("exposure");
	shader.addUniform("m_burnout");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	checkCritOpenGLError();

	_shaders[Shaders::TONE_MAP] = shader;
	std::cout << "\tTone map shader initialized" << std::endl;
}

void GlslShaderManager::initSSSSGaussianHori()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/Ssss_gauss_horiz.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");

	shader.addUniform("pixel_size");
	shader.addUniform("gaussian");
	shader.addUniform("correction");
	shader.addUniform("sssWidth");
	shader.addUniform("cam_fovy");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("lineal_depth_texture");
	glUniform1i(shader("lineal_depth_texture"), 1);
	shader.addUniform("pinpong_texture");
	glUniform1i(shader("pinpong_texture"), 2);
	shader.addUniform("cross_bilateral_factor");
	glUniform1i(shader("cross_bilateral_factor"), 3);
	shader.addUniform("curvature_texture");
	glUniform1i(shader("curvature_texture"), 4);
	shader.addUniform("albedo_texture");
	glUniform1i(shader("albedo_texture"), 5);

	shader.addUniform("ssss_n_samples");
	shader.addUniform("kernel");

	shader.unUse();
	checkCritOpenGLError();


	_shaders[Shaders::GAUSSIAN_SSSS_HORIZONTAL_BLUR] = shader;
	std::cout << "\tGaussian SSSS horizontal blur shader initialized" << std::endl;
}

void GlslShaderManager::initSSSSGaussianVert()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/Ssss_gauss_vert.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");

	shader.addUniform("pixel_size");
	shader.addUniform("gaussian");
	shader.addUniform("correction");
	shader.addUniform("sssWidth");
	shader.addUniform("cam_fovy");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("lineal_depth_texture");
	glUniform1i(shader("lineal_depth_texture"), 1);
	shader.addUniform("cross_bilateral_factor");
	glUniform1i(shader("cross_bilateral_factor"), 3);
	shader.addUniform("curvature_texture");
	glUniform1i(shader("curvature_texture"), 4);
	shader.addUniform("albedo_texture");
	glUniform1i(shader("albedo_texture"), 5);

	shader.addUniform("ssss_n_samples");
	shader.addUniform("kernel");

	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::GAUSSIAN_SSSS_VERTICAL_BLUR] = shader;
	std::cout << "\tGaussian SSSS vertical blur shader initialized" << std::endl;
}


void GlslShaderManager::initImageBlurHori()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/image_blur_vert.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("pixel_size");
	shader.addUniform("blur_width");
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::IMAGE_BLUR_SHADER_VERT] = shader;
	std::cout << "\tImage vertical blur shader initialized" << std::endl;
}

void GlslShaderManager::initImageBlurVert()
{
	GlslShader shader;
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/screen_space_quad.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/image_blur_horiz.frag");
	shader.createAndLinkProgram();
	shader.use();
	shader.addAttribute("vVertex");
	shader.addUniform("color_texture");
	glUniform1i(shader("color_texture"), 0);
	shader.addUniform("pixel_size");
	shader.addUniform("blur_width");
	shader.unUse();
	checkCritOpenGLError();

	_shaders[Shaders::IMAGE_BLUR_SHADER_HORI] = shader;
	std::cout << "\tImage vertical blur shader initialized" << std::endl;
}