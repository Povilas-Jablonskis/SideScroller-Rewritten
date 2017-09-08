#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		pressedkeys = new bool[pressedKeyCount];
		lastpressedkeys = new bool[pressedKeyCount];
		resetInput();
		setCurrentEditedKeyBinding(std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>>(keyBindings.end(), nullptr));
	}

	void InputManager::resetInput()
	{
		setLeftMouseState(false);
		setLastLeftMouseState(false);
		setRightMouseState(false);
		setLastRightMouseState(false);
		for (int i = 0; i < pressedKeyCount; i++)
		{
			setKey(i, false);
		}
	}

	bool InputManager::resetCurrentEditedKeyBinding()
	{
		auto currentKeyBinding = getCurrentEditedKeyBinding();

		if (currentKeyBinding->second == nullptr)
			return false;

		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == currentKeyBinding->first->first)
			{
				currentKeyBinding->second->setIsStatic(false);
				currentKeyBinding->second->changeColor(glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
				currentKeyBinding->second.reset();
				return true;
			}
		}

		return false;
	}

	void InputManager::fixInput()
	{
		for (int i = 0; i < pressedKeyCount; i++)
		{
			if (!GetAsyncKeyState(i) && getKey(i))
				setKey(i, false);
		}
	}

	void InputManager::setKeyBinding(const std::string& key, int value)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return;
		}

		keyBindings.push_back(std::pair<std::string, int>(key, value));
	}

	int InputManager::getKeyBinding(const std::string& key)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return keyBinding.second;
		}
		
		return -1;
	}

	void InputManager::updatePlayerInput(Player* player)
	{
		if (player == nullptr) return;

		if (player->getSecondState() == STATE_IDLE)
		{
			if (getKey(getKeyBinding("Jump")))
			{
				player->setSecondState(STATE_JUMPING);
				player->setVelocity(1, 100.0f);
			}
		}

		auto firstState = player->getFirstState();

		if (player->getSecondState() == STATE_CLIMBING)
		{
			if (getKey(getKeyBinding("Climb")))
				player->setVelocity(1, 20.0f);
			else if (getKey(getKeyBinding("Duck")))
				player->setVelocity(1, -20.0f);
			else
				player->setVelocity(1, 0.0f);
		}
		else
		{
			if (!player->getIsDucking() && getKey(getKeyBinding("Duck")))
			{
				player->applyAnimation(player->getAnimationByIndex("duck"));
				player->setIsDucking(true);
			}
			else if (player->getIsDucking() && !getKey(getKeyBinding("Duck")))
			{
				if (player->getFirstState() == STATE_WALKINGLEFT || player->getFirstState() == STATE_WALKINGRIGHT)
					player->applyAnimation(player->getAnimationByIndex("walk"));
				else if (player->getFirstState() == STATE_IDLE)
				{
					if (player->getSecondState() == STATE_FALLING || player->getSecondState() == STATE_JUMPING || player->getSecondState() == STATE_CLIMBING)
						player->applyAnimation(player->getAnimationByIndex("jump"));
					else if (player->getSecondState() == STATE_IDLE)
						player->applyAnimation(player->getAnimationByIndex("stand"));
				}
				player->setIsDucking(false);
			}
		}

		switch (firstState)
		{
			case STATE_IDLE:
			{
				if (getKey(getKeyBinding("Move Left")))
				{
					player->setFirstState(STATE_WALKINGLEFT);
					player->setVelocity(0, -20.0f);
				}
				else if (getKey(getKeyBinding("Move Right")))
				{
					player->setFirstState(STATE_WALKINGRIGHT);
					player->setVelocity(0, 20.0f);
				}
				break;
			}
			case STATE_WALKINGRIGHT:
			{
				if (!getKey(getKeyBinding("Move Right")))
				{
					if (getKey(getKeyBinding("Move Left")))
					{
						player->setFirstState(STATE_WALKINGLEFT);
						player->setVelocity(0, -20.0f);
					}
					else
					{
						player->setFirstState(STATE_IDLE);
						player->setVelocity(0, 0.0f);
					}
				}
				break;
			}
			case STATE_WALKINGLEFT:
			{
				if (!getKey(getKeyBinding("Move Left")))
				{
					if (getKey(getKeyBinding("Move Right")))
					{
						player->setFirstState(STATE_WALKINGRIGHT);
						player->setVelocity(0, 20.0f);
					}
					else
					{
						player->setFirstState(STATE_IDLE);
						player->setVelocity(0, 0.0f);
					}
				}
				break;
			}
		}
	}

	void InputManager::simulateInputForEntity(Entity* entity, int key, bool keyPressed)
	{
		if (entity == nullptr) return;

		if (entity->getSecondState() == STATE_IDLE)
		{
			if (getKeyBinding("Jump") == key && keyPressed)
			{
				entity->setSecondState(STATE_JUMPING);
				entity->setVelocity(1, 100.0f);
			}
		}

		auto firstState = entity->getFirstState();

		if (entity->getSecondState() == STATE_CLIMBING)
		{
			if (getKeyBinding("Climb") == key && keyPressed)
				entity->setVelocity(1, 20.0f);
			else if (getKeyBinding("Duck") == key && keyPressed)
				entity->setVelocity(1, -20.0f);
			else
				entity->setVelocity(1, 0.0f);
		}
		else
		{
			if (!entity->getIsDucking() && getKeyBinding("Duck") == key && keyPressed)
			{
				entity->applyAnimation(entity->getAnimationByIndex("duck"));
				entity->setIsDucking(true);
			}
			else if (entity->getIsDucking() && getKeyBinding("Duck") == key && !keyPressed)
			{
				if (entity->getFirstState() == STATE_WALKINGLEFT || entity->getFirstState() == STATE_WALKINGRIGHT)
					entity->applyAnimation(entity->getAnimationByIndex("walk"));
				else if (entity->getFirstState() == STATE_IDLE)
				{
					if (entity->getSecondState() == STATE_FALLING || entity->getSecondState() == STATE_JUMPING || entity->getSecondState() == STATE_CLIMBING)
						entity->applyAnimation(entity->getAnimationByIndex("jump"));
					else if (entity->getSecondState() == STATE_IDLE)
						entity->applyAnimation(entity->getAnimationByIndex("stand"));
				}
				entity->setIsDucking(false);
			}
		}

		switch (firstState)
		{
			case STATE_IDLE:
			{
				if (getKeyBinding("Move Left") == key && keyPressed)
				{
					entity->setFirstState(STATE_WALKINGLEFT);
					entity->setVelocity(0, -20.0f);
				}
				else if (getKeyBinding("Move Right") == key && keyPressed)
				{
					entity->setFirstState(STATE_WALKINGRIGHT);
					entity->setVelocity(0, 20.0f);
				}
				break;
			}
			case STATE_WALKINGRIGHT:
			{
				if (getKeyBinding("Move Right") == key && !keyPressed)
				{
					if (getKeyBinding("Move Left") == key && keyPressed)
					{
						entity->setFirstState(STATE_WALKINGLEFT);
						entity->setVelocity(0, -20.0f);
					}
					else
					{
						entity->setFirstState(STATE_IDLE);
						entity->setVelocity(0, 0.0f);
					}
				}
				break;
			}
			case STATE_WALKINGLEFT:
			{
				if (getKeyBinding("Move Left") == key && !keyPressed)
				{
					if (getKeyBinding("Move Right") == key && keyPressed)
					{
						entity->setFirstState(STATE_WALKINGRIGHT);
						entity->setVelocity(0, 20.0f);
					}
					else
					{
						entity->setFirstState(STATE_IDLE);
						entity->setVelocity(0, 0.0f);
					}
				}
				break;
			}
		}
	}
}