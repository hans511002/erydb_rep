/* Copyright (C) 2014 EryDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

/****************************************************************************
* $Id: func_microsecond.cpp 2477 2011-05-11 16:07:35Z chao $
*
*
****************************************************************************/

#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

#include "functor_int.h"
#include "funchelpers.h"
#include "functioncolumn.h"
#include "rowgroup.h"
using namespace execplan;

#include "dataconvert.h"

namespace funcexp
{

erydbSystemCatalog::ColType Func_microsecond::operationType( FunctionParm& fp, erydbSystemCatalog::ColType& resultType )
{
	return resultType;
}


int64_t Func_microsecond::getIntVal(rowgroup::Row& row,
						FunctionParm& parm,
						bool& isNull,
						erydbSystemCatalog::ColType& op_ct)
{
	int64_t val = 0;
	uint32_t microSecond = 0;
	
	switch (parm[0]->data()->resultType().colDataType)
	{
		case erydbSystemCatalog::DATE:
			val = parm[0]->data()->getIntVal(row, isNull);
			microSecond = 0;
			break;
		case erydbSystemCatalog::DATETIME:
			val = parm[0]->data()->getIntVal(row, isNull);
			microSecond = (uint32_t)((val & 0xfffff));
			break;
		case erydbSystemCatalog::CHAR:
		case erydbSystemCatalog::VARCHAR:
			val = dataconvert::DataConvert::stringToDatetime(parm[0]->data()->getStrVal(row, isNull));
			if (val == -1)
			{
				isNull = true;
				return -1;
			}
			else
			{
				microSecond = (uint32_t)((val & 0xfffff));
			}
			break;
		case erydbSystemCatalog::BIGINT:
		case erydbSystemCatalog::MEDINT:
		case erydbSystemCatalog::SMALLINT:
		case erydbSystemCatalog::TINYINT:
		case erydbSystemCatalog::INT:
			val = dataconvert::DataConvert::intToDatetime(parm[0]->data()->getIntVal(row, isNull));
			if (val == -1)
			{
				isNull = true;
				return -1;
			}
			else
			{
				microSecond = (uint32_t)((val & 0xfffff));
			}
			break;	
		case erydbSystemCatalog::DECIMAL:
			if (parm[0]->data()->resultType().scale == 0)
			{
				val = dataconvert::DataConvert::intToDatetime(parm[0]->data()->getIntVal(row, isNull));
				if (val == -1)
				{
					isNull = true;
					return -1;
				}
				else
				{
					microSecond = (uint32_t)((val & 0xfffff));
				}
			}
			break;
		default:
			isNull = true;
			return -1;
	}
	
	return microSecond;
}


} // namespace funcexp
// vim:ts=4 sw=4:
