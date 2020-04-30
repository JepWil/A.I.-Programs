#pragma once
#include <vector>
#include <memory>
#include "Grid.h"

struct AStarNode
{	
	int mH;
	int mG;
	int mF;
	std::shared_ptr<Cell> mCell;
	std::shared_ptr<AStarNode> mParent;

	AStarNode()
	{
		mH = 0;
		mG = 0;
		mF = 0;
		mParent = nullptr;
		mCell = nullptr;
	}

	int CalcFCost()
	{
		return mG + mH;
	}
};

class AStarPath
{
public:
	AStarPath();
	~AStarPath();
	bool AStarPath::CheckPtrObj(std::shared_ptr<AStarNode> arg_1, std::shared_ptr<AStarNode> arg_2);
	void AStarPath::DrawAStarPath();
	std::shared_ptr<std::vector<std::shared_ptr<AStarNode>>> CreateAStarPath(std::shared_ptr<Grid> arg_grid, int arg_startX, int arg_startY, int arg_endX, int arg_endY, int arg_cell_size);
	std::vector<std::shared_ptr<AStarNode>> mAStarNodes;
	std::vector<std::shared_ptr<AStarNode>> mAStarParentNodes;
	std::vector<std::shared_ptr<AStarNode>> mClosed;	
private:
	std::vector<std::shared_ptr<AStarNode>> mOpen;
	const int CELL_SIZE = 20;
};