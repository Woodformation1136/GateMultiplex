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
#include <direct.h>

using namespace std;

vector<string> originalFileTitle;

void deleteReturn(string &inputString);

string expDesignSearch(string strQuery);

char charFilenamePlateOrder[]="PlateOrder.csv";

int columnPlateOrderTitle(string titleQuery);

int signalTypeColumn;

//int isNegative(string &queryString);

bool inVector(int intQuery, vector<int> &intReferece);

map<string, double> BGcutoffs;
map<string, string> BGHLs;
bool passBG(int &indexPlate, double &querySingal);
bool passNC(double &cutoffNC, double &querySingal, string &filterLH);

void generatePlateOrderWellOrderSignal();

/* 
float calculateCutoffByReference();
*/ 
double percentileCal(vector<double> &dataset, float &queryPercentage);
double calculateCutoffByReference(map<string, map<string, vector<double> > > &labelledSignal, int &indexPlate, map<int, map<string, int> > &npPlatenumLabel);

void deletePlateOrderColumn(vector<int> &indexColumn, string scLabel);

string searchPlateOrderRowColumn(int indexRow, int &indexColumn);


map<string,set<int> > groupPlatesIndex;
set<string> setStringRow;
void groupPlates();


int intNumInVector(vector<int> &inputVector, int queryInt);


vector<string> indexPlatePromoter;
string nbpPromoterParameters(const int &columnPromoter, const string PromoterName, const string groupName, map<int, map<string, int> > &tempNPBPlatenumLabel, const vector<int> &vectorIndexesPlates, vector<int> &vectorColumnParameters, const vector<int> &vectorCutoffParameters);


void NPBtree(map<int, map<string, int> > &tempNPBPlatenumLabel);


void remove_tmp_files();

int main()
{
	//cout<<"GG"<<endl;
	if(expDesignSearch("Output fold change files (Default = Off)") == "On" || expDesignSearch("Output PNE files (Default = Off)") == "On")
	{
		_mkdir(".\\GM_outputfiles");
		system("rd /s/q .\\GM_outputfiles");
		_mkdir(".\\GM_outputfiles");
	}
	_mkdir(".\\GM_tmpfiles_WellOrder");
	generatePlateOrderWellOrderSignal();
	signalTypeColumn=columnPlateOrderTitle("Signal_type");
	int plateNum, numeachplate, cutoffTech=1, cutoffBio=1;
	int countPlateOrder=0;
	ifstream countedPlateOrder("PlateOrder.csv");
	string rowCountedPlateOrder;
	getline(countedPlateOrder,rowCountedPlateOrder,'\n');
	while(getline(countedPlateOrder,rowCountedPlateOrder,'\n'))
	{
		deleteReturn(rowCountedPlateOrder);
		if(rowCountedPlateOrder != "")
		{
			countPlateOrder++;
		}
	}
	plateNum = countPlateOrder;
	countedPlateOrder.close();
	cout<<"There are total "<<plateNum<<" kinds of treatment conditions."<<endl;
	
	int ifBiorep=0;
	if(expDesignSearch("Bio-replicate column") != "")
	{
		ifBiorep=1;
	}
	
	if(expDesignSearch("Technical replicate cut-off (Default = 1)") != "")
	{
		cutoffTech = atoi(expDesignSearch("Technical replicate cut-off (Default = 1)").c_str());
	}
	cout<<"Technical replicate cut-off is set to "<<cutoffTech<<"."<<endl;
	if(expDesignSearch("Biological replicate cut-off (Default = 1)") != "")
	{
		cutoffBio = atoi(expDesignSearch("Biological replicate cut-off (Default = 1)").c_str());
	}
	cout<<"Biological replicate cut-off is set to "<<cutoffBio<<"."<<endl;
	
	double cutoffBackground, foldChange=1, lowerNCRange, upperNCRange, cutoffInternal=0;
//	if((expDesignSearch("Background cut-off (Default = 3)") != "") && (expDesignSearch("Background cut-off (Default = 3)") != " "))
//	{
//		cutoffBackground = atof(expDesignSearch("Background cut-off (Default = 3)").c_str());
//		cout<<"Background cut-off is set to "<<cutoffBackground<<"."<<endl;
//	}
//	else
//	{
//		cutoffBackground = 3;
//		cout<<"Background cut-off is set to "<<cutoffBackground<<" as default value."<<endl;
//	}
	if(expDesignSearch("Fold change cut-off (Default = 1)") != "")
	{
		foldChange = atof(expDesignSearch("Fold change cut-off (Default = 1)").c_str());
	}
	cout<<"Fold change is set to "<<foldChange<<"."<<endl;
//	if((expDesignSearch("Negative control lower bound percentage (Default = 25)") != "") && (expDesignSearch("Negative control lower bound percentage (Default = 25)") != " "))
//	{
//		lowerNCRange = atof(expDesignSearch("Negative control lower bound percentage (Default = 25)").c_str());
//		cout<<"Negative control lower bound percentage is set to "<<lowerNCRange<<"."<<endl;
//	}
//	else
//	{
//		lowerNCRange = 25;
//		cout<<"Negative control lower bound percentage is set to "<<lowerNCRange<<" as default value."<<endl;
//	}
//	if((expDesignSearch("Negative control upper bound percentage (Default = 75)") != "") && (expDesignSearch("Negative control upper bound percentage (Default = 75)") != " "))
//	{
//		upperNCRange = atof(expDesignSearch("Negative control upper bound percentage (Default = 75)").c_str());
//		cout<<"Negative control upper bound percentage is set to "<<upperNCRange<<"."<<endl;
//	}
//	else
//	{
//		upperNCRange = 75;
//		cout<<"Negative control upper bound percentage is set to "<<upperNCRange<<" as default value."<<endl;
//	}
//
//	if(lowerNCRange > upperNCRange)
//	{
//		cout<<"##Error: lower bound cannot be higher than upper bound.##"<<endl;
//		exit(1);
//	}
//	
	if(expDesignSearch("Internal control cut-off") != "")
	{
		cutoffInternal = atof(expDesignSearch("Internal control cut-off").c_str());
		cout<<"Internal cut-off is set to "<<cutoffInternal<<"."<<endl;
	}
	
	string switchFoldChangeFile,titleSC,nameSC,filterLH="Higher or equal",scLH,path,BGcutoff,BGHL,BGbuffer,BGHLbuffer,fixedHL,fixedHLbuffer,signalTypeRow,signalTypeBuffer;
	switchFoldChangeFile = expDesignSearch("Output fold change files (Default = Off)");
	nameSC = expDesignSearch("Name of internal control group (Optional)");
	filterLH = expDesignSearch("Fold change cut-off (Default = 1) Higher or Lower");
	scLH = expDesignSearch("Internal control cut-off Higher or Lower");
	path = expDesignSearch("Path");
	titleSC=expDesignSearch("Title including internal control group");
	
	map<string, string> fixedRefHLs;
	signalTypeRow = expDesignSearch("Signal_type");
	fixedHL = expDesignSearch("Fixed reference higher or lower");
	BGcutoff = expDesignSearch("Background noise cut-off");
	BGHL = expDesignSearch("Background noise cut-off HL");
	istringstream streamSignalType(signalTypeRow);
	istringstream streamfixedHL(fixedHL);
	istringstream streamBGcutoff(BGcutoff);
	istringstream streamBGHL(BGHL);
	while(getline(streamSignalType, signalTypeBuffer, ','))
	{
		getline(streamBGcutoff, BGbuffer, ',');
		BGcutoffs[signalTypeBuffer] = atof(BGbuffer.c_str());
		getline(streamBGHL, BGHLbuffer, ',');
		if(BGbuffer != "")
		{
			BGHLs[signalTypeBuffer] = BGHLbuffer;
		}
		else
		{
			BGHLs[signalTypeBuffer] = "Pass";
		}
		if(expDesignSearch("Reference group chosen mode") == "C")
		{
			getline(streamfixedHL, fixedHLbuffer, ',');
			fixedRefHLs[signalTypeBuffer] = fixedHLbuffer;
		}
	}
	
	string lineSignal,lineWellOrder,linePlateOrder,data,tmps,label,bio;
	ifstream fileSignalcsv, filePlateOrdercsv,filePlateOrderForFC, fileWellOrdercsv;
	fstream testfile;

	
	string bufferLinePlateOrder;
	int countColumnBatch,countColumnPlate,countColumnPromoter;
	countColumnBatch = columnPlateOrderTitle(";");
	countColumnPromoter = columnPlateOrderTitle(";;");
	countColumnPlate = columnPlateOrderTitle(titleSC);
	map<int, map<string, int> > npPlatenumLabel;
	set<string> setAllNameGroup;
	
	
	//Internal control calculation
	if(nameSC != "")
	{
		fileSignalcsv.open("Signal.csv",ios::in);
		if(!fileSignalcsv)
		{
			cout<<"##Error: There is no file named Signal.csv.##"<<endl;
			exit(1);
		}
		getline(fileSignalcsv,lineSignal,'\n');
		
		filePlateOrdercsv.open(charFilenamePlateOrder,ios::in);
		getline(filePlateOrdercsv,linePlateOrder,'\n');
		
		filePlateOrderForFC.open(charFilenamePlateOrder,ios::in);
		getline(filePlateOrderForFC,linePlateOrder,'\n');
		for(int indexPlate=0;indexPlate<plateNum;indexPlate++) //read all plates one by one
		{
			getline(filePlateOrdercsv,linePlateOrder,'\n');
			deleteReturn(linePlateOrder);
			istringstream streamPlateOrderBatch(linePlateOrder);
			for(int i=0;i<=countColumnBatch;i++)
			{
				getline(streamPlateOrderBatch,bufferLinePlateOrder,',');
				if(i==countColumnBatch)
				{
					break;
				}
			}
			string nameWellOrder;
			nameWellOrder = "GM_tmpfiles_WellOrder\\WellOrder_" + bufferLinePlateOrder + ".csv";
			fileWellOrdercsv.open(nameWellOrder.c_str(),ios::in);
			if(!fileWellOrdercsv)
			{
				cout<<"##Error: There is no file named "<<nameWellOrder<<".##"<<endl;
				exit(1);
			}
			getline(fileWellOrdercsv,lineWellOrder,'\n');
			deleteReturn(lineWellOrder);
			map<string, map<string, vector<double> > > labelledSignal;
			map<string, set<string> > setLabelledBio;
			set<string> setNameGroup;
			while(getline(fileWellOrdercsv,lineWellOrder,'\n'))
			{
				deleteReturn(lineWellOrder);
				getline(fileSignalcsv,lineSignal,'\n');
				istringstream streamSignal(lineSignal),streamWellOrder(lineWellOrder);
				getline(streamWellOrder,label,',');
				getline(streamSignal,data,',');
				getline(streamWellOrder,bio,',');
	//			cout<<label<<":::"<<bio<<endl;
				if(label != "Negative" && label != "" && data !="") //labelling the experimental signals
				{
					labelledSignal[bio][label].push_back(atof(data.c_str()));
					setNameGroup.insert(label);
//					setAllNameGroup.insert(label);
					setLabelledBio[label].insert(bio);
				}
				else if(label=="Negative" && data !="") //labelling the negative control or blank signals with "Negative" or "Blank" and Bio[0]
				{
					labelledSignal["Negative"]["Negative"].push_back(atof(data.c_str()));
				}
			}
			fileWellOrdercsv.close();
			
			istringstream streamPlateOrderPlate(linePlateOrder);
			for(int i=0;i<=countColumnPlate;i++)
			{
				getline(streamPlateOrderPlate,bufferLinePlateOrder,',');
				if(i==countColumnPlate)
				{
					break;
				}
			}
			if(bufferLinePlateOrder==nameSC) //Condition for SC plates?
			{
				for (set<string>::iterator itLabel = setNameGroup.begin(); itLabel != setNameGroup.end(); ++itLabel) //call all the Label strings from a set
				{
					npPlatenumLabel[indexPlate][*itLabel]=0;
		        	for (set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); ++itBio) //call all the Bio integers from a set
					{
						for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
						{
							if(labelledSignal[*itBio][*itLabel][i]<cutoffInternal && scLH == "Higher or equal")
							{
								npPlatenumLabel[indexPlate][*itLabel]=2;
								break;
							}
							else if(labelledSignal[*itBio][*itLabel][i]<=cutoffInternal && scLH == "Higher")
							{
								npPlatenumLabel[indexPlate][*itLabel]=2;
								break;
							}
							else if(labelledSignal[*itBio][*itLabel][i]>cutoffInternal && scLH == "Lower or equal")
							{
								npPlatenumLabel[indexPlate][*itLabel]=2;
								break;
							}
							else if(labelledSignal[*itBio][*itLabel][i]>=cutoffInternal && scLH == "Lower")
							{
								npPlatenumLabel[indexPlate][*itLabel]=2;
								break;
							}
						}
						if(npPlatenumLabel[indexPlate][*itLabel]==2)
						{
							break;
						}
					}
		    	}
		    	int columnPlateOrder;
				columnPlateOrder = columnPlateOrderTitle(titleSC);
				istringstream streamlinePlateOrder(linePlateOrder);
				string stringCompared="";
				int countLine = 0;
				while(getline(streamlinePlateOrder,bufferLinePlateOrder,','))
				{
					if(countLine!=columnPlateOrder && countLine!=countColumnBatch && countLine!=countColumnPromoter)
					{
						stringCompared=stringCompared+bufferLinePlateOrder+",";
					}
					countLine++;
				}
				
				ifstream filePlateSearch;
				filePlateSearch.open(charFilenamePlateOrder,ios::in);
				getline(filePlateSearch,linePlateOrder,'\n');
				int indexPlateSearch = 0;
				while(getline(filePlateSearch,linePlateOrder,'\n'))
				{
					deleteReturn(linePlateOrder);
					istringstream streamlinePlateOrder(linePlateOrder);
					string stringComparing="";
					int columnLineSearch=0;
					while(getline(streamlinePlateOrder,bufferLinePlateOrder,','))
					{
						if(columnLineSearch!=columnPlateOrder && columnLineSearch!=countColumnBatch && columnLineSearch!=countColumnPromoter)
						{
							stringComparing=stringComparing+bufferLinePlateOrder+",";
						}
						columnLineSearch++;
					}
					istringstream streamPlateOrderSearchedPlate(linePlateOrder);
					for(int i=0;i<=countColumnPlate;i++)
					{
						getline(streamPlateOrderSearchedPlate,bufferLinePlateOrder,',');
						if(i==countColumnPlate)
						{
							break;
						}
					}
					if((stringComparing==stringCompared) && (bufferLinePlateOrder != nameSC))
					{
						for(map<string,int>::iterator itNP=npPlatenumLabel[indexPlate].begin();itNP != npPlatenumLabel[indexPlate].end();++itNP)
						{
							npPlatenumLabel[indexPlateSearch][itNP->first] = npPlatenumLabel[indexPlate][itNP->first];
						}
					}
					indexPlateSearch++;
				}
				
				filePlateSearch.close();
			}
		}
		filePlateOrdercsv.close();
		fileSignalcsv.close();
		filePlateOrderForFC.close();
	}
	fileSignalcsv.open("Signal.csv",ios::in);
	if(!fileSignalcsv)
	{
		cout<<"##Error: There is no file named Signal.csv.##"<<endl;
		exit(1);
	}
	getline(fileSignalcsv,lineSignal,'\n');
	
	filePlateOrdercsv.open(charFilenamePlateOrder,ios::in);
	getline(filePlateOrdercsv,linePlateOrder,'\n');
	
	filePlateOrderForFC.open(charFilenamePlateOrder,ios::in);
	getline(filePlateOrderForFC,linePlateOrder,'\n');
	
	//
	for(int indexPlate=0;indexPlate<plateNum;indexPlate++) //read all plates one by one
	{
		getline(filePlateOrdercsv,linePlateOrder,'\n');
		deleteReturn(linePlateOrder);
		istringstream streamPlateOrderBatch(linePlateOrder);
		for(int i=0;i<=countColumnBatch;i++)
		{
			getline(streamPlateOrderBatch,bufferLinePlateOrder,',');
			if(i==countColumnBatch)
			{
				break;
			}
		}
		string nameWellOrder;
		nameWellOrder = "GM_tmpfiles_WellOrder\\WellOrder_" + bufferLinePlateOrder + ".csv";
		fileWellOrdercsv.open(nameWellOrder.c_str(),ios::in);
		if(!fileWellOrdercsv)
		{
			cout<<"##Error: There is no file named "<<nameWellOrder<<".##"<<endl;
			exit(1);
		}
		getline(fileWellOrdercsv,lineWellOrder,'\n');
		deleteReturn(lineWellOrder);
		map<string, map<string, vector<double> > > labelledSignal;
		map<string, set<string> > setLabelledBio;
		set<string> setNameGroup;
		while(getline(fileWellOrdercsv,lineWellOrder,'\n'))
		{
			deleteReturn(lineWellOrder);
			getline(fileSignalcsv,lineSignal,'\n');
			istringstream streamSignal(lineSignal),streamWellOrder(lineWellOrder);
			getline(streamWellOrder,label,',');
			getline(streamSignal,data,',');
			getline(streamWellOrder,bio,',');
//			cout<<label<<":::"<<bio<<endl;
			if(label != "Negative" && label != "" && data !="") //labelling the experimental signals
			{
				labelledSignal[bio][label].push_back(atof(data.c_str()));
				setNameGroup.insert(label);
				setAllNameGroup.insert(label);
				setLabelledBio[label].insert(bio);
			}
			else if(label=="Negative" && data !="") //labelling the negative control or blank signals with "Negative" or "Blank" and Bio[0]
			{
				labelledSignal["Negative"]["Negative"].push_back(atof(data.c_str()));
			}
		}
		fileWellOrdercsv.close();
		
//		if(labelledSignal["Negative"]["Negative"].size()==0)
//		{
//			cout<<"##Error: In plate "<<indexPlate<<": no signal is labeled 'Negative' in "<<nameWellOrder<<".##"<<endl;
//		}
		
		istringstream streamPlateOrderPlate(linePlateOrder);
		for(int i=0;i<=countColumnPlate;i++)
		{
			getline(streamPlateOrderPlate,bufferLinePlateOrder,',');
			if(i==countColumnPlate)
			{
				break;
			}
		}
		
		if(bufferLinePlateOrder!=nameSC || nameSC == "")
		{
			double cutoffNC=calculateCutoffByReference(labelledSignal,indexPlate,npPlatenumLabel);
//			cout<<searchPlateOrderRowColumn(indexPlate,countColumnBatch)<<":::"<<cutoffNC<<endl;
			if(switchFoldChangeFile=="On") //output Fold change when switch is "On"
			{
				istringstream streamPlateOrderOutputName(linePlateOrder);
				string bufferLineOutputName, filenameFC="GM_outputfiles\\Foldchange_";
				for(int i=0;i<countColumnBatch;i++)
				{
					getline(streamPlateOrderOutputName,bufferLineOutputName,',');
					filenameFC = filenameFC + bufferLineOutputName + ";";
				}
//				cout<<"Reference cutoff of "<<filenameFC<<":"<<cutoffNC<<endl;
				filenameFC=filenameFC+".csv";
				
				ofstream filestreamFoldChange(filenameFC.c_str());
				
				if(!filestreamFoldChange)
				{
					cout<<"##WARNING: File name, "<<filenameFC<<", contains illegal characters.##"<<endl;
				}
				if(ifBiorep == 1)
				{
					filestreamFoldChange<<",Bio"<<endl;
					for(set<string>::iterator itLabel = setNameGroup.begin(); itLabel != setNameGroup.end(); ++itLabel) //call all the Label strings from a set
					{
						filestreamFoldChange<<*itLabel;
						if(npPlatenumLabel[indexPlate][*itLabel] != 2)
						{
				        	for(set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); ++itBio) //call all the Bio integers from a set
							{
								filestreamFoldChange<<","<<*itBio;
								for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
								{
									filestreamFoldChange<<","<<labelledSignal[*itBio][*itLabel][i]/cutoffNC;
								}
								filestreamFoldChange<<endl;
							}
						}
						else
						{
							for(set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); ++itBio) //call all the Bio integers from a set
							{
								filestreamFoldChange<<","<<*itBio;
								for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
								{
									filestreamFoldChange<<",E";
								}
								filestreamFoldChange<<endl;
							}
						}
			    	}
				}
				else
				{
//					filestreamFoldChange<<",Bio"<<endl;
					for(set<string>::iterator itLabel = setNameGroup.begin(); itLabel != setNameGroup.end(); ++itLabel) //call all the Label strings from a set
					{
						filestreamFoldChange<<*itLabel;
						if(npPlatenumLabel[indexPlate][*itLabel] != 2)
						{
				        	for(set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); ++itBio) //call all the Bio integers from a set
							{
//								filestreamFoldChange<<","<<*itBio;
								for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
								{
									filestreamFoldChange<<","<<labelledSignal[*itBio][*itLabel][i]/cutoffNC;
								}
								filestreamFoldChange<<endl;
							}
						}
						else
						{
							for(set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); ++itBio) //call all the Bio integers from a set
							{
//								filestreamFoldChange<<","<<*itBio;
								for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
								{
									filestreamFoldChange<<",E";
								}
								filestreamFoldChange<<endl;
							}
						}
			    	}
				}
		    	filestreamFoldChange.close();
	    	}
	    	if(expDesignSearch("Reference group chosen mode") != "C")
	    	{
	    		cutoffNC = cutoffNC*foldChange;
			}
			else
			{
				filterLH = fixedRefHLs[searchPlateOrderRowColumn(indexPlate, signalTypeColumn)];
			}
//			cout<<searchPlateOrderRowColumn(indexPlate, signalTypeColumn)<<endl;
			for (set<string>::iterator itLabel = setNameGroup.begin(); itLabel != setNameGroup.end(); ++itLabel) //call all the Label strings from a set
			{
				vector<int> vectorBioNP;
	        	for (set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); ++itBio) //call all the Bio integers from a set
				{
					vector<int> vectorTechNP;
					for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
					{
						if(passNC(cutoffNC, labelledSignal[*itBio][*itLabel][i], filterLH) && passBG(indexPlate, labelledSignal[*itBio][*itLabel][i]))
						{
							vectorTechNP.push_back(1);
						}
						else
						{
							vectorTechNP.push_back(0);
						}
					}
					if(accumulate(vectorTechNP.begin(),vectorTechNP.end(),0)>=cutoffTech)
					{
						vectorBioNP.push_back(1);
					}
					else
					{
						vectorBioNP.push_back(0);
					}
				}
				if(accumulate(vectorBioNP.begin(),vectorBioNP.end(),0)>=cutoffBio)
				{
					npPlatenumLabel[indexPlate][*itLabel] = npPlatenumLabel[indexPlate][*itLabel] + 1;
				}
				else
				{
					npPlatenumLabel[indexPlate][*itLabel] = npPlatenumLabel[indexPlate][*itLabel] + 0;
				}
	    	}
    	}
	}
	filePlateOrdercsv.close();
	fileSignalcsv.close();
	filePlateOrderForFC.close();
	
	//Labelling "BAD Negative" in integer 2 and "BAD Positive" in integer 3 onto npPlatenumLabel[indexPlate][Label]
//	int columnPlateOrder;
//	columnPlateOrder = columnPlateOrderTitle(titleSC);
//	filePlateOrdercsv.open(charFilenamePlateOrder,ios::in);
//	getline(filePlateOrdercsv,linePlateOrder,'\n');
//	if(nameSC != "")
//	{
//		for(int indexPlate=0;indexPlate<plateNum;indexPlate++)
//		{
//			getline(filePlateOrdercsv,linePlateOrder,'\n');
//			deleteReturn(linePlateOrder);
//			istringstream streamPlateOrderPlate(linePlateOrder);
//			for(int i=0;i<=countColumnPlate;i++)
//			{
//				getline(streamPlateOrderPlate,bufferLinePlateOrder,',');
//				if(i==countColumnPlate)
//				{
//					break;
//				}
//			}
//			if(bufferLinePlateOrder == nameSC)
//			{
//				istringstream streamlinePlateOrder(linePlateOrder);
//				string stringCompared="";
//				int countLine = 0;
//				while(getline(streamlinePlateOrder,bufferLinePlateOrder,','))
//				{
//					if(countLine!=columnPlateOrder)
//					{
//						stringCompared=stringCompared+bufferLinePlateOrder+",";
//					}
//					countLine++;
//				}
//				
//				ifstream filePlateSearch;
//				filePlateSearch.open(charFilenamePlateOrder,ios::in);
//				getline(filePlateSearch,linePlateOrder,'\n');
//				int indexPlateSearch = 0;
//				while(getline(filePlateSearch,linePlateOrder,'\n'))
//				{
//					deleteReturn(linePlateOrder);
//					istringstream streamlinePlateOrder(linePlateOrder);
//					string stringComparing="";
//					int columnLineSearch=0;
//					while(getline(streamlinePlateOrder,bufferLinePlateOrder,','))
//					{
//						if(columnLineSearch!=columnPlateOrder)
//						{
//							stringComparing=stringComparing+bufferLinePlateOrder+",";
//						}
//						columnLineSearch++;
//					}
//					istringstream streamPlateOrderSearchedPlate(linePlateOrder);
//					for(int i=0;i<=countColumnPlate;i++)
//					{
//						getline(streamPlateOrderSearchedPlate,bufferLinePlateOrder,',');
//						if(i==countColumnPlate)
//						{
//							break;
//						}
//					}
//					if((stringComparing==stringCompared) && (bufferLinePlateOrder != nameSC))
//					{
//						for(map<string,int>::iterator itNP=npPlatenumLabel[indexPlateSearch].begin();itNP != npPlatenumLabel[indexPlateSearch].end();itNP++)
//						{
//							npPlatenumLabel[indexPlateSearch][itNP->first] = npPlatenumLabel[indexPlateSearch][itNP->first] + npPlatenumLabel[indexPlate][itNP->first];
//						}
//					}
//					indexPlateSearch++;
//				}
//				
//				filePlateSearch.close();
//			}
//		}	
//	}

//	filePlateOrdercsv.close();
	//Lablling all plates with PlateOrder.csv
	if(expDesignSearch("Output PNE files (Default = Off)") == "On")
	{
		
		filePlateOrdercsv.open(charFilenamePlateOrder,ios::in);
		getline(filePlateOrdercsv,linePlateOrder,'\n');
		for(int indexPlate=0;indexPlate<plateNum;indexPlate++)
		{
			getline(filePlateOrdercsv,linePlateOrder,'\n');
			istringstream streamPlateOrderPlate(linePlateOrder);
			for(int i=0;i<=countColumnPlate;i++)
			{
				getline(streamPlateOrderPlate,bufferLinePlateOrder,',');
				if(i==countColumnPlate)
				{
					break;
				}
			}
		
			if(bufferLinePlateOrder!=nameSC || nameSC == "")
			{
				istringstream streamPlateOrderOutputName(linePlateOrder);
				string bufferLineOutputName, filenameNPB="GM_outputfiles\\PNE_";
				for(int i=0;i<countColumnBatch;i++)
				{
					getline(streamPlateOrderOutputName,bufferLineOutputName,',');
					filenameNPB = filenameNPB + bufferLineOutputName + ";";
				}
				filenameNPB=filenameNPB+".csv";
//				string filenameNPB="GM_outputfiles\\PNE_" + searchPlateOrderRowColumn(indexPlate,countColumnBatch);
		//		filenameNPB=filenameBuffer;
		//		getline(filePlateOrdercsv,linePlateOrder,'\n');
		//		deleteReturn(linePlateOrder);
		//		istringstream streamFilenameNPB(linePlateOrder);
		//		while(getline(streamFilenameNPB,bufferLinePlateOrder,','))
		//		{
		//			filenameNPB=filenameNPB+"_"+bufferLinePlateOrder;
		//		}
				
//				filenameNPB=filenameNPB+".csv";
				//cout<<filenameNPB<<endl;
				ofstream filestreamNPB(filenameNPB.c_str());
				
				if(!filestreamNPB)
				{
					cout<<"##WARNING: File name, "<<filenameNPB<<", contains illegal characters for this operating system.##"<<endl;
				}
				
				for(map<string,int>::iterator itNP=npPlatenumLabel[indexPlate].begin();itNP != npPlatenumLabel[indexPlate].end();++itNP)
				{
					//Output N/P/B files for all groups in each plate
					if((itNP->second)==0)
					{
						filestreamNPB<<itNP->first<<",N"<<endl;
					}
					else if((itNP->second)==1)
					{
						filestreamNPB<<itNP->first<<",P"<<endl;
					}
					else if((itNP->second)>1)
					{
						filestreamNPB<<itNP->first<<",E"<<endl;
					}
					else
					{
						filestreamNPB<<itNP->first<<",BUG!!!!!!"<<endl;
					}
				}
				filestreamNPB.close();
			}
		}
		filePlateOrdercsv.close();
	}
	if(expDesignSearch("Output result files (Default = On)") == "On")
	{
		NPBtree(npPlatenumLabel);
	}
	
	//delete temporary files
	//remove("tmp_PlateOrder.txt");
//	if(namePromoter=="")
//	{
//		remove("tmp_PlateOrder.csv");
//	}
	remove_tmp_files();
	
    cout<<"Finish running!"<<endl;
	system("PAUSE");
	return 0;
}

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
	nameStream.open("ExpDesign.txt",ios::in);
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
	//cout<<"GG"<<endl;
	string linePlateOrder, bufferLinePlateOrder;
	ifstream filePlateOrdercsv;
	filePlateOrdercsv.open(charFilenamePlateOrder,ios::in);
	getline(filePlateOrdercsv,linePlateOrder,'\n');
	deleteReturn(linePlateOrder);
	istringstream streamPlateOrder(linePlateOrder);
	int count=0;
	while(getline(streamPlateOrder,bufferLinePlateOrder,','))
	{
		if(bufferLinePlateOrder==titleQuery)
		{
			return count;
		}
		count++;
	}
	filePlateOrdercsv.close();
	if(titleQuery != "")
	{
		cout<<"Error: There is no title, "<<titleQuery<<", in PlateOrder.csv."<<endl;
	}
	//deleteReturn(titleQuery);
	//exit(1);
	return -1;
}

//int isNegative(string &queryString)
//{
//	string::size_type position;
//	position = queryString.find("Negative");
//	if(position == queryString.npos)
//	{
//		return 0;
//	}
//	else
//	{
//		return 1;
//	}
//}

bool inVector(int intQuery, vector<int> &intReferece)
{
	vector<int>::iterator it;
	it=find(intReferece.begin(),intReferece.end(),intQuery);
	if (it!=intReferece.end())
	{
		return true;
	}
	else
	{
		return false;
	}

}


void generatePlateOrderWellOrderSignal()
{
	string labelColumn;
	string stringParameters;
	stringParameters = expDesignSearch("Parameters (Optional)");
	istringstream streamParameters(stringParameters);
	vector<int> vectorParameters;
	if(stringParameters != "")
	{
		while(getline(streamParameters, labelColumn, ','))
		{
			if(labelColumn != "Signal_type")
			{
				vectorParameters.push_back(atoi(labelColumn.c_str()));
			}
			else
			{
				vectorParameters.push_back(-1);
			}
		}		
	}
	
	string stringTagnameColumn;
	stringTagnameColumn = expDesignSearch("TagName column");
	vector<int> vectorIndexTagnameColumn,vectorRevisedParameters;
	if(stringTagnameColumn != "")
	{
		istringstream streamTagNameColumn(stringTagnameColumn);
		string labelColumn;
		while(getline(streamTagNameColumn, labelColumn, ','))
		{
			vectorIndexTagnameColumn.push_back(atoi(labelColumn.c_str()));		
			if(inVector(atoi(labelColumn.c_str()),vectorParameters))
			{
				vectorRevisedParameters.push_back(vectorIndexTagnameColumn.size()-1);
			}
		}
	}
	if(stringParameters != "")
	{
		if(inVector(-1, vectorParameters))
		{
			vectorRevisedParameters.push_back(vectorIndexTagnameColumn.size());
		}
	}
	string stringSamplenameColumn;
	stringSamplenameColumn = expDesignSearch("SampleName column");
	istringstream streamSamplenameColumn(stringSamplenameColumn);
	vector<int> vectorIndexSamplenameColumn;
	while(getline(streamSamplenameColumn, labelColumn, ','))
	{
		vectorIndexSamplenameColumn.push_back(atoi(labelColumn.c_str()));		
	}
	
	string stringSignalColumn;
	stringSignalColumn = expDesignSearch("Signal column");
	istringstream streamSignalColumn(stringSignalColumn);
	vector<int> vectorIndexSignalColumn;
	while(getline(streamSignalColumn, labelColumn, ','))
	{
		vectorIndexSignalColumn.push_back(atoi(labelColumn.c_str()));
	}
	
	int biorepColumn=-1;
	if(expDesignSearch("Bio-replicate column") != "")
	{
		biorepColumn = atoi(expDesignSearch("Bio-replicate column").c_str());
	}
	
	string bufferPath, steamBuffer, revisedPath = "", revisedCommand;
	bufferPath = expDesignSearch("Path");
	istringstream streamPath(bufferPath);
	while(getline(streamPath,steamBuffer,'/'))
	{
		revisedPath = revisedPath + steamBuffer + "\\";
	}
	revisedCommand = "dir \"" + revisedPath + "\" /b > tmp_list.txt";
	//cout<<revisedCommand<<endl;
	system(revisedCommand.c_str());
	ifstream listFilename("tmp_list.txt");
	string filename;
	
	ofstream csvSignal("Signal.csv"), csvPlateOrder("PlateOrder.csv");
	int fileCount = 0;
	
	vector<string> vectorLabelRef;
	string stringLabelRef,modeRef;
	modeRef = expDesignSearch("Reference group chosen mode");
	stringLabelRef = expDesignSearch("Labels of reference group");
	istringstream streamLabelRef(stringLabelRef);
	
	if(stringLabelRef != "")
	{
		while(getline(streamLabelRef, labelColumn, ','))
		{
			vectorLabelRef.push_back(labelColumn);
		}	
	}
//	else
//	{
//		vectorLabelRef.push_back("");
//	}
	map<string, int> BatchNameCount;
	int indexBatch;
	while(getline(listFilename, filename, '\n'))
	{
		string pathFilename;
		pathFilename = revisedPath + filename;
		ifstream streamFilename(pathFilename.c_str());
		string rowFile;
		map<vector<string>, map<vector<string>, vector<float> > > labelledSignals;
		
		getline(streamFilename, rowFile, '\n');
		deleteReturn(rowFile);
		if(fileCount == 0)
		{
			istringstream row(rowFile);
			string singleTitle;
			while(getline(row, singleTitle, ','))
			{
				originalFileTitle.push_back(singleTitle);
			}
			csvSignal<<"Signal"<<endl;
		}
		istringstream row(rowFile);
		string singleLabel;
		int countColumn = 0;
		map<int, string> labelsSignalColumn;
		while(getline(row, singleLabel, ','))
		{
			if(inVector(countColumn, vectorIndexSignalColumn))
			{
				labelsSignalColumn[countColumn] = singleLabel;
			}
			else if((inVector(countColumn, vectorIndexTagnameColumn)) && fileCount == 0)
			{
				csvPlateOrder<<singleLabel<<",";
			}
			countColumn++;
		}
		
		if(fileCount == 0)
		{
			csvPlateOrder<<"Signal_type,;,;;"<<endl;
		}
//		int row_count=0;
		while(getline(streamFilename, rowFile, '\n'))
		{
			deleteReturn(rowFile);
			istringstream streamRow(rowFile);
			vector<string> labelsTagname, labelsSamplename;
			vector<string> vectorRow;
			while(getline(streamRow, singleLabel, ','))
			{
				vectorRow.push_back(singleLabel);
			}
			for(int i=0;i<vectorIndexTagnameColumn.size();i++)
			{
				labelsTagname.push_back(vectorRow[vectorIndexTagnameColumn[i]]);
			}
			for(int i=0;i<vectorIndexSamplenameColumn.size();i++)
			{
				labelsSamplename.push_back(vectorRow[vectorIndexSamplenameColumn[i]]);
			}
			if(biorepColumn != -1)
			{
				labelsSamplename.push_back(vectorRow[biorepColumn]);
			}
			for(int i=0;i<vectorIndexSignalColumn.size();i++)
			{
				labelsTagname.push_back(labelsSignalColumn[vectorIndexSignalColumn[i]]);
				labelledSignals[labelsTagname][labelsSamplename].push_back(atof(vectorRow[vectorIndexSignalColumn[i]].c_str()));
				labelsTagname.pop_back();
			}
//			row_count++;
		}
		for(map<vector<string>, map<vector<string>, vector<float> > >::iterator itLabelledSignals=labelledSignals.begin();itLabelledSignals!=labelledSignals.end();++itLabelledSignals)
		{
			string labelBatch="",labelPromoter="";
			for(int i=0;i<(itLabelledSignals->first).size();i++)
			{
//				csvPlateOrder<<(itLabelledSignals->first)[i];
//				csvPlateOrder<<",";
				labelBatch = labelBatch + (itLabelledSignals->first)[i] + ";";
				int ifParameter=0;
				for(int j=0;j<vectorRevisedParameters.size();j++)
				{
					if(i == vectorRevisedParameters[j])
					{
						ifParameter=1;
						break;
					}
				}
				if(ifParameter==0)
				{
					labelPromoter = labelPromoter + (itLabelledSignals->first)[i] + ";";
				}
			}
			
			string filenameWellOrder;
			stringstream streamFilenameWellOrder;
			ofstream csvWellOrder;
			if(BatchNameCount[labelBatch] > 0)
			{
				streamFilenameWellOrder<<"GM_tmpfiles_WellOrder\\WellOrder_"<<BatchNameCount[labelBatch]<<".csv";
				filenameWellOrder = streamFilenameWellOrder.str();
//				filenameWellOrder = "GM_tmpfiles_WellOrder\\WellOrder_" + to_string(BatchNameCount[labelBatch]) + ".csv";
				csvWellOrder.open(filenameWellOrder.c_str(), ios::out | ios::app);
			}
			else
			{
				indexBatch++;
				BatchNameCount[labelBatch]=indexBatch;
				streamFilenameWellOrder<<"GM_tmpfiles_WellOrder\\WellOrder_"<<BatchNameCount[labelBatch]<<".csv";
				filenameWellOrder = streamFilenameWellOrder.str();
//				filenameWellOrder = "GM_tmpfiles_WellOrder\\WellOrder_" + to_string(BatchNameCount[labelBatch]) + ".csv";
				for(int i=0;i<(itLabelledSignals->first).size();i++)
				{
					csvPlateOrder<<(itLabelledSignals->first)[i];
					csvPlateOrder<<",";
				}
				csvPlateOrder<<BatchNameCount[labelBatch]<<","<<labelPromoter<<endl;
				csvWellOrder.open(filenameWellOrder.c_str(), ios::out | ios::trunc);
				csvWellOrder<<"Group,Bio"<<endl;
				indexPlatePromoter.push_back(labelPromoter);
			}
			for(map<vector<string>, vector<float> >::iterator itSamplenameSignals=(itLabelledSignals->second).begin();itSamplenameSignals!=(itLabelledSignals->second).end();++itSamplenameSignals)
			{
				string rowWellOrder;
				if(modeRef=="A")
				{
					int ifNegative=1;
					rowWellOrder = (itSamplenameSignals->first)[0];
					if((itSamplenameSignals->first)[0] != vectorLabelRef[0] && vectorLabelRef[0] != "")
					{
						ifNegative=0;
					}
					//cout<<(itSamplenameSignals->first).size()<<endl;
					for(int i=1;i<(itSamplenameSignals->first).size();i++)
					{
						if(i == vectorIndexSamplenameColumn.size())
						{
							rowWellOrder = rowWellOrder + "," + (itSamplenameSignals->first)[i];
							break;
						}
						if((itSamplenameSignals->first)[i] != vectorLabelRef[i] && vectorLabelRef[i] != "")
						{
							ifNegative=0;
						}
						rowWellOrder =rowWellOrder + ";" + (itSamplenameSignals->first)[i];
					}
					if(ifNegative==1)
					{
						rowWellOrder="Negative";
					}
				}
				else
				{
					rowWellOrder = (itSamplenameSignals->first)[0];
					for(int i=1;i<(itSamplenameSignals->first).size();i++)
					{
						if(i == vectorIndexSamplenameColumn.size())
						{
							rowWellOrder = rowWellOrder + "," + (itSamplenameSignals->first)[i];
							break;
						}
						rowWellOrder =rowWellOrder + ";" + (itSamplenameSignals->first)[i];
					}
				}
				for(int i=0;i<(itSamplenameSignals->second).size();i++)
				{
					csvSignal<<(itSamplenameSignals->second)[i]<<endl;
					csvWellOrder<<rowWellOrder<<endl;
				}
			}
			csvWellOrder.close();
		}
		fileCount = 1;
	}
	csvSignal.close();
	csvPlateOrder.close();
}

void deletePlateOrderColumn(vector<int> &indexColumn, string scLabel)
{
	ifstream inputPlateOrder(charFilenamePlateOrder);
	ofstream outputPlateOrder("tmp_PlateOrder.txt");
	string bufferRow;
	int plateColumn;
	plateColumn=columnPlateOrderTitle(expDesignSearch("Title including internal control group"));
	getline(inputPlateOrder,bufferRow,'\n');
	deleteReturn(bufferRow);
	int countColumn=0;
	istringstream streamBufferRow(bufferRow);
	string bufferParameter,outputRow;
	while(getline(streamBufferRow,bufferParameter,','))
	{
		if(!inVector(countColumn,indexColumn))
		{
			outputPlateOrder<<bufferParameter<<";";
		}
		countColumn++;
	}
	while(getline(inputPlateOrder,bufferRow,'\n'))
	{
		deleteReturn(bufferRow);
		int countColumn=0;
		istringstream streamBufferRow(bufferRow);
		string bufferParameter,outputRow;
		outputPlateOrder<<endl;
		while(getline(streamBufferRow,bufferParameter,','))
		{
			if(!inVector(countColumn,indexColumn))
			{
				outputPlateOrder<<bufferParameter<<";";
			}
			if((countColumn==plateColumn) && (bufferParameter==scLabel))
			{
				outputPlateOrder<<"######,,,,,,";
			}
			countColumn++;
		}
	}
	inputPlateOrder.close();
	outputPlateOrder.close();
}

string searchPlateOrderRowColumn(int indexRow, int &indexColumn)
{
	ifstream inputPlateOrder(charFilenamePlateOrder);
	string bufferRow;
	getline(inputPlateOrder,bufferRow,'\n');
	int countRow=0;
	while(getline(inputPlateOrder,bufferRow,'\n'))
	{
		if(countRow==indexRow)
		{
			istringstream streamBufferRow(bufferRow);
			string bufferParameter;
			int countColumn=0;
			while(getline(streamBufferRow,bufferParameter,','))
			{
				if(countColumn==indexColumn)
				{
					inputPlateOrder.close();
					deleteReturn(bufferParameter);
					return bufferParameter;
					break;
				}
				countColumn++;
			}
		}
		countRow++;
	}
}

void groupPlates()
{
	groupPlatesIndex.clear();
	setStringRow.clear();
	ifstream inputPlateOrder("tmp_PlateOrder.txt");
	string stringRow;
	getline(inputPlateOrder,stringRow,'\n');
	while(getline(inputPlateOrder,stringRow,'\n'))
	{
		deleteReturn(stringRow);
		string::size_type position;
		position = stringRow.find("######,,,,,,");
		if(position == stringRow.npos)
		{
			setStringRow.insert(stringRow);
		}
	}
	inputPlateOrder.close();
	
	for(set<string>::iterator itSetStringRow=setStringRow.begin();itSetStringRow != setStringRow.end();++itSetStringRow)
	{
		ifstream searchedPlateOrder("tmp_PlateOrder.txt");
		string searchedString;
		getline(searchedPlateOrder,searchedString,'\n');
		int countRow=0;
		while(getline(searchedPlateOrder,searchedString,'\n'))
		{
			deleteReturn(searchedString);
			if(searchedString == *itSetStringRow)
			{
				groupPlatesIndex[*itSetStringRow].insert(countRow);
			}
			countRow++;
		}
		searchedPlateOrder.close();
	}
}

int intNumInVector(vector<int> &inputVector, int queryInt)
{
	int countNum = 0;
	for(int i=0;i<inputVector.size();i++)
	{
		if(inputVector[i]==queryInt)
		{
			countNum++;
		}
	}
	return countNum;
}

double percentileCal(vector<double> &dataset, float &queryPercentage)
{
	sort(dataset.begin(), dataset.end(), less<double>());
	double NPmutiply = queryPercentage*(dataset.size())/100;
	if(NPmutiply - int(NPmutiply) != 0)
	{
		return dataset[int(NPmutiply)];
	}
	else if(dataset.size()==1 || queryPercentage==0)
	{
		return dataset[0];
	}
	else if(queryPercentage==100)
	{
		return dataset[dataset.size()-1];
	}
	else
	{
		return (dataset[NPmutiply-1]+dataset[NPmutiply])/2;
	}
}

double calculateCutoffByReference(map<string, map<string, vector<double> > > &labelledSignals, int &indexPlate, map<int, map<string, int> > &npPlatenumLabel)
{
	double returnedCutoff;
	string modeReference;
	modeReference = expDesignSearch("Reference group chosen mode");
	if(modeReference == "A")
	{
		double lowerNCRange,upperNCRange;
		if((expDesignSearch("Bottom percentile") != ""))
		{
			lowerNCRange = atof(expDesignSearch("Bottom percentile").c_str());
		}
		else
		{
			lowerNCRange = 0;
		}
		if((expDesignSearch("Top percentile") != ""))
		{
			upperNCRange = atof(expDesignSearch("Top percentile").c_str());
		}
		else
		{
			upperNCRange = 100;
		}
		//cout<<labelledSignals["Negative"]["Negative"][0]<<endl;
		sort(labelledSignals["Negative"]["Negative"].begin(), labelledSignals["Negative"]["Negative"].end(), less<double>());
		double lowerRanking = (labelledSignals["Negative"]["Negative"].size())*lowerNCRange/100, upperRanking = (labelledSignals["Negative"]["Negative"].size())*upperNCRange/100;
		int startRanking,endRanking;
		startRanking = int(lowerRanking);
		if(upperRanking-int(upperRanking)>0)
		{
			endRanking = int(upperRanking);
		}
		else
		{
			endRanking = int(upperRanking)-1;
		}
		double cutoffNC = 0;
		for(int i=startRanking;i<=endRanking;i++)
		{
			cutoffNC = cutoffNC + labelledSignals["Negative"]["Negative"][i];
		}
		cutoffNC = cutoffNC/(endRanking-startRanking+1);
		returnedCutoff = cutoffNC;
	}
	else if(modeReference == "B")
	{
		int quantitySignals;
		float percentage;
		vector<double> dataset;
		for(map<string, map<string, vector<double> > >::iterator itLabelledSignals=labelledSignals.begin();itLabelledSignals!=labelledSignals.end();++itLabelledSignals)
		{
			for(map<string, vector<double> >::iterator itSamplenameSignals=(itLabelledSignals->second).begin();itSamplenameSignals!=(itLabelledSignals->second).end();++itSamplenameSignals)
			{
				if(npPlatenumLabel[indexPlate][itSamplenameSignals->first] != 2)
				{
					for(int i=0;i<(itSamplenameSignals->second).size();i++)
					{
						dataset.push_back((itSamplenameSignals->second)[i]);
					}
				}
			}
		}
		percentage = atof(expDesignSearch("Percentile of reference group").c_str());
		if(dataset.size() == 0)
		{
			dataset.push_back(0);
		}
		returnedCutoff = percentileCal(dataset, percentage);
	}
	else if(modeReference == "C")
	{
		map<string, double> signalTypeCutoff;

		string rowSignaltype=expDesignSearch("Signal_type"),rowCutoff=expDesignSearch("Fixed reference"),rowBGCutoff=expDesignSearch("Background noise cut-off"),bufferSignaltype,bufferCutoff;
		istringstream streamSignaltype(rowSignaltype);
		istringstream streamCutoff(rowCutoff);
		
		
		while(getline(streamSignaltype,bufferSignaltype,','))
		{
			getline(streamCutoff,bufferCutoff,',');
			signalTypeCutoff[bufferSignaltype]=atof(bufferCutoff.c_str());
		}
		returnedCutoff = signalTypeCutoff[searchPlateOrderRowColumn(indexPlate, signalTypeColumn)];
	}
	return returnedCutoff;
}


//map<string, double> BGcutoffs;
//map<string, string> BGHLs;

bool passBG(int &indexPlate, double &querySingal)
{
	string signalType = searchPlateOrderRowColumn(indexPlate, signalTypeColumn);
	if(BGHLs[signalType] == "Pass")
	{
		return true;
	}
	else if(BGHLs[signalType]=="Higher or equal")
	{
		if(querySingal>=BGcutoffs[signalType])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(BGHLs[signalType]=="Higher")
	{
		if(querySingal>BGcutoffs[signalType])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(BGHLs[signalType]=="Lower or equal")
	{
		if(querySingal<=BGcutoffs[signalType])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(BGHLs[signalType]=="Lower")
	{
		if(querySingal<BGcutoffs[signalType])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
bool passNC(double &cutoffNC, double &querySingal, string &filterLH)
{
	if(filterLH=="Higher or equal")
	{
		if(querySingal>=cutoffNC)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(filterLH=="Higher")
	{
		if(querySingal>cutoffNC)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(filterLH=="Lower or equal")
	{
		if(querySingal<=cutoffNC)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(filterLH=="Lower")
	{
		if(querySingal<cutoffNC)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

string nbpPromoterParameters(const int &columnPromoter, const string PromoterName,const string groupName, map<int, map<string, int> > &tempNPBPlatenumLabel, const vector<int> &vectorIndexesPlates, vector<int> &vectorColumnParameters, const vector<int> &vectorCutoffParameters)
{
	int count(0);
//	int columnPromoter;
//	string namePromoter = ";;";
	//namePromoter = expDesignSearch("Parameter of row in result files (Optional)");
//	columnPromoter=columnPlateOrderTitle(namePromoter);
	map<vector<string>, vector<int> > labelledNBP;
	for(int i=0;i<vectorIndexesPlates.size();i++)
	{
		//cout<<searchPlateOrderRowColumn(vectorIndexesPlates[i],columnPromoter)<<"::::"<<PromoterName<<endl; 
		if((indexPlatePromoter[vectorIndexesPlates[i]]==PromoterName) && ((tempNPBPlatenumLabel[vectorIndexesPlates[i]].count(groupName))>0))
		{
			vector<string> vectorParameters;
			for(int j=0;j<vectorCutoffParameters.size();j++)
			{
				vectorParameters.push_back(searchPlateOrderRowColumn(vectorIndexesPlates[i],vectorColumnParameters[j]));
			}
			labelledNBP[vectorParameters].push_back(tempNPBPlatenumLabel[vectorIndexesPlates[i]][groupName]);
			count=1;
		}
	}
	if(count==1)
	{
		set<vector<string> > allVectors,allVectorBuffer;
		for(map<vector<string>, vector<int> >::iterator itLabelledNBP=labelledNBP.begin();itLabelledNBP!=labelledNBP.end();++itLabelledNBP)
		{
			allVectors.insert(itLabelledNBP->first);
		}
		if(vectorCutoffParameters.size() != 0)
		{
			for(int i=0;i<vectorCutoffParameters.size();i++)
			{
				allVectorBuffer.clear();
				map<vector<string>, vector<int> > vectorNBPparameter;
				for(set<vector<string> >::iterator itVector=allVectors.begin();itVector!=allVectors.end();++itVector)
				{
					if((*itVector).size()==(vectorCutoffParameters.size()-i))
					{
						vector<string> vectorClassification;
						for(int j=(i+1);j<((*itVector).size());j++)
						{
							vectorClassification.push_back((*itVector)[j]);
						}
						allVectorBuffer.insert(vectorClassification);
						for(int k=0;k<((labelledNBP[*itVector]).size());k++)
						{
							vectorNBPparameter[vectorClassification].push_back(labelledNBP[*itVector][k]);
						}
					}
				}
				for(set<vector<string> >::iterator itVector=allVectorBuffer.begin();itVector!=allVectorBuffer.end();++itVector)
				{
					allVectors.insert(*itVector);
				}
				
				for(map<vector<string>, vector<int> >::iterator itNBPparameter=vectorNBPparameter.begin();itNBPparameter!=vectorNBPparameter.end();++itNBPparameter)
				{
					if((intNumInVector((itNBPparameter->second),1))>=vectorCutoffParameters[i])
					{
						labelledNBP[itNBPparameter->first].push_back(1);
					}
					else if((((itNBPparameter->second).size())-(intNumInVector((itNBPparameter->second),0)))<vectorCutoffParameters[i])
					{
						labelledNBP[itNBPparameter->first].push_back(0);
					}
					else
					{
						labelledNBP[itNBPparameter->first].push_back(2);
					}
				}
			}
			vector<string> callingNBP;
			if((labelledNBP[callingNBP][0])==1)
			{
				return "P";
			}
			else if((labelledNBP[callingNBP][0])==0)
			{
				return "N";
			}
			else
			{
				return "E";
			}
		}
		else
		{
			//cout<<vectorIndexesPlates[0]<<endl;
			if(tempNPBPlatenumLabel[vectorIndexesPlates[0]][groupName]==1)
			{
				return "P";
			}
			else if(tempNPBPlatenumLabel[vectorIndexesPlates[0]][groupName]==0)
			{
				return "N";
			}
			else
			{
				return "E";
			}
		}
	}
	else
	{
		return "-";
	}
}


void NPBtree(map<int, map<string, int> > &tempNPBPlatenumLabel)
{
	string bufferParameter,bufferCutoff,stringParameter,stringCutoff;
	stringParameter = expDesignSearch("Parameters title");
	istringstream streamParameter(stringParameter);
	stringCutoff = expDesignSearch("Parameter cutoff (Optional)");
	istringstream streamCutoff(stringCutoff);
	vector<int> vectorParameterColumn,vectorParameterCutoff;
	while(getline(streamParameter,bufferParameter,',') && getline(streamCutoff,bufferCutoff,','))
	{
		deleteReturn(bufferParameter);
		deleteReturn(bufferCutoff);
		if(bufferParameter!="" && bufferCutoff!="")
		{
			vectorParameterColumn.push_back(columnPlateOrderTitle(bufferParameter));
			//cout<<"Hi!"<<endl;
			vectorParameterCutoff.push_back(atoi(bufferCutoff.c_str()));
		}
	}
	string namePromoter=";;";
	//namePromoter = expDesignSearch("Batch");

	vectorParameterColumn.push_back(columnPlateOrderTitle(namePromoter));
	vectorParameterColumn.push_back(columnPlateOrderTitle(";"));

	int columnPromoter,columnBatch;
	columnPromoter=columnPlateOrderTitle(namePromoter);
	columnBatch=columnPlateOrderTitle(";");
	string nameSC;
	nameSC = expDesignSearch("Name of internal control group (Optional)");
	deletePlateOrderColumn(vectorParameterColumn,nameSC);
	groupPlates();
	
//	string filenameResults="",bufferFilename;
//	istringstream streamStringRow(*(itsetStringRow));
//	while(getline(streamStringRow,bufferFilename,','))
//	{
//		filenameResults = filenameResults + bufferFilename;
//	}
//	deleteReturn(filenameResults);
	ofstream fileFinalOutput("Results.csv");
	
	set<string> setPromoters,setGroupNames;
	for(set<string>::iterator itsetStringRow=setStringRow.begin();itsetStringRow!=setStringRow.end();++itsetStringRow)
	{
		for(set<int>::iterator itgroupPlatesIndex=groupPlatesIndex[*itsetStringRow].begin();itgroupPlatesIndex!=groupPlatesIndex[*itsetStringRow].end();++itgroupPlatesIndex)
		{
			setPromoters.insert(indexPlatePromoter[*itgroupPlatesIndex]);
			for(map<string, int>::iterator itTempNPBPlatenumLabel=tempNPBPlatenumLabel[*itgroupPlatesIndex].begin();itTempNPBPlatenumLabel!=tempNPBPlatenumLabel[*itgroupPlatesIndex].end();++itTempNPBPlatenumLabel)
			{
				setGroupNames.insert(itTempNPBPlatenumLabel->first);
			}
		}
	}
	
	vector<string> vectorPromoters;
	for(set<string>::iterator itPromoter=setPromoters.begin();itPromoter!=setPromoters.end();++itPromoter)
	{
		fileFinalOutput<<","<<(*itPromoter);
		vectorPromoters.push_back(*itPromoter);
	}
	fileFinalOutput<<endl;
	for(set<string>::iterator itGroupName=setGroupNames.begin();itGroupName!=setGroupNames.end();++itGroupName)
	{
		fileFinalOutput<<*itGroupName;
		for(int i=0;i<vectorPromoters.size();i++)
		{
			fileFinalOutput<<",";
			
			vector<int> vectorIndexPlates;
			for(set<int>::iterator itgroupPlatesIndex=groupPlatesIndex[vectorPromoters[i]].begin();itgroupPlatesIndex!=groupPlatesIndex[vectorPromoters[i]].end();++itgroupPlatesIndex)
			{
				vectorIndexPlates.push_back(*itgroupPlatesIndex);
			}
			fileFinalOutput<<nbpPromoterParameters(columnPromoter, vectorPromoters[i],(*itGroupName),tempNPBPlatenumLabel,vectorIndexPlates,vectorParameterColumn,vectorParameterCutoff);
		}
		fileFinalOutput<<endl;
	}
	
	fileFinalOutput.close();
	/*
	for(set<string>::iterator itsetStringRow=setStringRow.begin();itsetStringRow!=setStringRow.end();itsetStringRow++)
	{
		string filenameResults="Results",bufferFilename;
		istringstream streamStringRow(*(itsetStringRow));
		while(getline(streamStringRow,bufferFilename,','))
		{
			filenameResults = filenameResults + bufferFilename;
		}
		deleteReturn(filenameResults);
		filenameResults = filenameResults + ".csv";
		
		ofstream fileFinalOutput(filenameResults.c_str());
		
		if(!fileFinalOutput)
		{
			cout<<"##WARNING: File name, "<<filenameResults<<", contains illegal characters.##"<<endl;
		}
		
		set<string> setPromoters,setGroupNames;
		for(set<int>::iterator itgroupPlatesIndex=groupPlatesIndex[*itsetStringRow].begin();itgroupPlatesIndex!=groupPlatesIndex[*itsetStringRow].end();itgroupPlatesIndex++)
		{
			setPromoters.insert(searchPlateOrderRowColumn((*itgroupPlatesIndex),columnPromoter));
			for(map<string, int>::iterator itTempNPBPlatenumLabel=tempNPBPlatenumLabel[*itgroupPlatesIndex].begin();itTempNPBPlatenumLabel!=tempNPBPlatenumLabel[*itgroupPlatesIndex].end();itTempNPBPlatenumLabel++)
			{
				setGroupNames.insert(itTempNPBPlatenumLabel->first);
			}
		}
		vector<string> vectorPromoters;
		for(set<string>::iterator itPromoter=setPromoters.begin();itPromoter!=setPromoters.end();itPromoter++)
		{
			fileFinalOutput<<","<<(*itPromoter);
			vectorPromoters.push_back(*itPromoter);
		}
		fileFinalOutput<<endl;
		for(set<string>::iterator itGroupName=setGroupNames.begin();itGroupName!=setGroupNames.end();itGroupName++)
		{ 
			fileFinalOutput<<*itGroupName;
			for(int i=0;i<vectorPromoters.size();i++)
			{
				fileFinalOutput<<",";
				
				vector<int> vectorIndexPlates;
				for(set<int>::iterator itgroupPlatesIndex=groupPlatesIndex[*itsetStringRow].begin();itgroupPlatesIndex!=groupPlatesIndex[*itsetStringRow].end();itgroupPlatesIndex++)
				{
					vectorIndexPlates.push_back(*itgroupPlatesIndex);
				}
				fileFinalOutput<<nbpPromoterParameters(vectorPromoters[i],(*itGroupName),tempNPBPlatenumLabel,vectorIndexPlates,vectorParameterColumn,vectorParameterCutoff);
			}
			fileFinalOutput<<endl;
		}
		fileFinalOutput.close();
	}
	*/
}

void remove_tmp_files()
{
	system("rd /s/q .\\GM_tmpfiles_WellOrder");
	remove("ExpDesign.txt");
	remove("PlateOrder.csv");
	remove("Signal.csv");
	remove("tmp_list.txt");
	remove("tmp_PlateOrder.txt");
}


