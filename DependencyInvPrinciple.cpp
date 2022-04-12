/*

********Dependency inversion principle*********

 A. High-level modules should not depend on low-level modules.
    Both should depend on abstractions.
 B. Abstractions should not depend on details. 
    Details should depend on abstractions.

• Low-level classes implement basic operations such as working with a disk, 
  transferring data over a network, connecting to a database, etc.  
• High-level classes contain business logic that directs low-level classes to do something.   

*/

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

/***
Here we are designing a software to find the 
relationships between different people
***/

/*
 Relationships between people
*/
enum class Relationship
{
  parent,
  child,
  sibling
};

struct Person
{
  string name;
};

/* 
RelationshipBrowser is high -level abstraction
It has pure virtual functions which the low-level components will implement.
*/
struct RelationshipBrowser // high level abstract class 
{
  virtual vector<Person> find_all_children_of(const string& name) = 0;
};


/*
Relationships is a low level class that implements basic operations like
add_parent_and_child method. It implements the high level abstraction
RelationshipBrowser in accordance with the dependency inversion principle 
which requires the low-level components to be dependent on high level abstractions.
*/
struct Relationships : RelationshipBrowser
{
  vector<tuple<Person, Relationship, Person>> relations;

  void add_parent_and_child(const Person& parent, const Person& child)
  {
    relations.push_back({parent, Relationship::parent, child});
    relations.push_back({child, Relationship::child, parent});
  }

  vector<Person> find_all_children_of(const string &name) override
  {
    vector<Person> result;
    for (auto&& [first, rel, second] : relations)
    {
      if (first.name == name && rel == Relationship::parent)
      { 
        result.push_back(second);
      }
    }
    return result;
  }
};

/* 
 Research is a high-level module that composes the 
 high level abstract class RelationshipBrowser - in 
 accordance with the dependency inversion principle.

 It's function is to find the relationships
 Here as an example, we print the children of John 
*/
struct Research 
{
  Research(RelationshipBrowser& browser)
  {
    for (auto& child : browser.find_all_children_of("John"))
    {
      cout << "John has a child called " << child.name << endl;
    }
  }

/*
The following constructor is not compliant with the dependency inversion principle.
High-level module (Research) is dependent on a low-level module (Relationships).
The problem with this approach is - any future changes to Relationships will
effect the Research module because of this tight coupling. 
So, we can instead make it dependent on the interface
which is implemented by the low level module as in the above constructor.
*/  
//  Research(const Relationships& relationships)
//  {
//    auto& relations = relationships.relations;
//    for (auto&& [first, rel, second] : relations)
//    {
//      if (first.name == "John" && rel == Relationship::parent)
//      {
//        cout << "John has a child called " << second.name << endl;
//      }
//    }
//  }
};

int main()
{
  Person parent{"John"};
  Person child1{"Chris"};
  Person child2{"Matt"};

  Relationships relationships;
  relationships.add_parent_and_child(parent, child1);
  relationships.add_parent_and_child(parent, child2);

  Research _(relationships);

  return 0;
}