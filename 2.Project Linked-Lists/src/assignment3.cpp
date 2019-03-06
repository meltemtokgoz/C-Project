#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include <map>
using namespace std;
/******************** goal ***************************************/
typedef struct goal
{
    string awayTeam;
    int minute;
    int id;
    goal *next;
    goal *prev;
};
/**************** SECOND LINKED LIST ******************************/
class linked_list_goals
{
    public:
        goal *head;
        goal *tail;
    public:
        linked_list_goals()
        {
            head = NULL;
            tail = NULL;
        }
    /***********************************************************/
    void add_node(string a_team, int mnt , int m_id)
    {
        goal *temp = new goal;
        temp->awayTeam = a_team;
        temp->id = m_id;
        temp->minute = mnt;
        temp->next = NULL;

        if(head == NULL)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            tail->next = temp;
            tail = tail->next;
        }
    }
    /***********************************************************/
    int totalFirst(){
        goal *temp = head;
        int first = 0;
        while (temp != NULL)
        {
            if(temp->minute <= 45){
                first = first +1;
            }
            temp = temp->next;
        }
        return first;
    }
    /***********************************************************/
    int totalSecond(){
        goal *temp = head;
        int second = 0;
        while (temp != NULL)
        {
            if(temp->minute > 45){
                second = second +1;
            }
            temp = temp->next;
        }
        return second;
    }
    /***********************************************************/
    int totalGoals(){
        goal *temp = head;
        int goal = 0;
        while (temp != NULL)
        {
            //cout << temp->awayTeam << "," << temp->minute << "," << temp->id << endl;
            temp = temp->next;
            goal = goal + 1;
        }
        return goal;
    }
    /***********************************************************/
    bool hattrickGoals(){
        goal *temp = head;
        int goal = 0;
        map<int,int> my_map;
        while (temp != NULL)
        {
            map<int,int>::iterator it;
            it = my_map.find(temp->id);
            if (it != my_map.end()){
                my_map[temp->id] ++;
            }
            else{
                 my_map.insert(std::pair<int,int>(temp->id,1));
            }
            temp = temp->next;
        }
        map<int,int>::iterator i;
        for (i = my_map.begin(); i != my_map.end(); ++i){
            if(i->second >= 3){
                return true;
                break;
            }
        }
        return false;
    }
    /***********************************************************/
    void printGoal(string name,ofstream& outfile){
        goal *temp = head;
        while (temp != NULL)
        {
            outfile << "Footballer Name: " << name << "," << "Away Team: " << temp->awayTeam <<","
                    <<"Min of Goal: " << temp->minute <<","<<"Match ID: " <<temp->id  <<endl;
            temp = temp->next;
        }
    }
    /***********************************************************/
    void matchIDasc(string name,ofstream& outfile){
        list<int> idListAsc;
        goal *temp = head;
        while (temp != NULL)
        {
            std::list<int>::iterator it;
            it = std::find(idListAsc.begin(), idListAsc.end(), temp->id);
            if(it != idListAsc.end()){
            }
            else{
                idListAsc.push_back(temp->id);
            }
            temp = temp->next;
        }
        idListAsc.sort();
        //idListAsc.sort(std::greater<int>());
        for(std::list<int>::iterator i=idListAsc.begin(); i != idListAsc.end(); ++i)
            outfile << "footballer Name: " << name << "," <<"Match ID: " << *i  <<endl;
    }
    /***********************************************************/
    void matchIDdsc(string name,ofstream& outfile){
        list<int> idListDsc;
        goal *temp = head;
        while (temp != NULL)
        {
            std::list<int>::iterator it;
            it = std::find(idListDsc.begin(), idListDsc.end(), temp->id);
            if(it != idListDsc.end()){
            }
            else{
                idListDsc.push_back(temp->id);
            }
            temp = temp->next;
        }
        idListDsc.sort(std::greater<int>());
        for(std::list<int>::iterator i=idListDsc.begin(); i != idListDsc.end(); ++i)
            outfile << "footballer Name: " << name << "," <<"Match ID: " << *i << endl;
    }
    /***********************************************************/

};
/*************** footballer ********************************************/
typedef struct footballer
{
    string fName;
    string teamName;
    footballer *next;
    goal *head_goal;
    linked_list_goals list2;
};
/********************* FIRST LINKED LIST ********************************/
class linked_list_footballer
{
    private:
        footballer *head;
        footballer *tail;
    public:
        linked_list_footballer()
        {
            head = NULL;
            tail = NULL;
        }
    /***********************************************************/
    void addNode(string name, string team, string a_team, int mnt, int m_id)
    {
        //this footballer first add linked list
        if(check(name) != true){
            footballer* temp = new footballer;
            temp->fName = name;
            temp->teamName = team;
            temp->next = NULL;
            temp->list2.add_node(a_team,mnt,m_id);
            temp->head_goal = temp->list2.head;

            //empty list add node
            if(head == NULL)
            {
                head = temp;
            }
            else{
                footballer* current = head;
                footballer* trail = NULL;
                //trail is previous current
                //here find correct location for new node(temp)
                while(current != NULL){
                    if(current->fName >= temp->fName){
                        break;
                    }
                    else{
                        trail = current;
                        current = current->next;
                    }
                }
                //insert node at the head of the list
                if(current == head){
                    temp->next = head;
                    head = temp;
                }
                //insert node after the head
                else{
                    temp->next = current;
                    trail->next = temp;
                }
            }
        }
        //this footballer exist in linked list
        else{
            footballer *find_person = NULL;
            footballer *f = head;
            while (f != NULL){
                if (f->fName == name){
                    find_person = f;
                    break;
                }
                else{
                    f = f->next;
                }
            }
            find_person->list2.add_node(a_team,mnt,m_id);
        }
    }
    /***********************************************************/
    bool check(string name){
        footballer *temp = head;
        while (temp != NULL){
            if (name != temp->fName){
                temp = temp->next;
            }
            else{
                return true;
                break;
            }
        }
    }
    /***********************************************************/
    void mostScore(ofstream& outfile){
        int firsthalf = 0;
        int secondhalf = 0;
        footballer *temp = head;
        while (temp != NULL){
            firsthalf += temp->list2.totalFirst();
            secondhalf += temp->list2.totalSecond();
            temp = temp->next;
        }
        if(firsthalf > secondhalf){
            outfile << "0" << endl;
        }
        else{
            outfile << "1" << endl;
        }
    }
    /***********************************************************/
    void bestScore(ofstream& outfile){
        footballer *temp = head;
        int best = 0;
        while (temp != NULL){
            int number = temp->list2.totalGoals();
            if(number > best){
                best = number;
            }
            temp = temp->next;
        }
        goalScorer(best, outfile);
    }
    /***********************************************************/
    void goalScorer(int best, ofstream& outfile){
        footballer *temp = head;
         while (temp != NULL){
             int number = temp->list2.totalGoals();
             if(number == best){
                outfile << temp->fName << endl;
            }
            temp = temp->next;
         }
    }
    /***********************************************************/
     void hattrick(ofstream& outfile){
        footballer *temp = head;
        while (temp != NULL){
            bool isH = temp->list2.hattrickGoals();
            if(isH == true){
                outfile << temp->fName << endl;
            }
            temp = temp->next;
        }
    }
    /***********************************************************/
    void printTeam(ofstream& outfile){
        list<string> teamList;
        footballer *temp = head;
        while (temp != NULL)
        {
            std::list<string>::iterator it;
            it = std::find(teamList.begin(), teamList.end(), temp->teamName);
            if(it != teamList.end()){
            }
            else{
                teamList.push_back(temp->teamName);
            }
            temp = temp->next;
        }
        //print team list
        for(std::list<string>::iterator i=teamList.begin(); i != teamList.end(); ++i)
            outfile << *i <<endl;
    }
    /***********************************************************/
    void printFootballer(ofstream& outfile)
    {
        footballer *temp;
        temp = head;
        while (temp != NULL)
        {
            outfile << temp->fName << endl;
            temp = temp->next;
        }
    }
    /***********************************************************/
    void matchesF(string name, ofstream& outfile){
        //cout << f1 << "," << f2 <<endl;
        footballer *find_person = NULL;
        footballer *f = head;
        while (f != NULL){
            if (f->fName == name){
                find_person = f;
                break;
            }
            else{
                f = f->next;
            }
        }
        outfile << "Matches of " << find_person->fName << endl;
        find_person->list2.printGoal(name,outfile);
    }
    /***********************************************************/
    void mIDasc(string name,ofstream& outfile){
        footballer *find_person = NULL;
        footballer *f = head;
        while (f != NULL){
            if (f->fName == name){
                find_person = f;
                break;
            }
            else{
                f = f->next;
            }
        }
        find_person->list2.matchIDasc(name,outfile);
    }
    /***********************************************************/
     void mIDdsc(string name,ofstream& outfile){
        footballer *find_person = NULL;
        footballer *f = head;
        while (f != NULL){
            if (f->fName == name){
                find_person = f;
                break;
            }
            else{
                f = f->next;
            }
        }
        find_person->list2.matchIDdsc(name,outfile);
    }
};
/***********************MAIN************************************/
int main(int argc, char** argv) {

    string line, line2;
    ifstream inputFile, inputOperations;
    inputFile.open(argv[1]);
    inputOperations.open(argv[2]);
    ofstream outfile;
    outfile.open (argv[3]);

    linked_list_footballer list1;
    if (inputFile.is_open()){
        while(getline(inputFile,line))
		{
            stringstream ss(line);
            string name,team,a_team,time_s,id_s;
            int time, match_id;
            getline(ss, name, ',');
            getline(ss, team, ',');
            getline(ss, a_team, ',');
            getline(ss, time_s, ',');
            getline(ss, id_s);

            time = atoi(time_s.c_str());
            match_id = atoi(id_s.c_str());
            //check line
            //cout << name << "," << team << "," << a_team << "," << time << "," << match_id << "\n";
            list1.addNode(name,team,a_team,time,match_id);
		}
    }

    outfile << "1)THE MOST SCORED HALF"<< endl;
    list1.mostScore(outfile);
    outfile << "2)GOAL SCORER" << endl;
    list1.bestScore(outfile);
    outfile << "3)THE NAMES OF FOOTBALLERS WHO SCORED HAT-TRICK" << endl;
    list1.hattrick(outfile);
    outfile << "4)LIST OF TEAMS" << endl;
    list1.printTeam(outfile);
    outfile << "5)LIST OF FOOTBALLERS" << endl;
    list1.printFootballer(outfile);

    if (inputOperations.is_open()){
		for (int lineno = 0; getline (inputOperations,line2) && lineno < 3; lineno++){
            stringstream ss2(line2);
            string f1, f2;
            getline(ss2, f1, ',');
            getline(ss2, f2);
            if (lineno == 0){
                outfile << "6)MATCHES OF GIVEN FOOTBALLER" << endl;
                list1.matchesF(f1,outfile);
                list1.matchesF(f2,outfile);
            }
            else if(lineno == 1){
                outfile << "7)ASCENDING ORDER ACCORDING TO MATCH ID" << endl;
                list1.mIDasc(f1,outfile);
                list1.mIDasc(f2,outfile);
            }
            else if(lineno == 2){
                outfile << "8)DESCENDING ORDER ACCORDING TO MATCH ID" << endl;
                list1.mIDdsc(f1,outfile);
                list1.mIDdsc(f2,outfile);
            }
		}
    }
    inputFile.close();
    inputOperations.close();
    outfile.close();
}
 /****************** END *****************************************/


