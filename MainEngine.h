#pragma once

#include <vector>
#include "TetrisPiece.h"

class MainEngine
{
public:

	// data ==============

	int Score;
	std::vector<sf::Sprite*> Sprites;
	std::vector<sf::Texture*> Textures;
	sf::RenderWindow* window;
	std::vector<std::vector<int>> TetrisGrid;
	const int GridWidth = 30; 
	const int GridHeight = 40;

	const int Tile_Empty = 0; // draw a black tile
	const int Tile_Cyan = 1;

	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool upTriggered;
	bool downTriggered;
	bool leftTriggered;
	bool rightTriggered;

	bool RightRotationPressed;
	bool RightRotationTriggered;
	bool LeftRotationPressed;
	bool LeftRotationTriggered;

	bool endGame = false;

	// piece storage
	// Tetris Pieces vector, stores TetrisPiece *'s
	std::vector<TetrisPiece> TetrisPieceArray;

	// player piece
	int playerCoord_x;
	int playerCoord_y;
	int playerPiece; // 0-7
	int playerRotation; // 0-3
	int nextPlayerPiece; 

	int playGrid_width = 10;
	int playGrid_height = 24;
	int playGrid_x_Offset = 10; //offset
	int playGrid_y_Offset = 8;

	// sounds
	sf::Sound* Sound_PieceDrop;
	sf::SoundBuffer SoundBuffer_PieceDrop;


	// score display

	sf::Font* ScoreFont;
	sf::Text* ScoreText;
	int scoreDisplay_width = 6;
	int scoreDisplay_height = 3;
	int scoreDisplay_coordX = 2;
	int scoreDisplay_coordY = 2;



	//next piece display

	sf::Text* NextPieceText;
	int NextPieceDisplay_width = 6;
	int NextPieceDisplay_height = 6;
	int NextPieceDisplay_coordX = 20;
	int NextPieceDisplay_coordY = 3;



	sf::Clock gameClock;
	int Last_DropTime = 0;
	int DropInterval = 1000;

	// methods ===========

	MainEngine();
	~MainEngine();

	// main grid, including the background (walls) and the play rectangle that the player plays within
	// but not the dynamic piece that is falling
	//std::vector<std::vector<int>> TetrisBoard;
	

	void DrawTile(int x, int y, int sprite_index);
	bool CheckPieceBlocked(int playerX, int playerY, int playerPiece, int playerRotation);
	void LoadTextureAndSprites();


	void MainLoop();

	void CreatePiece(int pieceType);

	void InitializeGrid();
	void DrawEverything();
	bool ProcessInput(const sf::Event& event); // return true if user pressed ESC
	void MovePieces();
	void MoveObjectLeft();
	void MoveObjectRight();
	void MoveObjectDown();
	void DrawPiece(int playerX, int playerY, int playerPiece, int playerRotation);
	void SetGridFromPiece();
	void RotatePieceLeft();
	void RotatePieceRight();
	void MoveRowsDown();
	int PlayerPieceIndexToGridSpriteIndex(int PieceIndex);

	bool GameOver();


	
};

