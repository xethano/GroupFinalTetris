#include "common.h"
#include "MainEngine.h"
#include <Windows.h>
#include <fstream>
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

	showingStartScreen = true;

	gameover_song.openFromFile("Audio\\GameOver.wav");
	Kazoo.openFromFile("Audio\\TetrisKazoo.ogg");
	Kazoo.setVolume(50);
	
	std::ifstream score_text("highscore.txt", std::ios::in); //get current highscore off file
	score_text >> Highscore;
	score_text.close();
	
	// load tetris pieces
	TetrisPieceArray.push_back(Piece_Block());
	TetrisPieceArray.push_back(Piece_Straight());
	TetrisPieceArray.push_back(Piece_ZigZagL());
	TetrisPieceArray.push_back(Piece_ZigZagR());
	TetrisPieceArray.push_back(Piece_EllL());
	TetrisPieceArray.push_back(Piece_EllR());
	TetrisPieceArray.push_back(Piece_Tee());	

	ScoreFont = new sf::Font();												//setting up text and font
	bool bLoaded = ScoreFont->loadFromFile("ozone.ttf");
	GenericText = new sf::Text(*ScoreFont);
	AnyKeyText = new sf::Text(*ScoreFont);
	NextPieceText = new sf::Text(*ScoreFont);

	bool loaded = SoundBuffer_PieceDrop.loadFromFile("audio/landed.wav");
	Sound_PieceDrop = new sf::Sound(SoundBuffer_PieceDrop);

	loaded = SoundBuffer_PieceRotate.loadFromFile("audio/rotate.wav");
	Sound_PieceRotate = new sf::Sound(SoundBuffer_PieceRotate);

	
}

MainEngine::~MainEngine()
{
	delete Sound_PieceRotate;
	delete Sound_PieceDrop;
	delete this->AnyKeyText;
	delete this->GenericText;
	delete NextPieceText;
	delete ScoreFont;
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

	// main loop
	while (window->isOpen())
	{
		bool b = false;
		//events running 
		sf::Event event; 
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			else
			{
				if (endGame)
				{
					
					 b = ProcessInput_GameOverScreen(event);
				}
				else
				{
					if (showingStartScreen)
					{
						// b not used. oh well.
						 b = ProcessInput_StartScreen(event);

					}
					else
					{
						 b = ProcessInput_Game(event);
					}
				}
				
			}
		}
		

		window->clear();

		if (endGame)
		{
			Draw_GameOverScreen();
			if (b == true)
			{
				std::ofstream updated_score("highscore.txt", std::ios::out);
				updated_score << Highscore;
				updated_score.close();

				window->close();
				break;
			}
			
		}
		else
		{
			if (showingStartScreen)
			{
				Draw_StartScreen();
			}
			else
			{
				Draw_Game();
				MovePieces();
			}
		}

		window->display();
		Sleep(100);
		
	}
}

void MainEngine::StartNewGame()
{
	//this is like a mock constructor in some senses
	showingStartScreen = false;
	Kazoo.play();
	Kazoo.setLoop(true);
	
	Score = 0;
	TotalLinesCleared = 0;

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

	// set this first
	nextPlayerPiece = rand() % 7;
	CreateNewPiece();

}

void MainEngine::CreateNewPiece() //controls piece creation and where the piece will generate
{
	playerCoord_x = playGrid_width / 2;
	playerCoord_y = 0;
	playerPiece = nextPlayerPiece;
	playerRotation = 0;

	nextPlayerPiece = rand() % 7;
}

void MainEngine::DrawTile(int x, int y, int sprite_index)
{
	sf::Sprite* p = Sprites[sprite_index];
	p->setPosition(sf::Vector2f(x * 32.0f, y * 32.0f));
	window->draw(*p);
}

void MainEngine::LoadTextureAndSprites()
{	//                                        (grid)     v- pieces start here                                       ... never gets to here
	//                    Sprite Index          0        1       2         3....
	std::vector<std::string> SpriteNames = { "Black", "Cyan", "Purple", "Yellow", "Green", "Blue", "Red", "Orange", "noise", "Shadow"};
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

void MainEngine::InitializeGrid()
{
	for (int y = 0; y < GridHeight; y++) //background
	{
		for (int x = 0; x < GridWidth; x++)
		{
			TetrisGrid[y][x] = Tile_Noise;
		}
	}

	for (int y = 0; y < scoreDisplay_height; y++)
	{
		for (int x = 0; x < scoreDisplay_width; x++)
		{
			
			TetrisGrid[y + scoreDisplay_coordY - 1][x + scoreDisplay_coordX - 1] = Tile_Empty; // empty, allows falling
		}
	}

	for (int y = 0; y < NextPieceDisplay_height; y++) //next piece display window
	{
		for (int x = 0; x < NextPieceDisplay_width; x++)
		{
			TetrisGrid[y + NextPieceDisplay_coordY - 1][x + NextPieceDisplay_coordX + 3] = Tile_Empty;
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



void MainEngine::Draw_Game()
{
	for (int y = 0; y < GridHeight; y++) //draws grid to screen
	{
		for (int x = 0; x < GridWidth; x++)
		{
			int tileIndex = TetrisGrid[y][x];
			DrawTile(x, y, tileIndex);
		}
	}

	DrawShadowPiece();
	DrawPiece(NextPieceDisplay_coordX + 6, NextPieceDisplay_coordY + 1, nextPlayerPiece, 0);
	DrawPiece(playerCoord_x + playGrid_x_Offset, playerCoord_y + playGrid_y_Offset, playerPiece, playerRotation);

	GenericText->setCharacterSize(24);
	GenericText->setPosition(sf::Vector2f(scoreDisplay_coordX * 32.0f, scoreDisplay_coordY * 32.0f));
	GenericText->setString("Score: " + std::to_string(Score));
	window->draw(*GenericText);
	GenericText->setPosition(sf::Vector2f(scoreDisplay_coordX * 32.0f, (scoreDisplay_coordY+1) * 32.0f));
	GenericText->setString("Lines: " + std::to_string(TotalLinesCleared));
	window->draw(*GenericText);

	NextPieceText->setCharacterSize(32);
	NextPieceText->setPosition(sf::Vector2f((NextPieceDisplay_coordX + 3) * 32.0f, (NextPieceDisplay_coordY - 1) * 32.0f));
	NextPieceText->setString("Next Piece");
	window->draw(*NextPieceText);
	
}

void MainEngine::Draw_StartScreen()
{
	GenericText->setCharacterSize(32);
	GenericText->setPosition(sf::Vector2f(1 * 32.0f, 1 * 32.0f));
	GenericText->setString("Welcome To Tetris");
	window->draw(*GenericText);

	GenericText->setCharacterSize(30);
	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 5 * 32.0f));
	GenericText->setString("Controls:");
	window->draw(*GenericText);


	GenericText->setCharacterSize(48);
	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 6 * 32.0f));
	std::string shortenedSZ = szUseControls.substr(0, 30);
	GenericText->setString(shortenedSZ.c_str());
	window->draw(*GenericText);

	int32_t tNow = this->gameClock.getElapsedTime().asMilliseconds();
	if (lastScrollTime == 0 || (tNow - lastScrollTime > scrollDelay))
	{
		lastScrollTime = tNow;
		char ch = szUseControls[0];
		szUseControls = szUseControls.substr(1) + ch;
	}

	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 8 * 32.0f));
	GenericText->setString("Use Q and E to rotate your piece");
	window->draw(*GenericText);

	if (lastFlashTime == 0 || tNow - lastFlashTime > flashTimeInterval)
	{
		lastFlashTime = tNow;
		ShowPressKey = !ShowPressKey;
		int colorValue = rand() % colorVector.size();
		AnyKeyText->setFillColor(colorVector[colorValue]);
		int randomMenuBonus = rand() % 1000;
		if (randomMenuBonus % 100 == 0)
		{
			AnyKeyText->setString("Programmed By Ethan and Team");
		}
		else
		{
			AnyKeyText->setString("Press Any Key to Start");
		}
	}

	if (ShowPressKey)
	{
		AnyKeyText->setCharacterSize(48);
		AnyKeyText->setPosition(sf::Vector2f(2 * 32.0f, 32 * 32.0f));

		window->draw(*AnyKeyText);
	}

	GenericText->setCharacterSize(26);
	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 12 * 32.0f));
	GenericText->setString("We would like to thank johnathon for the kazoo solo");
	window->draw(*GenericText);

	
	
}

void MainEngine::Draw_GameOverScreen()
{
	GenericText->setCharacterSize(80);
	GenericText->setPosition(sf::Vector2f(8 * 32.0f, 1 * 32.0f));
	GenericText->setString("Game Over");
	window->draw(*GenericText);

	

	GenericText->setCharacterSize(40);
	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 5 * 32.0f));
	GenericText->setString("Highscore: " + std::to_string(Highscore));
	window->draw(*GenericText);

	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 7 * 32.0f));
	GenericText->setString("Your Score: " + std::to_string(Score));
	window->draw(*GenericText);

	if (Score == Highscore)
	{
		GenericText->setPosition(sf::Vector2f(2 * 32.0f, 9 * 32.0f));
		GenericText->setString("New Highscore!");
		window->draw(*GenericText);
	}
	

	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 16 * 32.0f));
	GenericText->setString("Press Y to continue");
	window->draw(*GenericText);

	GenericText->setPosition(sf::Vector2f(2 * 32.0f, 18 * 32.0f));
	GenericText->setString("Press N to exit");
	window->draw(*GenericText);

}

bool MainEngine::ProcessInput_Game(const sf::Event& event) // return true if user pressed ESC
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

bool MainEngine::ProcessInput_StartScreen(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		int key = event.KeyPressed;
		sf::Event::KeyEvent ak = event.key;
		auto keyCode = ak.scancode;

		StartNewGame();
		return true;
	}
	return false;
}

bool MainEngine::ProcessInput_GameOverScreen(const sf::Event& event)
{
	Draw_GameOverScreen(); //draw the window before processing the input

	if (event.type == sf::Event::KeyPressed)
	{
		int key = event.KeyPressed;
		sf::Event::KeyEvent ak = event.key;
		auto keyCode = ak.scancode;

		if (keyCode == sf::Keyboard::Scan::Y) //continue and reset to default game 
		{
			endGame = false;
			showingStartScreen = true;
			Score = 0;
			TotalLinesCleared = 0;
			window->clear();
			InitializeGrid();
			return false;
		}

		if (keyCode == sf::Keyboard::Scan::N) //exit program 
		{
			endGame = true;
			window->clear();
			return true;
		}
		
		endGame = true;
		return false;
	}
	
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
		PieceDoneFalling();
	}

	downTriggered = false;
}

void MainEngine::PieceDoneFalling()
{
	Sound_PieceDrop->play();
	SetGridFromPiece();

	if (endGame)
	{
		return;
	}

	MoveRowsDown();
	CreateNewPiece();

	if (DropInterval > 100)
	{
		DropInterval -= 10; // milliseconds
	}

	Score += 30;
}

void MainEngine::DrawPiece(int gridX, int gridY, int playerPiece, int playerRotation, bool bIsShadowPiece)
{
	int SpriteIndex = PlayerPieceIndexToGridSpriteIndex(playerPiece);
	if (bIsShadowPiece)
	{
		SpriteIndex = Tile_Shadow;
	}

	for (int blockIndex = 0; blockIndex < 4; blockIndex++)
	{
		sf::Vector2i BlockXYDelta = TetrisPieceArray[playerPiece].GetBlockXYDelta(playerRotation, blockIndex);
		int xBlockDelta = BlockXYDelta.x;
		int yBlockDelta = BlockXYDelta.y;
		int GridX = gridX + xBlockDelta;
		int GridY = gridY + yBlockDelta;
		DrawTile(GridX, GridY, SpriteIndex);
	}
}

void MainEngine::DrawShadowPiece()
{
	// go down from current piece X/Y until we can't go down any more. This is where you draw the shadow piece
	int yDelta = 0;
	for (; yDelta < GridHeight; yDelta++)
	{
		if (CheckPieceBlocked(playerCoord_x, playerCoord_y + yDelta, playerPiece, playerRotation))
		{
			yDelta--; // back up one space
			break;
		}
	}
	DrawPiece(playGrid_x_Offset + playerCoord_x, playGrid_y_Offset + playerCoord_y + yDelta, playerPiece, playerRotation, true);
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
			Kazoo.stop();
			gameover_song.setVolume(100);
			gameover_song.play();
		}
	}

	if (endGame)
	{
		if (Score > Highscore)
		{
			Highscore = Score;
		}
	}

	
	
}

void MainEngine::RotatePieceLeft()
{
	bool isBlockedRotateLeft = CheckPieceBlocked(playerCoord_x, playerCoord_y, playerPiece, (playerRotation + 3 /* + 4 - 1*/ ) % 4);
	if (!isBlockedRotateLeft)
	{
		playerRotation += 3;
		playerRotation %= 4;
	}
	else
	{
		// blocked rotating left. could have hit the left or the right wall with a long piece
		
		// okay, could we scoot a piece to the left and then rotate left and be okay?
		isBlockedRotateLeft = CheckPieceBlocked(playerCoord_x - 1, playerCoord_y, playerPiece, (playerRotation + 3 /* + 4 - 1*/) % 4);
		if (!isBlockedRotateLeft)
		{
			playerRotation += 3;
			playerRotation %= 4;
			playerCoord_x -= 1;
		}
		else
		{
			isBlockedRotateLeft = CheckPieceBlocked(playerCoord_x + 1, playerCoord_y, playerPiece, (playerRotation + 3 /* + 4 - 1*/) % 4);
			if (!isBlockedRotateLeft)
			{
				playerRotation += 3;
				playerRotation %= 4;
				playerCoord_x += 1;
			}
			else
			{
				isBlockedRotateLeft = CheckPieceBlocked(playerCoord_x - 2, playerCoord_y, playerPiece, (playerRotation + 3 /* + 4 - 1*/) % 4);
				if (!isBlockedRotateLeft)
				{
					playerRotation += 3;
					playerRotation %= 4;
					playerCoord_x -= 2;
				}
				else
				{
					isBlockedRotateLeft = CheckPieceBlocked(playerCoord_x + 2, playerCoord_y, playerPiece, (playerRotation + 3 /* + 4 - 1*/) % 4);
					if (!isBlockedRotateLeft)
					{
						playerRotation += 3;
						playerRotation %= 4;
						playerCoord_x += 2;
					}
				}
			}
		}
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
	else
	{
		// blocked rotating left. could have hit the left or the right wall with a long piece

		// okay, could we scoot a piece to the left and then rotate left and be okay?
		isBlockedRotateRight = CheckPieceBlocked(playerCoord_x - 1, playerCoord_y, playerPiece, (playerRotation + 1) % 4);
		if (!isBlockedRotateRight)
		{
			playerRotation++;
			playerRotation %= 4;
			playerCoord_x -= 1;
		}
		else
		{
			isBlockedRotateRight = CheckPieceBlocked(playerCoord_x + 1, playerCoord_y, playerPiece, (playerRotation + 1) % 4);
			if (!isBlockedRotateRight)
			{
				playerRotation++;
				playerRotation %= 4;
				playerCoord_x += 1;
			}
			else
			{
				isBlockedRotateRight = CheckPieceBlocked(playerCoord_x - 2, playerCoord_y, playerPiece, (playerRotation + 1) % 4);
				if (!isBlockedRotateRight)
				{
					playerRotation++;
					playerRotation %= 4;
					playerCoord_x -= 2;
				}
				else
				{
					isBlockedRotateRight = CheckPieceBlocked(playerCoord_x + 2, playerCoord_y, playerPiece, (playerRotation + 1) % 4);
					if (!isBlockedRotateRight)
					{
						playerRotation++;
						playerRotation %= 4;
						playerCoord_x += 2;
					}
				}
			}
		}
	}

	RightRotationTriggered = false;
}

void MainEngine::MoveRowsDown()
{
	int LinesRemoved = 0;

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
			LinesRemoved++;

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
			
			goto retry;
		}
		
	}

	TotalLinesCleared += LinesRemoved;

	if (LinesRemoved > 0)
	{
		this->Score += pow(2, LinesRemoved) * 100;
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
	for (int y = playGrid_y_Offset; y < playGrid_y_Offset+2; y++)
	{
		for (int x = 0; x < playGrid_width; x++)
		{
			if (TetrisGrid[y][x + playGrid_x_Offset] != 0)
			{
				return true;
			}
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
		Sound_PieceRotate->play();
		RotatePieceLeft();
	}

	if (RightRotationTriggered || RightRotationPressed)
	{
		Sound_PieceRotate->play();
		RotatePieceRight();
	}
}



