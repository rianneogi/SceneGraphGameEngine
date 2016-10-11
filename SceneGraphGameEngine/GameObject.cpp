#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::input()
{
	for (int i = 0;i < mComponents.size();i++)
	{
		mComponents[i]->input();
	}

	for (int i = 0;i < mChildren.size();i++)
	{
		mChildren[i]->input();
	}
}

void GameObject::render()
{
	for (int i = 0;i < mComponents.size();i++)
	{
		mComponents[i]->render();
	}

	for (int i = 0;i < mChildren.size();i++)
	{
		mChildren[i]->render();
	}
}

void GameObject::update()
{
	for (int i = 0;i < mComponents.size();i++)
	{
		mComponents[i]->update();
	}

	for (int i = 0;i < mChildren.size();i++)
	{
		mChildren[i]->update();
	}
}
