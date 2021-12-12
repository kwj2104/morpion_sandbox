//
// Created by Demo User on 12/6/21.
//

#ifndef SANDBOX__MAIN_H_
#define SANDBOX__MAIN_H_

#include <array>
#include <utility>

const int row_len = 9;
const int col_len = 9;
const int kNumCells = row_len * col_len;

struct Point {
  int x, y;
  Point() {}
  Point(int a, int b) { this->x = a; this->y = b; }

  bool operator==(const Point& other_point) {
    return (x == other_point.x) and (y == other_point.y);
  }

  bool operator<(const Point& other_point) const
  {
    if (x < other_point.x) {
      return true;
    }
    if (y < other_point.y){
      return true;
    }
    return false;
  }

  std::string toString(){
    return "[" + std::to_string(x) + "," + std::to_string(y) + "]";
  }
};


class Line {
 public:
  Line(Point p1, Point p2);

  bool isValid();
  std::string toString();
  std::vector<Point> getEndpoints();
  std::array<int, 2> getDirection();
  std::vector<Point> getAllPoints();
  std::vector<Line> getOverlaps();
  void addOverlap(Line l);
  bool operator==(Line other_line);
  bool checkOverlap(Line l);

 private:
  std::array<int, 2> direction_{};
  Point point1_;
  Point point2_;
  std::vector<Line> overlaps_;
  std::vector<Point> line_points_;
};

class Board {
 public:
  Board();

  void printPointBoard();
  std::vector<Line> getAllPossibleLines();
  std::vector<Line> getAllValidLines();
  void makeMove(Line l);

 private:
  std::array<int, kNumCells> board_ {};
  std::vector<Line> all_lines_;
  //std::vector<Line> valid_moves_;
  std::vector<Line> performed_moves_;
};

#endif //SANDBOX__MAIN_H_
