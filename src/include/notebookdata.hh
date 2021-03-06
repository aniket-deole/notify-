/***
Copyright (C) 2013 Aniket Deole <aniket.deole@gmail.com>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License version 2.1, as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranties of
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef _NOTEBOOKDATA_HH_
#define _NOTEBOOKDATA_HH_

class NotebookData {
private:
  int primary_key;
   
  std::string title;
  std::string guid;
  std::string stack;
  long int created;
  long int updated;

  bool isStack;

public:

  NotebookData () {}
  /* id integer primary key, title text unique, guid text, parent_guid text, created_time datetime, modified_time datetime */
  NotebookData (int p_key, std::string t, std::string g, std::string s, long int c_time, long int u_time, bool i_s = false) {
    primary_key = p_key;
    title = t;
    stack = s;
    guid = g;
    created = c_time;
    updated = u_time;
    isStack = i_s;
  }

  int getPrimaryKey () { return primary_key; }
  std::string getTitle () { return title; }
  std::string getGuid () { return guid; }
  std::string getStack () { return stack; }
  void setGuid (std::string g) { guid = g; }
  bool getIsStack () { return isStack; }
	std::string getInsertStatement () {
		std::string query = "INSERT INTO notebooks values (NULL, '" + title + "','";
			query += guid;
			query += "', '" + stack+ "', 0, 0, 0, 0)";
		return query;

	}
};


#endif
