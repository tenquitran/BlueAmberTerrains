#pragma once


namespace BlueAmberTerrainsApp
{
	class Scene
	{
	public:
		Scene(GLfloat terrainScaleFactor, HDC hDC, int clientWidth, int clientHeight, const CAtlString& heighmapFilePath);

		virtual ~Scene();

#if 0
		bool initialize(int clientWidth, int clientHeight);
#endif

		void render() const;

		void resize(int clientWidth, int clientHeight);

		void translateCameraX(GLfloat diff);
		void translateCameraY(GLfloat diff);
		void translateCameraZ(GLfloat diff);

		void rotateCameraX(GLfloat angle);
		void rotateCameraY(GLfloat angle);
		void rotateCameraZ(GLfloat angle);

		void scaleCamera(GLfloat amount);

	private:
		// Field of view angle.
		const GLfloat FieldOfView = 45.0f;

		// Frustum boundaries.
		const GLfloat FrustumNear = 0.1f;
		const GLfloat FrustumFar = 1000.0f;

		// Device context of the main window.
		HDC m_hDC;

		// Width and height of the main window's client area.
		int m_clientWidth;
		int m_clientHeight;

		std::unique_ptr<Camera> m_spCamera;

		Terrain m_terrain;
	};
}
