#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include "Texture.h"
#include "Mesh.h"

class System {
private:
	Mesh* mesh;

public:
	System();
	~System();

	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 accel;
	float frontMeters;
	float mass;
	float dragCoef;
	float rollingCoef;

	// Gearing
	float gearRatios[6] = { 3.262f, 2.188f, 1.541f, 1.213f, 1.000f, 0.767f };
	float finalDriveRatio = 4.100f;
	int gear = 0;
	float efficiency = 0.86f; // Efficiency of the drivetrain
	float idleRPM = 1000.0f;
	std::map<float, float> torqueCurve;
	float maxRPM = 7500.0f;

	// Wheels
	float sideWall;
	float rimRadius;
	float wheelRadius;

	void UpdatePosition(const float dt);
	void UpdateVelocity(const float dt);
	void ApplyForce(const glm::vec3& force);
	glm::vec3 CalculateForce(const float dt, const bool throttle);
	float CalculateRPM() const;
	float GetTorqueAtRPM(float rpm) const;

	const glm::mat4 getModelMatrix() const;
};
