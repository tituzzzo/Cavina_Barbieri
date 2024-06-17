#include "text_table.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace tb {

void table_file_management::create_table() const
{
  std::ofstream file(name_file_, std::ios::out | std::ios::trunc);

  if (!file.is_open()) {
    throw std::runtime_error{"Text file not found."};
  }

  for (size_t i = 0; i < columns_.size(); ++i) {
    file << columns_[i];
    if (i != columns_.size() - 1) {
      file << "\t";
    }
  }
  file << "\n";

  file.close();
}

// to add a new raw
void table_file_management::insert_row_into_table(std::vector<std::string> const& datas) const
{
  std::ofstream file(name_file_, std::ios::out | std::ios::app);

  if (datas.size() > columns_.size()) {
    throw std::runtime_error{"Number of datas inserted is major than number of columns."};
  }

  if (!file.is_open()) {
    throw std::runtime_error{"Text file not found."};
  }

  for (size_t i = 0; i < datas.size(); ++i) {
    file << datas[i];
    if (i != datas.size() - 1) {
      file << "\t\t";
    }
  }
  file << "\n";

  file.close();
}
} // namespace tb