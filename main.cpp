#include <iostream>
#include <cstring>
#include <fstream>
#include <list>
#include <map>
#include <stack>

#define MAX_NAME_LEN 1000
using namespace std;

class Scheduler{

  map<string, bool> runned_map;
  map< string, list<string> > dependency_map;
  int num_tasks;
  int num_actors;
  bool* actor_status;

public:
  Scheduler(int num_actors, string fname);
  void ParseFile(const char* fileName);
  void print_process_status(void);
  void print_actor_status(void);
  void print_dependency_map(void);
  void plan_tasks(void);
  void plan_tasks(string taskName, stack<string>& taskStack);
};

Scheduler::Scheduler(int num_actors, string fname) {
  this->num_actors = num_actors;
  actor_status = new bool[num_actors];
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
    list<string> curr_dependency;
    while(tmp != NULL) {
      tmp = strtok(NULL, ":, ");
      if(tmp != NULL)
        curr_dependency.push_back(string(tmp));
    }

    dependency_map[current_task_name] = curr_dependency;
  }

  map< string, list<string> >::iterator itr;
  for(itr = dependency_map.begin(); itr != dependency_map.end(); itr++) {
    // set all process to "not runned status"
    runned_map[itr->first] = false;
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

void Scheduler::plan_tasks(string taskName, stack<string>& taskStack) {
  taskStack.push(taskName);
  // runned_map[taskName] = true; // push all tasks into a stack, not considering task has been excuted
  if(dependency_map[taskName].empty()) {
    return;
  } else {
    for(list<string>::iterator itr = dependency_map[taskName].begin(); itr != dependency_map[taskName].end(); itr ++) {
      // if(!runned_map[*itr]) {
        plan_tasks(*itr, taskStack);
      // }
    }
  }
}

void printStack(stack<string> taskStack) {
  stack<string> a = taskStack;
  while(!a.empty()) {
    cout << a.top() << endl;
    a.pop();
  }
}

void Scheduler::plan_tasks(void) {
  for(map< string, list<string> >::iterator itr = dependency_map.begin(); itr != dependency_map.end(); itr ++) {
    if(itr->second.empty()) {

    }
  }
}

int main() {
  Scheduler a(2, "test.txt");
  // a.ParseFile("test.txt");
  // a.print_dependency_map();
  stack<string> taskStack;
  a.plan_tasks("task_name", taskStack);
  printStack(taskStack);
}
