#ifndef _uIElementH
#define _uIElementH

#include <iostream>
#include <vector>
#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <memory>

#include "Text.h"

namespace Engine
{
	class UIElement : public UIElementBase
	{
		public:
			~UIElement();
			UIElement(float, float, glm::vec2, glm::vec4, std::shared_ptr<UIElement>, glm::vec2);
			inline void addText(std::shared_ptr<Text> text) { texts.push_back(text); }
			inline void addUIElement(std::shared_ptr<UIElement> uiElement) { elements.push_back(uiElement); }
			void update(float);
			void hideMain(bool = true);
			void showMain(bool = true);
			void fixPosition(UIElementBase* = nullptr);
			inline const std::vector<std::shared_ptr<UIElement>>& getElements() { return elements; }
			inline const std::vector<std::shared_ptr<Text>>& getTexts() { return texts; }
			inline void setParent(std::shared_ptr<UIElement> _parent) { parentMenu = _parent; }
			inline std::shared_ptr<UIElement> getParent() const { return parentMenu; }
			void checkOnHover(glm::vec2);
			void checkOnMouseClick(glm::vec2);
			void checkOnMouseRelease();
			void GetAllChildrenElements(std::vector<std::shared_ptr<UIElement>>&);
			void GetAllChildrenTexts(std::vector<std::shared_ptr<Text>>&);
		private:
			std::shared_ptr<UIElement> parentMenu;
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
	};
}
#endif