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
        tempTank->friendlyName = smT->name;
        tempTank->velocity = glm::vec3(smT->xV,smT->yV,smT->zV);
        tempTank->positionXYZ = glm::vec3(smT->xP,smT->yP,smT->zP);
        tempTank->updateOrientation(glm::vec3(0,smT->degrees,0));
        ::g_map_GameObjects.insert({tempTank->friendlyName,tempTank});
    }

    for (int i = 0; i < bulletz.size(); i++)
    {
        sMessageBullet* smB = bulletz[i];
        cGameObject* tempBullet = new cGameObject(::g_map_GameObjects["sphere"]);
        // TODO:
        //     int fireCooldown = 0;
        //     int isAlive = 1;
        tempBullet->friendlyName = smB->name;
        tempBullet->velocity = glm::vec3(smB->xV,smB->yV,smB->zV);
        tempBullet->positionXYZ = glm::vec3(smB->xP,smB->yP,smB->zP);
        ::g_map_GameObjects.insert({tempBullet->friendlyName,tempBullet});
    }
}