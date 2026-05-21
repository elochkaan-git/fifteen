#pragma once
#include "board.h"

class BoardFactory
{
public:
    virtual Board* create() const = 0;
    virtual ~BoardFactory() = default;
};

class ClassicBoardFactory : public BoardFactory
{
public:
    Board* create() const override {
        return new Board(4, 4);
    }
};

class LargeBoardFactory : public BoardFactory
{
public:
    Board* create() const override {
        return new Board(8, 8);
    }
};

class SmallBoardFactory : public BoardFactory
{
public:
    Board* create() const override {
        return new Board(3, 3);
    }
};

class RectangleBoardFactory : public BoardFactory
{
public:
    Board* create() const override {
        return new Board(3, 5);
    }
};

class CustomBoardFactory : public BoardFactory
{
public:
    CustomBoardFactory(int rows, int cols) : rows(rows), cols(cols) {}
    Board* create() const override {
        return new Board(rows, cols);
    }
private:
    int rows, cols;
};
