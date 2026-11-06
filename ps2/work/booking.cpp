#include <vector>

using namespace std;

struct Booking {
  int room;
  int start;
  int end;
};

class Bookings {
public:
  // Each booking creates a start event (+room) and an end event (-room).
  void load(vector<int>& time, vector<int>& room, const vector<Booking>& books) {
    time.reserve(books.size() * 2);
    room.reserve(books.size() * 2);
    for (const Booking& book : books) {
      time.push_back(book.start);
      room.push_back(book.room);
      time.push_back(book.end);
      room.push_back(-book.room);
    }
  }

  vector<Booking> mergeBooking(const vector<Booking>& b1, const vector<Booking>& b2) {
    vector<int> left_time, left_room;
    vector<int> right_time, right_room;
    load(left_time, left_room, b1);
    load(right_time, right_room, b2);

    vector<Booking> res;
    size_t left = 0;
    size_t right = 0;
    int current_room = 0;
    int previous_time = 0;
    bool has_previous_time = false;

    while (left < left_time.size() || right < right_time.size()) {
      int current_time;
      if (right == right_time.size() ||
          (left < left_time.size() && left_time[left] < right_time[right])) {
        current_time = left_time[left];
      } else {
        current_time = right_time[right];
      }
      // The count before this event applies to [previous_time, current_time).
      if (has_previous_time && current_room > 0 && previous_time < current_time) {
        if (!res.empty() && res.back().end == previous_time &&
            res.back().room == current_room) {
          res.back().end = current_time;
        } else {
          res.push_back({current_room, previous_time, current_time});
        }
      }
      // Apply all events at this time together, including adjacent bookings.
      while (left < left_time.size() && left_time[left] == current_time)
        current_room += left_room[left++];
      while (right < right_time.size() && right_time[right] == current_time)
        current_room += right_room[right++];

      previous_time = current_time;
      has_previous_time = true;
    }
    return res;
  }
};

class Reservations {
public:
  struct Reservation {
    int start;
    int end;
  };

  // Each recursive call returns its own complete, ordered schedule.
  vector<Booking> mergeSort(const vector<Reservation>& requests, size_t left, size_t right) {
    if (right - left == 1) {
      const Reservation& request = requests[left];
      return {{1, request.start, request.end}};
    }

    size_t middle = left + (right - left) / 2;
    vector<Booking> b1 = mergeSort(requests, left, middle);
    vector<Booking> b2 = mergeSort(requests, middle, right);
    Bookings bookings;
    return bookings.mergeBooking(b1, b2);
  }

  vector<Booking> satisfyingBooking(const vector<Reservation>& requests) {
    if (requests.empty()) {
      return {};
    }
    return mergeSort(requests, 0, requests.size());
  }
};
