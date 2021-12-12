#include <iostream>
#include <vector>
#include <utility>
#include "main.h"
#include <algorithm>
#include <random>

// Board Methods
Board::Board() {

  // Initialize 4T starting points and find all possible lines on the board
  for (int i = 0; i < row_len; i++){
    for (int j = 0; j < col_len; j++) {

      // Initialize starting points
      if ((i == 1 or i == 7) and j > 2 and j < 6) {
        board_[j + (i * row_len)] = 1;
      }
      if ((i == 2 or i == 6) and (j == 3  or  j == 5)) {
        board_[j + (i * row_len)] = 1;
      }
      if ((i == 3 or i == 5) and ((j > 0  and  j < 4) or (j > 4  and  j < 8))) {
        board_[j + (i * row_len)] = 1;
      }
      if (i == 4 and ((j == 1) or (j == 7))) {
        board_[j + (i * row_len)] = 1;
      }


      // Get all possible lines on board
      if (j + 3 < col_len) {
        all_lines_.push_back(Line(Point(i, j), Point(i, j + 3)));
      }
      if ((j + 3 < col_len) and (i + 3 < row_len)) {
        all_lines_.push_back(Line(Point(i, j), Point(i + 3, j + 3)));
      }
      if (i + 3 < row_len) {
        all_lines_.push_back(Line(Point(i, j), Point(i + 3 , j)));
      }
      if ((j >= 3) and (i + 3 < row_len)) {
        all_lines_.push_back(Line(Point(i, j), Point(i + 3, j - 3)));
      }
    }
  }

  // For each line, find all possible overlapping lines on current board
  for (Line &line: all_lines_) {
    for (Line line2: all_lines_) {
      if (line.checkOverlap(line2)){
        line.addOverlap(line2);
      }
    }
  }
}

std::vector<Line> Board::getAllPossibleLines() {
  return all_lines_;
}

// Generate all valid lines / moves in current board state
std::vector<Line> Board::getAllValidLines(){

  std::vector<Line> current_valid_lines_ = {};
  for (Line l : all_lines_) {
    // Check that exactly one point is empty
    int count = 0;
    for (Point p : l.getAllPoints()) {
      if (board_[p.y + (p.x * row_len)] == 1) {
        count++;
      }
    }
    if (count != 3) {
      continue;
    }

    // Check that line does not overlap any existing moves / lines
    bool overlaps = false;
    for (Line m : performed_moves_) {
      for (Line o : m.getOverlaps()) {
        if (l == o) {
          overlaps = true;
        }
      }
    }
    if (overlaps) {
      continue;
    }

    current_valid_lines_.push_back(l);
  }

  return current_valid_lines_;
}

void Board::makeMove(Line l) {
  for (Point p : l.getAllPoints()){
    board_[p.y + (p.x * row_len)] = 1;
  }
  performed_moves_.push_back(l);
}

void Board::printPointBoard() {
  for (int i = 0; i < row_len; i++){
    for (int j = 0; j < col_len; j++) {
      std::cout << board_[i * row_len + j];
    }
    std::cout << "\n";
  }
}

// Line Methods
Line::Line(Point point1, Point point2) {

  if (point1 < point2) {
    point1_ = point1;
    point2_ = point2;
  } else {
    point1_ = point2;
    point2_ = point1;
  }

  // TODO: figure out why this doesn't work
//  point1_ = point1;
//  point2_ = point2;
//  point1_ < point2_ ? point1_ = point2 : point2_ = point1; // sort if needed

  // Categorize line in one of four directions ([0, 1], [1, 1], [1, -1], [1, 0])
  direction_[0] = int((point2.x - point1.x)/3);
  direction_[1] = int((point2.y - point1.y)/3);

  // Get all points in line (beyond the two initial endpoints) and sort
  for (int i = 0; i < 4; i++) {
    line_points_.push_back(Point(point1_.x + i*direction_[0], point1.y + i*direction_[1]));
  }
  std::sort(line_points_.begin(), line_points_.end());

}

bool Line::checkOverlap(Line l){
  // Only check for overlapping points for lines in the same direction
  if (direction_ != l.getDirection()) {
    return false;
  }

  // Check if it's the same line
  if ((point1_ == l.getEndpoints()[0]) and (point2_ == l.getEndpoints()[1])) {
    return false;
  }

  // Record overlapping line if other line already includes current checked line
  for (Line line : l.getOverlaps()){
    if ((point1_ == line.getEndpoints()[0]) and (point2_ == line.getEndpoints()[1])) {
      return true;
    }
  }

  // Otherwise check for overlapping points between the two lines
  std::vector<Point> intersect = {};
  std::vector<Point> l_points = l.getAllPoints();
  std::set_intersection(l_points.begin(), l_points.end(), line_points_.begin(), line_points_.end(), std::back_inserter(intersect));
  if (!intersect.empty()) { // line is overlapping if >=1 intersecting points in 4D version
    return true;
  }

  return false;
}

std::vector<Point> Line::getEndpoints(){
  return std::vector<Point> {point1_, point2_};
}

bool Line::operator==(Line other_line){
  return (point1_ == other_line.getEndpoints()[0]) and (point2_ == other_line.getEndpoints()[1]);
}

std::vector<Line> Line::getOverlaps() {
  return overlaps_;
}

void Line::addOverlap(Line l){
  overlaps_.push_back(l);
}

std::array<int, 2> Line::getDirection(){
  //return std::to_string(direction_[0]) + std::string(",") + std::to_string(direction_[1]);
  return direction_;
}

std::vector<Point> Line::getAllPoints(){
  return line_points_;
}

bool Line::isValid()  {
  int xdiff = abs(point1_.x - point2_.x);
  int ydiff = abs(point1_.y - point2_.y);
  int totdiff = xdiff + ydiff;
  return (xdiff == 3 or ydiff == 3) and (totdiff == 3 or totdiff == 6);
}

std::string Line::toString() {
  return "(" + point1_.toString() + " " + point2_.toString() + ")";
}

//void printDisplayBoard() {
//  for (int i = 0; i < row_len; i++){
//    for (int j = 0; j < col_len; j++) {
//      std::cout << displayBoard[i][j];
//    }
//    std::cout << "\n";
//  }
//}

std::vector<std::vector<Point>> getAllMoves(){
  std::vector<std::vector<Point>> moveList;
  return moveList;
}

int main() {

  bool terminal = false;
  Board b = Board();
  int move_counter = 0;

  while (!terminal){
    std::vector<Line> validMoves = b.getAllValidLines();
    auto numMoves = validMoves.size();

    // Check if board is out of moves
    //if ((numMoves == 0) or (move_counter == 4)) {
    if (numMoves == 0) {
      terminal = true;
    }

    // Generate a random number to pick a move
    // std::random_device rand_dev;
    // std::mt19937 generator(rand_dev());
    // std::uniform_int_distribution<int>  distr(0, numMoves - 1);
    // auto selectedMove = validMoves[distr(generator)];
    Line selectedMove = validMoves[0];
    b.makeMove(selectedMove);
    move_counter++;

    std::cout << "Move " + std::to_string(move_counter) + ": " + selectedMove.toString() << "\n";
    //b.printPointBoard();
    //std::cout << "\n";
  }

  return 0;
}