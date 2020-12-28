#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "stdio.h"
#include <map>
#include <numeric>
#include <string>
#include <algorithm>
#include <set>
#include <io.h>
#include <direct.h>

using namespace std;

void deleteReturn(string &inputString)
{
	string::size_type position=inputString.find_last_not_of("\n\r");
	if(position != inputString.npos)
	{
		inputString.erase(position+1, inputString.size()-position);
	}
	position = inputString.find_first_not_of ("\n\r");
	if( position != inputString.npos )
	{
    	inputString.erase( 0 , position);
	}
	if(inputString=="\n" || inputString=="\r")
	{
		inputString = "";
	}
}
string expDesignSearch(string strQuery)
{
	ifstream nameStream;
	string result="",buffer;
	nameStream.open("parametersConverter.txt",ios::in);
	if(!nameStream)
 	{
 		cout<<"##Error: There is no file named ExpDesign.txt.##"<<endl;
 		system("PAUSE");
 		exit(1);
	}
	while(getline(nameStream,buffer,':'))
	{
		deleteReturn(buffer);
		if(buffer == strQuery)
		{
			getline(nameStream,buffer);
			result = buffer;
		} 
		else
		{
			getline(nameStream,buffer,'\n');
		}
	}
	nameStream.close();
	deleteReturn(result);
	return result;
}

int columnPlateOrderTitle(string titleQuery)
{
	string linePlateOrder, bufferLinePlateOrder;
	ifstream filePlateOrdercsv;
	//cout<<titleQuery<<endl;
	filePlateOrdercsv.open("List_for_Converter.csv",ios::in);
	getline(filePlateOrdercsv,linePlateOrder,'\n');
	istringstream streamPlateOrder(linePlateOrder);
	int count=0;
	while(getline(streamPlateOrder,bufferLinePlateOrder,','))
	{
		//cout<<titleQuery<<":::"<<bufferLinePlateOrder<<endl;
		if(bufferLinePlateOrder==titleQuery)
		{
			return count;
		}
		count++;
	}
	filePlateOrdercsv.close();
	cout<<"Error: There is no title, "<<titleQuery<<", in List_for_Converter.csv."<<endl;
	return -1;
}

int isNum(string testedString)
{
	stringstream streamTested(testedString);
	double bufferDouble;
	char bufferChar;
	
	if(!(streamTested>>bufferDouble))
	{
		return 0;
	}
	if(streamTested>>bufferChar)
	{
		return 0;
	}
	return 1;
}

void converter()
{
	_mkdir(".\\GM_converted_data");
	string bufferPath, steamBuffer, revisedPath = "";
	bufferPath = expDesignSearch("Path");
	istringstream streamPath(bufferPath);
	while(getline(streamPath,steamBuffer,'/'))
	{
		revisedPath = revisedPath + steamBuffer + "\\";
	}
	ifstream inputList("List_for_Converter.csv");
	ofstream outputFile("GM_converted_data\\Converted_data.csv");
	string bufferInputList,bufferInputListFileName;
	getline(inputList,bufferInputList,'\n');
	deleteReturn(bufferInputList);
	istringstream streamTitle(bufferInputList);
	int columnCount=0;
	while(getline(streamTitle,bufferInputListFileName,','))
	{
		deleteReturn(bufferInputListFileName);
		if(bufferInputListFileName != "")
		{
			if(columnCount==0)
			{
				outputFile<<bufferInputListFileName;
			}
			else
			{
				outputFile<<","<<bufferInputListFileName;
			}
		}
		columnCount++;
	}
	int signalColumn,signalFileRow,signalFileColumn;
	signalColumn=columnPlateOrderTitle(expDesignSearch("Title of Signal column"));
	signalFileRow=atoi(expDesignSearch("Coordinate of row in Signal file").c_str());
	signalFileColumn=atoi(expDesignSearch("Coordinate of column in Signal file").c_str());
	
	while(getline(inputList,bufferInputList,'\n'))
	{
		deleteReturn(bufferInputList);
		ifstream streamFile[columnCount];
		istringstream streamInputList(bufferInputList);
		for(int i=0;i<columnCount;i++)
		{
			getline(streamInputList,bufferInputListFileName,',');
			string revisedFileName=revisedPath+bufferInputListFileName;
			streamFile[i].open(revisedFileName.c_str(),ios::in);
			if(!streamFile[i])
			{
				cout<<"Warning: No file named "<<revisedFileName<<"!"<<endl;
			}
		}
		string rowFile[columnCount];
		for(int i=0;i<signalFileRow;i++)
		{
			getline(streamFile[signalColumn],rowFile[signalColumn],'\n');
			deleteReturn(rowFile[signalColumn]);
		}
		while(getline(streamFile[signalColumn],rowFile[signalColumn],'\n'))
		{
			string labelFile[columnCount];
			istringstream streamRow[columnCount];
			deleteReturn(rowFile[signalColumn]);
			streamRow[signalColumn].str(rowFile[signalColumn]);
			for(int i=0;i<columnCount;i++)
			{
				if(i != signalColumn)
				{
					getline(streamFile[i],rowFile[i],'\n');
					deleteReturn(rowFile[i]);
					streamRow[i].str(rowFile[i]);
				}
			}
			for(int i=0;i<signalFileColumn;i++)
			{
				getline(streamRow[signalColumn],labelFile[signalColumn],',');
				deleteReturn(labelFile[signalColumn]);
			}
			while(getline(streamRow[signalColumn],labelFile[signalColumn],','))
			{
				//deleteReturn(labelFile[signalColumn]);
				int outputSwitch=1;
				for(int i=0;i<columnCount;i++)
				{
					if(i != signalColumn)
					{
						if(!getline(streamRow[i],labelFile[i],','))
						{
							outputSwitch=0;
						}
						//deleteReturn(labelFile[i]);
					}
					if(labelFile[i] == "")
					{
						outputSwitch=0;
					}
				}
				if(outputSwitch==1)
				{
					outputFile<<endl;
					for(int i=0;i<columnCount;i++)
					{
						if(i==0)
						{
							outputFile<<labelFile[i];
						}
						else
						{
							outputFile<<","<<labelFile[i];
						}
					}
				}
			}
		} 
//		for(int i=0;i<columnCount;i++)
//		{
//			getline(streamFile[i],rowFile[i],'\n');
//		}
		
	}
	
	
	inputList.close();
	outputFile.close();
}

int main()
{
	converter();
	//dir("abc");
	remove("parametersConverter.txt");
	system("PAUSE");
	return 0;
}
