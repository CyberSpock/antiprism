/*
   Copyright (c) 2020, Adrian Rossiter

   Antiprism - http://www.antiprism.com

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

      The above copyright notice and this permission notice shall be included
      in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.
*/

/*!\file iteration.cc
   \brief iteration control and support
*/

#include "../base/iteration.h"

#include <limits>
#include <stdarg.h>
#include <stdio.h>

namespace anti {

const unsigned int IterationControl::unlimited =
    std::numeric_limits<unsigned int>::max();

Status IterationControl::check_reporting()
{
  Status stat;
  if (max_iters == unlimited && status_period_iters <= 0)
    stat.set_warning("unlimited iterations but no status checking, iteration "
                     "may not terminate");
  return stat;
}

Status IterationControl::set_max_iters(int iters)
{
  Status stat;
  if (iters < -1)
    stat.set_error("number of iterations for status cannot be less than -1");
  else if (iters == -1)
    max_iters = unlimited;
  else
    max_iters = iters;

  if (stat)
    stat = check_reporting();

  return stat;
}

Status IterationControl::set_status_period_iters(int iters)
{
  Status stat;
  if (iters < -1)
    stat.set_error("number of iterations for status cannot be less than -1");
  else
    status_period_iters = iters;

  if (stat)
    stat = check_reporting();

  return stat;
}

Status IterationControl::set_sig_digits(int digits)
{
  Status stat;
  if (digits < 0)
    stat.set_warning("termination limit is negative");
  else if (digits > DEF_SIG_DGTS)
    stat.set_warning("termination limit is very small, may not be attainable");
  sig_digits = digits;
  return stat;
}

int IterationControl::print(const char *fmt, ...) const
{
  int ret = 0;
  va_list args;
  va_start(args, fmt);
  ret = vfprintf(stream, fmt, args);
  va_end(args);
  return ret;
}

}; // namespace anti
