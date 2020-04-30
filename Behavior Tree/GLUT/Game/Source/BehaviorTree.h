#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "Utilities.h"

class Blackboard
{
public:
	void Add(std::string key, bool value)
	{
		bBlackboard[key] = value;
	}

	void Add(std::string key, int value)
	{
		iBlackboard[key] = value;
	}

	void Add(std::string key, float value)
	{
		fBlackboard[key] = value;
	}

	void Add(std::string key, vec2 value)
	{
		v2Blackboard[key] = value;
	}

	void Add(int key, int value)
	{
		AttackBlackboard[key] = value;
	}

	bool Getbool(std::string key)
	{
		return bBlackboard[key];
	}

	int Getint(std::string key)
	{
		return iBlackboard[key];
	}

	float Getfloat(std::string key)
	{
		return fBlackboard[key];
	}

	vec2 Getvec2(std::string key)
	{
		return v2Blackboard[key];
	}

	int GetAttackBBoard(int key)
	{
		return AttackBlackboard[key];
	}

	int DamagingMonsters = 0;

protected:
	std::unordered_map<std::string, bool> bBlackboard;
	std::unordered_map<std::string, int> iBlackboard;
	std::unordered_map<std::string, float> fBlackboard;
	std::unordered_map<std::string, vec2> v2Blackboard;
	std::unordered_map<int, int> AttackBlackboard;
};


class Node
{
public:
	enum Status
	{
		SUCCESS,
		FAIL,
		RUNNING,
	};

	virtual Status Update() = 0;
};

class Composite : public Node
{
public:
	void AddChild(std::shared_ptr<Node> p_xNode)
	{
		mChildren.push_back(p_xNode);
	}

protected:
	std::vector<std::shared_ptr<Node>> mChildren;
};

class Decorator : public Node
{
public:
	void SetChild(std::shared_ptr<Node> p_xNode)
	{
		mxChild = p_xNode;
	}

protected:
	std::shared_ptr<Node> mxChild;
};

class Leaf : public Node
{
public:
	Leaf() {}
	virtual ~Leaf() {}
	Leaf(std::shared_ptr<Blackboard> p_Blackboard) : mxBlackboard(p_Blackboard) {}

	virtual Status Update() = 0;

protected:
	std::shared_ptr<Blackboard> mxBlackboard;

};

class Sequence : public Composite
{
public:
	Status Update()
	{
		for (size_t i = 0; i < mChildren.size(); i++)
		{
			auto status = mChildren[i]->Update();
			if (status != Status::SUCCESS)
				return status;
		}
		return Status::SUCCESS;
	}
};

class Selector : public Composite
{
public:
	Status Update()
	{
		for (size_t i = 0; i < mChildren.size(); i++)
		{
			auto status = mChildren[i]->Update();
			if (status != Status::FAIL)
				return status;
		}
		return Status::FAIL;
	}
};

class Inverter : public Decorator
{
public:
	Status Update() override
	{
		auto s = mxChild->Update();

		if (s == Status::SUCCESS) {
			return Status::FAIL;
		}
		if (s == Status::FAIL) {
			return Status::SUCCESS;
		}

		return s;
	}
};

class BehaviorTree
{
public:
	BehaviorTree() : mxBlackboard(std::make_shared<Blackboard>()) {}
	BehaviorTree(const std::shared_ptr<Node> &rootNode) : BehaviorTree() { mxRoot = rootNode; }
	Node::Status update() { return mxRoot->Update(); }
	void setRoot(const std::shared_ptr<Node> &node) { mxRoot = node; }
	std::shared_ptr<Blackboard> getBlackboard() const { return mxBlackboard; }
private:
	std::shared_ptr<Node> mxRoot = nullptr;
	std::shared_ptr<Blackboard> mxBlackboard = nullptr;
};
