#include "AppClass.h"
#include <SFML\Audio.hpp>

using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 5.0f, 0.0f), //Position
		vector3(0.0f, 5.0f, 5.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	// Create a floor under the player
	matrix4 m4Scale = glm::translate(IDENTITY_M4, vector3(0, -2.1f, 0)) * ToMatrix4(glm::angleAxis(-90.0f, AXIS_X)) * glm::scale(vector3(1000.0f, 1000.0f, 1.0f));
	m_pMeshMngr->AddPlaneToRenderList(m4Scale, m_v3FloorColor);

#ifdef DEBUG
	uint uInstances = 400;
#else
	uint uInstances = 9000;
#endif
	// Generate Stars in a random half sphere around the player
	for (uint i = 0; i < uInstances; i++)
	{
		m_pEntityMngr->AddEntity("Planets\\00_Sun.obj", "Star_" + std::to_string(i));

		// Generate a random sphere position
		vector3 v3Position = vector3(glm::sphericalRand(100.0f));

		// If the sphere point is under the floor move it above the floor by an equal distance
		if (v3Position.y < 0.0f) 
		{
			v3Position.y *= -1;
		}

		// If the sphere point is lower than a minimum amount relocate them further upwards
		if (v3Position.y < 10.0f)
		{
			v3Position.y = 10.0f;
		}

		// Move the star to the calculated position
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position);
		//m_pEntityMngr->UsePhysicsSolver();
		//m_pEntityMngr->SetMass(i+1);
		float grav = -0.05f;
		m_pEntityMngr->SetGravity(grav,i+1);
	}

	// Create them main timer clock
	m_uMainClock = m_pSystem->GenClock();

	// Initial set up for octree
	m_uOctantLevels = 3; // Found to be the ideal octant depth level
	m_pEntityMngr->GenerateOctants(m_uOctantLevels);
	m_pEntityMngr->UpdateDimensionSetAll();

	// Remove any stars that were generated as colliding at start
	m_pEntityMngr->RemoveCollidingObjects();

	// initialize bg music
	//sf::Music bgMusic;
	//if (!bgMusic.openFromFile(""))
	//{
	//	return; // an error has occured
	//}
	//bgMusic.play();

	// Initial update entity manager call
	m_pEntityMngr->Update();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Reconstructing the Octree each tenth of a second
	static uint nClock = m_pSystem->GenClock();
	static bool bStarted = false;
	if (m_pSystem->IsTimerDone(nClock) || !bStarted)
	{
		bStarted = true;
		m_pSystem->StartTimerOnClock(0.05, nClock);

		// Clear current octant associations, regenerate octants, and then regenerate octant associations
		m_pEntityMngr->UpdateOctantsAndDimensions(m_uOctantLevels);
	}

	// Remove Old Bullets
	m_pEntityMngr->RemoveOldBullets(m_pSystem->GetTimeSinceStart(m_uMainClock));

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
	//m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Display Current Octree
	m_pEntityMngr->DisplayOctree(m_pMeshMngr, m_uOctantID);

	// Create a floor under the player
	matrix4 m4Transform = glm::translate(IDENTITY_M4, vector3(0, -2.1f, 0)) * ToMatrix4(glm::angleAxis(-90.0f, AXIS_X)) * glm::scale(vector3(1000.0f, 1000.0f, 1.0f));
	m_pMeshMngr->AddPlaneToRenderList(m4Transform, m_v3FloorColor);

	// Render a gun for the character
	matrix4 m4Position = glm::translate(IDENTITY_M4, (m_pCameraMngr->GetPosition() + m_pCameraMngr->GetForward() + (m_pCameraMngr->GetRightward() * 0.7f) - (m_pCameraMngr->GetUpward() * 0.5f)));
	matrix4 m4InitialRotation = ToMatrix4(glm::angleAxis(90.0f, AXIS_X));
	matrix4 m4CameraRotation = ToMatrix4(glm::angleAxis(m_fCameraYaw, AXIS_Z)) * ToMatrix4(glm::angleAxis(m_fCameraPitch, AXIS_X));
	matrix4 m4Scale = glm::scale(vector3(0.5f, 8.0f, 0.5f));
	m4Transform = m4Position * m4InitialRotation * m4CameraRotation * m4Scale;
	m_pMeshMngr->AddTubeToRenderList(m4Transform, m_v3GunColor, RENDER_SOLID);

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList("Skybox_04.png");

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui,
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();

	//release GUI
	ShutdownGUI();
}
void Application::ShootBullet(void)
{
	//Create a new Bullet Entity
	m_pEntityMngr->AddEntity("Sorted\\PolyOut.obj", "Bullet_" + std::to_string(bulletCount));
	m_pEntityMngr->GetEntity()->GetEntity("Bullet_" + std::to_string(bulletCount))->m_fTimeWhenShot = m_pSystem->GetTimeSinceStart(m_uMainClock);

	//Move the bullet to the camera's current position
	vector3 v3Position = m_pCameraMngr->GetPosition() + m_pCameraMngr->GetForward() + (m_pCameraMngr->GetRightward() * 0.7f) - (m_pCameraMngr->GetUpward());
	matrix4 m4Position = glm::translate(IDENTITY_M4, v3Position);
	//Move and Rotate the Bullet to match the camera(Rotation does not work with solver)
	//matrix4 m4Position = glm::translate(IDENTITY_M4, v3Position) * ToMatrix4(glm::angleAxis(90.0f, glm::normalize(m_pCameraMngr->GetForward())));
	m_pEntityMngr->SetModelMatrix(m4Position);

	//Have the Bullet use a physics solver
	m_pEntityMngr->UsePhysicsSolver();
	m_pEntityMngr->SetMass(1);
	m_pEntityMngr->SetFriction(0.01f);

	//Have the bullet shoot out away from the player
	vector3 force = glm::normalize(m_pCameraMngr->GetForward()) * 10.0f;
	m_pEntityMngr->GetEntity()->GetEntity("Bullet_" + std::to_string(bulletCount))->GetSolver()->ApplyForce(force);

	bulletCount++;

	// load bullet sound effect from sound buffer
	// sf::Sound allows for layering of sounds! sf::Music does not
	/*sf::SoundBuffer bBuffer;
	if (!bBuffer.loadFromFile(""));
	{
		return;
	}

	sf::Sound bSFX;
	bSFX.setBuffer(bBuffer);*/
	//bSFX.play();
}