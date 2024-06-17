#ifndef FILE_TABLE_HPP
#define FILE_TABLE_HPP
#include <string>
#include <vector>

namespace fl {

class table_file_management {
private:
    std::string name_file_;
    std::vector<std::string> columns_;
public:
    // Costruttore
    table_file_management::table_file_management(const std::string& name_file, std::vector<std::string> columns) : name_file_{name_file}, columns_{columns} {}

    // Metodo per creare l'intestazione della tabella
    void create_table() const;

    // Metodo per inserire una riga di dati nella tabella
    void insert_datas_into_tables(const std::vector<std::string>& datas)const;
};


} // namespace fl
#endif
