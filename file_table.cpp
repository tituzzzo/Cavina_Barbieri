#include "file_table.hpp"
#include <fstream>
#include <iostream>
#include <error>
namespace fl{
// Metodo per creare l'intestazione della tabella
void table_file_management::create_table() const{
    std::ofstream file(name_file_, std::ios::out | std::ios::app); // Aprire il file in modalità append

    if (!file.is_open()) {
        throw std::runtime_error{"table_file not found"};
    }

    for (size_t i = 0; i < columns_.size(); ++i) {
        file << columns_[i];
        if (i != columns_.size() - 1) {
            file << "\t"; // Separatore di tabulazione
        }
    }
    file << "\n";

    file.close();
}

// Metodo per inserire una riga di dati nella tabella
void table_file_management::insert_datas_into_tables(const std::vector<std::string>& datas) const{
    std::ofstream file(name_file_, std::ios::out | std::ios::app); // Aprire il file in modalità append
    if (datas.size()>columns_.size()){
        throw std::runtime_error{"number of datas inserted is major than number of columns"}
    }

    if (!file.is_open()) {
        throw std::runtime_error{"table_file not found"};
    }

    for (size_t i = 0; i < datas.size(); ++i) {
        file << datas[i];
        if (i != datas.size() - 1) {
            file << "\t"; // Separatore di tabulazione
        }
    }
    file << "\n";

    file.close();
}
}