#include "sceneManager.h"

void renderScene(GameStateMessage* gameState)
{
    std::vector<sMessageTank*> tankz = gameState->tanks;
	std::vector<sMessageBullet*> bulletz = gameState->bullets;

    for (int i = 0; i < tankz.size(); i++)
    {
        sMessageTank* smT = tankz[i];
        cGameObject* tempTank = new cGameObject(::g_map_GameObjects["tank_template"]);
        // TODO:
        //     int fireCooldown = 0;
        //     int isAlive = 1;
        tempTank->inverseMass = 1.0f;
        tempTank->friendlyName = smT->name;
        tempTank->velocity = glm::vec3(smT->xV,smT->yV,smT->zV);
		tempTank->isVisible = smT->isAlive;
        tempTank->positionXYZ = glm::vec3(smT->xP,smT->yP,smT->zP);
        tempTank->setOrientation(glm::vec3(-90,0,smT->degrees));
        ::g_map_GameObjects[tempTank->friendlyName] = tempTank;
    }

    for (int i = 0; i < bulletz.size(); i++)
    {
        sMessageBullet* smB = bulletz[i];
        cGameObject* tempBullet = new cGameObject(::g_map_GameObjects["sphere"]);
        // TODO:
        //     int fireCooldown = 0;
        //     int isAlive = 1;
        tempBullet->inverseMass = 1.0f;
        tempBullet->lifetime = smB->lifetime;
        tempBullet->tag = "lifetime";
        tempBullet->friendlyName = smB->name;
		tempBullet->isVisible = smB->lifetime != 0;
        tempBullet->velocity = glm::vec3(smB->xV,smB->yV,smB->zV);
        tempBullet->positionXYZ = glm::vec3(smB->xP,smB->yP,smB->zP);
        ::g_map_GameObjects[tempBullet->friendlyName] = tempBullet;
    }
}