class MapGenerator
{
public:
	MapGenerator(int xSize = 32, int ySize = 32, int maxFeatures = 150, int chanceRoom = 75, int chanceHall = 25, int seed = -1);
	~MapGenerator();
	void setSeed(int seed);
	int getSeed();

	bool generateMap();
private:
	int xSize;
	int ySize;
	int maxFeatures;
	int chanceRoom;
	int chanceHall;
	int seed;

};