#pragma once

class Cell
{
public:
    virtual bool isNull() const = 0;
    virtual ~Cell() = default;
};

class NullCell : public Cell
{
public:
    bool isNull() const override { return true; }
};

class NumberCell : public Cell
{
public:
    NumberCell(int n) { number = n; }
    bool isNull() const override { return false; }
    int getNumber() const { return number; }
private:
    int number;
};
