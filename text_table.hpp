#ifndef FILE_TABLE_HPP
#define FILE_TABLE_HPP
#include <string>
#include <vector>

namespace tb {

class table_file_management
{
 private:
  std::string name_file_;
  std::vector<std::string> columns_;

 public:
  table_file_management(std::string const& name_file,
                        std::vector<std::string> const& columns)
      : name_file_{name_file}
      , columns_{columns}
  {}

  void create_table() const;

  // to add a new row
  void insert_row_into_table(std::vector<std::string> const& datas) const;
};

} // namespace tb
#endif
