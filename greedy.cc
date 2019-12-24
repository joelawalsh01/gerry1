#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>
#include <string>
#include <random>
#include <algorithm>
using namespace std;

class Voter {
private:
    int voter_id;
    int party_affiliation;
public:
    Voter(){}
    Voter(int voter_id, int party_affiliation)
            :voter_id(voter_id), party_affiliation(party_affiliation){}
    void print() {
        if (party_affiliation == +1) {
            cout << voter_id << ": " << "+" << endl;
        }
        else if (party_affiliation == 0) {
            cout << voter_id << ": " << "0" << endl;
        }
        else if (party_affiliation == -1) {
            cout << voter_id << ": " << "-" << endl;
        }
        else {
            cout << "Illegal affiliation value: " << voter_id << ": " << " Error in creating voter " << voter_id << endl;
        }
    }
    //accessor functions for private members
    int id(){
        return voter_id;
    }
    int affiliation(){
        return party_affiliation;
    }
    // creates a string representation of each voter. +1 is "+", -1 is "-"
    string string_representation(){
        string symbol;
        if (party_affiliation>0) symbol = "+";
        else symbol = "-";
        return symbol;
    }
};
class District {
private:
    vector<Voter> district;
    int maj,sum, numA, numB;  //holds sum of integers, number of voters in Party A, number of voters in party B
    string plusminus;
public:
    District(vector<Voter> voters)
            :district(voters){};
    District(Voter alone){
        district.push_back(alone);};

    //Returns total number of voters in vector of Voters
    int size(){
        return district.size();
    }
    //creates a int variable "num", and the  index points to a single voter. Sum is a running counter of each voter's integer.
    // Affiliation is a method of the voter class that returns an integer( +-1)
    int lean(){
        sum = 0;
        for (int num = 0; num < district.size() ; num ++){
            sum += district.at(num).affiliation();
        }

        // converts lean into +1 or -1 based on sign of "sum" (or 0 if no majority)
        int lean_result;
        if (sum>0) lean_result = 1;
        else if (sum<0) lean_result  = -1;
        else lean_result =0;
        return lean_result;
    }
    void push_back(Voter new_voter){
        district.push_back(new_voter);
    }
    // Iterates through the vector of voters.
    int majority(){
        numA = 0; numB = 0; maj = 0;

        for (int num = 0; num < district.size(); num ++){
            //counts B voters by keeping track of each negative entry
            if (district.at(num).affiliation()<0)
                numB++;
            else//counts Non-negative voters.
                numA++;
        }
        if (numA>numB){
            cout << "A majority with +" << numA<<endl;
            maj = numA;
        }
        else if (numB>numA){
            cout << "B majority with -" <<numB << endl;
            maj = -numB;
        }
        else {
            cout << "no majority: split evenly" << endl;
            maj = 0;
        }
        return maj;
    }

    int majoritycount(){
        int numA = 0, numB = 0, majcount = 0;

        for (int num = 0; num < district.size(); num ++){
            //counts B voters by keeping track of each negative entry
            if (district.at(num).affiliation()<0)
                numB++;
            else//counts Non-negative voters.
                numA++;
        }


        return abs(numB-numA);
    }
    //Appends + or - to a string based on the int affiliation from the Voter class
    string convert_to_string(){
        for (int num = 0 ; num < district.size() ; num++){
            if (district.at(num).affiliation()<0)
                plusminus += "-";
            else
                plusminus += "+";
        }
        cout << plusminus << endl;
        return plusminus;
    }
    void print(){
        cout<<"[";
        for (int i=0; i<district.size(); i++){
            cout<<district.at(i).affiliation()<<",";
        }
        cout <<"]";
    }


};

class Districting {
private:
    vector<District> districts;
public:
    Districting() {};

    Districting(vector<District> mydistricts)
            : districts(mydistricts) {};

    Districting(District new_district) {
        districts.push_back(new_district);
    };

    Districting(Voter new_voter) {
        District new_district(new_voter);
        districts.push_back(new_district);
    };

    /*constructor needs to take in voter string, assign Voter( position, string content), then add voter*/
    Districting(string voter) {
        Voter new_voter;
        if (voter.compare("+") == 0)
            new_voter = {1, +1};
        else new_voter = {1, -1};
        District new_district(new_voter);
        districts.push_back(new_district);
    };

    int size() { return districts.size(); }

    void extend_with_new_district(Voter new_voter) {
        District new_district(new_voter);
        districts.push_back(new_district);
    }

    void extend_last_district(Voter new_voter) {
        districts.at(districts.size() - 1).push_back(new_voter);
    }

    int lean() {
        int sum = 0;
        for (int num = 0; num < districts.size(); num++) {
            if (districts.at(num).lean() == 1)sum++;
            else sum--;
        }
        //Uses sum to determine lean, +1 for A and -1 for B.
        int lean_result;
        if (sum > 0) lean_result = 1;
        else if (sum < 0) lean_result = -1;
        else lean_result = 0;
        return lean_result;
    }

    int rule() {
        int sum = 0;
        for (int num = 0; num < districts.size(); num++) {
            if (districts.at(num).lean() == 1)sum++;
            else sum--;
        }
        return sum;
    }

    Districting push_back(District last) {
        districts.push_back(last);
        return Districting(districts);
    }

    District extract_district(int which_district) {
        return districts.at(which_district);
    }

    void print() {
        cout << "[";
        for (int i = 0; i < districts.size(); i++) {
            cout << districts.at(i).size();
            districts.at(i).print();
            cout << "," << endl;
        }
        cout << "]" << endl;
    }

    int  efficiency_gap() {

        int wasted_votes = 0;
        int wasteda;
        int wastedb;

        for (int i = 0; i < districts.size(); i++) {
            if (districts.at(i).lean() > 0) {
                wasteda += (districts.at(i).majoritycount() - 1);
            } else  {
                wastedb += (districts.at(i).majoritycount() - 1);
            }

        }
        double counter = 0;

        for (int j = 0; j < districts.size(); j++) {

            counter += districts.at(j).size();
        }


        double efficiency_gap;
        efficiency_gap =  (wasteda + wastedb) / counter;
        cout << "Wasted votes A: " << wasteda << endl;
        cout << "Wasted votes B: " << wastedb << endl;
        cout << "Efficiency gap: " << efficiency_gap<< endl;
    }
    int wasted_A() {

        int wasteda;
        int wasted_votes = 0;
        for (int i = 0; i < districts.size(); i++) {
            if (districts.at(i).lean() > 0) {
                wasteda += (districts.at(i).majoritycount() - 1);

            }

        }
        return wasteda;

    }

    int wasted_B() {

        int wastedb;
        int wasted_votes = 0;
        for (int i = 0; i < districts.size(); i++) {
            if (districts.at(i).lean() > 0) {
                wastedb += (districts.at(i).majoritycount() - 1);

            }

        }
        return wastedb;

    }


};


class Population {
private:
    vector<Voter> total;
    string total_string;
public:
    Population() {};

    Population(string onedist) { //constructor to add one district at a time
        for (int i = 0; i < onedist.size(); i++) {
            if (onedist.at(i) == '+') {
                total.push_back(Voter(i, +1));
            } else { total.push_back(Voter(i, -1)); }
        }
    };

    Population(Voter alone) { // constructor to add individual voters
        total.push_back(alone);
    };

    Population(
            vector<Voter> votergroup) {  //constructor to add vectors of Voters. Iterates through each voter and takes the string representation of that voters affiliation
        for (int eachvoter = 1; eachvoter < votergroup.size(); eachvoter++) {
            total.push_back(votergroup.at(eachvoter));
        }
    };

    Population(int population_size, int majority, bool trace = false) {
        int pos_goal, neg_goal;
        if (majority > 0) {
            pos_goal = (population_size + abs(majority)) / 2;
            neg_goal = (population_size - pos_goal);
        } else {
            neg_goal = (population_size + abs(majority)) / 2;
            pos_goal = (population_size - neg_goal);
        }
        for (int counter = 1; counter <= neg_goal; counter++) total.push_back(Voter(counter, -1));
        for (int counter = 1; counter <= pos_goal; counter++) total.push_back(Voter(counter, +1));
        std::random_device rd;
        std::shuffle(std::begin(total), std::end(total), rd);
    };

    // Returns size of the string using the built in string method .size()
    int size() {
        return total.size();
    }

    string convert_to_string() {
        for (int k = 0; k < total.size(); k++) {
            string total_string;
            total_string = "";
            if (total.at(k).affiliation() == 1) {
                total_string += "+";
            } else { total_string += "-"; }
        }
        return total_string;
    }

    // Returns lean by iterating through string total. If +/A is the entry at a specific index, add to int variable sum. Otherwise take away from sum.
    int rule() {
        int sum = 0;
        for (int num = 0; num < total.size(); num++) {
            if (total.at(num).affiliation() == 1)sum++;
            else sum--;
        }
        return sum;
    }

    vector<Voter> population_vector() {
        return total;
    }

    // modifies the existing Population by taking in a string of new "+-" 's
    Population add(string more) {
        for (int num = 0; num < more.size(); num++) {
            if (more.compare("+") == 0) {
                Voter newvoter(more.at(num), 1);
                total.push_back(newvoter);
            } else {
                Voter newvoter(more.at(num), -1);
                total.push_back(newvoter);
            }
        }
        Population output(total);
        return output;
    }

    //subsets Population. Output is still Populations, stored in subset variable
    Population sub(int lower, int upper) {
        vector<Voter> subset1;
        subset1.resize(upper - lower + 1);
        std::copy(total.begin() + lower, total.begin() + upper, subset1.begin());
        return subset1;
    }

    void print() {
        for (int k = 0; k < total.size(); k++) {
            cout << total.at(k).id() << ":" << total.at(k).affiliation() << " ";
        }
    }

    Voter at(int place) {
        return total.at(place);
    }

    int lean() {
        int sum = 0;
        for (int i = 0; i < total.size(); i++) {
            if (total.at(i).affiliation() > 0) sum++;
            else sum--;
        }
        if (sum > 0) return +1;
        else if (sum < 0) return -1;
        else return 0;
    }

    Districting minority_rules(int ndistricts) {
        vector<vector<Districting> > memo(total.size() + 1, vector<Districting>(ndistricts + 1));
        for (int people = 1; people <= total.size(); people++) {
            //cout << "filling out row " << people << endl;
            for (int num_dist = 1; num_dist <= ndistricts; num_dist++) {
                //impossible case due to more districts than people
                if (num_dist > people) break;
                //cout << "filling out row " << people << " column " << num_dist << endl;
                //everyone in one district
                if (num_dist == 1) {
                    vector<Voter> small;
                    small.resize(people);
                    std::copy(total.begin(), total.begin() + people, small.begin());
                    memo.at(people).at(num_dist) = Districting(District(small));
                }
                    //one person per district
                else if (people == num_dist) {
                    Districting individuals;
                    for (int num = 0; num < people; num++) {
                        individuals.push_back(District(total.at(num)));
                    }
                    memo.at(people).at(num_dist) = individuals;
                }
                    //base case of any other combinations of p and n
                else if (people == 3 && num_dist == 2) {
                    //Districting first_two_grouped, last_two_grouped;
                    int firsttwo, lasttwo;
                    Population total_pop;
                    Districting first_two_grouped = Districting(District(vector<Voter>{total.at(0), total.at(1)}));
                    first_two_grouped.push_back(District(total.at(2)));
                    Districting last_two_grouped = Districting(District(total.at(0)));
                    last_two_grouped.push_back(District(vector<Voter>{total.at(1), total.at(2)}));
                    firsttwo = first_two_grouped.rule();
                    lasttwo = last_two_grouped.rule();

                    if (total_pop.lean() > 0) {
                        if (firsttwo < lasttwo) memo.at(3).at(2) = first_two_grouped;
                        else memo.at(3).at(2) = last_two_grouped;
                    } else {
                        if (firsttwo < lasttwo) memo.at(3).at(2) = last_two_grouped;
                        else memo.at(3).at(2) = first_two_grouped;
                    }
                } else {//extended = (p-1, n) with p as part of nth, another = (p-1,n-1) with p as a new nth
                    Districting extended, another;
                    int erule, arule;
                    int max_size = 0.21 * total.size();
                    extended = memo.at(people - 1).at(num_dist);
                    extended.extend_last_district(total.at(people - 1));
                    erule = extended.rule();
                    another = memo.at(people - 1).at(num_dist - 1);
                    another.extend_with_new_district(total.at(people - 1));
                    arule = another.rule();
                    int district_size;
                    district_size = another.extract_district(another.size() - 1).size();
                    Population total_pop = Population(total);


                    if (erule == arule and total_pop.lean() > 0) {
                        if (extended.wasted_A() > another.wasted_A()) {
                            memo.at(people).at(num_dist) = extended;
                        }
                        if (another.wasted_A() >= extended.wasted_A()) {
                            memo.at(people).at(num_dist) = another;
                        }
                    } else if (erule == arule and total_pop.lean() < 0) {
                        if (extended.wasted_B() >= another.wasted_B()) {
                            memo.at(people).at(num_dist) = extended;
                        }
                        if (another.wasted_B() > extended.wasted_B()) {
                            memo.at(people).at(num_dist) = another;
                        }
                    } else if (total_pop.lean() > 0) {
                        if (erule < arule and district_size < max_size) memo.at(people).at(num_dist) = extended;
                        else memo.at(people).at(num_dist) = another;
                    } else if (total_pop.lean() < 0) {
                        if (erule < arule or district_size > max_size) memo.at(people).at(num_dist) = another;
                        else memo.at(people).at(num_dist) = extended;
                    }
                }
                //cout << "done" << endl;
                //memo.at(people).at(num_dist).print();
                //cout << "rule: " << memo.at(people).at(num_dist).rule() << endl;


            }
        }
        return memo.at(total.size()).at(ndistricts);
    }
};

int main() {
    /*
    Voter myvoter(1, +1), othervoter(2, -1), theothervoter(3, +1);
    vector<Voter> citizens = {myvoter, othervoter, theothervoter};
    myvoter.print();
    District nine(citizens);
    string result = nine.print();
    cout << "...size: " << nine.size() << endl;
    cout << "...lean: " << nine.lean() << endl;
    nine.majority();
    cout << "Making single voter population B"<< endl;
    Population people(myvoter);
    cout << "...size: " << people.size() << endl;
    cout << "...lean: " << people.lean() << endl;
    cout << "Making district ABA" << endl;
    District nine(vector<Voter>{{1, +1},{2, -1},{3, +1}});
    cout << "..size: " << nine.size() << endl;
    cout << "..lean: " << nine.lean() << endl;
    string pns("-++--");
    Population some(pns);
    cout << "Population from string " << pns << endl;
    cout << "...size: " << some.size() << endl;
    cout << "...lean: " << some.lean() << endl;
    Population group = some.sub(1, 3);
    cout << "sub population 1--3" << endl;
    cout << "...size: " << group.size() << endl;
    cout << "...lean: " << group.lean() << endl;
    Voter newvoter(6, -1);
    Districting newdistricts(newvoter);
    cout << "New district size is :" << newdistricts.size()<<endl;
    Population five("+++--");
    cout << "Redistricting population: " << endl ;
    five.print();
    cout << endl;
    Districting gerry;
    cout << "Start with an empty districting:" << endl;
    cout << "...number of districts: " << gerry.size() << endl;
    cout << "Add one voter..." << endl;
    gerry.extend_with_new_district(five.at(1));
    cout << "New size after adding newvoter: "<< gerry.size()<<endl;
    cout << "Add one A voter: " << endl;
    gerry.extend_last_district(five.at(0));
    cout << "...number of districts: " << gerry.size() << endl;
    cout << "...lean: "<< gerry.lean() << endl;
    cout << "Add two B districts:"  << endl;
    gerry.extend_with_new_district( Voter(3, -1)) ;
    gerry.extend_with_new_district( Voter(4, -1)) ;
    cout << "...number of districts: "<< gerry.size() << endl;
    cout << "...lean: " << gerry.lean()<< endl;
    */

    cout << "Population size: ";
    int size; cin >> size;
    cout << "Population rule: ";
    int rule; cin >> rule;

    Population tester(size, rule);
    cout << "Population with size "<< size << " and majority "<<rule <<endl;
    cout << "Redistricting population: " <<endl;
    //  tester.print();cout<<endl;
    cout << "...majority rule: " << tester.rule() <<endl;

    Districting mandering = tester.minority_rules(5);
    cout <<"rule: "<< mandering.rule()<<endl;
    mandering.print();
    mandering.efficiency_gap();

};
