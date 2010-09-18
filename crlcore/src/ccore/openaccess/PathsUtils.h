// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-09-17 17:22:49" - PathsUtils.h
// x-----------------------------------------------------------------x
// |  This file is part of the hurricaneAMS Software.                |
// |  Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                    Jean-Manuel Caba              |
// |  E-mail      :       Jean-Manuel.Caba@asim.lip6.fr              |
// x-----------------------------------------------------------------x
#ifndef __PATHSUTILS_H__
#define __PATHSUTILS_H__

#include <iostream>

using namespace std;
#include  <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

        /**
           handling realpath
        */
        static void realPath(string& pathToChange){
            if(bfs::path::default_name_check_writable())
                bfs::path::default_name_check(bfs::portable_posix_name);
            bfs::path resolvedPath = pathToChange;
            pathToChange = system_complete(resolvedPath).string();
        }

        /**
           suppose the path has been resolved with system_complete
           before calling this function and path are posix_name
           then split the path in boos::filesystem::path corresponding of each dir
           from most root parent to leaf dir
           @see create_all_dirs
        */
        static std::vector<bfs::path> split_in_dirs(const bfs::path& p){
            string pstr(p.string());
            register size_t len(pstr.length());
            register char delim('/');
            register size_t firstDelim=0;
            register size_t secondDelim=1;
            vector<bfs::path> dirs;
            while(firstDelim < len){
                while(secondDelim < len && pstr[secondDelim]!=delim)
                    secondDelim++;
                string dir = pstr.substr(0,secondDelim);
                if(dir.empty())
                    break;
                dirs.push_back(bfs::path(dir));
                firstDelim = secondDelim;
                secondDelim++;
            }
            return dirs;
        }


        /**
           work around for boost::filesystem::create_directories
           missing in old boost versions like 1.33.1
           and equivalent to recursivly creating directories
           instead this is done iteratively.
        */
        static void create_all_dirs(const bfs::path& p){
            if(p.empty() || bfs::exists(p))
                return;
            std::vector<bfs::path> test;
            test = split_in_dirs(p);
            std::vector<bfs::path>::iterator it = test.begin();
            for(;it != test.end();it++){
                if(it->empty() || bfs::exists(*it))
                    continue;
                bfs::create_directory(*it);
            }
        }

#endif//__PATHSUTILS_H__
