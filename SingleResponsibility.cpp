/* 
  ***Single Responsibility Principle***
  - Each class must have only one reason to change
  - Limit the possibility of potential code breakage with new requirements

  In the following example, the Journal class should only be responsible for
  add new entries(strings) . Saving the journal should be a seperate concern
  which should be handled by another class.
  This will ensure that any changes to saving the journal in the future will
  not impact the journal class.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Journal
{
  string title;
  vector<string> entries;

  explicit Journal(const string& title)
    : title{title}
  {
  }

  void add(const string& entry);

  // persistence is a separate concern
  // we should not add this here - as we are giving an additional
  // responsibility to the Journal class
  void save(const string& filename);
};

void Journal::add(const string& entry)
{
  static int count = 1;
  entries.push_back(std::to_string(count++)
    + ": " + entry);
}

void Journal::save(const string& filename)
{
  ofstream ofs(filename);
  for (auto& s : entries)
    ofs << s << endl;
}

/*
 Moving the save responsibility to a seperate class
 as the persistence manager grows i.e add new journals or other types that need to be saved,
 all changes will happen here and not in the journal class.
 if we want to change from filestreams to databases, even that change can happen now without touching
 the journal class
*/

/*
PersistenceManager has the single responsibility of saving the journal 
which it takes in as parameter in the constructor.

Any future changes to the save method will have no effect on the journal class.
*/
struct PersistenceManager
{
  static void save(const Journal& j, const string& filename)
  {
    ofstream ofs(filename);
    for (auto& s : j.entries)
      ofs << s << endl;
  }
};

int main()
{
  Journal journal{"Dear Diary"};
  journal.add("I ate a bug");
  journal.add("I cried today");

  //journal.save("diary.txt");

  PersistenceManager pm;
  pm.save(journal, "diary.txt");

  return 0;
}