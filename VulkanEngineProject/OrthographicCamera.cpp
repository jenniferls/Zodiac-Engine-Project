#include <Zodiacpch.h>
#include "OrthographicCamera.h"

Zodiac::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) {
	m_rotation = 0.0f;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_projMat = glm::ortho<float>(left, right, bottom, top, -1.0f, 1.0f);
	m_viewMat = glm::lookAt(m_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_modelMat = glm::mat4(1.0f);
}

Zodiac::OrthographicCamera::~OrthographicCamera() {

}

void Zodiac::OrthographicCamera::setPosition(const glm::vec3& pos) {
	m_position = pos;
	RecalculateViewMatrix();
}

void Zodiac::OrthographicCamera::setRotation(const float rotation) {
	m_rotation = rotation;
	RecalculateViewMatrix();
}

const glm::vec3& Zodiac::OrthographicCamera::getPosition() const {
	return m_position;
}

const float Zodiac::OrthographicCamera::getRotation() const {
	return m_rotation;
}

const glm::mat4& Zodiac::OrthographicCamera::getProjectionMatrix() const {
	return m_projMat;
}

const glm::mat4& Zodiac::OrthographicCamera::getViewMatrix() const {
	return m_viewMat;
}

const glm::mat4& Zodiac::OrthographicCamera::getModelMatrix() const {
	return m_modelMat;
}

void Zodiac::OrthographicCamera::RecalculateViewMatrix() {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	m_viewMat = glm::inverse(transform);
}
