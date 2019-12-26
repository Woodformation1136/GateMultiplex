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

char charFilenamePlateOrder[20];

int columnPlateOrderTitle(string titleQuery)
{
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
	cout<<"Error: There is no title, "<<titleQuery<<", in PlateOrder.csv."<<endl;
	deleteReturn(titleQuery);
	exit(1);
	return -1;
}

int isNegative(string &queryString)
{
	string::size_type position;
	position = queryString.find("Negative");
	if(position == queryString.npos)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int inVector(int intQuery, vector<int> &intReferece)
{
	int count=0;
	for(int i=0;i<intReferece.size();i++)
	{
		if(intQuery==intReferece[i])
		{
			count=1;
			break;
		}
	}
	return count;
}

void deletePlateOrderColumn(vector<int> &indexColumn, string scLabel)
{
	ifstream inputPlateOrder(charFilenamePlateOrder);
	ofstream outputPlateOrder("tmp_PlateOrder.txt");
	string bufferRow;
	int plateColumn;
	plateColumn=columnPlateOrderTitle("Plate");
	
	getline(inputPlateOrder,bufferRow,'\n');
	deleteReturn(bufferRow);
	int countColumn=0;
	istringstream streamBufferRow(bufferRow);
	string bufferParameter,outputRow;
	while(getline(streamBufferRow,bufferParameter,','))
	{
		if(inVector(countColumn,indexColumn)==0)
		{
			outputPlateOrder<<"_,"<<bufferParameter;
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
			if(inVector(countColumn,indexColumn)==0)
			{
				outputPlateOrder<<",_"<<bufferParameter;
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

map<string,set<int> > groupPlatesIndex;
set<string> setStringRow;
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
	
	for(set<string>::iterator itSetStringRow=setStringRow.begin();itSetStringRow != setStringRow.end();itSetStringRow++)
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


string nbpPromoterParameters(string PromoterName,string groupName, map<int, map<string, int> > &tempNPBPlatenumLabel, vector<int> &vectorIndexesPlates, vector<int> &vectorColumnParameters,vector<int> &vectorCutoffParameters)
{
	int columnPromoter,count;
	string namePromoter;
	namePromoter = expDesignSearch("Parameter of row in result files");
	count=0;
	columnPromoter=columnPlateOrderTitle(namePromoter);
	map<vector<string>, vector<int> > labelledNBP;
	for(int i=0;i<vectorIndexesPlates.size();i++)
	{
		if((searchPlateOrderRowColumn(vectorIndexesPlates[i],columnPromoter)==PromoterName) && ((tempNPBPlatenumLabel[vectorIndexesPlates[i]].count(groupName))>0))
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
	set<vector<string> > allVectors,allVectorBuffer;
	for(map<vector<string>, vector<int> >::iterator itLabelledNBP=labelledNBP.begin();itLabelledNBP!=labelledNBP.end();itLabelledNBP++)
	{
		allVectors.insert(itLabelledNBP->first);
	}
	if(count==1)
	{
		for(int i=0;i<vectorCutoffParameters.size();i++)
		{
			allVectorBuffer.clear();
			map<vector<string>, vector<int> > vectorNBPparameter;
			for(set<vector<string> >::iterator itVector=allVectors.begin();itVector!=allVectors.end();itVector++)
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
			for(set<vector<string> >::iterator itVector=allVectorBuffer.begin();itVector!=allVectorBuffer.end();itVector++)
			{
				allVectors.insert(*itVector);
			}
			
			for(map<vector<string>, vector<int> >::iterator itNBPparameter=vectorNBPparameter.begin();itNBPparameter!=vectorNBPparameter.end();itNBPparameter++)
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
			return "B";
		}
	}
	else
	{
		return "NO";
	}
}


void NPBtree(map<int, map<string, int> > &tempNPBPlatenumLabel)
{
	string bufferParameter,bufferCutoff,stringParameter,stringCutoff;
	stringParameter = expDesignSearch("Parameters");
	istringstream streamParameter(stringParameter);
	stringCutoff = expDesignSearch("Parameter cutoff");
	istringstream streamCutoff(stringCutoff);
	vector<int> vectorParameterColumn,vectorParameterCutoff;
	while(getline(streamParameter,bufferParameter,' ') && getline(streamCutoff,bufferCutoff,' '))
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
	string namePromoter;
	namePromoter = expDesignSearch("Parameter of row in result files");

	vectorParameterColumn.push_back(columnPlateOrderTitle(namePromoter));
	vectorParameterColumn.push_back(columnPlateOrderTitle("Batch"));

	int columnPromoter,columnBatch;
	columnPromoter=columnPlateOrderTitle(namePromoter);
	columnBatch=columnPlateOrderTitle("Batch");
	string nameSC;
	nameSC = expDesignSearch("Name of SC plate");
	deletePlateOrderColumn(vectorParameterColumn,nameSC);
	groupPlates();
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
}




int main()
{
	int plateNum, numeachplate, cutoffTech, cutoffBio;
	if(expDesignSearch("Total number of all plates") != "")
	{
		plateNum = atoi(expDesignSearch("Total number of all plates").c_str());
		cout<<"Total number of all plates is "<<plateNum<<" as user's setting."<<endl;
	}
	else
	{
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
		cout<<"Total number of all plates is "<<plateNum<<" according to PlateOrder.csv."<<endl;
	}
	cutoffTech = atoi(expDesignSearch("Technical replicate cut-off").c_str());
	cout<<"Technical replicate cut-off is set to "<<cutoffTech<<"."<<endl;
	cutoffBio = atoi(expDesignSearch("Biological replicate cut-off").c_str());
	cout<<"Biological replicate cut-off is set to "<<cutoffBio<<"."<<endl;
	
	double cutoffBackground, foldChange, lowerNCRange, upperNCRange, cutoffInternal;
	if((expDesignSearch("Background cut-off (default = 3)") != "") && (expDesignSearch("Background cut-off (default = 3)") != " "))
	{
		cutoffBackground = atof(expDesignSearch("Background cut-off (default = 3)").c_str());
		cout<<"Background cut-off is set to "<<cutoffBackground<<"."<<endl;
	}
	else
	{
		cutoffBackground = 3;
		cout<<"Background cut-off is set to "<<cutoffBackground<<" as default value."<<endl;
	}
	if((expDesignSearch("Fold change (default = 1.5)") != "") && (expDesignSearch("Fold change (default = 1.5)") != " "))
	{
		foldChange = atof(expDesignSearch("Fold change (default = 1.5)").c_str());
		cout<<"Fold change is set to "<<foldChange<<"."<<endl;
	}
	else
	{
		foldChange = 1.5;
		cout<<"Fold change is set to "<<foldChange<<" as default value."<<endl;
	}
	if((expDesignSearch("Negative control lower bound percentage (default = 25)") != "") && (expDesignSearch("Negative control lower bound percentage (default = 25)") != " "))
	{
		lowerNCRange = atof(expDesignSearch("Negative control lower bound percentage (default = 25)").c_str());
		cout<<"Negative control lower bound percentage is set to "<<lowerNCRange<<"."<<endl;
	}
	else
	{
		lowerNCRange = 25;
		cout<<"Negative control lower bound percentage is set to "<<lowerNCRange<<" as default value."<<endl;
	}
	if((expDesignSearch("Negative control upper bound percentage (default = 75)") != "") && (expDesignSearch("Negative control upper bound percentage (default = 75)") != " "))
	{
		upperNCRange = atof(expDesignSearch("Negative control upper bound percentage (default = 75)").c_str());
		cout<<"Negative control upper bound percentage is set to "<<upperNCRange<<"."<<endl;
	}
	else
	{
		upperNCRange = 75;
		cout<<"Negative control upper bound percentage is set to "<<upperNCRange<<" as default value."<<endl;
	}

	if(lowerNCRange > upperNCRange)
	{
		cout<<"##Error: lower bound cannot be higher than upper bound.##"<<endl;
		exit(1);
	}

	if((expDesignSearch("Internal cut-off (default = 3)") != "") && (expDesignSearch("Internal cut-off (default = 3)") != " "))
	{
		cutoffInternal = atof(expDesignSearch("Internal cut-off (default = 3)").c_str());
		cout<<"Internal cut-off is set to "<<cutoffInternal<<"."<<endl;
	}
	else
	{
		cutoffInternal = 3;
		cout<<"Internal cut-off is set to "<<cutoffInternal<<" as default value."<<endl;
	}
	
	string switchFoldChangeFile,nameSC,filterLH;
	switchFoldChangeFile = expDesignSearch("Output fold change files (default = Off)");
	nameSC = expDesignSearch("Name of SC plate");
	filterLH = expDesignSearch("Filter lower or higher signal than cutoff (default = lower)");
	
	string namePromoter;
	namePromoter = expDesignSearch("Parameter of row in result files");
	
	if(namePromoter=="")
	{
		strcpy(charFilenamePlateOrder,"tmp_PlateOrder.csv");
		ifstream originalPlateOrder("PlateOrder.csv");
		if(!originalPlateOrder)
		{
			cout<<"##Error: There is no file named PlateOrder.csv.##"<<endl;
			exit(1);
		}
		ofstream tmpPlateOrder("tmp_PlateOrder.csv");
		string rowOriginalPlateOrder;
		while(getline(originalPlateOrder,rowOriginalPlateOrder,'\n'))
		{
			deleteReturn(rowOriginalPlateOrder);
			tmpPlateOrder<<","<<rowOriginalPlateOrder<<endl;
		}
		tmpPlateOrder.close();
		originalPlateOrder.close();
	}
	else
	{
		strcpy(charFilenamePlateOrder,"PlateOrder.csv");
	}
	
	string lineSignal,lineWellOrder,linePlateOrder,data,tmps,label,bio;
	ifstream fileSignalcsv, filePlateOrdercsv,filePlateOrderForFC, fileWellOrdercsv;
	fstream testfile;
	
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
	
	string bufferLinePlateOrder;
	int countColumnBatch,countColumnPlate;
	countColumnBatch = columnPlateOrderTitle("Batch");
	countColumnPlate = columnPlateOrderTitle("Plate");
	map<int, map<string, int> > npPlatenumLabel;
	set<string> setAllNameGroup;
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
		nameWellOrder = "WellOrder_" + bufferLinePlateOrder + ".csv";
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
			if(isNegative(label)==0 && label != "Blank" && label != "" && data !="") //labelling the experimental signals
			{
				labelledSignal[bio][label].push_back(atof(data.c_str()));
				setNameGroup.insert(label);
				setAllNameGroup.insert(label);
				setLabelledBio[label].insert(bio);
			}
			else if(isNegative(label)==1 && data !="") //labelling the negative control or blank signals with "Negative" or "Blank" and Bio[0]
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
			for (set<string>::iterator itLabel = setNameGroup.begin(); itLabel != setNameGroup.end(); itLabel++) //call all the Label strings from a set
			{
				npPlatenumLabel[indexPlate][*itLabel]=0;
	        	for (set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); itBio++) //call all the Bio integers from a set
				{
					for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
					{
						if(labelledSignal[*itBio][*itLabel][i]>=cutoffInternal && filterLH == "higher")
						{
							npPlatenumLabel[indexPlate][*itLabel]=2;
							break;
						}
						else if(labelledSignal[*itBio][*itLabel][i]<cutoffInternal && filterLH != "higher")
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
		}
		
		else
		{
			sort(labelledSignal["Negative"]["Negative"].begin(), labelledSignal["Negative"]["Negative"].end(), less<double>());
			double lowerRanking = (labelledSignal["Negative"]["Negative"].size())*lowerNCRange/100, upperRanking = (labelledSignal["Negative"]["Negative"].size())*upperNCRange/100;
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
				cutoffNC = cutoffNC + labelledSignal["Negative"]["Negative"][i];
			}
			double denominatorFoldChange = cutoffNC/(endRanking-startRanking+1);
			cutoffNC = cutoffNC/(endRanking-startRanking+1)*foldChange;
			if(cutoffNC>=cutoffBackground && filterLH == "higher")
			{
				cutoffNC = cutoffBackground;
			}
			else if(cutoffNC<cutoffBackground && filterLH != "higher")
			{
				cutoffNC = cutoffBackground;
			}
			
			
			if(switchFoldChangeFile=="On" || switchFoldChangeFile=="on") //output Fold change when switch is "On"
			{
				char filenameFoldChange[30];
				sprintf(filenameFoldChange,"FoldChange_plate%d",indexPlate);
				string filenameFC="";
				filenameFC = filenameFoldChange;
				getline(filePlateOrderForFC,linePlateOrder,'\n');
				deleteReturn(linePlateOrder);
				istringstream streamFilenameNPB(linePlateOrder);
				while(getline(streamFilenameNPB,bufferLinePlateOrder,','))
				{
					filenameFC=filenameFC+"_"+bufferLinePlateOrder;
				}
				filenameFC=filenameFC+".csv";
				
				ofstream filestreamFoldChange(filenameFC.c_str());
				
				if(!filestreamFoldChange)
				{
					cout<<"##WARNING: File name, "<<filenameFC<<", contains illegal characters.##"<<endl;
				}
				
				for (set<string>::iterator itLabel = setNameGroup.begin(); itLabel != setNameGroup.end(); itLabel++) //call all the Label strings from a set
				{
					filestreamFoldChange<<*itLabel;
		        	for (set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); itBio++) //call all the Bio integers from a set
					{
						filestreamFoldChange<<",Bio"<<*itBio;
						for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
						{
							filestreamFoldChange<<","<<labelledSignal[*itBio][*itLabel][i]/denominatorFoldChange;
						}
						filestreamFoldChange<<endl;
					}
		    	}
		    	filestreamFoldChange.close();
	    	}
	    	
			for (set<string>::iterator itLabel = setNameGroup.begin(); itLabel != setNameGroup.end(); itLabel++) //call all the Label strings from a set
			{
				vector<int> vectorBioNP;
	        	for (set<string>::iterator itBio = setLabelledBio[*itLabel].begin(); itBio != setLabelledBio[*itLabel].end(); itBio++) //call all the Bio integers from a set
				{
					vector<int> vectorTechNP;
					for(int i=0;i<labelledSignal[*itBio][*itLabel].size();i++) //call all the Signal double values from labelled signal vector
					{
						if(filterLH=="higher")
						{
							if(labelledSignal[*itBio][*itLabel][i]<cutoffNC)
							{
								vectorTechNP.push_back(1);
							}
							else
							{
								vectorTechNP.push_back(0);
							}
						}
						else
						{
							if(labelledSignal[*itBio][*itLabel][i]>=cutoffNC)
							{
								vectorTechNP.push_back(1);
							}
							else
							{
								vectorTechNP.push_back(0);
							}
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
					npPlatenumLabel[indexPlate][*itLabel] = 1;
				}
				else
				{
					npPlatenumLabel[indexPlate][*itLabel] = 0;
				}
	    	}
    	}
	}
	filePlateOrdercsv.close();
	fileSignalcsv.close();
	filePlateOrderForFC.close();
	
	//Labelling "BAD Negative" in integer 2 and "BAD Positive" in integer 3 onto npPlatenumLabel[indexPlate][Label]
	int columnPlateOrder;
	columnPlateOrder = columnPlateOrderTitle("Plate");
	filePlateOrdercsv.open(charFilenamePlateOrder,ios::in);
	getline(filePlateOrdercsv,linePlateOrder,'\n');
	for(int indexPlate=0;indexPlate<plateNum;indexPlate++)
	{
		getline(filePlateOrdercsv,linePlateOrder,'\n');
		deleteReturn(linePlateOrder);
		istringstream streamPlateOrderPlate(linePlateOrder);
		for(int i=0;i<=countColumnPlate;i++)
		{
			getline(streamPlateOrderPlate,bufferLinePlateOrder,',');
			if(i==countColumnPlate)
			{
				break;
			}
		}
		if(bufferLinePlateOrder == nameSC)
		{
			istringstream streamlinePlateOrder(linePlateOrder);
			string stringCompared="";
			int countLine = 0;
			while(getline(streamlinePlateOrder,bufferLinePlateOrder,','))
			{
				if(countLine!=columnPlateOrder)
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
					if(columnLineSearch!=columnPlateOrder)
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
					for(map<string,int>::iterator itNP=npPlatenumLabel[indexPlateSearch].begin();itNP != npPlatenumLabel[indexPlateSearch].end();itNP++)
					{
						npPlatenumLabel[indexPlateSearch][itNP->first] = npPlatenumLabel[indexPlateSearch][itNP->first] + npPlatenumLabel[indexPlate][itNP->first];
					}
				}
				indexPlateSearch++;
			}
			
			filePlateSearch.close();
		}
	}
	filePlateOrdercsv.close();
	//Lablling all plates with PlateOrder.csv
	filePlateOrdercsv.open(charFilenamePlateOrder,ios::in);
	getline(filePlateOrdercsv,linePlateOrder,'\n');
	for(int indexPlate=0;indexPlate<plateNum;indexPlate++)
	{
		char filenameBuffer[20];
		sprintf(filenameBuffer,"NPB_%d",indexPlate);
		string filenameNPB;
		filenameNPB=filenameBuffer;
		getline(filePlateOrdercsv,linePlateOrder,'\n');
		deleteReturn(linePlateOrder);
		istringstream streamFilenameNPB(linePlateOrder);
		while(getline(streamFilenameNPB,bufferLinePlateOrder,','))
		{
			filenameNPB=filenameNPB+"_"+bufferLinePlateOrder;
		}
		filenameNPB=filenameNPB+".csv";
		//cout<<filenameNPB<<endl;
		ofstream filestreamNPB(filenameNPB.c_str());
		
		if(!filestreamNPB)
		{
			cout<<"##WARNING: File name, "<<filenameNPB<<", contains illegal characters for this operating system.##"<<endl;
		}
		
		for(map<string,int>::iterator itNP=npPlatenumLabel[indexPlate].begin();itNP != npPlatenumLabel[indexPlate].end();itNP++)
		{
			//Output N/P/B files for all groups in each plate
			if((itNP->second)==0)
			{
				filestreamNPB<<itNP->first<<":,N"<<endl;
			}
			else if((itNP->second)==1)
			{
				filestreamNPB<<itNP->first<<":,P"<<endl;
			}
			else if((itNP->second)==2)
			{
				filestreamNPB<<itNP->first<<":,B/N"<<endl;
			}
			else if((itNP->second)==3)
			{
				filestreamNPB<<itNP->first<<":,B/P"<<endl;
			}
			else
			{
				filestreamNPB<<itNP->first<<":,BUG!!!!!!"<<endl;
			}
		}
		filestreamNPB.close();
	}
	filePlateOrdercsv.close();

	NPBtree(npPlatenumLabel);
	
	//delete temporary files
	remove("tmp_PlateOrder.txt");
	if(namePromoter=="")
	{
		remove("tmp_PlateOrder.csv");
	} 
	
	system("PAUSE");
	return 0;
}
