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

#ifndef _EVERNOTEDATAPROVIDER_H_
#define _EVERNOTEDATAPROVIDER_H_

#include <Python.h>

class EvernoteDataProvider : public DataProvider {
private:
	bool hasOAuthToken;
	std::string authToken;

	PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;

public:
	int open ();
	int close ();
	int sync ();
	int login ();
	int logout ();

	void setHasOAuthToken (bool b) {
		hasOAuthToken = b;
	}

	bool getHasOAuthToken () {
		return hasOAuthToken;
	}

	void setAuthToken (std::string at) {
		authToken = at;
	}

	std::string getAuthToken () {
		return authToken;
	}
};

#endif