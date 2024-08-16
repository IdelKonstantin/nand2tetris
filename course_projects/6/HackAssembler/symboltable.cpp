#include "symboltable.h"

SymbolTable::SymbolTable() {}

const std::map<std::string, int>& SymbolTable::getData() const {

    return m_symbolMap;
}

void SymbolTable::addLableIfNotExists(std::pair<std::string, int> newAntry) {

    //Если это L-команда, то вставляем в символьную таблицу метку и
    //номер строки, следующей за ней минус m_lablesCounter

    auto lable = newAntry.first;
    auto addr = newAntry.second;

    if(m_symbolMap.find(lable) == m_symbolMap.end()) {

        m_symbolMap.insert({lable, addr - m_lablesCounter});
        m_lablesCounter++;
    }
}

void SymbolTable::addVariabeIfNotExists(const std::string& variable) {

    //Если это А-команда, то проверяем, есть ли в символьной таблице такая переменная
    //Но только, если это не число, добавляем в символьную таблицу начиная с адреса m_availableRAMAdress == 16

    try {
        std::stoi(variable);
        return;
    }
    catch (const std::invalid_argument& ex) {

        if(m_symbolMap.find(variable) == m_symbolMap.end()) {
            m_symbolMap.insert({variable, m_availableRAMAdress});
            m_availableRAMAdress++;
        }
    }
}
