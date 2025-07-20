#include "request.h"

Request::Request(std::string in, std::string out, int proc_time, int arrival)
    : ip_in(in), ip_out(out), process_time(proc_time),
      arrival_time(arrival), assigned_time(-1), processed(false) {}
