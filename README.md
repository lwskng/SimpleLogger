# Simple thread safe C++20 Logger

## Purpose
Consider this as a little helper for developer.
Use this if you need to log and you don't want to bring a heavy (or another) dependency into your project. 
Keep in mind it has the simpliest functionality - just writes thread-safely to ostream&. 

**Header only, naturally.**

## Details
As for C++20 it uses:
- jthread (demo purpose only)
- ostreamsync
- C++20 Lambda capture

## Usage
Just copy-paste /include folder, see usage_example.cpp as usage example. CMakeLists.txt is here just to show that you should do pretty much nothing.

## Third Party code
Used date.h that is a foundation for C++20 chrono update from respective authors. 
