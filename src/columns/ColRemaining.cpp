////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 - 2021, Tomas Babej, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// https://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <Context.h>
#include <Datetime.h>
#include <Duration.h>
#include <shared.h>
#include <format.h>
#include <utf8.h>
#include <stdlib.h>
#include <ColRemaining.h>


////////////////////////////////////////////////////////////////////////////////
ColumnRemaining::ColumnRemaining ()
{
  _name      = "time";
  _type      = "duration";
  _style     = "countdown";
  _label     = "Remaining";
}

////////////////////////////////////////////////////////////////////////////////
bool ColumnRemaining::validate (const std::string& value) const
{
  // No restrictions.
  if (_values.size () == 0)
    return true;

  // Look for exact match value.
  for (auto& i : _values)
    if (i == value)
      return true;

  // Fail if not found.
  return false;
}

////////////////////////////////////////////////////////////////////////////////
// Set the minimum and maximum widths for the value.
//
void ColumnRemaining::measure (Task& task, unsigned int& minimum, unsigned int& maximum)
{
  minimum = maximum = 0;
  if (task.has (_name))
  {
    if (_style == "default")
    {
      auto value = task.get (_name);
      if (value != "")
        minimum = maximum = Duration (value).formatISO ().length ();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void ColumnRemaining::render (
  std::vector <std::string>& lines,
  Task& task,
  int width,
  Color& color)
{
  if (task.has (_name))
  {
    if (_style == "default")
    {
      auto value = task.get (_name);
      Duration dur(value);

      if (task.has ("start")) {
	      Datetime started (task.get ("start"));
	      Duration remaining = dur.toTime_t() - Duration(Datetime() - started).toTime_t();
	      if (remaining.toTime_t() < 0)
		      renderStringLeft (lines, width, color, "Time up");
	      else
		      renderStringLeft (lines, width, color, remaining.formatVague());
      } else {
	      renderStringLeft (lines, width, color, Duration (value).formatVague ());
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
