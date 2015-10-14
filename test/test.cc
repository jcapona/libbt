#include <bt/bt.h>
#include <iostream>


int main(int argc, char **argv)
{
 try
 {
  lib::bt con("66:35:56:78:90:AB");
  
  std::string msg = "ATI\r";
  std::string a;
  while(1)
  { 
   con.write(msg);
   con.read(a);
   std::cout << a << "\n";  
  }
 }
 catch(const std::exception &e)
 {
  std::cout << e.what() << " " << argv[1] << "\n";
  return -1;
 }

 return 0;
}

