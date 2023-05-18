#include <iostream>
#include<algorithm>
#include <iomanip>
#include <fstream>
#include<cmath>
#include <vector>
#include <stdlib.h>     
#include <time.h>
#include <string>

using namespace std;

int msize, psz, q, cs, nof;// nof:# of frames,msize:mwmory size, psz:page size ,q :quantum,cs:context switch  
string memory[999999];
int i, ab;// ab is a variable we used it in SJF represent Completion time for previous process
float cpu_uti;//cpu_uti:cpu utilization 

struct pcb
{
	int pro_id, artime, bt, ct, ta, wt,  tabel[99999];//pro_id:process id,artime = Arrival time,bt = Burst time,ct = Completion time,ta = Turn around time,wt = Waiting time
	float prs, nop;//prs=process size, nop :#of pages
	bool ht;//ht :have tabel
}process[5];

bool compare(pcb p1, pcb p2)// to compare the arrival time between processes,where parameter p1  Process # 1 and paramter p2  Process # 2
{
	return p1.artime < p2.artime;   //return true   if  p1.artime < p2.artime, return false   if   p1.artime >= p2.artime
	// This process will always return TRUE if above condition comes
}
bool compare2(pcb p1, pcb p2) // to compare the burst time between processes and if the process arrival time less than or equal to the Completion time of the previous process
{
	return p1.bt < p2.bt&& p1.artime <= ab;
}

void FCFS(pcb processes[])//implement First Come First Serve Scheduling, paramter is array of processes
{
	sort(process, process + 5, compare);// sort the process according to its arrival time, sort is a predefined funcion  defined in algorithm 

	// initial values for first process
	process[0].ct = process[0].artime + process[0].bt;
	//Completion time for first process equals arrival time for it plus burst time 
	process[0].ta = process[0].ct - process[0].artime;
	//turnaround time for first  process equals Completion time for it minus arrival time
	process[0].wt = 0;
	//Waiting time for first process is zero 
	
	for (i = 1; i < 5; i++) //To fill the information of the other process
	{
		if (process[i].artime <= process[i - 1].ct)//to ensure that the arrival time of the process is smaller  than or equal the completion time for the previous one
		{
			process[i].ct = process[i - 1].ct + process[i].bt + cs;
			//Completion time for Current process equals Completion time for previous process plus context switch plus burst time for Current process
			process[i].ta = process[i].ct - process[i].artime;
			//turnaround time for Current process equals Completion time for it minus arrival time
			process[i].wt = process[i - 1].ct - process[i].artime + cs;
			//Waiting time for Current process equals Completion time for previous process minus arrival time plus context switch
		}
		else // if the arrival time of process is larger than the Completion time for previous one 
		{
			process[i].ct = process[i].bt + process[i].artime + cs;
            //Completion time for Current process equals Burst time for the process plus Arraival time of the process plus context switch  
			process[i].ta = process[i].ct - process[i].artime;
            //turnaround time for Current process equals Completion time for it minus arrival time
			process[i].wt = process[i].ta - process[i].bt + cs;
            //Waiting time for Current process equals turnaround time for the process minus Burst time for the Process plus context switch
		}
	}
	float sb = 0;//declare variable for the sum of burst time
	float sw = 0;//declare variable for the sum of Waitingtime 
	float st = 0;//declare variable for the sum of turnaround time 

	cout << "Gantt Chart" << endl;
	for (i = 0; i < 5; i++)
	{
		//construction of gantchart
		cout << "|---p" << process[0].pro_id;
		for (int j = 0; j < process[0].bt; j++)
			cout << "-";
		cout << "|";
		for (i = 1; i < 5; i++)
		{
			cout << "CS|---p" << process[i].pro_id;
			for (int j = 0; j < process[i].bt; j++)
				cout << "-";
			cout << "|";
		}
		cout << endl;
		cout << "     ";
		for (int j = 0; j < process[0].bt; j++)
			cout << " ";
		cout << process[0].ct;
		for (i = 1; i < 5; i++)
		{
			cout << "       ";
			for (int j = 0; j < process[i].bt; j++)
				cout << " ";
			cout << process[i].ct;
		}
		cout << endl;
		//calculating the sum of Waiting time, turnaround time and burst time
		for (i = 0; i < 5; i++) {
			sw += process[i].wt;
			st += process[i].ta;
			sb += process[i].bt;
		}
		cout << endl;
		//print information for each process: process id, arrival time, burst time, completion time, turnaround time , Waiting time
		for (i = 0; i < 5; i++)
		{
			cout << " Process ID: " << process[i].pro_id << "   Arrival Time: " << process[i].artime << "   Burst Time:  "
			<< process[i].bt <<"   Waiting time:  " << process[i].wt << "\t   Turnaround Time:  " << process[i].ta << "     \t  Completion Time: " << process[i].ct;
			cout << endl;
		}
		cout << endl;
		//print Avg of Waitingtime ,Avg of turnaround time  for each process and cpu utilization
		cout << "Average Waiting Time=  " << sw / 5 << "   Average Turnaround Time= " << st / 5 << endl;
		cout << "Cpu Utilization =" << (sb / (sb + 4 * cs))*100<<"%" << endl;//we calculated the CPU utilization and it is equal to the sum of the burst time divided by the amount of the sum of the burst time add to the number of context switch
	}
}

void sjf(pcb pro[])//implement Shortest Job First Scheduling, paramter is array of processes
{
	sort(pro, pro + 5, compare); // sort the process according to its arrival time
	
	// initial values for first process
	pro[0].ct = pro[0].bt + pro[0].artime;
	//Completion time for first process equals arrival time for it plus burst time 
	pro[0].ta = pro[0].ct - pro[0].artime;
	//turnaround time for first  process equals Completion time for it minus arrival time
	pro[0].wt = 0;
	//Waiting time for first process is zero

    //To fill the information of the other process
	for (i = 1; i < 5; i++)
	{
		ab = pro[i - 1].ct;//a variable that represents the completion time of the previous process
		sort(pro + i, pro + 5, compare2);
        /*to compare the burst time between processes and if the process arrival time less than or equal to the Completion time of the previous process
		or in other word sort processes according to burst time taking arrival time in consideration*/
        
		if (pro[i - 1].ct >= pro[i].artime)//to ensure that the arrival time of the process is smaller  than or equal the completion time for the previous one
		{
			pro[i].ct = pro[i - 1].ct + pro[i].bt + cs;
			/*Completion time for Current process equals Completion time for previous process plus
			context switch plus burst time for Current process*/
		}
		else // if the arrival time of process is larger than the Completion time for previous one
		{
			pro[i].ct = pro[i].bt + pro[i].artime;
            //Completion time for Current process equals burst time of the process plus the arrival time of the process

		}
		pro[i].ta = pro[i].ct - pro[i].artime;
		//turnaround time for Current process equals Completion time for it minus arrival time
		pro[i].wt = pro[i - 1].ct - pro[i].artime + cs;
		//Waiting time for Current process equals Completion time for previous process minus arrival time for Current process plus context switch
	}
	float sb = 0;//declare variable for the sum of burst time
	float sw = 0;//declare variable for the sum of Waiting time 
	float st = 0;//declare variable for the sum of turnaround time
	cout << "Gantt Chart" << endl;
	//construction of gantchart
	cout << "|---p" << pro[0].pro_id;
	for (int j = 0; j < pro[0].bt; j++)
		cout << "-";
	cout << "|";
	for (i = 1; i < 5; i++)
	{
		cout << "CS|---p" << pro[i].pro_id;
		for (int j = 0; j < pro[i].bt; j++)
			cout << "-";
		cout << "|";
	}
	cout << endl;
	cout << "     ";
	for (int j = 0; j < pro[0].bt; j++)
		cout << " ";
	cout << pro[0].ct;
	for (i = 1; i < 5; i++)
	{
		cout << "       ";
		for (int j = 0; j < pro[i].bt; j++)
			cout << " ";
		cout << pro[i].ct;
	}
	cout << endl << endl;
	for (i = 0; i < 5; i++)
	{
		//print information for each process: process id, arrival time, burst time, completion time ,turnaround time , Waiting time 
		cout << " Process ID: " << pro[i].pro_id << "   Arrival Time: " << pro[i].artime << "   Burst Time:  "
			<< pro[i].bt <<"   Waiting time:  " << pro[i].wt << "\t   Turnaround Time:  " << pro[i].ta << "     \t  Completion Time: " << pro[i].ct;
		cout << endl;
		//calculating  sum for Waitingtime,turnaround time and burst time
		sw += pro[i].wt;
		st += pro[i].ta;
		sb += pro[i].bt;
	}
	//calculating  sum for Avg of Waitingtime , Avg of turnaround time and cpu utilization
	cout << "Average Waiting Time=  " << sw / 5 << "   Average Turnaround Time= " << st / 5 << endl;
	cout << "Cpu Utilization= " << (sb / (sb + 4 * cs) )*100<<"%"<< endl;
}

void RR(pcb pro[])//implement Round Robin Scheduling, parameter is array of processes
{
	sort(pro, pro + 5, compare);//sort the process according to its arrival time 

	int rem_bt[5]; // define an array to fill it with the remaining burst time of the process 
	for (int i = 0; i < 5; i++)
	rem_bt[i] = pro[i].bt; // Make a copy of burst times to initialize remaining burst times

	int wt[5], at[5];// define a Waiting time array , a turnaround time array
	int completion_time[5];// define a completion_time array
	int t = 0; // define a variable to represent the Current time 
	int cou = -1;//define a variable to count number of contex switch and initialize it to -1

	// Keep traversing processes in round robin manner as long all of them are not done 
	//construction of Gant chart
	cout << "Gantt Chart \n" ;
	while (1)
	{
		bool done = true; // define a boolean variable called done and set it to true 
		// Traverse all processes one by one repeatedly 
		for (int i = 0; i < 5; i++)
		{
			// If burst time of a process is greater than 0 
			// then only need to process further 
			if (rem_bt[i] > 0) 
			{
				done = false; // There is a pending process 
				if (rem_bt[i] > q)
				{
					// Increase the value of t"Current time" shows 
					// how much time a process has been processed 
					t += q;
					cou++;
					// Decrease the burst_time of current process by quantum
					rem_bt[i] -= q;
					cout << "|---p" << pro[i].pro_id << "----" << t + (cou * cs) << "|CS";
					
				
					if( t + (cou * cs)<pro[i+1].artime)//to check if the next process doesnt arrive yet it will continue processing the same process 
					i--;
				}
				// If burst time is smaller than or equal to quantum. Last cycle for this process 
				else
				{
					// Increase the value of t"Current time" shows 
					// how much time a process has been processed 
					t = t + rem_bt[i];
					cou++;
			

					wt[i] = t - pro[i].artime - pro[i].bt + (cou * cs);
                    // Waiting time is current time minus time used by this process minus arrival time plus quantity of context switch
					at[i] = t - pro[i].artime + (cou * cs);
					//turnaround times is current time minus arrival time plus quantity of context switch
					completion_time[i] = at[i] + pro[i].artime;
                    //completion time is  turnaround time plus arrival time 
					cout << "|---p" << pro[i].pro_id << "----" << t + (cou * cs) << "|CS";
					  // As the process gets fully executed make its remaining burst time = 0 
					rem_bt[i] = 0;
				}
			}
		}
		// If all processes are done 
		if (done == true)
			break;
	}
	cout << endl ;
    //calculating  sum for Avg of Waitingtime , Avg of turnaround time and cpu utilization
    // print information for each process: process id, completion time, turnaround time , Waiting time 
	float average_turnaround_time = 0, average_wait_time = 0, sb = 0;//sb: variable that represent the sum of burst time
	for (int i = 0; i < 5; i++)
	{
		average_turnaround_time += at[i];
		average_wait_time += wt[i];
		sb += pro[i].bt;
		cout << endl;
		cout << " Process ID: " << pro[i].pro_id << "   Arrival Time: " << pro[i].artime << "     Burst Time:  "
			<< pro[i].bt <<"   Waiting time:  " << wt[i] << "\t    Turnaround Time:  " << at[i] << "     \t  Completion Time: " << completion_time[i];
	}
	cout << endl << "Average Waiting Time=  " << average_wait_time / 5 << "   Average Turnaround Time= " << average_turnaround_time / 5 << endl;
	cout << endl << "Cpu Utilization= " << (sb / (sb + (cou * cs)))*100<<"%" << endl;
}

int main() {
	ifstream inFile;
	// to read the file
	inFile.open("process.txt");
	if (!inFile) {
		cout << "Cannot open the file";
		exit(1); // terminate with error
	}
	inFile >> msize >> psz >> q >> cs;
	for (int i = 0; i < 5; i++)
	{
		inFile >> process[i].pro_id >> process[i].artime >> process[i].bt >> process[i].prs;
	}

	cout <<"FCFS  "<< endl;
    FCFS(process);
    cout<<"---------------------------------------------------";
    cout<<endl<<"SJF"<<endl;
    sjf(process);
    cout<<"---------------------------------------------------";
    cout<<endl<<"RR"<<endl;
    RR(process);
    cout<<"---------------------------------------------------";

	inFile.close();
	return 0;
}
