#pragma once


namespace BlueAmberTerrainsApp
{
	class Camera
	{
	public:
		// Parameters: aspectRatio - aspect ratio of the main application window;
		//             scaleFactor - initial scale factor for the camera;
		//             fieldOfView - field of view angle;
		//             frustumNear - near frustum boundary;
		//             frustumFar  - far frustum boundary.
		Camera(GLfloat aspectRatio, GLfloat scaleFactor = 1.0f,
			GLfloat fieldOfView = 45.0f, GLfloat frustumNear = 0.1f, GLfloat frustumFar = 1000.0f);

		virtual ~Camera();

		glm::mat4 getProjectionMatrix() const;

		glm::mat4 getModelMatrix() const;

		glm::mat4 getViewMatrix() const;

		glm::mat4 getModelViewMatrix() const;

		glm::mat4 getModelViewProjectionMatrix() const;

		// React on the resizing of the main application window.
		// Parameters: aspectRatio - aspect ratio of the main application window.
		void resize(GLfloat aspectRatio);

		void translateX(GLfloat diff);
		void translateY(GLfloat diff);
		void translateZ(GLfloat diff);

		void rotateX(GLfloat angleDegrees);
		void rotateY(GLfloat angleDegrees);
		void rotateZ(GLfloat angleDegrees);

		void rotateXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees);

		GLfloat getScale() const;

		void scale(GLfloat amount);

	private:
		Camera(const Camera&);
		Camera& operator=(const Camera&);

		// Calculate and update the Model-View-Projection matrix.
		void updateMVP();

	private:
		// Aspect ratio of the main application window.
		GLfloat m_aspectRatio;

		// Initial scale factor for the camera.
		const GLfloat SCALE_FACTOR_INITIAL;

		// Minimum scale factor value.
		const GLfloat SCALE_FACTOR_MINIMUM;

		// Variable (user-defined) scale factor for the camera.
		GLfloat m_scaleFactorVariable;

		// Field of view angle.
		const GLfloat FIELD_OF_VIEW;

		// Near frustum boundary.
		const GLfloat FRUSTUM_NEAR;

		// Far frustum boundary.
		const GLfloat FRUSTUM_FAR;

		// Translation vector of the camera.
		glm::vec3 m_translation;

		// Rotation vector of the camera (in degrees).
		glm::vec3 m_rotationDegrees;

		// Model matrix.
		glm::mat4 m_model;

		// View matrix.
		glm::mat4 m_view;

		// Projection matrix.
		glm::mat4 m_projection;

		// Model-View (MV) matrix.
		glm::mat4 m_mv;

		// Model-View-Projection (MVP) matrix.
		glm::mat4 m_mvp;
	};
}
