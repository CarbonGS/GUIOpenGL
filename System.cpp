#include "System.h"

System::System() : mesh(nullptr), vel{}, accel{}, mass(1.0f), pos{}
{
}

System::~System()
{
	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}
}

void System::UpdatePosition(const float dt)
{
	pos += vel * dt + 0.5f * accel * dt * dt;
}

void System::UpdateVelocity(const float dt)
{
	vel += accel * dt;
}

void System::ApplyForce(const glm::vec3& force)
{
	accel = force / mass; // F = ma => a = F/m
}

glm::vec3 System::CalculateForce(const float dt, const bool throttle)
{
    float speed = glm::length(vel);
    glm::vec3 u = glm::vec3(0.0f, 0.0f, 1.0f); // forward direction

	float rpm = CalculateRPM();
	float T_Eng = throttle ? throttle * GetTorqueAtRPM(rpm) : 0.0f; 

	if (rpm > maxRPM) {
		T_Eng = 0.0f; // Prevent excessive RPM
	}

	float gearRatio = gearRatios[gear];
	float T_Wheel = T_Eng * gearRatio * finalDriveRatio * efficiency;

	glm::vec3 traction = glm::vec3(0.0f);

    if (throttle)
    {
        traction = u * (T_Wheel / wheelRadius);
    }

    float cDrag = 0.5f * dragCoef * frontMeters * 1.29f;
    glm::vec3 dragForce = -cDrag * vel * speed;

    float C_rr = rollingCoef * cDrag;
    glm::vec3 rollingRes = -C_rr * vel;

    return traction + dragForce + rollingRes;
}

float System::CalculateRPM() const
{
	float wheelAngVel = glm::length(vel) / wheelRadius;
	float rpm = wheelAngVel * gearRatios[gear] * finalDriveRatio * 60.0f / (2.0f * glm::pi<float>());
	return glm::max(rpm, idleRPM);
}

float System::GetTorqueAtRPM(float rpm) const
{
	auto it = torqueCurve.lower_bound(rpm);
	if (it == torqueCurve.begin()) return it->second;
	if (it == torqueCurve.end()) return std::prev(it)->second;

	auto hi = it;
	auto lo = std::prev(it);

	float t = (rpm - lo->first) / (hi->first - lo->first);
	return lo->second + t * (hi->second - lo->second);
}


const glm::mat4 System::getModelMatrix() const
{
	glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 R = glm::mat4(1.0f);
	glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	return T * R * S;
}
