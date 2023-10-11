#include <iostream>
#include <string>
#include <tuple>
#include <set>

using namespace std;

template <size_t I = 0, typename... Tp>
void printTuple(std::tuple<Tp...> &t)
{
  std::cout << std::get<I>(t) << "\t";
  // do things
  if constexpr (I + 1 != sizeof...(Tp))
    printTuple<I + 1>(t);
}

template <typename... types>
struct Relation
{
  using OutputType = tuple<types...>;
  array<string, sizeof...(types)> schema;
  set<OutputType> data;
  Relation(){};
  Relation(array<string, sizeof...(types)> schema, set<OutputType> data) : schema(schema), data(data){};

  void printRelation()
  {
    cout << "";

    for (string label : schema)
      cout << label << "\t";

    cout << "\n";

    for (OutputType row : data)
    {
      printTuple(row);
      cout << "\n";
    }
  }
};

template <typename... types>
struct Operator : public Relation<types...>
{
};

template <typename InputOperator, typename... types>
struct Project : public Operator<types...>
{
  InputOperator input;
  set<pair<string, string>> projections;

  Project(InputOperator input, set<pair<string, string>> projections) : input(input), projections(projections)
  {

    array<string, sizeof...(types)> new_schema = {};
    int counter = 0;
    for (string label : this->schema)
    {
      for (pair<string, string> projection : projections)
      {
        if (strcmp(projection.first.c_str(), label.c_str()) != 0)
        {
          new_schema[counter] = projection.second;
          counter++;
        }
      }
    }
    this->schema = new_schema;
    this->data = {
        make_tuple("lucas"),
        make_tuple("holger"),
    };
  };
};

int main()
{

  Relation<int, string> users(
      {"id", "name"},
      {
          make_tuple(1, "lucas"),
          make_tuple(2, "holger"),
      });

  Relation<string> usernames = Project<decltype(users), string>(users, {{"name", "username"}});

  users.printRelation();
  cout << "\n";
  usernames.printRelation();
  cout << "\n";

  return 0;
}