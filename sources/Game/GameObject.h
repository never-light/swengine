#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>
#include <Engine\Components\Math\Transform.h>

#include <functional>

class GameObject {
public:
	using Id = size_t;

	enum class Usage {
		Dummy, StaticEnvironmentObject, DynamicObject, LightSource, Player
	};

	enum class InteractiveMode {
		None, Usable, Takeable
	};

	enum class Location {
		None, World, Inventory
	};

public:
	using ActionCallback = std::function<void(GameObject*)>;

public:
	GameObject();
	virtual ~GameObject();

	Id getGameObjectId() const;
	void setGameObjectId(Id id);
	void resetGameObjectId();

	Usage getGameObjectUsage() const;
	void setGameObjectUsage(Usage usage);

	bool isLightSource() const;
	bool isPlayer() const;

	Location getGameObjectLocation() const;
	void setGameObjectLocation(Location location);

	bool isLocatedInWorld();

	virtual vector3 getPosition() const = 0;

	void setGameObjectInteractiveMode(InteractiveMode mode);
	InteractiveMode getGameObjectInteractiveMode() const;

	bool isInteractive() const;
	bool isUsable() const;
	bool isTakeable() const;

	void setGameObjectInteractiveTitle(const std::string& title);
	const std::string& getGameObjectInteractiveTitle() const;

	void triggerUse();
	void setUseCallback(const ActionCallback& callback);

	void triggerTake();
	void setTakeCallback(const ActionCallback& callback);
protected:
	Id m_gameObjectId;
	Usage m_gameObjectUsage;

	Location m_gameObjectLocation;
	InteractiveMode m_gameObjectInteractiveMode;
	
	std::string m_gameObjectInteractiveTitle;

protected:
	ActionCallback m_useCallback;
	ActionCallback m_takeCallback;
};