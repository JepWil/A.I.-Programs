#include "AStarPath.h"
#include <iostream>
AStarPath::AStarPath()
{}

AStarPath::~AStarPath()
{}

bool AStarPath::CheckPtrObj(std::shared_ptr<AStarNode> arg_1, std::shared_ptr<AStarNode> arg_2)
{
	if (arg_1->mCell->mCellX == arg_2->mCell->mCellX && arg_1->mCell->mCellY == arg_2->mCell->mCellY)
		return true;
	else
		return false;
}

std::shared_ptr<std::vector<std::shared_ptr<AStarNode>>> AStarPath::CreateAStarPath(std::shared_ptr<Grid> arg_grid, int arg_startX, int arg_startY, int arg_endX, int arg_endY, int arg_cell_size)
{
	mOpen.clear();
	mClosed.clear();
	mAStarNodes.clear();
	mAStarParentNodes.clear();

	AStarNode startNode;
	AStarNode endNode;
	auto startPtr = std::make_shared<AStarNode>(startNode);
	auto endPtr = std::make_shared<AStarNode>(endNode);
	startPtr->mCell = arg_grid->GetCellAt(arg_startX, arg_startY);
	endPtr->mCell = arg_grid->GetCellAt(arg_endX, arg_endY);

	mOpen.push_back(startPtr); // Add Start node to Open list

	while (mOpen.size() > 0) // While there are Nodes to check...
	{
		std::shared_ptr<AStarNode> curNode = mOpen[0];
		int index = 0;

		for (size_t i = 0; i < mOpen.size(); i++) // Look for lowest F cost
		{
			if (mOpen[i]->CalcFCost() < curNode->CalcFCost() || (mOpen[i]->CalcFCost() == curNode->CalcFCost() && mOpen[i]->mH < curNode->mH)) // If lower F cost: Node = Current Node, or if equal, check H cost
			{
				curNode = mOpen[i]; 
				index = i;
			}
		}

		mClosed.push_back(curNode); // Send the Node to the Closed list
		mOpen.erase(mOpen.begin() + index);

		if (CheckPtrObj(curNode, endPtr)) // Target reached, add all Parent Nodes to generate path
		{
			std::cout << "Path found!" << std::endl;
			mAStarParentNodes.push_back(curNode);

			std::shared_ptr<AStarNode> parentTracer = curNode; // Start from Target and trace backwards
			mAStarParentNodes.push_back(parentTracer->mParent);
			while (!CheckPtrObj(parentTracer, startPtr))
			{
				mAStarParentNodes.push_back(parentTracer->mParent);
				parentTracer = parentTracer->mParent;
			}
			std::reverse(mAStarParentNodes.begin(), mAStarParentNodes.end()); // Reverse vector to get Start->Target
			break;
		}

		mAStarNodes.clear();
		for (std::shared_ptr<Cell> cell : arg_grid->GetNeighbourCells(curNode->mCell, arg_grid)) // Add AStarNodes to Neighbouring Cells of the Current Node
		{
			AStarNode neighbourNode;
			neighbourNode.mCell = cell;
			neighbourNode.mParent = curNode;
			mAStarNodes.push_back(std::make_shared<AStarNode>(neighbourNode));
		}

		for (std::shared_ptr<AStarNode> neighbourNode : mAStarNodes)
		{				
			bool mInOpen = false;
			bool mInClosed = false;

			for (std::shared_ptr<AStarNode> closedNode : mClosed)
			{
				if (CheckPtrObj(neighbourNode, closedNode))
				{
					mInClosed = true;
				}
			}

			if (mInClosed || neighbourNode->mCell->mCellAccess != cellaccess::FREE)
			{
				continue;
			}

			for (std::shared_ptr<AStarNode> openNode : mOpen)
			{
				if (CheckPtrObj(neighbourNode, openNode))
				{
					mInOpen = true;
				}
			}

			if (mInOpen)
			{
				continue;
			}

			else if (!mInOpen)
			{
				mOpen.push_back(neighbourNode);
				neighbourNode->mG = arg_grid->GetDistance(curNode->mCell, neighbourNode->mCell);
				neighbourNode->mH = arg_grid->GetDistance(neighbourNode->mCell, endPtr->mCell);
			}
			
			for (size_t i = 0; i < mOpen.size(); i++)
			{
				std::shared_ptr<AStarNode> tempOpenNode = mOpen[i];
				if (CheckPtrObj(neighbourNode, tempOpenNode)) // On open list, set Parent and Costs
				{
					int movementCost = curNode->mG + arg_grid->GetDistance(curNode->mCell, neighbourNode->mCell); // Manhattan / Euclidean depending on selection cost	
					if (movementCost < neighbourNode->mG)
					{
						neighbourNode->mParent = curNode;
						neighbourNode->mG = movementCost;
						neighbourNode->mH = arg_grid->GetDistance(neighbourNode->mCell, endPtr->mCell);
					}
					tempOpenNode = nullptr;
				}
			}
		}
	}

	if (mAStarParentNodes.size() <= 0)
	{
		std::cout << "No path was found!" << std::endl;
		return std::make_shared<std::vector<std::shared_ptr<AStarNode>>>(mAStarParentNodes);
	}
	else
	{
		return std::make_shared<std::vector<std::shared_ptr<AStarNode>>>(mAStarParentNodes);
	}
}

void AStarPath::DrawAStarPath()
{
	for (int i = 1; i < (int)mAStarParentNodes.size(); i++)
	{
		double halfside = CELL_SIZE / 2;
		glPushMatrix();
		glTranslatef(mAStarParentNodes[i]->mCell->mCellX, mAStarParentNodes[i]->mCell->mCellY + 1, 0);
		glColor3f(0, 0, 0);
		//bot
		glBegin(GL_LINES);
		glVertex3f(-halfside, halfside, 0.0);
		glVertex3f(halfside, halfside, 0.0);
		glEnd();
		//top
		glBegin(GL_LINES);
		glVertex3f(-halfside, -halfside, 0.0);
		glVertex3f(halfside, -halfside, 0.0);
		glEnd();
		//left
		glBegin(GL_LINES);
		glVertex3f(-halfside, -halfside, 0.0);
		glVertex3f(-halfside, halfside, 0.0);
		glEnd();
		//right
		glBegin(GL_LINES);
		glVertex3f(halfside, -halfside, 0.0);
		glVertex3f(halfside, halfside, 0.0);
		glEnd();
		// color selection
		glColor4f(0.75, 0.9f, 0.0f, 1.0f); // Yellow Path
		// draw quad
		glBegin(GL_QUADS);
		glVertex2d(halfside, halfside);
		glVertex2d(halfside, -halfside);
		glVertex2d(-halfside, -halfside);
		glVertex2d(-halfside, halfside);
		glEnd();
		glPopMatrix();
	}
}

