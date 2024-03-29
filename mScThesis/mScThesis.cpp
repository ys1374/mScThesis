#include <iostream>
#include "Autostore.h"
#include "AStar.h"
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <functional>


const unsigned int numOfFirstRobots{ 3 };
const unsigned int numOfSecondRobots{ 2 };

#if 1
const unsigned int filledPercentOfWarehouse{ 80 }; //in %
double shiftHours{ 8 };


// ************** if changed these 3 delete files************************
const unsigned int xLenghOfWarehouse{ 30 }; //max x size of gird
const unsigned int yLenghOfWarehouse{ 30 }; //max y size of gird
const unsigned int zLenghOfWarehouse{ 20 }; //max num of bins in a column

const unsigned int secondRobotCapacity{ 5 };
const unsigned int numOfPorts{ 3 };

//***********************************************************************

long long int locationId{ 0 }, binId{ 0 };
long long int retrivalTaskId{ 0 };

unsigned int numOfGridLocations{ xLenghOfWarehouse * yLenghOfWarehouse * zLenghOfWarehouse };
unsigned int maxNumOfBins{ xLenghOfWarehouse * yLenghOfWarehouse * zLenghOfWarehouse * filledPercentOfWarehouse / 100 };
int iteration{ 300 };

#endif

//functions
#if 1
int generateRandomNumber(int max);
void fillQueueOfBinRetrival(std::vector<Autostore::bin>& queueOfBinRetrival, const std::string& filename, std::vector<Autostore::bin> binsVector_);
void fillFirstRobotLocation(std::vector<Autostore::firstRobot>& firstRobotsVector_, const std::string& filename_);
void fillSecondRobotLocation(std::vector<Autostore::secondRobot>& secondRobotsVector_, const std::string& filename_);
bool fileExists(const std::string& filename);

//--------------------------------



int PortLocations[3][3] = {
{xLenghOfWarehouse / 2 , 0, 0},
{0 , yLenghOfWarehouse / 2 , 1},
{xLenghOfWarehouse - 1, yLenghOfWarehouse / 2, 2}
};

#endif


int main()
{	//vectors
#if 1
	std::vector<Autostore::port> portsVector;
	std::vector<Autostore::firstRobot> firstRobotsVector;
	std::vector<Autostore::secondRobot> secondRobotsVector;
	std::vector<Autostore::bin> binsVector;
	std::vector<std::vector<std::vector<Autostore::gridLocation>>> gridLocationVector(xLenghOfWarehouse,std::vector<std::vector<Autostore::gridLocation>>(yLenghOfWarehouse,std::vector<Autostore::gridLocation>(zLenghOfWarehouse)));
	std::vector<Autostore::bin> queueOfBinRetrival;

	std::vector<Autostore::bin> twoTypeOneRoundOfRetrivalTask;
	std::vector<Autostore::bin> bothTypeOneRoundOfRetrivalTask;

	std::vector<Autostore::retrivalTask> oneTypeFinishedRetriveTaskVector;
	std::vector<Autostore::retrivalTask> twoTypeFinishedRetriveTaskVector;
	std::vector<Autostore::retrivalTask> bothTypeFinishedRetriveTaskVector;

	
#endif

	//reserve vectors
#if 1
	portsVector.reserve(numOfPorts);
	firstRobotsVector.reserve(numOfFirstRobots);
	secondRobotsVector.reserve(numOfSecondRobots);
	binsVector.reserve(maxNumOfBins);
	queueOfBinRetrival.reserve(maxNumOfBins);

	twoTypeOneRoundOfRetrivalTask.reserve(numOfFirstRobots);
	bothTypeOneRoundOfRetrivalTask.reserve(numOfFirstRobots);

	oneTypeFinishedRetriveTaskVector.reserve(maxNumOfBins);
	twoTypeFinishedRetriveTaskVector.reserve(maxNumOfBins);
	bothTypeFinishedRetriveTaskVector.reserve(maxNumOfBins);

#endif
	//objects
#if 1
	Autostore::port portObject;
	Autostore::firstRobot firstRobotObject;
	Autostore::secondRobot secondRobotObject;
	Autostore::bin binsObject;
	Autostore::gridLocation gridLocationObject;
#endif

//filling Vectors----------------------------------------------------------------------

	//port
#if 1
	for (auto port : PortLocations) {
		portObject.xLocation = port[0];
		portObject.yLocation = port[1];

		portObject.id = port[2];

		portsVector.push_back(portObject);
	}
#endif
	//first robot
#if 1
	for (int i = 0; i < numOfFirstRobots; i++) {
		firstRobotObject.nameFillerFirstRobot(i);
		firstRobotObject.xLocation = generateRandomNumber(xLenghOfWarehouse-1);
		firstRobotObject.yLocation = generateRandomNumber(yLenghOfWarehouse-1);
		
		firstRobotsVector.push_back(firstRobotObject);
	}
	// write/read robots location to/from file
	fillFirstRobotLocation(firstRobotsVector, "firstRobots_locations.txt");
#endif
	//second robot
#if 1
	for (int i = 0; i < numOfSecondRobots; i++) {
		secondRobotObject.nameFillerSecondRobot(i);
		secondRobotObject.xLocation = generateRandomNumber(xLenghOfWarehouse-1);
		secondRobotObject.yLocation = generateRandomNumber(yLenghOfWarehouse-1);
		secondRobotObject.id = i;
		secondRobotObject.bins.reserve(secondRobotCapacity);
		secondRobotsVector.push_back(secondRobotObject);
	}
	// write/read robots location to/from file
	fillSecondRobotLocation(secondRobotsVector, "secondRobots_locations.txt");
#endif
	//locations and bins
#if 1
	for (int k = 0; k < zLenghOfWarehouse; k++) {
		for (int j = 0; j < yLenghOfWarehouse; j++) {
			for (int i = 0; i < xLenghOfWarehouse; i++) {

				gridLocationVector[i][j][k].gridFillerWithBin(i, j, k, locationId);//filling grid vector

				if (i == 15 && j ==2) {
					std::cout << "";
				}

				//ports excluded top grid location for --------
				for (auto port : PortLocations) {
					if (i == port[0] && j == port[1]) { goto skipFillingBins; }
				}

				//----------------------------------------------
				if (locationId < maxNumOfBins) { //just fill the locations which bins can take according to filledPercentOfWarehouse

					binsObject.binFillerWithdata(i, j, k, binId);
					binsObject.locationId = locationId;
					binsObject.locationName = gridLocationVector[i][j][k].locationName;

					binsVector.push_back(binsObject); //filling bins vector

					gridLocationVector[i][j][k].binId = binId;
					gridLocationVector[i][j][k].isFilledWithBin = true;

					binId++;

				}




			
			skipFillingBins:

				locationId++;
				//std::cout << "locid" << gridLocationVector[i][j][k].locationId << "\t" << gridLocationVector[i][j][k].locationName << "\n";


			}
		}
	}
	
	std::cout << "Objects on Warehouse all set!\n";
#endif
	//Queue for retrival
#if 1
	fillQueueOfBinRetrival(queueOfBinRetrival, "queueOfBinRetrival.txt", binsVector);
#endif


//EXCEL file ----------------------------------------------------------------------------

	//oneTypeExcel
#if 1
	auto oneTypeExcelfileName{ "oneTypeExcelfile.xlsx" };
	lxw_workbook* oneTypeWorkbook = workbook_new(oneTypeExcelfileName);
	lxw_worksheet* oneTypeWorksheet = workbook_add_worksheet(oneTypeWorkbook, NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 0, "LOCATION", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 1, "Bin X", NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 2, "Bin Y", NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 3, "Bin Z", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 4, "FR X", NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 5, "FR Y", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 6, "Port X", NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 7, "Port Y", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 8, "#BTo Relocate", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 9, "FR ToB CT", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 10, "E CT", NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 11, "L CT", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 12, "BToP CT", NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 13, "PToB CT", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 14, "E goal CT", NULL);
	worksheet_write_string(oneTypeWorksheet, 0, 15, "relocationCT", NULL);

	worksheet_write_string(oneTypeWorksheet, 0, 16, "CT", NULL);

	
#endif
	//twoTypeExcel
#if 1	
	auto twoTypeExcelfileName{ "twoTypeExcelfile.xlsx" };
	lxw_workbook* twoTypeWorkbook = workbook_new(twoTypeExcelfileName);
	lxw_worksheet* twoTypeWorksheet = workbook_add_worksheet(twoTypeWorkbook, NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 0, "LOCATION", NULL);

	worksheet_write_string(twoTypeWorksheet, 0, 1, "Bin X", NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 2, "Bin Y", NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 3, "Bin Z", NULL);

	worksheet_write_string(twoTypeWorksheet, 0, 4, "FR X", NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 5, "FR Y", NULL);

	worksheet_write_string(twoTypeWorksheet, 0, 6, "# SR", NULL);

	worksheet_write_string(twoTypeWorksheet, 0, 7, "Port X", NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 8, "Port Y", NULL);

	worksheet_write_string(twoTypeWorksheet, 0, 9, "FR ToB CT", NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 10, "SRs ToB CT", NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 11, "Rs ToB CT", NULL);
	
	worksheet_write_string(twoTypeWorksheet, 0, 12, "2T E CT", NULL);
	worksheet_write_string(twoTypeWorksheet, 0, 13, "2T L CT", NULL);

	worksheet_write_string(twoTypeWorksheet, 0, 14, "FR ToP CT", NULL);
		
	worksheet_write_string(twoTypeWorksheet, 0, 15, "CT", NULL);

#endif
	//bothTypeExcel
#if 1	
	auto bothTypeExcelfileName{ "bothTypeExcelfile.xlsx" };
	lxw_workbook* bothTypeWorkbook = workbook_new(bothTypeExcelfileName);
	lxw_worksheet* bothTypeWorksheet = workbook_add_worksheet(bothTypeWorkbook, NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 0, "LOCATION", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 1, "Bin X", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 2, "Bin Y", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 3, "Bin Z", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 4, "FR X", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 5, "FR Y", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 6, "# SR", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 7, "Port X", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 8, "Port Y", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 9, "FR ToB CT", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 10, "SR ToB CT", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 11, "Rs ToB CT", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 12, "1T E CT", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 13, "2T E CT", NULL);
		
	worksheet_write_string(bothTypeWorksheet, 0, 14, "B ToP CT", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 15, "P ToB CT", NULL);
	
	worksheet_write_string(bothTypeWorksheet, 0, 16, "1T L CT", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 17, "2T L CT", NULL);
	
	worksheet_write_string(bothTypeWorksheet, 0, 18, "1T CT", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 19, "2T CT", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 20, "1T CT IN", NULL);
	worksheet_write_string(bothTypeWorksheet, 0, 21, "2T CT IN", NULL);

	worksheet_write_string(bothTypeWorksheet, 0, 22, "CT", NULL);


#endif

//filling retrivalTaskObject--------------------------------------------------------------
	//Vectors Copy	
#if 1
	auto oneTypePortsVector = portsVector;
	auto twoTypePortsVector = portsVector;
	auto bothTypePortsVector = portsVector;

	auto oneTypeFirstRobotsVector = firstRobotsVector;
	auto twoTypeFirstRobotsVector = firstRobotsVector;
	auto bothTypeFirstRobotsVector = firstRobotsVector;

	auto oneTypeSecondRobotsVector = secondRobotsVector;
	auto twoTypeSecondRobotsVector = secondRobotsVector;
	auto bothTypeSecondRobotsVector = secondRobotsVector;

	auto oneTypeBinsVector = binsVector;
	auto twoTypeBinsVector = binsVector;
	auto bothTypeBinsVector = binsVector;

	auto oneTypeGridLocationVector = gridLocationVector;
	auto twoTypeGridLocationVector = gridLocationVector;
	auto bothTypeGridLocationVector = gridLocationVector;

	auto oneTypeQueueOfBinRetrival = queueOfBinRetrival;
	auto twoTypeQueueOfBinRetrival = queueOfBinRetrival;
	auto bothTypeQueueOfBinRetrival = queueOfBinRetrival;




#endif

	//retrival objects
	//One Type
#if 1
	Autostore::retrivalTask oneTypeRetrivalTaskObject(oneTypePortsVector, oneTypeFirstRobotsVector, oneTypeSecondRobotsVector, oneTypeBinsVector, oneTypeGridLocationVector);

	oneTypeRetrivalTaskObject.constants.xLenghOfWarehouse = xLenghOfWarehouse;
	oneTypeRetrivalTaskObject.constants.yLenghOfWarehouse = yLenghOfWarehouse;
	oneTypeRetrivalTaskObject.constants.zLenghOfWarehouse = zLenghOfWarehouse;

	oneTypeRetrivalTaskObject.constants.secondRobotCapacity = secondRobotCapacity;
#endif
	//Two type
#if 1
	Autostore::retrivalTask twoTypeRetrivalTaskObject(twoTypePortsVector, twoTypeFirstRobotsVector, twoTypeSecondRobotsVector, twoTypeBinsVector, twoTypeGridLocationVector);

	twoTypeRetrivalTaskObject.constants.xLenghOfWarehouse = xLenghOfWarehouse;
	twoTypeRetrivalTaskObject.constants.yLenghOfWarehouse = yLenghOfWarehouse;
	twoTypeRetrivalTaskObject.constants.zLenghOfWarehouse = zLenghOfWarehouse;

	twoTypeRetrivalTaskObject.constants.secondRobotCapacity = secondRobotCapacity;

#endif
	//Both type
#if 1
	Autostore::retrivalTask bothTypeRetrivalTaskObject(bothTypePortsVector, bothTypeFirstRobotsVector, bothTypeSecondRobotsVector, bothTypeBinsVector, bothTypeGridLocationVector);

	bothTypeRetrivalTaskObject.constants.xLenghOfWarehouse = xLenghOfWarehouse;
	bothTypeRetrivalTaskObject.constants.yLenghOfWarehouse = yLenghOfWarehouse;
	bothTypeRetrivalTaskObject.constants.zLenghOfWarehouse = zLenghOfWarehouse;

	bothTypeRetrivalTaskObject.constants.secondRobotCapacity = secondRobotCapacity;
#endif



	iteration = queueOfBinRetrival.size();
// Throughput------------------------------------------------------------------------------------------


	//one type
	//numOfFirstRobots = numOfFirstRobots_ + numOfSecondRobots_;
	//numOfSecondRobots = 0;
#if 0
	std::cout << "***********************One Type*******************************";
	while ((!oneTypeQueueOfBinRetrival.empty()))
	{	



		std::cout << "\nRetriving " << oneTypeQueueOfBinRetrival[0].locationName;

		oneTypeRetrivalTaskObject.reset();
		oneTypeRetrivalTaskObject.id = retrivalTaskId;
		oneTypeRetrivalTaskObject.binToRetrive = oneTypeBinsVector[oneTypeQueueOfBinRetrival[0].binId];
		
		worksheet_write_string(oneTypeWorksheet, retrivalTaskId + 1, 0, oneTypeQueueOfBinRetrival[0].locationName.c_str(), NULL);
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 1, oneTypeQueueOfBinRetrival[0].xLocation, NULL);
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 2, oneTypeQueueOfBinRetrival[0].yLocation, NULL);
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 3, oneTypeQueueOfBinRetrival[0].zLocation, NULL);

		oneTypeRetrivalTaskObject.firstRobotSelection();
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 4, oneTypeFirstRobotsVector[oneTypeRetrivalTaskObject.selectedfirstRobot.id].xLocation, NULL);
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 5, oneTypeFirstRobotsVector[oneTypeRetrivalTaskObject.selectedfirstRobot.id].yLocation, NULL);


		oneTypeRetrivalTaskObject.portSelection();
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 6, oneTypePortsVector[oneTypeRetrivalTaskObject.selectedPort.id].xLocation, NULL);
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 7, oneTypePortsVector[oneTypeRetrivalTaskObject.selectedPort.id].yLocation, NULL);


		double cycleTime = oneTypeRetrivalTaskObject.oneTypeCycleTime(oneTypeWorksheet);
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 16, cycleTime, NULL);
		worksheet_write_number(oneTypeWorksheet, retrivalTaskId + 1, 17, retrivalTaskId, NULL);

		oneTypeQueueOfBinRetrival.erase(oneTypeQueueOfBinRetrival.begin());
		oneTypeFinishedRetriveTaskVector.push_back(oneTypeRetrivalTaskObject);
		

		//shift chech
#if 1				
		oneTypeFirstRobotsVector[oneTypeRetrivalTaskObject.selectedfirstRobot.id].time =
			oneTypeFirstRobotsVector[oneTypeRetrivalTaskObject.selectedfirstRobot.id].time + cycleTime; // change robot time
		
		oneTypeFirstRobotsVector[oneTypeRetrivalTaskObject.selectedfirstRobot.id].numOfTasks++;

		if (oneTypeFirstRobotsVector[oneTypeRetrivalTaskObject.selectedfirstRobot.id].time >= (shiftHours * 60 * 60)) {
			std::cout << "\n\n\n************************Shift Hours is Passed**************************\n\n\n";

			

			for (auto robot : oneTypeFirstRobotsVector) {
				std::cout << "Robot Id:" << robot.id << " Robot time: " << robot.time << " Robot Tasks: " << robot.numOfTasks << "\n";
			}

			std::cout << "total num of retrived Bin: " << oneTypeFinishedRetriveTaskVector.size()
				<< " throughput/hour: " << oneTypeFinishedRetriveTaskVector.size()/shiftHours;

			break;
		}

		if (retrivalTaskId % numOfFirstRobots == 1) {

			for (auto& robot : oneTypeFirstRobotsVector) {
				robot.isBusy = false;
			}
			//break;
		}
#endif	
		
		if (retrivalTaskId == iteration) { 
			break; 
		}
		retrivalTaskId++;

		
		
	}
	
	workbook_close(oneTypeWorkbook);
#endif


	//two type
#if 0
	std::cout << "***********************Two Type*******************************";
	retrivalTaskId = 0;
	while ((!twoTypeQueueOfBinRetrival.empty()))
	{
		std::cout << "\nRetriving " << twoTypeQueueOfBinRetrival[0].locationName ;

		twoTypeRetrivalTaskObject.reset();
		twoTypeRetrivalTaskObject.id = retrivalTaskId;
		twoTypeRetrivalTaskObject.binToRetrive = twoTypeBinsVector[twoTypeQueueOfBinRetrival[0].binId];

		worksheet_write_string(twoTypeWorksheet, retrivalTaskId + 1, 0, twoTypeQueueOfBinRetrival[0].locationName.c_str(), NULL);
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 1, twoTypeQueueOfBinRetrival[0].xLocation, NULL);
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 2, twoTypeQueueOfBinRetrival[0].yLocation, NULL);
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 3, twoTypeQueueOfBinRetrival[0].zLocation, NULL);

		twoTypeRetrivalTaskObject.firstRobotSelection();
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 4, twoTypeFirstRobotsVector[twoTypeRetrivalTaskObject.selectedfirstRobot.id].xLocation, NULL);
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 5, twoTypeFirstRobotsVector[twoTypeRetrivalTaskObject.selectedfirstRobot.id].yLocation, NULL);


		twoTypeRetrivalTaskObject.portSelection();
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 6, twoTypePortsVector[twoTypeRetrivalTaskObject.selectedPort.id].xLocation, NULL);
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 7, twoTypePortsVector[twoTypeRetrivalTaskObject.selectedPort.id].yLocation, NULL);
		

		twoTypeRetrivalTaskObject.secondRobotSelection();
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 8, twoTypeRetrivalTaskObject.selectedsecondRobotsVector.size(), NULL);

		
		double twoTypeCycleTime = twoTypeRetrivalTaskObject.twoTypeCycleTime(twoTypeWorksheet);
		worksheet_write_number(twoTypeWorksheet, retrivalTaskId + 1, 15, twoTypeCycleTime, NULL);
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 19, twoTypeCycleTime, NULL);

		twoTypeQueueOfBinRetrival.erase(twoTypeQueueOfBinRetrival.begin());
		twoTypeFinishedRetriveTaskVector.push_back(twoTypeRetrivalTaskObject);

		if (retrivalTaskId == iteration) {break;}
		retrivalTaskId++;


	}
	workbook_close(twoTypeWorkbook);
#endif

	
	//Both type
#if 1
	numOfFirstRobots = numOfFirstRobots_ ;
	numOfSecondRobots = numOfSecondRobots_;
	std::cout << "***********************Both type*******************************" ;

	retrivalTaskId = 0;
	while (!bothTypeQueueOfBinRetrival.empty()){

		if (bothTypeOneRoundOfRetrivalTask.size() == 0) {

			for (int i = 0; i < numOfFirstRobots; i++) {

				bothTypeOneRoundOfRetrivalTask.push_back(bothTypeQueueOfBinRetrival[0]);
				bothTypeQueueOfBinRetrival.erase(bothTypeQueueOfBinRetrival.begin());
			}

			std::sort(bothTypeOneRoundOfRetrivalTask.begin(), bothTypeOneRoundOfRetrivalTask.end(),
				[](const Autostore::bin& a, const Autostore::bin& b) {
					auto az = a.zLocation;
					auto bz = b.zLocation;

					return az < bz;
				});
		}


		std::cout << "\nRetriving " << bothTypeOneRoundOfRetrivalTask[0].locationName;


		bothTypeRetrivalTaskObject.reset();
		bothTypeRetrivalTaskObject.id = retrivalTaskId;
		bothTypeRetrivalTaskObject.binToRetrive = bothTypeBinsVector[bothTypeOneRoundOfRetrivalTask[0].binId]; 

		worksheet_write_string(bothTypeWorksheet, retrivalTaskId + 1, 0, bothTypeOneRoundOfRetrivalTask[0].locationName.c_str(), NULL);
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 1, bothTypeOneRoundOfRetrivalTask[0].xLocation, NULL);
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 2, bothTypeOneRoundOfRetrivalTask[0].yLocation, NULL);
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 3, bothTypeOneRoundOfRetrivalTask[0].zLocation, NULL);

		bool twoTypeCondition;
		if (bothTypeOneRoundOfRetrivalTask.size() <= (numOfFirstRobots - numOfSecondRobots)) { twoTypeCondition = false; }
		else { twoTypeCondition = true; }
		
		bothTypeRetrivalTaskObject.firstRobotSelection();
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 4, bothTypeFirstRobotsVector[bothTypeRetrivalTaskObject.selectedfirstRobot.id].xLocation, NULL);
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 5, bothTypeFirstRobotsVector[bothTypeRetrivalTaskObject.selectedfirstRobot.id].yLocation, NULL);



		bothTypeRetrivalTaskObject.portSelection();
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 7, bothTypePortsVector[bothTypeRetrivalTaskObject.selectedPort.id].xLocation, NULL);
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 8, bothTypePortsVector[bothTypeRetrivalTaskObject.selectedPort.id].yLocation, NULL);


		double bothTypeCycleTime{ 0.0 };

		if (twoTypeCondition) {

			bothTypeRetrivalTaskObject.secondRobotSelection();
			bothTypeCycleTime = bothTypeRetrivalTaskObject.bothTypeTwoTypeCycleTime(bothTypeWorksheet);
			worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 21, bothTypeCycleTime, NULL);

		}
		else
		{
			bothTypeCycleTime = bothTypeRetrivalTaskObject.bothTypeOneTypeCycleTime(bothTypeWorksheet);
			worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 20, bothTypeCycleTime, NULL);
		}
		
		
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 6, bothTypeRetrivalTaskObject.selectedsecondRobotsVector.size(), NULL);
		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 22, bothTypeCycleTime, NULL);

		worksheet_write_number(bothTypeWorksheet, retrivalTaskId + 1, 23, retrivalTaskId, NULL);
		
		bothTypeOneRoundOfRetrivalTask.erase(bothTypeOneRoundOfRetrivalTask.begin());
		bothTypeFinishedRetriveTaskVector.push_back(bothTypeRetrivalTaskObject);


		//shift chech
#if 1				
		bothTypeFirstRobotsVector[bothTypeRetrivalTaskObject.selectedfirstRobot.id].time =
			bothTypeFirstRobotsVector[bothTypeRetrivalTaskObject.selectedfirstRobot.id].time + bothTypeCycleTime; // change robot time

		bothTypeFirstRobotsVector[bothTypeRetrivalTaskObject.selectedfirstRobot.id].numOfTasks++;

		if (bothTypeFirstRobotsVector[bothTypeRetrivalTaskObject.selectedfirstRobot.id].time >= (shiftHours * 60 * 60)) {
			std::cout << "\n\n\n************************Shift Hours is Passed**************************\n\n\n";



			for (auto robot : bothTypeFirstRobotsVector) {
				std::cout << "Robot Id:" << robot.id << " Robot time: " << robot.time << " Robot Tasks: " << robot.numOfTasks << "\n";
			}

			std::cout << "total num of retrived Bin: " << bothTypeFinishedRetriveTaskVector.size()
				<< " throughput/hour: " << bothTypeFinishedRetriveTaskVector.size() / shiftHours;

			break;
		}

		if (retrivalTaskId % numOfFirstRobots == 1) {

			for (auto& robot : bothTypeFirstRobotsVector) {
				robot.isBusy = false;
			}
			//break;
		}
#endif



		if (retrivalTaskId == iteration) {break;}
		retrivalTaskId++;


	}
	workbook_close(bothTypeWorkbook);
#endif



	


	//error handeling
#if 0
	if (queueOfBinRetrival.empty()){
		for (auto &robot : firstRobotsVector) {
			std::cout << "Robot Id:" << robot.id << " Robot time: " << robot.time << "\n";
		}
		std::cout << "\n\nEmpty Retrival Queue!\n";
	}
	else{ std::cout << "\n\nRetrival Queue Not Empty Or Something Wrong!\n"; }
#endif







	return 0;
}

