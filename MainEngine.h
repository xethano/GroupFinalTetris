#pragma once

#include <vector>
#include "TetrisPiece.h"

class MainEngine
{
public:


	// SFML stuff

	std::vector<sf::Sprite*> Sprites;
	std::vector<sf::Texture*> Textures;
	sf::RenderWindow* window;

	// game data

	std::vector<std::vector<int>> TetrisGrid;
	sf::Clock gameClock;
	int Last_DropTime = 0;
	int DropInterval = 1000;


	// user input

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

	// overall game state
	bool showingStartScreen = false;
	bool endGame = false;

	// piece storage
	// Tetris Pieces vector, stores TetrisPiece *'s
	std::vector<TetrisPiece> TetrisPieceArray; // these will be derived types with different block offsets

	// player data: piece, score, etc
	int playerCoord_x;
	int playerCoord_y;
	int playerPiece; // 0-7
	int playerRotation; // 0-3
	int nextPlayerPiece; 
	int Score;
	int TotalLinesCleared;

	// constants that define how the screen looks
	const int GridWidth = 30;
	const int GridHeight = 40;

	const int Tile_Empty = 0; // draw a black tile
	const int Tile_Cyan = 1;
	const int Tile_Shadow = 9;
	const int Tile_Noise = 8;

	// the tile playfield

	int playGrid_width = 10;
	int playGrid_height = 24;
	int playGrid_x_Offset = 10; //offset
	int playGrid_y_Offset = 8;

	// sounds
	sf::Sound* Sound_PieceDrop;
	sf::SoundBuffer SoundBuffer_PieceDrop;

	sf::Music Kazoo;
	sf::Music gameover_song;


	// score display

	sf::Font* ScoreFont;
	sf::Text* GenericText;
	sf::Text* AnyKeyText;
	int scoreDisplay_width = 6;
	int scoreDisplay_height = 3;
	int scoreDisplay_coordX = 2;
	int scoreDisplay_coordY = 2;
	std::vector<sf::Color> colorVector = { sf::Color::Red, sf::Color::Green ,sf::Color::Blue ,sf::Color::Magenta ,sf::Color::Yellow };

	//next piece display

	sf::Text* NextPieceText;
	int NextPieceDisplay_width = 6;
	int NextPieceDisplay_height = 6;
	int NextPieceDisplay_coordX = 20;
	int NextPieceDisplay_coordY = 3;

	// goofy start screen animation stuff
	
	std::string szUseControls = "       Use the Left, Right, and Down arrows to control the direction of your piece     ";
	int32_t lastScrollTime = 0;
	int32_t scrollDelay = 200;
	int lastFlashTime = 0;
	int flashTimeInterval = 1000;
	bool ShowPressKey = true;




	// methods ===========

	MainEngine();
	~MainEngine();

	// helper functions
	void DrawTile(int x, int y, int sprite_index);
	bool CheckPieceBlocked(int playerX, int playerY, int playerPiece, int playerRotation);

	// initialization
	void LoadTextureAndSprites();
	void InitializeGrid();

	// main game loop
	void MainLoop();
	bool GameOver();

	// start screen
	bool ProcessInput_StartScreen(const sf::Event& event); // return true if user pressed ESC

	bool ProcessInput_GameOverScreen(const sf::Event& event);
	void Draw_GameOverScreen(); 

	// in-game loop functions while playing
	void StartNewGame();
	void CreateNewPiece();
	void Draw_Game();
	void Draw_StartScreen();
	bool ProcessInput_Game(const sf::Event& event); // return true if user pressed ESC
	void MovePieces();
	void MoveObjectLeft();
	void MoveObjectRight();
	void MoveObjectDown();
	void DrawPiece(int playerX, int playerY, int playerPiece, int playerRotation, bool bIsShadowPiece = false);
	void DrawShadowPiece();
	void SetGridFromPiece();
	void RotatePieceLeft();
	void RotatePieceRight();
	void MoveRowsDown();
	int PlayerPieceIndexToGridSpriteIndex(int PieceIndex);
	void PieceDoneFalling();
};

