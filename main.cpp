#include <iostream>
#include <set>
#include <string>
#include <variant>
#include <vector>

using namespace std;

typedef variant<int, string> entry;

struct entry_string {
  std::string operator()(const std::string &x) const { return x; }
  std::string operator()(int x) const { return std::to_string(x); }
};

struct Relation {
  using OutputType = vector<entry>;
  vector<string> schema;
  set<OutputType> data;

  Relation(){};
  Relation(vector<string> schema) : schema(schema), data({}){};
  Relation(vector<string> schema, set<OutputType> data)
      : schema(schema), data(data){};

  /* Pretty print relation in CSV format. */
  void print() {
    for (string label : schema) {
      cout << label << ", ";
    }
    cout << "\n";
    for (OutputType row : data) {
      for (entry cell : row) {
        cout << visit(entry_string(), cell) << ", ";
      }
      cout << "\n";
    }
  }
};

struct Operator : public Relation {
  Operator(){};
  Operator(vector<string> schema) : Relation(schema){};
};

struct Project : public Operator {
  using OutputType = vector<entry>;

  Relation input;
  set<pair<string, string>> projections;

  Project(Relation input, set<pair<string, string>> projections)
      : input(input), projections(projections) {
    vector<string> schema = {};
    set<OutputType> data;

    /* Define new schema based on given projection and keep the
       corresponding columns. */
    vector<int> indicies = {};
    for (pair<string, string> projection : projections) {
      /* Add projected label to schema vector. */
      schema.push_back(projection.second);

      /* Find the index corresponding to the column which is to
       * be projected. */
      int i = 0;
      for (string label : input.schema) {
        if (strcmp(label.c_str(), projection.first.c_str()) == 0) {
          indicies.push_back(i);
          break;
        }
        i++;
      }
    }

    /* Extract the projected columns using the indicies found above. */
    for (OutputType row : input.data) {
      OutputType newRow = {};
      for (int index : indicies) {
        newRow.push_back(row[index]);
      }
      data.insert(newRow);
    }

    /* Set parent struct attributes according to projection. */
    this->schema = schema;
    this->data = data;
  };
};

int main() {
  vector<string> schema = {"id", "name", "address"};
  set<vector<entry>> data = {{1, "lucas", "matchbox"}, {2, "holger", "huxley"}};

  Relation users(schema, data);
  users.print();

  Relation usernames = Project(
      users, {{"id", "user_id"}, {"name", "username"}, {"address", "office"}});
  usernames.print();

  return 0;
}