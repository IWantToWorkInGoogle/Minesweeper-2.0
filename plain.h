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
    SMALL,
    NORMAL,
    LARGE,
    CUSTOM
};

enum class Visibility : uint8_t {
    HIDDEN,
    FLAG,
    VISIBLE
};

enum class Status {
    RUNNING,
    WIN,
    LOSE
};

class Plain {
private:
    uint16_t height, width, rest, unmarked_mines;
    int number_of_mines;
    int16_t** plain;
    int8_t** visible;
    std::set<std::pair<uint8_t,uint8_t>> free_cells;
    bool exploded_mine;
public:
    explicit Plain(Level level);

    int16_t visibility(uint16_t x, uint16_t y);
    bool is_visible(uint16_t x, uint16_t y);
    void set_visibility(uint16_t x, uint16_t y, int8_t);
    int16_t cell(uint16_t x, uint16_t y);

    void open(uint16_t x, uint16_t y);
    void put_mark(uint16_t x, uint16_t y);
    void remove_mark(uint16_t x, uint16_t y);

    void show();
    void show_revealed();
    void show_visibility();
    void set_mines();
    void show_stats();
    Status result();
};

class SinglePlayer {
private:
    Plain plain;
    bool game_over;

    void open_cell(uint16_t x, uint16_t y);
    void put_mark(uint16_t x, uint16_t y);
    void remove_mark(uint16_t x, uint16_t y);
    void update();

    void show();
    void show_revealed();
    void show_visibility();
    void end_of_game();
public:
    explicit SinglePlayer(Level level);
    void run();
};

#endif //MINESWEEPER_PLAIN_H
