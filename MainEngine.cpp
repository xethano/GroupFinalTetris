#include "common.h"
#include "MainEngine.h"
#include <Windows.h>
#include <iostream>

MainEngine::MainEngine()
{
	TetrisGrid.resize(GridHeight);
	for (std::vector<int> &xVector : TetrisGrid)
	{
		xVector.resize(GridWidth);
	}
	InitializeGrid();
	LoadTextureAndSprites();
	upPressed = false;
	downPressed = false;
	leftPressed = false;
	rightPressed = false;
	upTriggered = false;
	downTriggered = false;
	leftTriggered = false;
	rightTriggered = false;

	RightRotationPressed = false;
	RightRotationTriggered = false;
	LeftRotationPressed = false;
	LeftRotationTriggered = false;

	playerCoord_x = playGrid_width / 2;
	playerCoord_y = 1;
	playerPiece = 1;
	playerRotation = 0;
	nextPlayerPiece = rand() % 7;
	
	// load tetris pieces
	TetrisPieceArray.push_back(Piece_Block());
	TetrisPieceArray.push_back(Piece_Straight());
	TetrisPieceArray.push_back(Piece_ZigZagL());
	TetrisPieceArray.push_back(Piece_ZigZagR());
	TetrisPieceArray.push_back(Piece_EllL());
	TetrisPieceArray.push_back(Piece_EllR());
	TetrisPieceArray.push_back(Piece_Tee());	

	ScoreFont = new sf::Font();
	bool bLoaded = ScoreFont->loadFromFile("ozone.ttf");
	ScoreText = new sf::Text(*ScoreFont);
	NextPieceText = new sf::Text(*ScoreFont);

	ScoreText->setPosition(sf::Vector2f(scoreDisplay_coordX * 32.0f, scoreDisplay_coordY * 32.0f));
	NextPieceText->setPosition(sf::Vector2f( (NextPieceDisplay_coordX + 3) * 32.0f, (NextPieceDisplay_coordY - 2) * 32.0f));

	bool loaded = SoundBuffer_PieceDrop.loadFromFile("audio/landed.wav");
	Sound_PieceDrop = new sf::Sound(SoundBuffer_PieceDrop);

	/*for (int i = 9; i > 0; i--)
	{
		TetrisGrid[i + playGrid_y][3 + playGrid_x] = 13;
	}*/
	// - playerPiece (base rotation on type of player piece)
	
}

MainEngine::~MainEngine()
{
	delete Sound_PieceDrop;
}

void MainEngine::MainLoop()
{
	/*
	*
	* while(!user_wants_to_exit())
	* {
	*	// wait until the right time to draw....
	*	// draw whatever needs to be drawn
	*	// move all objects that need to be moved
	*	// check user input
	*	// does user want to exit?
	* }
	*/

	// create a window
	window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(32 * GridWidth, 32 * GridHeight)), "Tetris");

	// here's the loop
	while (window->isOpen())
	{
		//events running 
		sf::Event event; 
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			else
			{
				bool b = ProcessInput(event);
			}
		}
		

		window->clear();

		if (endGame)
		{
			std::cout << "You lose" << std::endl;
			window->close();
			break;
		}

		// do your drawing and moving and event processing here
		DrawEverything();
		MovePieces();

		window->display();
		Sleep(100);
		
	}
}

void MainEngine::DrawTile(int x, int y, int sprite_index)
{
	sf::Sprite* p = Sprites[sprite_index];
	p->setPosition(sf::Vector2f(x * 32.0f, y * 32.0f));
	window->draw(*p);
}

void MainEngine::LoadTextureAndSprites()
{
	//                    Sprite Index          0        1       2         3....
	std::vector<std::string> SpriteNames = { "Black", "Cyan", "Purple", "Yellow", "Green", "Blue", "White", "Red", "Orange" };
	size_t sz = SpriteNames.size();
	for (size_t i = 0; i < sz; i++)
	{
		std::string filename = "Graphics\\tile_" + SpriteNames[i] + ".png";
		sf::Texture* t1 = new sf::Texture();
		bool loaded = t1->loadFromFile(filename);
		Textures.push_back(t1);
		sf::Sprite* s1 = new sf::Sprite(*t1);
		Sprites.push_back(s1);
	}
}



void MainEngine::CreatePiece(int pieceType)
{
	switch (pieceType)
	{

	case 1: //generate squiggle block 
		break;

	case 2:	//other squiggle
		break;

	case 3:	//square
		break;

	case 4: //L
		break;

	case 5: //_|
		break;

	case 6: // |
		
		break;

	default:
		break;
	}
	
		
	
}

void MainEngine::InitializeGrid()
{
	for (int y = 0; y < GridHeight; y++)
	{
		for (int x = 0; x < GridWidth; x++)
		{
			
			TetrisGrid[y][x] = 7;
		}
	}

	for (int y = 0; y < scoreDisplay_height; y++)
	{
		for (int x = 0; x < scoreDisplay_width; x++)
		{
			
			TetrisGrid[y + scoreDisplay_coordY - 1][x + scoreDisplay_coordX - 1] = Tile_Empty; // empty, allows falling
		}
	}

	for (int y = 0; y < NextPieceDisplay_height; y++)
	{
		for (int x = 0; x < NextPieceDisplay_width; x++)
		{
			TetrisGrid[y + NextPieceDisplay_coordY -2][x + NextPieceDisplay_coordX + 3] = Tile_Empty;
		}
	}

	for (int y = 0; y < playGrid_height; y++)
	{
		for (int x = 0; x < playGrid_width; x++)
		{
			int x0 = x + playGrid_x_Offset; //include offset
			int y0 = y + playGrid_y_Offset;
			TetrisGrid[y0][x0] = Tile_Empty; // empty, allows falling
		}
	}

	/*for (int  i = playGrid_height-2; i < playGrid_height; i++)
	{
		for (int y = 0; y < playGrid_width; y++)
		{
			TetrisGrid[i + playGrid_y][y + playGrid_x] = 28;
		}
		
	}*/

	for (int y = -1; y <= playGrid_height; y++)
	{
		TetrisGrid[y + playGrid_y_Offset][9] = Tile_Cyan;
		TetrisGrid[y + playGrid_y_Offset][20] = Tile_Cyan;
	}
	
	for (int x = 0; x < playGrid_width; x++)
	{
		TetrisGrid[7][ x + playGrid_x_Offset] = Tile_Cyan;
		TetrisGrid[32][x + playGrid_x_Offset] = Tile_Cyan;
	}



	
}

void MainEngine::DrawEverything()
{
	for (int y = 0; y < GridHeight; y++)
	{
		for (int x = 0; x < GridWidth; x++)
		{
			int tileIndex = TetrisGrid[y][x];
			DrawTile(x, y, tileIndex);
		}
	}

	
	DrawPiece(NextPieceDisplay_coordX + 6, NextPieceDisplay_coordY + 1, nextPlayerPiece, 0);

	DrawPiece(playerCoord_x + playGrid_x_Offset, playerCoord_y + playGrid_y_Offset, playerPiece, playerRotation);

	ScoreText->setCharacterSize(24);
	ScoreText->setString("Score: " + std::to_string(Score));
	window->draw(*ScoreText);

	NextPieceText->setCharacterSize(32);
	NextPieceText->setString("Next Piece");
	window->draw(*NextPieceText);
	
}

bool MainEngine::ProcessInput(const sf::Event& event) // return true if user pressed ESC
{
	if (event.type == sf::Event::KeyPressed)
	{
		int key = event.KeyPressed;
		sf::Event::KeyEvent ak = event.key;
		auto keyCode = ak.scancode;

		if (keyCode == sf::Keyboard::Scan::Down)
		{
			//OnKeyDownDown();
			downPressed = true;
			downTriggered = true;
		}

		if (keyCode == sf::Keyboard::Scan::Left)
		{
			//OnKeyDownDown();
			leftPressed = true;
			leftTriggered = true;
		}

		if (keyCode == sf::Keyboard::Scan::Right)
		{
			//OnKeyDownDown();
			rightPressed = true;
			rightTriggered = true;
		}

		if (keyCode == sf::Keyboard::Scan::Up)
		{
			//hold
			upPressed = true;
			upTriggered = true;
		}

		if (keyCode == sf::Keyboard::Scan::Q)
		{
			//hold
			LeftRotationPressed = true;
			LeftRotationTriggered = true;
		}

		if (keyCode == sf::Keyboard::Scan::E)
		{
			//hold
			RightRotationPressed = true;
			RightRotationTriggered = true;
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		int key = event.KeyReleased;
		sf::Event::KeyEvent ak = event.key;
		auto keyCode = ak.scancode;

		if (keyCode == sf::Keyboard::Scan::Down)
		{
			//OnKeyDownDown();
			downPressed = false;
			
		}

		if (keyCode == sf::Keyboard::Scan::Left)
		{
			//OnKeyDownDown();
			leftPressed = false;

		}

		if (keyCode == sf::Keyboard::Scan::Right)
		{
			//OnKeyDownDown();
			rightPressed = false;

		}

		if (keyCode == sf::Keyboard::Scan::Up)
		{
			//OnKeyDownDown();
			upPressed = false;

		}

		if (keyCode == sf::Keyboard::Scan::Q)
		{
			//hold
			LeftRotationPressed = false;
			
		}

		if (keyCode == sf::Keyboard::Scan::E)
		{
			//hold
			RightRotationPressed = false;
			
		}
	}
	return false;
}

void MainEngine::MoveObjectLeft()
{

	bool isBlockedLeft = CheckPieceBlocked(playerCoord_x - 1, playerCoord_y, playerPiece, playerRotation); //checks all blocks, can be used for all movements
	if (!isBlockedLeft)
	{
		playerCoord_x--;
	}

	
	leftTriggered = false; //event handling
}

void MainEngine::MoveObjectRight()
{
	bool isBlockedLeft = CheckPieceBlocked(playerCoord_x + 1, playerCoord_y, playerPiece, playerRotation); //checks all blocks, can be used for all movements
	if (!isBlockedLeft)
	{
		playerCoord_x++;
	}

	
	rightTriggered = false;
}

void MainEngine::MoveObjectDown()
{
	bool isBlockedDown = CheckPieceBlocked(playerCoord_x, playerCoord_y+1, playerPiece, playerRotation); //checks all blocks, can be used for all movements
	if (!isBlockedDown)
	{
		playerCoord_y++;
	}
	else
	{
		Sound_PieceDrop->play();
		SetGridFromPiece();
		MoveRowsDown();
		playerCoord_x = playGrid_width / 2;
		playerCoord_y = 1;
		
		playerPiece = nextPlayerPiece;
		nextPlayerPiece = rand() % 7;
	}
	downTriggered = false;
}

void MainEngine::DrawPiece(int playerX, int playerY, int playerPiece, int playerRotation)
{
	int SpriteIndex = PlayerPieceIndexToGridSpriteIndex(playerPiece);

	for (int blockIndex = 0; blockIndex < 4; blockIndex++)
	{
		sf::Vector2i BlockXYDelta = TetrisPieceArray[playerPiece].GetBlockXYDelta(playerRotation, blockIndex);
		int xBlockDelta = BlockXYDelta.x;
		int yBlockDelta = BlockXYDelta.y;
		int GridX = playerX + xBlockDelta;
		int GridY = playerY + yBlockDelta;
		DrawTile(GridX, GridY, SpriteIndex);
	}
}

void MainEngine::SetGridFromPiece() //set grid 
{
	for (int blockIndex = 0; blockIndex < 4; blockIndex++)
	{
		sf::Vector2i BlockXYDelta = TetrisPieceArray[playerPiece].GetBlockXYDelta(playerRotation, blockIndex);
		int xBlockDelta = BlockXYDelta.x;
		int yBlockDelta = BlockXYDelta.y;
		int GridX = playGrid_x_Offset + playerCoord_x + xBlockDelta;
		int GridY = playGrid_y_Offset + playerCoord_y + yBlockDelta;
		TetrisGrid[GridY][GridX] = PlayerPieceIndexToGridSpriteIndex(playerPiece);
		bool isGameOver = GameOver();
		if (isGameOver)
		{
			endGame = true;
		}
	}

	Score += 30;
	
}

void MainEngine::RotatePieceLeft()
{
	bool isBlockedRotateLeft = CheckPieceBlocked(playerCoord_x, playerCoord_y, playerPiece, (playerRotation + 3 /* + 4 - 1*/ ) % 4);
	if (!isBlockedRotateLeft)
	{
		
		playerRotation += 3;
		playerRotation %= 4;
		
	}

	LeftRotationTriggered = false;
	
}

void MainEngine::RotatePieceRight()
{

	bool isBlockedRotateRight = CheckPieceBlocked(playerCoord_x, playerCoord_y, playerPiece, (playerRotation + 1) % 4);
	if (!isBlockedRotateRight)
	{
		playerRotation++;
		playerRotation %= 4;
	}
	
	RightRotationTriggered = false;
}

void MainEngine::MoveRowsDown()
{
retry:

	for (int y = playGrid_height - 1; y > 0; y--)
	{
		//for every row, check if complete, if so move everything down by 1 row by row
		bool filled = true;
		for (int x = 0; x < playGrid_width; x++)
		{
			if (TetrisGrid[y+ playGrid_y_Offset][x + playGrid_x_Offset] == Tile_Empty)
			{
				filled = false;
				break;
				
			}
		}

		if (!filled)
		{
			continue;
		}
		else
		{
			for (int k = y; k >= 1; k--) //move everything down by one
			{
				for (int d = 0; d < playGrid_width; d++)
				{
					TetrisGrid[k + playGrid_y_Offset][d + playGrid_x_Offset] = TetrisGrid[k + playGrid_y_Offset - 1][d + playGrid_x_Offset];
				}
			}

			for (int t = 0; t < playGrid_width; t++)
			{
				TetrisGrid[playGrid_y_Offset][t + playGrid_x_Offset] = Tile_Empty;
			}
			
			Score += 100;
			goto retry;
		}
		
	}
}

int MainEngine::PlayerPieceIndexToGridSpriteIndex(int PieceIndex)
{
	// PieceIndex is guaranteed 0-6, 0 = square, 1 = line, etc
	// TileIndex is the index of which texture to draw for that PieceIndex
	int SpriteIndex = PieceIndex + 1; // map a piece to a color. tile[1] is yellow, tile[2] is red
	return SpriteIndex;
}

bool MainEngine::GameOver()
{
	
	for (int x = 0; x < playGrid_width; x++)
	{
		if (TetrisGrid[playGrid_y_Offset][x + playGrid_x_Offset] != 0)
		{
			return true;
		}
	}
	return false;
}

bool MainEngine::CheckPieceBlocked(int playerX, int playerY, int playerPiece, int playerRotation)
{
	// at every piece for the player, see if that grid location is blocked
	for (int blockIndex = 0; blockIndex < 4; blockIndex++)
	{
		sf::Vector2i BlockXYDelta = TetrisPieceArray[playerPiece].GetBlockXYDelta(playerRotation, blockIndex);
		int xBlockDelta = BlockXYDelta.x;
		int yBlockDelta = BlockXYDelta.y;
		int GridX = playGrid_x_Offset + playerX + xBlockDelta;
		int GridY = playGrid_y_Offset + playerY + yBlockDelta;
		// if grid is non-empty at DrawX, DrawY, then this piece is blocked
		if (TetrisGrid[GridY][GridX] != Tile_Empty)
		{
			return true;
		}
	}

	return false;
}



void MainEngine::MovePieces()
{
	int time_ms = gameClock.getElapsedTime().asMilliseconds();
	int TimeSinceLastDrop = time_ms - Last_DropTime;
	if (TimeSinceLastDrop > DropInterval)
	{
		downTriggered = true;
		Last_DropTime = time_ms;
	}
	

	if (leftTriggered || leftPressed)
	{
		MoveObjectLeft();
	}

	if (rightTriggered || rightPressed)
	{
		MoveObjectRight();
	}

	if (downTriggered || downPressed)
	{
		MoveObjectDown();
	}

	if (LeftRotationTriggered || LeftRotationPressed)
	{
		RotatePieceLeft();
	}

	if (RightRotationTriggered || RightRotationPressed)
	{
		RotatePieceRight();
	}
}



