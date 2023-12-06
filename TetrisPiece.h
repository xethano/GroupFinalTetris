#pragma once

#include "common.h"

class TetrisPiece
{
protected:

	std::vector<std::vector<sf::Vector2i>> PieceOffsetData;
	
public:

	TetrisPiece()
	{
		PieceOffsetData.resize(4); // 4 rotations
		for (int r = 0; r < 4; r++)
		{
			PieceOffsetData[r].resize(4); // 4 blocks
		}
	}

	sf::Vector2i GetBlockXYDelta(int Rotation, int BlockIndex)
	{
		return PieceOffsetData[Rotation][BlockIndex];
	}
};

class Piece_Block : public TetrisPiece
{
public:
	Piece_Block()
	{
		PieceOffsetData = 
		{
			{
				{ 0, 0 },
				{ 1, 0 },
				{ 0, 1 },
				{ 1, 1 }
			},

			{
				{ 0, 0 },
				{ 1, 0 },
				{ 0, 1 },
				{ 1, 1 }
			},

			{
				{ 0, 0 },
				{ 1, 0 },
				{ 0, 1 },
				{ 1, 1 }
			},

			{
				{ 0, 0 },
				{ 1, 0 },
				{ 0, 1 },
				{ 1, 1 }
			}
		};
	}
};

class Piece_Straight : public TetrisPiece
{
public:
	Piece_Straight()
	{
		PieceOffsetData =
		{
			{
				{ 0, 0 },
				{ 0, 1 },
				{ 0, 2 },
				{ 0, 3 }
			},

			{
				{ -1, 0 },
				{ 0, 0 },
				{ 1, 0 },
				{ 2, 0 }
			},

			{
				{ 0, 0 },
				{ 0, 1 },
				{ 0, 2 },
				{ 0, 3 }
			},

			{
				{ -1, 0 },
				{ 0, 0 },
				{ 1, 0 },
				{ 2, 0 }
			}
		};
	}
};

class Piece_ZigZagL : public TetrisPiece
{
public:
	Piece_ZigZagL()
	{
		PieceOffsetData =
		{
			{
				{ 0, 0 },
				{ 0, 1 },
				{ -1, 1 },
				{ -1, 2 },
			},

			{
				{ -1, 0 },
				{ 0, 0 },
				{ 0, 1 },
				{ 1, 1 },
			},

			{
				{ 0, 0 },
				{ 0, 1 },
				{ -1, 1 },
				{ -1, 2 },
			},

			{
				{ -1, 0 },
				{ 0, 0 },
				{ 0, 1 },
				{ 1, 1 },
			}
		};
	}
};

class Piece_ZigZagR : public TetrisPiece
{
public:
	Piece_ZigZagR()
	{
		PieceOffsetData =
		{
			{
				{ 0, 0 },
				{ 0, 1 },
				{ 1, 1 },
				{ 1, 2 },
			},

			{
				{ -1, 0 },
				{ 0, 0 },
				{ 0, -1 },
				{ 1, -1 },
			},

			{
				{ 0, 0 },
				{ 0, 1 },
				{ 1, 1 },
				{ 1, 2 },
			},

			{
				{ -1, 0 },
				{ 0, 0 },
				{ 0, -1 },
				{ 1, -1 },
			}
		};
	}
};

class Piece_EllL : public TetrisPiece
{
public:
	Piece_EllL()
	{
		PieceOffsetData =
		{
			{
				{ 0,0 },
				{ -1,0 },
				{ 0,1 },
				{ 0,2 }
			},

			{
				{0,0},
				{ 0, -1 },
				{ -1, 0 },
				{ -2, 0 }
			},

			{
				{0,0},
				{ 0, -1 },
				{ 0, -2 },
				{ 1,0 }
			},

			{
				{0,0},
				{ 0, 1 },
				{ 1, 0 },
				{ 2, 0 }
			}
		};
	}
};

class Piece_EllR : public TetrisPiece
{
public:
	Piece_EllR()
	{
		PieceOffsetData =
		{
			{
				{ 0,0 },
				{ 1,0 },
				{ 0,1 },
				{ 0,2 }
			},

			{
				{ 0,0 },
				{ 0,1 },
				{ -1,0 },
				{ -2,0 }
			},

			{
				{ 0,0 },
				{ 0,-1 },
				{ 0,-2 },
				{ -1,0 }
			},

			{
				{ 0,0 },
				{ 0,-1 },
				{ 1, 0 },
				{ 2, 0 }
			}
		};
	}
};

class Piece_Tee : public TetrisPiece
{
public:
	Piece_Tee()
	{
		PieceOffsetData =
		{
			{
				{ -1, 0 },
				{ 0, 0 },
				{ 1, 0 },
				{ 0, 1 }
			},

			{
				{ 0, 0 },
				{ 0, -1 },
				{ -1, 0 },
				{ 0, 1 }
			},

			{
				{ 0, 0 },
				{ -1, 0 },
				{ 1, 0 },
				{ 0, -1 }
			},

			{
				{ 0, 0 },
				{ 0, -1 },
				{ 1, 0 },
				{ 0, 1 }
			}
		};
	}
};
