//
// Created by mufti on 19.12.2023.
//
#ifndef MINESWEEPER_PLAIN_H
#define MINESWEEPER_PLAIN_H

#include <cstdint>
#include <set>
#include <queue>
#include <utility>
#include <iostream>
#include <random>
#include <chrono>

enum class Level : uint8_t {
    EASY,
    NORMAL,
    HARD,
    CUSTOM
};

enum class Visibility : uint8_t {
    HIDDEN,
    FLAG,
    VISIBLE
};

class Plain {
private:
    uint16_t height, width, rest;
    int number_of_mines;
    int16_t** plain;
    int8_t** visible;
    std::set<std::pair<uint8_t,uint8_t>> free_cells;
public:
    explicit Plain(Level level);

    int16_t visibility(uint16_t x, uint16_t y);
    bool is_visible(uint16_t x, uint16_t y);
    void set_visibility(uint16_t x, uint16_t y, int8_t);
    int16_t cell(uint16_t x, uint16_t y);

    bool open(uint16_t x, uint16_t y);
    bool put_flag(uint16_t x, uint16_t y);

    void show();
    void show_revealed();
    void set_mines();
};

class SinglePlayer {
private:
    Plain plain;
    bool game_over;

    void open_cell(uint16_t x, uint16_t y);
    void put_flag(uint16_t x, uint16_t y);

    void show();
    void show_revealed();
public:
    SinglePlayer(Level level);
    void run();
};

#endif //MINESWEEPER_PLAIN_H
