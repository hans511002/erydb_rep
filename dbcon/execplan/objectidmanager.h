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

/******************************************************************************
 * $Id: objectidmanager.h 9744 2013-08-07 03:32:19Z bwilkinson $
 *
 *****************************************************************************/

/** @file 
 * class ObjectIDManager interface. 
 */

#ifndef OBJECTIDMANAGER_H_
#define OBJECTIDMANAGER_H_

#include <string>
#include <stdint.h>
#include "dbrm.h"

namespace execplan {

/** @brief The class that manages the Object ID space
 *
 * The class that manages the allocation and deallocation of IDs
 * for objects in the database.
 */
class ObjectIDManager
{

public:
	/** @brief Default constructor
	 *
	 * @note Throws ios::failure on a file IO error
	 */
	ObjectIDManager();
	virtual ~ObjectIDManager();

	/** @brief Allocate one Object ID
	 *
	 * @return The allocated OID (>= 0) on success, -1 on error.
	 */
	int allocOID();

	/** @brief Allocate a contiguous range of Object IDs
	 *
	 * @param num The number of contiguous Object IDs to allocate
	 * @return The first OID allocated on success, -1 on failure
	 */
	int allocOIDs(int num);

	/** @brief Allocates a new oid for a version buffer file, associates it
	 * with dbroot.
	 *
	 * @return Returns -1 on all errors.
	 */
	// int allocVBOID(uint32_t dbroot);

	/** @brief Returns the DBRoot of the given version buffer OID, or -1 on error. */
	int getDBRootOfVBOID(uint32_t vboid);

	/** @brief Returns the VB OID -> DB Root mapping.  ret[0] = dbroot of VB OID 0
	 *
	 * @return vector where index n = dbroot of VBOID n.
	 * @throw runtime_exception on error
	 */
	std::vector<uint16_t> getVBOIDToDBRootMap();
	/** @brief Return an OID to the pool
	 *
	 * @param oid The OID to return
	 */
	void returnOID(int oid);

	/** @brief Return a list of OIDs to the pool
	 *
	 * @param start The first OID to return
	 * @param end The last OID to return
	 */
	void returnOIDs(int start, int end);

	/** @brief Counts the number of allocated OIDs
	 *
	 * @note This currently assumes the bitmap length is a multiple of 4096
	 * @return The number of allocated OIDs
	 */
	int size();

	/** @brief Get the OID bitmap filename
	 */
	const std::string getFilename() const;

private:
	std::string fFilename;
	BRM::DBRM dbrm;
     
};

}	// namespace

#endif
