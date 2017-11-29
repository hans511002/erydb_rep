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
 * $Id: logicalpartition.h 1823 2013-01-21 14:13:09Z rdempsey $
 *
 *****************************************************************************/

/** @file 
 * Put LogicalPartition define here to avoid header file include confilct.
 * This struct will be used in connector, brm and writeengine
 */

#ifndef LOGICALPARTITION_H_
#define LOGICALPARTITION_H_

#include "bytestream.h"
#include "liboam.h"

#if defined(_MSC_VER) && defined(xxxBRMTYPES_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace BRM 
{
    union DBROOTS_struct {
    uint16_t	dbRoots[MAX_DATA_REPLICATESIZE];
    uint64_t dbRoot;

	EXPORT DBROOTS_struct();
	EXPORT DBROOTS_struct(uint64_t dbRoot){
	    dbRoots[0]=dbRoot&0xFFFF;
	    dbRoots[1]=(dbRoot>>16) &0xFFFF;
	    dbRoots[2]=(dbRoot>>32) &0xFFFF;
	    dbRoots[3]=(dbRoot>>48) &0xFFFF;
	};
    EXPORT   uint64_t getUintVal() {
        uint64_t dbRoot=0;
        dbRoot=(uint64_t)dbRoots[3]<<48 + (uint64_t)dbRoots[2]<<32 + (uint64_t)dbRoots[1]<<16 + (uint64_t)dbRoots[0];
        return dbRoot;
    };

    EXPORT   uint16_t & operator [](int i) ;
    EXPORT   uint16_t get(int i) const;
    EXPORT   bool isMaster(uint16_t dbr) const{
        return dbr==dbRoots[0];
        };
    EXPORT   void remove(uint16_t dbr);
    EXPORT   uint16_t getPmDbr(uint16_t pmid=0)const;
    EXPORT DBROOTS_struct(const DBROOTS_struct&);
    EXPORT DBROOTS_struct& operator= (const DBROOTS_struct&);
    EXPORT DBROOTS_struct& set(const DBROOTS_struct&);
    EXPORT inline  int size() const {
        int size = 0;
        while (size < MAX_DATA_REPLICATESIZE && dbRoots[size])
        {
            size++;
        }
    };

    EXPORT void serialize(messageqcpp::ByteStream &bs) const;
	EXPORT void deserialize(messageqcpp::ByteStream &bs);
};

EXPORT bool operator==( const BRM::DBROOTS_struct&, const BRM::DBROOTS_struct&);
EXPORT bool operator!=( const BRM::DBROOTS_struct&, const BRM::DBROOTS_struct&);
EXPORT bool operator>( const BRM::DBROOTS_struct&, const BRM::DBROOTS_struct&);
EXPORT bool operator>=( const BRM::DBROOTS_struct&, const BRM::DBROOTS_struct&);
EXPORT bool operator<( const BRM::DBROOTS_struct&, const BRM::DBROOTS_struct&);
EXPORT bool operator<=( const BRM::DBROOTS_struct&, const BRM::DBROOTS_struct&);
EXPORT std::ostream & operator<<(std::ostream &, const DBROOTS_struct &);
EXPORT std::istream & operator>>(std::istream &, DBROOTS_struct &);
inline messageqcpp::ByteStream& operator<<(messageqcpp::ByteStream& bs, const DBROOTS_struct& s){
    s.serialize(bs); return bs;
    };
inline messageqcpp::ByteStream& operator>>(messageqcpp::ByteStream& bs, DBROOTS_struct& s){
    s.deserialize(bs); return bs;
    };

// Logical partition number descriptor
struct LogicalPartition
{
	DBROOTS_struct dbRoot;  // dbroot #
	uint32_t pp;      // physical partition #
	uint16_t seg;     // segment #

	LogicalPartition() : dbRoot ((uint16_t)-1),pp ((uint32_t)-1),seg ((uint16_t)-1) {}

	LogicalPartition(DBROOTS_struct& d, uint32_t p, uint16_t s) : dbRoot(d),pp(p),seg(s){}
	
	bool operator<( const LogicalPartition &n ) const
	{
		return ((pp < n.pp) ||
		        (pp == n.pp && seg < n.seg) ||
		        (pp == n.pp && seg == n.seg && dbRoot < n.dbRoot));
	}

	void serialize(messageqcpp::ByteStream& b) const
	{
		b << dbRoot;
		b << (uint32_t)pp;
		b << (uint16_t)seg;
	}

	void unserialize(messageqcpp::ByteStream& b)
	{
		b >> dbRoot;
		b >> (uint32_t&)pp;
		b >> (uint16_t&)seg;
	}
	
	/** @bug4816. For output to user purpose */
	std::string toString() const;
};

/**
 * ostream operator
 */
std::ostream& operator<<(std::ostream& output, const LogicalPartition& rhs);
std::istream& operator>>(std::istream& input, LogicalPartition& rhs);

/**
 * bytestream operator
 */
messageqcpp::ByteStream& operator<<(messageqcpp::ByteStream &bs, const LogicalPartition& rhs);

messageqcpp::ByteStream& operator>>(messageqcpp::ByteStream &bs, LogicalPartition& rhs);

}
#include "oamcache.h"

#endif
