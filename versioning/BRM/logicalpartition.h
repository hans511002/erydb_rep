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

        EXPORT  uint16_t & operator [](int i) ;
        EXPORT  uint16_t get(int i) const;
        EXPORT  inline bool isMaster(uint16_t dbr=0) const{
            if(dbr==0){
                dbr=getPmDbr();
            }
            return dbr==dbRoots[0];
        };
        EXPORT   int remove(uint16_t dbr);
        EXPORT inline  int getLocalDbrIndex() const{
            int dbrIdx=-1;
            int dbr=getPmDbr(0,&dbrIdx);
            return dbrIdx;
        };
        EXPORT inline  int getDbrIndex(uint16_t dbr) const{
            if(dbr){
                int size = 0;
                while (size < MAX_DATA_REPLICATESIZE && dbRoots[size])
                {
                    if(dbr==dbRoots[size] )
                        return size;
                    size++;
                }
            }
            return -1;
        };
        EXPORT   uint16_t getPmDbr(uint16_t pmid=0,int * dbrIdx=0)const;
        EXPORT  std::vector<uint16_t> getPms() const;
        EXPORT  DBROOTS_struct(const DBROOTS_struct&);
        EXPORT  DBROOTS_struct& operator= (const DBROOTS_struct&);
        EXPORT  DBROOTS_struct& set(const DBROOTS_struct&);
        EXPORT  inline  int size() const {
            int size = 0;
            while (size < MAX_DATA_REPLICATESIZE && dbRoots[size])
            {
                size++;
            }
            return size;
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
inline std::ostream & operator<<(std::ostream & os, const std::vector<uint16_t>& vuint){
    for (int n=0; n<vuint.size(); n++){
        if(n>0){
            os<<",";
        }
        os <<  vuint[n]  ;
    }
	return os;
    };
	    
    struct FBO_struct {
        //DBROOTS_struct vbOids;
        uint32_t fbos[MAX_DATA_REPLICATESIZE];
    
        EXPORT FBO_struct(){
            memset(fbos,0,sizeof(BRM::FBO_struct));
            };
    	EXPORT inline FBO_struct(int32_t fbo){
            fbos[0] = fbo;
            fbos[1] = fbo;
            fbos[2] = fbo;
            fbos[3] = fbo;
        };
        EXPORT inline uint32_t & operator [](int i) {
            return fbos[i];
            };
        EXPORT inline  void set(int i,int32_t fbo){
            fbos[i]=fbo;
            };
        //EXPORT   int remove(uint16_t dbr){
        //    int i=vbOids.remove(dbr);
        //    if(i>=0){
        //        return remove(i);
        //    }
        //    return -1;
        //};
        EXPORT inline  int32_t remove(int i){
            assert(i<MAX_DATA_REPLICATESIZE);
            uint32_t fbo=fbos[i];
            int j=i;
            for (j=i; j < MAX_DATA_REPLICATESIZE-1 ; j++){
                if(!fbos[j+1])
                    break;
                fbos[j]=fbos[j+1];
            }
            fbos[j]=0;
            return fbo;
        };
        EXPORT inline FBO_struct(const FBO_struct& e){
            memcpy(&fbos,&e.fbos,sizeof(FBO_struct&));
        };
        EXPORT inline FBO_struct& operator= (const FBO_struct& e){
            memcpy(&fbos,&e.fbos,sizeof(FBO_struct&));
	        return *this;
	    };
        EXPORT inline FBO_struct& operator= (uint32_t fbo)
        {
            fbos[0] = fbo;
            fbos[1] = fbo;
            fbos[2] = fbo;
            fbos[3] = fbo;
            return *this;
        };
        EXPORT  inline  int size() const {
            int size = 0;
            while (size < MAX_DATA_REPLICATESIZE && fbos[size])
            {
                size++;
            }
            return size;
        };
    
    EXPORT void serialize(messageqcpp::ByteStream &bs) const{
        for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        bs<<fbos[n];
    }};
    EXPORT void deserialize(messageqcpp::ByteStream &bs){
        for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        bs>>fbos[n];
    }};
    };
    
inline EXPORT bool operator==( const BRM::FBO_struct& a, const BRM::FBO_struct& b){
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++)
    {
        if(a.fbos[n] != b.fbos[n]){
            return false;
        }
    }
    return true;
    };
inline EXPORT bool operator!=( const BRM::FBO_struct&a, const BRM::FBO_struct&b){return !(a==b);};
inline EXPORT bool operator>( const BRM::FBO_struct&a, const BRM::FBO_struct&b);
inline EXPORT bool operator>=( const BRM::FBO_struct&a, const BRM::FBO_struct&b);
inline EXPORT bool operator<( const BRM::FBO_struct&a, const BRM::FBO_struct&b);
inline EXPORT bool operator<=( const BRM::FBO_struct&a, const BRM::FBO_struct&b);
inline EXPORT std::ostream & operator<<(std::ostream & os, const FBO_struct &a){
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        if(n>0){
            os<<".";
        }
        os <<  a.fbos[n]  ;
    }
	return os;
	};
inline EXPORT std::istream & operator>>(std::istream & is, FBO_struct & a){
    for (int n=0; n<MAX_DATA_REPLICATESIZE; n++){
        if(n>0){
            is.ignore();
        }
        is >>  a.fbos[n] ;
    }
	return is;
	};
inline messageqcpp::ByteStream& operator<<(messageqcpp::ByteStream& bs, const FBO_struct& s){
    s.serialize(bs); return bs;
    };
inline messageqcpp::ByteStream& operator>>(messageqcpp::ByteStream& bs, FBO_struct& s){
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
