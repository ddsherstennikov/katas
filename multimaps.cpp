// multimaps.cpp
// handling geographic data with mulimaps
//=======================================

/*
input:_______________________________________________________
            PK        FK
river.txt : id, name, seaid 
sea.txt   : id, name, oceanid 
ocean.txt : id, name 

output:______________________________________________________
  ocean.name | 
  sea1.name, sea2.name, ..., seaN.name | 
  river1.name, river2.name, ... riverM.name
*/

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;


const string ocean_path = "ocean.txt";
const string sea_path = "sea.txt";
const string river_path = "river.txt";

const string delim = " | ";
const string sep = " ";

struct Ocean 
{ 
  int id; 
  string name;
  int owner_id;
  
  Ocean( string csv_line )
  {
    stringstream ss( csv_line );  
    string substr;
    
    getline( ss, substr, ',' );
    id = atoi( substr.c_str() );
    getline( ss, name, ',' );    
  }  
};
typedef multimap<int, Ocean>::const_iterator ocean_citer;

struct Sea
{ 
  int id; 
  string name;
  int owner_id;
  
  Sea( string csv_line )
  {
    stringstream ss( csv_line );  
    string substr;
    
    getline( ss, substr, ',' );
    id = atoi( substr.c_str() );
    getline( ss, name, ',' );
    getline( ss, substr, ',' );
    owner_id = atoi( substr.c_str() );
  }  
};
typedef multimap<int, Sea>::const_iterator sea_citer;
typedef pair<sea_citer,sea_citer> sea_pair;

struct River
{ 
  int id; 
  string name;
  int owner_id;
  
  River( string csv_line )
  {
    stringstream ss( csv_line );  
    string substr;
    
    getline( ss, substr, ',' );
    id = atoi( substr.c_str() );
    getline( ss, name, ',' );
    getline( ss, substr, ',' );
    owner_id = atoi( substr.c_str() );
  }  
};
typedef multimap<int, River>::const_iterator river_citer;
typedef pair<river_citer,river_citer> river_pair;


template<typename T>
  bool ProcessInput( string path, multimap<int, T>& dict )
{
  ifstream file;
  string s;
  file.open( path.c_str() );
  while ( getline( file, s ) )
  { 
    T elem( s );
    dict.insert( make_pair( elem.owner_id, elem ) ); // performance degradation for multimap in oceans case
  }
  file.close();  
}

void ProcessOutput( multimap<int, Ocean> oceans, multimap<int, Sea>   seas, multimap<int, River> rivers )
{
  stringstream ssout;
  for( ocean_citer oit = oceans.begin(); oit != oceans.end(); ++oit )
  {
    ssout << oit->second.name << delim;
    
    sea_pair inner_seas = seas.equal_range( oit->second.id );
    vector<string> river_names;
    for( sea_citer sit = inner_seas.first; sit != inner_seas.second; ++sit )
    {
      ssout << sit->second.name << sep;
      
      river_pair inner_rivers = rivers.equal_range( sit->second.id );
      for( river_citer rit = inner_rivers.first; rit != inner_rivers.second; ++rit )
        river_names.push_back( rit->second.name );
    }
    
    ssout << delim;
    
    for( vector<string>::const_iterator vit = river_names.begin(); vit != river_names.end(); ++vit )
      ssout << *vit << sep;
    
    ssout << endl;
    cout << ssout.str();
    
    ssout.str("");
  }
}

int main(int argc, char** argv) 
{
  multimap<int, Ocean> oceans;
  multimap<int, Sea>   seas;
  multimap<int, River> rivers;
  
  ProcessInput<Ocean>( ocean_path, oceans );
  ProcessInput<Sea>( sea_path, seas );
  ProcessInput<River>( river_path, rivers );
  
  ProcessOutput( oceans, seas, rivers );  
  
  return 0;
}