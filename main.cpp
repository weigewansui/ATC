#include <iostream>
#include <cstring>
#include <fstream>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <set>
#include <algorithm>

#define MAX_NAME_LEN 1000
using namespace std;

struct task_t {
  string name;
  int depth;
}; // record the depth in order to optimize the sequence of execution

class Scheduler{

  map<string, bool> runned_map;
  map< string, list<string> > dependency_map;
  map< string, set<string> > fathers_map;
  list<string> to_run_list;
  int num_tasks;
  int num_actors;
  bool* actor_status;
  queue<string>* actor_queues;
public:
  Scheduler(int num_actors, string fname);
  void ParseFile(const char* fileName);
  void print_process_status(void);
  void print_actor_status(void);
  void print_dependency_map(void);
  void plan_tasks(void);
  void plan_tasks(string taskName, stack<task_t>& taskStack, int depth);
  void print_father_map(void);
  void get_task(void);
  void find_leaf_util(string name, int& queueNo);
  void print_actor_schedule(void);
};

Scheduler::Scheduler(int num_actors, string fname) {
  this->num_actors = num_actors;
  actor_status = new bool[num_actors];
  actor_queues = new queue<string>[2];
  for (int i = 0; i < num_actors; i ++) {
    actor_status[i] = false;
  }
  if(!fname.empty()) {
    ParseFile(fname.c_str());
  }
}

void Scheduler::ParseFile(const char* fileName) {
  fstream f(fileName, f.in);
  // count lines in the file
  char buff[MAX_NAME_LEN];
  int num_lines = 0;
  while(f.getline(buff, MAX_NAME_LEN)) {
    ++ num_lines;
  }
  // move pointer to beginning
  f.clear();
  f.seekg(0, f.beg);


  while(f.getline(buff, MAX_NAME_LEN)) {
    // split each line
    char* tmp = strtok(buff, ":, ");
    string current_task_name = string(tmp);
    to_run_list.push_back(string(tmp));
    list<string> curr_dependency;
    while(tmp != NULL) {
      tmp = strtok(NULL, ":, ");
      if(tmp != NULL) {
        curr_dependency.push_back(string(tmp));
        fathers_map[string(tmp)].insert(current_task_name);
      }
    }

    dependency_map[current_task_name] = curr_dependency;
  }

  map< string, list<string> >::iterator itr;
  for(itr = dependency_map.begin(); itr != dependency_map.end(); itr++) {
    // set all process to "not runned status"
    runned_map[itr->first] = false;
  }

}
void Scheduler::print_father_map(void) {
  for(map< string, set<string> >::iterator itr = fathers_map.begin(); itr != fathers_map.end(); itr ++) {
    cout << itr->first << endl;
    for(set<string>::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2 ++) {
      cout <<"----" << *itr2 << endl;
    }
  }
}

void Scheduler::print_dependency_map(void) {
  map< string, list<string> >::iterator itr;
  for(itr = dependency_map.begin(); itr != dependency_map.end(); itr++) {
    cout << itr->first <<"depends on:" <<endl;

    list <string>::iterator itr2;
    for(itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2 ++) {
      cout <<" " << *itr2 <<endl;
    }
  }
}

void print_list(list<string> l) {
  for(list<string>::iterator itr = l.begin(); itr != l.end(); itr ++) {
    cout << *itr <<"=>";
  }
  cout << endl;
}
void Scheduler::find_leaf_util(string name, int& queueNo) {
    map< string, list<string> > tmp = dependency_map; // list remove will cause pointer issue so copy it for iteration
    bool leaf = true;
    for(list<string>::iterator itr = tmp[name].begin(); itr != tmp[name].end(); itr ++) {
      if(find(to_run_list.begin(), to_run_list.end(), *itr) == to_run_list.end()) continue;
      leaf = false;
      cout << "Checking task: " << *itr <<endl;
      find_leaf_util(*itr, queueNo);
    }
    if(leaf) {
        cout << "LEAF: " << name << endl;
        cout << "pushing task " << name << " to Actor " << queueNo <<endl;
        actor_queues[queueNo].push(name);
        to_run_list.remove(name);
        if(queueNo == num_actors - 1) {
          queueNo = 0;
        } else {
          queueNo ++;
        }
    }
}

void Scheduler::print_actor_schedule(void) {
  for (int i = 0; i < num_actors; i ++) {
    while(!actor_queues[i].empty()) {
      cout << actor_queues[i].front() <<"\t";
      actor_queues[i].pop();
    }
    cout<<endl;
  }
}

void Scheduler::get_task(void) {
  int queueNo = 0;
  while(!to_run_list.empty())
    find_leaf_util("task_name", queueNo);
}


int main() {
  Scheduler a(2, "test.txt");
  a.get_task();
  a.print_actor_schedule();
}
