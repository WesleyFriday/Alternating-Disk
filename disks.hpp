///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_DARK, DISK_LIGHT };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_DARK) {

      assert(light_count > 0);

      for (size_t i = 1; i < _colors.size(); i += 2) {
        _colors[i] = DISK_LIGHT;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream v;
    bool first = true;
    for (auto color : _colors) {

      if (!first) {
        v << " ";
      }

      if (color == DISK_LIGHT) {
        v << "L";
      } else {
        v << "D";
      }

      first = false;
    }
    return v.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is dark, the second disk at index 1
  // is light, and so on for the entire row of disks.
  bool is_alternating() const {
    for(int i = 0; i < _colors.size(); i++){

        if(i%2 == 0){
          if(_colors[i] == DISK_LIGHT){return false;}
        }

        else{
          if(_colors[i] == DISK_DARK){return false;}
        }
    }
    return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the right (high indices) and all dark disks on the left (low
  // indices).
  bool is_sorted() const {
    int time = 0;

    if(_colors[0] == DISK_LIGHT){return false;}

    for(int i = 0; i < _colors.size()-1; i++){

      if(_colors[i] != _colors[i+1]){
        time++;
      if(time > 1){return false;}

      }
    }
    return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
  // TODO: Write code for this function, including rewriting the return
  // statement, and then delete these comments.
  assert(before.is_alternating());

  disk_state tmp = before;
  int swapcounter = 0;
  int current = 0, next = 1;
  while(!tmp.is_sorted()){
    if(tmp.get(current) == DISK_LIGHT && tmp.get(next) == DISK_DARK){
      tmp.swap(current);
      swapcounter++;
    }
    current++; next++;
    if(current == tmp.total_count() - 2  || next == tmp.total_count() ){
      current = 0; next = 1;
    }
  }
  return sorted_disks(tmp, swapcounter);

}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  // check that the input is in alternating format
  assert(before.is_alternating());

  disk_state tmp = before;
  int swapbl = 0;
  while(!tmp.is_sorted()){

    for(int j= 0; j < tmp.total_count()-1; j++){
      if(tmp.get(j) == DISK_LIGHT && tmp.get(j+1) == DISK_DARK){
        //who knows if ths works
        tmp.swap(j);
        swapbl++;
      }
    }

    for(int j = before.total_count()-1; j > 0; j--){
      if(tmp.get(j) == DISK_DARK && tmp.get(j-1) == DISK_LIGHT){
        tmp.swap(j-1);
        swapbl++;
      }
    }
  }

   return sorted_disks(tmp,swapbl);

}
