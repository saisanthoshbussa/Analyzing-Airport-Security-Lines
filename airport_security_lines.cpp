#include<iostream>
#include<queue>
#include<random>
#include<math.h>
#include<bits/stdc++.h>
using namespace std;
//generating exponential time
double generateExponentialTime(double lambda)
{
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<double> exp_dist(lambda);
    return exp_dist(gen);
}
//class for each passenger
class Passenger{
    public:
    int arrival;
    int service_time_left;
    int serviced_time;
    Passenger(int arr)
    {
        arrival=arr;
        service_time_left=-1;
        serviced_time=0;
    }
};
class server{
    public:
    int busy;
    int serving_time; //total serving time till now
    int temp_serving; //how much left
    server()
    {
        busy=0;
        serving_time=0;
        temp_serving=-1;
    }
};
class security{
    public:
    int total_passengers;
    int buffer_size;
    vector<server*> all_servers;
    deque<Passenger*> wait_queue;
    security(int k,int number_of_servers)
    {
        if(k==-1)
        {
            buffer_size=INT_MAX;	//infinte buffer capacity
        }
        else 
        {
            buffer_size=k;
        }
        total_passengers=0;
        all_servers.resize(number_of_servers);
        for(int i=0;i<number_of_servers;i++)
        {
            all_servers[i]=new server();
        }
    }
};

int main()
{
    double arrival_rate,dep_rate;
    cout<<"Enter arrival rate ";
    cin>>arrival_rate;
    cout<<"Enter Departure rate ";
    cin>>dep_rate;
    long long simulated_time;
    long long tot_passengers = 0;
    long long tot_waiting_time = 0;
    long long current_time;
    long long next_arrival_time;
    long long total;
	
	printf("\n\n======== Single server with infinite buffer capacity ========\n\n");
    while(1)
    {
    
        cout<<"\tEnter simulation time ";
        cin>>simulated_time;
        
        security security1(-1,1);
        current_time=0;
        next_arrival_time = generateExponentialTime(arrival_rate);
        total=0;
        for(current_time=0;current_time<simulated_time;current_time++)
        {
            if (current_time == next_arrival_time)
            {
                Passenger *new_passenger = new Passenger(current_time);
                security1.wait_queue.push_back(new_passenger);
                total++;
				//if next arrival time generated is zero then we need to admit it in same iteration only
                while ((next_arrival_time = (generateExponentialTime(arrival_rate))) == 0)
                {
                    Passenger *new_passenger = new Passenger(current_time);
                    total++;
                    security1.wait_queue.push_back(new_passenger);
                }
                next_arrival_time += current_time;
            }
			//if idle serve
            if((security1.all_servers[0]->busy==0 || security1.all_servers[0]->temp_serving==0) &&security1.wait_queue.size()!=0)
            {
                int temp;
                while((temp=(generateExponentialTime(dep_rate)))==0&&security1.wait_queue.size()!=0)
                {
                    Passenger *to_be_served = security1.wait_queue.front();
                    security1.wait_queue.pop_front();
                    tot_waiting_time+=current_time-to_be_served->arrival;
                }
                if(security1.wait_queue.size()==0)
                {
                    continue;
                }
                else
                {
                    Passenger *to_be_served = security1.wait_queue.front();
                    security1.wait_queue.pop_front();
                    tot_waiting_time+=current_time-to_be_served->arrival;
                    security1.all_servers[0]->busy=1;
                    security1.all_servers[0]->temp_serving=temp;
                }
            }
            if(security1.all_servers[0]->temp_serving>0)
            {
                security1.all_servers[0]->temp_serving--;
                security1.all_servers[0]->serving_time++;
            }
            tot_passengers+=security1.wait_queue.size();
        }
		//standered formulas
        cout<<"\tAverage Queue Length is = "<<(double)tot_passengers/simulated_time<<endl;
        cout<<"\tAvgerage Waiting time is = "<<(double)tot_waiting_time/total<<endl;
		long long total_service_time=security1.all_servers[0]->serving_time;
        cout<<"\tServer utilization is = "<<(double)total_service_time/simulated_time<<endl;
        char req;
        cout << "\tWant to try again(y/n): ";
        cin>>req;
		cout<<endl;
        if(req=='n') break;
    }
	printf("\n\n======== Single server with finite buffer capacity ========\n\n");
    while(1)
    {
        cout<<"\tEnter buffer size ";
        int buf_size;
        cin>>buf_size;
        cout << "\tEnter the simulating time ";
        cin >> simulated_time;
        tot_passengers = 0;
        tot_waiting_time = 0;
        security security2(buf_size, 1);
        current_time = 0;
        next_arrival_time = generateExponentialTime(arrival_rate);
        total=0;
        for (current_time = 0; current_time < simulated_time; current_time++)
        {
            if (current_time == next_arrival_time && security2.wait_queue.size()<buf_size)
            {
                Passenger *new_passenger = new Passenger(current_time);
                security2.wait_queue.push_back(new_passenger);
                total++;
                while ((next_arrival_time = (generateExponentialTime(arrival_rate))) == 0 && security2.wait_queue.size()<buf_size)
                {
                    Passenger *new_passenger = new Passenger(current_time);
                    security2.wait_queue.push_back(new_passenger);
                    total++;
                }
                next_arrival_time += current_time;
            }
            if ((security2.all_servers[0]->busy == 0 || security2.all_servers[0]->temp_serving == 0) && security2.wait_queue.size() != 0)
            {
                int temp;
                while ((temp = (generateExponentialTime(dep_rate))) == 0 && security2.wait_queue.size() != 0)
                {
                    Passenger *to_be_served = security2.wait_queue.front();
                    security2.wait_queue.pop_front();
                    tot_waiting_time += current_time - to_be_served->arrival;
                }
                if (security2.wait_queue.size() == 0)
                {
                    continue;
                }
                else
                {
                    Passenger *to_be_served = security2.wait_queue.front();
                    security2.wait_queue.pop_front();
                    tot_waiting_time += current_time - to_be_served->arrival;
                    security2.all_servers[0]->busy = 1;
                    security2.all_servers[0]->temp_serving = temp;
                }
            }
            if (security2.all_servers[0]->temp_serving > 0)
            {
                security2.all_servers[0]->temp_serving--;
                security2.all_servers[0]->serving_time++;
            }
            tot_passengers += security2.wait_queue.size();
        }
        cout << "\tAverage Queue Length is = " << (double)tot_passengers / simulated_time << endl;
        cout << "\tAvgerage Waiting time is = " << (double)tot_waiting_time / total << endl;
        cout << "\tServer utilization is = "<< (double)security2.all_servers[0]->serving_time / simulated_time << endl;
        char req;
        cout<<"\tWant to try again(y/n): ";
        cin>>req;
		cout<<endl;
        if(req=='n') break;
    }
	printf("\n\n======== Multiple servers with infinite buffer capacity ========\n\n");
    while(1)
    {
        cout << "\tEnter simulating time:";
        cin >> simulated_time;
        tot_passengers = 0;
        tot_waiting_time = 0;
        int num_servers;
        cout<<"\tEnter the number of servers ";
        cin>>num_servers;
        security security3(-1,num_servers);
        current_time = 0;
        next_arrival_time = generateExponentialTime(arrival_rate);
        int tot_serving_time=0;
        int tot_serving_time2=0;
        int total=0;
        for (current_time = 0; current_time < simulated_time; current_time++)
        {
            if (current_time == next_arrival_time)
            {
                Passenger *new_passenger = new Passenger(current_time);
                security3.wait_queue.push_back(new_passenger);
                total++;
                while ((next_arrival_time = (generateExponentialTime(arrival_rate))) == 0)
                {
                    Passenger *new_passenger = new Passenger(current_time);
                    security3.wait_queue.push_back(new_passenger);
                    total++;
                }
                next_arrival_time += current_time;
            }
            for(int i=0;i<num_servers;i++)
            {
                if ((security3.all_servers[i]->busy == 0 || security3.all_servers[i]->temp_serving == 0) && security3.wait_queue.size() != 0)
                {
                    int temp;
                    while ((temp = (generateExponentialTime(dep_rate))) == 0 && security3.wait_queue.size() != 0)
                    {
                        Passenger *to_be_served = security3.wait_queue.front();
                        security3.wait_queue.pop_front();
                        tot_waiting_time += current_time - to_be_served->arrival;
                    }
                    if (security3.wait_queue.size() == 0)
                    {
                        continue;
                    }
                    else
                    {
                        Passenger *to_be_served = security3.wait_queue.front();
                        security3.wait_queue.pop_front();
                        tot_waiting_time += current_time - to_be_served->arrival;
                        security3.all_servers[i]->busy = 1;
                        security3.all_servers[i]->temp_serving = temp;
                    }
                }
            }
            int served=0;
            bool served2=false;
            for(int i=0;i<num_servers;i++)
            {
                if (security3.all_servers[i]->temp_serving > 0)
                {
                    if(served==0) served=1;
                    served2=true;
                    security3.all_servers[i]->temp_serving--;
                    security3.all_servers[i]->serving_time++;
                }
                else
                {
                    served=2;
                }
                if(security3.all_servers[i]->temp_serving==0)
                {
                    security3.all_servers[i]->busy=0;
                }
            }
            if(served==1)
            {
                tot_serving_time++;
            }
            if(served2==true)
            {
                tot_serving_time2++;
            }
            tot_passengers += security3.wait_queue.size();
        }
        cout << "\tAverage Queue Length is = " << (double)tot_passengers / simulated_time << endl;
        cout << "\tAvgerage Waiting time is = " << (double)tot_waiting_time / (total) << endl;
        cout << "\tServer utilization is = " << (double) (tot_serving_time+tot_serving_time2) / (2*simulated_time) << endl;
        char req;
        cout<<"\tWant to try again(y/n): ";
        cin>>req;
		cout<<endl;
        if(req=='n') break;
    }
	printf("\n\n======== Multiple servers with finite buffer capacity ========\n\n");
    while(1)
    {
        cout << "\tEnter simulating time:";
        cin >> simulated_time;
        tot_passengers = 0;
        tot_waiting_time = 0;
        int num_servers;
        cout << "\tEnter the number of servers ";
        cin >> num_servers;
        security security4(-1, num_servers);
        current_time = 0;
        next_arrival_time = (generateExponentialTime(arrival_rate));
        int buf_size;
        cout<<"\tEnter the buffer size ";
        cin>>buf_size;
        int tot_serving_time = 0;
        int tot_serving_time2=0;
        int total=0;
        for (current_time = 0; current_time < simulated_time; current_time++)
        {
            if (current_time == next_arrival_time && security4.wait_queue.size() < buf_size)
            {
                Passenger *new_passenger = new Passenger(current_time);
                security4.wait_queue.push_back(new_passenger);
                total++;
                while ((next_arrival_time = (generateExponentialTime(arrival_rate))) == 0 && security4.wait_queue.size()< buf_size)
                {
                    Passenger *new_passenger = new Passenger(current_time);
                    security4.wait_queue.push_back(new_passenger);
                    total++;
                }
                next_arrival_time += current_time;
            }
            for (int i = 0; i < num_servers; i++)
            {
                if ((security4.all_servers[i]->busy == 0 || security4.all_servers[i]->temp_serving == 0) && security4.wait_queue.size() != 0)
                {
                    int temp;
                    while ((temp = (generateExponentialTime(dep_rate))) == 0 && security4.wait_queue.size() != 0)
                    {
                        Passenger *to_be_served = security4.wait_queue.front();
                        security4.wait_queue.pop_front();
                        tot_waiting_time += current_time - to_be_served->arrival;
                    }
                    if (security4.wait_queue.size() == 0)
                    {
                        continue;
                    }
                    else
                    {
                        Passenger *to_be_served = security4.wait_queue.front();
                        security4.wait_queue.pop_front();
                        tot_waiting_time += current_time - to_be_served->arrival;
                        security4.all_servers[i]->busy = 1;
                        security4.all_servers[i]->temp_serving = temp;
                    }
                }
            }
            int served = 0;
            bool served2 = false;
            for (int i = 0; i < num_servers; i++)
            {
                if (security4.all_servers[i]->temp_serving > 0)
                {
                    if (served == 0)
                        served = 1;
                    served2 = true;
                    security4.all_servers[i]->temp_serving--;
                    security4.all_servers[i]->serving_time++;
                }
                else
                {
                    served = 2;
                }
                if (security4.all_servers[i]->temp_serving == 0)
                {
                    security4.all_servers[i]->busy = 0;
                }
            }
            if (served == 1)
            {
                tot_serving_time++;
            }
            if (served2 == true)
            {
                tot_serving_time2++;
            }
            tot_passengers += security4.wait_queue.size();
        }
        cout << "\tAverage Queue Length is = " << (double)tot_passengers / simulated_time << endl;
        //cout<<"total waiting time:"<<tot_waiting_time<<endl;
        cout << "\tAvgerage Waiting time is = " << (double)tot_waiting_time / total << endl;
        cout << "\tServer utilization is = " <<(double)(tot_serving_time+tot_serving_time2) / (2*simulated_time) << endl;
        char req;
        cout<<"\tWant to try again(y/n): ";
        cin>>req;
		cout<<endl;
        if(req=='n') break;
    }

}


